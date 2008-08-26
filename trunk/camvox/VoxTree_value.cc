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
#include <math.h>
#include <fenv.h>
#include <camvox/VoxTree.h>

namespace camvox {

uint8_t VoxTree::getVoxelLayers(uint32_t node_nr, const VoxCoord &coord, const VoxCoord &end_coord, int voxel_index) const
{
	VoxCoord voxel_coord = coord.childCoord(voxel_index);
	Voxel voxel_data = nodes[node_nr].voxels[voxel_index];

	if (voxel_data.isNodeNr()) {
		uint32_t voxel_node_nr = voxel_data.getNodeNr();
		return getNodeLayers(voxel_node_nr, voxel_coord, end_coord);

	} else if (voxel_data.isLayers()) {
		return voxel_data.getLayers();
	} else {
		return 0;
	}
}

uint8_t VoxTree::getNodeLayers(uint32_t node_nr, const VoxCoord &coord, const VoxCoord &end_coord) const
{
	if (coord.depth < end_coord.depth) {
		// We still know which route to take.
		int voxel_index = end_coord.indexAtDepth(coord.depth + 1);
		//fprintf(stderr, "index %i, depth %i\n", voxel_index, coord.depth + 1);
		return getVoxelLayers(node_nr, coord, end_coord, voxel_index);

	} else {
		// We need to go deeper to find the layers.
		uint8_t data = 0;

		for (int voxel_index = 0; voxel_index < 8; voxel_index++) {
			data|= getVoxelLayers(node_nr, coord, end_coord, voxel_index);
		}
		return data;
	}
}

uint8_t VoxTree::getLayers(const VoxCoord &coord) const
{
	return getNodeLayers(root, VoxCoord(), coord);
}

void VoxTree::getSkin(uint8_t layers, double resolution, std::vector<VoxCoord> &coords) const
{
	int depth = ceil(log2(size / resolution));
	int length = 1 << depth;

	for (int z = 1; z < length - 1; z++) {
	for (int y = 1; y < length - 1; y++) {
	for (int x = 1; x < length - 1; x++) {
		VoxCoord coord = VoxCoord(depth, x, y, z);

		if (getLayers(coord) == layers) {
			for (int z2 = z - 1; z2 < z + 1; z2++) {
			for (int y2 = y - 1; y2 < y + 1; y2++) {
			for (int x2 = x - 1; x2 < x + 1; x2++) {
				VoxCoord neighbour = VoxCoord(depth, x2, y2, z2);

				if (getLayers(neighbour) == 0) {
					fprintf(stderr, "(%i %i %i) %i| %x %x %x\n", x, y, z, coord.depth, coord.x, coord.y, coord.z);
					coords.push_back(coord);
					goto next;
				}

			} } }
		}
next:;
	} } }
}

}

