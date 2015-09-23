#include "SoloRay.h"
#include "SoloMath.h"
#include "SoloBoundingSphere.h"
#include "SoloBoundingBox.h"
#include "SoloMatrix.h"
#include "SoloFrustum.h"
#include "SoloException.h"

using namespace solo;


Ray::Ray(const Vector3& origin, const Vector3& direction)
{
	set(origin, direction);
}


Ray::Ray(float originX, float originY, float originZ, float dirX, float dirY, float dirZ)
{
	set(Vector3(originX, originY, originZ), Vector3(dirX, dirY, dirZ));
}


Vector3 Ray::getOrigin() const
{
	return origin;
}


void Ray::setOrigin(const Vector3& origin)
{
	this->origin = origin;
}


void Ray::setOrigin(float x, float y, float z)
{
	origin.x = x;
	origin.y = y;
	origin.z = z;
}


Vector3 Ray::getDirection() const
{
	return direction;
}


void Ray::setDirection(const Vector3& direction)
{
	this->direction = direction;
	normalize();
}


void Ray::setDirection(float x, float y, float z)
{
	direction.x = x;
	direction.y = y;
	direction.z = z;
	normalize();
}


float Ray::getIntersection(const BoundingSphere &sphere) const
{
	return sphere.getRayIntersection(*this);
}


float Ray::getIntersection(const BoundingBox &box) const
{
	return box.getRayIntersection(*this);
}


float Ray::getIntersection(const Frustum &frustum) const
{
	auto n = frustum.getNearPlane();
	auto nD = getIntersection(n);
	auto nOD = n.getDistanceToPoint(origin);

	auto f = frustum.getFarPlane();
	auto fD = getIntersection(f);
	auto fOD = f.getDistanceToPoint(origin);

	auto l = frustum.getLeftPlane();
	auto lD = getIntersection(l);
	auto lOD = l.getDistanceToPoint(origin);

	auto r = frustum.getRightPlane();
	auto rD = getIntersection(r);
	auto rOD = r.getDistanceToPoint(origin);

	auto b = frustum.getBottomPlane();
	auto bD = getIntersection(b);
	auto bOD = b.getDistanceToPoint(origin);

	auto t = frustum.getTopPlane();
	auto tD = getIntersection(t);
	auto tOD = t.getDistanceToPoint(origin);

	// If the ray's origin is in the negative half-space of one of the frustum's planes
	// and it does not intersect that same plane, then it does not intersect the frustum.
	if ((nOD < 0.0f && nD < 0.0f) || (fOD < 0.0f && fD < 0.0f) ||
		(lOD < 0.0f && lD < 0.0f) || (rOD < 0.0f && rD < 0.0f) ||
		(bOD < 0.0f && bD < 0.0f) || (tOD < 0.0f && tD < 0.0f))
	{
		return static_cast<float>(RayIntersection::None);
	}

	// Otherwise, the intersection distance is the minimum positive intersection distance.
	auto d = (nD > 0.0f) ? nD : 0.0f;
	d = (fD > 0.0f) ? ((d == 0.0f) ? fD : std::min(fD, d)) : d;
	d = (lD > 0.0f) ? ((d == 0.0f) ? lD : std::min(lD, d)) : d;
	d = (rD > 0.0f) ? ((d == 0.0f) ? rD : std::min(rD, d)) : d;
	d = (tD > 0.0f) ? ((d == 0.0f) ? bD : std::min(bD, d)) : d;
	d = (bD > 0.0f) ? ((d == 0.0f) ? tD : std::min(tD, d)) : d;

	return d;
}


float Ray::getIntersection(const Plane &plane) const
{
	const auto& normal = plane.getNormal();
	// If the origin of the ray is on the plane then the distance is zero.
	auto alpha = (normal.dot(origin) + plane.getDistance());
	if (fabs(alpha) < MATH_EPSILON)
		return 0.0f;

	auto dot = normal.dot(direction);

	// If the dot product of the plane's normal and this ray's direction is zero,
	// then the ray is parallel to the plane and does not intersect it.
	if (dot == 0.0f)
		return static_cast<float>(RayIntersection::None);

	// Calculate the distance along the ray's direction vector to the point where
	// the ray getIntersection the plane (if it is negative the plane is behind the ray).
	auto d = -alpha / dot;
	if (d < 0.0f)
		return static_cast<float>(RayIntersection::None);
	
	return d;
}


void Ray::set(const Vector3& origin, const Vector3& direction)
{
	this->origin = origin;
	this->direction = direction;
	normalize();
}


void Ray::set(const Ray& ray)
{
	origin = ray.origin;
	direction = ray.direction;
	normalize();
}


void Ray::transform(const Matrix& matrix)
{
	origin = matrix.transformPoint(origin);
	direction = matrix.transformDirection(direction);
	direction.normalize();
}


void Ray::normalize()
{
	if (direction.isZero())
		THROW_FMT(EngineException, "Invalid ray object: the ray's direction must be non-zero");

	// Normalize the ray's direction vector
	auto normalizeFactor = 1.0f / sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	if (normalizeFactor != 1.0f)
	{
		direction.x *= normalizeFactor;
		direction.y *= normalizeFactor;
		direction.z *= normalizeFactor;
	}
}