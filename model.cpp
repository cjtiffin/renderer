#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"


#define READ(qty, into) { iss >> ignore; Vector ## qty ## f v; for (int i=0; i<qty; ++i) iss >> v[i]; into.push_back(v); }

inline bool optional_push_back(std::istringstream &iss, std::vector<int> &container, size_t max)
{
	if (iss.peek() != '/')
	{
		int val; iss >> val;
		container.push_back(val > 0 ? val - 1 : max + val);
		return true;
	}
	return false;
}

std::streamsize remaining(std::istream &is)
{
    std::streampos pos = is.tellg();
    is.seekg(0, std::ios::end);
    std::streamsize len = is.tellg() - pos;
    is.seekg(pos);
    return len;
}

Model::Model(const char *filename)
	: verts(), faces()
{
	std::ifstream in(filename, std::ifstream::in);
	if (in.fail())
		return;

	std::string line;
	while (std::getline(in, line))
	{
		std::istringstream iss(line.c_str());
		std::string ignore;

		     if (!line.compare(0, 2, "v "))
			READ(3, verts)
		else if (!line.compare(0, 3, "vt "))
			READ(2, verts_tex)
		else if (!line.compare(0, 3, "vn "))
			READ(3, verts_norm)
		else if (!line.compare(0, 2, "f "))
		{
			char ignore; // use char for "/"
			iss >> ignore;

			bool v = false, vt = false, vn = false;
			std::vector<int> f, ft, fn;

			while (iss && remaining(iss) > 1)
			{
				v  |= optional_push_back(iss, f, verts.size()); iss >> ignore;
				vt |= optional_push_back(iss, ft, verts_tex.size()); iss >> ignore;
				vn |= optional_push_back(iss, fn, verts_norm.size());
			}

			if (v)  faces.push_back(f);
			if (vt) faces_tex.push_back(ft);
			if (vn) faces_norm.push_back(fn);
		}
	}

	std::cout << "model: " << filename << std::endl
	          << " v# " << verts.size() << " vt# " << verts_tex.size() << " vn# " << verts_norm.size() << std::endl
	          << " f# "  << faces.size() << " ft# "  << faces_tex.size() << " fn# "  << faces_norm.size() << std::endl;
}
