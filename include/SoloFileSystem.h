#pragma once

#include "SoloBase.h"

namespace solo
{
	class FileSystem
	{
	public:
		static shared<FileSystem> create();

		FileSystem(const FileSystem& other) = delete;
		FileSystem(FileSystem&& other) = delete;
		FileSystem& operator=(const FileSystem& other) = delete;
		FileSystem& operator=(FileSystem&& other) = delete;
		virtual ~FileSystem() {}

		std::vector<uint8_t> readBytes(const std::string& path);
		void writeBytes(const std::string& path, const std::vector<uint8_t>& data);

		std::string readText(const std::string& path);
		std::vector<std::string> readLines(const std::string& path);
		void iterateLines(const std::string& path, std::function<bool(const std::string&)> process);
		void writeLines(const std::string& path, const std::vector<std::string>& lines);

	private:
		FileSystem() {}
	};
}
