/* VoxTree.cc - Management of all octtree voxel nodes.
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fenv.h>
#include <camvox/VoxTree.h>

namespace camvox {

VoxTree::VoxTree(double _size) :
	free_list(0x7ffffffe),
	size(_size),
	scale(_size / 2147483648.0),
	nr_nodes_created(0),
	nr_nodes_destroyed(0)
{
	// Start with a lot of voxels.
	nodes_size = 0x100000;
	nodes = new vox_node_t [nodes_size];

	// Start with a root node, make it clear.
	root = alloc();
	vox_node_t *node = &nodes[root];
	for (int i = 0; i < 8; i++) {
		node->voxels[i].setLayers(0);
	}
}

VoxTree::~VoxTree(void)
{
	delete nodes;
}

inline void VoxTree::free(uint32_t node_nr)
{
	free_list.free(node_nr);
	nr_nodes_destroyed++;
}

inline uint32_t VoxTree::alloc(void)
{
	uint32_t ret = free_list.alloc();
	checkSize();
	nr_nodes_created++;
	return ret;
}

void VoxTree::checkSize(void)
{
	uint32_t array_size = free_list.arraySize();

	if (__builtin_expect(array_size > nodes_size, 0)) {
		// Calculate a new size and create the new nodes.
		uint32_t new_nodes_size = array_size + array_size / 2;
		vox_node_t *new_nodes = new vox_node_t [new_nodes_size];

		// Copy the old voxels. The free list should have these
		// pretty much packed already.
		memcpy(new_nodes, nodes, nodes_size * sizeof(vox_node_t));

		// Delete the old nodes, replace with the new.
		delete nodes;
		nodes = new_nodes;
		nodes_size = new_nodes_size;
	}
}


Voxel VoxTree::pruneVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index)
{
	VoxCoord voxel_coord = coord.childCoord(voxel_index);
	Voxel voxel_data = nodes[node_nr].voxels[voxel_index];

	if (voxel_data.isNodeNr()) {
		// There already exists a child object. Walk the childtree.
		uint32_t voxel_node_nr = voxel_data.getNodeNr();
		Voxel tmp_data = pruneNode(voxel_node_nr, voxel_coord);

		if (tmp_data.isDontPrune()) {
			// The data is different in the child node, so dont prune.
			return Voxel();
		} else {
			// All the data are equal so delete the child node.
			free(voxel_node_nr);

			// Set the current voxel to the color of the child node.
			nodes[node_nr].voxels[voxel_index] = tmp_data;
			return tmp_data;
		}
	} else {
		// Maybe we can be pruned.
		return voxel_data;
	}
}

Voxel VoxTree::pruneNode(uint32_t node_nr, const VoxCoord &coord)
{
	Voxel		data = Voxel();
	bool		dont_prune = false;

	for (int voxel_index = 0; voxel_index < 8; voxel_index++) {
		Voxel tmp_data = pruneVoxel(node_nr, coord, voxel_index);
		if (tmp_data.isDontPrune()) {
			// There was a child tree.
			dont_prune = true;
		} else if (voxel_index == 0) {
			// First color.
			data = tmp_data;
		} else if (data != tmp_data) {
			// Different colors in child voxels.
			dont_prune = true;
		}
	}
	if (dont_prune) {
		return Voxel();
	} else {
		return data;
	}
}

void VoxTree::prune(void)
{
	pruneNode(root, VoxCoord());
}

Voxel VoxTree::addCSGObjectToVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index, const CSGObject *obj, VoxOperation &csg_op)
{
	VoxCoord	voxel_coord = coord.childCoord(voxel_index);
	Voxel		voxel_data = nodes[node_nr].voxels[voxel_index];
	IntervalVector	voxel_bound = voxel_coord.boundingBox(scale);
	const CSGObject	*bt = obj->boxType(voxel_bound);

	if (voxel_data.isLayers()) {
		// There is just data at this level.
		if (bt == BLACK_BOX) {
			// Do the operation on the voxel.
			switch (csg_op.inside_op) {
			case VOX_OP_OR:
				voxel_data.orLayers(csg_op.inside_mask);
				break;
			case VOX_OP_AND:
				voxel_data.andLayers(csg_op.inside_mask);
				break;
			case VOX_OP_XOR:
				voxel_data.xorLayers(csg_op.inside_mask);
				break;
			case VOX_OP_NOP:
			case VOX_OP_TST:
				break;
			}

		} else if (bt == WHITE_BOX) {
			// We don't do anything to the voxel.
			switch (csg_op.outside_op) {
			case VOX_OP_OR:
				voxel_data.orLayers(csg_op.outside_mask);
				break;
			case VOX_OP_AND:
				voxel_data.andLayers(csg_op.outside_mask);
				break;
			case VOX_OP_XOR:
				voxel_data.xorLayers(csg_op.outside_mask);
				break;
			case VOX_OP_NOP:
			case VOX_OP_TST:
				break;
			}

		} else {
			//fprintf(stderr, "depth %i\n", voxel_coord.depth);
			if (voxel_coord.size(scale) < obj->total_resolution) {
				// We are too deep, so we don't do anything to the voxel.
				switch (csg_op.edge_op) {
				case VOX_OP_OR:
					voxel_data.orLayers(csg_op.edge_mask);
					break;
				case VOX_OP_AND:
					voxel_data.andLayers(csg_op.edge_mask);
					break;
				case VOX_OP_XOR:
					voxel_data.xorLayers(csg_op.edge_mask);
					break;
					break;
				case VOX_OP_NOP:
				case VOX_OP_TST:
					break;
				}
				if (csg_op.set_edge_obj) {
					voxel_data.setCSGObject(obj);
				}

			} else {
				// We create a subnode
				uint32_t child_node_nr = alloc();

				// Set all its voxel to the current voxel data.
				vox_node_t *child_node = &nodes[child_node_nr];
				for (int i = 0; i < 8; i++) {
					child_node->voxels[i] = voxel_data;
				}

				voxel_data.setNodeNr(child_node_nr);
			}
		}

		// Record the new value.
		nodes[node_nr].voxels[voxel_index] = voxel_data;
	}

	if (voxel_data.isNodeNr()) {
		// There already was a child node, or the previous part created this child node.
		// Walk the tree. We will use the simplyfied CSGTree.
		// The CSGTree, could even be a BLACK_BOX or WHITE_BOX, which is really fast to test for.
		uint32_t voxel_node_nr = voxel_data.getNodeNr();
		Voxel tmp_data = addCSGObjectToNode(voxel_node_nr, voxel_coord, bt, csg_op);

		if (tmp_data.isDontPrune()) {
			// The data is unequal, so we can not prune the child probject
			return Voxel();
		} else {
			// Data is all the same in the child node, remove the unneeded child.
			free(voxel_node_nr);

			// Record the data from the child node.
			nodes[node_nr].voxels[voxel_index] = tmp_data;
			return tmp_data;
		}
	} else {
		return voxel_data;
	}
}

Voxel VoxTree::addCSGObjectToNode(uint32_t node_nr, const VoxCoord &coord, const CSGObject *obj, VoxOperation &csg_op)
{
	Voxel	data = 0;
	bool	dont_prune = false;

	for (int voxel_index = 0; voxel_index < 8; voxel_index++) {
		Voxel tmp_data = addCSGObjectToVoxel(node_nr, coord, voxel_index, obj, csg_op);
		if (tmp_data.isDontPrune()) {
			// There is a child tree.
			dont_prune = true;
		} else if (voxel_index == 0) {
			// First color.
			data = tmp_data;
		} else if (data != tmp_data) {
			// Different colors in child voxels.
			dont_prune = true;
		}
	}
	if (dont_prune) {
		return Voxel();
	} else {
		return data;
	}
}

void VoxTree::addCSGObject(const CSGObject *obj, VoxOperation &csg_op)
{
	int orig_round = fegetround();

	// The interval arithmatic used in these calls like the FPU to
	// round everything down.
	fesetround(FE_DOWNWARD);

	addCSGObjectToNode(root, VoxCoord(), obj, csg_op);

	fesetround(orig_round);
}

void VoxTree::generatePOVCodeForVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index)
{
	VoxCoord	voxel_coord = coord.childCoord(voxel_index);
	Voxel	 	voxel_data = nodes[node_nr].voxels[voxel_index];

	if (voxel_data.isNodeNr()) {
		// There already exists a child object. Walk the childtree.
		generatePOVCodeForNode(voxel_data.getNodeNr(), voxel_coord);

	} else {
		if (voxel_data.getLayers() & 1) {
			IntervalVector voxel_bound = voxel_coord.boundingBox(scale);

			// Our Y and Z axis is switched compared to almost everyone else.
			fprintf(stdout, "box {\n");
			fprintf(stdout, "  <%lf, %lf, %lf>, <%lf, %lf, %lf>\n",
				voxel_bound.x.low,
				voxel_bound.z.low,
				voxel_bound.y.low,
				voxel_bound.x.high,
				voxel_bound.z.high,
				voxel_bound.y.high
			);
			fprintf(stdout, "}\n");
		}
	}
}

void VoxTree::generatePOVCodeForNode(uint32_t node_nr, const VoxCoord &coord)
{
	for (int voxel_index = 0; voxel_index < 8; voxel_index++) {
		generatePOVCodeForVoxel(node_nr, coord, voxel_index);
	}
}

void VoxTree::generatePOVCode(void)
{
	fprintf(stdout, "#include \"colors.inc\"\n\n");
	fprintf(stdout, "#include \"glass.inc\"\n\n");

	fprintf(stdout, "background { color Black }\n");
	fprintf(stdout, "global_settings { ambient_light <1.2, 1.2, 1.2> }\n");
	fprintf(stdout, "camera {\n");
	fprintf(stdout, "  orthographic\n");
	fprintf(stdout, "  location <%lf, %lf, %lf>\n", -size * 0.4, size * 0.4, -size * 0.8);
	fprintf(stdout, "  look_at <0.0, 0.0, 0.0>\n");
	fprintf(stdout, "}\n");
	fprintf(stdout, "light_source { <%lf, %lf, %lf> color <%lf, %lf, %lf> }\n",
		-size * 0.5, size * 0.4, -size,
		2.0, 2.0, 2.0
	);


	fprintf(stdout, "cylinder {\n");
	fprintf(stdout, " <0, 0, 0>, <%lf, 0, 0>, 0.1\n", size);
	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Red }\n");
	fprintf(stdout, "  }\n");
	fprintf(stdout, "}\n");

	fprintf(stdout, "cylinder {\n");
	fprintf(stdout, " <0, 0, 0>, <0.0, 0.0, %lf>, 0.1\n", size);
	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Green }\n");
	fprintf(stdout, "  }\n");
	fprintf(stdout, "}\n");

	fprintf(stdout, "cylinder {\n");
	fprintf(stdout, " <0, 0, 0>, <0.0, %lf, 0.0>, 0.1\n", size);
	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Blue }\n");
	fprintf(stdout, "  }\n");
	fprintf(stdout, "}\n");


	fprintf(stdout, "union {\n");

	generatePOVCodeForNode(root, VoxCoord());

	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Green }\n");
	fprintf(stdout, "  }\n");
	//fprintf(stdout, "  interior {I_Glass_Dispersion1}\n");
	fprintf(stdout, "}\n");
}

}
