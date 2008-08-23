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

}
