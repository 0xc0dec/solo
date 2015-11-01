#pragma once

namespace solo
{
	struct Vector2
	{
		float x = 0;
		float y = 0;

		Vector2() {}
		Vector2(float x, float y);

		static Vector2 zero();
		static Vector2 unit();
		static Vector2 unitX();
		static Vector2 unitY();
		
		bool isZero() const;
		bool isUnit() const;

		// in radians
		static float angle(const Vector2& v1, const Vector2& v2);
		
		void clamp(const Vector2& min, const Vector2& max);

		float distance(const Vector2& v) const;
		float distanceSquared(const Vector2& v) const;

		float dot(const Vector2& v) const;
		static float dot(const Vector2& v1, const Vector2& v2);

		float length() const;
		float lengthSquared() const;

		Vector2 normalized() const;
		void normalize();

		Vector2 operator+(float scalar) const;
		Vector2 operator+(const Vector2& v) const;
		Vector2& operator+=(float scalar);
		Vector2& operator+=(const Vector2& v);

		Vector2 operator-() const;
		Vector2 operator-(float scalar) const;
		Vector2 operator-(const Vector2& v) const;
		Vector2& operator-=(float scalar);
		Vector2& operator-=(const Vector2& v);
		
		Vector2 operator*(float scalar) const;
		Vector2& operator*=(float scalar);

		Vector2 operator/(float scalar) const;
		Vector2& operator/=(float scalar);

		bool operator<(const Vector2& v) const;
		
		bool operator==(const Vector2& v) const;
		bool operator!=(const Vector2& v) const;
	};

	inline Vector2 Vector2::operator+(float scalar) const
	{
		auto result(*this);
		result += scalar;
		return result;
	}

	inline Vector2 Vector2::operator+(const Vector2& v) const
	{
		auto result(*this);
		result += v;
		return result;
	}

	inline Vector2 operator+(float scalar, const Vector2& v)
	{
		return Vector2(v.x + scalar, v.y + scalar);
	}

	inline Vector2& Vector2::operator+=(float scalar)
	{
		x += scalar;
		y += scalar;
		return *this;
	}

	inline Vector2& Vector2::operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	inline Vector2 Vector2::operator-() const
	{
		auto result(*this);
		result.x = -result.x;
		result.y = -result.y;
		return result;
	}

	inline Vector2 Vector2::operator-(float scalar) const
	{
		auto result(*this);
		result -= scalar;
		return result;
	}

	inline Vector2 Vector2::operator-(const Vector2& v) const
	{
		auto result(*this);
		result -= v;
		return result;
	}

	inline Vector2& Vector2::operator-=(float scalar)
	{
		x -= scalar;
		y -= scalar;
		return *this;
	}

	inline Vector2& Vector2::operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	inline Vector2 Vector2::operator*(float scalar) const
	{
		auto result(*this);
		result *= scalar;
		return result;
	}

	inline Vector2 operator*(float scalar, const Vector2& v)
	{
		return Vector2(v.x * scalar, v.y * scalar);
	}

	inline Vector2& Vector2::operator*=(float scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		return *this;
	}

	inline Vector2 Vector2::operator/(const float scalar) const
	{
		Vector2 result;
		result /= scalar;
		return result;
	}

	inline Vector2& Vector2::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	inline bool Vector2::operator<(const Vector2& v) const
	{
		if (x == v.x)
			return y < v.y;
		return x < v.x;
	}

	inline bool Vector2::operator==(const Vector2& v) const
	{
		return x == v.x && y == v.y;
	}

	inline bool Vector2::operator!=(const Vector2& v) const
	{
		return x != v.x || y != v.y;
	}
}
