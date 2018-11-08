/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloLuaCommon.h"
#include "SoloRigidBody.h"
#include "SoloPhysics.h"
#include "SoloBoxCollider.h"

using namespace solo;

static void registerRigidBodyConstructionParams(CppBindModule<LuaBinding> &module)
{
    auto params = BEGIN_CLASS(module, RigidBodyConstructionParameters);
    REG_CTOR(params);
    REG_FIELD(params, RigidBodyConstructionParameters, mass);
    REG_FIELD(params, RigidBodyConstructionParameters, friction);
    REG_FIELD(params, RigidBodyConstructionParameters, restitution);
    REG_FIELD(params, RigidBodyConstructionParameters, linearDamping);
    REG_FIELD(params, RigidBodyConstructionParameters, angularDamping);
    REG_FIELD(params, RigidBodyConstructionParameters, kinematic);
    REG_FIELD(params, RigidBodyConstructionParameters, linearFactor);
    REG_FIELD(params, RigidBodyConstructionParameters, angularFactor);
    params.endClass();
}

static void registerRigidBody(CppBindModule<LuaBinding> &module)
{
    auto rb = BEGIN_CLASS_EXTEND(module, RigidBody, Component);
    REG_METHOD_NULLABLE_1ST_ARG(rb, RigidBody, setCollider, sptr<Collider>);
    REG_METHOD(rb, RigidBody, isKinematic);
    REG_METHOD(rb, RigidBody, setKinematic);
    rb.endClass();
}

static void registerRayTestResult(CppBindModule<LuaBinding> &module)
{
    auto rcr = BEGIN_CLASS(module, RayTestResult);
    REG_FIELD(rcr, RayTestResult, body);
    REG_FIELD(rcr, RayTestResult, point);
    REG_FIELD(rcr, RayTestResult, normal);
    rcr.endClass();
}

static void registerPhysics(CppBindModule<LuaBinding> &module)
{
    auto ph = BEGIN_CLASS(module, Physics);
    REG_METHOD(ph, Physics, setGravity);
    REG_METHOD(ph, Physics, rayTestFirst);
    REG_METHOD(ph, Physics, rayTestAll);
    ph.endClass();
}

static void registerColliders(CppBindModule<LuaBinding> &module)
{
    auto coll = BEGIN_CLASS(module, Collider);
    coll.endClass();

    auto box = module.beginExtendClass<BoxCollider, Collider>("BoxCollider");
    REG_STATIC_METHOD(box, BoxCollider, create);
    box.endClass();
}

void registerPhysicsApi(CppBindModule<LuaBinding> &module)
{
    registerRigidBodyConstructionParams(module);
    registerRigidBody(module);
    registerRayTestResult(module);
    registerPhysics(module);
    registerColliders(module);
}