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
		buffer.resize(size, std::numeric_limits<zbuf_type>::lowest());
	}

	virtual bool test(size_t x, size_t y, zbuf_type z)
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
		PROFILE_SCOPED_FN

		TGAImage image(width, height, TGAImage::GRAYSCALE);

		zbuf_type min = std::numeric_limits<zbuf_type>::max(),
		          max = std::numeric_limits<zbuf_type>::lowest();

		// normalize between min and max
		for (unsigned y = 0; y < height; ++y)
			for (unsigned x = 0; x < width; ++x)
			{
				zbuf_type const &px = get_pixel(x, y);
				if (px <= std::numeric_limits<zbuf_type>::lowest())
					continue;
				if (px < min)
					min = px;
				if (px > max)
					max = px;
			}

		double mm = max - min;
		for (unsigned y = 0; y < height; ++y)
			for (unsigned x = 0; x < width; ++x)
				image.set(x, y, (get_pixel(x, y) - min) / mm);

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


class ZBuffer_AlwaysAllow
	: public ZBuffer
{
public:
	ZBuffer_AlwaysAllow() : ZBuffer(0, 0) {}
	inline bool test(size_t, size_t, ZBuffer::zbuf_type) { return true; }
};


#endif //_ZBUFFER_H_