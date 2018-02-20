/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMesh.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "obj/SoloObjMeshLoading.h"
#include "gl/SoloOpenGLMesh.h"
#include "vk/SoloVulkanMesh.h"
#include "null/SoloNullMesh.h"

using namespace solo;

auto Mesh::create(Device *device) -> sptr<Mesh>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLMesh>();
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanMesh>(device);
#endif
        default:
            return std::make_shared<NullMesh>();
    }
}

auto Mesh::createFromPrefab(Device *device, MeshPrefab prefab) -> sptr<Mesh>
{
    auto mesh = create(device);

    switch (prefab)
    {
        case MeshPrefab::Quad:
            mesh->initAsQuadMesh();
            break;
        case MeshPrefab::Cube:
            mesh->initAsCubeMesh();
            break;
        default:
            panic("Unknown mesh prefab type");
            break;
    }

    return mesh;
}

auto Mesh::loadFromFile(Device *device, const str &path) -> sptr<Mesh>
{
    if (obj::canLoadMesh(path))
        return obj::loadMesh(device, path);
    return panic<nullptr_t>(SL_FMT("Unsupported mesh file ", path));
}

auto Mesh::loadFromFileAsync(Device *device, const str &path) -> sptr<AsyncHandle<Mesh>>
{
    if (obj::canLoadMesh(path))
        return obj::loadMeshAsync(device, path);
    return panic<nullptr_t>(SL_FMT("Unsupported mesh file ", path));
}

void Mesh::initAsQuadMesh()
{
    float vertices[] =
    {
        -1, -1, 0,  0, 0,
        -1, 1, 0,   0, 1,
        1, 1, 0,    1, 1,
        -1, -1, 0,  0, 0,
        1, 1, 0,    1, 1,
        1, -1, 0,   1, 0
    };

    VertexBufferLayout layout;
    layout.addSemanticAttribute(VertexAttributeSemantics::Position);
    layout.addSemanticAttribute(VertexAttributeSemantics::TexCoord);

    addVertexBuffer(layout, vertices, 6);
    setPrimitiveType(PrimitiveType::Triangles);
}

void Mesh::initAsCubeMesh()
{
    float vertices[] =
    {
        -1, -1,  1,  0,  0,  1, 0, 0,    -1,  1,  1,  0,  0,  1, 0, 1,      1,  1,  1,  0,  0,  1, 1, 1,     1, -1,  1,  0,  0,  1, 1, 0,
        -1, -1, -1, -1,  0,  0, 0, 0,    -1,  1, -1, -1,  0,  0, 0, 1,     -1,  1,  1, -1,  0,  0, 1, 1,    -1, -1,  1, -1,  0,  0, 1, 0,
         1, -1, -1,  0,  0, -1, 0, 0,     1,  1, -1,  0,  0, -1, 0, 1,     -1,  1, -1,  0,  0, -1, 1, 1,    -1, -1, -1,  0,  0, -1, 1, 0,
         1, -1,  1,  1,  0,  0, 0, 0,     1,  1,  1,  1,  0,  0, 0, 1,      1,  1, -1,  1,  0,  0, 1, 1,     1, -1, -1,  1,  0,  0, 1, 0,
        -1,  1,  1,  0,  1,  0, 0, 0,    -1,  1, -1,  0,  1,  0, 0, 1,      1,  1, -1,  0,  1,  0, 1, 1,     1,  1,  1,  0,  1,  0, 1, 0,
        -1, -1, -1,  0, -1,  0, 0, 0,    -1, -1,  1,  0, -1,  0, 0, 1,      1, -1,  1,  0, -1,  0, 1, 1,     1, -1, -1,  0, -1,  0, 1, 0
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
    layout.addSemanticAttribute(VertexAttributeSemantics::Position);
    layout.addSemanticAttribute(VertexAttributeSemantics::Normal);
    layout.addSemanticAttribute(VertexAttributeSemantics::TexCoord);

    addVertexBuffer(layout, vertices, 24);
    addPart(indices, 36);
    setPrimitiveType(PrimitiveType::Triangles);
}
