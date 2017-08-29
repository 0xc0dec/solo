/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloComponent.h"

namespace solo
{
    class Collider
    {
    public:
        SL_DISABLE_COPY_AND_MOVE(Collider)

        virtual ~Collider() {}

    protected:
        Collider() {}
    };
}