#ifndef __SOLO_PLANE_H__
#define __SOLO_PLANE_H__

#include "SoloMath.h"
#include "SoloVector3.h"
#include "SoloAxisAlignedBox.h"

namespace solo
{
	class Plane
	{
	public:
		Plane();
		Plane(const Plane& rhs);
		/** Construct a plane through a normal, and a distance to move the plane along the normal.*/
		Plane(const Vector3& rkNormal, f32 fConstant);
		/** Construct a plane using the 4 constants directly **/
		Plane(f32 a, f32 b, f32 c, f32 d);
		Plane(const Vector3& rkNormal, const Vector3& rkPoint);
		Plane(const Vector3& rkPoint0, const Vector3& rkPoint1, const Vector3& rkPoint2);

		/** The "positive side" of the plane is the half space to which the
		plane normal points. The "negative side" is the other half
		space. The flag "no side" indicates the plane itself.
		*/
		enum Side
		{
			NO_SIDE,
			POSITIVE_SIDE,
			NEGATIVE_SIDE,
			BOTH_SIDE
		};

		Side getSide(const Vector3& rkPoint) const;

		/**
		Returns the side where the alignedBox is. The flag BOTH_SIDE indicates an intersecting box.
		One corner ON the plane is sufficient to consider the box and the plane intersecting.
		*/
		Side getSide(const AxisAlignedBox& rkBox) const;

		/** Returns which side of the plane that the given box lies on.
		The box is defined as centre/half-size pairs for effectively.
		@param centre The centre of the box.
		@param halfSize The half-size of the box.
		@return
		POSITIVE_SIDE if the box complete lies on the "positive side" of the plane,
		NEGATIVE_SIDE if the box complete lies on the "negative side" of the plane,
		and BOTH_SIDE if the box intersects the plane.
		*/
		Side getSide(const Vector3& centre, const Vector3& halfSize) const;

		/** This is a pseudodistance. The sign of the return value is
		positive if the point is on the positive side of the plane,
		negative if the point is on the negative side, and zero if the
		point is on the plane.
		@par
		The absolute value of the return value is the true distance only
		when the plane normal is a unit length vector.
		*/
		f32 getDistance(const Vector3& rkPoint) const;

		/** Redefine this plane based on 3 points. */
		void redefine(const Vector3& rkPoint0, const Vector3& rkPoint1,
					const Vector3& rkPoint2);

		/** Redefine this plane based on a normal and a point. */
		void redefine(const Vector3& rkNormal, const Vector3& rkPoint);

		/** Project a vector onto the plane.
		@remarks This gives you the element of the input vector that is perpendicular
		to the normal of the plane. You can get the element which is parallel
		to the normal of the plane by subtracting the result of this method
		from the original vector, since parallel + perpendicular = original.
		@param v The input vector
		*/
		Vector3 projectVector(const Vector3& v) const;

		/** Normalises the plane.
		@remarks
		This method normalises the plane's normal and the length scale of d
		is as well.
		@note
		This function will not crash for zero-sized vectors, but there
		will be no changes made to their components.
		@return The previous length of the plane's normal.
		*/
		f32 normalise(void);

		Vector3 normal;
		f32 d;

		/// Comparison operator
		bool operator==(const Plane& rhs) const
		{
			return (rhs.d == d && rhs.normal == normal);
		}

		bool operator!=(const Plane& rhs) const
		{
			return (rhs.d != d || rhs.normal != normal);
		}

		friend std::ostream& operator<<(std::ostream& o, const Plane& p);
	};
}

#endif