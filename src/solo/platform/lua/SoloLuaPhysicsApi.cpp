/*
    Copyright (c) Aleksey Fedotov
    MIT license
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
    REG_VARIABLE(params, RigidBodyConstructionParameters, mass);
    REG_VARIABLE(params, RigidBodyConstructionParameters, friction);
    REG_VARIABLE(params, RigidBodyConstructionParameters, restitution);
    REG_VARIABLE(params, RigidBodyConstructionParameters, linearDamping);
    REG_VARIABLE(params, RigidBodyConstructionParameters, angularDamping);
    REG_VARIABLE(params, RigidBodyConstructionParameters, kinematic);
    REG_VARIABLE(params, RigidBodyConstructionParameters, linearFactor);
    REG_VARIABLE(params, RigidBodyConstructionParameters, angularFactor);
    params.endClass();
}


static void registerRigidBody(CppBindModule<LuaBinding> &module)
{
    auto rb = BEGIN_CLASS_EXTEND(module, RigidBody, Component);
    REG_METHOD_NULLABLE_1ST_ARG(rb, RigidBody, setCollider, sptr<Collider>);
    rb.endClass();
}


static void registerRaycastResult(CppBindModule<LuaBinding> &module)
{
    auto rcr = BEGIN_CLASS(module, RaycastResult);
    REG_VARIABLE(rcr, RaycastResult, body);
    REG_VARIABLE(rcr, RaycastResult, point);
    REG_VARIABLE(rcr, RaycastResult, normal);
    rcr.endClass();
}


static void registerPhysics(CppBindModule<LuaBinding> &module)
{
    auto ph = BEGIN_CLASS(module, Physics);
    REG_METHOD(ph, Physics, update);
    REG_METHOD(ph, Physics, setGravity);
    REG_METHOD(ph, Physics, castRay);
    REG_METHOD(ph, Physics, castRayAll);
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
    registerRaycastResult(module);
    registerPhysics(module);
    registerColliders(module);
}