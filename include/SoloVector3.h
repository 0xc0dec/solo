#pragma once

namespace solo
{
	struct Vector3
	{
		float x = 0;
		float y = 0;
		float z = 0;

		Vector3() {}
		Vector3(float x, float y, float z);

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
		
		Vector3 operator+(float scalar) const;
		Vector3 operator+(const Vector3& v) const;
		Vector3& operator+=(const Vector3& v);
		Vector3& operator+=(float scalar);

		Vector3 operator-() const;
		Vector3 operator-(float scalar) const;
		Vector3 operator-(const Vector3& v) const;
		Vector3& operator-=(float scalar);
		Vector3& operator-=(const Vector3& v);

		Vector3 operator*(float scalar) const;
		Vector3& operator*=(float scalar);

		Vector3 operator/(float scalar) const;
		Vector3& operator/=(float scalar);

		bool operator<(const Vector3& v) const;
		
		bool operator==(const Vector3& v) const;
		bool operator!=(const Vector3& v) const;
	};


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

	inline Vector3 operator+(float scalar, const Vector3& v)
	{
		return Vector3(v.x + scalar, v.y + scalar, v.z + scalar);
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

	inline Vector3 operator*(float scalar, const Vector3& v)
	{
		return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
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
}
