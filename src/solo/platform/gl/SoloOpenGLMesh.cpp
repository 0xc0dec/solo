/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGLMesh.h"
#include "SoloDevice.h"
#include "SoloOpenGLEffect.h"
#include <algorithm>

using namespace solo;


gl::Mesh::Mesh(Device *device)
{
    renderer = dynamic_cast<Renderer *>(device->getRenderer());
}


gl::Mesh::Mesh(Device *device, MeshData *data):
    Mesh(device)
{
    VertexBufferLayout positionLayout;
    positionLayout.add(VertexBufferLayoutSemantics::Position, 3);
    addVertexBuffer(positionLayout, reinterpret_cast<const float *>(data->vertices.data()), static_cast<uint32_t>(data->vertices.size()));

    if (!data->uvs.empty())
    {
        VertexBufferLayout uvLayout;
        uvLayout.add(VertexBufferLayoutSemantics::TexCoord0, 2);
        addVertexBuffer(uvLayout, reinterpret_cast<const float *>(data->uvs.data()), static_cast<uint32_t>(data->uvs.size()));
    }
    if (!data->normals.empty())
    {
        VertexBufferLayout normalLayout;
        normalLayout.add(VertexBufferLayoutSemantics::Normal, 3);
        addVertexBuffer(normalLayout, reinterpret_cast<const float *>(data->normals.data()), static_cast<uint32_t>(data->normals.size()));
    }

    for (const auto &indices : data->indices)
        addPart(reinterpret_cast<const void *>(indices.data()), static_cast<uint32_t>(indices.size()));

    setPrimitiveType(PrimitiveType::Triangles);
}


gl::Mesh::Mesh(Device *device, MeshPrefab prefab):
    Mesh(device)
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
            SL_PANIC("Unknown mesh prefab type");
            break;
    }
}


gl::Mesh::~Mesh()
{
    if (programBinding != EmptyHandle)
        renderer->destroyVertexProgramBinding(programBinding);
    while (!vertexBuffers.empty())
        removeVertexBuffer(0);
    while (!indexBuffers.empty())
        removePart(0);
}


auto gl::Mesh::addVertexBuffer(uint32_t bufferHandle, const VertexBufferLayout &layout, uint32_t vertexCount) -> uint32_t
{
    vertexBuffers.push_back(bufferHandle);
    vertexCounts.push_back(vertexCount);
    vertexSizes.push_back(layout.getSize());
    recalculateMinVertexCount();
    return static_cast<uint32_t>(vertexBuffers.size() - 1);
}


void gl::Mesh::rebuildEffectBinding(solo::Effect *effect)
{
    if (effect == lastEffect)
        return;
    if (programBinding != EmptyHandle)
        renderer->destroyVertexProgramBinding(programBinding);
    lastEffect = dynamic_cast<Effect *>(effect);
    programBinding = renderer->createVertexProgramBinding(vertexBuffers.data(), static_cast<uint32_t>(vertexBuffers.size()), lastEffect->getHandle());
}


void gl::Mesh::recalculateMinVertexCount()
{
    minVertexCount = std::numeric_limits<uint32_t>::max();
    for (const auto &count : vertexCounts)
        minVertexCount = std::min(count, minVertexCount);
}


auto gl::Mesh::addVertexBuffer(const VertexBufferLayout &layout, const float *data, uint32_t vertexCount) -> uint32_t
{
    auto handle = renderer->createVertexBuffer(layout, data, vertexCount);
    return addVertexBuffer(handle, layout, vertexCount);
}


auto gl::Mesh::addDynamicVertexBuffer(const VertexBufferLayout &layout, const float *data, uint32_t vertexCount) -> uint32_t
{
    auto handle = renderer->createDynamicVertexBuffer(layout, data, vertexCount);
    return addVertexBuffer(handle, layout, vertexCount);
}


void gl::Mesh::updateDynamicVertexBuffer(uint32_t index, uint32_t vertexOffset, const float *data, uint32_t vertexCount)
{
    const auto &handle = vertexBuffers[index];
    auto vertexSize = vertexSizes[index];
    renderer->updateDynamicVertexBuffer(handle, data, vertexOffset * vertexSize, vertexCount * vertexSize);
}


void gl::Mesh::removeVertexBuffer(uint32_t index)
{
    renderer->destroyVertexBuffer(vertexBuffers[index]);
    vertexBuffers.erase(vertexBuffers.begin() + index);
    vertexCounts.erase(vertexCounts.begin() + index);
    vertexSizes.erase(vertexSizes.begin() + index);
    recalculateMinVertexCount();
}


auto gl::Mesh::addPart(const void *data, uint32_t elementCount) -> uint32_t
{
    auto handle = renderer->createIndexBuffer(data, 2, elementCount); // 2 because we currently support only UNSIGNED_SHORT indexes
    indexBuffers.push_back(handle);
    return static_cast<uint32_t>(indexBuffers.size() - 1);
}


void gl::Mesh::removePart(uint32_t part)
{
    renderer->destroyIndexBuffer(indexBuffers[part]);
    indexBuffers.erase(indexBuffers.begin() + part);
}


void gl::Mesh::draw(solo::Effect *effect)
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


void gl::Mesh::drawPart(solo::Effect *effect, uint32_t part)
{
    rebuildEffectBinding(effect);
    if (programBinding != EmptyHandle)
        renderer->drawIndexed(primitiveType, programBinding, indexBuffers[part]);
}

#endif
