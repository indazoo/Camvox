/* VoxCoord.h - A coord inside a octtree voxel volume, uniqely identifying each node.
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
#ifndef VOXCOORD_H
#define VOXCOORD_H

#include <stdint.h>
#include <camvox/Vector.h>

namespace camvox {

/** VoxCoord is an object that describes the position in oct-tree voxel space.
 * The root voxel that encapsulated the full voxel space, has coord (0, 0, 0, depth=0).
 * The coord describes the near/left/bottom corner. The far/right/top corner has
 * coord (0x8000, 0x8000, 0x8000, depth=0). There are no voxels outside of these
 * bounds, because it leads to an integer overflow when calculating the outer bound.
 *
 * There are 8 voxels inside the root voxel, they have the following coords:
 * <ul>
 * <li>(0x0000, 0x0000, 0x0000, depth=1)</li>
 * <li>(0x4000, 0x0000, 0x0000, depth=1)</li>
 * <li>(0x0000, 0x4000, 0x0000, depth=1)</li>
 * <li>(0x4000, 0x4000, 0x0000, depth=1)</li>
 * <li>(0x0000, 0x0000, 0x4000, depth=1)</li>
 * <li>(0x4000, 0x0000, 0x4000, depth=1)</li>
 * <li>(0x0000, 0x4000, 0x4000, depth=1)</li>
 * <li>(0x4000, 0x4000, 0x4000, depth=1) opposite corner is (0x8000, 0x8000, 0x8000, depth=1)</li>
 * </ul>
 *
 * This can continue until depth = 30, i.e. the 31th level. At his level each voxel has only the size of 1.
 */
class VoxCoord {
public:
	uint32_t	v[3];	///< 3D coordinate of the left,near,bottom corner of a voxel box.
	int		depth;	///< Depth within the oct-tree.

	/** Construct a new coord identifying the root voxel in the oct-tree.
	 */
	VoxCoord(void) {
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
		depth = 0;
	}

	/** Copy constructor.
	 */
	VoxCoord(const VoxCoord &a) {
		v[0] = a.v[0];
		v[1] = a.v[1];
		v[2] = a.v[2];
		depth = a.depth;
	}

	/** Copy operator.
	 */
	VoxCoord &operator=(const VoxCoord &a) {
		v[0] = a.v[0];
		v[1] = a.v[1];
		v[2] = a.v[2];
		depth = a.depth;
		return *this;
	}

	/** Get the coord of a neighbour voxel at the same oct-tree depth.
	 * To get the other corner of this voxel, one would specify (1, 1, 1)
	 * on this method.
	 * 
	 * @param x	number of voxels away on the x-axis.
	 * @param y	number of voxels away on the y-axis.
	 * @param z	number of voxels away on the z-axis.
	 * @returns The new voxel coord pointing to a neighbour voxel.
	 */
	VoxCoord nextNeighbour(int x, int y, int z) const {
		VoxCoord r = *this;

		uint32_t movement = 0x80000000 >> depth;
		r.v[0]+= movement * x;
		r.v[1]+= movement * y;
		r.v[2]+= movement * z;
		return r;
	}

	/** Get the coord of a child voxel at the next depth of the oct-tree.
	 * @param child_index	An index pointing to one of the eight childs of an oct-tree node.
	 * @returns The new voxel coord pointing to a child voxel.
	 */
	VoxCoord childCoord(int child_index) const {
		VoxCoord r = *this;
		r.depth++;

		return r.nextNeighbour(child_index & 1, (child_index >> 1) & 1, (child_index >> 2) & 1);
	}

	/** Get the axis aligned bounding box describing this voxel in cartesion coords.
	 * @param scale		The size of the smalles voxel.
	 * @returns An IntervalVector describing the axis aligned bounding box.
	 */
	IntervalVector boundingBox(double scale) const {
		VoxCoord opposite = nextNeighbour(1, 1, 1);

		double half_size = scale * 1073741824.0;

		return IntervalVector(
			Interval(v[0] * scale - half_size, opposite.v[0] * scale - half_size),
			Interval(v[1] * scale - half_size, opposite.v[1] * scale - half_size),
			Interval(v[2] * scale - half_size, opposite.v[2] * scale - half_size),
			1.0
		);
	}

	/** The size of this voxel.
	 * @returns the size of this voxel in the scale of the oct tree.
	 */
	double size(double scale) const {
		return (0x80000000 >> depth) * scale;
	}
};

}
#endif
