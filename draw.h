#ifndef _DRAW_H_
#define _DRAW_H_
#pragma once

#include <iostream>
#include "tgaimage.h"
#include "vector.h"
#include "vertex.h"
#include "zbuffer.h"


// naive line drawing algorithm
void line(int x0, int y0, int z0, int x1, int y1, int z1, ZBuffer &zbuf, TGAImage &image, Colour color)
{
	// figure out the primary axis
	const bool xprimary = std::abs(x0 - x1) > std::abs(y0 - y1);

	// adjust so we're drawing from left to right along the primary axis
	if (!xprimary)
	{
		if (y0 > y1)
		{
			std::swap(x0, y1);
			std::swap(x1, y0);
			std::swap(z0, z1);
		}
		else
		{
			std::swap(x0, y0);
			std::swap(x1, y1);
		}
	}
	else if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
		std::swap(z0, z1);
	}

	// take the gradient of the line as our secondary axis step
	const double xmul = 1.0 / (x1 - x0);
	const double ystep = double(y1 - y0) * xmul;
	const double zstep = double(z1 - z0) * xmul;
	double y = y0, z = z0;

	// move along the primary axis one pixel at a time, and along the 
	// secondary axis by "ystep" at a time
	for (unsigned x = x0; x < x1; ++x)
	{
		if (xprimary)
		{
			if (zbuf.test(x, y, z))
				image.set(x, y, color);
		}
		else
		{
			if (zbuf.test(y, x, z))
				image.set(y, x, color);
		}
		y += ystep;
		z += zstep;
	}
}

// allow line drawing given two points
template <typename T>
void line(const Vector3<T> &start, const Vector3<T> &end, ZBuffer &zbuf, TGAImage &image, Colour color)
{
	line(start.x, start.y, start.z, end.x, end.y, end.z, zbuf, image, color);
}

// scanline (horizontal line) drawing
inline void scan_line(int x0, int x1, int y, float z0, float z1, ColourD c0, ColourD c1, ZBuffer &zbuf, TGAImage &image)
{
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(z0, z1);
	}

	const double xmul = 1.0 / (x1 - x0);
	const double zstep = double(z1 - z0) * xmul;
	const ColourD cstep = ColourD(c1 - c0) * xmul;
	double z = z0;
	ColourD c = c0;

	for (unsigned x = x0; x < x1; ++x)
	{
		if (zbuf.test(x, y, z))
			image.set(x, y, c);
		z += zstep;
		c += cstep;
	}
}

// naive triangle drawing algorithm
void triangle(Vertex v0, Vertex v1, Vertex v2, ZBuffer &zbuf, TGAImage &image)
{
	// sort y ascending, then x ascending
	if (v0.point.y > v1.point.y || (is_equal(v0.point.y, v1.point.y) && v0.point.x > v1.point.x)) std::swap(v0, v1);
	if (v1.point.y > v2.point.y || (is_equal(v1.point.y, v2.point.y) && v1.point.x > v2.point.x)) std::swap(v1, v2);
	if (v0.point.y > v1.point.y || (is_equal(v0.point.y, v1.point.y) && v0.point.x > v1.point.x)) std::swap(v0, v1);

	// precalculate what we can
	const double y01mul = 1.0 / double(v1.point.y - v0.point.y),
	             y02mul = 1.0 / double(v2.point.y - v0.point.y),
	             y12mul = 1.0 / double(v2.point.y - v1.point.y);

	// take the gradient of the lines as our x steps
	const double x01step = double(v1.point.x - v0.point.x) * y01mul,
	             x02step = double(v2.point.x - v0.point.x) * y02mul,
	             x12step = double(v2.point.x - v1.point.x) * y12mul,
	             z01step = double(v1.point.z - v0.point.z) * y01mul,
 	             z02step = double(v2.point.z - v0.point.z) * y02mul,
 	             z12step = double(v2.point.z - v1.point.z) * y12mul;

 	ColourD c01step = ColourD(v1.colour - v0.colour) * y01mul,
	        c02step = ColourD(v2.colour - v0.colour) * y02mul,
	        c12step = ColourD(v2.colour - v1.colour) * y12mul;

	double x0 = v0.point.x, x1 = v0.point.x,
	       z0 = v0.point.z, z1 = v0.point.z;
	ColourD c0 = v0.colour, c1 = v0.colour;

	// flip if we need to
	if (v0.point.x < v1.point.x)
		std::swap(c01step, c02step);

	// work our way up the first half of the triangle
	for (int y = v0.point.y; y < v1.point.y; ++y)
	{
		scan_line(x0, x1, y, z0, z1, c0, c1, zbuf, image);
		x0 += x01step; x1 += x02step;
		z0 += z01step; z1 += z02step;
		c0 += c01step; c1 += c02step;
	}


	// flip if we need to
	if (v0.point.x < v1.point.x)
	{
		std::swap(c01step, c02step);
		std::swap(c02step, c12step);
	}

	x0 = v1.point.x; z0 = v1.point.z;
	if (is_equal(v0.point.y, v1.point.y) || is_equal(v0.point.y, v2.point.y))
		c1 = v1.colour;

	// make our way up the second half of the triangle
	for (int y = v1.point.y; y < v2.point.y; ++y)
	{
		scan_line(x0, x1, y, z0, z1, c0, c1, zbuf, image);
		x0 += x12step; x1 += x02step;
		z0 += z12step; z1 += z02step;
		c0 += c12step; c1 += c02step;
	}
}

// // allow triangle drawing given three points
// template <typename T>
// void triangle(Vector3<T> v0, Vector3<T> v1, Vector3<T> v2, TGAImage &image, Colour color)
// {
// 	triangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, image, color);
// }


#endif //_DRAW_H_