#ifndef __SOLO_VECTOR2_H__
#define __SOLO_VECTOR2_H__

namespace solo
{
	class Matrix;

	class Vector2
	{
	public:
		float x;
		float y;

		Vector2();
		Vector2(float x, float y);
		Vector2(const float* array);
		Vector2(const Vector2& p1, const Vector2& p2);
		Vector2(const Vector2& copy);

		~Vector2();

		static const Vector2& zero();
		static const Vector2& one();
		
		static const Vector2& unitX();
		static const Vector2& unitY();
		
		bool isZero() const;
		bool isOne() const;

		/**
		 * Returns the angle (in radians) between the specified vectors.
		 */
		static float angle(const Vector2& v1, const Vector2& v2);

		void add(const Vector2& v);
		static void add(const Vector2& v1, const Vector2& v2, Vector2* dst);
		
		void clamp(const Vector2& min, const Vector2& max);
		static void clamp(const Vector2& v, const Vector2& min, const Vector2& max, Vector2* dst);

		float distance(const Vector2& v) const;
		float distanceSquared(const Vector2& v) const;

		float dot(const Vector2& v) const;
		static float dot(const Vector2& v1, const Vector2& v2);

		float length() const;
		float lengthSquared() const;

		void negate();

		Vector2& normalize();
		void normalize(Vector2* dst) const;

		void scale(float scalar);
		void scale(const Vector2& scale);

		/**
		 * Rotates this vector by angle (specified in radians) around the given point.
		 *
		 * @param point The point to rotate around.
		 * @param angle The angle to rotate by (in radians).
		 */
		void rotate(const Vector2& point, float angle);

		void set(float x, float y);
		void set(const float* array);
		void set(const Vector2& v);
		void set(const Vector2& p1, const Vector2& p2);

		void subtract(const Vector2& v);
		static void subtract(const Vector2& v1, const Vector2& v2, Vector2* dst);

		/**
		 * Updates this vector towards the given target using a smoothing function.
		 * The given response time determines the amount of smoothing (lag). A longer
		 * response time yields a smoother result and more lag. To force this vector to
		 * follow the target closely, provide a response time that is very small relative
		 * to the given elapsed time.
		 *
		 * @param target target value.
		 * @param elapsedTime elapsed time between calls.
		 * @param responseTime response time (in the same units as elapsedTime).
		 */
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

	/**
	 * Calculates the scalar product of the given vector with the given value.
	 * 
	 * @param x The value to scale by.
	 * @param v The vector to scale.
	 * @return The scaled vector.
	 */
	inline Vector2 operator*(float x, const Vector2& v)
	{
		Vector2 result(v);
		result.scale(x);
		return result;
	}

	inline Vector2 Vector2::operator+(const Vector2& v) const
	{
		Vector2 result(*this);
		result.add(v);
		return result;
	}

	inline Vector2& Vector2::operator+=(const Vector2& v)
	{
		add(v);
		return *this;
	}

	inline Vector2 Vector2::operator-(const Vector2& v) const
	{
		Vector2 result(*this);
		result.subtract(v);
		return result;
	}

	inline Vector2& Vector2::operator-=(const Vector2& v)
	{
		subtract(v);
		return *this;
	}

	inline Vector2 Vector2::operator-() const
	{
		Vector2 result(*this);
		result.negate();
		return result;
	}

	inline Vector2 Vector2::operator*(float x) const
	{
		Vector2 result(*this);
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
		{
			return y < v.y;
		}
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



#endif