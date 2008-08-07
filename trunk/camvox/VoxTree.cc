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

inline vox_node_t *VoxTree::nr2ptr(uint32_t node_nr) const
{
	return nodes + node_nr;
}

inline uint32_t VoxTree::ptr2nr(const vox_node_t *node) const
{
	return node - nodes;
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

VoxTree::VoxTree(void) : free_list(0x7ffffffe), nr_nodes_created(0), nr_nodes_destroyed(0)
{
	// Start with a lot of voxels.
	nodes_size = 0x100000;
	nodes = new vox_node_t [nodes_size];

	// Start with a root node, make it clear.
	root = alloc();
	vox_node_t *node = &nodes[root];
	for (int i = 0; i < 8; i++) {
		node->voxels[i] = 0;
	}

	max_depth = 3;
	scale = 1.0;
}

void VoxTree::checkSize(void)
{
	uint32_t max_node_nr = free_list.maxItemNr();

	if (__builtin_expect(max_node_nr > (uint32_t)(nodes_size - 1), 0)) {
		// Calculate a new size and create the new nodes.
		uint32_t new_nodes_size = max_node_nr + max_node_nr / 2;
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

voxel_t VoxTree::pruneVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index)
{
	VoxCoord voxel_coord = coord.childCoord(voxel_index);
	voxel_t voxel_data = nodes[node_nr].voxels[voxel_index];

	if (voxIsNodeNr(voxel_data)) {
		// There already exists a child object. Walk the childtree.
		uint32_t voxel_node_nr = voxGetNodeNr(voxel_data);
		voxel_t tmp_data = pruneNode(voxel_node_nr, voxel_coord);

		if (voxIsDontPrune(tmp_data)) {
			// The data is different in the child node, so dont prune.
			return voxSetDontPrune();
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

voxel_t VoxTree::pruneNode(uint32_t node_nr, const VoxCoord &coord)
{
	uint32_t	data = 0;
	bool		dont_prune = false;

	for (int voxel_index = 0; voxel_index < 8; voxel_index++) {
		voxel_t tmp_data = pruneVoxel(node_nr, coord, voxel_index);
		if (voxIsDontPrune(tmp_data)) {
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
		return voxSetDontPrune();
	} else {
		return data;
	}
}

void VoxTree::prune(void)
{
	pruneNode(root, VoxCoord());
}

voxel_t VoxTree::addCSGObjectToVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index, const CSGObject *obj, voxel_t *new_data)
{
	VoxCoord	voxel_coord = coord.childCoord(voxel_index);
	voxel_t		voxel_data = nodes[node_nr].voxels[voxel_index];
	IntervalVector	voxel_bound = voxel_coord.boundingBox(scale);
	const CSGObject	*bt = obj->boxType(voxel_bound);


	if (!voxIsNodeNr(voxel_data)) {
		// There is just data at this level.
		if (bt == BLACK_BOX) {
			// Do the operation on the voxel.
			switch (voxGetOperation(*new_data)) {
			case VOX_OP_OR:
				voxel_data |= voxGetLayers(*new_data);
				break;
			case VOX_OP_AND:
				voxel_data &= voxGetLayers(*new_data);
				break;
			case VOX_OP_XOR:
				voxel_data ^= voxGetLayers(*new_data);
				break;
			case VOX_OP_TST:
				*new_data |= voxGetLayers(voxel_data);
				break;
			}

		} else if (bt == WHITE_BOX) {
			// We don't do anything to the voxel.

		} else {
			//fprintf(stderr, "depth %i\n", voxel_coord.depth);
			if (voxel_coord.depth > max_depth) {
				// We are too deep, so we don't do anything to the voxel.

			} else {
				// We create a subnode
				uint32_t child_node_nr = alloc();

				// Set all its voxel to the current voxel data.
				vox_node_t *child_node = &nodes[child_node_nr];
				for (int i = 0; i < 8; i++) {
					child_node->voxels[i] = voxel_data;
				}

				voxel_data = voxSetNodeNr(child_node_nr);
			}
		}

		// Record the new value.
		nodes[node_nr].voxels[voxel_index] = voxel_data;
	}

	if (voxIsNodeNr(voxel_data)) {
		// There already was a child node, or the previous part created this child node.
		// Walk the tree. We will use the simplyfied CSGTree.
		// The CSGTree, could even be a BLACK_BOX or WHITE_BOX, which is really fast to test for.
		uint32_t voxel_node_nr = voxGetNodeNr(voxel_data);
		voxel_t tmp_data = addCSGObjectToNode(voxel_node_nr, voxel_coord, bt, new_data);

		if (voxIsDontPrune(tmp_data)) {
			// The data is unequal, so we can not prune the child probject
			return voxSetDontPrune();
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

voxel_t VoxTree::addCSGObjectToNode(uint32_t node_nr, const VoxCoord &coord, const CSGObject *obj, voxel_t *new_data)
{
	uint32_t	data = 0;
	bool		dont_prune = false;

	for (int voxel_index = 0; voxel_index < 8; voxel_index++) {
		voxel_t tmp_data = addCSGObjectToVoxel(node_nr, coord, voxel_index, obj, new_data);
		if (voxIsDontPrune(tmp_data)) {
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
		return voxSetDontPrune();
	} else {
		return data;
	}
}

void VoxTree::addCSGObject(const CSGObject *obj, voxel_t *new_data)
{
	assert(new_data);
	assert(!voxIsNodeNr(*new_data));
	assert(obj);

	int orig_round = fegetround();

	// The interval arithmatic used in these calls like the FPU to
	// round everything down.
	fesetround(FE_DOWNWARD);

	addCSGObjectToNode(root, VoxCoord(), obj, new_data);

	fesetround(orig_round);
}

void VoxTree::addCSGObjectOR(const CSGObject *obj, uint32_t layers)
{
	voxel_t voxel_data = voxSetLayersAndOperation(layers, VOX_OP_OR);
	addCSGObject(obj, &voxel_data);
}

void VoxTree::addCSGObjectAND(const CSGObject *obj, uint32_t layers)
{
	voxel_t voxel_data = voxSetLayersAndOperation(layers, VOX_OP_AND);
	addCSGObject(obj, &voxel_data);
}

void VoxTree::addCSGObjectXOR(const CSGObject *obj, uint32_t layers)
{
	voxel_t voxel_data = voxSetLayersAndOperation(layers, VOX_OP_XOR);
	addCSGObject(obj, &voxel_data);
}

uint32_t VoxTree::addCSGObjectTST(const CSGObject *obj)
{
	voxel_t voxel_data = voxSetLayersAndOperation(0, VOX_OP_TST);
	addCSGObject(obj, &voxel_data);
	return voxel_data;
}

void VoxTree::generatePOVCodeForVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index)
{
	VoxCoord	voxel_coord = coord.childCoord(voxel_index);
	uint32_t 	voxel_data = nodes[node_nr].voxels[voxel_index];

	if (voxIsNodeNr(voxel_data)) {
		// There already exists a child object. Walk the childtree.
		generatePOVCodeForNode(voxGetNodeNr(voxel_data), voxel_coord);

	} else {
		if (voxel_data & 1) {
			IntervalVector voxel_bound = voxel_coord.boundingBox(scale);

			// Our Y and Z axis is switched compared to almost everyone else.
			fprintf(stdout, "box {\n");
			fprintf(stdout, "  <%lf %lf %lf>, <%lf %lf %lf>\n",
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
	fprintf(stdout, "global_settings { ambient_light <0.4, 0.4, 0.4> }\n");
	fprintf(stdout, "camera {\n");
	fprintf(stdout, "  orthographic\n");
	fprintf(stdout, "  location <0.0, 8.0, -10.0>\n");
	fprintf(stdout, "  look_at <5.0, 4.0, 0.0>\n");
	fprintf(stdout, "}\n");
	fprintf(stdout, "light_source { <-5, 10.0, -10.0> color <2.0, 2.0, 2.0> }\n");


	fprintf(stdout, "cylinder {\n");
	fprintf(stdout, " <0, 0, 0>, <10.0, 0, 0>, 0.1\n");
	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Red }\n");
	fprintf(stdout, "  }\n");
	fprintf(stdout, "}\n");

	fprintf(stdout, "cylinder {\n");
	fprintf(stdout, " <0, 0, 0>, <0.0, 0.0, 10.0>, 0.1\n");
	fprintf(stdout, "  texture {\n");
	fprintf(stdout, "    pigment { color Green }\n");
	fprintf(stdout, "  }\n");
	fprintf(stdout, "}\n");

	fprintf(stdout, "cylinder {\n");
	fprintf(stdout, " <0, 0, 0>, <0.0, 10.0, 0.0>, 0.1\n");
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
