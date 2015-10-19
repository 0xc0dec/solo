#pragma once

#include "SoloBase.h"

namespace solo
{
	struct Image;
	class FileSystem;
	class ResourceManager;
	
	class TextureLoader // TODO rename
	{
	public:
		virtual ~TextureLoader() {}

		virtual bool isLoadable(const std::string& uri) = 0;
		virtual shared<Image> load2D(const std::string& uri) = 0;

	protected:
		TextureLoader(FileSystem *fs, ResourceManager *resourceManager):
			fs(fs),
			resourceManager(resourceManager)
		{
		}

		FileSystem *fs;
		ResourceManager *resourceManager;

	private:
		TextureLoader(const TextureLoader& other) = delete;
		TextureLoader(TextureLoader&& other) = delete;
		TextureLoader& operator=(const TextureLoader& other) = delete;
		TextureLoader& operator=(TextureLoader&& other) = delete;
	};
}