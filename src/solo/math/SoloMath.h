/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

#include <glm/gtc/epsilon.hpp>

namespace solo
{
    namespace math
    {
        static bool isZero(float value);
        static bool areEqual(float first, float second);
    }

    inline bool math::isZero(float value)
    {
        return areEqual(value, 0);
    }

    inline bool math::areEqual(float first, float second)
    {
        return glm::epsilonEqual(first, second, glm::epsilon<float>());
    }
}
