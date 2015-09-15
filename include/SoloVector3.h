#pragma once

namespace solo
{
	class Matrix;
	class Quaternion;

	class Vector3
	{
	public:
		float x = 0;
		float y = 0;
		float z = 0;

		Vector3() {}
		Vector3(float x, float y, float z);
		Vector3(const float* array);
		Vector3(const Vector3& p1, const Vector3& p2);

		static Vector3 fromColor(unsigned color);

		static const Vector3& zero();
		static const Vector3& one();

		static const Vector3& unitX();
		static const Vector3& unitY();
		static const Vector3& unitZ();

		bool isZero() const;
		bool isOne() const;

		// in radians
		static float angle(const Vector3& v1, const Vector3& v2);

		static void add(const Vector3& v1, const Vector3& v2, Vector3* dst);
		static void subtract(const Vector3& v1, const Vector3& v2, Vector3* dst);

		void clamp(const Vector3& min, const Vector3& max);
		static void clamp(const Vector3& v, const Vector3& min, const Vector3& max, Vector3* dst);

		void cross(const Vector3& v);
		static void cross(const Vector3& v1, const Vector3& v2, Vector3* dst);

		float distance(const Vector3& v) const;
		float distanceSquared(const Vector3& v) const;

		float dot(const Vector3& v) const;
		static float dot(const Vector3& v1, const Vector3& v2);

		float length() const;
		float lengthSquared() const;

		Vector3 normalized() const;
		void normalize();
		void normalize(Vector3* dst) const;
		
		void set(float x, float y, float z);
		void set(const float* array);
		void set(const Vector3& v);
//		void set(const Vector3& p1, const Vector3& p2);

		void smooth(const Vector3& target, float elapsedTime, float responseTime);

		inline Vector3 operator+(const Vector3& v) const;
		inline Vector3& operator+=(const Vector3& v);

		inline Vector3 operator-(const Vector3& v) const;
		inline Vector3 operator-() const;
		inline Vector3& operator-=(const Vector3& v);

		inline Vector3 operator*(float x) const;
		inline Vector3& operator*=(float x);

		inline Vector3 operator/(float x) const;

		inline bool operator<(const Vector3& v) const;
		
		inline bool operator==(const Vector3& v) const;
		inline bool operator!=(const Vector3& v) const;
	};

	
	inline Vector3 Vector3::operator+(const Vector3& v) const
	{
		auto result(*this);
		add(result, v, &result);
		return result;
	}

	inline Vector3& Vector3::operator+=(const Vector3& v)
	{
		add(*this, v, this);
		return *this;
	}

	inline Vector3 Vector3::operator-(const Vector3& v) const
	{
		auto result(*this);
		subtract(result, v, &result);
		return result;
	}

	inline Vector3& Vector3::operator-=(const Vector3& v)
	{
		subtract(*this, v, this);
		return *this;
	}

	inline Vector3 Vector3::operator-() const
	{
		Vector3 result;
		result.x = -x;
		result.y = -y;
		result.z = -z;
		return result;
	}

	inline Vector3 Vector3::operator*(float x) const
	{
		auto result(*this);
		result *= x;
		return result;
	}

	inline Vector3& Vector3::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	inline Vector3 Vector3::operator/(const float x) const
	{
		return Vector3(this->x / x, this->y / x, this->z / x);
	}

	inline bool Vector3::operator<(const Vector3& v) const
	{
		if (x == v.x)
		{
			if (y == v.y)
				return z < v.z;
			return y < v.y;
		}
		return x < v.x;
	}

	inline bool Vector3::operator==(const Vector3& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	inline bool Vector3::operator!=(const Vector3& v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}

	inline Vector3 operator*(float x, const Vector3& v)
	{
		auto result(const_cast<Vector3&>(v));
		result *= x;
		return result;
	}
}
