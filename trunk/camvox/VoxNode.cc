/* VoxNode.cc - A single node in an octtree voxel space.
 * Copyright (C) 2008  Take Vos
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <camvox/VoxNode.h>

namespace camvox {

VoxNode::VoxNode(long data)
{
	mask = 0;
	for (int i = 0; i < 8; i++) {
		voxels[i].data = data;
	}
	//fprintf(stderr, "NEW at <%i %i %i, %i>\n", coord.v[0], coord.v[1], coord.v[2], coord.depth);
}

void VoxNode::deleteChildAtIndex(VoxTree &tree, int child_index, long data)
{
	VoxNode	*child = voxels[child_index].child;

	delete child;
	tree.nr_nodes_destroyed++;

	mask &= ~(1 << child_index);
	voxels[child_index].data = data;
}

long VoxNode::pruneVoxel(VoxTree &tree, const VoxCoord &coord, int child_index)
{
	if (mask & (1 << child_index)) {
		// There already exists a child object.
		VoxNode *child = voxels[child_index].child;

		// Walk the childtree.
		long data;
		VoxCoord child_coord = coord.childCoord(child_index);

		if ((data = child->prune(tree, child_coord)) == -1) {
			// We have to keep the childtree.
			return -1;

		} else {
			// We can get rid of the child tree.
			deleteChildAtIndex(tree, child_index, data);
		}
	}

	// Only a color is left.
	return voxels[child_index].data;
}

long VoxNode::prune(VoxTree &tree, const VoxCoord &coord)
{
	long	data = 0;
	bool	dont_prune = false;

	for (int child_index = 0; child_index < 8; child_index++) {
		long tmp_data = pruneVoxel(tree, coord, child_index);
		if (tmp_data == -1) {
			// There was a child tree.
			dont_prune = true;
		} else if (child_index == 0) {
			// First color.
			data = tmp_data;
		} else if (data != tmp_data) {
			// Different colors in child voxels.
			dont_prune = true;
		}
	}
	if (dont_prune) {
		return -1;
	} else {
		return data;
	}
}

long VoxNode::addCSGObjectToVoxel(VoxTree &tree, const VoxCoord &coord, int child_index, const CSGObject &obj, int layer)
{
	long		data;

	VoxCoord child_coord = coord.childCoord(child_index);

	if (mask & (1 << child_index)) {
		// There already exists a child object.
		VoxNode *child = voxels[child_index].child;

		// Walk the childtree.
		if ((data = child->addCSGObject(tree, child_coord, obj, layer)) == -1) {
			return -1;
		}

		// We can prune the child tree, seems all the data are equal in the child.
		deleteChildAtIndex(tree, child_index, data);

	} else {
		// There is just data at this level.
		IntervalVector child_voxel_bound = child_coord.boundingBox(tree.scale);
		switch(obj.boxType(child_voxel_bound)) {
		case BLACK_BOX:
			//fprintf(stderr, "black at <%i %i %i, %i>\n", child_coord.v[0], child_coord.v[1], child_coord.v[2], child_coord.depth);
			voxels[child_index].data|= (1 << layer);
			break;

		case WHITE_BOX:
			//fprintf(stderr, "white at <%i %i %i, %i>\n", child_coord.v[0], child_coord.v[1], child_coord.v[2], child_coord.depth);
			voxels[child_index].data&= ~(1 << layer);
			break;

		case GREY_BOX:
			//fprintf(stderr, "grey at <%i %i %i, %i>\n", child_coord.v[0], child_coord.v[1], child_coord.v[2], child_coord.depth);
			if (child_coord.depth > tree.max_depth) {
				// We are too deep, so we clear the voxel
				//fprintf(stderr, "max depth %i\n", tree.max_depth);
				voxels[child_index].data&= ~(1 << layer);

			} else {
				VoxNode *child = new VoxNode(voxels[child_index].data);
				voxels[child_index].child = child;
				mask|= (1 << child_index);
				tree.nr_nodes_created++;

				// Walk the child tree.
				if ((data = child->addCSGObject(tree, child_coord, obj, layer)) == -1) {
					return -1;
				}

				// We can prune the child tree, seems all the data are equal in the child.
				deleteChildAtIndex(tree, child_index, data);
			}
			break;
		}
	}
	return voxels[child_index].data;
}

long VoxNode::addCSGObject(VoxTree &tree, const VoxCoord &coord, const CSGObject &obj, int layer)
{
	long	data = 0;
	bool	dont_prune = false;

	for (int child_index = 0; child_index < 8; child_index++) {
		long tmp_data = addCSGObjectToVoxel(tree, coord, child_index, obj, layer);
		if (tmp_data == -1) {
			// There is a child tree.
			dont_prune = true;
		} else if (child_index == 0) {
			// First color.
			data = tmp_data;
		} else if (data != tmp_data) {
			// Different colors in child voxels.
			dont_prune = true;
		}
	}
	if (dont_prune) {
		return -1;
	} else {
		return data;
	}
}

void VoxNode::generatePOVCodeForVoxel(VoxTree &tree, const VoxCoord &coord, int child_index)
{
	VoxCoord	child_coord = coord.childCoord(child_index);

	if (mask & (1 << child_index)) {
		// There already exists a child object.
		VoxNode *child = voxels[child_index].child;

		// Walk the childtree.
		child->generatePOVCode(tree, child_coord);
	} else {
		long data = voxels[child_index].data;
		if (data & 1) {
			IntervalVector child_voxel_bound = child_coord.boundingBox(tree.scale);

			fprintf(stdout, "box {\n");

			// Our Y and Z axis is switched compared to almost everyone else.
			fprintf(stdout, "  <%lf %lf %lf>, <%lf %lf %lf>\n",
				child_voxel_bound.x.low,
				child_voxel_bound.z.low,
				child_voxel_bound.y.low,
				child_voxel_bound.x.high,
				child_voxel_bound.z.high,
				child_voxel_bound.y.high
			);

			//fprintf(stdout, "  texture {\n");
			//fprintf(stdout, "    pigment { color Blue }\n");
			//fprintf(stdout, "  }\n");
			fprintf(stdout, "}\n");
		}
	}
}

void VoxNode::generatePOVCode(VoxTree &tree, const VoxCoord &coord)
{
	for (int child_index = 0; child_index < 8; child_index++) {
		generatePOVCodeForVoxel(tree, coord, child_index);
	}
}

}
