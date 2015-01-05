#pragma once

#include "SoloVector3.h"

namespace solo
{
	class BoundingSphere;
	class Frustum;
	class Plane;
	class Ray;

	class BoundingBox
	{
	public:
		Vector3 min;
		Vector3 max;

		BoundingBox();
		BoundingBox(const Vector3& min, const Vector3& max);
		BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		BoundingBox(const BoundingBox& copy);

		~BoundingBox();

		static const BoundingBox& empty();

		Vector3 getCenter() const;
		void getCenter(Vector3* dst) const;

		void getCorners(Vector3* dst) const;

		bool intersects(const BoundingBox& box) const;
		bool intersects(const BoundingSphere& sphere) const;
		bool intersects(const Frustum& frustum) const;
		float intersects(const Plane& plane) const;
		float intersects(const Ray& ray) const;

		bool isEmpty() const;

		void merge(const BoundingSphere& sphere);
		void merge(const BoundingBox& box);

		void set(const Vector3& min, const Vector3& max);
		void set(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		void set(const BoundingBox& box);
		void set(const BoundingSphere& sphere);
		
		void transform(const Matrix& matrix);

		inline BoundingBox& operator*=(const Matrix& matrix);
	};

	inline BoundingBox& BoundingBox::operator*=(const Matrix& matrix)
	{
		transform(matrix);
		return *this;
	}

	inline BoundingBox operator*(const Matrix& matrix, const BoundingBox& box)
	{
		BoundingBox b(box);
		b.transform(matrix);
		return b;
	}
}
