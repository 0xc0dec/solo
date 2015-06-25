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
		PlaneIntersection getIntersection(const Plane &plane) const;
		float intersects(const Ray& ray) const;

		bool isEmpty() const;

		void merge(const BoundingSphere& sphere);
		void merge(const BoundingBox& box);

		void set(const Vector3& center, float radius);
		void set(const BoundingSphere& sphere);
		void set(const BoundingBox& box);

		void transform(const Matrix& matrix);

		inline BoundingSphere& operator*=(const Matrix& matrix);

	private:
		float distance(const BoundingSphere& sphere, const Vector3&);
		bool contains(const BoundingSphere& sphere, Vector3* points, unsigned count);
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
