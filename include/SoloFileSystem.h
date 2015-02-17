#pragma once

#include "SoloBase.h"

namespace solo
{
	class FileSystem
	{
	public:
		virtual ~FileSystem() {}

		std::vector<char> readBytes(const std::string& path);
		void writeBytes(const std::string& path, const std::vector<char>& data);

		std::vector<std::string> readLines(const std::string& path);
		void writeLines(const std::string& path, const std::vector<std::string>& lines);

	private:
		friend class FileSystemFactory;

		FileSystem();
		FileSystem(const FileSystem& other) = delete;
		FileSystem(FileSystem&& other) = delete;
		FileSystem& operator=(const FileSystem& other) = delete;
		FileSystem& operator=(FileSystem&& other) = delete;
	};

	class FileSystemFactory
	{
		friend class Engine;
		static shared<FileSystem> create();
	};
}
