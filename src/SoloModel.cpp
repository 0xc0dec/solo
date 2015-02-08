#include "SoloModel.h"

using namespace solo;


ptr<Model> Model::create()
{
	return NEW2(Model);
}


void Model::addMesh(const ptr<Mesh> mesh)
{
	meshes.push_back(mesh);
}


void Model::removeMesh(const ptr<Mesh> mesh)
{
	auto pos = std::find(meshes.begin(), meshes.end(), mesh);
	if (pos != meshes.end())
		meshes.erase(pos);
}


ptr<Mesh> Model::getMesh(unsigned index) const
{
	return meshes[index];
}


size_t Model::getMeshCount() const
{
	return meshes.size();
}
