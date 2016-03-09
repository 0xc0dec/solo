#include "SoloMesh.h"
#include "SoloEffect.h"
#include <algorithm>

using namespace solo;


shared<Mesh> Mesh::createPrefab(Renderer* renderer, MeshPrefab prefab)
{
    switch (prefab)
    {
        case MeshPrefab::Quad:
            return createQuadMesh(renderer);
        case MeshPrefab::Cube:
            return createBoxMesh(renderer);
        default:
            SL_THROW_FMT(InvalidInputException, "Unknown mesh prefab");
    }
}


Mesh::Mesh(Renderer* renderer):
    renderer(renderer)
{
}


Mesh::~Mesh()
{
    while (!vertexBuffers.empty())
        removeBuffer(0);
    while (!indexBuffers.empty())
        removeIndex(0);
}


int Mesh::addBuffer(const VertexBufferLayout& layout, const float* data, int vertexCount)
{
    auto handle = renderer->createVertexBuffer(layout, data, vertexCount);
    vertexBuffers.push_back(handle);
    vertexCounts.push_back(vertexCount);
    rebuildVertexObject();
    recalculateMinVertexCount();
    return vertexBuffers.size() - 1;
}


void Mesh::removeBuffer(int index)
{
    SL_DEBUG_FMT_THROW_IF(index < 0 || index >= vertexBuffers.size(), InvalidInputException, "Invalid buffer index")
    renderer->destroyVertexBuffer(vertexBuffers[index]);
    vertexBuffers.erase(vertexBuffers.begin() + index);
    vertexCounts.erase(vertexCounts.begin() + index);
    rebuildVertexObject();
    recalculateMinVertexCount();
}


int Mesh::addIndex(const void* data, int elementCount)
{
    auto handle = renderer->createIndexBuffer(data, 2, elementCount); // 2 because we currently support only UNSIGNED_SHORT indexes
    indexBuffers.push_back(handle);
    return indexBuffers.size() - 1;
}


void Mesh::removeIndex(int index)
{
    SL_DEBUG_FMT_THROW_IF(index < 0 || index >= indexBuffers.size(), InvalidInputException, "Invalid index index")
    renderer->destroyIndexBuffer(indexBuffers[index]);
    indexBuffers.erase(indexBuffers.begin() + index);
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
        vertexObjectHandle = renderer->createVertexObject(vertexBuffers.data(), vertexBuffers.size(), EmptyProgramHandle);
}


void Mesh::recalculateMinVertexCount()
{
    minVertexCount = INT_MAX;
    for (const auto& count : vertexCounts)
        minVertexCount = std::min(count, minVertexCount);
}


MeshEffectBinding Mesh::createEffectBinding(Effect* effect)
{
    SL_DEBUG_FMT_THROW_IF(!effect, InvalidInputException, "No effect specified")
    const auto vo = renderer->createVertexObject(vertexBuffers.data(), vertexBuffers.size(), effect->getHandle());
    return MeshEffectBinding{ vo };
}


void Mesh::draw(MeshEffectBinding* effectBinding = nullptr)
{
    const auto& handle = effectBinding ? effectBinding->vertexObjectHandle : vertexObjectHandle;
    if (!handle.empty())
        renderer->renderVertexObject(primitiveType, handle, minVertexCount);
}


void Mesh::drawIndex(int index, MeshEffectBinding* effectBinding = nullptr)
{
    SL_DEBUG_FMT_THROW_IF(index < 0 || index >= indexBuffers.size(), InvalidInputException, "Invalid index")
    const auto& handle = effectBinding ? effectBinding->vertexObjectHandle : vertexObjectHandle;
    if (!handle.empty())
        renderer->renderIndexedVertexObject(primitiveType, handle, indexBuffers[index]);
}


shared<Mesh> Mesh::createQuadMesh(Renderer* renderer)
{
    float data[] =
    {
        -1, -1, 0,  0, 0, -1,   0, 0,
        -1, 1, 0,   0, 0, -1,   0, 1,
        1, 1, 0,    0, 0, -1,   1, 1,
        1, -1, 0,   0, 0, -1,   1, 0
    };
    unsigned short indexData[] =
    {
        0, 1, 2, 0, 2, 3
    };

    VertexBufferLayout layout;
    layout.add(VertexBufferLayoutSemantics::Position, 3);
    layout.add(VertexBufferLayoutSemantics::Normal, 3);
    layout.add(VertexBufferLayoutSemantics::TexCoord0, 2);

    auto mesh = SL_NEW_SHARED(Mesh, renderer);
    mesh->addBuffer(layout, data, 4);
    mesh->addIndex(indexData, 6);
    mesh->setPrimitiveType(PrimitiveType::Triangles);

    return mesh;
}


shared<Mesh> Mesh::createBoxMesh(Renderer* renderer)
{
    float positionData[] =
    {
        -1, -1, 1,      -1, 1, 1,       1, 1, 1,        1, -1, 1,
        -1, -1, -1,     -1, 1, -1,      -1, 1, 1,       -1, -1, 1,
        1, -1, -1,      1, 1, -1,       -1, 1, -1,      -1, -1, -1,
        1, -1, 1,       1, 1, 1,        1, 1, -1,       1, -1, -1,
        -1, 1, 1,       -1, 1, -1,      1, 1, -1,       1, 1, 1,
        -1, -1, -1,     -1, -1, 1,      1, -1, 1,       1, -1, -1,
    };
    float texCoordData[] =
    {
        0, 0,   0, 1,   1, 1,   1, 0,
        0, 0,   0, 1,   1, 1,   1, 0,
        0, 0,   0, 1,   1, 1,   1, 0,
        0, 0,   0, 1,   1, 1,   1, 0,
        0, 0,   0, 1,   1, 1,   1, 0,
        0, 0,   0, 1,   1, 1,   1, 0
    };
    unsigned short indexData[] =
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

    VertexBufferLayout layout1;
    layout1.add(VertexBufferLayoutSemantics::Position, 3);

    VertexBufferLayout layout2;
    layout2.add(VertexBufferLayoutSemantics::TexCoord0, 2);

    auto mesh = SL_NEW_SHARED(Mesh, renderer);
    mesh->addBuffer(layout1, positionData, 24);
    mesh->addBuffer(layout2, texCoordData, 24);
    mesh->addIndex(indexData, 36);
    mesh->setPrimitiveType(PrimitiveType::Triangles);

    return mesh;
}
