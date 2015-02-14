#include "SoloModel.h"

using namespace solo;


shared<Model> ModelFactory::create()
{
	return NEW2(Model);
}


void Model::addMesh(shared<Mesh> mesh)
{
	meshes.push_back(mesh);
}


void Model::removeMesh(Mesh* mesh)
{
	auto pos = std::find_if(meshes.begin(), meshes.end(),
							[mesh](shared<Mesh> p) -> bool { return p.get() == mesh; });
	if (pos != meshes.end())
		meshes.erase(pos);
}


Mesh* Model::getMesh(unsigned index) const
{
	return meshes[index].get();
}


size_t Model::getMeshCount() const
{
	return meshes.size();
}
