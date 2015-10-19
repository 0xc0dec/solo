#pragma once

#include "SoloTextureLoader.h"

namespace solo
{
	class PngTextureLoader : public TextureLoader
	{
	public:
		PngTextureLoader(FileSystem* fs, ResourceManager *resourceManager);

		virtual bool isLoadable(const std::string& uri) override;
		virtual shared<Image> load2D(const std::string& uri) override;
	};
}