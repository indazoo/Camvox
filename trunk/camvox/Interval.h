/* Interval.h - Interval arrithmatic class.
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
#ifndef INTERVAL_H
#define INTERVAL_H

#include <camvox/GMath.h>

#include <fenv.h>

namespace camvox {

template <class T>
class TInterval {
public:
	T	low, high;

	TInterval(T _low, T _high) {
		low = _low;
		high = _high;
	}

	TInterval(T value) {
		low = high = value;
	}

	~TInterval() {}

	TInterval operator+(const TInterval &other) const {
		int orig = fegetround();

		fesetround(FE_DOWNWARD);
		T low = this->low + other.low;
		T high = (-this->high) + (-other.high);
		fesetround(orig);

		return TInterval(low, -high);
	}

	TInterval operator-(const TInterval &other) const {
		int orig = fegetround();

		fesetround(FE_DOWNWARD);
		T low = this->low + (-other.high);
		T high = (-this->high) + other.low;
		fesetround(orig);

		return TInterval(low, -high);
	}

	TInterval operator*(const TInterval &other) const {
		int orig = fegetround();

		fesetround(FE_DOWNWARD);
		T low = gmin(
				gmin(this->low * other.low, this->low * other.high),
				gmin(this->high * other.low, this->high * other.high)
		);
		fesetround(FE_UPWARD);
		T high = gmax(
				gmax(this->low * other.low, this->low * other.high),
				gmax(this->high * other.low, this->high * other.high)
		);
		fesetround(orig);

		return TInterval(low, high);
	}

	TInterval operator/(const TInterval &other) const {
		int orig = fegetround();

		fesetround(FE_DOWNWARD);
		T low = gmin(
				gmin(this->low / other.low, this->low / other.high),
				gmin(this->high / other.low, this->high / other.high)
		);
		fesetround(FE_UPWARD);
		T high = gmax(
				gmax(this->low / other.low, this->low / other.high),
				gmax(this->high / other.low, this->high / other.high)
		);
		fesetround(orig);

		return TInterval(low, high);
	}

	TInterval abs(void) const {
		T low = gmax(gmax(this->low, -this->high), 0.0);
		T high = gmax(-this->low, this->high);
		return TInterval(low, high);
	}

	TInterval square(void) const {
		int orig = fegetround();
		TInterval tmp = this->abs();

		fesetround(FE_DOWNWARD);
		T low = gmin(tmp.low * tmp.low, tmp.high * tmp.low);
		fesetround(FE_UPWARD);
		T high = gmax(tmp.high * tmp.high, tmp.low * tmp.high);
		fesetround(orig);

		return TInterval(low, high);
	}

	TInterval squareroot(void) const {
		int orig = fegetround();
		TInterval tmp = this->abs();

		fesetround(FE_DOWNWARD);
		T low = sqrt(tmp.low);
		fesetround(FE_UPWARD);
		T high = sqrt(tmp.high);
		fesetround(orig);

		return TInterval(low, high);
	}
};

typedef TInterval<double>	Interval;

}
#endif
