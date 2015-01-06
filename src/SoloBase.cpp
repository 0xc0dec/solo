#include "SoloBase.h"
#include "SoloEngine.h"

using namespace solo;


extern "C" Engine *getEngine()
{
	return Engine::get();
}


extern "C" size_t getHash(const std::string &s)
{
	return std::hash<std::string>()(s);
}