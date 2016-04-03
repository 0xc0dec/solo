#pragma once

#include <cmath>
#include <cstdlib>


namespace solo
{
    class Math final
    {
    public:
        static constexpr float smallFloat1 = 0.000001f;
        static constexpr float smallFloat2 = 1.0e-37f;
        static constexpr float smallFloat3 = 2e-37f;
        static constexpr float pi = 3.14159265358979323846f;
        static constexpr float piOver2 = 1.57079632679489661923f;

        static bool approxZero(float value, float tolerance);
        static bool approxEqual(float first, float second, float tolerance);

        static auto degToRad(float degrees) -> float;
        static auto radToDeg(float radians) -> float;

        static auto getRandomAroundZero() -> float;
        static auto getRandom01() -> float;

        static auto clamp(float x, float lo, float hi) -> float;
    };

    inline bool Math::approxZero(float value, float tolerance)
    {
        return fabs(value) <= tolerance;
    }

    inline bool Math::approxEqual(float first, float second, float tolerance)
    {
        return fabs(first - second) <= tolerance;
    }

    inline auto Math::degToRad(float degrees) -> float
    {
        return degrees * 0.0174532925f;
    }

    inline auto Math::radToDeg(float radians) -> float
    {
        return radians * 57.29577951f;
    }

    inline auto Math::getRandomAroundZero() -> float
    {
        return 2.0f * (static_cast<float>(rand()) / RAND_MAX) - 1.0f;
    }

    inline auto Math::getRandom01() -> float
    {
        return static_cast<float>(rand()) / RAND_MAX;
    }

    inline auto Math::clamp(float x, float lo, float hi) -> float
    {
        return (x < lo) ? lo : ((x > hi) ? hi : x);
    }
}
