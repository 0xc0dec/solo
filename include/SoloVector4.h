#ifndef __SOLO_VECTOR4_H__
#define __SOLO_VECTOR4_H__

#include "SoloMath.h"
#include "SoloVector3.h"

namespace solo
{
	class Vector4
	{
	public:
		f32 x, y, z, w;

		inline Vector4()
		{
		}

		inline Vector4(const f32 x, const f32 y, const f32 z, const f32 w)
			: x(x), y(y), z(z), w(w)
		{
		}

		inline explicit Vector4(const f32 coords[4])
			: x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3])
		{
		}

		inline explicit Vector4(const int coords[4])
		{
			x = static_cast<f32>(coords[0]);
			y = static_cast<f32>(coords[1]);
			z = static_cast<f32>(coords[2]);
			w = static_cast<f32>(coords[3]);
		}

		inline explicit Vector4(f32* const r)
			: x(r[0]), y(r[1]), z(r[2]), w(r[3])
		{
		}

		inline explicit Vector4(const f32 scaler)
			: x(scaler) , y(scaler) , z(scaler) , w(scaler)
		{
		}

		inline explicit Vector4(const Vector3& other)
			: x(other.x), y(other.y), z(other.z), w(1.0f)
		{
		}

		/** Exchange the contents of this vector with another.
		*/
		inline void swap(Vector4& other)
		{
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
			std::swap(w, other.w);
		}

		inline f32 operator [](const size_t i) const
		{
			assert(i < 4);

			return *(&x + i);
		}

		inline f32& operator [](const size_t i)
		{
			assert(i < 4);

			return *(&x + i);
		}

		/// Pointer accessor for direct copying
		inline f32* ptr()
		{
			return &x;
		}

		/// Pointer accessor for direct copying
		inline const f32* ptr() const
		{
			return &x;
		}

		/** Assigns the value of the other vector.
		@param
		other The other vector
		*/
		inline Vector4& operator =(const Vector4& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;

			return *this;
		}

		inline Vector4& operator =(const f32 scalar)
		{
			x = scalar;
			y = scalar;
			z = scalar;
			w = scalar;
			return *this;
		}

		inline bool operator ==(const Vector4& other) const
		{
			return (x == other.x &&
				y == other.y &&
				z == other.z &&
				w == other.w);
		}

		inline bool operator !=(const Vector4& other) const
		{
			return (x != other.x ||
				y != other.y ||
				z != other.z ||
				w != other.w);
		}

		inline Vector4& operator =(const Vector3& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = 1.0f;
			return *this;
		}

		// arithmetic operations
		inline Vector4 operator +(const Vector4& other) const
		{
			return Vector4(
				x + other.x,
				y + other.y,
				z + other.z,
				w + other.w);
		}

		inline Vector4 operator -(const Vector4& other) const
		{
			return Vector4(
				x - other.x,
				y - other.y,
				z - other.z,
				w - other.w);
		}

		inline Vector4 operator *(const f32 scalar) const
		{
			return Vector4(
				x * scalar,
				y * scalar,
				z * scalar,
				w * scalar);
		}

		inline Vector4 operator *(const Vector4& other) const
		{
			return Vector4(
				other.x * x,
				other.y * y,
				other.z * z,
				other.w * w);
		}

		inline Vector4 operator /(const f32 scalar) const
		{
			assert(scalar != 0.0);

			f32 fInv = 1.0f / scalar;

			return Vector4(
				x * fInv,
				y * fInv,
				z * fInv,
				w * fInv);
		}

		inline Vector4 operator /(const Vector4& other) const
		{
			return Vector4(
				x / other.x,
				y / other.y,
				z / other.z,
				w / other.w);
		}

		inline const Vector4& operator +() const
		{
			return *this;
		}

		inline Vector4 operator -() const
		{
			return Vector4(-x, -y, -z, -w);
		}

		inline friend Vector4 operator *(const f32 scalar, const Vector4& other)
		{
			return Vector4(
				scalar * other.x,
				scalar * other.y,
				scalar * other.z,
				scalar * other.w);
		}

		inline friend Vector4 operator /(const f32 scalar, const Vector4& other)
		{
			return Vector4(
				scalar / other.x,
				scalar / other.y,
				scalar / other.z,
				scalar / other.w);
		}

		inline friend Vector4 operator +(const Vector4& first, const f32 second)
		{
			return Vector4(
				first.x + second,
				first.y + second,
				first.z + second,
				first.w + second);
		}

		inline friend Vector4 operator +(const f32 first, const Vector4& second)
		{
			return Vector4(
				first + second.x,
				first + second.y,
				first + second.z,
				first + second.w);
		}

		inline friend Vector4 operator -(const Vector4& first, f32 second)
		{
			return Vector4(
				first.x - second,
				first.y - second,
				first.z - second,
				first.w - second);
		}

		inline friend Vector4 operator -(const f32 first, const Vector4& second)
		{
			return Vector4(
				first - second.x,
				first - second.y,
				first - second.z,
				first - second.w);
		}

		// arithmetic updates
		inline Vector4& operator +=(const Vector4& rkVector)
		{
			x += rkVector.x;
			y += rkVector.y;
			z += rkVector.z;
			w += rkVector.w;

			return *this;
		}

		inline Vector4& operator -=(const Vector4& rkVector)
		{
			x -= rkVector.x;
			y -= rkVector.y;
			z -= rkVector.z;
			w -= rkVector.w;

			return *this;
		}

		inline Vector4& operator *=(const f32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}

		inline Vector4& operator +=(const f32 scalar)
		{
			x += scalar;
			y += scalar;
			z += scalar;
			w += scalar;
			return *this;
		}

		inline Vector4& operator -=(const f32 scalar)
		{
			x -= scalar;
			y -= scalar;
			z -= scalar;
			w -= scalar;
			return *this;
		}

		inline Vector4& operator *=(const Vector4& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;

			return *this;
		}

		inline Vector4& operator /=(const f32 scalar)
		{
			assert(scalar != 0.0);

			f32 fInv = 1.0f / scalar;

			x *= fInv;
			y *= fInv;
			z *= fInv;
			w *= fInv;

			return *this;
		}

		inline Vector4& operator /=(const Vector4& rkVector)
		{
			x /= rkVector.x;
			y /= rkVector.y;
			z /= rkVector.z;
			w /= rkVector.w;

			return *this;
		}

		/** Calculates the dot (scalar) product of this vector with another.
		@param
		vec Vector with which to calculate the dot product (together
		with this one).
		@return
		A float representing the dot product value.
		*/
		inline f32 dot(const Vector4& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
		}

		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
		}

		inline friend std::ostream& operator <<(std::ostream& o, const Vector4& v)
		{
			o << "Vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
			return o;
		}

		static const Vector4 ZERO;
	};
}


#endif