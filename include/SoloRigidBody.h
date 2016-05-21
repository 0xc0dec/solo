#pragma once

#include "SoloComponent.h"
#include "SoloVector3.h"


namespace solo
{
    class Collider;

    struct RigidBodyConstructionParameters
    {
        float mass = 0;
        float friction = 0;
        float restitution = 0;
        float linearDamping;
        float angularDamping;
        bool kinematic = false;
        Vector3 linearFactor;
        Vector3 angularFactor;
    };

    class RigidBody: public ComponentBase<RigidBody>
    {
    public:
        static auto create(const Node& node, const RigidBodyConstructionParameters& parameters) -> sptr<RigidBody>;

        virtual void setCollider(sptr<Collider> collider) = 0;

    protected:
        explicit RigidBody(const Node& node);
    };

    template <>
    template <class... Args>
    auto NodeHelper<RigidBody>::addComponent(Scene* scene, uint32_t nodeId, Args&&... args) -> RigidBody*
    {
        auto body = std::shared_ptr<RigidBody>(RigidBody::create(Node(scene, nodeId), std::forward<Args>(args)...));
        scene->addComponent(nodeId, body);
        return body.get();
    }
}