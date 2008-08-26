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
		node->voxels[i] = Voxel(0);
	}
}

VoxTree::~VoxTree(void)
{
	delete nodes;
}

void VoxTree::free(uint32_t node_nr)
{
	free_list.free(node_nr);
	nr_nodes_destroyed++;
}

uint32_t VoxTree::alloc(void)
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

}

