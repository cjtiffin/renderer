#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"


#define READ(qty, into) { iss >> ignore; Vector ## qty ## f v; for (int i=0; i<qty; ++i) iss >> v[i]; into.push_back(v); }

Model::Model(const char *filename)
	: verts(), faces()
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail())
		return;

	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
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
			char ignore;
			iss >> ignore;
			std::vector<int> f, ft, fn;
			int v, vt, vn;
			while (iss >> v >> ignore >> vt >> ignore >> vn)
			{
				f.push_back (v  > 0 ? v -1 : verts.size()     +v);
				ft.push_back(vt > 0 ? vt-1 : verts_tex.size() +vt);
				fn.push_back(vn > 0 ? vn-1 : verts_norm.size()+vn);
			}

			faces.push_back(f);
			faces_tex.push_back(ft);
			faces_norm.push_back(fn);
		}
	}

	std::cout << "model: " << filename
	          << " v# " << verts.size() << " vt# " << verts_tex.size() << " vn# " << verts_norm.size()
	          << " f# "  << faces.size() << " ft# "  << faces_tex.size() << " fn# "  << faces_norm.size() << std::endl;
}
