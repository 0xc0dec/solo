#include "SoloObjModelLoader.h"
#include "SoloMesh.h"
#include "SoloModel.h"
#include "SoloFileSystem.h"
#include "SoloResourceManager.h"
#include <sstream>

using namespace solo;


ObjModelLoader::ObjModelLoader(FileSystem* fs, ResourceManager *resourceManager):
	ModelLoader(fs, resourceManager)
{
}


bool ObjModelLoader::isLoadable(const std::string& uri)
{
	return uri.find(".obj", uri.size() - 5) != std::string::npos;
}


Vector3 parseVector3(const char *from, const char *to)
{
	char buf[16];
	float result[3];
	size_t bufIdx = 0;
	size_t resultIdx = -1;
	for (; from <= to; from += 1)
	{
		auto c = *from;
		if ((from == to || isspace(c)) && bufIdx > 0)
		{
			buf[bufIdx] = '\0';
			result[++resultIdx] = atof(buf);
			bufIdx = 0;
			if (resultIdx == 2)
				break;
		}
		else
			buf[bufIdx++] = c;
	}
	return Vector3(result[0], result[1], result[2]);
}


shared<Model> ObjModelLoader::load(const std::string& uri)
{
	// Yes, no materials support
	// Yes, no support for anything other than triangles
	// Yes, tons of other TODOs... but still what we have is enough for now

	std::vector<Vector3> inputVertices, vertices;
	std::vector<Vector3> inputNormals, normals;
	std::vector<Vector2> inputUvs, uvs;
	std::vector<uint16_t> indices;
	std::unordered_map<std::string, uint16_t> uniqueIndices;
	std::vector<shared<Mesh>> meshes;

	auto finishObject = [&](const std::string& meshName)
	{
		auto meshUri = uri + "/";
		if (!meshName.empty())
			meshUri += meshName;
		else
			meshUri += std::to_string(meshes.size());
		auto mesh = resourceManager->getOrCreateMesh(meshUri); // assume that the mesh doesn't exist yet
		mesh->setVertices(vertices);
		mesh->setUVs(uvs);
		mesh->setNormals(normals);
		mesh->setIndices(indices);
		meshes.push_back(mesh);
		vertices.clear();
		uvs.clear();
		normals.clear();
		indices.clear();
		uniqueIndices.clear();
	};
	
	std::string meshName;
	std::string threes[3];
	fs->iterateLines(uri, [&](const std::string& line)
	{
		std::istringstream l(line);
		std::string type;
		auto lineSize = line.size();
		l >> type;
		if (type == "v")
			inputVertices.push_back(parseVector3(line.c_str() + 2, line.c_str() + lineSize - 2));
		else if (type == "vn")
			inputNormals.push_back(parseVector3(line.c_str() + 3, line.c_str() + lineSize - 3));
		else if (type == "vt")
		{
			auto uv = parseVector3(line.c_str() + 3, line.c_str() + lineSize - 3);
			inputUvs.push_back(Vector2(uv.x, uv.y));
		}
		else if (type == "f")
		{
			char dummy;
			auto from = line.c_str() + 2;
			l >> threes[0] >> threes[1] >> threes[2];
			for (const auto &three: threes)
			{
				auto it = uniqueIndices.find(three);
				if (it != uniqueIndices.end())
					indices.push_back(it->second);
				else
				{
					std::istringstream ss(three);
					unsigned vIdx, uvIdx, nIdx;
					ss >> vIdx >> dummy;
					vertices.push_back(inputVertices[vIdx - 1]);
					if (ss >> uvIdx)
						uvs.push_back(inputUvs[uvIdx - 1]);
					ss >> dummy;
					if (ss >> nIdx)
						normals.push_back(inputNormals[nIdx - 1]);
					auto newIndex = static_cast<uint16_t>(vertices.size() - 1);
					uniqueIndices[three] = newIndex;
					indices.push_back(newIndex);
				}
			}
		}
		else if (type == "o")
		{
			if (!vertices.empty())
				finishObject(meshName);
			l >> meshName;
		}
		return true;
	});

	if (!vertices.empty())
		finishObject(meshName);

	auto model = resourceManager->getOrCreateModel(uri); // assume that the model doesn't exist yet
	for (auto mesh : meshes)
		model->addMesh(mesh);
	return model;
}