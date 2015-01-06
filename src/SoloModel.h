#pragma once

#include "SoloBaseInternal.h"
#include "SoloIModel.h"

namespace solo
{
	class Mesh;

	// Represents a renderable 3D model composed of one or more meshes.
	class Model : public IModel
	{
	public:
		static ptr<Model> create();

		virtual void addMesh(const ptr<IMesh> mesh) override;
		virtual void removeMesh(const ptr<IMesh> mesh) override;
		virtual ptr<IMesh> getMesh(unsigned index) const override;
		virtual unsigned getMeshCount() const override;

	private:
		Model() {}
		virtual ~Model() {}

		std::vector<ptr<Mesh>> _meshes;
	};
}