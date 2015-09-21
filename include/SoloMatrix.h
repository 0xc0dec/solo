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
		explicit Matrix(const float* m);
		Matrix(const Matrix& copy);

		~Matrix();

		static const Matrix& identity();
		static const Matrix& zero();

		bool isIdentity() const;

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

		bool decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;

		float getDeterminant() const;

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
		void negate();

		void rotate(const Quaternion& q);
		void rotate(const Vector3& axis, float angleRadians);
		void rotateX(float angleRadians);
		void rotateY(float angleRadians);
		void rotateZ(float angleRadians);

		void scale(float value);
		void scale(float xScale, float yScale, float zScale);
		void scale(const Vector3& s);

		void translate(float x, float y, float z);
		void translate(const Vector3& t);

		void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
				float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

		void set(const float* m);
		void set(const Matrix& m);

		void setIdentity();
		void setZero();

		inline Matrix operator+(float scalar) const;
		inline Matrix operator+(const Matrix& m) const;
		Matrix& operator+=(float scalar);
		Matrix& operator+=(const Matrix& m);

		inline Matrix operator-() const;
		inline Matrix operator-(float scalar) const;
		inline Matrix operator-(const Matrix& m) const;
		Matrix& operator-=(float scalar);
		Matrix& operator-=(const Matrix& m);

		inline Matrix operator*(float scalar) const;
		inline Matrix operator*(const Matrix& m) const;
		Matrix& operator*=(float scalar);
		Matrix& operator*=(const Matrix& m);

		Vector3 transformPoint(const Vector3& point) const;

		Vector3 transformDirection(const Vector3& dir) const;
		Vector4 transformDirection(const Vector4& dir) const;

	private:
		static Matrix createBillboardHelper(const Vector3& objectPosition, const Vector3& cameraPosition,
		                                  const Vector3& cameraUpVector, const Vector3* cameraForwardVector);
	};

	inline Matrix Matrix::operator+(float scalar) const
	{
		auto result(*this);
		result += scalar;
		return result;
	}

	inline Matrix Matrix::operator+(const Matrix& m) const
	{
		auto result(*this);
		result += m;
		return result;
	}

	inline Matrix Matrix::operator-(float scalar) const
	{
		auto result(*this);
		result -= scalar;
		return result;
	}

	inline Matrix Matrix::operator-(const Matrix& m) const
	{
		auto result(*this);
		result -= m;
		return result;
	}

	inline Matrix Matrix::operator-() const
	{
		auto m(*this);
		m.negate();
		return m;
	}

	Matrix Matrix::operator*(float scalar) const
	{
		auto result(*this);
		result *= scalar;
		return result;
	}

	inline Matrix Matrix::operator*(const Matrix& m) const
	{
		auto result(*this);
		return result *= m;
	}

	inline Vector3& operator*=(Vector3& v, const Matrix& m)
	{
		v = m.transformDirection(v);
		return v;
	}

	inline Vector3 operator*(const Matrix& m, const Vector3& v)
	{
		return m.transformDirection(v);
	}

	inline Vector4& operator*=(Vector4& v, const Matrix& m)
	{
		v = m.transformDirection(v);
		return v;
	}

	inline Vector4 operator*(const Matrix& m, const Vector4& v)
	{
		return m.transformDirection(v);
	}
}
