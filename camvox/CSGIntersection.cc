/* CSGIntersection.cc - All CSG object contained in an union are merged together.
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
#include <camvox/CSGIntersection.h>

namespace camvox {

box_type_t CSGIntersection::boxType(const IntervalVector &a) const
{
	bool grey = false;

	for (unsigned int i = 0; i < childs.size(); i++) {
		switch (childs[i]->boxType(a)) {
		case BLACK_BOX:
			break;
		case GREY_BOX:
			grey = true;
			break;
		case WHITE_BOX:
			return WHITE_BOX;
		}
	}

	if (grey) {
		return GREY_BOX;
	} else {
		return BLACK_BOX;
	}
}

}
