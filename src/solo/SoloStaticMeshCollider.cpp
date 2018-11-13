/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloStaticMeshCollider.h"
#include "bullet/SoloBulletStaticMeshCollider.h"

using namespace solo;

auto StaticMeshCollider::create() -> sptr<StaticMeshCollider>
{
    return std::make_shared<BulletStaticMeshCollider>();
}
