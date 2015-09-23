#pragma once

#include "SoloPlane.h"
#include "SoloMatrix.h"

namespace solo
{
	struct Vector3;
	class BoundingSphere;
	class BoundingBox;
	struct Ray;

	class Frustum
	{
	public:
		Frustum();

		const Plane& getNear() const;
		const Plane& getFar() const;
		const Plane& getLeft() const;
		const Plane& getRight() const;
		const Plane& getBottom() const;
		const Plane& getTop() const;

		Matrix getMatrix() const;

		void getCorners(Vector3* corners) const;
		void getNearCorners(Vector3* corners) const;
		void getFarCorners(Vector3* corners) const;

		bool intersects(const Vector3& point) const;
		bool intersects(float x, float y, float z) const;
		bool intersects(const BoundingSphere& sphere) const;
		bool intersects(const BoundingBox& box) const;
		PlaneIntersection getIntersection(const Plane& plane) const;
		float intersects(const Ray& ray) const;

		void set(const Frustum& frustum);
		void set(const Matrix& matrix);

	private:
		void updatePlanes();

		Plane near;
		Plane far;
		Plane bottom;
		Plane top;
		Plane left;
		Plane right;
		Matrix matrix;
	};
}
