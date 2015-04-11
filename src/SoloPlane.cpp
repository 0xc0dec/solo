#include "SoloPlane.h"
#include "SoloMath.h"
#include "SoloMatrix.h"
#include "SoloBoundingSphere.h"
#include "SoloBoundingBox.h"
#include "SoloFrustum.h"
#include "SoloRay.h"

using namespace solo;


Plane::Plane(const Vector3& normal, float distance)
{
	set(normal, distance);
}


Plane::Plane(float normalX, float normalY, float normalZ, float distance)
{
	set(Vector3(normalX, normalY, normalZ), distance);
}


const Vector3& Plane::getNormal() const
{
	return normal;
}


void Plane::setNormal(const Vector3& normal)
{
	this->normal = normal;
	normalize();
}


void Plane::setNormal(float x, float y, float z)
{
	normal.set(x, y, z);
	normalize();
}


float Plane::getDistance() const
{
	return distance;
}


void Plane::setDistance(float distance)
{
	this->distance = distance;
}


float Plane::getDistance(const Vector3& point) const
{
	return normal.x * point.x + normal.y * point.y + normal.z * point.z + distance;
}


void Plane::intersection(const Plane& p1, const Plane& p2, const Plane& p3, Vector3* point)
{
	// The planes' normals must be all normalized (which we guarantee in the Plane class).
	// Calculate the determinant of the matrix (i.e | n1 n2 n3 |).
	auto det = p1.normal.x * (p2.normal.y * p3.normal.z -
		p2.normal.z * p3.normal.y) - p2.normal.x *(p1.normal.y * p3.normal.z -
		p1.normal.z * p3.normal.y) + p3.normal.x * (p1.normal.y * p2.normal.z - p1.normal.z * p2.normal.y);

	// If the determinant is zero, then the planes do not all intersect.
	if (fabs(det) <= MATH_EPSILON)
		return;

	// Create 3 points, one on each plane.
	// (We just pick the point on the plane directly along its normal from the origin).
	auto p1x = -p1.normal.x * p1.distance;
	auto p1y = -p1.normal.y * p1.distance;
	auto p1z = -p1.normal.z * p1.distance;
	auto p2x = -p2.normal.x * p2.distance;
	auto p2y = -p2.normal.y * p2.distance;
	auto p2z = -p2.normal.z * p2.distance;
	auto p3x = -p3.normal.x * p3.distance;
	auto p3y = -p3.normal.y * p3.distance;
	auto p3z = -p3.normal.z * p3.distance;

	// Calculate the cross products of the normals.
	auto c1x = (p2.normal.y * p3.normal.z) - (p2.normal.z * p3.normal.y);
	auto c1y = (p2.normal.z * p3.normal.x) - (p2.normal.x * p3.normal.z);
	auto c1z = (p2.normal.x * p3.normal.y) - (p2.normal.y * p3.normal.x);
	auto c2x = (p3.normal.y * p1.normal.z) - (p3.normal.z * p1.normal.y);
	auto c2y = (p3.normal.z * p1.normal.x) - (p3.normal.x * p1.normal.z);
	auto c2z = (p3.normal.x * p1.normal.y) - (p3.normal.y * p1.normal.x);
	auto c3x = (p1.normal.y * p2.normal.z) - (p1.normal.z * p2.normal.y);
	auto c3y = (p1.normal.z * p2.normal.x) - (p1.normal.x * p2.normal.z);
	auto c3z = (p1.normal.x * p2.normal.y) - (p1.normal.y * p2.normal.x);

	// Calculate the point of intersection using the formula:
	// x = (| n1 n2 n3 |)^-1 * [(x1 * n1)(n2 x n3) + (x2 * n2)(n3 x n1) + (x3 * n3)(n1 x n2)]
	auto s1 = p1x * p1.normal.x + p1y * p1.normal.y + p1z * p1.normal.z;
	auto s2 = p2x * p2.normal.x + p2y * p2.normal.y + p2z * p2.normal.z;
	auto s3 = p3x * p3.normal.x + p3y * p3.normal.y + p3z * p3.normal.z;
	auto detI = 1.0f / det;
	point->x = (s1 * c1x + s2 * c2x + s3 * c3x) * detI;
	point->y = (s1 * c1y + s2 * c2y + s3 * c3y) * detI;
	point->z = (s1 * c1z + s2 * c2z + s3 * c3z) * detI;
}


Plane::PlaneIntersection Plane::getIntersection(const BoundingSphere &sphere) const
{
	return sphere.intersects(*this);
}


Plane::PlaneIntersection Plane::getIntersection(const BoundingBox &box) const
{
	return box.intersects(*this);
}


Plane::PlaneIntersection Plane::getIntersection(const Frustum &frustum) const
{
	Vector3 corners[8];
	frustum.getCorners(corners);

	// Calculate the distances from all of the corners to the plane.
	// If all of the distances are positive, then the frustum is in the
	// positive half-space of this plane; if all the distances are negative,
	// then the frustum is in the negative half-space of this plane; if some of
	// the distances are positive and some are negative, the frustum getIntersection.
	auto d = getDistance(corners[0]);
	if (d > 0.0f)
	{
		if (getDistance(corners[1]) <= 0.0f ||
			getDistance(corners[2]) <= 0.0f ||
			getDistance(corners[3]) <= 0.0f ||
			getDistance(corners[4]) <= 0.0f ||
			getDistance(corners[5]) <= 0.0f ||
			getDistance(corners[6]) <= 0.0f ||
			getDistance(corners[7]) <= 0.0f)
		{
			return PlaneIntersection::Intersecting;
		}

		return PlaneIntersection::Front;
	}
	if (d < 0.0f)
	{
		if (getDistance(corners[1]) >= 0.0f ||
			getDistance(corners[2]) >= 0.0f ||
			getDistance(corners[3]) >= 0.0f ||
			getDistance(corners[4]) >= 0.0f ||
			getDistance(corners[5]) >= 0.0f ||
			getDistance(corners[6]) >= 0.0f ||
			getDistance(corners[7]) >= 0.0f)
		{
			return PlaneIntersection::Intersecting;
		}

		return PlaneIntersection::Back;
	}
	
	return PlaneIntersection::Intersecting;
}


Plane::PlaneIntersection Plane::getIntersection(const Plane &plane) const
{
	// Check if the planes intersect.
	if ((normal.x == plane.normal.x && normal.y == plane.normal.y && normal.z == plane.normal.z) || !isParallel(plane))
		return PlaneIntersection::Intersecting;

	// Calculate the point where the given plane's normal vector getIntersection the given plane.
	Vector3 point(plane.normal.x * -plane.distance, plane.normal.y * -plane.distance, plane.normal.z * -plane.distance);

	// Calculate whether the given plane is in the positive or negative half-space of this plane
	// (corresponds directly to the sign of the distance from the point calculated above to this plane).
	if (getDistance(point) > 0.0f)
		return PlaneIntersection::Front;
	return PlaneIntersection::Back;
}


Plane::PlaneIntersection Plane::getIntersection(const Ray &ray) const
{
	// Calculate the distance from the ray's origin to the plane.
	auto d = getDistance(ray.getOrigin());

	// If the origin of the ray lies in the plane, then it getIntersection.
	if (d == 0.0f)
		return PlaneIntersection::Intersecting;
	auto rayDirection = ray.getDirection();
	// If the dot product of this plane's normal and the ray's direction is positive, and
	// if the distance from this plane to the ray's origin is negative -> intersection, OR
	// if the dot product of this plane's normal and the ray's direction is negative, and
	// if the distance from this plane to the ray's origin is positive -> intersection.
	if (normal.x * rayDirection.x + normal.y * rayDirection.y + normal.z * rayDirection.z > 0.0f)
		return d < 0.0f ? PlaneIntersection::Intersecting : PlaneIntersection::Front;
	return d > 0.0f ? PlaneIntersection::Intersecting : PlaneIntersection::Back;
}


bool Plane::isParallel(const Plane& plane) const
{
	return (normal.y * plane.normal.z) - (normal.z * plane.normal.y) == 0.0f &&
		(normal.z * plane.normal.x) - (normal.x * plane.normal.z) == 0.0f &&
		(normal.x * plane.normal.y) - (normal.y * plane.normal.x) == 0.0f;
}


void Plane::set(const Vector3& normal, float distance)
{
	this->normal = normal;
	this->distance = distance;
	normalize();
}


void Plane::set(const Plane& plane)
{
	normal = plane.normal;
	distance = plane.distance;
}


void Plane::transform(const Matrix& matrix)
{
	Matrix inverted;
	if (matrix.invert(&inverted))
	{
		// Treat the plane as a four-tuple and multiply by the inverse transpose of the matrix to get the transformed plane.
		// Then we normalize the plane by dividing both the normal and the distance by the length of the normal.
		auto nx = normal.x * inverted.m[0] + normal.y * inverted.m[1] + normal.z * inverted.m[2] + distance * inverted.m[3];
		auto ny = normal.x * inverted.m[4] + normal.y * inverted.m[5] + normal.z * inverted.m[6] + distance * inverted.m[7];
		auto nz = normal.x * inverted.m[8] + normal.y * inverted.m[9] + normal.z * inverted.m[10] + distance * inverted.m[11];
		auto d = normal.x * inverted.m[12] + normal.y * inverted.m[13] + normal.z * inverted.m[14] + distance * inverted.m[15];
		auto divisor = sqrt(nx * nx + ny * ny + nz * nz);
		auto factor = 1.0f / divisor;

		normal.x = nx * factor;
		normal.y = ny * factor;
		normal.z = nz * factor;
		distance = d * factor;
	}
}


void Plane::normalize()
{
	if (normal.isZero())
		return;

	// Normalize the plane's normal.
	auto normalizeFactor = 1.0f / sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

	if (normalizeFactor != 1.0f)
	{
		normal.x *= normalizeFactor;
		normal.y *= normalizeFactor;
		normal.z *= normalizeFactor;
		distance *= normalizeFactor;
	}
}
