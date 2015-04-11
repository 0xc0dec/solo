#pragma once

#include "SoloVector3.h"

namespace solo
{
	class Frustum;
	class Plane;
	class BoundingSphere;
	class BoundingBox;

	class Ray
	{
	public:
		enum class RayIntersection
		{
			None = -1
		};

		Ray() {}
		Ray(const Vector3& origin, const Vector3& direction);
		Ray(float originX, float originY, float originZ, float dirX, float dirY, float dirZ);

		const Vector3& getOrigin() const;
		void setOrigin(const Vector3& origin);
		void setOrigin(float x, float y, float z);

		const Vector3& getDirection() const;
		void setDirection(const Vector3& direction);
		void setDirection(float x, float y, float z);

		float getIntersection(const BoundingSphere &sphere) const;
		float getIntersection(const BoundingBox &box) const;
		float getIntersection(const Frustum &frustum) const;
		float getIntersection(const Plane &plane) const;
		
		void set(const Vector3& origin, const Vector3& direction);
		void set(const Ray& ray);

		void transform(const Matrix& matrix);
		
		inline Ray& operator*=(const Matrix& matrix);

	private:
		void normalize();

		Vector3 origin = { 0, 0, 0 };
		Vector3 direction = { 0, 0, 1 };
	};

	inline Ray& Ray::operator*=(const Matrix& matrix)
	{
		transform(matrix);
		return *this;
	}

	inline Ray operator*(const Matrix& matrix, const Ray& ray)
	{
		auto r(const_cast<Ray&>(ray));
		r.transform(matrix);
		return r;
	}
}
