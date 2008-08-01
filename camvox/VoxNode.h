/* VoxNode.h - A single node in an octtree voxel space.
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
#ifndef VOXNODE_H
#define VOXNODE_H

#include <camvox/VoxCoord.h>
#include <camvox/VoxTree.h>
#include <camvox/CSGObject.h>

namespace camvox {

class VoxNode;

typedef union {
	long	data;
	VoxNode	*child;
} voxel_t;

class VoxTree;

class VoxNode {
private:
	void deleteChildAtIndex(VoxTree &tree, int child_index, long data);
	long pruneVoxel(VoxTree &tree, const VoxCoord &coord, int child_index);
	long addCSGObjectToVoxel(VoxTree &tree, const VoxCoord &coord, int child_index, const CSGObject &obj, int layer);
	void generatePOVCodeForVoxel(VoxTree &tree, const VoxCoord &coord, int child_index);

public:
	uint8_t		mask;
	voxel_t		voxels[8];

	VoxNode(long data);
	long prune(VoxTree &tree, const VoxCoord &coord);
	long addCSGObject(VoxTree &tree, const VoxCoord &coord, const CSGObject &obj, int layer);
	void generatePOVCode(VoxTree &tree, const VoxCoord &coord);
};

}
#endif
