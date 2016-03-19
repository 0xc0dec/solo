#include "SoloMesh.h"
#include "SoloEffect.h"
#include <algorithm>
#include <limits>

using namespace solo;


Mesh::Mesh(Renderer* renderer):
    renderer(renderer)
{
}


Mesh::Mesh(Renderer* renderer, MeshPrefab prefab):
    renderer(renderer)
{
    switch (prefab)
    {
        case MeshPrefab::Quad:
            initQuadMesh();
            break;
        case MeshPrefab::Cube:
            initCubeMesh();
            break;
        default:
            SL_DEBUG_THROW(InvalidInputException, "Unknown mesh prefab");
    }
}


Mesh::~Mesh()
{
    if (!vertexObjectHandle.empty())
        renderer->destroyVertexObject(vertexObjectHandle);
    if (!effectBindingVertexObjectHandle.empty())
        renderer->destroyVertexObject(effectBindingVertexObjectHandle);
    while (!vertexBuffers.empty())
        removeBuffer(0);
    while (!indexBuffers.empty())
        removePart(0);
}


uint32_t Mesh::addBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount)
{
    auto handle = renderer->createVertexBuffer(layout, data, vertexCount);
    vertexBuffers.push_back(handle);
    vertexCounts.push_back(vertexCount);
    rebuildVertexObject();
    recalculateMinVertexCount();
    return static_cast<uint32_t>(vertexBuffers.size() - 1);
}


uint32_t Mesh::addDynamicBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount)
{
    auto handle = renderer->createDynamicVertexBuffer(layout, data, vertexCount);
    vertexBuffers.push_back(handle);
    vertexCounts.push_back(vertexCount);
    rebuildVertexObject();
    recalculateMinVertexCount();
    return static_cast<uint32_t>(vertexBuffers.size() - 1);
}


void Mesh::updateDynamicBuffer(uint32_t index, uint32_t offset, const float* data, uint32_t vertexCount)
{
    const auto& handle = vertexBuffers[index];
    renderer->updateDynamicVertexBuffer(handle, data, offset, vertexCount);
}


void Mesh::removeBuffer(uint32_t index)
{
    renderer->destroyVertexBuffer(vertexBuffers[index]);
    vertexBuffers.erase(vertexBuffers.begin() + index);
    vertexCounts.erase(vertexCounts.begin() + index);
    rebuildVertexObject();
    recalculateMinVertexCount();
}


uint32_t Mesh::addPart(const void* data, uint32_t elementCount)
{
    auto handle = renderer->createIndexBuffer(data, 2, elementCount); // 2 because we currently support only UNSIGNED_SHORT indexes
    indexBuffers.push_back(handle);
    return static_cast<uint32_t>(indexBuffers.size() - 1);
}


void Mesh::removePart(uint32_t part)
{
    renderer->destroyIndexBuffer(indexBuffers[part]);
    indexBuffers.erase(indexBuffers.begin() + part);
}


void Mesh::draw(Effect* effect)
{
    rebuildEffectBinding(effect);

    const auto& voHandle = !effectBindingVertexObjectHandle.empty() ? effectBindingVertexObjectHandle : vertexObjectHandle;
    if (voHandle.empty())
        return;

    if (indexBuffers.empty())
        renderer->drawVertexObject(primitiveType, voHandle, minVertexCount);
    else
    {
        for (auto i = 0; i < indexBuffers.size(); i++)
            renderer->drawIndexedVertexObject(primitiveType, voHandle, indexBuffers[i]);
    }
}


void Mesh::drawPart(Effect* effect, uint32_t part)
{
    rebuildEffectBinding(effect);
    const auto& handle = !effectBindingVertexObjectHandle.empty() ? effectBindingVertexObjectHandle : vertexObjectHandle;
    if (!handle.empty())
        renderer->drawIndexedVertexObject(primitiveType, handle, indexBuffers[part]);
}


void Mesh::rebuildVertexObject()
{
    if (!vertexObjectHandle.empty())
    {
        renderer->destroyVertexObject(vertexObjectHandle);
        vertexObjectHandle = EmptyVertexObjectHandle;
    }
    // Build with default vertex attribute values
    if (!vertexBuffers.empty())
        vertexObjectHandle = renderer->createVertexObject(vertexBuffers.data(), static_cast<uint32_t>(vertexBuffers.size()), EmptyProgramHandle);
}


void Mesh::rebuildEffectBinding(Effect* effect)
{
    if (effect == lastEffect)
        return;
    if (!effectBindingVertexObjectHandle.empty())
        renderer->destroyVertexObject(effectBindingVertexObjectHandle);
    effectBindingVertexObjectHandle = renderer->createVertexObject(vertexBuffers.data(), static_cast<uint32_t>(vertexBuffers.size()), effect->getHandle());
    lastEffect = effect;
}


void Mesh::recalculateMinVertexCount()
{
    minVertexCount = std::numeric_limits<uint32_t>::max();
    for (const auto& count : vertexCounts)
        minVertexCount = std::min(count, minVertexCount);
}


void Mesh::initQuadMesh()
{
    float data[] =
    {
        -1, -1, 0,  0, 0, -1,   0, 0,
        -1, 1, 0,   0, 0, -1,   0, 1,
        1, 1, 0,    0, 0, -1,   1, 1,
        -1, -1, 0,  0, 0, -1,   0, 0,
        1, 1, 0,    0, 0, -1,   1, 1,
        1, -1, 0,   0, 0, -1,   1, 0
    };

    VertexBufferLayout layout;
    layout.add(VertexBufferLayoutSemantics::Position, 3);
    layout.add(VertexBufferLayoutSemantics::Normal, 3);
    layout.add(VertexBufferLayoutSemantics::TexCoord0, 2);

    addBuffer(layout, data, 6);
    setPrimitiveType(PrimitiveType::Triangles);
}


void Mesh::initCubeMesh()
{
    float positionData[] =
    {
        -1, -1, 1,  0, 0,    -1, 1, 1,  0, 1,     1, 1, 1,   1, 1,     1, -1, 1,   1, 0,
        -1, -1, -1, 0, 0,    -1, 1, -1, 0, 1,     -1, 1, 1,  1, 1,     -1, -1, 1,  1, 0,
        1, -1, -1,  0, 0,    1, 1, -1,  0, 1,     -1, 1, -1, 1, 1,     -1, -1, -1, 1, 0,
        1, -1, 1,   0, 0,    1, 1, 1,   0, 1,     1, 1, -1,  1, 1,     1, -1, -1,  1, 0,
        -1, 1, 1,   0, 0,    -1, 1, -1, 0, 1,     1, 1, -1,  1, 1,     1, 1, 1,    1, 0,
        -1, -1, -1, 0, 0,    -1, -1, 1, 0, 1,     1, -1, 1,  1, 1,     1, -1, -1,  1, 0
    };
    uint16_t indexData[] =
    {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
        8, 9, 10,
        8, 10, 11,
        12, 13, 14,
        12, 14, 15,
        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23
    };

    VertexBufferLayout layout;
    layout.add(VertexBufferLayoutSemantics::Position, 3);
    layout.add(VertexBufferLayoutSemantics::TexCoord0, 2);
    addBuffer(layout, positionData, 24);
    addPart(indexData, 36);
    setPrimitiveType(PrimitiveType::Triangles);
}
