#include "SoloCommonsInternal.h"
#include "SoloEngine.h"

using namespace solo;


extern "C" IEngine *getEngine()
{
	return Engine::getEngine();
}
