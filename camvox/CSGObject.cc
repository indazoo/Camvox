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

CSGObject *WHITE_BOX = new CSGObject();
CSGObject *BLACK_BOX = new CSGObject();

std::vector<CSGObject *>	csgobject_list;

CSGObject::CSGObject()
{
	id = csgobject_list.size();
	csgobject_list.push_back(this);

	parent = NULL;
	transform = Matrix();
	resolution = 1.0;
	mergeResolutions();
	mergeTransforms();
}

CSGObject::~CSGObject()
{
	csgobject_list[id] = NULL;
}

/* mergeResolution() should be called on any change in resolution.
 */
void CSGObject::mergeResolutions(void)
{
	if (parent) {
		if (parent->total_resolution < this->resolution) {
			total_resolution = parent->total_resolution;
		} else {
			total_resolution = this->resolution;
		}
	} else {
		total_resolution = this->resolution;
	}
}

/* mergeTransforms() should be called on any change in geometry parameters.
 */
void CSGObject::mergeTransforms(void)
{
	if (parent) {
		total_transform = parent->total_transform * transform;
	} else {
		total_transform = transform;
	}
	total_inv_transform = total_transform.invert();
}

void CSGObject::translate(const Vector &a)
{
	transform = transform.translate(a);
	mergeTransforms();
}

void CSGObject::scale(const Vector &a)
{
	transform = transform.scale(a);
	mergeTransforms();
}

void CSGObject::rotate(const Vector &a, double angle)
{
	transform = transform.rotate(a, angle);
	mergeTransforms();
}

void CSGObject::setResolution(double _resolution)
{
	resolution = _resolution;
	mergeResolutions();
}

const CSGObject *CSGObject::boxType(const IntervalVector &a) const
{
	return this;
}

}
