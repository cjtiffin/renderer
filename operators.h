#ifndef _OPERATORS_H_
#define _OPERATORS_H_
#pragma once

#include <cmath>
#include <iostream>

template<class T, class U> bool is_equal(const T& l, const U& r, std::true_type) { return l == r; }
template<class T, class U> bool is_equal(const T& l, const U& r, std::false_type) { return std::abs(r - l) <= 0.0001; }
template<class T, class U> bool is_equal(const T& l, const U& r) { return is_equal(l, r, typename std::is_integral<T>::type()); }

namespace operators
{
// requires the < and = operators
template <class T> struct comparable
{
	friend bool operator > (const T& l, const T& r) { return r < l; }
	friend bool operator <= (const T& l, const T& r) { return !(r < l); }
	friend bool operator >= (const T& l, const T& r) { return !(l < r); }
	friend bool operator != (const T& l, const T& r) { return !(l == r); }
};

// uses subscript op and .size to provide comparisons
template <class T> struct comparable_raw_arr
	: comparable<T>
{
	friend bool operator == (const T& l, const T& r)
	{
		for (unsigned i=0; i<l.size; ++i)
			if (!(is_equal(l[i], r[i])))
				return false;
		return true;
	}
	friend bool operator < (const T& l, const T& r)
	{
		for (unsigned i=0; i<l.size; ++i)
			if (r[i] < l[i])
				return false;
		return true;
	}
};

// requires the equivalent x= operators
template <class T, class U> struct arithmetic_add { friend T operator + (const T& l, const U& r) { T v = l; v += r; return v; } };
template <class T, class U> struct arithmetic_sub { friend T operator - (const T& l, const U& r) { T v = l; v -= r; return v; } };
template <class T, class U> struct arithmetic_mul { friend T operator * (const T& l, const U& r) { T v = l; v *= r; return v; } };
template <class T, class U> struct arithmetic_div { friend T operator / (const T& l, const U& r) { T v = l; v /= r; return v; } };
template <class T, class U> struct arithmetic_all : arithmetic_add<T, U>, arithmetic_sub<T, U>, arithmetic_mul<T, U>, arithmetic_div<T, U> {};

// uses the subscript op and .size to provide operators
template <class T> struct arithmetic_add_raw_arr : arithmetic_add<T, T> { friend T operator += (T &l, const T& r) { for (size_t i=0; i<l.size; ++i) l[i] += r[i]; return l; } };
template <class T> struct arithmetic_sub_raw_arr : arithmetic_sub<T, T> { friend T operator -= (T &l, const T& r) { for (size_t i=0; i<l.size; ++i) l[i] -= r[i]; return l; } };
template <class T> struct arithmetic_mul_raw_arr : arithmetic_mul<T, T> { friend T operator *= (T &l, const T& r) { for (size_t i=0; i<l.size; ++i) l[i] *= r[i]; return l; } };
template <class T> struct arithmetic_div_raw_arr : arithmetic_div<T, T> { friend T operator /= (T &l, const T& r) { for (size_t i=0; i<l.size; ++i) l[i] /= r[i]; return l; } };
template <class T> struct arithmetic_all_raw_arr : arithmetic_add_raw_arr<T>, arithmetic_sub_raw_arr<T>, arithmetic_mul_raw_arr<T>, arithmetic_div_raw_arr<T> {};

// uses the subscript op and .size to provide operators
template <class T, class U> struct arithmetic_add_raw_arr2 : arithmetic_add<T, U> { friend T operator += (T &l, const U& r) { for (size_t i=0; i<l.size; ++i) l[i] += r; return l; } };
template <class T, class U> struct arithmetic_sub_raw_arr2 : arithmetic_sub<T, U> { friend T operator -= (T &l, const U& r) { for (size_t i=0; i<l.size; ++i) l[i] -= r; return l; } };
template <class T, class U> struct arithmetic_mul_raw_arr2 : arithmetic_mul<T, U> { friend T operator *= (T &l, const U& r) { for (size_t i=0; i<l.size; ++i) l[i] *= r; return l; } };
template <class T, class U> struct arithmetic_div_raw_arr2 : arithmetic_div<T, U> { friend T operator /= (T &l, const U& r) { for (size_t i=0; i<l.size; ++i) l[i] /= r; return l; } };
template <class T, class U> struct arithmetic_all_raw_arr2 : arithmetic_add_raw_arr2<T, U>, arithmetic_sub_raw_arr2<T, U>, arithmetic_mul_raw_arr2<T, U>, arithmetic_div_raw_arr2<T, U> {};


// uses the subscript op and .size to provide operators
template <class T, class U> struct subscript_raw_arr
{
	U & operator [] (size_t i) { T &v = T_cast(); assert(i<v.size); return v.raw[i]; }
	U & operator () (size_t i) { T &v = T_cast(); assert(i<v.size); return v.raw[i]; }
	const U & operator [] (size_t i) const { const T &v = T_ccast(); assert(i<v.size); return v.raw[i]; }
	const U & operator () (size_t i) const { const T &v = T_ccast(); assert(i<v.size); return v.raw[i]; }

private:
	T & T_cast() { return static_cast<T &>(*this); }
	const T & T_ccast() const { return static_cast<const T &>(*this); }
};

// uses the subscript op and .size to provide operators
template <class T> struct streams_raw_arr
{
	friend std::istream & operator >> (std::istream& is, T &v)
	{
		char t;
		for (unsigned i=0; i<v.size-1 && is.good(); ++i)
			is >> v[i] >> t;
		if (is.good())
			is >> v[v.size-1];
		return is;
	}

	friend std::ostream & operator << (std::ostream& os, T const &v)
	{
		for (unsigned i=0; i<v.size-1; ++i)
			os << static_cast<double>(v[i]) << ',';
		os << static_cast<double>(v[v.size-1]);
		return os;
	}
};

// requires the dot product function to be defined
template <class T> struct vector_ops
{
	float squared_distance(const T& r) const { return (r-T_ccast()).squared_length(); }
	float distance(const T& r) const { return std::sqrt(squared_distance(r)); }

	float dot(const T& r)  const { const T& l = T_ccast(); float s=0; for (unsigned i=0; i<l.size; ++i) s += l[i] * r[i]; return s; }
	float squared_length() const { return dot(T_ccast()); }
	float length()         const { return std::sqrt(squared_length()); }
	T     normalize()      const { return T(T_ccast()).normalize(); }
	T &   normalize()            { T &v = T_cast(); v /= v.length(); return v; }
	bool  is_unit()        const { return is_equal(length(), 1); }

private:
	T & T_cast() { return static_cast<T &>(*this); }
	const T & T_ccast() const { return static_cast<const T &>(*this); }
};

// provide a helper for all associated vector ops
template <class T, class U> struct all_vector_ops
	: comparable_raw_arr<T>
	, arithmetic_add_raw_arr<T>
	, arithmetic_sub_raw_arr<T>
	, arithmetic_all_raw_arr2<T, float>
	, subscript_raw_arr<T, U>
	, streams_raw_arr<T>
	, public vector_ops<T>
{};

// provide a helper for all associated matrix ops
template <class T, class U> struct all_matrix_ops
	: comparable_raw_arr<T>
	, arithmetic_add_raw_arr<T>
	, arithmetic_sub_raw_arr<T>
	, arithmetic_all_raw_arr2<T, U>
	, subscript_raw_arr<T, U>
	, streams_raw_arr<T>
{};

// provide a helper for all associated colour ops
template <class T, class U> struct all_colour_ops
	: comparable_raw_arr<T>
	, arithmetic_all_raw_arr<T>
	, arithmetic_all_raw_arr2<T, float>
	, subscript_raw_arr<T, U>
	, streams_raw_arr<T>
{};

}

#endif //_OPERATORS_H_