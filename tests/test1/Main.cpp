#include "Solo.h"

using namespace solo;

int main(int argc, char *argv[])
{
	auto engine = getEngine();
	EngineLaunchArgs engineArgs = { 640, 480, 32, 24, false };
	engine->run(engineArgs);
	return 0;
}