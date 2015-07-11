#pragma once

#include "SoloBase.h"

namespace solo
{
	class FileSystem
	{
	public:
		virtual ~FileSystem() {}

		std::vector<uint8_t> readBytes(const std::string& path);
		void writeBytes(const std::string& path, const std::vector<uint8_t>& data);

		std::string readText(const std::string& path);
		std::vector<std::string> readLines(const std::string& path);
		void writeLines(const std::string& path, const std::vector<std::string>& lines);

	private:
		friend class FileSystemFactory;

		FileSystem() {}
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
