/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloBulletStaticMeshCollider.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace solo;

//auto BulletStaticMeshCollider::loadMeshData(Device *device, const str &path) -> uptr<MeshData>
//{
//    // TODO Implement proper io system for assimp to avoid loading file into memory
//    const auto bytes = device->fileSystem()->readBytes(path);
//
//    Assimp::Importer importer;
//    const auto flags = aiProcess_Triangulate;
//    const auto scene = importer.ReadFileFromMemory(bytes.data(), bytes.size(), flags);
//    SL_DEBUG_PANIC(!scene, "Unable to parse file ", path);
//
//    MeshData data;
//    u16 indexBase = 0;
//
//    // TODO resize vertices beforehand
//    for (u32 i = 0; i < scene->mNumMeshes; i++)
//    {
//        const aiMesh *mesh = scene->mMeshes[i];
//
//        for (u32 j = 0; j < mesh->mNumVertices; j++)
//        {
//            const auto pos = &mesh->mVertices[j];
//            data.vertexData.push_back(pos->x);
//            data.vertexData.push_back(pos->y);
//            data.vertexData.push_back(pos->z);
//            data.vertexCount++;
//        }
//
//        vec<u16> part;
//        part.resize(mesh->mNumFaces * 3);
//        for (u32 j = 0; j < mesh->mNumFaces; j++)
//        {
//            const auto &face = mesh->mFaces[j];
//            if (face.mNumIndices == 3)
//            {
//                const auto startIdx = j * 3;
//                part[startIdx] = indexBase + face.mIndices[0];
//                part[startIdx + 1] = indexBase + face.mIndices[1];
//                part[startIdx + 2] = indexBase + face.mIndices[2];
//            }
//        }
//
//        indexBase += mesh->mNumFaces * 3;
//        data.indexData.emplace_back(std::move(part));
//    }
//
//    return std::make_unique<MeshData>(std::move(data));
//}

auto BulletStaticMeshCollider::fromFileAsync(Device *device, const str &path) -> sptr<BulletStaticMeshCollider>
{
    auto collider = std::make_shared<BulletStaticMeshCollider>();

    return collider;
}

//BulletStaticMeshCollider::BulletStaticMeshCollider()
//{
    /*data_ = loadMeshData(device, "../../assets/meshes/backdrop.obj");

    mesh_ = std::make_unique<btIndexedMesh>();
    mesh_->m_indexType = PHY_SHORT;
    mesh_->m_vertexType = PHY_FLOAT;
    mesh_->m_numTriangles = data_->indexData[0].size() / 3;
    mesh_->m_numVertices = data_->vertexCount;
    mesh_->m_triangleIndexBase = reinterpret_cast<const u8*>(data_->indexData[0].data());
    mesh_->m_triangleIndexStride = 3 * sizeof(u16);
    mesh_->m_vertexBase = reinterpret_cast<const u8*>(data_->vertexData.data());
    mesh_->m_vertexStride = 3 * sizeof(float);

    arr_ = std::make_unique<btTriangleIndexVertexArray>();
    arr_->addIndexedMesh(*mesh_, PHY_SHORT);

    shape_ = std::make_unique<btBvhTriangleMeshShape>(arr_.get(), true);*/
//}
