#include "SoloFrustum.h"
#include "SoloBoundingSphere.h"
#include "SoloBoundingBox.h"
#include "SoloRay.h"

using namespace solo;


Frustum::Frustum()
{
	setMatrix(Matrix::identity());
}


const Plane& Frustum::getNearPlane() const
{
	return near;
}


const Plane& Frustum::getFarPlane() const
{
	return far;
}


const Plane& Frustum::getLeftPlane() const
{
	return left;
}


const Plane& Frustum::getRightPlane() const
{
	return right;
}


const Plane& Frustum::getBottomPlane() const
{
	return bottom;
}


const Plane& Frustum::getTopPlane() const
{
	return top;
}


void Frustum::setMatrix(const Matrix& m)
{
	this->matrix = m;
	updatePlanes();
}


Matrix Frustum::getMatrix() const
{
	return matrix;
}


std::vector<Vector3> Frustum::getCorners() const
{
	auto near = getNearCorners();
	auto far = getFarCorners();
	near.insert(near.end(), far.begin(), far.end());
	return near;
}


std::vector<Vector3> Frustum::getNearCorners() const
{
	std::vector<Vector3> result;
	result.emplace_back(Plane::intersection(near, left, top));
	result.emplace_back(Plane::intersection(near, left, bottom));
	result.emplace_back(Plane::intersection(near, right, bottom));
	result.emplace_back(Plane::intersection(near, right, top));
	return result;
}


std::vector<Vector3> Frustum::getFarCorners() const
{
	std::vector<Vector3> result;
	result.emplace_back(Plane::intersection(far, right, top));
	result.emplace_back(Plane::intersection(far, right, bottom));
	result.emplace_back(Plane::intersection(far, left, bottom));
	result.emplace_back(Plane::intersection(far, left, top));
	return result;
}


bool Frustum::intersectsPoint(const Vector3& point) const
{
	if (near.getDistance(point) <= 0)
		return false;
	if (far.getDistance(point) <= 0)
		return false;
	if (left.getDistance(point) <= 0)
		return false;
	if (right.getDistance(point) <= 0)
		return false;
	if (top.getDistance(point) <= 0)
		return false;
	if (bottom.getDistance(point) <= 0)
		return false;

	return true;
}


bool Frustum::intersectsBoundingSphere(const BoundingSphere& sphere) const
{
	return sphere.intersects(*this);
}


bool Frustum::intersectsBoundingBox(const BoundingBox& box) const
{
	return box.intersects(*this);
}


PlaneIntersection Frustum::getPlaneIntersection(const Plane& plane) const
{
	return plane.getIntersection(*this);
}


float Frustum::intersectsRay(const Ray& ray) const
{
	return ray.getIntersection(*this);
}


void Frustum::updatePlanes()
{
	near = Plane(Vector3(matrix.m[3] + matrix.m[2], matrix.m[7] + matrix.m[6], matrix.m[11] + matrix.m[10]), matrix.m[15] + matrix.m[14]);
	far = Plane(Vector3(matrix.m[3] - matrix.m[2], matrix.m[7] - matrix.m[6], matrix.m[11] - matrix.m[10]), matrix.m[15] - matrix.m[14]);
	bottom = Plane(Vector3(matrix.m[3] + matrix.m[1], matrix.m[7] + matrix.m[5], matrix.m[11] + matrix.m[9]), matrix.m[15] + matrix.m[13]);
	top = Plane(Vector3(matrix.m[3] - matrix.m[1], matrix.m[7] - matrix.m[5], matrix.m[11] - matrix.m[9]), matrix.m[15] - matrix.m[13]);
	left = Plane(Vector3(matrix.m[3] + matrix.m[0], matrix.m[7] + matrix.m[4], matrix.m[11] + matrix.m[8]), matrix.m[15] + matrix.m[12]);
	right = Plane(Vector3(matrix.m[3] - matrix.m[0], matrix.m[7] - matrix.m[4], matrix.m[11] - matrix.m[8]), matrix.m[15] - matrix.m[12]);
}
