/* Matrix.h - Matrix class, uses boost++.
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
#ifndef MATRIX_H
#define MATRIX_H

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace camvox {

typedef boost::numeric::ublas::matrix<double>		_Matrix;
typedef boost::numeric::ublas::matrix<Interval>		_IntervalMatrix;

class Matrix : public _Matrix {
public:
	Matrix(void) : _Matrix(4, 4) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				(*this)(i, j) = (i == j) ? 1.0 : 0.0;
			}
		}
	}
};

class IntervalMatrix : public _IntervalMatrix {
public:
	IntervalMatrix(void) : _IntervalMatrix(4, 4) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				(*this)(i, j) = (i == j) ? 1.0 : 0.0;
			}
		}
	}
};
}
#endif
