#pragma once

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

		static Vector3 fromColor(unsigned int color);

		~Vector3();

		static const Vector3& zero();
		static const Vector3& one();

		static const Vector3& unitX();
		static const Vector3& unitY();
		static const Vector3& unitZ();

		bool isZero() const;
		bool isOne() const;

		// in radians
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
