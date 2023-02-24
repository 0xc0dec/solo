/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloRigidBody.h"
#include "bullet/SoloBulletRigidBody.h"

using namespace solo;

auto RigidBody::create(const Node &node, const RigidBodyParams &params) -> sptr<RigidBody>
{
    return std::make_shared<BulletRigidBody>(node, params);
}

RigidBody::RigidBody(const Node &node):
    ComponentBase<RigidBody>(node)
{
}
