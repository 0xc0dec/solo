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
    class BulletStaticMeshCollider final: public BulletCollider, public StaticMeshCollider
    {
    public:
    	explicit BulletStaticMeshCollider(sptr<Mesh> mesh);

        auto shape() -> btCollisionShape* override { return shape_.get(); }

    private:
    	sptr<Mesh> inputMesh_;
        uptr<btIndexedMesh> mesh_;
        uptr<btTriangleIndexVertexArray> indexVertexArray_;
        uptr<btBvhTriangleMeshShape> shape_;
    };
}