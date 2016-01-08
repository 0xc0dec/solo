#include "SoloLuaIndexedMeshPart.h"

using namespace solo;


void LuaIndexedMeshPart::resetIndexData(IndexedMeshPart *part, const std::vector<uint16_t>& data, unsigned elementCount, bool dynamic)
{
    part->resetIndexData(MeshIndexFormat::UnsignedShort, data.data(), elementCount, dynamic);
}


void LuaIndexedMeshPart::updateIndexData(IndexedMeshPart *part, const std::vector<uint16_t>& data, unsigned elementCount, unsigned updateFromIndex)
{
    part->updateIndexData(data.data(), elementCount, updateFromIndex);
}
