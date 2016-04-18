#pragma once

#include "SoloComponent.h"


namespace solo
{
    class RigidBody: public ComponentBase<RigidBody>
    {
    public:
        static uptr<RigidBody> create(Node node);

    protected:
        RigidBody(Node node);
    };
}