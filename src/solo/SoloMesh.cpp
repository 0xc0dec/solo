/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMesh.h"
#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloObjMeshLoader.h"
#include "gl/SoloOpenGLMesh.h"
#include "vk/SoloVulkanMesh.h"
#include "null/SoloNullMesh.h"

using namespace solo;

// TODO remove when proper loading is implemented
static std::vector<sptr<MeshLoader>> meshLoaders;

static void initMeshLoaders(Device *device)
{
    if (!meshLoaders.empty())
        return;
    meshLoaders.push_back(std::make_unique<ObjMeshLoader>(device));
}

static auto getMeshLoader(const std::string &path) -> MeshLoader*
{
    for (const auto &loader : meshLoaders)
    {
        if (loader->isLoadable(path))
            return loader.get();
    }
    SL_PANIC(SL_FMT("No suitable loader found for mesh '", path, "'"));
    return nullptr;
}

auto Mesh::create(Device *device) -> sptr<Mesh>
{
    switch (device->getSetup().mode)
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<gl::Mesh>();
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
            mesh->initAsQuadMesh();
            break;
        case MeshPrefab::Cube:
            mesh->initAsCubeMesh();
            break;
        default:
            SL_PANIC("Unknown mesh prefab type");
            break;
    }

    return mesh;
}

auto Mesh::createFromData(Device *device, MeshData *data) -> sptr<Mesh>
{
    auto mesh = create(device);
    mesh->initFromData(data);
    return mesh;
}

auto Mesh::loadFromFile(Device *device, const std::string &path) -> sptr<Mesh>
{
    initMeshLoaders(device);
    const auto loader = getMeshLoader(path);
    auto data = loader->loadData(path);
    return createFromData(device, data.get());
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
    layout.addAttribute(3, 0);
    layout.addAttribute(2, 1);

    addVertexBuffer(layout, vertices, 6);
    setPrimitiveType(PrimitiveType::Triangles);
}

void Mesh::initAsCubeMesh()
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
    layout.addAttribute(3, 0);
    layout.addAttribute(2, 1);
    addVertexBuffer(layout, vertices, 24);
    addPart(indices, 36);
    setPrimitiveType(PrimitiveType::Triangles);
}

void Mesh::initFromData(MeshData *data)
{
    VertexBufferLayout positionLayout;
    positionLayout.addAttribute(3, 0);
    addVertexBuffer(positionLayout, reinterpret_cast<const float *>(data->vertices.data()), static_cast<uint32_t>(data->vertices.size()));

    if (!data->normals.empty())
    {
        VertexBufferLayout normalLayout;
        normalLayout.addAttribute(3, 1);
        addVertexBuffer(normalLayout, reinterpret_cast<const float *>(data->normals.data()), static_cast<uint32_t>(data->normals.size()));
    }

    if (!data->uvs.empty())
    {
        VertexBufferLayout uvLayout;
        uvLayout.addAttribute(2, 2);
        addVertexBuffer(uvLayout, reinterpret_cast<const float *>(data->uvs.data()), static_cast<uint32_t>(data->uvs.size()));
    }
    

    for (const auto &indices : data->indices)
        addPart(reinterpret_cast<const void *>(indices.data()), static_cast<uint32_t>(indices.size()));

    setPrimitiveType(PrimitiveType::Triangles);
}
