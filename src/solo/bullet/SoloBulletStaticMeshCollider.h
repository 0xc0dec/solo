/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloBulletCollider.h"
#include "SoloBulletCommon.h"
#include "SoloStaticMeshCollider.h"

namespace solo
{
    class Device;

    class BulletStaticMeshCollider final: public BulletCollider, public StaticMeshCollider
    {
    public:
        static auto fromFileAsync(Device *device, const str &path) -> sptr<BulletStaticMeshCollider>;

        auto shape() -> btCollisionShape* override final { return shape_.get(); }

    private:
        struct MeshData
        {
            vec<float> vertexData;
            u32 vertexCount = 0;
            vec<vec<u16>> indexData;
        };

        uptr<MeshData> data_;
        uptr<btIndexedMesh> mesh_;
        uptr<btTriangleIndexVertexArray> arr_;
        uptr<btBvhTriangleMeshShape> shape_;

        auto loadMeshData(Device *device, const str &path) -> uptr<MeshData>;
    };
}