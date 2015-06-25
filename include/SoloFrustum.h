#pragma once

#include "SoloPlane.h"
#include "SoloMatrix.h"

namespace solo
{
	class Vector3;
	class BoundingSphere;
	class BoundingBox;
	class Ray;

	class Frustum
	{
	public:
		Frustum();
		Frustum(const Matrix& matrix);

		const Plane& getNear() const;
		const Plane& getFar() const;
		const Plane& getLeft() const;
		const Plane& getRight() const;
		const Plane& getBottom() const;
		const Plane& getTop() const;

		void getMatrix(Matrix* dst) const;

		void getCorners(Vector3* corners) const;
		void getNearCorners(Vector3* corners) const;
		void getFarCorners(Vector3* corners) const;

		bool intersects(const Vector3& point) const;
		bool intersects(float x, float y, float z) const;
		bool intersects(const BoundingSphere& sphere) const;
		bool intersects(const BoundingBox& box) const;
		PlaneIntersection intersects(const Plane& plane) const;
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
