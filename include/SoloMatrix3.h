#ifndef __SOLO_MATRIX3_H__
#define __SOLO_MATRIX3_H__

#include "SoloCommon.h"
#include "SoloMath.h"
#include "SoloVector3.h"

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

		inline Matrix3(const Matrix3& rkMatrix)
		{
			memcpy(m, rkMatrix.m, 9 * sizeof(f32));
		}

		Matrix3(f32 fEntry00, f32 fEntry01, f32 fEntry02,
				f32 fEntry10, f32 fEntry11, f32 fEntry12,
				f32 fEntry20, f32 fEntry21, f32 fEntry22)
		{
			m[0][0] = fEntry00;
			m[0][1] = fEntry01;
			m[0][2] = fEntry02;
			m[1][0] = fEntry10;
			m[1][1] = fEntry11;
			m[1][2] = fEntry12;
			m[2][0] = fEntry20;
			m[2][1] = fEntry21;
			m[2][2] = fEntry22;
		}

		/** Exchange the contents of this matrix with another.
		*/
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

		// member access, allows use of construct mat[r][c]
		inline f32* operator[](size_t iRow) const
		{
			return (f32*)m[iRow];
		}

		/*inline operator f32* ()
		{
		return (f32*)m[0];
		}*/
		Vector3 GetColumn(size_t iCol) const;
		void SetColumn(size_t iCol, const Vector3& vec);
		void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

		// assignment and comparison
		inline Matrix3& operator=(const Matrix3& rkMatrix)
		{
			memcpy(m, rkMatrix.m, 9 * sizeof(f32));
			return *this;
		}

		/** Tests 2 matrices for equality.
		*/
		bool operator==(const Matrix3& rkMatrix) const;

		/** Tests 2 matrices for inequality.
		*/
		inline bool operator!=(const Matrix3& rkMatrix) const
		{
			return !operator==(rkMatrix);
		}

		// arithmetic operations
		/** Matrix addition.
		*/
		Matrix3 operator+(const Matrix3& rkMatrix) const;

		/** Matrix subtraction.
		*/
		Matrix3 operator-(const Matrix3& rkMatrix) const;

		/** Matrix concatenation using '*'.
		*/
		Matrix3 operator*(const Matrix3& rkMatrix) const;
		Matrix3 operator-() const;

		/// Matrix * vector [3x3 * 3x1 = 3x1]
		Vector3 operator*(const Vector3& rkVector) const;

		/// Vector * matrix [1x3 * 3x3 = 1x3]
		friend Vector3 operator*(const Vector3& rkVector, const Matrix3& rkMatrix);

		/// Matrix * scalar
		Matrix3 operator*(f32 fScalar) const;

		/// Scalar * matrix
		friend Matrix3 operator*(f32 fScalar, const Matrix3& rkMatrix);

		// utilities
		Matrix3 Transpose() const;
		bool Inverse(Matrix3& rkInverse, f32 fTolerance = 1e-06) const;
		Matrix3 Inverse(f32 fTolerance = 1e-06) const;
		f32 Determinant() const;

		// singular value decomposition
		void SingularValueDecomposition(Matrix3& rkL, Vector3& rkS,
										Matrix3& rkR) const;
		void SingularValueComposition(const Matrix3& rkL,
									const Vector3& rkS, const Matrix3& rkR);

		/// Gram-Schmidt orthonormalization (applied to columns of rotation matrix)
		void Orthonormalize();

		/// Orthogonal Q, diagonal D, upper triangular U stored as (u01,u02,u12)
		void QDUDecomposition(Matrix3& rkQ, Vector3& rkD,
							Vector3& rkU) const;

		f32 SpectralNorm() const;

		// matrix must be orthonormal
		void ToAngleAxis(Vector3& rkAxis, Radian& rfAngle) const;

		inline void ToAngleAxis(Vector3& rkAxis, Degree& rfAngle) const
		{
			Radian r;
			ToAngleAxis(rkAxis, r);
			rfAngle = r;
		}

		void FromAngleAxis(const Vector3& rkAxis, const Radian& fRadians);

		// The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
		// where yaw is rotation about the Up vector, pitch is rotation about the
		// Right axis, and roll is rotation about the Direction axis.
		bool ToEulerAnglesXYZ(Radian& rfYAngle, Radian& rfPAngle,
							Radian& rfRAngle) const;
		bool ToEulerAnglesXZY(Radian& rfYAngle, Radian& rfPAngle,
							Radian& rfRAngle) const;
		bool ToEulerAnglesYXZ(Radian& rfYAngle, Radian& rfPAngle,
							Radian& rfRAngle) const;
		bool ToEulerAnglesYZX(Radian& rfYAngle, Radian& rfPAngle,
							Radian& rfRAngle) const;
		bool ToEulerAnglesZXY(Radian& rfYAngle, Radian& rfPAngle,
							Radian& rfRAngle) const;
		bool ToEulerAnglesZYX(Radian& rfYAngle, Radian& rfPAngle,
							Radian& rfRAngle) const;
		void FromEulerAnglesXYZ(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void FromEulerAnglesXZY(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void FromEulerAnglesYXZ(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void FromEulerAnglesYZX(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void FromEulerAnglesZXY(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		void FromEulerAnglesZYX(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
		/// Eigensolver, matrix must be symmetric
		void EigenSolveSymmetric(f32 afEigenvalue[3],
								Vector3 akEigenvector[3]) const;

		static void TensorProduct(const Vector3& rkU, const Vector3& rkV,
								Matrix3& rkProduct);

		/** Determines if this matrix involves a scaling. */
		inline bool hasScale() const
		{
			// check magnitude of column vectors (==local axes)
			f32 t = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
			if (!Math::f32Equal(t, 1.0, (f32)1e-04))
				return true;
			t = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1];
			if (!Math::f32Equal(t, 1.0, (f32)1e-04))
				return true;
			t = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];
			if (!Math::f32Equal(t, 1.0, (f32)1e-04))
				return true;

			return false;
		}

		/** Function for writing to a stream.
		*/
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
		void Tridiagonal(f32 afDiag[3], f32 afSubDiag[3]);
		bool QLAlgorithm(f32 afDiag[3], f32 afSubDiag[3]);

		// support for singular value decomposition
		static const f32 msSvdEpsilon;
		static const unsigned int msSvdMaxIterations;
		static void Bidiagonalize(Matrix3& kA, Matrix3& kL,
								Matrix3& kR);
		static void GolubKahanStep(Matrix3& kA, Matrix3& kL,
									Matrix3& kR);

		// support for spectral norm
		static f32 MaxCubicRoot(f32 afCoeff[3]);

		f32 m[3][3];

		// for faster access
		friend class Matrix4;
	};
}

#endif