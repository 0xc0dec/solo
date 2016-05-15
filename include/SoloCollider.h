#pragma once

#include "SoloComponent.h"


namespace solo
{
    class Collider
    {
    public:
        virtual ~Collider() {}
        SL_NONCOPYABLE(Collider)

    protected:
        Collider() {}
    };
}