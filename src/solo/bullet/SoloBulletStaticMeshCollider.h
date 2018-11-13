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
        BulletStaticMeshCollider();

        auto shape() -> btCollisionShape* override final { return shape_.get(); }

    private:
        uptr<btBvhTriangleMeshShape> shape_; // TODO avoid uptr?
    };
}