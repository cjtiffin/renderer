#include <utility>
#include <cmath>

#include "core/core.h"
#include "colour.h"
#include "tgaimage.h"
#include "model.h"
#include "vector.h"
#include "draw.h"

// https://github.com/ssloy/tinyrenderer/wiki/

using namespace Colours;

template <typename T>
T lerp(T start, T end, double percent)
{
	return start + percent * (end - start);
}

template <typename T>
bool half_space_test(int x, int y, Vector2<T> v0, Vector2<T> v1)
{
	return (v0.x - v1.x) * (y - v0.y) - (v0.y - v1.y) * (x - v0.x) > 0;
}

float randfp()
{
	return float(rand()) / RAND_MAX;
}

int main(int argc, char** argv)
{
	// PROFILE_SCOPED_FN

	TGAImage *texture = NULL;

#if 0
	const float width_offset = 1.0, width_scale = 2.0,
	            height_offset = 0.2, height_scale = 2.3;
	Model model("Models/BAC_Batman70s_rocksteady/batman.obj");
#elif 0
	// this doesn't work as quads aren't supported
	const float width_offset = 1.0, width_scale = 2.0,
	            height_offset = 0.2, height_scale = 2.3;
	Model model("Models/Harley/Harley.obj");
#elif 0
	const float width_offset = 0.5, width_scale = 1.0,
	            height_offset = 0.05, height_scale = 1.0;
	Model model("Models/cat/cat.obj");
#elif 1
	const float width_offset = 1.0, width_scale = 2.0,
	            height_offset = 1.0, height_scale = 2.0;
	Model model("Models/Head/african_head.obj");
	TGAImage head = TGAImage("Models/Head/african_head_diffuse.tga");
	texture = &head;
#endif

	const unsigned width = 1000, height = 1000;
	TGAImage image(width, height, TGAImage::RGB);
	ZBuffer zbuf(width, height);

// #define UT_TESTS_ONLY
// #define UNIT_TEST_VERBOSE
// #include "unittests/unittest_vector.h"
// #include "unittests/unittest_colour.h"
// #define UT_WIREFRAME
// #include "unittests/unittest_colouredtriangles.h"
// #include "unittests/unittest_lines.h"


#ifndef UT_TESTS_ONLY
	// 3D Drawing
	const Vector3f view(0, 0, -1);
	Vector3f light(-1, -1, -3); light.normalize();

	// shaded render
	for (int i = 0; i < model.nfaces(); ++i)
	{
		Vector3f world_coords[3];
		Vertex screen_coords[3];

		std::vector<int> const &face = model.face(i);
		std::vector<int> const &face_uv = model.face_uv(i);
		for (int j = 0; j < 3; ++j)
		{
			assert(face.size() >= 3);
			assert(face_uv.size() >= 3);

			Vector3f const &vert = model.vert(face[j]);
			screen_coords[j].point = Vector3f(int((vert.x + width_offset) * width / width_scale),
			                                  int((vert.y + height_offset) * height / height_scale),
			                                      (vert.z + 1.0) / 2.0);
			screen_coords[j].uv = model.vert_uv(face_uv[j]);
			world_coords[j] = vert;
		}

		Vector3f tri_normal = (world_coords[2]-world_coords[0]).cross(world_coords[1]-world_coords[0]).normalize();

		// back face occlusion - test for dot product being positive
		if (view.dot(tri_normal) > 0)
		{
			const bool SMOOTH_SHADED = true;
			if (SMOOTH_SHADED)
			{
				// calculate the light intensity at each normal
				std::vector<int> const &face_norm = model.face_norm(i);
				screen_coords[0].colour = ColourD(-light.dot(model.vert_norm(face_norm[0]))).clamp();
				screen_coords[1].colour = ColourD(-light.dot(model.vert_norm(face_norm[1]))).clamp();
				screen_coords[2].colour = ColourD(-light.dot(model.vert_norm(face_norm[2]))).clamp();
			}
			else
				// just use the face's light intensity
				screen_coords[0].colour = screen_coords[1].colour = screen_coords[2].colour = ColourD(light.dot(tri_normal)).clamp();

			triangle(screen_coords[0], screen_coords[1], screen_coords[2], zbuf, image, texture);
		}
	}

	// // 2D Drawing
	// for (int i = 0; i < model.nfaces(); ++i)
	// {
	// 	std::vector<int> face = model.face(i);
	// 	Vector2i screen_coords[3];
	// 	for (int j = 0; j < 3; ++j)
	// 	{
	// 		Vector3f world_coords = model.vert(face[j]);
	// 		screen_coords[j] = Vector2i((world_coords.x + 1.0) * width / 2.0, (world_coords.y + 1.0) * height / 2.0);
	// 	}
	// 	triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, Colour(rand() % 255, rand() % 255, rand() % 255, 255));
	// }

	// // wireframe render
	// for (int i = 0; i < model.nfaces(); ++i)
	// {
	// 	std::vector<int> const &face = model.face(i);
	// 	for (int j = 0; j < 3; ++j)
	// 	{
	// 		Vector3f const &v0 = model.vert(face[j]);
	// 		Vector3f const &v1 = model.vert(face[(j + 1) % 3]);
	// 		int x0 = (v0.x + width_offset) * width / width_scale;
	// 		int y0 = (v0.y + height_offset) * height / height_scale;
	// 		int x1 = (v1.x + width_offset) * width / width_scale;
	// 		int y1 = (v1.y + height_offset) * height / height_scale;
	// 		line(x0, y0, x1, y1, image, Colours::white);
	// 	}
	// }
#endif

	image.flip_vertically();
	image.write_tga_file("output.tga");
	// zbuf.output("zbuf.tga");
	return 0;
}