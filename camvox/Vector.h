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

/** A homogenious vector and point class.
 * A homogenious 3D vector has 4 values: x, y, z, w.
 * The w value can have one of two values; 0.0 means it is a vector,
 * while 1.0 means it is a point.
 *
 * Both a point and a vector can be multiplied by a 4 x 4 affine Matrix;
 * to be scaled, sheared, rotated and translated (translation only for points).
 * normal 3D points multiplied by a 3 x 3 matrix needed a seperate translation step.
 *
 * This vector template is compatible with both floating point values and with interval values.
 * @see TMatrix
 * @see TInterval
 */
template <class T>
class TVector {
public:
	T	x;	///< x-axis value.
	T	y;	///< y-axis value.
	T	z;	///< z-axis value.
	T	w;	///< 0.0 means vector, 1.0 means point.

	/** Construct a zero vector.
	 */
	TVector(void) : x(0.0), y(0.0), z(0.0), w(0.0) {}

	/** Construct a vector.
	 * @param _x	x-axis value
	 * @param _y	y-axis value
	 * @param _z	z-axis value
	 */
	TVector(T _x, T _y, T _z) : x(_x), y(_y), z(_z), w(0.0) {} 

	/** Construct a vector or point.
	 * @param _x	x-axis value
	 * @param _y	y-axis value
	 * @param _z	z-axis value
	 * @param _w	0.0 means vector, 1.0 means point
	 */
	TVector(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

	/** Invert the vector.
	 * Or mirror the point throught the origin.
	 */
	TVector operator-() const {
		return TVector(-x, -y, -z, w);
	}

	/** In place add.
	 */
	TVector &operator+=(const TVector &other) {
		x+= other.x;
		y+= other.y;
		z+= other.z;
		w+= other.w;
		return *this;
	}

	/** Return the length of the vector.
	 * Or the distance between the point and the origin.
	 */
	T length() const {
		return gsqrt(gsquare(x) + gsquare(y) + gsquare(z));
	}

	/** Set the length of this vector to 1, while keeping the same direction.
	 */
	TVector &normalize() {
		T c = 1.0 / length();

		x*= c;
		y*= c;
		z*= c;
		return *this;
	}

	/** Set the length of this vector to 1, while keeping the same direction.
	 */
	TVector getNormalized() const {
		T c = 1.0 / length();

		return TVector(x*c, y*c, z*c, w);
	}
};

typedef TVector<double>		Vector;
typedef TVector<Interval>	IntervalVector;

/*
static inline Vector operator/(double a, const Vector &b)
{
	return Vector(
		a / b.x,
		a / b.y,
		a / b.z,
		b.w
	);
}
*/

}

#endif
