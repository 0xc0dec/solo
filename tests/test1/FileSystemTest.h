#pragma once

#include "TestBase.h"

class FileSystemTest : public TestBase
{
public:
	FileSystemTest(Engine* engine): TestBase(engine)
	{
	}

	virtual void run() override
	{
		test_WriteBytes_ReadBytes();
		test_WriteLines_ReadLines();
		test_WriteLines_ReadText();
	}

	void test_WriteLines_ReadText()
	{
		std::vector<std::string> lines =
		{
			"First",
			"Sec ond",
			"Third",
			"   ... line."
		};
		engine->getFileSystem()->writeLines("temp2.txt", lines);
		auto text = engine->getFileSystem()->readText("temp2.txt");
		auto expected = R"_(First
Sec ond
Third
   ... line.)_";
		assert(text == expected);
	}

	void test_WriteBytes_ReadBytes()
	{
		std::vector<uint8_t> data = { 0x01, 0x02, 0x03, 0x04 };
		engine->getFileSystem()->writeBytes("temp.bin", data);
		auto readData = engine->getFileSystem()->readBytes("temp.bin");
		assert(readData.size() == data.size());
		assert(readData[0] == data[0]);
		assert(readData[1] == data[1]);
		assert(readData[2] == data[2]);
		assert(readData[3] == data[3]);
	}

	void test_WriteLines_ReadLines()
	{
		std::vector<std::string> lines =
		{
			"First",
			"Sec ond",
			"Third",
			"   ... line."
		};
		engine->getFileSystem()->writeLines("temp.txt", lines);
		auto readLines = engine->getFileSystem()->readLines("temp.txt");
		assert(readLines.size() == lines.size());
		assert(readLines[0] == lines[0]);
		assert(readLines[1] == lines[1]);
		assert(readLines[2] == lines[2]);
		assert(readLines[3] == lines[3]);
	}
};