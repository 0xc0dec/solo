#pragma once

#include <cmath>

#define MATH_FLOAT_SMALL			1.0e-37f
#define MATH_TOLERANCE				2e-37f
#define MATH_E						2.71828182845904523536f
#define MATH_LOG10E					0.4342944819032518f
#define MATH_LOG2E					1.442695040888963387f
#define MATH_PI						3.14159265358979323846f
#define MATH_PIOVER2				1.57079632679489661923f
#define MATH_PIOVER4				0.785398163397448309616f
#define MATH_PIX2					6.28318530717958647693f
#define MATH_EPSILON				0.000001f
#ifndef M_1_PI
#	define M_1_PI					0.31830988618379067154
#endif

namespace solo
{
	class Math
	{
	public:
		Math() = delete;

		static void smooth(float* x, float target, float elapsedTime, float responseTime)
		{
			if (elapsedTime > 0)
				*x += (target - *x) * elapsedTime / (elapsedTime + responseTime);
		}

		static void smooth(float* x, float target, float elapsedTime, float riseTime, float fallTime)
		{
			if (elapsedTime > 0)
			{
				auto delta = target - *x;
				*x += delta * elapsedTime / (elapsedTime + (delta > 0 ? riseTime : fallTime));
			}
		}

		static float degToRad(float degrees)
		{
			return degrees * 0.0174532925f;
		}

		static float radToDeg(float radians)
		{
			return radians * 57.29577951f;
		}

		static float getRandomAroundZero()
		{
			return 2.0f * (static_cast<float>(rand()) / RAND_MAX) - 1.0f;
		}

		static float getRandom01()
		{
			return static_cast<float>(rand()) / RAND_MAX;
		}

		static float clamp(float x, float lo, float hi)
		{
			return (x < lo) ? lo : ((x > hi) ? hi : x);
		}
	};
}
