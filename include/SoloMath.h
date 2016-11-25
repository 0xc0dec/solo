/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <cmath>


namespace solo
{
    namespace math
    {
        static constexpr float epsilon1 = 0.000001f;
        static constexpr float epsilon2 = 1.0e-37f;
        static constexpr float epsilon3 = 2e-37f;
        static constexpr float pi = 3.14159265358979323846f;
        static constexpr float piOver2 = 1.57079632679489661923f;

        static bool approxZero(float value, float epsilon);
        static bool approxEqual(float first, float second, float epsilon);

        static auto degToRad(float degrees) -> float;
        static auto radToDeg(float radians) -> float;

        static auto clamp(float x, float lo, float hi) -> float;
    }

    inline bool math::approxZero(float value, float epsilon)
    {
        return fabs(value) <= epsilon;
    }

    inline bool math::approxEqual(float first, float second, float epsilon)
    {
        return fabs(first - second) <= epsilon;
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
        return (x < lo) ? lo : ((x > hi) ? hi : x);
    }
}
