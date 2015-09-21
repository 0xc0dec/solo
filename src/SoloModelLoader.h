#pragma once

#include "SoloBase.h"

namespace solo
{
	class Model;
	class FileSystem;
	class ResourceManager;

	class ModelLoader
	{
	public:
		virtual ~ModelLoader() {}

		virtual bool isLoadable(const std::string& uri) = 0;
		virtual shared<Model> load(const std::string& uri) = 0;

	protected:
		ModelLoader(FileSystem *fs, ResourceManager *resourceManager):
			fs(fs),
			resourceManager(resourceManager)
		{
		}

		FileSystem *fs;
		ResourceManager *resourceManager;

	private:
		ModelLoader(const ModelLoader& other) = delete;
		ModelLoader(ModelLoader&& other) = delete;
		ModelLoader& operator=(const ModelLoader& other) = delete;
		ModelLoader& operator=(ModelLoader&& other) = delete;
	};
}