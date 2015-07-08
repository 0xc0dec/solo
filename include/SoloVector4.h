#pragma once

namespace solo
{
	class Vector4
	{
	public:
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;

		Vector4() {}
		Vector4(float x, float y, float z, float w);
		Vector4(const float* array);
		Vector4(const Vector4& p1, const Vector4& p2);

		static Vector4 fromColor(unsigned color);

		static const Vector4& zero();
		static const Vector4& one();

		static const Vector4& unitX();
		static const Vector4& unitY();
		static const Vector4& unitZ();
		static const Vector4& unitW();

		bool isZero() const;
		bool isOne() const;

		// in radians
		static float angle(const Vector4& v1, const Vector4& v2);

		static void add(const Vector4& v1, const Vector4& v2, Vector4* dst);
		static void subtract(const Vector4& v1, const Vector4& v2, Vector4* dst);

		void clamp(const Vector4& min, const Vector4& max);
		static void clamp(const Vector4& v, const Vector4& min, const Vector4& max, Vector4* dst);

		float distance(const Vector4& v) const;
		float distanceSquared(const Vector4& v) const;

		float dot(const Vector4& v) const;
		static float dot(const Vector4& v1, const Vector4& v2);

		float length() const;
		float lengthSquared() const;

		Vector4 normalized() const;
		void normalize();
		void normalize(Vector4* dst) const;

		void scale(float scalar);

		void set(float x, float y, float z, float w);
		void set(const float* array);
		void set(const Vector4& v);
		void set(const Vector4& p1, const Vector4& p2);

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
		auto result(*this);
		add(result, v, &result);
		return result;
	}

	inline Vector4& Vector4::operator+=(const Vector4& v)
	{
		add(*this, v, this);
		return *this;
	}

	inline Vector4 Vector4::operator-(const Vector4& v) const
	{
		auto result(*this);
		subtract(result, v, &result);
		return result;
	}

	inline Vector4& Vector4::operator-=(const Vector4& v)
	{
		subtract(*this, v, this);
		return *this;
	}

	inline Vector4 Vector4::operator-() const
	{
		Vector4 result;
		result.x = -x;
		result.y = -y;
		result.z = -z;
		result.w = -w;
		return result;
	}

	inline Vector4 Vector4::operator*(float x) const
	{
		auto result(*this);
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
		auto result(const_cast<Vector4&>(v));
		result.scale(x);
		return result;
	}
}
