#include "SoloBaseInternal.h"
#include "SoloEngine.h"

using namespace solo;


extern "C" IEngine *getEngine()
{
	return Engine::getEngine();
}


extern "C" size_t computeHash(const String &s)
{
	return std::hash<String>()(s);
}