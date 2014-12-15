#ifndef __SOLO_VECTOR3_H__
#define __SOLO_VECTOR3_H__

namespace solo
{
	class Matrix;
	class Quaternion;

	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		Vector3();
		Vector3(float x, float y, float z);
		Vector3(const float* array);
		Vector3(const Vector3& p1, const Vector3& p2);
		Vector3(const Vector3& copy);

		/**
		* Creates a new vector from an integer interpreted as an RGB value.
		* E.g. 0xff0000 represents red or the vector (1, 0, 0).
		*
		* @param color The integer to interpret as an RGB value.
		*
		* @return A vector corresponding to the interpreted RGB color.
		*/
		static Vector3 fromColor(unsigned int color);

		~Vector3();

		static const Vector3& zero();
		static const Vector3& one();

		static const Vector3& unitX();
		static const Vector3& unitY();
		static const Vector3& unitZ();

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
		static float angle(const Vector3& v1, const Vector3& v2);

		void add(const Vector3& v);
		static void add(const Vector3& v1, const Vector3& v2, Vector3* dst);

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

		void negate();

		Vector3& normalize();
		void normalize(Vector3* dst) const;

		void scale(float scalar);
		
		void set(float x, float y, float z);
		void set(const float* array);
		void set(const Vector3& v);
		void set(const Vector3& p1, const Vector3& p2);

		void subtract(const Vector3& v);
		static void subtract(const Vector3& v1, const Vector3& v2, Vector3* dst);

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
		Vector3 result(*this);
		result.add(v);
		return result;
	}

	inline Vector3& Vector3::operator+=(const Vector3& v)
	{
		add(v);
		return *this;
	}

	inline Vector3 Vector3::operator-(const Vector3& v) const
	{
		Vector3 result(*this);
		result.subtract(v);
		return result;
	}

	inline Vector3& Vector3::operator-=(const Vector3& v)
	{
		subtract(v);
		return *this;
	}

	inline Vector3 Vector3::operator-() const
	{
		Vector3 result(*this);
		result.negate();
		return result;
	}

	inline Vector3 Vector3::operator*(float x) const
	{
		Vector3 result(*this);
		result.scale(x);
		return result;
	}

	inline Vector3& Vector3::operator*=(float x)
	{
		scale(x);
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
		Vector3 result(v);
		result.scale(x);
		return result;
	}
}

#endif