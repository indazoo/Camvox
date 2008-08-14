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

/** Gauss-Jordan elimination.
 * Basically converts the the augmented matrix, until the first half is
 * an identity matrix.
 *
 * This is useful to invert a matrix by augmenting it with a identity matrix, after
 * this function the right side of the augmented matrix contains the inverted
 * original matrix.
 *
 * @param m[H][W]	The augmented matrix. It will return the augmented matrix with
 *			the leftside being the identity matrix.
 * @returns true if successful.
 */
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

/** An affine transformation matrix.
 * An 4 x 4 transformation matrix to scale, shear, rotate and translate
 * homogeious vectors and points.
 */
template <class T>
class TMatrix {
public:
	T m[4][4];	///< the 4 x 4 matrix.

	/** Construct an identity matrix.
	 */
	TMatrix(void) {
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				m[r][c] = (r == c) ? 1.0 : 0.0;
			}
		}
	}

	/** Multiply the matrix by a scalar.
	 */
	TMatrix operator*(const T &other) const {
		TMatrix	result;

		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				result.m[r][c] = other * m[r][0];
			}
		}
		return result;
	}

	/** Cross product of two matrices.
	 */
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

	/** Transform a Vector by this matrix.
	 */
	Vector operator*(const Vector &other) const {
		Vector	result;

		result.x = other.x * m[0][0] + other.y * m[0][1] + other.z * m[0][2] + other.w * m[0][3];
		result.y = other.x * m[1][0] + other.y * m[1][1] + other.z * m[1][2] + other.w * m[1][3];
		result.z = other.x * m[2][0] + other.y * m[2][1] + other.z * m[2][2] + other.w * m[2][3];
		result.w = other.x * m[3][0] + other.y * m[3][1] + other.z * m[3][2] + other.w * m[3][3];
		return result;
	}

	/** Transform an IntervalVector by this matrix.
	 * An IntervalVector is like an axis-aligned bounded box.
	 */
	IntervalVector operator*(const IntervalVector &other) const {
		IntervalVector	result;

		result.x = other.x * m[0][0] + other.y * m[0][1] + other.z * m[0][2] + other.w * m[0][3];
		result.y = other.x * m[1][0] + other.y * m[1][1] + other.z * m[1][2] + other.w * m[1][3];
		result.z = other.x * m[2][0] + other.y * m[2][1] + other.z * m[2][2] + other.w * m[2][3];
		result.w = other.x * m[3][0] + other.y * m[3][1] + other.z * m[3][2] + other.w * m[3][3];
		return result;
	}

	/** Calculate the inversion of this matrix.
	 */
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

	/** Add a translation operation to this matrix.
	 *
	 * @param v Translation vector
	 * @returns A new matrix with the translation applied.
	 */
	TMatrix translate(const Vector &v) const {
		TMatrix	B = TMatrix();

		B.m[0][3] = v.x;
		B.m[1][3] = v.y;
		B.m[2][3] = v.z;
		return B * (*this);
	}

	/** Add a scale operation to this matrix.
	 *
	 * @param v Vector with scale values along each axis.
	 * @returns A new matrix with the scale applied.
	 */
	TMatrix scale(const Vector &v) const {
		TMatrix	B = TMatrix();

		B.m[0][0] = v.x;
		B.m[1][1] = v.y;
		B.m[2][2] = v.z;
		return B * (*this);
	}

	/** Add a rotation operation to this matrix.
	 * The rotation is around a vector oriented at the origin. The rotation is
	 * counter-clockwise if one looks from the origin along the vector.
	 *
	 * @param v The rotation is along this vector.
	 * @param angle The rotation is radians around the vector.
	 * @returns A new matrix with the rotation applied.
	 */
	TMatrix rotate(const TVector<T> &v, double _angle) const {
		double		angle = -_angle;
		TMatrix		B = TMatrix();
		TVector<T>	nv = v.normalize();

		fprintf(stderr, "%lf, %lf, %lf\n", nv.x, nv.y, nv.z);

		B.m[0][0] = (1.0 - gcos(angle)) * (nv.x * nv.x) + (       gcos(angle));
		B.m[0][1] = (1.0 - gcos(angle)) * (nv.x * nv.y) + (nv.z * gsin(angle));
		B.m[0][2] = (1.0 - gcos(angle)) * (nv.x * nv.z) - (nv.y * gsin(angle));
		B.m[1][0] = (1.0 - gcos(angle)) * (nv.x * nv.y) - (nv.z * gsin(angle));
		B.m[1][1] = (1.0 - gcos(angle)) * (nv.y * nv.y) + (       gcos(angle));
		B.m[1][2] = (1.0 - gcos(angle)) * (nv.y * nv.z) + (nv.x * gsin(angle));
		B.m[2][0] = (1.0 - gcos(angle)) * (nv.x * nv.z) + (nv.y * gsin(angle));
		B.m[2][1] = (1.0 - gcos(angle)) * (nv.y * nv.z) - (nv.x * gsin(angle));
		B.m[2][2] = (1.0 - gcos(angle)) * (nv.z * nv.z) + (       gcos(angle));
		return B * (*this);
	}
};

typedef TMatrix<double>		Matrix;

}
#endif
