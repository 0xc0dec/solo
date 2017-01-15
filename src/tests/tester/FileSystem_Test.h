/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "TestBase.h"


class FileSystem_Test final: public TestBase
{
public:
    FileSystem_Test(Device *device): TestBase(device)
    {
    }

    void run() override final
    {
        test_BytesReading();
        test_LinesReading();
        test_TextReading();
    }

private:
    void test_TextReading()
    {
        std::vector<std::string> lines =
        {
            "First",
            "Sec ond",
            "Third",
            "   ... line."
        };
        fileSystem->writeLines("temp2.txt", lines);
        auto text = fileSystem->readText("temp2.txt");
        auto expected = "First\nSec ond\nThird\n   ... line.";
        assert(text == expected);
    }

    void test_BytesReading()
    {
        std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04};
        fileSystem->writeBytes("temp.bin", data);
        auto readData = fileSystem->readBytes("temp.bin");
        assert(readData.size() == data.size());
        assert(readData[0] == data[0]);
        assert(readData[1] == data[1]);
        assert(readData[2] == data[2]);
        assert(readData[3] == data[3]);
    }

    void test_LinesReading()
    {
        std::vector<std::string> lines =
        {
            "First",
            "Sec ond",
            "Third",
            "   ... line."
        };
        fileSystem->writeLines("temp.txt", lines);
        auto readLines = fileSystem->readLines("temp.txt");
        assert(readLines.size() == lines.size());
        assert(readLines[0] == lines[0]);
        assert(readLines[1] == lines[1]);
        assert(readLines[2] == lines[2]);
        assert(readLines[3] == lines[3]);
    }
};