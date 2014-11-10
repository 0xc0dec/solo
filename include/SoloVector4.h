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

		inline Vector4(const f32 fX, const f32 fY, const f32 fZ, const f32 fW)
			: x(fX), y(fY), z(fZ), w(fW)
		{
		}

		inline explicit Vector4(const f32 afCoordinate[4])
			: x(afCoordinate[0]), y(afCoordinate[1]), z(afCoordinate[2]), w(afCoordinate[3])
		{
		}

		inline explicit Vector4(const int afCoordinate[4])
		{
			x = static_cast<f32>(afCoordinate[0]);
			y = static_cast<f32>(afCoordinate[1]);
			z = static_cast<f32>(afCoordinate[2]);
			w = static_cast<f32>(afCoordinate[3]);
		}

		inline explicit Vector4(f32* const r)
			: x(r[0]), y(r[1]), z(r[2]), w(r[3])
		{
		}

		inline explicit Vector4(const f32 scaler)
			: x(scaler) , y(scaler) , z(scaler) , w(scaler)
		{
		}

		inline explicit Vector4(const Vector3& rhs)
			: x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
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
		rkVector The other vector
		*/
		inline Vector4& operator =(const Vector4& rkVector)
		{
			x = rkVector.x;
			y = rkVector.y;
			z = rkVector.z;
			w = rkVector.w;

			return *this;
		}

		inline Vector4& operator =(const f32 fScalar)
		{
			x = fScalar;
			y = fScalar;
			z = fScalar;
			w = fScalar;
			return *this;
		}

		inline bool operator ==(const Vector4& rkVector) const
		{
			return (x == rkVector.x &&
				y == rkVector.y &&
				z == rkVector.z &&
				w == rkVector.w);
		}

		inline bool operator !=(const Vector4& rkVector) const
		{
			return (x != rkVector.x ||
				y != rkVector.y ||
				z != rkVector.z ||
				w != rkVector.w);
		}

		inline Vector4& operator =(const Vector3& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = 1.0f;
			return *this;
		}

		// arithmetic operations
		inline Vector4 operator +(const Vector4& rkVector) const
		{
			return Vector4(
				x + rkVector.x,
				y + rkVector.y,
				z + rkVector.z,
				w + rkVector.w);
		}

		inline Vector4 operator -(const Vector4& rkVector) const
		{
			return Vector4(
				x - rkVector.x,
				y - rkVector.y,
				z - rkVector.z,
				w - rkVector.w);
		}

		inline Vector4 operator *(const f32 fScalar) const
		{
			return Vector4(
				x * fScalar,
				y * fScalar,
				z * fScalar,
				w * fScalar);
		}

		inline Vector4 operator *(const Vector4& rhs) const
		{
			return Vector4(
				rhs.x * x,
				rhs.y * y,
				rhs.z * z,
				rhs.w * w);
		}

		inline Vector4 operator /(const f32 fScalar) const
		{
			assert(fScalar != 0.0);

			f32 fInv = 1.0f / fScalar;

			return Vector4(
				x * fInv,
				y * fInv,
				z * fInv,
				w * fInv);
		}

		inline Vector4 operator /(const Vector4& rhs) const
		{
			return Vector4(
				x / rhs.x,
				y / rhs.y,
				z / rhs.z,
				w / rhs.w);
		}

		inline const Vector4& operator +() const
		{
			return *this;
		}

		inline Vector4 operator -() const
		{
			return Vector4(-x, -y, -z, -w);
		}

		inline friend Vector4 operator *(const f32 fScalar, const Vector4& rkVector)
		{
			return Vector4(
				fScalar * rkVector.x,
				fScalar * rkVector.y,
				fScalar * rkVector.z,
				fScalar * rkVector.w);
		}

		inline friend Vector4 operator /(const f32 fScalar, const Vector4& rkVector)
		{
			return Vector4(
				fScalar / rkVector.x,
				fScalar / rkVector.y,
				fScalar / rkVector.z,
				fScalar / rkVector.w);
		}

		inline friend Vector4 operator +(const Vector4& lhs, const f32 rhs)
		{
			return Vector4(
				lhs.x + rhs,
				lhs.y + rhs,
				lhs.z + rhs,
				lhs.w + rhs);
		}

		inline friend Vector4 operator +(const f32 lhs, const Vector4& rhs)
		{
			return Vector4(
				lhs + rhs.x,
				lhs + rhs.y,
				lhs + rhs.z,
				lhs + rhs.w);
		}

		inline friend Vector4 operator -(const Vector4& lhs, f32 rhs)
		{
			return Vector4(
				lhs.x - rhs,
				lhs.y - rhs,
				lhs.z - rhs,
				lhs.w - rhs);
		}

		inline friend Vector4 operator -(const f32 lhs, const Vector4& rhs)
		{
			return Vector4(
				lhs - rhs.x,
				lhs - rhs.y,
				lhs - rhs.z,
				lhs - rhs.w);
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

		inline Vector4& operator *=(const f32 fScalar)
		{
			x *= fScalar;
			y *= fScalar;
			z *= fScalar;
			w *= fScalar;
			return *this;
		}

		inline Vector4& operator +=(const f32 fScalar)
		{
			x += fScalar;
			y += fScalar;
			z += fScalar;
			w += fScalar;
			return *this;
		}

		inline Vector4& operator -=(const f32 fScalar)
		{
			x -= fScalar;
			y -= fScalar;
			z -= fScalar;
			w -= fScalar;
			return *this;
		}

		inline Vector4& operator *=(const Vector4& rkVector)
		{
			x *= rkVector.x;
			y *= rkVector.y;
			z *= rkVector.z;
			w *= rkVector.w;

			return *this;
		}

		inline Vector4& operator /=(const f32 fScalar)
		{
			assert(fScalar != 0.0);

			f32 fInv = 1.0f / fScalar;

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
		inline f32 dotProduct(const Vector4& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
		}

		/// Check whether this vector contains valid values
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
		}

		/** Function for writing to a stream.
		*/
		inline friend std::ostream& operator <<(std::ostream& o, const Vector4& v)
		{
			o << "Vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
			return o;
		}

		// special
		static const Vector4 ZERO;
	};
}


#endif