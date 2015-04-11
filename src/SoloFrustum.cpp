#include "SoloFrustum.h"
#include "SoloBoundingSphere.h"
#include "SoloBoundingBox.h"
#include "SoloRay.h"

using namespace solo;


Frustum::Frustum()
{
	set(Matrix::identity());
}


Frustum::Frustum(const Matrix& matrix)
{
	set(matrix);
}


const Plane& Frustum::getNear() const
{
	return near;
}


const Plane& Frustum::getFar() const
{
	return far;
}


const Plane& Frustum::getLeft() const
{
	return left;
}


const Plane& Frustum::getRight() const
{
	return right;
}


const Plane& Frustum::getBottom() const
{
	return bottom;
}


const Plane& Frustum::getTop() const
{
	return top;
}


void Frustum::getMatrix(Matrix* dst) const
{
	dst->set(matrix);
}


void Frustum::getCorners(Vector3* corners) const
{
	getNearCorners(corners);
	getFarCorners(corners + 4);
}


void Frustum::getNearCorners(Vector3* corners) const
{
	Plane::intersection(near, left, top, &corners[0]);
	Plane::intersection(near, left, bottom, &corners[1]);
	Plane::intersection(near, right, bottom, &corners[2]);
	Plane::intersection(near, right, top, &corners[3]);
}


void Frustum::getFarCorners(Vector3* corners) const
{
	Plane::intersection(far, right, top, &corners[0]);
	Plane::intersection(far, right, bottom, &corners[1]);
	Plane::intersection(far, left, bottom, &corners[2]);
	Plane::intersection(far, left, top, &corners[3]);
}


bool Frustum::intersects(const Vector3& point) const
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


bool Frustum::intersects(float x, float y, float z) const
{
	return intersects(Vector3(x, y, z));
}


bool Frustum::intersects(const BoundingSphere& sphere) const
{
	return sphere.intersects(*this);
}


bool Frustum::intersects(const BoundingBox& box) const
{
	return box.intersects(*this);
}


Plane::PlaneIntersection Frustum::intersects(const Plane& plane) const
{
	return plane.getIntersection(*this);
}


float Frustum::intersects(const Ray& ray) const
{
	return ray.intersects(*this);
}


void Frustum::set(const Frustum& frustum)
{
	near = frustum.near;
	far = frustum.far;
	bottom = frustum.bottom;
	top = frustum.top;
	left = frustum.left;
	right = frustum.right;
	matrix.set(frustum.matrix);
}


void Frustum::updatePlanes()
{
	near.set(Vector3(matrix.m[3] + matrix.m[2], matrix.m[7] + matrix.m[6], matrix.m[11] + matrix.m[10]), matrix.m[15] + matrix.m[14]);
	far.set(Vector3(matrix.m[3] - matrix.m[2], matrix.m[7] - matrix.m[6], matrix.m[11] - matrix.m[10]), matrix.m[15] - matrix.m[14]);
	bottom.set(Vector3(matrix.m[3] + matrix.m[1], matrix.m[7] + matrix.m[5], matrix.m[11] + matrix.m[9]), matrix.m[15] + matrix.m[13]);
	top.set(Vector3(matrix.m[3] - matrix.m[1], matrix.m[7] - matrix.m[5], matrix.m[11] - matrix.m[9]), matrix.m[15] - matrix.m[13]);
	left.set(Vector3(matrix.m[3] + matrix.m[0], matrix.m[7] + matrix.m[4], matrix.m[11] + matrix.m[8]), matrix.m[15] + matrix.m[12]);
	right.set(Vector3(matrix.m[3] - matrix.m[0], matrix.m[7] - matrix.m[4], matrix.m[11] - matrix.m[8]), matrix.m[15] - matrix.m[12]);
}


void Frustum::set(const Matrix& matrix)
{
	this->matrix.set(matrix);
	updatePlanes();
}