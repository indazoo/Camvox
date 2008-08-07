/* CSGPrimative.cc - Abstract baseclass for all CSG primative objects.
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
#include <cassert>
#include <camvox/CSGPrimative.h>

namespace camvox {

Interval CSGPrimative::characteristic(const IntervalVector &a) const
{
	assert(0);
	return 0.0;
}

const CSGObject *CSGPrimative::boxType(const IntervalVector &a) const
{
	Interval I = characteristic(a);
	if (I.high < 0.0) {
		return BLACK_BOX;
	} else if (I.low > 0.0) {
		return WHITE_BOX;
	} else {
		// We need to be reevaluated.
		return this;
	}
}

}
