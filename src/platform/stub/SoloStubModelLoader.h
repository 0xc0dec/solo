#pragma once

#include "../../SoloModelLoader.h"

namespace solo
{
	class StubModelLoader: public ModelLoader
	{
	public:
		StubModelLoader(FileSystem* fs, ResourceManager* resourceManager);

		virtual bool isLoadable(const std::string& uri) override;
		virtual shared<Model> load(const std::string& uri) override;
	};
}