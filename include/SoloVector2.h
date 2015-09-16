#pragma once

namespace solo
{
	class Matrix;

	class Vector2
	{
	public:
		float x = 0;
		float y = 0;

		Vector2() {}
		Vector2(float x, float y);
		Vector2(const float* array);
		Vector2(const Vector2& p1, const Vector2& p2);

		static const Vector2& zero();
		static const Vector2& unit();
		static const Vector2& unitX();
		static const Vector2& unitY();
		
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

		void set(float x, float y);
		void set(const float* array);
		void set(const Vector2& v);

		inline Vector2 operator+(float scalar) const;
		inline Vector2 operator+(const Vector2& v) const;
		inline Vector2& operator+=(float scalar);
		inline Vector2& operator+=(const Vector2& v);

		inline Vector2 operator-(const Vector2& v) const;
		inline Vector2& operator-=(const Vector2& v);

		inline Vector2 operator-() const;
		
		inline Vector2 operator*(float x) const;
		inline Vector2& operator*=(float x);

		inline Vector2 operator/(float x) const;

		inline bool operator<(const Vector2& v) const;
		
		inline bool operator==(const Vector2& v) const;
		inline bool operator!=(const Vector2& v) const;
	};

	inline Vector2 operator*(float x, const Vector2& v)
	{
		auto result(const_cast<Vector2&>(v));
		result *= x;
		return result;
	}

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

	inline Vector2 Vector2::operator-(const Vector2& v) const
	{
		auto result(*this);
		result -= v;
		return result;
	}

	inline Vector2& Vector2::operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	inline Vector2 Vector2::operator-() const
	{
		auto result(*this);
		result.x = -result.x;
		result.y = -result.y;
		return result;
	}

	inline Vector2 Vector2::operator*(float x) const
	{
		auto result(*this);
		result *= x;
		return result;
	}

	inline Vector2& Vector2::operator*=(float x)
	{
		this->x *= x;
		this->y *= x;
		return *this;
	}

	inline Vector2 Vector2::operator/(const float x) const
	{
		return Vector2(this->x / x, this->y / x);
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
