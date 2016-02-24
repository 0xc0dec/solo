#include "SoloVertexFormat.h"

using namespace solo;


VertexFormat::VertexFormat(const std::vector<VertexFormatElement>& elements):
    elements(elements)
{
    if (elements.empty())
        return;

    int maxSlot = 0;
    std::unordered_set<int> uniqueSlots;
    for (const auto& e : elements)
    {
        auto slot = e.slot;
        uniqueSlots.insert(slot);
        if (slot >= maxSlot)
            maxSlot = slot;
        sizesPerSlot[e.slot] += e.size * sizeof(float);
    }

    if (uniqueSlots.size() != maxSlot + 1)
        SL_THROW_FMT(InvalidInputException, "Vertex format slots must consitute a continuous sequence");
}
