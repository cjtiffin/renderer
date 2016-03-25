#ifndef _COLOUR_H_
#define _COLOUR_H_
#pragma once

#include <algorithm>

struct Colour
{
	Colour() : val(0) {}
	Colour(unsigned char g) : grey(g) {}
	Colour(unsigned char R, unsigned char G, unsigned char B) : b(B), g(G), r(R), a(0xFF) {}
	Colour(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A) {}
	Colour(unsigned int v) : val(v) {}
	Colour(const Colour &c) : val(c.val) {}
	Colour(const unsigned char *p, int bpp) : val(0)
	{
		memcpy(&raw, p, sizeof(unsigned char) * std::min(4, bpp));
	}

	Colour & operator =(const Colour &c)
	{
		if (this != &c)
			val = c.val;
		return *this;
	}

	union
	{
		struct { unsigned char b, g, r, a; };
		unsigned char grey;
		unsigned char raw[4];
		unsigned int val;
	};
};

#endif //_COLOUR_H_