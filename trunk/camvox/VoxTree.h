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

/*
 * A voxel value is overloaded quite a bit, so that as little memory as possible can be used.
 * 
 * 1nnnnnnn nnnnnnnn nnnnnnnn nnnnnnnn	   n = node number
 * 0oo      WWWWWWWW NNNNNNNN llllllll     l = layers
 *                                         W = longitude of the normal on the xy-plane, x-axis is 0.
 *                                         N = latitude of the normal above the xy-plane, xy-plane is 0.
 *                                         o = layer operation (not stored in the voxel).
 *                                             0 OR       Add object to layers
 *                                             1 AND      Remove object from layers
 *                                             2 XOR      Toggle object from layers
 *                                             3 TST      return layers that intersect with object
 */


typedef uint32_t voxel_t;

typedef enum {
	VOX_OP_OR = 0,
	VOX_OP_AND,
	VOX_OP_XOR,
	VOX_OP_TST
} vox_op_t;

typedef struct {
	voxel_t	voxels[8];
} vox_node_t;

static inline bool voxIsNodeNr(voxel_t data)
{
	return (data & 0x80000000) == 0x80000000;
}

static inline uint32_t voxGetNodeNr(voxel_t data)
{
	return data & 0x7fffffff;
}

static inline voxel_t voxSetNodeNr(uint32_t node_nr)
{
	return node_nr | 0x80000000;
}

static inline voxel_t voxSetDontPrune(void)
{
	return 0xffffffff;
}

static inline bool voxIsDontPrune(voxel_t data)
{
	return data == 0xffffffff;
}

static inline vox_op_t voxGetOperation(voxel_t data)
{
	return (vox_op_t)(data >> 29);
}

static inline uint32_t voxGetLayers(voxel_t data)
{
	return data & 0xff;
}

static inline voxel_t voxSetLayersAndOperation(uint32_t mask, vox_op_t op)
{
	return (op << 29) | mask;
}

class VoxTree {
protected:
	voxel_t pruneVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index);
	voxel_t pruneNode(uint32_t node_nr, const VoxCoord &coord);
	voxel_t addCSGObjectToVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index, const CSGObject *obj, voxel_t *new_data);
	voxel_t addCSGObjectToNode(uint32_t node_nr, const VoxCoord &coord, const CSGObject *obj, voxel_t *new_data);
	void generatePOVCodeForVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index);
	void generatePOVCodeForNode(uint32_t node_nr, const VoxCoord &coord);
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
	void addCSGObject(const CSGObject *obj, voxel_t *new_data);
	void addCSGObjectOR(const CSGObject *obj, uint32_t layers);
	void addCSGObjectAND(const CSGObject *obj, uint32_t layers);
	void addCSGObjectXOR(const CSGObject *obj, uint32_t layers);
	uint32_t addCSGObjectTST(const CSGObject *obj);
	void prune(void);
};

}
#endif
