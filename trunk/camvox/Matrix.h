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

namespace camvox {

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
		Vector	result;

		result.x = other.x * m[0][0] + other.y * m[0][1] + other.z * m[0][2] + other.w * m[0][3];
		result.y = other.x * m[1][0] + other.y * m[1][1] + other.z * m[1][2] + other.w * m[1][3];
		result.z = other.x * m[2][0] + other.y * m[2][1] + other.z * m[2][2] + other.w * m[2][3];
		result.w = other.x * m[3][0] + other.y * m[3][1] + other.z * m[3][2] + other.w * m[3][3];
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
