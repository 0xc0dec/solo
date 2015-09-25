#pragma once

#include "SoloTextureLoader.h"

namespace solo
{
	class StubTextureLoader: public TextureLoader
	{
	public:
		StubTextureLoader(FileSystem* fs, ResourceManager* resourceManager);

		virtual bool isLoadable(const std::string& uri) override;
		virtual shared<Texture2D> load2D(const std::string& uri) override;
	};
}