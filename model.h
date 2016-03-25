#ifndef _MODEL_H_
#define _MODEL_H_
#pragma once

#include <vector>
#include "vector.h"

class Model
{
public:
	Model(const char *filename);

	size_t nverts() { return verts.size(); }
	size_t nfaces() { return faces.size(); }

	Vector3f const &vert(size_t idx)      { return verts[idx]; }
	Vector2f const &vert_tex(size_t idx)  { return verts_tex[idx]; }
	Vector3f const &vert_norm(size_t idx) { return verts_norm[idx]; }

	std::vector<int> const &face(size_t idx)      { return faces[idx]; }
	std::vector<int> const &face_tex(size_t idx)  { return faces_tex[idx]; }
	std::vector<int> const &face_norm(size_t idx) { return faces_norm[idx]; }

private:
	std::vector<Vector3f> verts, verts_norm;
	std::vector<Vector2f> verts_tex;
	std::vector<std::vector<int>> faces, faces_tex, faces_norm;
};

#endif //_MODEL_H_
