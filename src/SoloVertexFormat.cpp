#include "SoloVertexFormat.h"

using namespace solo;


VertexFormat::VertexFormat(const std::vector<VertexFormatElement> &elements):
    elements(elements)
{
    if (elements.empty())
        return;

    unsigned maxStorageId = 0;
    std::unordered_set<unsigned> uniqueStorageIds;
    for (const auto &e : elements)
    {
        auto storageId = e.storageId;
        uniqueStorageIds.insert(storageId);
        if (storageId >= maxStorageId)
            maxStorageId = storageId;
        sizesPerStorage[e.storageId] += e.size * sizeof(float);
    }

    if (uniqueStorageIds.size() != maxStorageId + 1)
        SL_THROW_FMT(EngineException, "Vertex format storage ids must consitute a continuous sequence");
}
