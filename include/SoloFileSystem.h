#pragma once

#include "SoloBase.h"

namespace solo
{
	class Device;

	class FileSystem
	{
	public:
		static shared<FileSystem> create(Device *device);

		FileSystem(const FileSystem& other) = delete;
		FileSystem(FileSystem&& other) = delete;
		FileSystem& operator=(const FileSystem& other) = delete;
		FileSystem& operator=(FileSystem&& other) = delete;
		virtual ~FileSystem() {}

		virtual std::vector<uint8_t> readBytes(const std::string& path);
		virtual void writeBytes(const std::string& path, const std::vector<uint8_t>& data);

		virtual std::string readText(const std::string& path);
		virtual std::vector<std::string> readLines(const std::string& path);
		virtual void iterateLines(const std::string& path, std::function<bool(const std::string&)> process);
		virtual void writeLines(const std::string& path, const std::vector<std::string>& lines);

	protected:
		FileSystem() {}
	};
}
