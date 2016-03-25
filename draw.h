#ifndef _DRAW_H_
#define _DRAW_H_
#pragma once

#include <iostream>
#include "tgaimage.h"
#include "vector.h"


// naive line drawing algorithm
void line(int x0, int y0, int x1, int y1, TGAImage &image, Colour color)
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
	}

	// take the gradient of the line as our secondary axis step
	const double ystep = double(y1 - y0) / double(x1 - x0);
	double y = y0;

	// move along the primary axis one pixel at a time, and along the 
	// secondary axis by "ystep" at a time
	for (unsigned x = x0; x < x1; ++x)
	{
		if (xprimary)
			image.set(x, y, color);
		else
			image.set(y, x, color);
		y += ystep;
	}
}

// allow line drawing given two points
template <typename T>
void line(const Vector2<T> &start, const Vector2<T> &end, TGAImage &image, Colour color)
{
	line(start.x, start.y, end.x, end.y, image, color);
}

// scanline (horizontal line) drawing
inline void scan_line(int x0, int x1, int y, TGAImage &image, Colour color)
{
	if (x0 > x1) std::swap(x0, x1);
	for (unsigned x = x0; x < x1; ++x)
		image.set(x, y, color);
}

// naive triangle drawing algorithm
template <typename T>
void triangle(Vector2<T> v0, Vector2<T> v1, Vector2<T> v2, TGAImage &image, Colour color)
{
	// sort y ascending
	if (v0.y > v1.y) std::swap(v0, v1);
	if (v1.y > v2.y) std::swap(v1, v2);
	if (v0.y > v1.y) std::swap(v0, v1);

	// take the gradient of the lines as our x steps
	const double v0v1step = double(v1.x - v0.x) / double(v1.y - v0.y),
	             v0v2step = double(v2.x - v0.x) / double(v2.y - v0.y),
	             v1v2step = double(v2.x - v1.x) / double(v2.y - v1.y);

	// work our way up the first half of the triangle
	double x0 = v0.x, x1 = v0.x;
	for (int y = v0.y; y < v1.y; ++y)
	{
		scan_line(x0, x1, y, image, color);
		x0 += v0v1step; x1 += v0v2step;
	}

	// make our way up the second half of the triangle
	x0 = v1.x;
	for (int y = v1.y; y < v2.y; ++y)
	{
		scan_line(x0, x1, y, image, color);
		x0 += v1v2step; x1 += v0v2step;
	}
}

// // allow triangle drawing given three points
// template <typename T>
// void triangle(Vector2<T> v0, Vector2<T> v1, Vector2<T> v2, TGAImage &image, Colour color)
// {
// 	triangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, image, color);
// }


#endif //_DRAW_H_