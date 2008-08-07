/* CSGUnion.cc - All CSG object contained in an union are merged together.
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
#include <camvox/CSGUnion.h>

namespace camvox {

const CSGObject *CSGUnion::boxType(const IntervalVector &a) const
{
	int		nr_grey = 0;
	const CSGObject	*bt;
	const CSGObject	*grey_bt = NULL;
	
	for (unsigned int i = 0; i < childs.size(); i++) {
		bt = childs[i]->boxType(a);
		if (bt == BLACK_BOX) {
			return BLACK_BOX;
		} else if (bt == WHITE_BOX) {
			// Don't do anything, all the boxes need to be white to return white.
		} else {
			// We can no longer return white, but we could return black.
			nr_grey++;
			grey_bt = bt;
		}
	}

	if (nr_grey == 1) {
		// Only one box was grey, so only that needs to be evaluated.
		return grey_bt;
	} else if (nr_grey > 1) {
		// We need to be evaluated, because more boxes are grey.
		return this;
	} else {
		// All boxes are white.
		return WHITE_BOX;
	}
}

}
