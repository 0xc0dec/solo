#pragma once

#include "SoloMatrix.h"

namespace solo
{
	class Quaternion
	{
	public:
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 1;

		Quaternion() {}
		Quaternion(float x, float y, float z, float w);
		Quaternion(float* array);
		Quaternion(const Matrix& m);
		Quaternion(const Vector3& axis, float angleRadians);

		static const Quaternion& identity();
		static const Quaternion& zero();

		bool isIdentity() const;
		bool isZero() const;

		static void createFromRotationMatrix(const Matrix& m, Quaternion* dst);
		static void createFromAxisAngle(const Vector3& axis, float angleRadians, Quaternion* dst);
		static Quaternion createFromRotationMatrix(const Matrix& m);
		static Quaternion createFromAxisAngle(const Vector3& axis, float angleRadians);

		void conjugate();
		void conjugate(Quaternion* dst) const;

		bool inverse();
		bool inverse(Quaternion* dst) const;

		void multiply(const Quaternion& q);
		static void multiply(const Quaternion& q1, const Quaternion& q2, Quaternion* dst);

		void normalize();
		void normalize(Quaternion* dst) const;

		void set(float x, float y, float z, float w);
		void set(float* array);
		void set(const Matrix& m);
		void set(const Vector3& axis, float angleRadians);
		void set(const Quaternion& q);

		void setIdentity();
		
		float toAxisAngle(Vector3* e) const;
		
		static void lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
		static void slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
		static void squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t, Quaternion* dst);

		inline Quaternion operator*(const Quaternion& q) const;
		inline Quaternion& operator*=(const Quaternion& q);

	private:
		static void slerp(float q1x, float q1y, float q1z, float q1w, float q2x, float q2y, float q2z, float q2w,
			float t, float* dstx, float* dsty, float* dstz, float* dstw);

		static void slerpForSquad(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst);
	};

	inline Quaternion Quaternion::operator*(const Quaternion& q) const
	{
		auto result(*this);
		result.multiply(q);
		return result;
	}

	inline Quaternion& Quaternion::operator*=(const Quaternion& q)
	{
		multiply(q);
		return *this;
	}
}
