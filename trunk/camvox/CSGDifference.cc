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

const CSGObject *CSGDifference::boxType(const IntervalVector &a) const
{
	const CSGObject *bt;
	const CSGObject *grey_bt = NULL;
	bool grey = false;

	// It should at least be inside the first child	
	bt = childs[0]->boxType(a);
	if (bt == WHITE_BOX) {
		return WHITE_BOX;
	} else if (bt == BLACK_BOX) {
		// We still need to check if something is subtracted.
	} else {
		// We may be there, but also check if something is subtracted.
		grey_bt = bt;
	}

	// All the other childs are subtracted
	for (unsigned int i = 1; i < childs.size(); i++) {
		bt = childs[i]->boxType(a);
		if (bt == BLACK_BOX) {
			return WHITE_BOX;
		} else if (bt == WHITE_BOX) {
			// Don't do anything, could be grey.
		} else {
			// Don't return anything, could be black.
			grey = true;
		}
	}

	if (grey) {
		// We need to be reevaluated.
		return this;
	} else if (grey_bt) {
		// Nothing is subtracted so only our primary object needs to be reevaluated.
		return grey_bt;
	} else {
		// All secondary boxes are white, so the primary object is still there.
		return BLACK_BOX;
	}
}

}
