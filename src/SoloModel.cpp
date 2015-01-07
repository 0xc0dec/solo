#include "SoloModel.h"

using namespace solo;


ptr<Model> Model::create()
{
	return NEW2(Model);
}


void Model::addMesh(const ptr<Mesh> mesh)
{
	_meshes.push_back(mesh);
}


void Model::removeMesh(const ptr<Mesh> mesh)
{
	auto pos = std::find(_meshes.begin(), _meshes.end(), mesh);
	if (pos != _meshes.end())
		_meshes.erase(pos);
}


ptr<Mesh> Model::getMesh(unsigned index) const
{
	return _meshes[index];
}


size_t Model::getMeshCount() const
{
	return _meshes.size();
}
