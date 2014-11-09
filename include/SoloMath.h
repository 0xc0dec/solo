#ifndef __SOLO_MATH_H__
#define __SOLO_MATH_H__

#include <cmath>
#include "SoloCommons.h"

namespace solo
{
	const f32 ROUNDING_ERROR_F32 = 0.000001f;
	const f64 ROUNDING_ERROR_F64 = 0.00000001;

	const f32 PI = 3.14159265359f;
	const f32 RECIPROCAL_PI = 1.0f / PI;
	const f32 HALF_PI = PI / 2.0f;

	const f64 PI64 = 3.1415926535897932384626433832795028841971693993751;
	const f64 RECIPROCAL_PI64 = 1.0 / PI64;

	const f32 DEGTORAD = PI / 180.0f;
	const f32 RADTODEG = 180.0f / PI;
	const f64 DEGTORAD64 = PI64 / 180.0;
	const f64 RADTODEG64 = 180.0 / PI64;

	inline f32 radToDeg(f32 radians)
	{
		return RADTODEG * radians;
	}

	inline f32 degToRad(f32 degrees)
	{
		return DEGTORAD * degrees;
	}

	template<class T> inline const T &min(const T &a, const T &b)
	{
		return a < b ? a : b;
	}

	template<class T> inline const T &min(const T &a, const T &b, const T &c)
	{
		return a < b ? min(a, c) : min(b, c);
	}

	template<class T> inline const T &max(const T &a, const T &b)
	{
		return a < b ? b : a;
	}

	template<class T> inline const T &max(const T &a, const T &b, const T &c)
	{
		return a < b ? max(b, c) : max(a, c);
	}

	template<class T> inline T abs(const T &a)
	{
		return a < static_cast<T>(0) ? -a : a;
	}

	template<class T> inline T lerp(const T &a, const T &b, const f32 t)
	{
		return static_cast<T>(a * (1.f - t)) + (b * t);
	}
	
	template <class T> inline T clamp(const T &value, const T &low, const T &high)
	{
		return min(max(value, low), high);
	}

	inline bool equals(const f32 a, const f32 b, const f32 tolerance = ROUNDING_ERROR_F32)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	}

	inline f32 sqrt(const f32 f)
	{
		return std::sqrtf(f);
	}

	inline f64 sqrt(const f64 x)
	{
		return std::sqrt(x);
	}

	inline f64 reciprocalSqrt(const f64 x)
	{
		return 1.0 / sqrt(x);
	}

	inline bool iszero(const f64 a, const f64 tolerance = ROUNDING_ERROR_F64)
	{
		return fabs(a) <= tolerance;
	}

	inline bool iszero(const f32 a, const f32 tolerance = ROUNDING_ERROR_F32)
	{
		return fabsf(a) <= tolerance;
	}

	inline f32 reciprocal(const f32 f)
	{
		return 1.f / f;
	}

	inline f64 reciprocal(const f64 f)
	{
		return 1.0 / f;
	}
}

#endif