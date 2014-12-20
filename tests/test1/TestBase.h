#ifndef __TEST_BASE_H__
#define __TEST_BASE_H__

#include <iostream>
#include "../../include/solo.h"

#define LOG(msg) std::cout << msg << std::endl;

using namespace solo;


class TestBase
{
public:
	virtual ~TestBase() {}

	virtual void run(IEngine *engine) = 0;
};

#endif