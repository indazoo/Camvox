/* Vector.h - Vector class, uses boost++.
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
#ifndef VECTOR_H
#define VECTOR_H

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <camvox/Interval.h>

namespace camvox {

typedef boost::numeric::ublas::vector<double>		_Vector;
typedef boost::numeric::ublas::vector<Interval>		_IntervalVector;

class Vector : public _Vector {
public:
	Vector(void) : _Vector(4) {
		(*this)[0] = 0.0;
		(*this)[1] = 0.0;
		(*this)[2] = 0.0;
		(*this)[3] = 0.0;
	}

	Vector(double x, double y, double z) : _Vector(4) {
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		(*this)[3] = 0.0;
	}

	Vector(double x, double y, double z, double w) : _Vector(4) {
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		(*this)[3] = w;
	}
};

class IntervalVector : public _IntervalVector {
public:
	IntervalVector(void) : _IntervalVector(4) {
		(*this)[0] = Interval(0.0);
		(*this)[1] = Interval(0.0);
		(*this)[2] = Interval(0.0);
		(*this)[3] = Interval(0.0);
	}

	IntervalVector(Interval x, Interval y, Interval z) : _IntervalVector(4) {
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		(*this)[3] = Interval(0.0);
	}

	IntervalVector(Interval x, Interval y, Interval z, Interval w) : _IntervalVector(4) {
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		(*this)[3] = w;
	}
};


}
#endif
