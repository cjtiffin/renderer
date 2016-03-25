#ifndef _ZBUFFER_H_
#define _ZBUFFER_H_
#pragma once

#include "core/assert.h"

class ZBuffer
{
public:
	typedef  double  zbuf_type;

	ZBuffer(size_t w, size_t h)
		: width(w), height(h), buffer(NULL)
	{
		size_t size = width * height;
		buffer = new zbuf_type[size];
		memset(buffer, 0, sizeof(zbuf_type) * size);
	}
	~ZBuffer()
	{
		delete buffer; buffer = NULL;
	}

	bool test(int x, int y, zbuf_type z)
	{
		zbuf_type &px = get_pixel(x, y);
		if (z <= px)
			return false;
		px = z;
		return true;
	}

	zbuf_type &get_pixel(size_t x, size_t y)
	{
		assert(x < width && y < height);
		return buffer[y * width + x];
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
	size_t width, height;
	zbuf_type *buffer;
};

#endif //_ZBUFFER_H_