/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include "SoloCommon.h"
#include "SoloComponent.h"
#include "math/SoloVector3.h"
#include "SoloTransform.h"

namespace solo
{
    class Collider;

    // class because lua binding doesn't like structs :(
    class RigidBodyParams
    {
    public:
        float mass = 0;
        float friction = 0;
        float restitution = 0;
        float linearDamping = 0;
        float angularDamping = 0;

        bool kinematic = false;

        Vector3 linearFactor;
        Vector3 angularFactor;
    };

    class RigidBody: public ComponentBase<RigidBody>
    {
    public:
        static auto create(const Node &node, const RigidBodyParams &params) -> sptr<RigidBody>;

        virtual void setCollider(sptr<Collider> collider) = 0;
        // TODO add collider() method

        virtual bool isKinematic() = 0;
        virtual void setKinematic(bool kinematic) = 0;

        virtual bool isStatic() = 0;
        virtual void setStatic(bool isStatic) = 0;

    protected:
        explicit RigidBody(const Node &node);
    };

    template <>
    template <class... Args>
    auto NodeHelper<RigidBody>::addComponent(Scene *scene, u32 nodeId, Args &&...args) -> RigidBody *
    {
        const auto body = std::shared_ptr<RigidBody>(RigidBody::create(Node(scene, nodeId), std::forward<Args>(args)...));
        scene->addComponent(nodeId, body);
        return body.get();
    }
}
