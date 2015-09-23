#pragma once

#include "SoloMatrix.h"

namespace solo
{
	struct Quaternion
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 1;

		Quaternion() {}
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Vector3& axis, float angleRadians);

		static Quaternion createFromRotationMatrix(const Matrix& m);
		static Quaternion createFromAxisAngle(const Vector3& axis, float angleRadians);

		static const Quaternion& identity();
		static const Quaternion& zero();

		bool isIdentity() const;
		bool isZero() const;

		void conjugate();
		bool inverse();

		Quaternion normalized() const;
		void normalize();

		float toAxisAngle(Vector3& e) const;
		
		static Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t);
		static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
		static Quaternion squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t);

		inline Quaternion operator*(const Quaternion& q) const;
		Quaternion& operator*=(const Quaternion& q);

	private:
		static Quaternion slerpForSquad(const Quaternion& q1, const Quaternion& q2, float t);
	};

	inline Quaternion Quaternion::operator*(const Quaternion& q) const
	{
		auto result(*this);
		result *= q;
		return result;
	}
}
