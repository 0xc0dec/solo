#include "SoloModel.h"

using namespace solo;


shared<Model> ModelFactory::create()
{
	return NEW2(Model);
}


void Model::addMesh(Mesh* mesh)
{
	meshes.push_back(mesh);
}


void Model::removeMesh(Mesh* mesh)
{
	auto pos = std::find(meshes.begin(), meshes.end(), mesh);
	if (pos != meshes.end())
		meshes.erase(pos);
}


Mesh* Model::getMesh(unsigned index) const
{
	return meshes[index];
}


size_t Model::getMeshCount() const
{
	return meshes.size();
}
