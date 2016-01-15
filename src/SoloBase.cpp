#include "SoloBase.h"
#include "SoloComponent.h"

using namespace solo;


int ComponentTypeId::counter = 0;


extern "C" size_t getHash(const std::string& s)
{
    return std::hash<std::string>()(s);
}