#ifndef _COLOUR_H_
#define _COLOUR_H_
#pragma once

#include "core/core.h"
#include <algorithm>
#include <limits>
#include "operators.h"

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}

template <class T> struct ColourBase
	: operators::all_colour_ops<ColourBase<T>, T>
{
	ColourBase() : b(0), g(0), r(0), a(0) {}
	ColourBase(T R, T G, T B) : b(B), g(G), r(R), a(0xFF) {}
	ColourBase(T R, T G, T B, T A) : b(B), g(G), r(R), a(A) {}
	ColourBase(const ColourBase<T> &c) : b(c.b), g(c.g), r(c.r), a(c.a) {}
	ColourBase(const T *p, size_t bpp) : b(0), g(0), r(0), a(0)
	{
		memcpy(&raw, p, sizeof(T) * std::min(4ul, bpp));
	}

	// ensure conversions between floating and integer types are handled correctly
	template <typename T2>
	ColourBase(const T2& o) { b = g = r = convert<T2>(o); a = convert<double>(1.0); }
	template <typename T2>
	ColourBase(const ColourBase<T2>& c) : b(convert<T2>(c.b)), g(convert<T2>(c.g)), r(convert<T2>(c.r)), a(convert<T2>(c.a)) {}

	ColourBase & clamp() { r = ::clamp(r, 0.0, 1.0); g = ::clamp(g, 0.0, 1.0); b = ::clamp(b, 0.0, 1.0); a = ::clamp(a, 0.0, 1.0); return *this; }


	static const size_t size = 4;

	union
	{
		struct { T b, g, r, a; };
		T grey;
		T raw[size];
	};

	template <typename T2> T convert(T2 const &t) { return convert(t, typename std::is_integral<T>::type(), typename std::is_integral<T2>::type()); }
	template <typename T2> T convert(T2 const &t, std::true_type, std::true_type)   { return double(t) / std::numeric_limits<T2>::max() * std::numeric_limits<T>::max(); } // from int to int
	template <typename T2> T convert(T2 const &t, std::true_type, std::false_type)  { return t * std::numeric_limits<T>::max(); } // from float to int 
	template <typename T2> T convert(T2 const &t, std::false_type, std::true_type)  { return T(t) / std::numeric_limits<T2>::max(); } // from int to float
	template <typename T2> T convert(T2 const &t, std::false_type, std::false_type) { return t; } // from float to float
};

typedef  ColourBase<unsigned char>  Colour;
typedef  ColourBase<unsigned char>  ColourUC;
typedef  ColourBase<int>            ColourI;
typedef  ColourBase<float>          ColourF;
typedef  ColourBase<double>         ColourD;

namespace Colours {
const Colour white = Colour(255, 255, 255, 255);
const Colour red   = Colour(255, 0,   0,   255);
const Colour green = Colour(0,   255, 0,   255);
const Colour blue  = Colour(0,   0,   255, 255);
const Colour black = Colour(0,   0,   0,   255);
}

#endif //_COLOUR_H_