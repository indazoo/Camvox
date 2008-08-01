/* camvox.cc - Render objects into voxels.
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
#include <camvox/Vector.h>
#include <camvox/CSGSphere.h>
#include <camvox/VoxNode.h>

int main(int argc, char *argv[])
{
	// Initialization of constants
	camvox::VoxTree		tree = camvox::VoxTree();
	tree.max_depth = 10;
	tree.scale = 10.0l / 2147483648.0l;

	camvox::CSGSphere	sphere;

	tree.addCSGObject(sphere, 0);
	tree.generatePOVCode();
}

