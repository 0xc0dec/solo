#pragma once

#include <iostream>
#include "../../include/solo.h"

#define LOG(msg) std::cout << msg << std::endl;

using namespace solo;


class TestBase
{
public:
	TestBase(Engine *engine) :
		_engine(engine), _device(engine->getDevice()), _scene(_engine->getScene())
	{
	}

	virtual ~TestBase() {}

	virtual void run() = 0;

protected:
	Engine *_engine;
	ptr<Device> _device;
	ptr<Scene> _scene;
};
