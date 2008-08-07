/* CSGInvert.h - Turn all CSG objects contained inside-out.
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
#ifndef CSGINVERT_H
#define CSGINVERT_H

#include <camvox/CSGOperation.h>

namespace camvox {

class CSGInvert : public CSGOperation {
public:
	CSGInvert() {}
	~CSGInvert() {}
	const CSGObject *boxType(const IntervalVector &a) const;
};

}
#endif
