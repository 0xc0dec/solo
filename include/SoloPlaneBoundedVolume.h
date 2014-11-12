#ifndef __SOLO_PLANE_BOUNDED_VOLUME_H__
#define __SOLO_PLANE_BOUNDED_VOLUME_H__

#include "SoloCommon.h"
#include "SoloPlane.h"
#include "SoloAxisAlignedBox.h"
#include "SoloSphere.h"

namespace solo
{
	class Ray;

	class PlaneBoundedVolume
	{
	public:
		typedef Array<Plane> PlaneList;
		/// Publicly accessible plane list, you can modify this direct
		PlaneList planes;
		Plane::Side outside;

		PlaneBoundedVolume() :outside(Plane::NEGATIVE_SIDE)
		{
		}

		/** Constructor, determines which side is deemed to be 'outside' */
		PlaneBoundedVolume(Plane::Side theOutside)
			: outside(theOutside)
		{
		}

		/** Intersection test with AABB
		@remarks May return false positives but will never miss an intersection.
		*/
		inline bool intersects(const AxisAlignedBox& box) const
		{
			if (box.isNull()) return false;
			if (box.isInfinite()) return true;

			// Get centre of the box
			Vector3 centre = box.getCenter();
			// Get the half-size of the box
			Vector3 halfSize = box.getHalfSize();

			PlaneList::const_iterator i, iend;
			iend = planes.end();
			for (i = planes.begin(); i != iend; ++i)
			{
				const Plane& plane = *i;

				Plane::Side side = plane.getSide(centre, halfSize);
				if (side == outside)
				{
					// Found a splitting plane therefore return not intersecting
					return false;
				}
			}

			// couldn't find a splitting plane, assume intersecting
			return true;
		}

		/** Intersection test with Sphere
		@remarks May return false positives but will never miss an intersection.
		*/
		inline bool intersects(const Sphere& sphere) const
		{
			PlaneList::const_iterator i, iend;
			iend = planes.end();
			for (i = planes.begin(); i != iend; ++i)
			{
				const Plane& plane = *i;

				// Test which side of the plane the sphere is
				f32 d = plane.getDistance(sphere.getCenter());
				// Negate d if planes point inwards
				if (outside == Plane::NEGATIVE_SIDE) d = -d;

				if ((d - sphere.getRadius()) > 0)
					return false;
			}

			return true;
		}

		/** Intersection test with a Ray
		@return Pair of hit (bool) and distance
		@remarks May return false positives but will never miss an intersection.
		*/
		inline Pair<bool, f32> intersects(const Ray& ray)
		{
			return Math::intersects(ray, planes, outside == Plane::POSITIVE_SIDE);
		}
	};
}


#endif