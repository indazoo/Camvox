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

}

