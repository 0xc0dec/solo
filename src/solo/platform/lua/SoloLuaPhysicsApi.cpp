/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#include "SoloLuaCommon.h"
#include "SoloRigidBody.h"
#include "SoloPhysics.h"
#include "SoloBoxCollider.h"

using namespace solo;


// Allow passing nulls. TODO make this easier for future uses
static void setCollider(RigidBody *body, LuaRef collider)
{
    auto realCollider = collider.isValid() && collider.toPtr()
        ? collider.toValue<sptr<Collider>>()
        : nullptr;
    body->setCollider(realCollider);
}


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
    REG_FREE_FUNC_AS_METHOD(rb, setCollider);
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