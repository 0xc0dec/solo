#pragma once

#include "SoloVector3.h"
#include "SoloPlane.h"

namespace solo
{
	class BoundingSphere
	{
	public:
		Vector3 center;
		float radius = 0;

		BoundingSphere() {}
		BoundingSphere(const Vector3& center, float radius);

		static const BoundingSphere& empty();

		bool intersects(const BoundingSphere& sphere) const;
		bool intersects(const BoundingBox& box) const;
		bool intersects(const Frustum& frustum) const;
		PlaneIntersection getPlaneIntersection(const Plane &plane) const;
		float getRayIntersection(const Ray& ray) const;

		bool isEmpty() const;

		void merge(const BoundingSphere& sphere);
		void merge(const BoundingBox& box);

		void transform(const Matrix& matrix);

		inline BoundingSphere& operator*=(const Matrix& matrix);
	};

	inline BoundingSphere& BoundingSphere::operator*=(const Matrix& matrix)
	{
		transform(matrix);
		return *this;
	}

	inline BoundingSphere operator*(const Matrix& matrix, const BoundingSphere& sphere)
	{
		auto s(const_cast<BoundingSphere&>(sphere));
		s.transform(matrix);
		return s;
	}
}
