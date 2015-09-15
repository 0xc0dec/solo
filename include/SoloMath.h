#pragma once

#include <cmath>
#include "SoloBase.h"

#define MATH_DEG_TO_RAD(x)			((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)			((x) * 57.29577951f)
#define MATH_RANDOM_MINUS1_1()		((2.0f * ((float)rand() / RAND_MAX)) - 1.0f)		// Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()			((float)rand() / RAND_MAX)					// Returns a random float between 0 and 1.
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
#define MATH_CLAMP(x, lo, hi)		((x < lo) ? lo : ((x > hi) ? hi : x))
#ifndef M_1_PI
#	define M_1_PI					0.31830988618379067154
#endif

#define MATRIX_SIZE (sizeof(float) * 16)

namespace solo
{
	class Math
	{
	public:
		static void smooth(float* x, float target, float elapsedTime, float responseTime);
		static void smooth(float* x, float target, float elapsedTime, float riseTime, float fallTime);

		inline static void transformVector4(const float* m, float x, float y, float z, float w, float* dst);
		inline static void transformVector4(const float* m, const float* v, float* dst);
		inline static void crossVector3(const float* v1, const float* v2, float* dst);

	private:
		Math() {}
	};

	inline void Math::transformVector4(const float* m, float x, float y, float z, float w, float* dst)
	{
		dst[0] = x * m[0] + y * m[4] + z * m[8] + w * m[12];
		dst[1] = x * m[1] + y * m[5] + z * m[9] + w * m[13];
		dst[2] = x * m[2] + y * m[6] + z * m[10] + w * m[14];
	}

	inline void Math::transformVector4(const float* m, const float* v, float* dst)
	{
		// Handle case where v == dst.
		auto x = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + v[3] * m[12];
		auto y = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + v[3] * m[13];
		auto z = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + v[3] * m[14];
		auto w = v[0] * m[3] + v[1] * m[7] + v[2] * m[11] + v[3] * m[15];

		dst[0] = x;
		dst[1] = y;
		dst[2] = z;
		dst[3] = w;
	}

	inline void Math::crossVector3(const float* v1, const float* v2, float* dst)
	{
		auto x = (v1[1] * v2[2]) - (v1[2] * v2[1]);
		auto y = (v1[2] * v2[0]) - (v1[0] * v2[2]);
		auto z = (v1[0] * v2[1]) - (v1[1] * v2[0]);

		dst[0] = x;
		dst[1] = y;
		dst[2] = z;
	}
}
