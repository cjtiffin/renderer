#ifndef _MODEL_H_
#define _MODEL_H_
#pragma once

#include <vector>
#include "vector.h"
#include "core/assert.h"

class Model
{
public:
	Model(const char *filename);

	size_t nverts() { return verts.size(); }
	size_t nfaces() { return faces.size(); }

	Vector3f const &vert(size_t idx)      { assert(idx < verts.size()); return verts[idx]; }
	Vector2f const &vert_uv(size_t idx)   { assert(idx < verts_uv.size()); return verts_uv[idx]; }
	Vector3f const &vert_norm(size_t idx) { assert(idx < verts_norm.size()); return verts_norm[idx]; }

	std::vector<int> const &face(size_t idx)      { assert(idx < faces.size()); return faces[idx]; }
	std::vector<int> const &face_uv(size_t idx)   { assert(idx < faces_uv.size()); return faces_uv[idx]; }
	std::vector<int> const &face_norm(size_t idx) { assert(idx < faces_norm.size()); return faces_norm[idx]; }

private:
	std::vector<Vector3f> verts, verts_norm;
	std::vector<Vector2f> verts_uv;
	std::vector<std::vector<int>> faces, faces_uv, faces_norm;
};

#endif //_MODEL_H_
