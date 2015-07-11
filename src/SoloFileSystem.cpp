#include <fstream>
#include "SoloFileSystem.h"

using namespace solo;


shared<FileSystem> FileSystemFactory::create()
{
	return NEW2(FileSystem);
}


std::vector<uint8_t> FileSystem::readBytes(const std::string& path)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		THROW_FMT(EngineException, "Failed to open file '", path, "'");
	auto size = file.tellg();
	file.seekg(0, std::ios::beg);
	auto result = std::vector<uint8_t>(size);
	file.read(reinterpret_cast<char*>(&result[0]), size);
	file.close();
	return result;
}


void FileSystem::writeBytes(const std::string& path, const std::vector<uint8_t>& data)
{
	std::ofstream file(path, std::ios::binary | std::ios::trunc);
	if (!file.is_open())
		THROW_FMT(EngineException, "Failed to open file '", path, "'");
	file.write(reinterpret_cast<const char*>(&data[0]), data.size());
	file.close();
}


std::string FileSystem::readText(const std::string& path)
{
	std::ifstream f(path);
	auto result = std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
	return result;
}


std::vector<std::string> FileSystem::readLines(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
		THROW_FMT(EngineException, "Failed to open file '", path, "'");
	std::vector<std::string> result;
	while (!file.eof())
	{
		std::string line;
		std::getline(file, line);
		result.push_back(line);
	}
	file.close();
	return result;
}


void FileSystem::writeLines(const std::string& path, const std::vector<std::string>& lines)
{
	std::ofstream file(path, std::ios::trunc);
	if (!file.is_open())
		THROW_FMT(EngineException, "Failed to open file '", path, "'");
	for (size_t i = 0; i < lines.size(); ++i)
	{
		file << lines[i];
		if (i < lines.size() - 1)
			file << std::endl;
	}
	file.close();
}