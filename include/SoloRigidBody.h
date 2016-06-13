#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloVector3.h"
#include "SoloTransform.h"


namespace solo
{
    class Collider;

    SL_FLUENT_DTO(RigidBodyConstructionParameters,
        SL_FLUENT_DTO_FIELD(float, mass, withMass, = 0)
        SL_FLUENT_DTO_FIELD(float, friction, withFriction, = 0)
        SL_FLUENT_DTO_FIELD(float, restitution, withRestitution, = 0)
        SL_FLUENT_DTO_FIELD(float, linearDamping, withLinearDamping, = 0)
        SL_FLUENT_DTO_FIELD(float, angularDamping, withAngularDamping, = 0)
        SL_FLUENT_DTO_FIELD(bool, kinematic, withKinematic, = false)
        SL_FLUENT_DTO_FIELD(Vector3, linearFactor, withLinearFactor, = Vector3())
        SL_FLUENT_DTO_FIELD(Vector3, angularFactor, withAngularFactor, = Vector3())
    )

    class RigidBody: public ComponentBase<RigidBody>, public TransformCallback
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
