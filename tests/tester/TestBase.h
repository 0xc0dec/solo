#pragma once

#include "../../include/solo.h"
#include <iostream>
#include <cassert>

#define LOG(msg) std::cout << msg << std::endl;

using namespace solo;


class TestBase
{
public:
	TestBase(Device *device) :
		device(device),
		scene(device->getScene()),
		resourceManager(device->getResourceManager()),
		fileSystem(device->getFileSystem())
	{
	}

	virtual ~TestBase() {}

	virtual void run() = 0;

protected:
	template <typename T>
	void assertThrows(std::function<void(void)> code, const std::string& expectedMsg = "")
	{
		try
		{
			code();
			assert(false);
		}
		catch (T &e)
		{
			if (!expectedMsg.empty())
				assert(expectedMsg == e.what());
		}
		catch (...)
		{
			assert(false);
		}
	}

	Device *device;
	Scene *scene;
	ResourceManager *resourceManager;
	FileSystem *fileSystem;
};
