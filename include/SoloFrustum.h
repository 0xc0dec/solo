#pragma once

#include <vector>
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
		void setMatrix(const Matrix& m);

		std::vector<Vector3> getCorners() const;
		std::vector<Vector3> getNearCorners() const;
		std::vector<Vector3> getFarCorners() const;

		bool intersects(const Vector3& point) const;
		bool intersects(const BoundingSphere& sphere) const;
		bool intersects(const BoundingBox& box) const;
		float intersects(const Ray& ray) const;
		PlaneIntersection getIntersection(const Plane& plane) const;

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
