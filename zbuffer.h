#ifndef _ZBUFFER_H_
#define _ZBUFFER_H_
#pragma once

#include "core/core.h"
#include <vector>


class ZBuffer
{
public:
	typedef  double  zbuf_type;

	ZBuffer(size_t w, size_t h)
		: width(w), height(h)
	{
		size_t size = width * height;
		buffer.resize(size, 0);
	}

	bool test(size_t x, size_t y, zbuf_type z)
	{
		if (x >= width || y >= height)
			return false;

		zbuf_type &px = get_pixel(x, y);
		if (z <= px)
			return false;

		px = z;
		return true;
	}

	void output(const char *filename)
	{
		TGAImage image(width, height, TGAImage::GRAYSCALE);

		for (unsigned y = 0; y < height; ++y)
			for (unsigned x = 0; x < width; ++x)
				image.set(x, y, Colour(unsigned(get_pixel(x, y) * 255.0)));

		image.flip_vertically();
		image.write_tga_file(filename);
	}

private:
	zbuf_type &get_pixel(size_t x, size_t y)
	{
		assert(x < width && y < height);
		return buffer[y * width + x];
	}

	size_t width, height;
	std::vector<zbuf_type> buffer;
};

#endif //_ZBUFFER_H_