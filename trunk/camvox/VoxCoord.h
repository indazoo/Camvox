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

/* VoxCoord is an object that describes the position in oct-tree voxel space.
 *
 * The top-box that encapsulated the full voxel space, has coord (0, 0, 0, depth=0).
 * The coord describes the near/left/bottom corner. The far/right/top corner has
 * coord (0x8000, 0x8000, 0x8000, depth=0). There are no boxes outside of these
 * bounds, because it leads to an integer overflow when calculating the outer bound.
 *
 * The depth attribute is the size of the box, 0 is the outer box, 1 is 1/8 of the outer box.
 *
 * There are 8 boxes inside the top-box, they have the following coords:
 *    (0x0000, 0x0000, 0x0000, depth=1)
 *    (0x4000, 0x0000, 0x0000, depth=1)
 *    (0x0000, 0x4000, 0x0000, depth=1)
 *    (0x4000, 0x4000, 0x0000, depth=1)
 *    (0x0000, 0x0000, 0x4000, depth=1)
 *    (0x4000, 0x0000, 0x4000, depth=1)
 *    (0x0000, 0x4000, 0x4000, depth=1)
 *    (0x4000, 0x4000, 0x4000, depth=1) bounded by (0x8000, 0x8000, 0x8000, depth=1)
 *
 */
class VoxCoord {
public:
	uint32_t	v[3];
	int		depth;

	VoxCoord(void);
	VoxCoord(const VoxCoord &a);
	VoxCoord &operator=(const VoxCoord &a);
	VoxCoord nextNeighbour(int x, int y, int z) const;
	VoxCoord childCoord(int child_index) const;
	IntervalVector boundingBox(double scale) const;
};

}
#endif
