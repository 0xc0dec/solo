/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloCommon.h"

namespace solo
{
    class Collider: public NoCopyAndMove
    {
    public:
        virtual ~Collider() = default;

    protected:
        Collider() = default;
    };
}