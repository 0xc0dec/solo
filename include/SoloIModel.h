#pragma once

#include "SoloBase.h"

namespace solo
{
	class IMesh;

	class IModel
	{
	public:
		virtual ~IModel() {}

		virtual void addMesh(const ptr<IMesh> mesh) = 0;
		virtual void removeMesh(const ptr<IMesh> mesh) = 0;
		virtual ptr<IMesh> getMesh(unsigned index) const = 0;
		virtual unsigned getMeshCount() const = 0;
		virtual void iterateMeshes(std::function<void(const ptr<IMesh>)> f) = 0;
	};
}