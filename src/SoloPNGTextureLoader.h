#pragma once

#include "SoloTextureLoader.h"

namespace solo
{
	class PNGTextureLoader : public TextureLoader
	{
	public:
		explicit PNGTextureLoader(FileSystem* fs):
			TextureLoader(fs)
		{
		}

		virtual bool isLoadable(const std::string& url) override;
		virtual shared<Texture2D> load2D(const std::string& url) override;
	};
}