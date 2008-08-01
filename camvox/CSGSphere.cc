/* CSGSphere.h - CSG sphere objects.
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
#include <camvox/CSGSphere.h>

namespace camvox {

box_type_t CSGSphere::characteristic(const IntervalVector &a) const
{
	Interval I =  (square(a[0]) + square(a[1]) + square(a[2]) - Interval(1.0, 1.0));
	if (I.upper() < 0.0) {
		return BLACK_BOX;
	} else if (I.lower() > 0.0) {
		return WHITE_BOX;
	} else {
		return GREY_BOX;
	}
}

}
