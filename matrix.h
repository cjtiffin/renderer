#ifndef _MATRIX_H_
#define _MATRIX_H_
#pragma once

#include "core/core.h"
#include "operators.h"


// Row major
template <class T> class Matrix4
	: public operators::all_matrix_ops<Matrix4<T>, T>
{
public:
	typedef  Matrix4<T>  MType;

	Matrix4() {}
	Matrix4(T m00, T m01, T m02, T m03,
	        T m10, T m11, T m12, T m13,
	        T m20, T m21, T m22, T m23,
	        T m30, T m31, T m32, T m33)
	{
		mm[0][0] = m00, mm[0][1] = m01, mm[0][2] = m02, mm[0][3] = m03,
		mm[1][0] = m10, mm[1][1] = m11, mm[1][2] = m12, mm[1][3] = m13,
		mm[2][0] = m20, mm[2][1] = m21, mm[2][2] = m22, mm[2][3] = m23,
		mm[3][0] = m30, mm[3][1] = m31, mm[3][2] = m32, mm[3][3] = m33;
	}

	static MType ZERO()
	{
		MType m;
		memset(&m.raw, 0, sizeof(T) * size);
		return m;
	}
	static MType ZERO_AFFINE()
	{
		MType m = ZERO();
		m.mm[3][3] = 1;
		return m;
	}
	static MType IDENTITY()
	{
		MType m = ZERO_AFFINE();
		m.mm[0][0] = m.mm[1][1] = m.mm[2][2] = 1;
		return m;
	}
	static MType SCALE(T x, T y, T z)
	{
		MType m = ZERO_AFFINE();
		m.mm[0][0] = x;
		m.mm[1][1] = y;
		m.mm[2][2] = z;
		return m;
	}
	static MType TRANSLATION(T x, T y, T z)
	{
		MType m = IDENTITY();
		m.mm[0][3] = x;
		m.mm[1][3] = y;
		m.mm[2][3] = z;
		return m;
	}
	static MType FROM_AXES(Vector3<T> const &x, Vector3<T> const &y, Vector3<T> const &z)
	{
		MType m = ZERO_AFFINE();
		m.set_column(0, x);
		m.set_column(1, y);
		m.set_column(2, z);
		return m;
	}


	MType transpose() const
	{
		return MType(mm[0][0], mm[1][0], mm[2][0], mm[3][0],
		             mm[0][1], mm[1][1], mm[2][1], mm[3][1],
		             mm[0][2], mm[1][2], mm[2][2], mm[3][2],
		             mm[0][3], mm[1][3], mm[2][3], mm[3][3]);
	}
	MType & transpose()
	{
		*this = static_cast<MType const &>(*this).transpose();
		return *this;
	}

	// inverse, determinant
	MType inverse () const
	{
		assert(is_affine());

		double m00 = mm[0][0], m01 = mm[0][1], m02 = mm[0][2];
		double m10 = mm[1][0], m11 = mm[1][1], m12 = mm[1][2];
		double m20 = mm[2][0], m21 = mm[2][1], m22 = mm[2][2];
		double m03 = mm[0][3], m13 = mm[1][3], m23 = mm[2][3];

		double t00 = m22 * m11 - m21 * m12;
		double t10 = m20 * m12 - m22 * m10;
		double t20 = m21 * m10 - m20 * m11;

		double inv_det = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

		t00 *= inv_det; t10 *= inv_det; t20 *= inv_det;
		m00 *= inv_det; m01 *= inv_det; m02 *= inv_det;

		double r00 = t00;
		double r01 = m02 * m21 - m01 * m22;
		double r02 = m01 * m12 - m02 * m11;

		double r10 = t10;
		double r11 = m00 * m22 - m02 * m20;
		double r12 = m02 * m10 - m00 * m12;

		double r20 = t20;
		double r21 = m01 * m20 - m00 * m21;
		double r22 = m00 * m11 - m01 * m10;

		double r03 = - (r00 * m03 + r01 * m13 + r02 * m23);
		double r13 = - (r10 * m03 + r11 * m13 + r12 * m23);
		double r23 = - (r20 * m03 + r21 * m13 + r22 * m23);

		return MType(r00, r01, r02, r03,
		             r10, r11, r12, r13,
		             r20, r21, r22, r23,
		             0,   0,   0,   1);
	}

	bool is_affine() const { return mm[3][0] == 0 && mm[3][1] == 0 && mm[3][2] == 0 && mm[3][3] == 1; }

	Vector2<T> row2(size_t r) const { assert(r<width); return Vector2<T>(mm[r][0], mm[r][1]); }
	Vector3<T> row3(size_t r) const { assert(r<width); return Vector3<T>(mm[r][0], mm[r][1], mm[r][2]); }
	Vector4<T> row4(size_t r) const { assert(r<width); return Vector4<T>(mm[r][0], mm[r][1], mm[r][2], mm[r][3]); }

	Vector2<T> column2(size_t c) const { assert(c<height); return Vector2<T>(mm[0][c], mm[1][c]); }
	Vector3<T> column3(size_t c) const { assert(c<height); return Vector3<T>(mm[0][c], mm[1][c], mm[2][c]); }
	Vector4<T> column4(size_t c) const { assert(c<height); return Vector4<T>(mm[0][c], mm[1][c], mm[2][c], mm[3][c]); }

	void set_row(size_t r, Vector2<T> const &v) { assert(r<width); mm[r][0] = v[0]; mm[r][1] = v[1]; mm[r][2] = r == 2 ? 1 : 0; mm[r][3] = r == 3 ? 1 : 0; }
	void set_row(size_t r, Vector3<T> const &v) { assert(r<width); mm[r][0] = v[0]; mm[r][1] = v[1]; mm[r][2] = v[2]; mm[r][3] = r == 3 ? 1 : 0; }
	void set_row(size_t r, Vector4<T> const &v) { assert(r<width); mm[r][0] = v[0]; mm[r][1] = v[1]; mm[r][2] = v[2]; mm[r][3] = v[3]; }

	void set_column(size_t c, Vector2<T> const &v) { assert(c<height); mm[0][c] = v[0]; mm[1][c] = v[1]; mm[2][c] = c == 2 ? 1 : 0; mm[3][c] = c == 3 ? 1 : 0; }
	void set_column(size_t c, Vector3<T> const &v) { assert(c<height); mm[0][c] = v[0]; mm[1][c] = v[1]; mm[2][c] = v[2]; mm[3][c] = c == 3 ? 1 : 0; }
	void set_column(size_t c, Vector4<T> const &v) { assert(c<height); mm[0][c] = v[0]; mm[1][c] = v[1]; mm[2][c] = v[2]; mm[3][c] = v[3]; }

	Vector3<T> scale() const { return Vector3<T>(mm[0][0], mm[1][1], mm[2][2]); }
	Vector3<T> translation() const { return column3(3); }


	// multiplication matrix
	Vector3<T> operator * (Vector3<T> const &v) const
	{
		Vector3<T> r;
		const double inv_w = 1.0 / (mm[3][0] * v.x + mm[3][1] * v.y + mm[3][2] * v.z + mm[3][3]);
		r.x = (mm[0][0] * v.x + mm[0][1] * v.y + mm[0][2] * v.z + mm[0][3]) * inv_w;
		r.y = (mm[1][0] * v.x + mm[1][1] * v.y + mm[1][2] * v.z + mm[1][3]) * inv_w;
		r.z = (mm[2][0] * v.x + mm[2][1] * v.y + mm[2][2] * v.z + mm[2][3]) * inv_w;
		return r;
	}

	Vector4<T> operator * (Vector4<T> const &v) const
	{
		return Vector4<T>(mm[0][0] * v.x + mm[0][1] * v.y + mm[0][2] * v.z + mm[0][3] * v.w,
		                  mm[1][0] * v.x + mm[1][1] * v.y + mm[1][2] * v.z + mm[1][3] * v.w,
		                  mm[2][0] * v.x + mm[2][1] * v.y + mm[2][2] * v.z + mm[2][3] * v.w,
		                  mm[3][0] * v.x + mm[3][1] * v.y + mm[3][2] * v.z + mm[3][3] * v.w);
	}

	Matrix4<T> operator * (Matrix4<T> const &m) const
	{
		// TODO: make this more efficient
		return Matrix4<T>(row4(0).dot(m.column4(0)), row4(0).dot(m.column4(1)), row4(0).dot(m.column4(2)), row4(0).dot(m.column4(3)),
		                  row4(1).dot(m.column4(0)), row4(1).dot(m.column4(1)), row4(1).dot(m.column4(2)), row4(1).dot(m.column4(3)),
		                  row4(2).dot(m.column4(0)), row4(2).dot(m.column4(1)), row4(2).dot(m.column4(2)), row4(2).dot(m.column4(3)),
		                  row4(3).dot(m.column4(0)), row4(3).dot(m.column4(1)), row4(3).dot(m.column4(2)), row4(3).dot(m.column4(3)));
	}

	T & operator () (size_t r, size_t c) { return (*this)[r * width + c]; }
	const T & operator () (size_t r, size_t c) const { return (*this)[r * width + c]; }

	// also provided are:
	//  comparison operators <, >, <=, >= == !=
	//  arithmetic operators +, -, *, /, +=, -=, *=, /= for T
	//  stream operators << >>

	static const size_t size = 16;
	static const size_t width = 4;
	static const size_t height = 4;
	union
	{
		T mm[height][width];
		T raw[size];
	};
};

typedef  Matrix4<float>  Matrix4f;
typedef  Matrix4<double> Matrix4d;
typedef  Matrix4<int>    Matrix4i;

#endif //_MATRIX_H_