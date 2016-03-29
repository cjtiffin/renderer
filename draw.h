#ifndef _DRAW_H_
#define _DRAW_H_
#pragma once

#include <iostream>
#include "tgaimage.h"
#include "vector.h"
#include "vertex.h"
#include "zbuffer.h"


// naive line drawing algorithm
void line(int x0, int y0, int z0, ColourD c0,
          int x1, int y1, int z1, ColourD c1,
          ZBuffer &zbuf, TGAImage &image)
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
			std::swap(c0, c1);
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
		std::swap(c0, c1);
	}

	// take the gradient of the line as our secondary axis step
	const double xmul = 1.0 / (x1 - x0);
	const double ystep = double(y1 - y0) * xmul;
	const double zstep = double(z1 - z0) * xmul;
	const ColourD cstep = ColourD(c1 - c0) * xmul;

	double y = y0, z = z0;
	ColourD c = c0;

	// move along the primary axis one pixel at a time, and along the 
	// secondary axis by "ystep" at a time
	for (unsigned x = x0; x < x1; ++x)
	{
		if (xprimary)
		{
			if (zbuf.test(x, y, z))
				image.set(x, y, c);
		}
		else
		{
			if (zbuf.test(y, x, z))
				image.set(y, x, c);
		}
		y += ystep;
		z += zstep;
		c += cstep;
	}
}

// allow line drawing given two points
void line(const Vertex &start, const Vertex &end, ZBuffer &zbuf, TGAImage &image)
{

	line(start.point.x, start.point.y, start.point.z, start.colour,
	     end.point.x, end.point.y, end.point.z, end.colour,
	     zbuf, image);
}

// scanline (horizontal line) drawing
inline void scan_line(int y,
                      int x0, double z0, ColourD c0, Vector2d uv0,
                      int x1, double z1, ColourD c1, Vector2d uv1,
                      ZBuffer &zbuf, TGAImage &image, TGAImage *tex = NULL)
{
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(z0, z1);
		std::swap(c0, c1);
		std::swap(uv0, uv1);
	}

	const double xmul = 1.0 / (x1 - x0);
	const double zstep = double(z1 - z0) * xmul;
	const ColourD cstep = ColourD(c1 - c0) * xmul;
	const Vector2d uvstep = Vector2d(uv1 - uv0) * xmul;

	double z = z0;
	ColourD c = c0;
	Vector2d uv = uv0;

	for (unsigned x = x0; x < x1; ++x)
	{
		if (zbuf.test(x, y, z))
			image.set(x, y, tex ? Colour((ColourD(tex->get_from_uv(uv.u, uv.v)) * c).clamp()) : Colour(c));
		z += zstep;
		c += cstep;
		uv += uvstep;
	}
}

// naive triangle drawing algorithm
void triangle(Vertex v0, Vertex v1, Vertex v2, ZBuffer &zbuf, TGAImage &image, TGAImage *tex = NULL)
{
	// sort y ascending, then x ascending
	if (v0.point.y > v1.point.y) std::swap(v0, v1);
	if (v1.point.y > v2.point.y) std::swap(v1, v2);
	if (v0.point.y > v1.point.y) std::swap(v0, v1);

	// precalculate what we can
	const double y01mul = 1.0 / double(v1.point.y - v0.point.y),
	             y02mul = 1.0 / double(v2.point.y - v0.point.y),
	             y12mul = 1.0 / double(v2.point.y - v1.point.y);

	// take the gradient of the lines as our x steps
	Vector3d v01step = (v1.point - v0.point) * y01mul,
	         v02step = (v2.point - v0.point) * y02mul,
	         v12step = (v2.point - v1.point) * y12mul;

 	ColourD c01step = ColourD(v1.colour - v0.colour) * y01mul,
	        c02step = ColourD(v2.colour - v0.colour) * y02mul,
	        c12step = ColourD(v2.colour - v1.colour) * y12mul;
  
	Vector2d uv01step = (v1.uv - v0.uv) * y01mul,
	         uv02step = (v2.uv - v0.uv) * y02mul,
	         uv12step = (v2.uv - v1.uv) * y12mul;

	// scan_line handles correcting left-right ordering for us

	double x0 = v0.point.x, x1 = v0.point.x,
	       z0 = v0.point.z, z1 = v0.point.z;
	ColourD c0 = v0.colour, c1 = v0.colour;
	Vector2d uv0 = v0.uv, uv1 = v0.uv;

	// work our way up the first half of the triangle
	for (int y = v0.point.y; y < v1.point.y; ++y)
	{
		scan_line(y,  x0, z0, c0, uv0,  x1, z1, c1, uv1,  zbuf, image, tex);

		x0 += v01step.x; x1 += v02step.x;
		z0 += v01step.z; z1 += v02step.z;
		c0 += c01step; c1 += c02step;
		uv0 += uv01step; uv1 += uv02step;
	}


	x0 = v1.point.x; z0 = v1.point.z;
	c0 = v1.colour;
	uv0 = v1.uv;

	// make our way up the second half of the triangle
	for (int y = v1.point.y; y < v2.point.y; ++y)
	{
		scan_line(y,  x0, z0, c0, uv0,  x1, z1, c1, uv1,  zbuf, image, tex);

		x0 += v12step.x; x1 += v02step.x;
		z0 += v12step.z; z1 += v02step.z;
		c0 += c12step; c1 += c02step;
		uv0 += uv12step; uv1 += uv02step;
	}
}

#endif //_DRAW_H_