/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMesh.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "platform/gl/SoloOpenGLMesh.h"
#include "platform/vk/SoloVulkanMesh.h"
#include "platform/null/SoloNullMesh.h"

using namespace solo;


auto Mesh::create(Device *device) -> sptr<Mesh>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Mesh>(device);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<vk::Mesh>(device);
#endif
        default:
            return std::make_shared<null::Mesh>();
    }
}


auto Mesh::createFromPrefab(Device *device, MeshPrefab prefab) -> sptr<Mesh>
{
    auto mesh = create(device);

    switch (prefab)
    {
        case MeshPrefab::Quad:
            mesh->initQuadMesh();
            break;
        case MeshPrefab::Cube:
            mesh->initCubeMesh();
            break;
        default:
            SL_PANIC("Unknown mesh prefab type");
            break;
    }

    return mesh;
}


auto Mesh::createFromData(Device *device, MeshData *data) -> sptr<Mesh>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Mesh>(device, data);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<vk::Mesh>(device, data);
#endif
        default:
            return std::make_shared<null::Mesh>();
    }
}


void Mesh::initQuadMesh()
{
    float vertices[] =
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

    addVertexBuffer(layout, vertices, 6);
    setPrimitiveType(PrimitiveType::Triangles);
}


void Mesh::initCubeMesh()
{
    float vertices[] =
    {
        -1, -1, 1,  0, 0,    -1, 1, 1,  0, 1,     1, 1, 1,   1, 1,     1, -1, 1,   1, 0,
        -1, -1, -1, 0, 0,    -1, 1, -1, 0, 1,     -1, 1, 1,  1, 1,     -1, -1, 1,  1, 0,
        1, -1, -1,  0, 0,    1, 1, -1,  0, 1,     -1, 1, -1, 1, 1,     -1, -1, -1, 1, 0,
        1, -1, 1,   0, 0,    1, 1, 1,   0, 1,     1, 1, -1,  1, 1,     1, -1, -1,  1, 0,
        -1, 1, 1,   0, 0,    -1, 1, -1, 0, 1,     1, 1, -1,  1, 1,     1, 1, 1,    1, 0,
        -1, -1, -1, 0, 0,    -1, -1, 1, 0, 1,     1, -1, 1,  1, 1,     1, -1, -1,  1, 0
    };
    uint16_t indices[] =
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
    addVertexBuffer(layout, vertices, 24);
    addPart(indices, 36);
    setPrimitiveType(PrimitiveType::Triangles);
}
