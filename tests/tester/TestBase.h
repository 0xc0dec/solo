#pragma once

#include <iostream>
#include "../../include/solo.h"

#define LOG(msg) std::cout << msg << std::endl;

using namespace solo;


class TestBase
{
public:
	TestBase(Engine *engine) :
		engine(engine),
		device(engine->getDevice()),
		scene(engine->getScene()),
		resourceManager(engine->getResourceManager()),
		fileSystem(engine->getFileSystem())
	{
	}

	virtual ~TestBase() {}

	virtual void run() = 0;

protected:
	Engine *engine;
	Device *device;
	Scene *scene;
	ResourceManager *resourceManager;
	FileSystem *fileSystem;
};
