/* CSGCylinder.h - CSG cylinder object.
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
#include <camvox/CSGCylinder.h>

namespace camvox {

CSGCylinder::CSGCylinder(double _diameter) : diameter(_diameter)
{
}

Interval CSGCylinder::characteristic(const IntervalVector &a) const
{
	IntervalVector b = total_inv_transform * a;
	return (gsquare(b.x) + gsquare(b.y) - gsquare(diameter));
}

}
