#pragma once

#include "SoloVector3.h"
#include "SoloVector4.h"

#define MATRIX_SIZE (sizeof(float) * 16)

namespace solo
{
	struct Plane;
	struct Quaternion;

	struct Matrix
	{
		// column-major order
		float m[16];

		Matrix();
		Matrix(float m11, float m12, float m13, float m14,
			   float m21, float m22, float m23, float m24,
		       float m31, float m32, float m33, float m34,
			   float m41, float m42, float m43, float m44);
		Matrix(const float* m);
		Matrix(const Matrix& copy);

		~Matrix();

		static const Matrix& identity();
		static const Matrix& zero();

		static Matrix createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up);
		static Matrix createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
				float targetCenterX, float targetCenterY, float targetCenterZ,
				float upX, float upY, float upZ);

		static Matrix createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane);
		static Matrix createOrthographic(float width, float height, float zNearPlane, float zFarPlane);
		static Matrix createOrthographicOffCenter(float left, float right, float bottom, float top, float near, float far);
		static Matrix createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraUpVector);
		static Matrix createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraUpVector,
				const Vector3& cameraForwardVector);

		static Matrix createReflection(const Plane& plane);
		static Matrix createScale(const Vector3& scale);
		static Matrix createScale(float xScale, float yScale, float zScale);
		static Matrix createRotation(const Quaternion& quat);
		static Matrix createRotation(const Vector3& axis, float angleRadians);
		static Matrix createRotationX(float angleRadians);
		static Matrix createRotationY(float angleRadians);
		static Matrix createRotationZ(float angleRadians);
		static Matrix createTranslation(const Vector3& translation);
		static Matrix createTranslation(float xTranslation, float yTranslation, float zTranslation);

		void add(float scalar);
		void add(float scalar, Matrix* dst);
		void add(const Matrix& m);
		static void add(const Matrix& m1, const Matrix& m2, Matrix* dst);

		bool decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;

		float determinant() const;

		Vector3 getScale() const;
		Quaternion getRotation() const;
		Vector3 getTranslation() const;

		Vector3 getUpVector() const;
		Vector3 getDownVector() const;
		Vector3 getLeftVector() const;
		Vector3 getRightVector() const;
		Vector3 getForwardVector() const;
		Vector3 getBackVector() const;

		bool invert();

		void transpose();
		void transpose(Matrix* dst) const;

		bool isIdentity() const;

		static void multiply(const Matrix& m, float scalar, Matrix* dst);
		static void multiply(const Matrix& m1, const Matrix& m2, Matrix* dst);

		void negate();
		void negate(Matrix* dst) const;

		void rotate(const Quaternion& q);
		void rotate(const Quaternion& q, Matrix* dst) const;
		void rotate(const Vector3& axis, float angleRadians);
		void rotate(const Vector3& axis, float angleRadians, Matrix* dst) const;
		void rotateX(float angleRadians);
		void rotateX(float angleRadians, Matrix* dst) const;
		void rotateY(float angleRadians);
		void rotateY(float angleRadians, Matrix* dst) const;
		void rotateZ(float angleRadians);
		void rotateZ(float angleRadians, Matrix* dst) const;

		void scale(float value);
		void scale(float value, Matrix* dst) const;
		void scale(float xScale, float yScale, float zScale);
		void scale(float xScale, float yScale, float zScale, Matrix* dst) const;
		void scale(const Vector3& s);
		void scale(const Vector3& s, Matrix* dst) const;

		void translate(float x, float y, float z);
		void translate(float x, float y, float z, Matrix* dst) const;
		void translate(const Vector3& t);
		void translate(const Vector3& t, Matrix* dst) const;

		void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
		         float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

		void set(const float* m);
		void set(const Matrix& m);

		void setIdentity();
		void setZero();

		void subtract(const Matrix& m);
		static void subtract(const Matrix& m1, const Matrix& m2, Matrix* dst);

		inline Matrix operator+(const Matrix& m) const;
		inline Matrix& operator+=(const Matrix& m);

		inline Matrix operator-() const;
		inline Matrix operator-(const Matrix& m) const;
		inline Matrix& operator-=(const Matrix& m);

		inline Matrix operator*(float scalar) const;
		inline Matrix operator*(const Matrix& m) const;
		inline Matrix& operator*=(float scalar);
		inline Matrix& operator*=(const Matrix& m);

		void transformPoint(Vector3* point) const;
		void transformPoint(const Vector3& point, Vector3* dst) const;
		Vector3 transformPoint(const Vector3& point) const;

		void transformDirection(Vector3* dir) const;
		void transformDirection(const Vector3& dir, Vector3* dst) const;
		void transformDirection(float x, float y, float z, float w, Vector3* dst) const;
		void transformDirection(Vector4* dir) const;
		void transformDirection(const Vector4& dir, Vector4* dst) const;
		Vector3 transformDirection(const Vector3& direction) const;

	private:
		static Matrix createBillboardHelper(const Vector3& objectPosition, const Vector3& cameraPosition,
		                                  const Vector3& cameraUpVector, const Vector3* cameraForwardVector);
	};

	inline Matrix Matrix::operator+(const Matrix& m) const
	{
		auto result(*this);
		result.add(m);
		return result;
	}

	inline Matrix& Matrix::operator+=(const Matrix& m)
	{
		add(m);
		return *this;
	}

	inline Matrix Matrix::operator-(const Matrix& m) const
	{
		auto result(*this);
		result.subtract(m);
		return result;
	}

	inline Matrix& Matrix::operator-=(const Matrix& m)
	{
		subtract(m);
		return *this;
	}

	inline Matrix Matrix::operator-() const
	{
		auto m(*this);
		m.negate();
		return m;
	}

	Matrix Matrix::operator*(float scalar) const
	{
		Matrix result;
		Matrix::multiply(*this, scalar, &result);
		return result;
	}

	inline Matrix Matrix::operator*(const Matrix& m) const
	{
		auto result(*this);
		return result *= m;
	}

	Matrix& Matrix::operator*=(float scalar)
	{
		Matrix::multiply(*this, scalar, this);
		return *this;
	}

	inline Matrix& Matrix::operator*=(const Matrix& m)
	{
		multiply(*this, m, this);
		return *this;
	}

	inline Vector3& operator*=(Vector3& v, const Matrix& m)
	{
		m.transformDirection(&v);
		return v;
	}

	inline Vector3 operator*(const Matrix& m, const Vector3& v)
	{
		Vector3 x;
		m.transformDirection(v, &x);
		return x;
	}

	inline Vector4& operator*=(Vector4& v, const Matrix& m)
	{
		m.transformDirection(&v);
		return v;
	}

	inline Vector4 operator*(const Matrix& m, const Vector4& v)
	{
		Vector4 x;
		m.transformDirection(v, &x);
		return x;
	}
}
