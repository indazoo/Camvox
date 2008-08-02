/* CSGObject.h - Abstract baseclass for all CSG objects.
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
#ifndef CSGOBJECT_H
#define CSGOBJECT_H

#include <camvox/Vector.h>
#include <camvox/Matrix.h>

namespace camvox {

typedef enum {
	WHITE_BOX = -1,		// Completely Outside
	GREY_BOX,		// Location Indeterminate
	BLACK_BOX		// Completely Inside
} box_type_t;

class CSGObject {
public:
	Matrix		transform;
	Matrix		inv_transform;
	Matrix		total_transform;
	Matrix		total_inv_transform;

	CSGObject() {}
	virtual ~CSGObject() {}
	virtual void translate(const Vector &a);
	virtual void scale(const Vector &a);
	virtual box_type_t boxType(const IntervalVector &a) const;
};

}
#endif
