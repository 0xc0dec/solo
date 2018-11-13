/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloBulletStaticMeshCollider.h"

using namespace solo;

// TODO remove after testing
struct MeshData
{
    vec<float> vertexData;
    u32 vertexCount = 0;
    vec<vec<u16>> indexData;
};

BulletStaticMeshCollider::BulletStaticMeshCollider()
{
    //MeshData data;

    // btIndexedMesh mesh;

    /*btTriangleIndexVertexArray arr{
        data.vertexCount / 3,
        reinterpret_cast<int*>(data.indexData[0].data()),
        3 * sizeof(u16),
        data.vertexCount,
        data.vertexData.data(),
        3 * sizeof(float)
    }*/;
    
    //btBvhTriangleMeshShape shape{&arr, true};
}
