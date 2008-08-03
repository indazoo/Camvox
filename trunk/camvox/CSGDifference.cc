/* CSGDifference.cc - All CSG object contained in an union are merged together.
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
#include <camvox/CSGDifference.h>

namespace camvox {

box_type_t CSGDifference::boxType(const IntervalVector &a) const
{
	bool grey = false;

	// It should at least be inside the first child	
	switch (childs[0]->boxType(a)) {
	case WHITE_BOX:
		return WHITE_BOX;
	case GREY_BOX:
		grey = true;
		break;
	case BLACK_BOX:
		break;
	}

	// All the other childs are subtracted
	for (unsigned int i = 1; i < childs.size(); i++) {
		switch (childs[i]->boxType(a)) {
		case BLACK_BOX:
			return WHITE_BOX;
		case GREY_BOX:
			grey = true;
			break;
		case WHITE_BOX:
			break;
		}
	}
	if (grey) {
		// One of the boxes is grey.
		return GREY_BOX;
	} else {
	// All boxes are white, so the original object is still there.
		return BLACK_BOX;
	}
}

}
