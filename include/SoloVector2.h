#ifndef __SOLO_VECTOR2_H__
#define __SOLO_VECTOR2_H__

#include "SoloCommon.h"
#include "SoloMath.h"

namespace solo
{
	class Vector2
	{
	public:
		f32 x, y;

		inline Vector2()
		{
		}

		inline Vector2(const f32 x, const f32 y)
			: x(x), y(y)
		{
		}

		inline explicit Vector2(const f32 scaler)
			: x(scaler), y(scaler)
		{
		}

		inline explicit Vector2(const f32 coords[2])
			: x(coords[0]), y(coords[1])
		{
		}

		inline explicit Vector2(const int coords[2])
		{
			x = static_cast<f32>(coords[0]);
			y = static_cast<f32>(coords[1]);
		}

		inline explicit Vector2(f32* const r)
			: x(r[0]), y(r[1])
		{
		}

		/** Exchange the contents of this vector with another.
		*/
		inline void swap(Vector2& other)
		{
			std::swap(x, other.x);
			std::swap(y, other.y);
		}

		inline f32 operator [](const size_t i) const
		{
			assert(i < 2);

			return *(&x + i);
		}

		inline f32& operator [](const size_t i)
		{
			assert(i < 2);

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
		inline Vector2& operator =(const Vector2& other)
		{
			x = other.x;
			y = other.y;

			return *this;
		}

		inline Vector2& operator =(const f32 scalar)
		{
			x = scalar;
			y = scalar;

			return *this;
		}

		inline bool operator ==(const Vector2& other) const
		{
			return (x == other.x && y == other.y);
		}

		inline bool operator !=(const Vector2& other) const
		{
			return (x != other.x || y != other.y);
		}

		inline Vector2 operator +(const Vector2& other) const
		{
			return Vector2(
				x + other.x,
				y + other.y);
		}

		inline Vector2 operator -(const Vector2& other) const
		{
			return Vector2(
				x - other.x,
				y - other.y);
		}

		inline Vector2 operator *(const f32 scalar) const
		{
			return Vector2(
				x * scalar,
				y * scalar);
		}

		inline Vector2 operator *(const Vector2& rhs) const
		{
			return Vector2(
				x * rhs.x,
				y * rhs.y);
		}

		inline Vector2 operator /(const f32 scalar) const
		{
			assert(scalar != 0.0);

			f32 fInv = 1.0f / scalar;

			return Vector2(
				x * fInv,
				y * fInv);
		}

		inline Vector2 operator /(const Vector2& other) const
		{
			return Vector2(x / other.x, y / other.y);
		}

		inline const Vector2& operator +() const
		{
			return *this;
		}

		inline Vector2 operator -() const
		{
			return Vector2(-x, -y);
		}

		// overloaded operators to help Vector2
		inline friend Vector2 operator *(const f32 scalar, const Vector2& other)
		{
			return Vector2(scalar * other.x, scalar * other.y);
		}

		inline friend Vector2 operator /(const f32 scalar, const Vector2& other)
		{
			return Vector2(scalar / other.x, scalar / other.y);
		}

		inline friend Vector2 operator +(const Vector2& vec, const f32 scalar)
		{
			return Vector2(vec.x + scalar, vec.y + scalar);
		}

		inline friend Vector2 operator +(const f32 scalar, const Vector2& vector)
		{
			return Vector2(scalar + vector.x, scalar + vector.y);
		}

		inline friend Vector2 operator -(const Vector2& vector, const f32 scalar)
		{
			return Vector2(vector.x - scalar, vector.y - scalar);
		}

		inline friend Vector2 operator -(const f32 scalar, const Vector2& vector)
		{
			return Vector2(scalar - vector.x, scalar - vector.y);
		}

		// arithmetic updates
		inline Vector2& operator +=(const Vector2& other)
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		inline Vector2& operator +=(const f32 scalar)
		{
			x += scalar;
			y += scalar;
			return *this;
		}

		inline Vector2& operator -=(const Vector2& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		inline Vector2& operator -=(const f32 scalar)
		{
			x -= scalar;
			y -= scalar;
			return *this;
		}

		inline Vector2& operator *=(const f32 scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		inline Vector2& operator *=(const Vector2& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}

		inline Vector2& operator /=(const f32 scalar)
		{
			assert(scalar != 0.0);

			f32 inv = 1.0f / scalar;

			x *= inv;
			y *= inv;

			return *this;
		}

		inline Vector2& operator /=(const Vector2& other)
		{
			x /= other.x;
			y /= other.y;

			return *this;
		}

		/** Returns the length (magnitude) of the vector.
		@warning
		This operation requires a square root and is expensive in
		terms of CPU operations. If you don't need to know the exact
		length (e.g. for just comparing lengths) use squaredLength()
		instead.
		*/
		inline f32 length() const
		{
			return Math::Sqrt(x * x + y * y);
		}

		/** Returns the square of the length(magnitude) of the vector.
		@remarks
		This  method is for efficiency - calculating the actual
		length of a vector requires a square root, which is expensive
		in terms of the operations required. This method returns the
		square of the length of the vector, i.e. the same as the
		length but before the square root is taken. Use this if you
		want to find the longest / shortest vector without incurring
		the square root.
		*/
		inline f32 squaredLength() const
		{
			return x * x + y * y;
		}

		/** Returns the distance to another vector.
		@warning
		This operation requires a square root and is expensive in
		terms of CPU operations. If you don't need to know the exact
		distance (e.g. for just comparing distances) use squaredDistance()
		instead.
		*/
		inline f32 distance(const Vector2& other) const
		{
			return (*this - other).length();
		}

		/** Returns the square of the distance to another vector.
		@remarks
		This method is for efficiency - calculating the actual
		distance to another vector requires a square root, which is
		expensive in terms of the operations required. This method
		returns the square of the distance to another vector, i.e.
		the same as the distance but before the square root is taken.
		Use this if you want to find the longest / shortest distance
		without incurring the square root.
		*/
		inline f32 squaredDistance(const Vector2& rhs) const
		{
			return (*this - rhs).squaredLength();
		}

		/** Calculates the dot (scalar) product of this vector with another.
		@remarks
		The dot product can be used to calculate the angle between 2
		vectors. If both are unit vectors, the dot product is the
		cosine of the angle; otherwise the dot product must be
		divided by the product of the lengths of both vectors to get
		the cosine of the angle. This result can further be used to
		calculate the distance of a point from a plane.
		@param
		vec Vector with which to calculate the dot product (together
		with this one).
		@return
		A float representing the dot product value.
		*/
		inline f32 dot(const Vector2& vec) const
		{
			return x * vec.x + y * vec.y;
		}

		/** Normalises the vector.
		@remarks
		This method normalises the vector such that it's
		length / magnitude is 1. The result is called a unit vector.
		@note
		This function will not crash for zero-sized vectors, but there
		will be no changes made to their components.
		@return The previous length of the vector.
		*/

		inline f32 normalize()
		{
			f32 len = Math::Sqrt(x * x + y * y);

			// Will also work for zero-sized vectors, but will change nothing
			// We're not using epsilons because we don't need to.
			// Read http://www.ogre3d.org/forums/viewtopic.php?f=4&t=61259
			if (len > f32(0.0f))
			{
				f32 invLen = 1.0f / len;
				x *= invLen;
				y *= invLen;
			}

			return len;
		}

		/** Returns a vector at a point half way between this and the passed
		in vector.
		*/
		inline Vector2 midPoint(const Vector2& vec) const
		{
			return Vector2(
				(x + vec.x) * 0.5f,
				(y + vec.y) * 0.5f);
		}

		/** Returns true if the vector's scalar components are all greater
		that the ones of the vector it is compared against.
		*/
		inline bool operator <(const Vector2& other) const
		{
			if (x < other.x && y < other.y)
				return true;
			return false;
		}

		/** Returns true if the vector's scalar components are all smaller
		that the ones of the vector it is compared against.
		*/
		inline bool operator >(const Vector2& other) const
		{
			if (x > other.x && y > other.y)
				return true;
			return false;
		}

		/** Sets this vector's components to the minimum of its own and the
		ones of the passed in vector.
		@remarks
		'Minimum' in this case means the combination of the lowest
		value of x, y and z from both vectors. Lowest is taken just
		numerically, not magnitude, so -1 < 0.
		*/
		inline void makeFloor(const Vector2& cmp)
		{
			if (cmp.x < x) x = cmp.x;
			if (cmp.y < y) y = cmp.y;
		}

		/** Sets this vector's components to the maximum of its own and the
		ones of the passed in vector.
		@remarks
		'Maximum' in this case means the combination of the highest
		value of x, y and z from both vectors. Highest is taken just
		numerically, not magnitude, so 1 > -3.
		*/
		inline void makeCeil(const Vector2& cmp)
		{
			if (cmp.x > x) x = cmp.x;
			if (cmp.y > y) y = cmp.y;
		}

		/** Generates a vector perpendicular to this vector (eg an 'up' vector).
		@remarks
		This method will return a vector which is perpendicular to this
		vector. There are an infinite number of possibilities but this
		method will guarantee to generate one of them. If you need more
		control you should use the Quaternion class.
		*/
		inline Vector2 perpendicular(void) const
		{
			return Vector2(-y, x);
		}

		/** Calculates the 2 dimensional cross-product of 2 vectors, which results
		in a single floating point value which is 2 times the area of the triangle.
		*/
		inline f32 crossProduct(const Vector2& other) const
		{
			return x * other.y - y * other.x;
		}

		/** Generates a new random vector which deviates from this vector by a
		given angle in a random direction.
		@remarks
		This method assumes that the random number generator has already
		been seeded appropriately.
		@param
		angle The angle at which to deviate in radians
		@param
		up Any vector perpendicular to this one (which could generated
		by cross-product of this vector and any other non-colinear
		vector). If you choose not to provide this the function will
		derive one on it's own, however if you provide one yourself the
		function will be faster (this allows you to reuse up vectors if
		you call this method more than once)
		@return
		A random vector which deviates from this vector by angle. This
		vector will not be normalised, normalise it if you wish
		afterwards.
		*/
		inline Vector2 randomDeviant(f32 angle) const
		{
			angle *= Math::UnitRandom() * Math::TWO_PI;
			f32 cosa = cos(angle);
			f32 sina = sin(angle);
			return Vector2(cosa * x - sina * y,
							sina * x + cosa * y);
		}

		/** Returns true if this vector is zero length. */
		inline bool isZeroLength(void) const
		{
			f32 sqlen = (x * x) + (y * y);
			return (sqlen < (1e-06 * 1e-06));
		}

		/** As normalise, except that this vector is unaffected and the
		normalised vector is returned as a copy. */
		inline Vector2 normalisedCopy(void) const
		{
			Vector2 ret = *this;
			ret.normalize();
			return ret;
		}

		/** Calculates a reflection vector to the plane with the given normal .
		@remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
		*/
		inline Vector2 reflect(const Vector2& normal) const
		{
			return Vector2(*this - (2 * this->dot(normal) * normal));
		}

		/// Check whether this vector contains valid values
		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y);
		}

		/**	 Gets the angle between 2 vectors.
		@remarks
		Vectors do not have to be unit-length but must represent directions.
		*/
		inline Radian angleBetween(const Vector2& other) const
		{
			f32 lenProduct = length() * other.length();
			// Divide by zero check
			if (lenProduct < 1e-6f)
				lenProduct = 1e-6f;

			f32 f = dot(other) / lenProduct;

			f = Math::Clamp(f, static_cast<f32>(- 1.0), static_cast<f32>(1.0));
			return Math::ACos(f);
		}

		/**	 Gets the oriented angle between 2 vectors.
		@remarks
		Vectors do not have to be unit-length but must represent directions.
		The angle is comprised between 0 and 2 PI.
		*/
		inline Radian angleTo(const Vector2& other) const
		{
			Radian angle = angleBetween(other);

			if (crossProduct(other) < 0)
				angle = static_cast<Radian>(Math::TWO_PI) - angle;

			return angle;
		}

		// special points
		static const Vector2 ZERO;
		static const Vector2 UNIT_X;
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;
		static const Vector2 UNIT_SCALE;

		inline friend std::ostream& operator <<(std::ostream& o, const Vector2& v)
		{
			o << "Vector2(" << v.x << ", " << v.y << ")";
			return o;
		}
	};
}

#endif