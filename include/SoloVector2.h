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
		static const Vector2& one();
		
		static const Vector2& unitX();
		static const Vector2& unitY();
		
		bool isZero() const;
		bool isOne() const;

		static float angle(const Vector2& v1, const Vector2& v2);

		static void add(const Vector2& v1, const Vector2& v2, Vector2* dst);
		static void subtract(const Vector2& v1, const Vector2& v2, Vector2* dst);
		
		void clamp(const Vector2& min, const Vector2& max);
		static void clamp(const Vector2& v, const Vector2& min, const Vector2& max, Vector2* dst);

		float distance(const Vector2& v) const;
		float distanceSquared(const Vector2& v) const;

		float dot(const Vector2& v) const;
		static float dot(const Vector2& v1, const Vector2& v2);

		float length() const;
		float lengthSquared() const;

		void negate();

		Vector2 normalized() const;
		void normalize();
		void normalize(Vector2* dst) const;

		void scale(float scalar);
		void scale(const Vector2& scale);

		void rotate(const Vector2& point, float angleRadians);

		void set(float x, float y);
		void set(const float* array);
		void set(const Vector2& v);
		void set(const Vector2& p1, const Vector2& p2);

		void smooth(const Vector2& target, float elapsedTime, float responseTime);
		
		inline Vector2 operator+(const Vector2& v) const;
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
		result.scale(x);
		return result;
	}

	inline Vector2 Vector2::operator+(const Vector2& v) const
	{
		auto result(*this);
		add(result, v, &result);
		return result;
	}

	inline Vector2& Vector2::operator+=(const Vector2& v)
	{
		add(*this, v, this);
		return *this;
	}

	inline Vector2 Vector2::operator-(const Vector2& v) const
	{
		auto result(*this);
		subtract(result, v, &result);
		return result;
	}

	inline Vector2& Vector2::operator-=(const Vector2& v)
	{
		subtract(*this, v, this);
		return *this;
	}

	inline Vector2 Vector2::operator-() const
	{
		auto result(*this);
		result.negate();
		return result;
	}

	inline Vector2 Vector2::operator*(float x) const
	{
		auto result(*this);
		result.scale(x);
		return result;
	}

	inline Vector2& Vector2::operator*=(float x)
	{
		scale(x);
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
