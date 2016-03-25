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

// requires the equivalent x= operators
template <class T, class U> struct arithmetic
{
	friend T operator + (const T& l, const U& r) { T v = l; v += r; return v; }
	friend T operator - (const T& l, const U& r) { T v = l; v -= r; return v; }
	friend T operator * (const T& l, const U& r) { T v = l; v *= r; return v; }
	friend T operator / (const T& l, const U& r) { T v = l; v /= r; return v; }
};

// uses raw array to provide comparisons
template <class T> struct comparable_for_vector
	: comparable<T>
{
	friend bool operator == (const T& l, const T& r)
	{
		for (unsigned i=0; i<l.size; ++i)
			if (!(is_equal(l.raw[i], r.raw[i])))
				return false;
		return true;
	}
	friend bool operator < (const T& l, const T& r)
	{
		for (unsigned i=0; i<l.size; ++i)
			if (r.raw[i] < l.raw[i])
				return false;
		return true;
	}
};

// uses the raw array to provide operators
template <class T> struct arithmetic_for_vector
	: arithmetic<T, T>
{
	friend T operator += (T &l, const T& r) { for (unsigned i=0; i<l.size; ++i) l.raw[i] += r.raw[i]; return l; }
	friend T operator -= (T &l, const T& r) { for (unsigned i=0; i<l.size; ++i) l.raw[i] -= r.raw[i]; return l; }
	friend T operator *= (T &l, const T& r) { for (unsigned i=0; i<l.size; ++i) l.raw[i] *= r.raw[i]; return l; }
	friend T operator /= (T &l, const T& r) { for (unsigned i=0; i<l.size; ++i) l.raw[i] /= r.raw[i]; return l; }
};

// uses the raw array to provide operators
template <class T, class U> struct arithmetic_for_vector2
	: arithmetic<T, U>
{
	friend T operator += (T &l, const U& r) { for (unsigned i=0; i<l.size; ++i) l.raw[i] += r; return l; }
	friend T operator -= (T &l, const U& r) { for (unsigned i=0; i<l.size; ++i) l.raw[i] -= r; return l; }
	friend T operator *= (T &l, const U& r) { for (unsigned i=0; i<l.size; ++i) l.raw[i] *= r; return l; }
	friend T operator /= (T &l, const U& r) { for (unsigned i=0; i<l.size; ++i) l.raw[i] /= r; return l; }
};

// uses the raw array to provide operators
template <class T, class U> struct subscript_for_vector
{
	U & operator [] (size_t i) { T &v = T_cast(); assert(i < v.size); return v.raw[i]; }
	U & operator () (size_t i) { T &v = T_cast(); assert(i < v.size); return v.raw[i]; }
	const U & operator [] (size_t i) const { const T &v = T_cast(); assert(i < v.size); return v.raw[i]; }
	const U & operator () (size_t i) const { const T &v = T_cast(); assert(i < v.size); return v.raw[i]; }

private:
	T & T_cast() { return static_cast<T &>(*this); }
	const T & T_cast() const { return static_cast<const T &>(*this); }
};

// uses the raw array to provide operators
template <class T> struct streams_for_vector
{
	friend std::istream & operator >> (std::istream& is, T &v)
	{
		char t;
		for (unsigned i=0; i<v.size-1; ++i)
			is >> v.raw[i] >> t;
		is >> v.raw[v.size-1];
		return is;
	}

	friend std::ostream & operator << (std::ostream& os, T const &v)
	{
		for (unsigned i=0; i<v.size-1; ++i)
			os << v.raw[i] << ',';
		os << v.raw[v.size-1];
		return os;
	}
};

// requires the dot product function to be defined
template <class T> struct vector_ops
{
	float squared_distance(const T& r) const { return (r-T_cast()).squared_length(); }
	float distance(const T& r) const { return std::sqrt(squared_distance(r)); }

	float dot(const T& r)  const { const T& l = T_cast(); float s=0; for (unsigned i=0; i<l.size; ++i) s += l.raw[i] * r.raw[i]; return s; }
	float squared_length() const { return dot(T_cast()); }
	float length()         const { return std::sqrt(squared_length()); }
	T     normalize()      const { return normalize(); }
	T &   normalize()            { T &v = T_cast(); v /= v.length(); return v; }
	bool  is_unit()        const { return is_equal(length(), 1); }

private:
	T & T_cast() { return static_cast<T &>(*this); }
	const T & T_cast() const { return static_cast<const T &>(*this); }
};

// provide a helper for all associated vector ops
// requires only < and == to be defined
template <class T, class U> struct all_vector_ops
	: comparable_for_vector<T>
	, arithmetic_for_vector<T>
	, arithmetic_for_vector2<T, float>
	, subscript_for_vector<T, U>
	, streams_for_vector<T>
	, public vector_ops<T>
{};

}

#endif //_OPERATORS_H_