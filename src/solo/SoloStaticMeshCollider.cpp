/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloStaticMeshCollider.h"
#include "bullet/SoloBulletStaticMeshCollider.h"

using namespace solo;

auto StaticMeshCollider::fromMesh(sptr<Mesh> mesh) -> sptr<StaticMeshCollider>
{
    return std::make_shared<BulletStaticMeshCollider>(mesh);
}
