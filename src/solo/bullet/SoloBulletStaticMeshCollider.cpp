/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloBulletStaticMeshCollider.h"
#include "SoloDevice.h"
#include "SoloMesh.h"
#include "SoloMeshData.h"

using namespace solo;

static auto toIndexType(IndexElementSize indexElementSize) -> PHY_ScalarType
{
	switch (indexElementSize)
	{
		case IndexElementSize::Bits16:
			return PHY_SHORT;
		case IndexElementSize::Bits32:
			return PHY_INTEGER;
		default:
			SL_DEBUG_PANIC(true, "Unknown index element size");
			break;
	}
}

// BulletStaticMeshCollider::BulletStaticMeshCollider(sptr<MeshData> data):
//     data_(data)
// {
//     SL_DEBUG_PANIC(!data_->partsCount(), "Collision mesh index data is empty");
//
//     mesh_ = std::make_unique<btIndexedMesh>();
//
// 	mesh_->m_vertexType = PHY_FLOAT;
// 	mesh_->m_numVertices = data_->vertexCount();
// 	mesh_->m_vertexBase = reinterpret_cast<const u8*>(data_->vertexData().data());
//     mesh_->m_vertexStride = 3 * sizeof(float);
//
// 	const auto indexType = toIndexType(data->indexElementSize());
//
// 	mesh_->m_indexType = indexType;
//     mesh_->m_numTriangles = data_->indexElementCount(0) / 3; // TODO this seems to take into account only the first part
//     mesh_->m_triangleIndexBase = static_cast<const u8*>(data_->indexData(0));
//     mesh_->m_triangleIndexStride = 3 * static_cast<int>(data->indexElementSize());
//
//     arr_ = std::make_unique<btTriangleIndexVertexArray>();
//     arr_->addIndexedMesh(*mesh_, indexType);
//
//     shape_ = std::make_unique<btBvhTriangleMeshShape>(arr_.get(), true);
// }

BulletStaticMeshCollider::BulletStaticMeshCollider(sptr<Mesh> mesh):
	inputMesh_(mesh)
{
	SL_DEBUG_PANIC(mesh->vertexBufferCount() > 1, "Only single-buffer meshes are supported for now"); // TODO
	SL_DEBUG_PANIC(mesh->partCount() > 1, "Only single-part meshes are supported for now"); // TODO
	
	mesh_ = std::make_unique<btIndexedMesh>();

	const auto layout = mesh->vertexBufferLayout(0);

	const auto posAttrIdx = layout.attributeIndex(VertexAttributeUsage::Position);
	SL_DEBUG_PANIC(posAttrIdx == -1, "No position attribute found in mesh buffer layout");
	const auto posAttr = layout.attribute(posAttrIdx);
	
	mesh_->m_vertexType = PHY_FLOAT;
	mesh_->m_numVertices = mesh->vertexBufferVertexCount(0);
	mesh_->m_vertexBase = reinterpret_cast<const u8*>(mesh->vertexBufferData(0).data()) + posAttr.offset; // TODO verify
    mesh_->m_vertexStride = layout.size();

	const auto indexType = toIndexType(mesh->partElementSize(0));

	mesh_->m_indexType = indexType;
    mesh_->m_numTriangles = mesh->partElementCount(0) / 3;
    mesh_->m_triangleIndexBase = reinterpret_cast<const u8*>(mesh->partData(0).data());
    mesh_->m_triangleIndexStride = 3 * static_cast<s32>(mesh->partElementSize(0));

	arr_ = std::make_unique<btTriangleIndexVertexArray>();
    arr_->addIndexedMesh(*mesh_, indexType);

    shape_ = std::make_unique<btBvhTriangleMeshShape>(arr_.get(), true);
}
