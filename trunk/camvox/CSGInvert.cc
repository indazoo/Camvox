/* CSGInvert.cc - Turn all CSG objects contained inside-out.
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
#include <camvox/CSGInvert.h>

namespace camvox {

const CSGObject *CSGInvert::boxType(const IntervalVector &a) const
{
	bool grey = false;
	const CSGObject *bt;

	for (unsigned int i = 0; i < childs.size(); i++) {
		bt = childs[i]->boxType(a);
		if (bt == BLACK_BOX) {
			// Don't do anything. All of the boxes need to be black
			// to return white.

		} else if (bt == WHITE_BOX) {
			return BLACK_BOX;

		} else {
			// Mark as grey, so we won't return white. We don't return white
			// emidiatly, cause any white box would imediatly return black.
			grey = true;
		}
	}

	if (grey) {
		return this;
	} else {
		return WHITE_BOX;
	}
}

}
