/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloLuaCommon.h"
#include "SoloMesh.h"

using namespace solo;

static auto addVertexBuffer(Mesh *mesh, const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32
{
    return mesh->addVertexBuffer(layout, data.data(), vertexCount);
}

static auto addDynamicVertexBuffer(Mesh *mesh, const VertexBufferLayout &layout, const vec<float> &data, u32 vertexCount) -> u32
{
    return mesh->addDynamicVertexBuffer(layout, data.data(), vertexCount);
}

static void updateDynamicVertexBuffer(Mesh *mesh, u32 index, u32 vertexOffset, const vec<float> &data, u32 vertexCount)
{
    mesh->updateDynamicVertexBuffer(index, vertexOffset, data.data(), vertexCount);
}

static auto addPart(Mesh *mesh, const vec<uint16_t> &indexData, u32 indexElementCount) -> u32
{
    return mesh->addPart(indexData.data(), indexElementCount);
}

static void registerVertexBufferLayout(CppBindModule<LuaBinding> &module)
{
    auto el = BEGIN_CLASS(module, VertexAttribute);
    REG_FIELD(el, VertexAttribute, name);
    REG_FIELD(el, VertexAttribute, location);
    REG_FIELD(el, VertexAttribute, elementCount);
    REG_FIELD(el, VertexAttribute, size);
    REG_FIELD(el, VertexAttribute, offset);
    el.endClass();

    auto layout = BEGIN_CLASS(module, VertexBufferLayout);
    REG_CTOR(layout);
    REG_METHOD(layout, VertexBufferLayout, addNamedAttribute);
    REG_METHOD(layout, VertexBufferLayout, addSemanticAttribute);
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
    REG_STATIC_METHOD(mesh, Mesh, loadFromFile);
    REG_STATIC_METHOD(mesh, Mesh, loadFromFileAsync);
    REG_FREE_FUNC_AS_METHOD(mesh, addVertexBuffer);
    REG_FREE_FUNC_AS_METHOD(mesh, addDynamicVertexBuffer);
    REG_FREE_FUNC_AS_METHOD(mesh, updateDynamicVertexBuffer);
    REG_METHOD(mesh, Mesh, removeVertexBuffer);
    REG_FREE_FUNC_AS_METHOD(mesh, addPart);
    REG_METHOD(mesh, Mesh, removePart);
    REG_METHOD(mesh, Mesh, getPartCount);
    REG_METHOD(mesh, Mesh, getPrimitiveType);
    REG_METHOD(mesh, Mesh, setPrimitiveType);
    mesh.endClass();
}

void registerMeshApi(CppBindModule<LuaBinding> &module)
{
    registerMesh(module);
    registerVertexBufferLayout(module);
}
