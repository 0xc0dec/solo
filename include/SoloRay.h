#ifndef __SOLO_RAY_H__
#define __SOLO_RAY_H__

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
		/**
		* Represents when a 3D entity does not intersect a ray.
		*/
		static const int INTERSECTS_NONE = -1;

		Ray();
		Ray(const Vector3& origin, const Vector3& direction);
		Ray(float originX, float originY, float originZ, float dirX, float dirY, float dirZ);
		Ray(const Ray& copy);

		~Ray();

		const Vector3& getOrigin() const;
		void setOrigin(const Vector3& origin);
		void setOrigin(float x, float y, float z);

		const Vector3& getDirection() const;
		void setDirection(const Vector3& direction);
		void setDirection(float x, float y, float z);

		float intersects(const BoundingSphere& sphere) const;
		float intersects(const BoundingBox& box) const;
		float intersects(const Frustum& frustum) const;
		float intersects(const Plane& plane) const;
		
		void set(const Vector3& origin, const Vector3& direction);
		void set(const Ray& ray);

		void transform(const Matrix& matrix);
		
		inline Ray& operator*=(const Matrix& matrix);

	private:
		void normalize();

		Vector3 _origin;
		Vector3 _direction;
	};

	inline Ray& Ray::operator*=(const Matrix& matrix)
	{
		transform(matrix);
		return *this;
	}

	inline Ray operator*(const Matrix& matrix, const Ray& ray)
	{
		Ray r(ray);
		r.transform(matrix);
		return r;
	}
}

#endif