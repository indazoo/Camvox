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
	int		depth;	///< Depth within the oct-tree.
	uint32_t	x;	///< 3D x-axis of the left,near,bottom corner of a voxel box.
	uint32_t	y;	///< 3D y-axis of the left,near,bottom corner of a voxel box.
	uint32_t	z;	///< 3D z-axis of the left,near,bottom corner of a voxel box.

	/** Construct a new coord identifying the root voxel in the oct-tree.
	 */
	VoxCoord(void);

	/** Copy constructor.
	 */
	VoxCoord(const VoxCoord &other);

	/** Create a coord at a specific depth.
	 * @param depth	at what level is this voxel, 0 is root.
	 * @param x	number of voxels right at this depth.
	 * @param y	number of voxels away at this depth.
	 * @param z	number of voxels up at this depth.
	 */
	VoxCoord(int _depth, int _x = 0, int _y = 0, int _z = 0);

	/** Copy operator.
	 */
	VoxCoord &operator=(const VoxCoord &other);

	/** Get the coord of a neighbour voxel at the same oct-tree depth.
	 * To get the other corner of this voxel, one would specify (1, 1, 1)
	 * on this method.
	 * 
	 * @param x	number of voxels away on the x-axis.
	 * @param y	number of voxels away on the y-axis.
	 * @param z	number of voxels away on the z-axis.
	 * @returns The new voxel coord pointing to a neighbour voxel.
	 */
	VoxCoord nextNeighbour(int _x, int _y, int _z) const;

	/** Get the coord of a child voxel at the next depth of the oct-tree.
	 * @param child_index	An index pointing to one of the eight childs of an oct-tree node.
	 * @returns The new voxel coord pointing to a child voxel.
	 */
	VoxCoord childCoord(int child_index) const;

	/** Get the axis aligned bounding box describing this voxel in cartesion coords.
	 * @param scale		The size of the smalles voxel.
	 * @returns An IntervalVector describing the axis aligned bounding box.
	 */
	IntervalVector boundingBox(double scale) const;

	/** Get the center of this voxel in cartesion coords.
	 * @param scale		The size of the smalles voxel.
	 * @returns An point in the center.
	 */
	Vector center(double scale) const;

	/** The size of this voxel.
	 * @param scale		The scale for the smallest voxel.
	 * @returns the size of this voxel in the scale of the oct tree.
	 */
	double size(double scale) const;

	/** The index inside the node at a specific depth.
	 * @param _depth	the node selected at depth.
	 * @returns index of child node.
	 */
	int indexAtDepth(int _depth) const;
};

inline VoxCoord::VoxCoord(void) :
	depth(0), x(0), y(0), z(0)
{
}

inline VoxCoord::VoxCoord(const VoxCoord &other) :
	depth(other.depth), x(other.x), y(other.y), z(other.z)
{
}

inline VoxCoord::VoxCoord(int _depth, int _x, int _y, int _z) :
	depth(_depth)
{
	uint32_t movement = 0x80000000 >> depth;
	x = movement * _x;
	y = movement * _y;
	z = movement * _z;
}

inline VoxCoord &VoxCoord::operator=(const VoxCoord &other)
{
	depth = other.depth;
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

inline VoxCoord VoxCoord::nextNeighbour(int _x, int _y, int _z) const
{
	VoxCoord r = *this;

	uint32_t movement = 0x80000000 >> depth;
	r.x+= movement * _x;
	r.y+= movement * _y;
	r.z+= movement * _z;
	return r;
}

inline VoxCoord VoxCoord::childCoord(int child_index) const
{
	VoxCoord r = *this;
	r.depth++;

	return r.nextNeighbour(child_index & 1, (child_index >> 1) & 1, (child_index >> 2) & 1);
}

inline IntervalVector VoxCoord::boundingBox(double scale) const
{
	VoxCoord opposite = nextNeighbour(1, 1, 1);

	double half_size = scale * 1073741824.0;

	// Find the two opposite corners.
	// Then scale it to cartesion coords and
	// move the origin to the middle of voxel space.
	return IntervalVector(
		Interval(x * scale - half_size, opposite.x * scale - half_size),
		Interval(y * scale - half_size, opposite.y * scale - half_size),
		Interval(z * scale - half_size, opposite.z * scale - half_size),
		1.0
	);
}

inline Vector VoxCoord::center(double scale) const
{
	VoxCoord opposite = nextNeighbour(1, 1, 1);

	double half_size = scale * 1073741824.0;

	// Take the average between the two opposite corners.
	// Then scale it to cartesion coords and
	// move the origin to the middle of voxel space.
	return Vector(
		(x + opposite.x) * 0.5 * scale - half_size,
		(y + opposite.y) * 0.5 * scale - half_size,
		(z + opposite.z) * 0.5 * scale - half_size,
		1.0
	);
}

inline double VoxCoord::size(double scale) const
{
	return (0x80000000 >> depth) * scale;
}

inline int VoxCoord::indexAtDepth(int _depth) const
{
	uint32_t mask = (0x80000000 >> _depth);

	return (
		((x & mask) ? 1 : 0) |
		((y & mask) ? 2 : 0) |
		((z & mask) ? 4 : 0)
	);
}

}
#endif
