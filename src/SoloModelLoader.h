#pragma once

#include "SoloBase.h"

namespace solo
{
	class Model;
	class FileSystem;

	class ModelLoader
	{
	public:
		virtual ~ModelLoader() {}

		virtual bool isLoadable(const std::string& url) = 0;
		virtual shared<Model> load(const std::string& url) = 0;

	protected:
		ModelLoader(FileSystem *fs): fs(fs) {}

		FileSystem *fs;

	private:
		ModelLoader(const ModelLoader& other) = delete;
		ModelLoader(ModelLoader&& other) = delete;
		ModelLoader& operator=(const ModelLoader& other) = delete;
		ModelLoader& operator=(ModelLoader&& other) = delete;
	};
}