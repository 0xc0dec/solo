#include "CommonInternal.h"
#include "Engine.h"

using namespace solo;

extern "C" IEngine *getEngine()
{
	return Engine::getEngine();
}
