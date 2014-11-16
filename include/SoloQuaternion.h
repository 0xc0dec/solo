#ifndef __SOLO_QUATERNION_H__
#define __SOLO_QUATERNION_H__

#include "SoloCommon.h"
#include "SoloMath.h"

namespace solo
{
	class Vector3;
	class Matrix3;

	class Quaternion
	{
	public:
		/// Default constructor, initializes to identity rotation (aka 0°)
		inline Quaternion()
			: w(1), x(0), y(0), z(0)
		{
		}

		/// Construct from an explicit list of values
		inline Quaternion(f32 fW, f32 fX, f32 fY, f32 fZ)
			: w(fW), x(fX), y(fY), z(fZ)
		{
		}

		/// Construct a quaternion from a rotation matrix
		inline Quaternion(const Matrix3& rot)
		{
			this->fromRotationMatrix(rot);
		}

		/// Construct a quaternion from an angle/axis
		inline Quaternion(const Radian& rfAngle, const Vector3& rkAxis)
		{
			this->fromAngleAxis(rfAngle, rkAxis);
		}

		/// Construct a quaternion from 3 orthonormal local axes
		inline Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
		{
			this->fromAxes(xaxis, yaxis, zaxis);
		}

		/// Construct a quaternion from 3 orthonormal local axes
		inline Quaternion(const Vector3* akAxis)
		{
			this->fromAxes(akAxis);
		}

		/// Construct a quaternion from 4 manual w/x/y/z values
		inline Quaternion(f32* valptr)
		{
			memcpy(&w, valptr, sizeof(f32) * 4);
		}

		/** Exchange the contents of this quaternion with another.
		*/
		inline void swap(Quaternion& other)
		{
			std::swap(w, other.w);
			std::swap(x, other.x);
			std::swap(y, other.y);
			std::swap(z, other.z);
		}

		/// Array accessor operator
		inline f32 operator [](const size_t i) const
		{
			assert(i < 4);

			return *(&w + i);
		}

		/// Array accessor operator
		inline f32& operator [](const size_t i)
		{
			assert(i < 4);

			return *(&w + i);
		}

		/// Pointer accessor for direct copying
		inline f32* ptr()
		{
			return &w;
		}

		/// Pointer accessor for direct copying
		inline const f32* ptr() const
		{
			return &w;
		}

		void fromRotationMatrix(const Matrix3& rot);
		void toRotationMatrix(Matrix3& rot) const;

		/** Setups the quaternion using the supplied vector, and "roll" around
		that vector by the specified radians.
		*/
		void fromAngleAxis(const Radian& angle, const Vector3& axis);
		void toAngleAxis(Radian& angle, Vector3& axis) const;

		inline void toAngleAxis(Degree& angle, Vector3& axis) const
		{
			Radian a;
			toAngleAxis(a, axis);
			angle = a;
		}

		/** Constructs the quaternion using 3 axes, the axes are assumed to be orthonormal
		@see fromAxes
		*/
		void fromAxes(const Vector3* axes);
		void fromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

		/** Gets the 3 orthonormal axes defining the quaternion. @see fromAxes */
		void toAxes(Vector3* axes) const;
		void toAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;

		/** Returns the X orthonormal axis defining the quaternion. Same as doing
		xAxis = Vector3::UNIT_X * this. Also called the local X-axis
		*/
		Vector3 xAxis(void) const;

		/** Returns the Y orthonormal axis defining the quaternion. Same as doing
		yAxis = Vector3::UNIT_Y * this. Also called the local Y-axis
		*/
		Vector3 yAxis(void) const;

		/** Returns the Z orthonormal axis defining the quaternion. Same as doing
		zAxis = Vector3::UNIT_Z * this. Also called the local Z-axis
		*/
		Vector3 zAxis(void) const;

		inline Quaternion& operator=(const Quaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
			return *this;
		}

		Quaternion operator+(const Quaternion& other) const;
		Quaternion operator-(const Quaternion& other) const;
		Quaternion operator*(const Quaternion& other) const;
		Quaternion operator*(f32 scalar) const;
		friend Quaternion operator*(f32 scalar, const Quaternion& q);

		Quaternion operator-() const;

		inline bool operator==(const Quaternion& other) const
		{
			return (other.x == x) && (other.y == y) &&
				(other.z == z) && (other.w == w);
		}

		inline bool operator!=(const Quaternion& other) const
		{
			return !operator==(other);
		}

		// functions of a quaternion
		/// Returns the dot product of the quaternion
		f32 dot(const Quaternion& other) const;

		/* Returns the normal length of this quaternion.
		@note This does <b>not</b> alter any values.
		*/
		f32 normalLength() const;

		/// Normalises this quaternion, and returns the previous length
		f32 normalise(void);

		Quaternion inverse() const; // apply to non-zero quaternion
		Quaternion unitInverse() const; // apply to unit-length quaternion
		Quaternion exp() const;
		Quaternion log() const;

		/// Rotation of a vector by a quaternion
		Vector3 operator*(const Vector3& rkVector) const;

		/** Calculate the local roll element of this quaternion.
		@param reprojectAxis By default the method returns the 'intuitive' result
		that is, if you projected the local Y of the quaternion onto the X and
		Y axes, the angle between them is returned. If set to false though, the
		result is the actual yaw that will be used to implement the quaternion,
		which is the shortest possible path to get to the same orientation and
		may involve less axial rotation.  The co-domain of the returned value is
		from -180 to 180 degrees.
		*/
		Radian roll(bool reprojectAxis = true) const;

		/** Calculate the local pitch element of this quaternion
		@param reprojectAxis By default the method returns the 'intuitive' result
		that is, if you projected the local Z of the quaternion onto the X and
		Y axes, the angle between them is returned. If set to true though, the
		result is the actual yaw that will be used to implement the quaternion,
		which is the shortest possible path to get to the same orientation and
		may involve less axial rotation.  The co-domain of the returned value is
		from -180 to 180 degrees.
		*/
		Radian pitch(bool reprojectAxis = true) const;

		/** Calculate the local yaw element of this quaternion
		@param reprojectAxis By default the method returns the 'intuitive' result
		that is, if you projected the local Y of the quaternion onto the X and
		Z axes, the angle between them is returned. If set to true though, the
		result is the actual yaw that will be used to implement the quaternion,
		which is the shortest possible path to get to the same orientation and
		may involve less axial rotation. The co-domain of the returned value is
		from -180 to 180 degrees.
		*/
		Radian yaw(bool reprojectAxis = true) const;

		bool equals(const Quaternion& rhs, const Radian& tolerance) const;

		/** Performs Spherical linear interpolation between two quaternions, and returns the result.
		slerp ( 0.0f, A, B ) = A
		slerp ( 1.0f, A, B ) = B
		@return Interpolated quaternion
		@remarks
		slerp has the proprieties of performing the interpolation at constant
		velocity, and being torque-minimal (unless shortestPath=false).
		However, it's NOT commutative, which means
		slerp ( 0.75f, A, B ) != slerp ( 0.25f, B, A );
		therefore be careful if your code relies in the order of the operands.
		This is specially important in IK animation.
		*/
		static Quaternion slerp(f32 t, const Quaternion& p, const Quaternion& q, bool shortestPath = false);

		/** @see slerp. It adds extra "spins" (i.e. rotates several times) specified
		by parameter 'extraSpins' while interpolating before arriving to the
		final values
		*/
		static Quaternion slerpExtraSpins(f32 t, const Quaternion& p, const Quaternion& q, int extraSpins);

		// setup for spherical quadratic interpolation
		static void intermediate(const Quaternion& q0,
								const Quaternion& q1, const Quaternion& q2,
								Quaternion& a, Quaternion& b);

		// spherical quadratic interpolation
		static Quaternion squad(f32 t, const Quaternion& p,
								const Quaternion& a, const Quaternion& b,
								const Quaternion& q, bool shortestPath = false);

		/** Performs Normalised linear interpolation between two quaternions, and returns the result.
		nlerp ( 0.0f, A, B ) = A
		nlerp ( 1.0f, A, B ) = B
		@remarks
		Nlerp is faster than slerp.
		Nlerp has the proprieties of being commutative (@see slerp;
		commutativity is desired in certain places, like IK animation), and
		being torque-minimal (unless shortestPath=false). However, it's performing
		the interpolation at non-constant velocity; sometimes this is desired,
		sometimes it is not. Having a non-constant velocity can produce a more
		natural rotation feeling without the need of tweaking the weights; however
		if your scene relies on the timing of the rotation or assumes it will point
		at a specific angle at a specific weight value, slerp is a better choice.
		*/
		static Quaternion nlerp(f32 t, const Quaternion& p, const Quaternion& q, bool shortestPath = false);

		/// Cutoff for sine near zero
		static const f32 epsilon;

		static const Quaternion ZERO;
		static const Quaternion IDENTITY;

		f32 w, x, y, z;

		inline bool isNaN() const
		{
			return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
		}

		inline friend std::ostream& operator <<(std::ostream& o, const Quaternion& q)
		{
			o << "Quaternion(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
			return o;
		}
	};
}

#endif