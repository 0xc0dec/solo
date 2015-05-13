#include "SoloObjMeshLoader.h"
#include "SoloMesh.h"
#include "SoloFileSystem.h"

using namespace solo;


bool ObjMeshLoader::isLoadable(const std::string& url)
{
	return url.find(".obj", url.size() - 5) != std::string::npos;
}


shared<Mesh> ObjMeshLoader::load(const std::string& url)
{
	std::vector<Vector3> inputVertices, vertices;
	std::vector<Vector3> inputNormals, normals;
	std::vector<Vector2> inputUvs, uvs;
	std::vector<unsigned short> indices;
	std::unordered_map<std::string, unsigned short> uniqueIndices;

	// no materials import
	// no support for anything other than triangles
	auto lines = fs->readLines(url);
	for (auto &line: lines)
	{
		std::istringstream l(line);
		std::string type;
		l >> type;
		if (type == "v")
		{
			Vector3 v;
			l >> v.x >> v.y >> v.z;
			inputVertices.push_back(v);
		}
		else if (type == "vn")
		{
			Vector3 n;
			l >> n.x >> n.y >> n.z;
			inputNormals.push_back(n);
		}
		else if (type == "vt")
		{
			Vector2 uv;
			l >> uv.x >> uv.y;
			inputUvs.push_back(uv);
		}
		else if (type == "f")
		{
			std::string threes[3];
			char dummy;
			l >> threes[0] >> threes[1] >> threes[2];
			for (auto &three: threes)
			{
				auto it = uniqueIndices.find(three);
				if (it != uniqueIndices.end())
					indices.push_back(it->second);
				else
				{
					std::istringstream ss(three);
					unsigned vertexIndex, uvIndex, normalIndex;
					ss >> vertexIndex >> dummy;
					vertices.push_back(inputVertices[vertexIndex - 1]);
					if (ss >> uvIndex)
						uvs.push_back(inputUvs[uvIndex - 1]);
					ss >> dummy;
					if (ss >> normalIndex)
						normals.push_back(inputNormals[normalIndex - 1]);
					auto newIndex = vertices.size() - 1;
					uniqueIndices[three] = newIndex;
					indices.push_back(newIndex);
				}
			}
		}
	}

	auto mesh = MeshFactory::create();
	mesh->setVertices(vertices);
	mesh->setUVs(uvs);
	mesh->setNormals(normals);
	mesh->setIndices(indices);
	return mesh;
}