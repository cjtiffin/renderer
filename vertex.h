#ifndef _VERTEX_H_
#define _VERTEX_H_
#pragma once

#include "vector.h"

// template <class T> struct Optional
// {
// 	Optional() : has(false), value(T()) {}
// 	Optional(const T& t) : has(true), value(t) {}

// 	Optional<T> & operator = (const T &t) { has = true; value = t; }

// 	operator bool () const { return has; }
// 	bool operator ! () const { return !has; }

// 	T &       get()       { return has ? value : T(); };
// 	T const & get() const { return has ? value : T(); };

// 	void clear() { has = false; }

// private:
// 	bool has;
// 	T value;
// };

struct Vertex
{
public:
	Vertex() {}
	Vertex(const Vector3f& p) : point(p) {}
	Vertex(const Vector3f& p, const Colour& c) : point(p), colour(c) {}
	Vertex(const Vector3f& p, const Vector2i& uvs) : point(p), uv(uvs) {}
	Vertex(const Vector3f& p, const Vector3f& norm) : point(p), normal(norm) {}
	Vertex(const Vector3f& p, const Vector2i& uvs, const Vector3f& norm) : point(p), uv(uvs), normal(norm) {}
	Vertex(const Vector3f& p, const Vector2i& uvs, const Vector3f& norm, const Colour& c) : point(p), uv(uvs), normal(norm), colour(c) {}

	// Vertex & set_point(const Vector3f& p) { point = p; }
	// Vertex & set_uv    (const Vector2i& uvs)  { uv = uvs; }
	// Vertex & set_colour(const Vector3f& norm) { normal = norm; }
	// Vertex & set_normal(const Colour& c)      { colour = c; }

	// void clear_uv()     { uv.clear(); }
	// void clear_colour() { colour.clear(); }
	// void clear_normal() { normal.clear(); }

	Vector3f  point;
	Vector2i  uv;
	Vector3f  normal;
	ColourD   colour;

};

#endif //_VERTEX_H_