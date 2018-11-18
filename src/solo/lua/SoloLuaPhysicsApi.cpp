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
    auto params = BEGIN_CLASS(module, RigidBodyParams);
    REG_CTOR(params);
    REG_FIELD(params, RigidBodyParams, mass);
    REG_FIELD(params, RigidBodyParams, friction);
    REG_FIELD(params, RigidBodyParams, restitution);
    REG_FIELD(params, RigidBodyParams, linearDamping);
    REG_FIELD(params, RigidBodyParams, angularDamping);
    REG_FIELD(params, RigidBodyParams, kinematic);
    REG_FIELD(params, RigidBodyParams, linearFactor);
    REG_FIELD(params, RigidBodyParams, angularFactor);
    params.endClass();
}

static void registerRigidBody(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS_EXTEND(module, RigidBody, Component);
    REG_METHOD_NULLABLE_1ST_ARG(binding, RigidBody, setCollider, sptr<Collider>);
    REG_METHOD(binding, RigidBody, isKinematic);
    REG_METHOD(binding, RigidBody, setKinematic);
    REG_PTR_EQUALITY(binding, RigidBody);
    binding.endClass();
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
    auto binding = BEGIN_CLASS(module, Physics);
    REG_METHOD(binding, Physics, setGravity);
    REG_METHOD(binding, Physics, rayTestFirst);
    REG_METHOD(binding, Physics, rayTestAll);
    REG_PTR_EQUALITY(binding, Physics);
    binding.endClass();
}

static void registerColliders(CppBindModule<LuaBinding> &module)
{
    auto coll = BEGIN_CLASS(module, Collider);
    coll.endClass();

    auto binding = module.beginExtendClass<BoxCollider, Collider>("BoxCollider");
    REG_STATIC_METHOD(binding, BoxCollider, create);
    REG_PTR_EQUALITY(binding, BoxCollider);
    binding.endClass();
}

void registerPhysicsApi(CppBindModule<LuaBinding> &module)
{
    registerRigidBodyConstructionParams(module);
    registerRigidBody(module);
    registerRayTestResult(module);
    registerPhysics(module);
    registerColliders(module);
}