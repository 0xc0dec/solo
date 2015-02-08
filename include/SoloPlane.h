#pragma once

#include "SoloVector3.h"

namespace solo
{
	class BoundingSphere;
	class BoundingBox;
	class Frustum;
	class Ray;

	class Plane
	{
	public:
		static const int INTERSECTS_INTERSECTING = 0;
		static const int INTERSECTS_FRONT = 1;
		static const int INTERSECTS_BACK = -1;

		Plane();
		Plane(const Vector3& normal, float distance);
		Plane(float normalX, float normalY, float normalZ, float distance);
		
		const Vector3& getNormal() const;
		void setNormal(const Vector3& normal);
		void setNormal(float x, float y, float z);

		float getDistance() const;
		void setDistance(float distance);

		float getDistance(const Vector3& point) const;

		static void intersection(const Plane& p1, const Plane& p2, const Plane& p3, Vector3* point);

		float intersects(const BoundingSphere& sphere) const;
		float intersects(const BoundingBox& box) const;
		float intersects(const Frustum& frustum) const;
		float intersects(const Plane& plane) const;
		float intersects(const Ray& ray) const;

		bool isParallel(const Plane& plane) const;

		void set(const Vector3& normal, float distance);
		void set(const Plane& plane);

		void transform(const Matrix& matrix);

		inline Plane& operator*=(const Matrix& matrix);

	private:
		void normalize();

		Vector3 normal;
		float distance;
	};

	inline Plane& Plane::operator*=(const Matrix& matrix)
	{
		transform(matrix);
		return *this;
	}

	inline Plane operator*(const Matrix& matrix, const Plane& plane)
	{
		auto p(plane);
		p.transform(matrix);
		return p;
	}
}
