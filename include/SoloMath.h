#pragma once

#include <cmath>

namespace solo
{
	class Math
	{
	public:
		static constexpr float SMALL_FLOAT1 = 0.000001f;
		static constexpr float SMALL_FLOAT2 = 1.0e-37f;
		static constexpr float SMALL_FLOAT3 = 2e-37f;
		static constexpr float E = 2.71828182845904523536f;
		static constexpr float LOG10E = 0.4342944819032518f;
		static constexpr float LOG2E = 1.442695040888963387f;
		static constexpr float PI = 3.14159265358979323846f;
		static constexpr float PI_OVER_2 = 1.57079632679489661923f;
		static constexpr float PI_OVER_4 = 0.785398163397448309616f;
		static constexpr float PIX2 = 6.28318530717958647693f;

		static bool isApproxZero(float value);

		static float degToRad(float degrees);
		static float radToDeg(float radians);

		static float getRandomAroundZero();
		static float getRandom01();

		static float clamp(float x, float lo, float hi);
	};

	inline bool Math::isApproxZero(float value)
	{
		return fabs(value) <= SMALL_FLOAT1;
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
