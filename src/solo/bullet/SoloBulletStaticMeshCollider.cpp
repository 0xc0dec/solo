/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloBulletStaticMeshCollider.h"
#include "SoloDevice.h"
#include "SoloMeshData.h"

using namespace solo;

BulletStaticMeshCollider::BulletStaticMeshCollider(sptr<MeshData> data):
    data_(data)
{
    // TODO more versatile?
    mesh_ = std::make_unique<btIndexedMesh>();
    mesh_->m_indexType = PHY_SHORT;
    mesh_->m_vertexType = PHY_FLOAT;
    mesh_->m_numTriangles = data_->indexData()[0].size() / 3;
    mesh_->m_numVertices = data_->vertexCount();
    mesh_->m_triangleIndexBase = reinterpret_cast<const u8*>(data_->indexData()[0].data());
    mesh_->m_triangleIndexStride = 3 * sizeof(u16);
    mesh_->m_vertexBase = reinterpret_cast<const u8*>(data_->vertexData().data());
    mesh_->m_vertexStride = 3 * sizeof(float);

    arr_ = std::make_unique<btTriangleIndexVertexArray>();
    arr_->addIndexedMesh(*mesh_, PHY_SHORT); // TODO support 32bit indices

    shape_ = std::make_unique<btBvhTriangleMeshShape>(arr_.get(), true);
}
