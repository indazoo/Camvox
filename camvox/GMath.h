/* GMath.h - Generic mathematical operations
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
#ifndef GMATH_H
#define GMATH_H

#include <math.h>

namespace camvox {

template <class T>
static inline T gmin(T a, T b)
{
	return a < b ? a : b;
}

static inline double gmin(double a, double b)
{
	return fmin(a, b);
}

template <class T>
static inline T gmax(T a, T b)
{
	return a > b ? a : b;
}

static inline double gmax(double a, double b)
{
	return fmax(a, b);
}

template <class T>
static inline T gabs(T a)
{
	return a < 0 ? -a : a;
}

static inline double gabd(double a)
{
	return fabs(a);
}

static inline double gcos(double a) {
	return cos(a);
}

static inline double gsin(double a) {
	return sin(a);
}

static inline double gsquare(double a) {
	return a * a;
}

static inline double gsqrt(double a) {
	return sqrt(a);
}

}

#endif
