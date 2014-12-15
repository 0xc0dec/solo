#ifndef __SOLO_VECTOR4_H__
#define __SOLO_VECTOR4_H__

namespace solo
{
	class Vector4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Vector4();
		Vector4(float x, float y, float z, float w);
		Vector4(const float* array);
		Vector4(const Vector4& p1, const Vector4& p2);
		Vector4(const Vector4& copy);

		/**
		* Creates a new vector from an integer interpreted as an RGBA value.
		* E.g. 0xff0000ff represents opaque red or the vector (1, 0, 0, 1).
		*
		* @param color The integer to interpret as an RGBA value.
		*
		* @return A vector corresponding to the interpreted RGBA color.
		*/
		static Vector4 fromColor(unsigned int color);

		~Vector4();

		static const Vector4& zero();
		static const Vector4& one();

		static const Vector4& unitX();
		static const Vector4& unitY();
		static const Vector4& unitZ();
		static const Vector4& unitW();

		bool isZero() const;
		bool isOne() const;

		/**
		* Returns the angle (in radians) between the specified vectors.
		*
		* @param v1 The first vector.
		* @param v2 The second vector.
		*
		* @return The angle between the two vectors (in radians).
		*/
		static float angle(const Vector4& v1, const Vector4& v2);

		void add(const Vector4& v);
		static void add(const Vector4& v1, const Vector4& v2, Vector4* dst);

		void clamp(const Vector4& min, const Vector4& max);
		static void clamp(const Vector4& v, const Vector4& min, const Vector4& max, Vector4* dst);

		float distance(const Vector4& v) const;
		float distanceSquared(const Vector4& v) const;

		float dot(const Vector4& v) const;
		static float dot(const Vector4& v1, const Vector4& v2);

		float length() const;
		float lengthSquared() const;

		void negate();
		
		Vector4& normalize();
		void normalize(Vector4* dst) const;

		void scale(float scalar);

		void set(float x, float y, float z, float w);
		void set(const float* array);
		void set(const Vector4& v);
		void set(const Vector4& p1, const Vector4& p2);

		void subtract(const Vector4& v);
		static void subtract(const Vector4& v1, const Vector4& v2, Vector4* dst);

		inline Vector4 operator+(const Vector4& v) const;
		inline Vector4& operator+=(const Vector4& v);

		inline Vector4 operator-(const Vector4& v) const;
		inline Vector4 operator-() const;
		inline Vector4& operator-=(const Vector4& v);
		
		inline Vector4 operator*(float x) const;
		inline Vector4& operator*=(float x);
		
		inline Vector4 operator/(float x) const;
		
		inline bool operator<(const Vector4& v) const;
		
		inline bool operator==(const Vector4& v) const;
		
		inline bool operator!=(const Vector4& v) const;
	};


	inline Vector4 Vector4::operator+(const Vector4& v) const
	{
		Vector4 result(*this);
		result.add(v);
		return result;
	}

	inline Vector4& Vector4::operator+=(const Vector4& v)
	{
		add(v);
		return *this;
	}

	inline Vector4 Vector4::operator-(const Vector4& v) const
	{
		Vector4 result(*this);
		result.subtract(v);
		return result;
	}

	inline Vector4& Vector4::operator-=(const Vector4& v)
	{
		subtract(v);
		return *this;
	}

	inline Vector4 Vector4::operator-() const
	{
		Vector4 result(*this);
		result.negate();
		return result;
	}

	inline Vector4 Vector4::operator*(float x) const
	{
		Vector4 result(*this);
		result.scale(x);
		return result;
	}

	inline Vector4& Vector4::operator*=(float x)
	{
		scale(x);
		return *this;
	}

	inline Vector4 Vector4::operator/(const float x) const
	{
		return Vector4(this->x / x, this->y / x, this->z / x, this->w / x);
	}

	inline bool Vector4::operator<(const Vector4& v) const
	{
		if (x == v.x)
		{
			if (y == v.y)
			{
				if (z == v.z)
					return w < v.w;
				return z < v.z;
			}
			return y < v.y;
		}
		return x < v.x;
	}

	inline bool Vector4::operator==(const Vector4& v) const
	{
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}

	inline bool Vector4::operator!=(const Vector4& v) const
	{
		return x != v.x || y != v.y || z != v.z || w != v.w;
	}

	inline Vector4 operator*(float x, const Vector4& v)
	{
		Vector4 result(v);
		result.scale(x);
		return result;
	}
}

#endif