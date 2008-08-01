/* VoxCoord.cc - A coord inside a octtree voxel volume, uniqely identifying each node.
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
#include <camvox/VoxCoord.h>

namespace camvox {

VoxCoord::VoxCoord(void)
{
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
	depth = 0;
}

VoxCoord::VoxCoord(const VoxCoord &a)
{
	v[0] = a.v[0];
	v[1] = a.v[1];
	v[2] = a.v[2];
	depth = a.depth;
}

VoxCoord &VoxCoord::operator=(const VoxCoord &a)
{
	v[0] = a.v[0];
	v[1] = a.v[1];
	v[2] = a.v[2];
	depth = a.depth;
	return *this;
}

/* This returns the coord of one if its neighbours.
 */
VoxCoord VoxCoord::nextNeighbour(int x, int y, int z) const {
	VoxCoord r = *this;

	uint32_t movement = 0x80000000 >> depth;
	r.v[0]+= movement * x;
	r.v[1]+= movement * y;
	r.v[2]+= movement * z;
	return r;
}

/* Returns one of the eight child voxels.
 */
VoxCoord VoxCoord::childCoord(int child_index) const {
	VoxCoord r = *this;
	r.depth++;

	return r.nextNeighbour(child_index & 1, (child_index >> 1) & 1, (child_index >> 2) & 1);
}

/* The boudingBox in world space.
 * This box is used to intersect with the CSG objects.
 */
IntervalVector VoxCoord::boundingBox(double scale) const {
	VoxCoord opposite = nextNeighbour(1, 1, 1);

	IntervalVector	r = IntervalVector(
		Interval(v[0] * scale, opposite.v[0] * scale),
		Interval(v[1] * scale, opposite.v[1] * scale),
		Interval(v[2] * scale, opposite.v[2] * scale),
		1.0
	);

	// Return an interval vector bounded around this voxel .
	return r;
}

}
