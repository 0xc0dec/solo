#ifndef __SOLO_VEC3_H__
#define __SOLO_VEC3_H__

#include "SoloMath.h"

namespace solo
{
	template <class T>
	class Vec3
	{
	public:
		T x, y, z;

		Vec3() : x(0), y(0), z(0) {}
		Vec3(T nx, T ny, T nz) : x(nx), y(ny), z(nz) {}
		Vec3(const Vec3<T> &other) : x(other.x), y(other.y), z(other.z) {}
		explicit Vec3(T n) : x(n), y(n), z(n) {}

		Vec3<T> operator-() const
		{
			return Vec3<T>(-x, -y, -z);
		}

		Vec3<T> &operator=(const Vec3<T> &other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		Vec3<T> operator+(const Vec3<T> &other) const
		{
			return Vec3<T>(x + other.x, y + other.y, z + other.z);
		}

		Vec3<T> &operator+=(const Vec3<T> &other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vec3<T> operator+(const T val) const
		{
			return Vec3<T>(x + val, y + val, z + val);
		}

		Vec3<T> &operator+=(const T val)
		{
			x += val;
			y += val;
			z += val;
			return *this;
		}

		Vec3<T> operator-(const Vec3<T> &other) const
		{
			return Vec3<T>(x - other.x, y - other.y, z - other.z);
		}

		Vec3<T> &operator-=(const Vec3<T> &other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		Vec3<T> operator-(const T val) const
		{
			return Vec3<T>(x - val, y - val, z - val);
		}

		Vec3<T> &operator-=(const T val)
		{
			x -= val;
			y -= val;
			z -= val;
			return *this;
		}

		Vec3<T> operator*(const Vec3<T> &other) const
		{
			return Vec3<T>(x * other.x, y * other.y, z * other.z);
		}

		Vec3<T> &operator*=(const Vec3<T> &other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		Vec3<T> operator*(const T v) const
		{
			return Vec3<T>(x * v, y * v, z * v);
		}

		Vec3<T> &operator*=(const T v)
		{
			x *= v;
			y *= v;
			z *= v;
			return *this;
		}

		Vec3<T> operator/(const Vec3<T> &other) const
		{
			return Vec3<T>(x / other.x, y / other.y, z / other.z);
		}

		Vec3<T> &operator/=(const Vec3<T> &other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}

		Vec3<T> operator/(const T v) const
		{
			T i = static_cast<T>(1.0) / v;
			return Vec3<T>(x * i, y * i, z * i);
		}

		Vec3<T> &operator/=(const T v)
		{
			T i = static_cast<T>(1.0) / v;
			x *= i;
			y *= i;
			z *= i;
			return *this;
		}

		bool operator <=(const Vec3<T> &other) const
		{
			return 	(x < other.x || solo::equals(x, other.x)) ||
				(solo::equals(x, other.x) && (y < other.y || solo::equals(y, other.y))) ||
				(solo::equals(x, other.x) && solo::equals(y, other.y) && (z < other.z || solo::equals(z, other.z)));
		}

		bool operator >=(const Vec3<T> &other) const
		{
			return (x > other.x || solo::equals(x, other.x)) ||
				(solo::equals(x, other.x) && (y > other.y || solo::equals(y, other.y))) ||
				(solo::equals(x, other.x) && solo::equals(y, other.y) && (z > other.z || solo::equals(z, other.z)));
		}

		bool operator <(const Vec3<T> &other) const
		{
			return (x < other.x && !solo::equals(x, other.x)) ||
				(solo::equals(x, other.x) && y < other.y && !solo::equals(y, other.y)) ||
				(solo::equals(x, other.x) && solo::equals(y, other.y) && z < other.z && !solo::equals(z, other.z));
		}

		bool operator >(const Vec3<T> &other) const
		{
			return (x > other.x && !solo::equals(x, other.x)) ||
				(solo::equals(x, other.x) && y > other.y && !solo::equals(y, other.y)) ||
				(solo::equals(x, other.x) && solo::equals(y, other.y) && z > other.z && !solo::equals(z, other.z));
		}

		bool operator ==(const Vec3<T> &other) const
		{
			return this->equals(other);
		}

		bool operator !=(const Vec3<T> &other) const
		{
			return !this->equals(other);
		}

		bool equals(const Vec3<T> &other, const T tolerance = static_cast<T>(ROUNDING_ERROR_F32)) const
		{
			return solo::equals(x, other.x, tolerance) &&
				solo::equals(y, other.y, tolerance) &&
				solo::equals(z, other.z, tolerance);
		}

		Vec3<T> &set(const T nx, const T ny, const T nz)
		{
			x = nx;
			y = ny;
			z = nz;
			return *this;
		}

		Vec3<T> &set(const Vec3<T> &p)
		{
			x = p.x;
			y = p.y;
			z = p.z;
			return *this;
		}

		T getLength() const
		{
			return solo::sqrt(x * x + y * y + z * z);
		}

		T getSquaredLength() const
		{
			return x * x + y * y + z * z;
		}

		T dot(const Vec3<T> &other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		T getDistanceFrom(const Vec3<T> &other) const
		{
			return Vec3<T>(x - other.x, y - other.y, z - other.z).getLength();
		}

		T getSquaredDistanceFrom(const Vec3<T> &other) const
		{
			return Vec3<T>(x - other.x, y - other.y, z - other.z).getSquaredLength();
		}

		Vec3<T> crossProduct(const Vec3<T> &p) const
		{
			return Vec3<T>(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
		}

		bool isBetweenPoints(const Vec3<T> &begin, const Vec3<T> &end) const
		{
			const T f = (end - begin).getLengthSquared();
			return getSquaredDistanceFrom(begin) <= f && getSquaredDistanceFrom(end) <= f;
		}

		Vec3<T> &normalize()
		{
			f64 length = x * x + y * y + z * z;
			if (length == 0)
				return *this;
			length = reciprocalSqrt(length);

			x = static_cast<T>(x * length);
			y = static_cast<T>(y * length);
			z = static_cast<T>(z * length);
			return *this;
		}

		Vec3<T> &setLength(T newlength)
		{
			normalize();
			return (*this *= newlength);
		}

		Vec3<T> &invert()
		{
			x *= -1;
			y *= -1;
			z *= -1;
			return *this;
		}
	};

	typedef Vec3<f32> Vec3f;
}

#endif