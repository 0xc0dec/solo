#pragma once

#include <math.h>

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

		static Vector3 zero();
		static Vector3 unit();
		static Vector3 unitX();
		static Vector3 unitY();
		static Vector3 unitZ();

		bool isZero() const;
		bool isUnit() const;

		// in radians
		static float angle(const Vector3& v1, const Vector3& v2);

		void clamp(const Vector3& min, const Vector3& max);

		static Vector3 cross(const Vector3& v1, const Vector3& v2);

		float distance(const Vector3& v) const;
		float distanceSquared(const Vector3& v) const;

		float dot(const Vector3& v) const;
		static float dot(const Vector3& v1, const Vector3& v2);

		float length() const;
		float lengthSquared() const;

		Vector3 normalized() const;
		void normalize();
		
		inline void set(float x, float y, float z);
		inline void set(const float* array);
		inline void set(const Vector3& v);

		inline Vector3 operator+(float scalar) const;
		inline Vector3 operator+(const Vector3& v) const;
		inline Vector3& operator+=(const Vector3& v);
		inline Vector3& operator+=(float scalar);

		inline Vector3 operator-() const;
		inline Vector3 operator-(float scalar) const;
		inline Vector3 operator-(const Vector3& v) const;
		inline Vector3& operator-=(float scalar);
		inline Vector3& operator-=(const Vector3& v);

		inline Vector3 operator*(float scalar) const;
		inline Vector3& operator*=(float scalar);

		inline Vector3 operator/(float scalar) const;
		inline Vector3& operator/=(float scalar);

		inline bool operator<(const Vector3& v) const;
		
		inline bool operator==(const Vector3& v) const;
		inline bool operator!=(const Vector3& v) const;
	};


	inline void Vector3::set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline void Vector3::set(const float* array)
	{
		x = array[0];
		y = array[1];
		z = array[2];
	}

	inline void Vector3::set(const Vector3& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}

	inline Vector3 Vector3::operator+(const Vector3& v) const
	{
		auto result(*this);
		result += v;
		return result;
	}

	inline Vector3 Vector3::operator+(float scalar) const
	{
		auto result(*this);
		result += scalar;
		return result;
	}

	inline Vector3& Vector3::operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Vector3& Vector3::operator+=(float scalar)
	{
		x += scalar;
		y += scalar;
		z += scalar;
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

	inline Vector3 Vector3::operator-(float scalar) const
	{
		auto result(*this);
		result -= scalar;
		return result;
	}

	inline Vector3 Vector3::operator-(const Vector3& v) const
	{
		auto result(*this);
		result -= v;
		return result;
	}

	inline Vector3& Vector3::operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline Vector3& Vector3::operator-=(float scalar)
	{
		x -= scalar;
		y -= scalar;
		z -= scalar;
		return *this;
	}

	inline Vector3 Vector3::operator*(float scalar) const
	{
		auto result(*this);
		result *= scalar;
		return result;
	}

	inline Vector3& Vector3::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	inline Vector3 Vector3::operator/(const float scalar) const
	{
		return Vector3(this->x / scalar, this->y / scalar, this->z / scalar);
	}

	inline Vector3& Vector3::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
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
