#ifndef __SOLO_RAY_H__
#define __SOLO_RAY_H__

#include "SoloVector3.h"
#include "SoloPlaneBoundedVolume.h"

namespace solo
{
	class Ray
	{
	protected:
		Vector3 mOrigin;
		Vector3 mDirection;
		
	public:
		Ray() :mOrigin(Vector3::ZERO), mDirection(Vector3::UNIT_Z)
		{
		}

		Ray(const Vector3& origin, const Vector3& direction)
			:mOrigin(origin), mDirection(direction)
		{
		}

		/** Sets the origin of the ray. */
		void setOrigin(const Vector3& origin)
		{
			mOrigin = origin;
		}

		/** Gets the origin of the ray. */
		const Vector3& getOrigin(void) const
		{
			return mOrigin;
		}

		/** Sets the direction of the ray. */
		void setDirection(const Vector3& dir)
		{
			mDirection = dir;
		}

		/** Gets the direction of the ray. */
		const Vector3& getDirection(void) const
		{
			return mDirection;
		}

		/** Gets the position of a point t units along the ray. */
		Vector3 getPoint(f32 t) const
		{
			return Vector3(mOrigin + (mDirection * t));
		}

		/** Gets the position of a point t units along the ray. */
		Vector3 operator*(f32 t) const
		{
			return getPoint(t);
		}

		/** Tests whether this ray intersects the given plane.
		@return A pair structure where the first element indicates whether
		an intersection occurs, and if true, the second element will
		indicate the distance along the ray at which it intersects.
		This can be converted to a point in space by calling getPoint().
		*/
		Pair<bool, f32> intersects(const Plane& p) const
		{
			return Math::intersects(*this, p);
		}

		/** Tests whether this ray intersects the given plane bounded volume.
		@return A pair structure where the first element indicates whether
		an intersection occurs, and if true, the second element will
		indicate the distance along the ray at which it intersects.
		This can be converted to a point in space by calling getPoint().
		*/
		Pair<bool, f32> intersects(const PlaneBoundedVolume& p) const
		{
			return Math::intersects(*this, p.planes, p.outside == Plane::POSITIVE_SIDE);
		}

		/** Tests whether this ray intersects the given sphere.
		@return A pair structure where the first element indicates whether
		an intersection occurs, and if true, the second element will
		indicate the distance along the ray at which it intersects.
		This can be converted to a point in space by calling getPoint().
		*/
		Pair<bool, f32> intersects(const Sphere& s) const
		{
			return Math::intersects(*this, s);
		}

		/** Tests whether this ray intersects the given box.
		@return A pair structure where the first element indicates whether
		an intersection occurs, and if true, the second element will
		indicate the distance along the ray at which it intersects.
		This can be converted to a point in space by calling getPoint().
		*/
		Pair<bool, f32> intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}
	};
}


#endif