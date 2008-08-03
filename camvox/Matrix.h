/* Matrix.h - Affine transform matrix class
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

#include <stdio.h>
#include <camvox/GMath.h>

namespace camvox {

template <class T, int H, int W>
bool gauss_jordan(T m[H][W])
{
	const double eps = 0.00000001;
	for (int y = 0; y < H; y++) {
		int maxrow = y;

		// Find the maximum pivot.
		for (int y2 = y + 1; y2 < H; y2++) {
			if (gabs(m[y2][y]) > gabs(m[maxrow][y])) {
				maxrow = y2;
			}
		}

		// Switch rows.
		for (int x = 0; x < W; x++) {
			T tmp = m[y][x];
			m[y][x] = m[maxrow][x];
			m[maxrow][x] = tmp;
		}

		// Is the matrix singular.
		if (gabs(m[y][y]) <= eps) {
			return false;
		}

		// Eliminate column y.
		for (int y2 = y + 1; y2 < H; y2++) {
			T c = m[y2][y] / m[y][y];
			for (int x = y; x < W; x++) {
				m[y2][x] -= m[y][x] * c;
			}
		}
	}

	// Backsubstitude
	for (int y = H - 1; y >= 0; y--) {
		T c = 1.0 / m[y][y];

		for (int y2 = 0; y2 < y; y2++) {
			for (int x = W - 1; x >= y; x--) {
				m[y2][x] -= m[y][x] * m[y2][y] * c;
			}
		}
		m[y][y] *= c;

		// Normalize row y
		for (int x = H; x < W; x++) {
			m[y][x] *= c;
		}
	}

	return true;
}


template <class T>
class TMatrix {
public:
	T m[4][4];

	TMatrix(void) {
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				m[r][c] = (r == c) ? 1.0 : 0.0;
			}
		}
	}

	TMatrix operator*(const T &other) const {
		TMatrix	result;

		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				result.m[r][c] = other * m[r][0];
			}
		}
		return result;
	}

	TMatrix operator*(const TMatrix &other) const {
		TMatrix	result;

		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				result.m[r][c] =
					m[r][0] * other.m[0][c] +
					m[r][1] * other.m[1][c] +
					m[r][2] * other.m[2][c] +
					m[r][3] * other.m[3][c];
			}
		}
		return result;
	}

	Vector operator*(const Vector &other) const {
		Vector	result;

		result.x = other.x * m[0][0] + other.y * m[0][1] + other.z * m[0][2] + other.w * m[0][3];
		result.y = other.x * m[1][0] + other.y * m[1][1] + other.z * m[1][2] + other.w * m[1][3];
		result.z = other.x * m[2][0] + other.y * m[2][1] + other.z * m[2][2] + other.w * m[2][3];
		result.w = other.x * m[3][0] + other.y * m[3][1] + other.z * m[3][2] + other.w * m[3][3];
		return result;
	}

	IntervalVector operator*(const IntervalVector &other) const {
		IntervalVector	result;

		result.x = other.x * m[0][0] + other.y * m[0][1] + other.z * m[0][2] + other.w * m[0][3];
		result.y = other.x * m[1][0] + other.y * m[1][1] + other.z * m[1][2] + other.w * m[1][3];
		result.z = other.x * m[2][0] + other.y * m[2][1] + other.z * m[2][2] + other.w * m[2][3];
		result.w = other.x * m[3][0] + other.y * m[3][1] + other.z * m[3][2] + other.w * m[3][3];
		return result;
	}

	TMatrix invert(void) const {
		double	a[4][8];
		TMatrix	result;

		// Create an augmented matrix.
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				// Left side is our original matrix.
				a[r][c] = m[r][c];
				// Right side an identity matrix.
				a[r][c+4] = (r == c) ? 1.0 : 0.0;
			}
		}

		if (!gauss_jordan<T, 4, 8>(a)) {
			fprintf(stderr, "Error: Matrix is singular\n");
		}

		// Retrieve the inverted matrix
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				// Left side is an identity matrix.
				// Right side is the inverted matrix.
				result.m[r][c] = a[r][c+4];
			}
		}

		return result;
	}

	TMatrix translate(const Vector &v) const {
		TMatrix	B = TMatrix();

		B.m[0][3] = v.x;
		B.m[1][3] = v.y;
		B.m[2][3] = v.z;
		return B * (*this);
	}

	TMatrix scale(const Vector &v) const {
		TMatrix	B = TMatrix();

		B.m[0][0] = v.x;
		B.m[1][1] = v.y;
		B.m[2][2] = v.z;
		return B * (*this);
	}
};

typedef TMatrix<double>		Matrix;

}
#endif
