/* Vector.h - Homogeious vector class.
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

#include <camvox/Interval.h>

namespace camvox {

template <class T>
class TVector {
public:
	T	x, y, z, w;

	TVector(void) : x(0.0), y(0.0), z(0.0), w(0.0) {}
	TVector(T _x, T _y, T _z) : x(_x), y(_y), z(_z), w(0.0) {} 
	TVector(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

	TVector operator-() const {
		return TVector(-x, -y, -z, w);
	}

	T length() const {
		return gsqrt(gsquare(x) + gsquare(y) + gsquare(z));
	}

	TVector normalize() const {
		T c = 1.0 / length();
		return TVector(x * c, y * c, z * c, w);
	}
};

typedef TVector<double>		Vector;
typedef TVector<Interval>	IntervalVector;

static inline Vector operator/(double a, const Vector &b)
{
	return Vector(
		a / b.x,
		a / b.y,
		a / b.z,
		b.w
	);
}

}

#endif
