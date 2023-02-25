/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloBulletStaticMeshCollider.h"
#include "SoloDevice.h"
#include "SoloMesh.h"

using namespace solo;

static auto toIndexType(IndexElementSize indexElementSize) -> PHY_ScalarType {
    switch (indexElementSize) {
        case IndexElementSize::Bits16:
            return PHY_SHORT;
        case IndexElementSize::Bits32:
            return PHY_INTEGER;
    }

    panic("Unknown index element size");

    return PHY_INTEGER;;
}

// TODO Support meshes with multiple parts/non-indexed meshes
BulletStaticMeshCollider::BulletStaticMeshCollider(sptr<Mesh> mesh):
    inputMesh_(mesh) {
    panicIf(mesh->vertexBufferCount() != 1, "Only single-buffer meshes are supported for now");
    panicIf(mesh->indexBufferCount() != 1, "Only single-index meshes are supported for now");

    mesh_ = std::make_unique<btIndexedMesh>();

    const auto layout = mesh->vertexBufferLayout(0);

    const auto posAttrIdx = layout.attributeIndex(VertexAttributeUsage::Position);
    panicIf(posAttrIdx == -1, "No position attribute found in mesh buffer layout");

    const auto posAttr = layout.attribute(posAttrIdx);

    mesh_->m_vertexType = PHY_FLOAT;
    mesh_->m_numVertices = mesh->vertexBufferVertexCount(0);
    mesh_->m_vertexBase = reinterpret_cast<const u8 *>(mesh->vertexBufferData(0).data()) + posAttr.offset; // TODO verify
    mesh_->m_vertexStride = layout.size();

    const auto indexType = toIndexType(mesh->indexBufferElementSize(0));

    mesh_->m_indexType = indexType;
    mesh_->m_numTriangles = mesh->indexBufferElementCount(0) / 3;
    mesh_->m_triangleIndexBase = reinterpret_cast<const u8 *>(mesh->indexData(0).data());
    mesh_->m_triangleIndexStride = 3 * static_cast<s32>(mesh->indexBufferElementSize(0));

    indexVertexArray_ = std::make_unique<btTriangleIndexVertexArray>();
    indexVertexArray_->addIndexedMesh(*mesh_, indexType);

    shape_ = std::make_unique<btBvhTriangleMeshShape>(indexVertexArray_.get(), true);
}
