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

        static float degToRad(float degrees);
        static float radToDeg(float radians);

        static float getRandomAroundZero();
        static float getRandom01();

        static float clamp(float x, float lo, float hi);
    };

    inline bool Math::approxZero(float value, float tolerance)
    {
        return fabs(value) <= tolerance;
    }

    inline bool Math::approxEqual(float first, float second, float tolerance)
    {
        return fabs(first - second) <= tolerance;
    }

    inline float Math::degToRad(float degrees)
    {
        return degrees * 0.0174532925f;
    }

    inline float Math::radToDeg(float radians)
    {
        return radians * 57.29577951f;
    }

    inline float Math::getRandomAroundZero()
    {
        return 2.0f * (static_cast<float>(rand()) / RAND_MAX) - 1.0f;
    }

    inline float Math::getRandom01()
    {
        return static_cast<float>(rand()) / RAND_MAX;
    }

    inline float Math::clamp(float x, float lo, float hi)
    {
        return (x < lo) ? lo : ((x > hi) ? hi : x);
    }
}
