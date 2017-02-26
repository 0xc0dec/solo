/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloLuaCommon.h"
#include "SoloMesh.h"

using namespace solo;


static auto addVertexBuffer(Mesh *mesh, const VertexBufferLayout &layout, const std::vector<float> &data, uint32_t vertexCount) -> uint32_t
{
    return mesh->addVertexBuffer(layout, data.data(), vertexCount);
}


static auto addDynamicVertexBuffer(Mesh *mesh, const VertexBufferLayout &layout, const std::vector<float> &data, uint32_t vertexCount) -> uint32_t
{
    return mesh->addDynamicVertexBuffer(layout, data.data(), vertexCount);
}


static void updateDynamicVertexBuffer(Mesh *mesh, uint32_t index, uint32_t vertexOffset, const std::vector<float> &data, uint32_t vertexCount)
{
    mesh->updateDynamicVertexBuffer(index, vertexOffset, data.data(), vertexCount);
}


static auto addPart(Mesh *mesh, const std::vector<uint16_t> &indexData, uint32_t indexElementCount) -> uint32_t
{
    return mesh->addPart(indexData.data(), indexElementCount);
}


static void registerVertexBufferLayout(CppBindModule<LuaBinding> &module)
{
    auto el = BEGIN_CLASS(module, VertexAttribute);
    REG_VARIABLE(el, VertexAttribute, location);
    REG_VARIABLE(el, VertexAttribute, elementCount);
    REG_VARIABLE(el, VertexAttribute, size);
    el.endClass();

    auto layout = BEGIN_CLASS(module, VertexBufferLayout);
    REG_CTOR(layout);
    REG_METHOD(layout, VertexBufferLayout, addAttribute);
    REG_METHOD(layout, VertexBufferLayout, getAttribute);
    REG_METHOD(layout, VertexBufferLayout, getAttributeCount);
    REG_METHOD(layout, VertexBufferLayout, getSize);
    layout.endClass();
}


static void registerMesh(CppBindModule<LuaBinding> &module)
{
    auto mesh = BEGIN_CLASS(module, Mesh);
    REG_STATIC_METHOD(mesh, Mesh, create);
    REG_STATIC_METHOD(mesh, Mesh, createFromPrefab);
    REG_STATIC_METHOD(mesh, Mesh, createFromData);
    REG_FREE_FUNC_AS_METHOD(mesh, addVertexBuffer);
    REG_FREE_FUNC_AS_METHOD(mesh, addDynamicVertexBuffer);
    REG_FREE_FUNC_AS_METHOD(mesh, updateDynamicVertexBuffer);
    REG_METHOD(mesh, Mesh, removeVertexBuffer);
    REG_FREE_FUNC_AS_METHOD(mesh, addPart);
    REG_METHOD(mesh, Mesh, removePart);
    REG_METHOD(mesh, Mesh, getPartCount);
    REG_METHOD(mesh, Mesh, getPrimitiveType);
    REG_METHOD(mesh, Mesh, setPrimitiveType);
}


static void registerMeshData(CppBindModule<LuaBinding> &module)
{
    auto data = BEGIN_CLASS(module, MeshData);
    REG_CTOR(data);
    REG_VARIABLE(data, MeshData, vertices);
    REG_VARIABLE(data, MeshData, uvs);
    REG_VARIABLE(data, MeshData, normals);
    REG_VARIABLE(data, MeshData, indices);
    data.endClass();
}


void registerMeshApi(CppBindModule<LuaBinding> &module)
{
    registerMesh(module);
    registerVertexBufferLayout(module);
    registerMeshData(module);
}
