#ifndef _VERTEX_H_
#define _VERTEX_H_
#pragma once

#include "core/core.h"
#include "vector.h"

// template <class T> struct Optional
// {
// 	Optional() : has(false), val(T()) {}
// 	Optional(const T& t) : has(true), val(t) {}
// 	Optional & operator = (const T &t) { has = true; val = t; return *this; }
// 	// Optional & operator = (const Optional &t) { has = true; val = t; }

// 	// type conversion
// 	operator T & () { return val; }
// 	operator T const & () const { return val; }
// 	T & get() { return val; };
// 	T const & get() const {  return val; };
// 	T value() const { return val; };
// 	template <class U> T value_or(U && v) { return has ? val : v; };

// 	// presence tests
// 	explicit operator bool () const { return has; }
// 	bool operator ! () const { return !has; }


// 	void clear() { has = false; val = T(); }

// private:
// 	bool has;
// 	T val;
// };

struct Vertex
{
public:
	Vertex() {}
	Vertex(const Vector3f& p) : point(p) {}
	Vertex(const Vector3f& p, const Colour& c) : point(p), colour(c) {}
	Vertex(const Vector3f& p, const Vector2f& uvs) : point(p), uv(uvs) {}
	Vertex(const Vector3f& p, const Vector3f& norm) : point(p), normal(norm) {}
	Vertex(const Vector3f& p, const Vector2f& uvs, const Vector3f& norm) : point(p), uv(uvs), normal(norm) {}
	Vertex(const Vector3f& p, const Vector2f& uvs, const Vector3f& norm, const Colour& c) : point(p), uv(uvs), normal(norm), colour(c) {}

	// Vertex & set_point(const Vector3f& p) { point = p; }
	// Vertex & set_uv    (const Vector2f& uvs)  { uv = uvs; }
	// Vertex & set_colour(const Vector3f& norm) { normal = norm; }
	// Vertex & set_normal(const Colour& c)      { colour = c; }

	// void clear_uv()     { uv.clear(); }
	// void clear_colour() { colour.clear(); }
	// void clear_normal() { normal.clear(); }

	Vector3f  point;
	Vector2f  uv;
	Vector3f  normal;
	ColourD   colour;


	friend std::ostream & operator << (std::ostream& os, Vertex const &v)
	{
		os << "P:" << v.point << ". UV:" << v.uv << ". N:" << v.normal << ". C:" << v.colour;
		return os;
	}

};

#endif //_VERTEX_H_