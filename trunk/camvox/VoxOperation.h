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
#ifndef VOXOPERATION_H
#define VOXOPERATION_H

#include <camvox/Voxel.h>

namespace camvox {

/** Voxel operations.
 * These are the operations that can be made on the voxel volume.
 */
typedef enum {
	VOX_OP_OR = 0,	///< binary OR layer masks when inside CSG object.
	VOX_OP_AND,	///< binary AND layer masks when inside CSG object.
	VOX_OP_XOR,	///< binary XOR layer masks when inside CSG object.
	VOX_OP_TST,	///< return layers that are inside CSG object.
	VOX_OP_NOP	///< Don't do anything.
} vox_op_t;

class VoxOperation {
public:
	Voxel		inside_mask;
	Voxel		outside_mask;
	Voxel		edge_mask;
	vox_op_t	inside_op;
	vox_op_t	outside_op;
	vox_op_t	edge_op;
	bool		set_edge_obj;
	double		collision_size[32];
};

}

#endif
