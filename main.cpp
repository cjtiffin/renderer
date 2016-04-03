#include <utility>
#include <cmath>

#include "core/core.h"
#include "colour.h"
#include "tgaimage.h"
#include "model.h"
#include "vector.h"
#include "matrix.h"
#include "draw.h"

#include <sstream>

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

const int depth = 255;

Matrix4f get_viewport_matrix(int x, int y, int width, int height)
{
	double half_width = width / 2.0,
	       half_height = height / 2.0,
	       half_depth = depth / 2.0;

	Matrix4f m = Matrix4f::TRANSLATION(x + half_width, y + half_height, half_depth);
	m(0,0) = half_width;
	m(1,1) = half_height;
	m(2,2) = half_depth;
	return m;
}

int main(int argc, char** argv)
{
	TGAImage *texture = NULL;
	const int width = 1000, height = 1000;
	int vp_scale_w = 1, vp_scale_h = 1;
	int vp_x = 0, vp_y = 0;

#if 0
	vp_scale_w = vp_scale_h = 8;
	vp_x = width / 2 - (width / vp_scale_w / 2);
	vp_y = height / 2 - (height / vp_scale_h / 2);
	Model model("Models/Ambassador/Ambassador.obj");
#elif 0
	vp_y = -height * 0.5;
	Model model("Models/BAC_Batman70s_rocksteady/batman.obj");
#elif 0
	// this model doesn't work well as quads aren't supported
	vp_scale_w = vp_scale_h = 80;
	vp_x = width / 2 - (width / vp_scale_w / 2);
	vp_y = height / 2 - (height / vp_scale_h / 2);
	Model model("Models/Harley/Harley.obj");
#elif 0
	vp_y = -height * 0.33;
	Model model("Models/cat/cat.obj");
	TGAImage cat = TGAImage("Models/cat/cat_diff.tga");
	texture = &cat;
#elif 0
	// doesn't render and takes a long time
	vp_scale_w = vp_scale_h = 80;
	vp_x = width / 2 - (width / vp_scale_w / 2);
	vp_y = height / 2 - (height / vp_scale_h / 2);
	Model model("Models/LionKing/Simba/Simba.obj");
	TGAImage body = TGAImage("Models/LionKing/Simba/Texture/5c817acd1.tga");
	texture = &body;
#elif 1
	Model model("Models/Head/african_head.obj");
	TGAImage head = TGAImage("Models/Head/african_head_diffuse.tga");
	texture = &head;
#endif

	TGAImage image(width, height, TGAImage::RGB);
	ZBuffer zbuf(width, height);
	// ZBuffer_AlwaysAllow zbuf;

// #define UT_TESTS_ONLY
// #define UNIT_TEST_VERBOSE
// #include "unittests/unittest_vector.h"
// #include "unittests/unittest_colour.h"
// #include "unittests/unittest_matrix.h"

// #define UT_WIREFRAME
// #include "unittests/unittest_colouredtriangles.h"
// #include "unittests/unittest_lines.h"


#ifndef UT_TESTS_ONLY
	PROFILE_SCOPED_FN

	// 3D Drawing
	Vector3f camera(0, 0, -2);
	Vector3f light(-1, -1, -3); //light.normalize();

	Matrix4f viewport = get_viewport_matrix(vp_x, vp_y, width / vp_scale_w, height / vp_scale_h);
	Matrix4f projection = Matrix4f::IDENTITY();
	projection(3, 2) = -1.0 / -camera.z;
	Matrix4f viewport_projection = viewport * projection;;

	// shaded render
	for (int i = 0; i < model.nfaces(); ++i)
	{
		Vertex screen_coords[3];
		Vector3f world_coords[3];

		std::vector<int> const &face = model.face(i);
		std::vector<int> const *face_uv = texture && model.has_uv(i) ? &model.face_uv(i) : NULL;
		for (int j = 0; j < 3; ++j)
		{
			assert(face.size() >= 3);
			if (face_uv)
				assert(face_uv->size() >= 3);

			Vector3f const &vert = model.vert(face[j]);
			screen_coords[j].point = Vector3i(viewport_projection * vert);
			world_coords[j] = vert;

			if (face_uv)
				screen_coords[j].uv = model.vert_uv((*face_uv)[j]);
		}

		Vector3f tri_normal = (world_coords[2]-world_coords[0]).cross(world_coords[1]-world_coords[0]).normalize();

		// back face occlusion - test for dot product being positive
		if (camera.dot(tri_normal) > 0)
		{
			const bool SMOOTH_SHADED = true;
			if (SMOOTH_SHADED && model.has_norm(i))
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
	zbuf.output("zbuf.tga");
	return 0;
}