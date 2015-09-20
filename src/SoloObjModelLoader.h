#pragma once

#include "SoloModelLoader.h"

namespace solo
{
	class ObjModelLoader: public ModelLoader
	{
	public:
		explicit ObjModelLoader(FileSystem* fs);

		virtual bool isLoadable(const std::string& url) override;
		virtual shared<Model> load(const std::string& url) override;
	};
}