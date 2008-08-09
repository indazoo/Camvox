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

typedef uint32_t voxel_t;

/** Voxel operations.
 * These are the operations that can be made on the voxel volume.
 */
typedef enum {
	VOX_OP_OR = 0,	///< binary OR layer masks when inside CSG object.
	VOX_OP_AND,	///< binary AND layer masks when inside CSG object.
	VOX_OP_XOR,	///< binary XOR layer masks when inside CSG object.
	VOX_OP_TST	///< return layers that are inside CSG object.
} vox_op_t;

/** An voxel oct-tree node.
 * Contains all 8 child voxels two for each dimension.
 */
typedef struct {
	/** A value of a voxel.
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

/** An VoxTree instance holds a complete oct-tree of voxels.
 * The VoxTree includes the memory menagement of all of the nodes in the octtree.
 */
class VoxTree {
private:
	FreeList	free_list;
	int		nodes_size;
	vox_node_t	*nodes;
	uint32_t	root;

	/** Alocate a new oct-tree node.
	 * This takes a node from the free list, this is garanteed to be
	 * the lowest value available.
	 *
	 * @returns node_nr
	 */
	uint32_t alloc(void);

	/** Frees an oct-tree node.
	 * This returns the node back to the free list.
	 *
	 * @param node_nr The node_nr of the node to be freed.
	 */
	void free(uint32_t node_nr);

	/** Checks and resizes the available memory for holding the nodes.
	 * This should be run everytime a new node gets allocated. It increases
	 * the size of the buffer by 50%. Because the nodes may be moved in memory
	 * after this call, one should always use node_nrs when accesing nodes.
	 */
	void checkSize(void);

	/** Optimizes the voxel tree.
	 * @see void pruneVoxel(void)
	 */
	voxel_t pruneVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index);

	/** Optimizes the voxel tree.
	 * @see void pruneVoxel(void)
	 */
	voxel_t pruneNode(uint32_t node_nr, const VoxCoord &coord);

	/** Adds an CSG object to a voxel.
	 * @see void addCSGObject(CSGObject *obj, voxel_t *new_data)
	 */
	voxel_t addCSGObjectToVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index, const CSGObject *obj, voxel_t *new_data);

	/** Adds an CSG object to a voxel.
	 * @see void addCSGObject(CSGObject *obj, voxel_t *new_data)
	 */
	voxel_t addCSGObjectToNode(uint32_t node_nr, const VoxCoord &coord, const CSGObject *obj, voxel_t *new_data);

	/** Generate POVRAY code.
	 * @see void generatePOVCode(void)
	 */
	void generatePOVCodeForVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index);

	/** Generate POVRAY code.
	 * @see void generatePOVCode(void)
	 */
	void generatePOVCodeForNode(uint32_t node_nr, const VoxCoord &coord);

public:
	uint64_t	nr_nodes_created;	///< A counter showing the number of nodes created.
	uint64_t	nr_nodes_destroyed;	///< A counter showing the number of nodes removed.
	int		max_depth;		///< The maximum depth at which the reevaluation of a CSG object stops.
	double		scale;			///< The size of the smallest possible voxel.

	VoxTree(void);
	~VoxTree(void);

	/** Generate POV-RAY code.
	 * This method outputs a POV-RAY file representing the oct-tree.
	 * It writes out a set of boxes in different colors, each one node of the oct-tree.
	 */
	void generatePOVCode(void);

	/** Render a CSG object into voxel space.
	 * @param obj An CSGObject, either a primative or a compound object.
	 * @param new_data The new data and the command to be executed when
	 *        a voxel is inside the CSG Object.
	 */
	void addCSGObject(const CSGObject *obj, voxel_t *new_data);

	/** Render an CSG object into voxel space.
	 * @see void addCSGObject(const CSGObject *obj, voxel_t *new_data)
	 */
	void addCSGObjectOR(const CSGObject *obj, uint32_t layers);

	/** Render an CSG object into voxel space.
	 * @see void addCSGObject(const CSGObject *obj, voxel_t *new_data)
	 */
	void addCSGObjectAND(const CSGObject *obj, uint32_t layers);

	/** Render an CSG object into voxel space.
	 * @see void addCSGObject(const CSGObject *obj, voxel_t *new_data)
	 */
	void addCSGObjectXOR(const CSGObject *obj, uint32_t layers);

	/** Make a collision test between CSG object and voxel space.
	 * @see void addCSGObject(const CSGObject *obj, voxel_t *new_data)
	 */
	uint32_t addCSGObjectTST(const CSGObject *obj);

	/** Reduce the memory footprint of the voxel space.
	 * addCSGObject* alreay automatically prune, so this function
	 * rarelly needs to be called.
	 */
	void prune(void);
};

}
#endif
