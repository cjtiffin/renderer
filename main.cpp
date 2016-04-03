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

Matrix4f get_projection_matrix(double fov, double aspect = 4.0/3.0, double near_clip = 0.1, double far_clip = 100.0)
{
	Matrix4f m = Matrix4f::IDENTITY();
	// m(3, 2) = -1.0 / (camera_pos - camera_look_at).length();
	return m;
}

Matrix4f look_at(const Vector3f & eye, const Vector3f & pos, Vector3f up)
{
	Vector3f forward = (eye - pos).normalize();
	Vector3f side = up.cross(forward).normalize();
	up = forward.cross(side).normalize();

	Matrix4f mt = Matrix4f::TRANSLATION(eye.x, eye.y, eye.z);
	Matrix4f m = Matrix4f::IDENTITY();
	m.set_column(0, side);
	m.set_column(1, up);
	m.set_column(2, forward);
	return m * mt;
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

	Vector3f camera_look_at(0, 0, 0);
	Vector3f camera_pos(1, 1, -3);
	Vector3f camera_up(0, 1, 0);
	Vector3f light_dir(-1, -1, -1); //direction light points in

	// transformation matrices
	Matrix4f mat_model = Matrix4f::IDENTITY();
	Matrix4f mat_view = Matrix4f::IDENTITY();
	Matrix4f mat_projection = Matrix4f::IDENTITY();
	Matrix4f mat_viewport = Matrix4f::IDENTITY();

	mat_view = look_at(camera_look_at, camera_pos, camera_up);
	mat_projection(3, 2) = -1.0 / (camera_pos - camera_look_at).length();
	mat_viewport = get_viewport_matrix(vp_x, vp_y, width / vp_scale_w, height / vp_scale_h);

	Matrix4f mat_model_view = mat_model * mat_view;
	Matrix4f mat_vpmv = mat_viewport * mat_projection * mat_model_view;
	Matrix4f mat_model_view_inv = mat_model_view.inverse();

	PRINTX(mat_model);
	PRINTX(mat_view);
	PRINTX(mat_projection);
	PRINTX(mat_viewport);
	PRINTX(mat_model_view);
	PRINTX(mat_vpmv);
	PRINTX(mat_model_view_inv);

	PRINTX(mat_vpmv * Vector3f(0,0,0));

	// shaded render
	for (int i = 0; i < model.nfaces(); ++i)
	{
		Vertex screen_coords[3];
		Vector3f world_coords[3];

		std::vector<int> const &face = model.face(i);
		std::vector<int> const *face_uv = texture && model.has_uv(i) ? &model.face_uv(i) : NULL;
		for (int j = 0; j < 3; ++j)
		{
			// assert(face.size() >= 3);
			if (face_uv)
				assert(face_uv->size() >= 3);

			Vector3f const &vert = model.vert(face[j]);
			screen_coords[j].point = Vector3i(mat_vpmv * vert);
			world_coords[j] = vert;

			if (face_uv)
				screen_coords[j].uv = model.vert_uv((*face_uv)[j]);
		}

		Vector3f tri_normal = mat_model_view_inv * (world_coords[2]-world_coords[0]).cross(world_coords[1]-world_coords[0]);

		// back face occlusion - test for dot product being positive
		if (camera_look_at.dot(tri_normal) <= 0)
		{
			const bool SMOOTH_SHADED = true;
			if (SMOOTH_SHADED && model.has_norm(i))
			{
				// calculate the light intensity at each normal
				std::vector<int> const &face_norm = model.face_norm(i);
				screen_coords[0].colour = ColourD(-light_dir.dot(model.vert_norm(face_norm[0]))).clamp();
				screen_coords[1].colour = ColourD(-light_dir.dot(model.vert_norm(face_norm[1]))).clamp();
				screen_coords[2].colour = ColourD(-light_dir.dot(model.vert_norm(face_norm[2]))).clamp();
			}
			else
				// just use the face's light intensity
				screen_coords[0].colour = screen_coords[1].colour = screen_coords[2].colour = ColourD(-light_dir.dot(tri_normal)).clamp();

			triangle(screen_coords[0], screen_coords[1], screen_coords[2], zbuf, image, texture);
		}
		// else
		// 	PRINTX(tri_normal);
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