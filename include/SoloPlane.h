#pragma once

#include "SoloVector3.h"

namespace solo
{
	class BoundingSphere;
	class BoundingBox;
	class Frustum;
	struct Ray;
	struct Matrix;

	enum class PlaneIntersection
	{
		Intersecting = 0,
		Front = 1,
		Back = -1
	};

	struct Plane
	{
		Plane() {}
		Plane(const Vector3& normal, float distance);
		Plane(float normalX, float normalY, float normalZ, float distance);
		
		const Vector3& getNormal() const;
		void setNormal(const Vector3& normal);

		float getDistance() const;
		void setDistance(float distance);

		float getDistanceToPoint(const Vector3& point) const;

		static Vector3 intersection(const Plane& p1, const Plane& p2, const Plane& p3);

		PlaneIntersection getIntersection(const BoundingSphere &sphere) const;
		PlaneIntersection getIntersection(const BoundingBox &box) const;
		PlaneIntersection getIntersection(const Frustum &frustum) const;
		PlaneIntersection getIntersection(const Plane &plane) const;
		PlaneIntersection getIntersection(const Ray &ray) const;

		bool isParallel(const Plane& plane) const;

		void transform(const Matrix& matrix);

		inline Plane& operator*=(const Matrix& matrix);

	private:
		void normalize();

		Vector3 normal { 0, 1, 0 };
		float distance = 0;
	};

	inline Plane& Plane::operator*=(const Matrix& matrix)
	{
		transform(matrix);
		return *this;
	}

	inline Plane operator*(const Matrix& matrix, const Plane& plane)
	{
		auto p(const_cast<Plane&>(plane));
		p.transform(matrix);
		return p;
	}
}
