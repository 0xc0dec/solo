#include "SoloBase.h"
#include "SoloTypeId.h"

using namespace solo;


size_t TypeId::counter = 0;


extern "C" size_t getHash(const std::string& s)
{
    return std::hash<std::string>()(s);
}