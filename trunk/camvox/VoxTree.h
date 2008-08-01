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
#include <camvox/VoxNode.h>
#include <camvox/CSGObject.h>

namespace camvox {

class VoxNode;

class VoxTree {
public:
	int	max_depth;
	int	nr_nodes_created;
	int	nr_nodes_destroyed;
	double	scale;
	VoxNode	*root;

	VoxTree(void);
	void generatePOVCode(void);
	long addCSGObject(const CSGObject &obj, int layer);
	long prune(void);
};

}
#endif
