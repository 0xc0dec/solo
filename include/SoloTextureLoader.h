#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;
	class FileSystem;

	class TextureLoader
	{
	public:
		virtual ~TextureLoader() {}

		virtual bool isLoadable(const std::string& uri) = 0;
		virtual shared<Texture2D> load2D(const std::string& uri) = 0;

	protected:
		TextureLoader(FileSystem *fs): fs(fs) {}

		FileSystem *fs;

	private:
		TextureLoader(const TextureLoader& other) = delete;
		TextureLoader(TextureLoader&& other) = delete;
		TextureLoader& operator=(const TextureLoader& other) = delete;
		TextureLoader& operator=(TextureLoader&& other) = delete;
	};
}