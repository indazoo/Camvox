/* VoxTree.h - Management of all octtree voxel nodes.
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
#ifndef VOXTREE_H
#define VOXTREE_H

#include <camvox/VoxCoord.h>
#include <camvox/CSGObject.h>
#include <camvox/FreeList.h>

namespace camvox {

typedef struct {
	uint32_t	voxels[8];
} vox_node_t;

const uint32_t DONT_PRUNE = 0xfffffff;
const uint32_t NODE_MASK = 0x7ffffff;
const uint32_t NODE_FLAG = 0x8000000;

class VoxTree {
protected:
	uint32_t pruneVoxel(vox_node_t *node, const VoxCoord &coord, int voxel_index);
	uint32_t pruneNode(vox_node_t *node, const VoxCoord &coord);
	uint32_t addCSGObjectToVoxel(vox_node_t *node, const VoxCoord &coord, int voxel_index, const CSGObject &obj, int layer);
	uint32_t addCSGObjectToNode(vox_node_t *node, const VoxCoord &coord, const CSGObject &obj, int layer);
	void generatePOVCodeForVoxel(vox_node_t *node, const VoxCoord &coord, int voxel_index);
	void generatePOVCodeForNode(vox_node_t *node, const VoxCoord &coord);
public:
	FreeList	free_list;
	int		nodes_size;
	vox_node_t	*nodes;
	uint64_t	nr_nodes_created;
	uint64_t	nr_nodes_destroyed;

	uint32_t	root;

	int		max_depth;
	double		scale;

	vox_node_t *nr2ptr(uint32_t node_nr) const;
	uint32_t ptr2nr(const vox_node_t *node) const;
	uint32_t alloc(void);
	void free(uint32_t node_nr);
	void checkSize(void);

	VoxTree(void);
	void generatePOVCode(void);
	long addCSGObject(const CSGObject &obj, int layer);
	long prune(void);
};

}
#endif
