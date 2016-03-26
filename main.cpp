#include <utility>
#include <cmath>

#include "core/core.h"
#include "colour.h"
#include "tgaimage.h"
#include "model.h"
#include "vector.h"
#include "draw.h"

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


int main(int argc, char** argv)
{
	PROFILE_SCOPED_FN

// #include "unittests/unittest_vector.h"

#if 0
	const float width_offset = 1.0, width_scale = 2.0,
	            height_offset = 0.2, height_scale = 2.3;
	Model model("Models/BAC_Batman70s_rocksteady/batman.obj");
#elif 0
	const float width_offset = 1.0, width_scale = 2.0,
	            height_offset = 0.2, height_scale = 2.3;
	Model model("Models/Harley/Harley.obj");
#elif 1
	const float width_offset = 0.5, width_scale = 1.0,
	            height_offset = 0.05, height_scale = 1.0;
	Model model("Models/cat/cat.obj");
#elif 1
	const float width_offset = 1.0, width_scale = 2.0,
	            height_offset = 1.0, height_scale = 2.0;
	Model model("Models/african_head.obj");
#endif

	const unsigned width = 1000, height = 1000;
	TGAImage image(width, height, TGAImage::RGB);
	ZBuffer zbuf(width, height);
	ZBuffer_AlwaysAllow zb_always;

// #define UT_WIREFRAME
// #include "unittests/unittest_colouredtriangles.h"

	// 3D Drawing
	Vector3f light(0, 0, -1);

	// shaded render
	for (int i = 0; i < model.nfaces(); ++i)
	{
		Vector3f world_coords[3];
		Vertex screen_coords[3];

		std::vector<int> const &face = model.face(i);
		for (int j = 0; j < 3; ++j)
		{
			assert(face.size() >= 3);
			Vector3f const &vert = model.vert(face[j]);
			screen_coords[j].point = Vector3f(int((vert.x + width_offset) * width / width_scale),
			                                  int((vert.y + height_offset) * height / height_scale),
			                                      (vert.z + 1.0) / 2.0);
			screen_coords[j].colour = Colour(rand()%255, rand()%255, rand()%255, 255);
			world_coords[j] = vert;
		}

		float intensity = 0.0;
		#if 0 // use given normals?
		{
			std::vector<int> const &face_norm = model.face_norm(i);
			Vector3f norm = model.vert_norm(face_norm[0]) + model.vert_norm(face_norm[1]) + model.vert_norm(face_norm[2]);
			intensity = -light.dot(norm.normalize());
		}
		#else // calculate normals
		{
			Vector3f norm = (world_coords[2]-world_coords[0]).cross(world_coords[1]-world_coords[0]);
			intensity = light.dot(norm.normalize());
		}
		#endif

		// back face occlusion - test for dot product being positive
		if (intensity > 0)
		{
			triangle(screen_coords[0], screen_coords[1], screen_coords[2], zbuf, image);
			// line(screen_coords[0].point, screen_coords[1].point, zb_always, image, Colours::white);
			// line(screen_coords[1].point, screen_coords[2].point, zb_always, image, Colours::white);
			// line(screen_coords[2].point, screen_coords[0].point, zb_always, image, Colours::white);
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

	image.flip_vertically();
	image.write_tga_file("output.tga");
	// zbuf.output("zbuf.tga");
	return 0;
}