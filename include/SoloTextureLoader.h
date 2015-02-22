#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;

	class TextureLoader
	{
	public:
		virtual ~TextureLoader() {}

		virtual bool isLoadable(const std::string& url) = 0;
		virtual shared<Texture2D> load2D(const std::string& url) = 0;

	protected:
		TextureLoader() {}

	private:
		TextureLoader(const TextureLoader& other) = delete;
		TextureLoader(TextureLoader&& other) = delete;
		TextureLoader& operator=(const TextureLoader& other) = delete;
		TextureLoader& operator=(TextureLoader&& other) = delete;
	};
}