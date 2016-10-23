#include "SoloOpenGLMesh.h"
#include "SoloDevice.h"
#include "SoloOpenGLEffect.h"
#include <algorithm>

using namespace solo;


OpenGLMesh::OpenGLMesh(Device* device)
{
    renderer = dynamic_cast<OpenGLRenderer*>(device->getRenderer());
}


OpenGLMesh::OpenGLMesh(Device* device, MeshData* data):
    OpenGLMesh(device)
{
    VertexBufferLayout positionLayout;
    positionLayout.add(VertexBufferLayoutSemantics::Position, 3);
    addVertexBuffer(positionLayout, reinterpret_cast<const float*>(data->vertices.data()), static_cast<uint32_t>(data->vertices.size()));

    if (!data->uvs.empty())
    {
        VertexBufferLayout uvLayout;
        uvLayout.add(VertexBufferLayoutSemantics::TexCoord0, 2);
        addVertexBuffer(uvLayout, reinterpret_cast<const float*>(data->uvs.data()), static_cast<uint32_t>(data->uvs.size()));
    }
    if (!data->normals.empty())
    {
        VertexBufferLayout normalLayout;
        normalLayout.add(VertexBufferLayoutSemantics::Normal, 3);
        addVertexBuffer(normalLayout, reinterpret_cast<const float*>(data->normals.data()), static_cast<uint32_t>(data->normals.size()));
    }

    for (const auto& indices : data->indices)
        addPart(reinterpret_cast<const void*>(indices.data()), static_cast<uint32_t>(indices.size()));

    setPrimitiveType(PrimitiveType::Triangles);
}


OpenGLMesh::OpenGLMesh(Device* device, MeshPrefab prefab):
    OpenGLMesh(device)
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
            SL_ERR("Unknown mesh prefab type");
            break;
    }
}


OpenGLMesh::~OpenGLMesh()
{
    if (!programBinding.empty())
        renderer->destroyVertexProgramBinding(programBinding);
    while (!vertexBuffers.empty())
        removeVertexBuffer(0);
    while (!indexBuffers.empty())
        removePart(0);
}


auto OpenGLMesh::addVertexBuffer(VertexBufferHandle bufferHandle, const VertexBufferLayout& layout, uint32_t vertexCount) -> uint32_t
{
    vertexBuffers.push_back(bufferHandle);
    vertexCounts.push_back(vertexCount);
    vertexSizes.push_back(layout.getSize());
    recalculateMinVertexCount();
    return static_cast<uint32_t>(vertexBuffers.size() - 1);
}


void OpenGLMesh::rebuildEffectBinding(Effect* effect)
{
    if (effect == lastEffect)
        return;
    if (!programBinding.empty())
        renderer->destroyVertexProgramBinding(programBinding);
    lastEffect = dynamic_cast<OpenGLEffect*>(effect);
    programBinding = renderer->createVertexProgramBinding(vertexBuffers.data(), static_cast<uint32_t>(vertexBuffers.size()), lastEffect->getHandle());
}


void OpenGLMesh::recalculateMinVertexCount()
{
    minVertexCount = std::numeric_limits<uint32_t>::max();
    for (const auto& count : vertexCounts)
        minVertexCount = std::min(count, minVertexCount);
}


auto OpenGLMesh::addVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount) -> uint32_t
{
    auto handle = renderer->createVertexBuffer(layout, data, vertexCount);
    return addVertexBuffer(handle, layout, vertexCount);
}


auto OpenGLMesh::addDynamicVertexBuffer(const VertexBufferLayout& layout, const float* data, uint32_t vertexCount) -> uint32_t
{
    auto handle = renderer->createDynamicVertexBuffer(layout, data, vertexCount);
    return addVertexBuffer(handle, layout, vertexCount);
}


void OpenGLMesh::updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float* data, uint32_t vertexCount)
{
    const auto& handle = vertexBuffers[index];
    auto vertexSize = vertexSizes[index];
    renderer->updateDynamicVertexBuffer(handle, data, vertexOffset * vertexSize, vertexCount * vertexSize);
}


void OpenGLMesh::removeVertexBuffer(uint32_t index)
{
    renderer->destroyVertexBuffer(vertexBuffers[index]);
    vertexBuffers.erase(vertexBuffers.begin() + index);
    vertexCounts.erase(vertexCounts.begin() + index);
    vertexSizes.erase(vertexSizes.begin() + index);
    recalculateMinVertexCount();
}


auto OpenGLMesh::addPart(const void* data, uint32_t elementCount) -> uint32_t
{
    auto handle = renderer->createIndexBuffer(data, 2, elementCount); // 2 because we currently support only UNSIGNED_SHORT indexes
    indexBuffers.push_back(handle);
    return static_cast<uint32_t>(indexBuffers.size() - 1);
}


void OpenGLMesh::removePart(uint32_t part)
{
    renderer->destroyIndexBuffer(indexBuffers[part]);
    indexBuffers.erase(indexBuffers.begin() + part);
}


void OpenGLMesh::draw(Effect* effect)
{
    rebuildEffectBinding(effect);

    if (indexBuffers.empty())
        renderer->draw(primitiveType, programBinding, minVertexCount);
    else
    {
        for (auto i = 0; i < indexBuffers.size(); i++)
            renderer->drawIndexed(primitiveType, programBinding, indexBuffers[i]);
    }
}


void OpenGLMesh::drawPart(Effect* effect, uint32_t part)
{
    rebuildEffectBinding(effect);
    if (!programBinding.empty())
        renderer->drawIndexed(primitiveType, programBinding, indexBuffers[part]);
}
