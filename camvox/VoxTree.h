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

#include <camvox/Voxel.h>
#include <camvox/VoxOperation.h>
#include <camvox/VoxCoord.h>
#include <camvox/CSGObject.h>
#include <camvox/FreeList.h>

namespace camvox {

typedef struct {
	Voxel	voxels[8];
} vox_node_t;

/** An VoxTree instance holds a complete oct-tree of voxels.
 * The VoxTree includes the memory menagement of all of the nodes in the octtree.
 */
class VoxTree {
private:
	FreeList	free_list;
	uint32_t	nodes_size;
	vox_node_t	*nodes;
	uint32_t	root;
	double		size;		///< The length of each axis of voxel space.
	double		scale;		///< The size of the smallest possible voxel.

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
	Voxel pruneVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index);

	/** Optimizes the voxel tree.
	 * @see void pruneVoxel(void)
	 */
	Voxel pruneNode(uint32_t node_nr, const VoxCoord &coord);

	/** Adds an CSG object to a voxel.
	 * @see void addCSGObject(CSGObject *obj, VoxOperation *new_data)
	 */
	Voxel addCSGObjectToVoxel(uint32_t node_nr, const VoxCoord &coord, int voxel_index, const CSGObject *obj, VoxOperation &csg_op);

	/** Adds an CSG object to a voxel.
	 * @see void addCSGObject(CSGObject *obj, VoxOperation *new_data)
	 */
	Voxel addCSGObjectToNode(uint32_t node_nr, const VoxCoord &coord, const CSGObject *obj, VoxOperation &csg_op);

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

	/** Construct a voxel space.
	 * @param _size		size of each axis of the voxel space in mm, inches or anything else.
	 *			but make sure that you keep units consistant troughout.
	 */
	VoxTree(double _size);
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
	void addCSGObject(const CSGObject *obj, VoxOperation &csg_op);

	/** Reduce the memory footprint of the voxel space.
	 * addCSGObject* alreay automatically prune, so this function
	 * rarelly needs to be called.
	 */
	void prune(void);
};

}

#endif
