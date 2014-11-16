#ifndef __SOLO_SPHERE_H__
#define __SOLO_SPHERE_H__

#include "SoloVector3.h"
#include "SoloAxisAlignedBox.h"

namespace solo
{
	class Sphere
	{
	protected:
		f32 mRadius;
		Vector3 mCenter;
	public:
		/** Standard constructor - creates a unit sphere around the origin.*/
		Sphere() : mRadius(1.0), mCenter(Vector3::ZERO)
		{
		}

		/** Constructor allowing arbitrary spheres.
		@param center The center point of the sphere.
		@param radius The radius of the sphere.
		*/
		Sphere(const Vector3& center, f32 radius)
			: mRadius(radius), mCenter(center)
		{
		}

		/** Returns the radius of the sphere. */
		f32 getRadius(void) const
		{
			return mRadius;
		}

		/** Sets the radius of the sphere. */
		void setRadius(f32 radius)
		{
			mRadius = radius;
		}

		/** Returns the center point of the sphere. */
		const Vector3& getCenter(void) const
		{
			return mCenter;
		}

		/** Sets the center point of the sphere. */
		void setCenter(const Vector3& center)
		{
			mCenter = center;
		}

		/** Returns whether or not this sphere intersects another sphere. */
		bool intersects(const Sphere& s) const
		{
			return (s.mCenter - mCenter).squaredLength() <=
				Math::sqr(s.mRadius + mRadius);
		}

		/** Returns whether or not this sphere intersects a box. */
		bool intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}

		/** Returns whether or not this sphere intersects a plane. */
		bool intersects(const Plane& plane) const
		{
			return Math::intersects(*this, plane);
		}

		/** Returns whether or not this sphere intersects a point. */
		bool intersects(const Vector3& v) const
		{
			return ((v - mCenter).squaredLength() <= Math::sqr(mRadius));
		}

		/** Merges another Sphere into the current sphere */
		void merge(const Sphere& oth)
		{
			Vector3 diff = oth.getCenter() - mCenter;
			f32 lengthSq = diff.squaredLength();
			f32 radiusDiff = oth.getRadius() - mRadius;

			// Early-out
			if (Math::sqr(radiusDiff) >= lengthSq)
			{
				// One fully contains the other
				if (radiusDiff <= 0.0f)
					return; // no change
				mCenter = oth.getCenter();
				mRadius = oth.getRadius();
				return;
			}

			f32 length = Math::sqrt(lengthSq);

			Vector3 newCenter;
			f32 newRadius;
			if ((length + oth.getRadius()) > mRadius)
			{
				f32 t = (length + radiusDiff) / (2.0f * length);
				newCenter = mCenter + diff * t;
			}
			// otherwise, we keep our existing center

			newRadius = 0.5f * (length + mRadius + oth.getRadius());

			mCenter = newCenter;
			mRadius = newRadius;
		}
	};
}

#endif