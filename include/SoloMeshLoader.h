#pragma once

#include "SoloBase.h"

namespace solo
{
	class Mesh;
	class FileSystem;

	class MeshLoader
	{
	public:
		virtual ~MeshLoader() {}

		virtual bool isLoadable(const std::string& url) = 0;
		virtual shared<Mesh> load(const std::string& url) = 0;

	protected:
		MeshLoader(FileSystem *fs): fs(fs) {}

		FileSystem *fs;

	private:
		MeshLoader(const MeshLoader& other) = delete;
		MeshLoader(MeshLoader&& other) = delete;
		MeshLoader& operator=(const MeshLoader& other) = delete;
		MeshLoader& operator=(MeshLoader&& other) = delete;
	};
}