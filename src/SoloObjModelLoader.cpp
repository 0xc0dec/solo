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
	for (; from <= to; ++from)
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


void parseIndexes(const char **from, const char *to, unsigned **result)
{
	char buf[16];
	size_t bufIdx = 0;
	size_t resultIdx = 0;
	for (; *from <= to; *from += 1)
	{
		auto c = **from;
		auto okSym = (c >= '0' && c <= '9') || c == '-' || c == '.';
		if (okSym)
			buf[bufIdx++] = c;
		
		if (!okSym || *from == to)
		{
			if (bufIdx > 0)
			{
				buf[bufIdx] = '\0';
				*result[resultIdx++] = atof(buf);
				bufIdx = 0;
			}
			else if (resultIdx > 0)
				result[resultIdx++] = nullptr;
			if (resultIdx > 2)
				break;
		}
	}
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
	fs->iterateLines(uri, [&](const std::string& line)
	{
		auto lineSize = line.size();
		if (line[0] == 'v')
		{
			if (line[1] == 'n')
				inputNormals.push_back(parseVector3(line.c_str() + 3, line.c_str() + lineSize - 3));
			else if (line[1] == 't')
			{
				auto uv = parseVector3(line.c_str() + 3, line.c_str() + lineSize - 3);
				inputUvs.push_back(Vector2(uv.x, uv.y));
			}
			else
				inputVertices.push_back(parseVector3(line.c_str() + 2, line.c_str() + lineSize - 2));
		}
		else if (line[0] == 'f')
		{
			auto from = line.c_str() + 2;
			auto to = from + lineSize - 3;
			unsigned spaceIdx = 1;
			unsigned vIdx, uvIdx, nIdx;
			unsigned *idxs[] = { &vIdx, &uvIdx, &nIdx };
			std::string three;
			for (auto i = 0; i < 3; ++i)
			{
				auto nextSpaceIdx = line.find(' ', spaceIdx + 1);
				three.assign(line.substr(spaceIdx + 1, nextSpaceIdx != std::string::npos ? (nextSpaceIdx - spaceIdx - 1): lineSize - spaceIdx));
				spaceIdx = nextSpaceIdx;
				auto it = uniqueIndices.find(three);
				if (it != uniqueIndices.end())
				{
					indices.push_back(it->second);
					from += three.size() + 1;
				}
				else
				{
					parseIndexes(&from, to, idxs);
					vertices.push_back(inputVertices[vIdx - 1]);
					if (idxs[1])
						uvs.push_back(inputUvs[uvIdx - 1]);
					if (idxs[2])
						normals.push_back(inputNormals[nIdx - 1]);
					auto newIndex = static_cast<uint16_t>(vertices.size() - 1);
					uniqueIndices[three] = newIndex;
					indices.push_back(newIndex);
				}
			}
		}
		else if (line[0] == 'o')
		{
			if (!vertices.empty())
				finishObject(meshName);
			meshName.assign(line.substr(2));
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