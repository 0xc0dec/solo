/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include <cmath>
#include <cfloat>
#include <glm/gtc/epsilon.hpp>

namespace solo
{
    namespace math
    {
        static constexpr float pi = 3.14159265358979323846f;
        static constexpr float piOver2 = 1.57079632679489661923f;

        static bool isZero(float value);
        static bool areEqual(float first, float second);

        static auto degToRad(float degrees) -> float;
        static auto radToDeg(float radians) -> float;

        static auto clamp(float x, float lo, float hi) -> float;
    }

    inline bool math::isZero(float value)
    {
        return areEqual(value, 0);
    }

    inline bool math::areEqual(float first, float second)
    {
        return glm::epsilonEqual(first, second, glm::epsilon<float>());
    }

    inline auto math::degToRad(float degrees) -> float
    {
        return degrees * 0.0174532925f;
    }

    inline auto math::radToDeg(float radians) -> float
    {
        return radians * 57.29577951f;
    }

    inline auto math::clamp(float x, float lo, float hi) -> float
    {
		return glm::clamp(x, lo, hi);
    }
}
