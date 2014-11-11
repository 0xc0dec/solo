#include "SoloCommonInternal.h"
#include "SoloEngine.h"

using namespace solo;


extern "C" IEngine *getEngine()
{
	return Engine::getEngine();
}


extern "C" size_t computeHash(const str &s)
{
	return std::hash<str>()(s);
}