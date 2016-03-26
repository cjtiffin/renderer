#ifndef _VECTOR_H_
#define _VECTOR_H_
#pragma once

#include "core/core.h"
#include "operators.h"
#include "angle.h"
#include <cmath>


template <class T> class Vector2
	: public operators::all_vector_ops<Vector2<T>, T>
{
public:
	typedef  Vector2<T>  VType;

	Vector2() : x(0), y(0) {}
	Vector2(T a, T b) : x(a), y(b) {}
	Vector2(VType const &v) : x(v.x), y(v.y) {}

	template <class T2>
	Vector2(const Vector2<T2>& v) : x(v.x), y(v.y) {}

	Angle angle() const { return atan2(y, x); }
	VType rotate(Angle a) const { VType v(*this); return v.rotate(a); }
	VType & rotate(Angle a) { float c = a.cos(), s = a.sin(); T t = x; x = x*c - y*s; y = t*s + y*c; return *this; }

	// also provided are:
	//  comparison operators <, >, <=, >= == !=
	//  arithmetic operators +, -, *, /, +=, -=, *=, /= for vector and float
	//  vector operators dot, squared_length, length, normalize, is_unit, squared_distance, distance
	//  stream operators << >>

	static const size_t size = 2;
	union
	{
		struct { T x, y; };
		struct { T u, v; };
		T raw[size];
	};
};

template <class T> class Vector3
	: public operators::all_vector_ops<Vector3<T>, T>
{
public:
	typedef  Vector3<T>  VType;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(T a, T b, T c) : x(a), y(b), z(c) {}
	Vector3(VType const &v) : x(v.x), y(v.y), z(v.z) {}

	template <class T2>
	Vector3(const Vector3<T2>& v) : x(v.x), y(v.y), z(v.z) {}

	VType cross(const VType& v) const { return VType(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

	// also provided are:
	//  comparison operators <, >, <=, >= == !=
	//  arithmetic operators +, -, *, /, +=, -=, *=, /= for vector and float
	//  vector operators dot, squared_length, length, normalize, is_unit, squared_distance, distance
	//  stream operators << >>

	static const size_t size = 3;
	union
	{
		struct { T x, y, z; };
		T raw[size];
	};
};

template <class T> class Vector4
	: public operators::all_vector_ops<Vector4<T>, T>
{
public:
	typedef  Vector4<T>  VType;

	Vector4() : x(0), y(0), z(0), w(0) {}
	Vector4(T a, T b, T c, T d) : x(a), y(b), z(c), w(d) {}
	Vector4(VType const &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	template <class T2>
	Vector4(const Vector4<T2>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	// also provided are:
	//  comparison operators <, >, <=, >= == !=
	//  arithmetic operators +, -, *, /, +=, -=, *=, /= for vector and float
	//  vector operators dot, squared_length, length, normalize, is_unit, squared_distance, distance
	//  stream operators << >>

	static const size_t size = 4;
	union
	{
		struct { T x, y, z, w; };
		T raw[size];
	};
};

typedef  Vector2<float>  Vector2f;
typedef  Vector2<int>    Vector2i;
typedef  Vector3<float>  Vector3f;
typedef  Vector3<int>    Vector3i;
typedef  Vector4<float>  Vector4f;
typedef  Vector4<int>    Vector4i;

#endif //_VECTOR_H_