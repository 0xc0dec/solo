#pragma once

#include "SoloRigidBody.h"


namespace solo
{
    class BulletRigidBody: public RigidBody
    {
    public:

    private:
        friend class RigidBody;

        BulletRigidBody(Node node);
    };
}