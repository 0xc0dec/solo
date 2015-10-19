#pragma once

#include "SoloImageLoader.h"

namespace solo
{
	class StubImageLoader: public ImageLoader
	{
	public:
		StubImageLoader(FileSystem* fs, ResourceManager* resourceManager);

		virtual bool isLoadable(const std::string& uri) override;
		virtual shared<Image> load(const std::string& uri) override;
	};
}