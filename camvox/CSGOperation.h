/* CSGOperation.h - Abstract baseclass for all boolean CSG operations.
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
#ifndef CSGOPERATION_H
#define CSGOPERATION_H

#include <vector>
#include <camvox/CSGObject.h>

namespace camvox {

class CSGOperation : public CSGObject {
public:
	std::vector<CSGObject *>	childs;

	CSGOperation() {}
	virtual ~CSGOperation() {}
	virtual void add(CSGObject *obj);
	virtual void mergeTransforms(void);
	virtual void mergeResolutions(void);
};

}
#endif
