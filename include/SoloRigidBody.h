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

#pragma once

#include "SoloCommon.h"
#include "SoloComponent.h"
#include "SoloVector3.h"
#include "SoloTransform.h"


namespace solo
{
    class Collider;

    // class because lua binding doesn't like structs :(
    class RigidBodyConstructionParameters
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


    class RigidBody: public ComponentBase<RigidBody>, public TransformCallback
    {
    public:
        static auto create(const Node &node, const RigidBodyConstructionParameters &parameters) -> sptr<RigidBody>;

        virtual void setCollider(sptr<Collider> collider) = 0;

        int lala() { return 1; }

    protected:
        explicit RigidBody(const Node &node);
    };

    template <>
    template <class... Args>
    auto NodeHelper<RigidBody>::addComponent(Scene *scene, uint32_t nodeId, Args &&...args) -> RigidBody*
    {
        auto body = std::shared_ptr<RigidBody>(RigidBody::create(Node(scene, nodeId), std::forward<Args>(args)...));
        scene->addComponent(nodeId, body);
        return body.get();
    }
}
