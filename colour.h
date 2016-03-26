#ifndef _COLOUR_H_
#define _COLOUR_H_
#pragma once

#include <algorithm>
#include "operators.h"

template <class T> struct ColourBase
	: operators::all_colour_ops<ColourBase<T>, T>
{
	ColourBase() : b(0), g(0), r(0), a(0) {}
	ColourBase(T g) : grey(g) {}
	ColourBase(T R, T G, T B) : b(B), g(G), r(R), a(0xFF) {}
	ColourBase(T R, T G, T B, T A) : b(B), g(G), r(R), a(A) {}
	ColourBase(const ColourBase<T> &c) : b(c.b), g(c.g), r(c.r), a(c.a) {}
	ColourBase(const T *p, size_t bpp) : b(0), g(0), r(0), a(0)
	{
		memcpy(&raw, p, sizeof(T) * std::min(4ul, bpp));
	}

	template <typename T2>
	ColourBase(const ColourBase<T2>& c) : b(c.b), g(c.g), r(c.r), a(c.a) {}

	static const size_t size = 4;

	union
	{
		struct { T b, g, r, a; };
		T grey;
		T raw[size];
	};
};

typedef  ColourBase<unsigned char>  Colour;
typedef  ColourBase<unsigned char>  ColourUC;
typedef  ColourBase<float>          ColourF;
typedef  ColourBase<double>         ColourD;
typedef  ColourBase<int>            ColourI;

#endif //_COLOUR_H_