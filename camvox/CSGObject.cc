/* CSGObject.cc - Abstract baseclass for all CSG objects.
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
#include <camvox/CSGObject.h>

namespace camvox {

CSGObject::CSGObject()
{
	parent = NULL;
	transform = Matrix();
	inv_transform = Matrix();
	mergeTransforms();
}

void CSGObject::mergeTransforms(void)
{
	if (parent) {
		total_transform = transform * parent->total_transform;
		total_inv_transform = parent->total_inv_transform * inv_transform;
	} else {
		total_transform = transform;
		total_inv_transform = inv_transform;
	}
}

void CSGObject::translate(const Vector &a)
{
	transform = transform.translate(a);
	inv_transform = transform.translate(-a);
	mergeTransforms();
}

void CSGObject::scale(const Vector &a)
{
	transform = transform.scale(a);
	inv_transform = transform.scale(1.0 / a);
	mergeTransforms();
}

box_type_t CSGObject::boxType(const IntervalVector &a) const
{
	assert(0);
	return GREY_BOX;
}

}
