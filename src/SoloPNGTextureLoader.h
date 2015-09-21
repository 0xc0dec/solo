#pragma once

#include "SoloTextureLoader.h"

namespace solo
{
	class PngTextureLoader : public TextureLoader
	{
	public:
		explicit PngTextureLoader(FileSystem* fs);

		virtual bool isLoadable(const std::string& uri) override;
		virtual shared<Texture2D> load2D(const std::string& uri) override;
	};
}