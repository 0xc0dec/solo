#ifndef __SOLO_MATRIX3_H__
#define __SOLO_MATRIX3_H__

#include "SoloCommon.h"
#include "SoloMath.h"

namespace solo
{
	class Matrix3
	{
	public:
		inline Matrix3()
		{
		}

		inline explicit Matrix3(const f32 arr[3][3])
		{
			memcpy(m, arr, 9 * sizeof(f32));
		}

		inline Matrix3(const Matrix3& other)
		{
			memcpy(m, other.m, 9 * sizeof(f32));
		}

		Matrix3(f32 el00, f32 el01, f32 el02,
				f32 el10, f32 el11, f32 el12,
				f32 el20, f32 el21, f32 el22)
		{
			m[0][0] = el00;
			m[0][1] = el01;
			m[0][2] = el02;
			m[1][0] = el10;
			m[1][1] = el11;
			m[1][2] = el12;
			m[2][0] = el20;
			m[2][1] = el21;
			m[2][2] = el22;
		}

		inline void swap(Matrix3& other)
		{
			std::swap(m[0][0], other.m[0][0]);
			std::swap(m[0][1], other.m[0][1]);
			std::swap(m[0][2], other.m[0][2]);
			std::swap(m[1][0], other.m[1][0]);
			std::swap(m[1][1], other.m[1][1]);
			std::swap(m[1][2], other.m[1][2]);
			std::swap(m[2][0], other.m[2][0]);
			std::swap(m[2][1], other.m[2][1]);
			std::swap(m[2][2], other.m[2][2]);
		}

		inline f32* operator[](size_t i) const
		{
			return const_cast<f32*>(m[i]);
		}

		Vector3 column(size_t i) const;
		void setColumn(size_t i, const Vector3& vector);
		void fromAxes(const Vector3& x, const Vector3& y, const Vector3& z);

		inline Matrix3& operator=(const Matrix3& other)
		{
			memcpy(m, other.m, 9 * sizeof(f32));
			return *this;
		}

		bool operator==(const Matrix3& other) const;

		inline bool operator!=(const Matrix3& other) const
		{
			return !operator==(other);
		}

		Matrix3 operator+(const Matrix3& other) const;
		Matrix3 operator-(const Matrix3& other) const;
		Matrix3 operator*(const Matrix3& other) const;
		Matrix3 operator-() const;

		Vector3 operator*(const Vector3& vector) const;
		Matrix3 operator*(f32 scalar) const;

		friend Vector3 operator*(const Vector3& vector, const Matrix3& matrix);
		friend Matrix3 operator*(f32 scalar, const Matrix3& matrix);

		Matrix3 transpose() const;
		bool inverse(Matrix3& inverse, f32 tolerance = 1e-06) const;
		Matrix3 inverse(f32 tolerance = 1e-06) const;
		f32 determinant() const;

		void singularValueDecomposition(Matrix3& l, Vector3& s, Matrix3& r) const;
		void singularValueComposition(const Matrix3& l, const Vector3& s, const Matrix3& r);

		/// Gram-Schmidt orthonormalization (applied to columns of rotation matrix)
		void orthonormalize();

		/// Orthogonal Q, diagonal D, upper triangular U stored as (u01,u02,u12)
		void qduDecomposition(Matrix3& q, Vector3& d, Vector3& u) const;

		f32 spectralNorm() const;

		// matrix must be orthonormal
		void toAngleAxis(Vector3& axis, Radian& angle) const;

		inline void toAngleAxis(Vector3& axis, Degree& angle) const
		{
			Radian r;
			toAngleAxis(axis, r);
			angle = r;
		}

		void fromAngleAxis(const Vector3& axis, const Radian& angle);

		// The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
		// where yaw is rotation about the Up vector, pitch is rotation about the
		// Right axis, and roll is rotation about the Direction axis.
		bool toEulerAnglesXYZ(Radian& yawAngle, Radian& pitchAngle, Radian& rollAngle) const;
		bool toEulerAnglesXZY(Radian& yawAngle, Radian& pitchAngle, Radian& rollAngle) const;
		bool toEulerAnglesYXZ(Radian& yawAngle, Radian& pitchAngle, Radian& rollAngle) const;
		bool toEulerAnglesYZX(Radian& yawAngle, Radian& pitchAngle, Radian& rollAngle) const;
		bool toEulerAnglesZXY(Radian& yawAngle, Radian& pitchAngle, Radian& rollAngle) const;
		bool toEulerAnglesZYX(Radian& yawAngle, Radian& pitchAngle, Radian& rollAngle) const;
		void fromEulerAnglesXYZ(const Radian& yawAngle, const Radian& pitchAngle, const Radian& rollAngle);
		void fromEulerAnglesXZY(const Radian& yawAngle, const Radian& pitchAngle, const Radian& rollAngle);
		void fromEulerAnglesYXZ(const Radian& yawAngle, const Radian& pitchAngle, const Radian& rollAngle);
		void fromEulerAnglesYZX(const Radian& yawAngle, const Radian& pitchAngle, const Radian& rollAngle);
		void fromEulerAnglesZXY(const Radian& yawAngle, const Radian& pitchAngle, const Radian& rollAngle);
		void fromEulerAnglesZYX(const Radian& yawAngle, const Radian& pitchAngle, const Radian& rollAngle);
		/// Eigensolver, matrix must be symmetric
		void eigenSolveSymmetric(f32 eigenValue[3], Vector3 eigenVector[3]) const;

		static void tensorProduct(const Vector3& u, const Vector3& v, Matrix3& product);

		inline bool hasScale() const
		{
			// check magnitude of column vectors (==local axes)
			f32 t = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
			if (!Math::f32Equal(t, 1.0, static_cast<f32>(1e-04)))
				return true;
			t = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1];
			if (!Math::f32Equal(t, 1.0, static_cast<f32>(1e-04)))
				return true;
			t = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];
			if (!Math::f32Equal(t, 1.0, static_cast<f32>(1e-04)))
				return true;

			return false;
		}

		inline friend std::ostream& operator <<(std::ostream& o, const Matrix3& mat)
		{
			o << "Matrix3(" << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", "
				<< mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", "
				<< mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ")";
			return o;
		}

		static const f32 EPSILON;
		static const Matrix3 ZERO;
		static const Matrix3 IDENTITY;

	protected:
		// support for eigensolver
		void tridiagonal(f32 diag[3], f32 subDiag[3]);
		bool qlAlgorithm(f32 diag[3], f32 subDiag[3]);

		// support for singular value decomposition
		static const f32 _svdEpsilon;
		static const unsigned int _svdMaxIterations;
		static void _bidiagonalize(Matrix3& a, Matrix3& l, Matrix3& r);
		static void _golubKahanStep(Matrix3& a, Matrix3& l, Matrix3& r);

		// support for spectral norm
		static f32 _maxCubicRoot(f32 coeff[3]);

		f32 m[3][3];

		// for faster access
		friend class Matrix4;
	};
}

#endif