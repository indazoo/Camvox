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

const CSGObject *CSGIntersection::boxType(const IntervalVector &a) const
{
	const CSGObject *bt;
	const CSGObject *grey_bt = NULL;
	int nr_grey = 0;

	for (unsigned int i = 0; i < childs.size(); i++) {
		bt = childs[i]->boxType(a);
		if (bt == BLACK_BOX) {
			// Don't do anything, could be grey.
		} else if (bt == WHITE_BOX) {
			return WHITE_BOX;
		} else {
			// Don't return anything, could be white.
			nr_grey++;
			grey_bt = bt;
		}
	}

	if (nr_grey == 1) {
		// Only one object needs to be reevaluated.
		return grey_bt;
	} else if (nr_grey > 1) {
		// We need to be reevaluated.
		return this;
	} else {
		return BLACK_BOX;
	}
}

}
