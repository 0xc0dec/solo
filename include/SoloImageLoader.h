#pragma once

#include "SoloBase.h"

namespace solo
{
	struct Image;
	class FileSystem;
	class ResourceManager;
	
	class ImageLoader
	{
	public:
		virtual ~ImageLoader() {}
		ImageLoader(const ImageLoader& other) = delete;
		ImageLoader(ImageLoader&& other) = delete;
		ImageLoader& operator=(const ImageLoader& other) = delete;
		ImageLoader& operator=(ImageLoader&& other) = delete;

		virtual bool isLoadable(const std::string& uri) = 0;
		virtual shared<Image> load(const std::string& uri) = 0;

	protected:
		ImageLoader(FileSystem *fs, ResourceManager *resourceManager):
			fs(fs),
			resourceManager(resourceManager)
		{
		}

		FileSystem *fs;
		ResourceManager *resourceManager;
	};
}