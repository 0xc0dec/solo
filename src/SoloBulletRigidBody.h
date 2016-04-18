#pragma once

#include "SoloRigidBody.h"
#include <btBulletDynamicsCommon.h>


namespace solo
{
    class BulletRigidBody final: public RigidBody
    {
    public:

    private:
        friend class RigidBody;

        BulletRigidBody(Node node, const RigidBodyConstructionParameters& parameters);

        uptr<btRigidBody> body;
    };
}