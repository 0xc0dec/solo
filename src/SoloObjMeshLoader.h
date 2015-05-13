#pragma once

#include "SoloMeshLoader.h"

namespace solo
{
	class ObjMeshLoader: public MeshLoader
	{
	public:
		explicit ObjMeshLoader(FileSystem* fs);

		virtual bool isLoadable(const std::string& url) override;
		virtual shared<Mesh> load(const std::string& url) override;
	};
}