#ifndef __SOLO_MATRIX4_H__
#define __SOLO_MATRIX4_H__

#include "SoloMath.h"
#include "SoloVec3.h"

namespace solo
{
	template <class T>
	class Matrix4
	{
	public:
		enum class MatrixConstructor
		{
			Nothing = 0,
			Copy,
			Identity,
			Transposed,
			Inverse,
			InverseTransposed
		};

		explicit Matrix4(MatrixConstructor constructor = MatrixConstructor::Identity);
		Matrix4(const Matrix4<T> &other, MatrixConstructor constructor = MatrixConstructor::Copy);

		T &operator()(const s32 row, const s32 col)
		{
			return _data[row * 4 + col];
		}

		const T &operator()(const s32 row, const s32 col) const
		{
			return _data[row * 4 + col];
		}

		T &operator[](u32 index)
		{
			return _data[index];
		}

		const T &operator[](u32 index) const
		{
			return _data[index];
		}

		inline Matrix4<T> &operator=(const Matrix4<T> &other);
		inline Matrix4<T> &operator=(const T &scalar);

		const T *pointer() const
		{
			return _data;
		}

		T *pointer()
		{
			return _data;
		}

		bool operator==(const Matrix4<T> &other) const;
		bool operator!=(const Matrix4<T> &other) const;

		Matrix4<T> operator+(const Matrix4<T> &other) const;
		Matrix4<T> &operator+=(const Matrix4<T> &other);

		Matrix4<T> operator-(const Matrix4<T> &other) const;
		Matrix4<T> &operator-=(const Matrix4<T> &other);

		inline Matrix4<T> &setByProduct(const Matrix4<T> &first, const Matrix4<T> &second);
		Matrix4<T> &setByProductNoCheck(const Matrix4<T> &other_a, const Matrix4<T> &other_b);

		Matrix4<T> operator*(const Matrix4<T> &other) const;
		Matrix4<T> &operator*=(const Matrix4<T> &other);

		Matrix4<T> operator*(const T &scalar) const;
		Matrix4<T> &operator*=(const T &scalar);

		inline Matrix4<T> &makeIdentity();

		inline bool isIdentity() const;
		inline bool isOrthogonal() const;

		Matrix4<T> &setTranslation(const Vec3<T> &translation);
		Vec3<T> getTranslation() const;
		Matrix4<T> &setInverseTranslation(const Vec3<T> &translation);

		inline Matrix4<T> &setRotationRadians(const Vec3<T> &rotation);
		Matrix4<T> &setRotationDegrees(const Vec3<T> &rotation);
		Vec3<T> getRotationDegrees() const;
		inline Matrix4<T> &setInverseRotationRadians(const Vec3<T> &rotation);
		inline Matrix4<T> &setInverseRotationDegrees(const Vec3<T> &rotation);

		inline Matrix4<T> &setRotationAxisRadiansLH(const T &angle, const Vec3<T> &axis);
		inline Matrix4<T> &setRotationAxisRadiansRH(const T &angle, const Vec3<T> &axis);

		Vec3<T> getScale() const;
		Matrix4<T> &setScale(const Vec3<T> &scale);
		Matrix4<T> &setScale(const T scale)
		{
			return setScale(Vec3<T>(scale, scale, scale));
		}

		//! Translate a vector by the inverse of the translation part of this matrix.
		void inverseTranslateVect(Vec3f &vect) const;

		//! Rotate a vector by the inverse of the rotation part of this matrix.
		void inverseRotateVect(Vec3f &vect) const;

		//! Rotate a vector by the rotation part of this matrix.
		void rotateVect(Vec3f &vect) const;

		//! An alternate transform vector method, writing into a second vector
		void rotateVect(Vec3f &out, const Vec3f &in) const;

		//! An alternate transform vector method, writing into an array of 3 floats
		void rotateVect(T *out, const Vec3f &in) const;

		//! Transforms the vector by this matrix
		void transformVect(Vec3f &vect) const;

		//! Transforms input vector by this matrix and stores result in output vector
		void transformVect(Vec3f &out, const Vec3f &in) const;

		//! An alternate transform vector method, writing into an array of 4 floats
		void transformVect(T *out, const Vec3f &in) const;

		//! An alternate transform vector method, reading from and writing to an array of 3 floats
		void transformVec3(T *out, const T *in) const;

		//! Translate a vector by the translation part of this matrix.
		void translateVect(Vec3f &vect) const;

		//! Transforms a plane by this matrix
		//void transformPlane(Plane3d<f32> &plane) const;

		//! Transforms a plane by this matrix
		//void transformPlane(const Plane3d<f32> &in, Plane3d<f32> &out) const;

		//! Transforms a axis aligned bounding box
		/** The result box of this operation may not be accurate at all. For
		correct results, use transformBoxEx() */
		//void transformBox(BoundingBox<f32> &box) const;

		//! Transforms a axis aligned bounding box
		/** The result box of this operation should by accurate, but this operation
		is slower than transformBox(). */
		//void transformBoxEx(BoundingBox<f32> &box) const;

		//! Multiplies this matrix by a 1x4 matrix
		void multiplyWith1x4Matrix(T *matrix) const;

		//! Calculates inverse of matrix. Slow.
		/** \return Returns false if there is no inverse matrix.*/
		bool makeInverse();

		//! Inverts a primitive matrix which only contains a translation and a rotation
		/** \param out: where result matrix is written to. */
		bool getInversePrimitive(Matrix4<T> &out) const;

		//! Gets the inversed matrix of this one
		/** \param out: where result matrix is written to.
		\return Returns false if there is no inverse matrix. */
		bool getInverse(Matrix4<T> &out) const;

		//! Builds a right-handed perspective projection matrix based on a field of view
		Matrix4<T> &buildProjectionMatrixPerspectiveFovRH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar);

		//! Builds a left-handed perspective projection matrix based on a field of view
		Matrix4<T> &buildProjectionMatrixPerspectiveFovLH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar);

		//! Builds a left-handed perspective projection matrix based on a field of view, with far plane at infinity
		Matrix4<T> &buildProjectionMatrixPerspectiveFovInfinityLH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 epsilon = 0);

		//! Builds a right-handed perspective projection matrix.
		Matrix4<T> &buildProjectionMatrixPerspectiveRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

		//! Builds a left-handed perspective projection matrix.
		Matrix4<T> &buildProjectionMatrixPerspectiveLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

		//! Builds a left-handed orthogonal projection matrix.
		Matrix4<T> &buildProjectionMatrixOrthoLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

		//! Builds a right-handed orthogonal projection matrix.
		Matrix4<T> &buildProjectionMatrixOrthoRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

		//! Builds a left-handed look-at matrix.
		Matrix4<T> &buildCameraLookAtMatrixLH(
			const Vec3f &position,
			const Vec3f &target,
			const Vec3f &upVector);

		//! Builds a right-handed look-at matrix.
		Matrix4<T> &buildCameraLookAtMatrixRH(
			const Vec3f &position,
			const Vec3f &target,
			const Vec3f &upVector);

		//! Builds a matrix that flattens geometry into a plane.
		/** \param light: light source
		\param plane: plane into which the geometry if flattened into
		\param point: value between 0 and 1, describing the light source.
		If this is 1, it is a point light, if it is 0, it is a directional light. */
		//Matrix4<T> &buildShadowMatrix(const Vec3f &light, Plane3df plane, f32 point = 1.0f);

		//! Builds a matrix which transforms a normalized Device Coordinate to Device Coordinates.
		/** Used to scale < - 1, - 1><1,1> to viewport, for example from < - 1, - 1> <1,1> to the viewport <0,0><0,640> */
		//Matrix4<T> &buildNDCToDCMatrix(const Rect<s32> &area, f32 zScale);

		//! Creates a new matrix as interpolated matrix from two other ones.
		/** \param b: other matrix to interpolate with
		\param time: Must be a value between 0 and 1. */
		Matrix4<T> interpolate(const Matrix4<T> &b, f32 time) const;

		//! Gets transposed matrix
		Matrix4<T> getTransposed() const;

		//! Gets transposed matrix
		inline void getTransposed(Matrix4<T> &dest) const;

		//! Builds a matrix that rotates from one vector to another
		/** \param from: vector to rotate from
		\param to: vector to rotate to
		*/
		Matrix4<T> &buildRotateFromTo(const Vec3f &from, const Vec3f &to);

		//! Builds a combined matrix which translates to a center before rotation and translates from origin afterwards
		/** \param center Position to rotate around
		\param translate Translation applied after the rotation
		*/
		void setRotationCenter(const Vec3f &center, const Vec3f &translate);

		//! Builds a matrix which rotates a source vector to a look vector over an arbitrary axis
		/** \param camPos: viewer position in world coo
		\param center: object position in world-coo and rotation pivot
		\param translation: object final translation from center
		\param axis: axis to rotate about
		\param from: source vector to rotate from
		*/
		void buildAxisAlignedBillboard(const Vec3f &camPos,
			const Vec3f &center,
			const Vec3f &translation,
			const Vec3f &axis,
			const Vec3f &from);

		/*
		construct 2D Texture transformations
		rotate about center, scale, and transform.
		*/
		//! Set to a texture transformation matrix with the given parameters.
//		Matrix4<T> &buildTextureTransform(f32 rotateRad,
//			const Vec2df &rotatecenter,
//			const Vec2df &translate,
//			const Vec2df &scale);

		//! Set texture transformation rotation
		/** Rotate about z axis, recenter at (0.5,0.5).
		Doesn't clear other elements than those affected
		\param radAngle Angle in radians
		\return Altered matrix */
		Matrix4<T> &setTextureRotationCenter(f32 radAngle);

		//! Set texture transformation translation
		/** Doesn't clear other elements than those affected.
		\param x Offset on x axis
		\param y Offset on y axis
		\return Altered matrix */
		Matrix4<T> &setTextureTranslate(f32 x, f32 y);

		//! Set texture transformation translation, using a transposed representation
		/** Doesn't clear other elements than those affected.
		\param x Offset on x axis
		\param y Offset on y axis
		\return Altered matrix */
		Matrix4<T> &setTextureTranslateTransposed(f32 x, f32 y);

		//! Set texture transformation scale
		/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
		Matrix4<T> &setTextureScale(f32 sx, f32 sy);

		//! Set texture transformation scale, and recenter at (0.5,0.5)
		/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
		Matrix4<T> &setTextureScaleCenter(f32 sx, f32 sy);

		//! Sets all matrix data members at once
		Matrix4<T> &setData(const T *data);

		//! Sets if the matrix is definitely identity matrix
		void setDefinitelyIdentityMatrix(bool isDefinitelyIdentityMatrix);

		//! Gets if the matrix is definitely identity matrix
		bool getDefinitelyIdentityMatrix() const;

		//! Compare two matrices using the equal method
		bool equals(const Matrix4<T> &other, const T tolerance = (T)ROUNDING_ERROR_F64) const;

	private:
		// row-major order
		T _data[16];
	};

	template <class T>
	inline Matrix4<T>::Matrix4(MatrixConstructor constructor)
	{
		switch (constructor)
		{
		case MatrixConstructor::CONST_NOTHING:
		case MatrixConstructor::CONST_COPY:
			break;
		case MatrixConstructor::CONST_IDENTITY:
		case MatrixConstructor::CONST_INVERSE:
		default:
			makeIdentity();
			break;
		}
	}

	template <class T>
	inline Matrix4<T>::Matrix4(const Matrix4<T> &other, MatrixConstructor constructor)
	{
		switch (constructor)
		{
		case MatrixConstructor::Identity:
			makeIdentity();
			break;
		case MatrixConstructor::Nothing:
			break;
		case MatrixConstructor::Copy:
			*this = other;
			break;
		case MatrixConstructor::Transposed:
			other.getTransposed(*this);
			break;
		case MatrixConstructor::Inverse:
			if (!other.getInverse(*this))
				memset(_data, 0, 16 * sizeof(T));
			break;
		case MatrixConstructor::InverseTransposed:
			if (!other.getInverse(*this))
				memset(_data, 0, 16 * sizeof(T));
			else
				*this = getTransposed();
			break;
		}
	}

	template <class T>
	inline Matrix4<T> Matrix4<T>::operator+(const Matrix4<T> &other) const
	{
		Matrix4<T> temp(MatrixConstructor::Nothing);

		temp[0] = _data[0] + other[0];
		temp[1] = _data[1] + other[1];
		temp[2] = _data[2] + other[2];
		temp[3] = _data[3] + other[3];
		temp[4] = _data[4] + other[4];
		temp[5] = _data[5] + other[5];
		temp[6] = _data[6] + other[6];
		temp[7] = _data[7] + other[7];
		temp[8] = _data[8] + other[8];
		temp[9] = _data[9] + other[9];
		temp[10] = _data[10] + other[10];
		temp[11] = _data[11] + other[11];
		temp[12] = _data[12] + other[12];
		temp[13] = _data[13] + other[13];
		temp[14] = _data[14] + other[14];
		temp[15] = _data[15] + other[15];

		return temp;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::operator+=(const Matrix4<T> &other)
	{
		_data[0] += other[0];
		_data[1] += other[1];
		_data[2] += other[2];
		_data[3] += other[3];
		_data[4] += other[4];
		_data[5] += other[5];
		_data[6] += other[6];
		_data[7] += other[7];
		_data[8] += other[8];
		_data[9] += other[9];
		_data[10] += other[10];
		_data[11] += other[11];
		_data[12] += other[12];
		_data[13] += other[13];
		_data[14] += other[14];
		_data[15] += other[15];

		return *this;
	}

	template <class T>
	inline Matrix4<T> Matrix4<T>::operator-(const Matrix4<T> &other) const
	{
		Matrix4<T> temp(MatrixConstructor::Nothing);

		temp[0] = _data[0] - other[0];
		temp[1] = _data[1] - other[1];
		temp[2] = _data[2] - other[2];
		temp[3] = _data[3] - other[3];
		temp[4] = _data[4] - other[4];
		temp[5] = _data[5] - other[5];
		temp[6] = _data[6] - other[6];
		temp[7] = _data[7] - other[7];
		temp[8] = _data[8] - other[8];
		temp[9] = _data[9] - other[9];
		temp[10] = _data[10] - other[10];
		temp[11] = _data[11] - other[11];
		temp[12] = _data[12] - other[12];
		temp[13] = _data[13] - other[13];
		temp[14] = _data[14] - other[14];
		temp[15] = _data[15] - other[15];

		return temp;
	}
	
	template <class T>
	inline Matrix4<T> &Matrix4<T>::operator-=(const Matrix4<T> &other)
	{
		_data[0] -= other[0];
		_data[1] -= other[1];
		_data[2] -= other[2];
		_data[3] -= other[3];
		_data[4] -= other[4];
		_data[5] -= other[5];
		_data[6] -= other[6];
		_data[7] -= other[7];
		_data[8] -= other[8];
		_data[9] -= other[9];
		_data[10] -= other[10];
		_data[11] -= other[11];
		_data[12] -= other[12];
		_data[13] -= other[13];
		_data[14] -= other[14];
		_data[15] -= other[15];

		return *this;
	}

	template <class T>
	inline Matrix4<T> Matrix4<T>::operator*(const T &scalar) const
	{
		Matrix4<T> temp(MatrixConstructor::Nothing);

		temp[0] = _data[0] * scalar;
		temp[1] = _data[1] * scalar;
		temp[2] = _data[2] * scalar;
		temp[3] = _data[3] * scalar;
		temp[4] = _data[4] * scalar;
		temp[5] = _data[5] * scalar;
		temp[6] = _data[6] * scalar;
		temp[7] = _data[7] * scalar;
		temp[8] = _data[8] * scalar;
		temp[9] = _data[9] * scalar;
		temp[10] = _data[10] * scalar;
		temp[11] = _data[11] * scalar;
		temp[12] = _data[12] * scalar;
		temp[13] = _data[13] * scalar;
		temp[14] = _data[14] * scalar;
		temp[15] = _data[15] * scalar;

		return temp;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::operator*=(const T &scalar)
	{
		_data[0] *= scalar;
		_data[1] *= scalar;
		_data[2] *= scalar;
		_data[3] *= scalar;
		_data[4] *= scalar;
		_data[5] *= scalar;
		_data[6] *= scalar;
		_data[7] *= scalar;
		_data[8] *= scalar;
		_data[9] *= scalar;
		_data[10] *= scalar;
		_data[11] *= scalar;
		_data[12] *= scalar;
		_data[13] *= scalar;
		_data[14] *= scalar;
		_data[15] *= scalar;

		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::operator*=(const Matrix4<T> &other)
	{
		Matrix4<T> temp(*this);
		return setByProductNoCheck(temp, other);
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setByProductNoCheck(const Matrix4<T> &other_a, const Matrix4<T> &other_b)
	{
		const T *m1 = other_a._data;
		const T *m2 = other_b._data;

		_data[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
		_data[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
		_data[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
		_data[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

		_data[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
		_data[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
		_data[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
		_data[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

		_data[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
		_data[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
		_data[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
		_data[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

		_data[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
		_data[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
		_data[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
		_data[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
		
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setByProduct(const Matrix4<T> &other_a, const Matrix4<T> &other_b)
	{
		return setByProductNoCheck(other_a, other_b);
	}

	template <class T>
	inline Matrix4<T> Matrix4<T>::operator*(const Matrix4<T> &m2) const
	{
		Matrix4<T> m3(MatrixConstructor::Nothing);

		const T *m1 = _data;

		m3[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
		m3[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
		m3[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
		m3[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

		m3[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
		m3[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
		m3[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
		m3[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

		m3[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
		m3[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
		m3[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
		m3[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

		m3[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
		m3[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
		m3[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
		m3[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

		return m3;
	}

	template <class T>
	inline Vec3<T> Matrix4<T>::getTranslation() const
	{
		return Vec3<T>(_data[12], _data[13], _data[14]);
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setTranslation(const Vec3<T> &translation)
	{
		_data[12] = translation.x;
		_data[13] = translation.y;
		_data[14] = translation.z;
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setInverseTranslation(const Vec3<T> &translation)
	{
		_data[12] = -translation.x;
		_data[13] = -translation.y;
		_data[14] = -translation.z;
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setScale(const Vec3<T> &scale)
	{
		_data[0] = scale.x;
		_data[5] = scale.y;
		_data[10] = scale.z;
		return *this;
	}

	//! Returns the absolute values of the scales of the matrix.
	/**
	Note that this returns the absolute (positive) values unless only scale is set.
	Unfortunately it does not appear to be possible to extract any original negative
	values. The best that we could do would be to arbitrarily make one scale
	negative if one or three of them were negative.
	FIXME - return the original values.
	*/
	template <class T>
	inline Vec3<T> Matrix4<T>::getScale() const
	{
		// See http://www.robertblum.com/articles/2005/02/14/decomposing-matrices

		// Deal with the 0 rotation case first
		// Prior to Solo 1.6, we always returned this value.
		if (iszero(_data[1]) && iszero(_data[2]) &&
			iszero(_data[4]) && iszero(_data[6]) &&
			iszero(_data[8]) && iszero(_data[9]))
			return Vec3<T>(_data[0], _data[5], _data[10]);

		// We have to do the full calculation.
		return Vec3<T>(sqrtf(_data[0] * _data[0] + _data[1] * _data[1] + _data[2] * _data[2]),
			sqrtf(_data[4] * _data[4] + _data[5] * _data[5] + _data[6] * _data[6]),
			sqrtf(_data[8] * _data[8] + _data[9] * _data[9] + _data[10] * _data[10]));
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setRotationDegrees(const Vec3<T> &rotation)
	{
		return setRotationRadians(rotation * DEGTORAD);
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setInverseRotationDegrees(const Vec3<T> &rotation)
	{
		return setInverseRotationRadians(rotation * DEGTORAD);
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setRotationRadians(const Vec3<T> &rotation)
	{
		const f64 cr = cos(rotation.x);
		const f64 sr = sin(rotation.x);
		const f64 cp = cos(rotation.y);
		const f64 sp = sin(rotation.y);
		const f64 cy = cos(rotation.z);
		const f64 sy = sin(rotation.z);

		_data[0] = (T)(cp * cy);
		_data[1] = (T)(cp * sy);
		_data[2] = (T)(-sp);

		const f64 srsp = sr * sp;
		const f64 crsp = cr * sp;

		_data[4] = (T)(srsp * cy - cr * sy);
		_data[5] = (T)(srsp * sy + cr * cy);
		_data[6] = (T)(sr * cp);

		_data[8] = (T)(crsp * cy + sr * sy);
		_data[9] = (T)(crsp * sy - sr * cy);
		_data[10] = (T)(cr * cp);
		
		return *this;
	}

	//! Returns a rotation that is equivalent to that set by setRotationDegrees().
	/** This code was sent in by Chev.  Note that it does not necessarily return
	the *same* Euler angles as those set by setRotationDegrees(), but the rotation will
	be equivalent, i.e. will have the same result when used to rotate a vector or node. */
	template <class T>
	inline Vec3<T> Matrix4<T>::getRotationDegrees() const
	{
		const Matrix4<T> &mat = *this;
		Vec3<T> scale = getScale();
		// we need to check for negative scale on to axes, which would bring up wrong results
		if (scale.y < 0 && scale.z < 0)
		{
			scale.y = -scale.y;
			scale.z = -scale.z;
		}
		else if (scale.x < 0 && scale.z < 0)
		{
			scale.x = -scale.x;
			scale.z = -scale.z;
		}
		else if (scale.x < 0 && scale.y < 0)
		{
			scale.x = -scale.x;
			scale.y = -scale.y;
		}
		const Vec3<f64> invScale(reciprocal(scale.x), reciprocal(scale.y), reciprocal(scale.z));

		f64 Y = -asin(clamp(mat[2] * invScale.x, -1.0, 1.0));
		const f64 C = cos(Y);
		Y *= RADTODEG64;

		f64 rotx, roty, X, Z;

		if (!iszero(C))
		{
			const f64 invC = reciprocal(C);
			rotx = mat[10] * invC * invScale.z;
			roty = mat[6] * invC * invScale.y;
			X = atan2(roty, rotx) * RADTODEG64;
			rotx = mat[0] * invC * invScale.x;
			roty = mat[1] * invC * invScale.x;
			Z = atan2(roty, rotx) * RADTODEG64;
		}
		else
		{
			X = 0.0;
			rotx = mat[5] * invScale.y;
			roty = -mat[4] * invScale.y;
			Z = atan2(roty, rotx) * RADTODEG64;
		}

		// fix values that get below zero
		if (X < 0.0) X += 360.0;
		if (Y < 0.0) Y += 360.0;
		if (Z < 0.0) Z += 360.0;

		return Vec3<T>((T)X, (T)Y, (T)Z);
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setInverseRotationRadians(const Vec3<T> &rotation)
	{
		f64 cr = cos(rotation.x);
		f64 sr = sin(rotation.x);
		f64 cp = cos(rotation.y);
		f64 sp = sin(rotation.y);
		f64 cy = cos(rotation.z);
		f64 sy = sin(rotation.z);

		_data[0] = (T)(cp * cy);
		_data[4] = (T)(cp * sy);
		_data[8] = (T)(-sp);

		f64 srsp = sr * sp;
		f64 crsp = cr * sp;

		_data[1] = (T)(srsp * cy - cr * sy);
		_data[5] = (T)(srsp * sy + cr * cy);
		_data[9] = (T)(sr * cp);

		_data[2] = (T)(crsp * cy + sr * sy);
		_data[6] = (T)(crsp * sy - sr * cy);
		_data[10] = (T)(cr * cp);
		
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setRotationAxisRadiansLH(const T &angle, const Vec3<T> &axis)
	{
		const f64 c = cos(angle);
		const f64 s = sin(angle);
		const f64 t = 1.0 - c;

		const f64 tx = t * axis.x;

		_data[0] = (T)(tx * axis.x + c);
		_data[1] = (T)(tx * axis.y - s * axis.z);
		_data[2] = (T)(tx * axis.z + s * axis.y);

		const f64 ty = t * axis.y;

		_data[4] = (T)(ty * axis.x + s * axis.z);
		_data[5] = (T)(ty * axis.y + c);
		_data[6] = (T)(ty * axis.z - s * axis.x);

		const f64 tz = t * axis.z;

		_data[8] = (T)(tz * axis.x - s * axis.y);
		_data[9] = (T)(tz * axis.z + s * axis.x);
		_data[10] = (T)(tz * axis.z + c);

		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setRotationAxisRadiansRH(const T &angle, const Vec3<T> &axis)
	{
		const f64 c = cos(angle);
		const f64 s = sin(angle);
		const f64 t = 1.0 - c;

		const f64 tx = t * axis.x;

		_data[0] = (T)(tx * axis.x + c);
		_data[1] = (T)(tx * axis.y + s * axis.z);
		_data[2] = (T)(tx * axis.z - s * axis.y);

		const f64 ty = t * axis.y;

		_data[4] = (T)(ty * axis.x - s * axis.z);
		_data[5] = (T)(ty * axis.y + c);
		_data[6] = (T)(ty * axis.z + s * axis.x);

		const f64 tz = t * axis.z;

		_data[8] = (T)(tz * axis.x + s * axis.y);
		_data[9] = (T)(tz * axis.z - s * axis.x);
		_data[10] = (T)(tz * axis.z + c);
		
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::makeIdentity()
	{
		memset(_data, 0, 16 * sizeof(T));
		_data[0] = _data[5] = _data[10] = _data[15] = (T)1;
		return *this;
	}

	/*
	check identity with epsilon
	solve floating range problems..
	*/
	template <class T>
	inline bool Matrix4<T>::isIdentity() const
	{
		if (!equals(_data[12], (T)0) || !equals(_data[13], (T)0) || !equals(_data[14], (T)0) || !equals(_data[15], (T)1))
			return false;

		if (!equals(_data[0], (T)1) || !equals(_data[1], (T)0) || !equals(_data[2], (T)0) || !equals(_data[3], (T)0))
			return false;

		if (!equals(_data[4], (T)0) || !equals(_data[5], (T)1) || !equals(_data[6], (T)0) || !equals(_data[7], (T)0))
			return false;

		if (!equals(_data[8], (T)0) || !equals(_data[9], (T)0) || !equals(_data[10], (T)1) || !equals(_data[11], (T)0))
			return false;
		
		return true;
	}

	template <class T>
	inline bool Matrix4<T>::isOrthogonal() const
	{
		T dp = _data[0] * _data[4] + _data[1] * _data[5] + _data[2] * _data[6] + _data[3] * _data[7];
		if (!iszero(dp))
			return false;
		dp = _data[0] * _data[8] + _data[1] * _data[9] + _data[2] * _data[10] + _data[3] * _data[11];
		if (!iszero(dp))
			return false;
		dp = _data[0] * _data[12] + _data[1] * _data[13] + _data[2] * _data[14] + _data[3] * _data[15];
		if (!iszero(dp))
			return false;
		dp = _data[4] * _data[8] + _data[5] * _data[9] + _data[6] * _data[10] + _data[7] * _data[11];
		if (!iszero(dp))
			return false;
		dp = _data[4] * _data[12] + _data[5] * _data[13] + _data[6] * _data[14] + _data[7] * _data[15];
		if (!iszero(dp))
			return false;
		dp = _data[8] * _data[12] + _data[9] * _data[13] + _data[10] * _data[14] + _data[11] * _data[15];
		return (iszero(dp));
	}

	template <class T>
	inline void Matrix4<T>::rotateVect(Vec3f &vect) const
	{
		Vec3f tmp = vect;
		vect.x = tmp.x * _data[0] + tmp.y * _data[4] + tmp.z * _data[8];
		vect.y = tmp.x * _data[1] + tmp.y * _data[5] + tmp.z * _data[9];
		vect.z = tmp.x * _data[2] + tmp.y * _data[6] + tmp.z * _data[10];
	}

	//! An alternate transform vector method, writing into a second vector
	template <class T>
	inline void Matrix4<T>::rotateVect(Vec3f &out, const Vec3f &in) const
	{
		out.x = in.x * _data[0] + in.y * _data[4] + in.z * _data[8];
		out.y = in.x * _data[1] + in.y * _data[5] + in.z * _data[9];
		out.z = in.x * _data[2] + in.y * _data[6] + in.z * _data[10];
	}

	//! An alternate transform vector method, writing into an array of 3 floats
	template <class T>
	inline void Matrix4<T>::rotateVect(T *out, const Vec3f &in) const
	{
		out[0] = in.x * _data[0] + in.y * _data[4] + in.z * _data[8];
		out[1] = in.x * _data[1] + in.y * _data[5] + in.z * _data[9];
		out[2] = in.x * _data[2] + in.y * _data[6] + in.z * _data[10];
	}

	template <class T>
	inline void Matrix4<T>::inverseRotateVect(Vec3f &vect) const
	{
		Vec3f tmp = vect;
		vect.x = tmp.x * _data[0] + tmp.y * _data[1] + tmp.z * _data[2];
		vect.y = tmp.x * _data[4] + tmp.y * _data[5] + tmp.z * _data[6];
		vect.z = tmp.x * _data[8] + tmp.y * _data[9] + tmp.z * _data[10];
	}

	template <class T>
	inline void Matrix4<T>::transformVect(Vec3f &vect) const
	{
		f32 vector[3];

		vector[0] = vect.x * _data[0] + vect.y * _data[4] + vect.z * _data[8] + _data[12];
		vector[1] = vect.x * _data[1] + vect.y * _data[5] + vect.z * _data[9] + _data[13];
		vector[2] = vect.x * _data[2] + vect.y * _data[6] + vect.z * _data[10] + _data[14];

		vect.x = vector[0];
		vect.y = vector[1];
		vect.z = vector[2];
	}

	template <class T>
	inline void Matrix4<T>::transformVect(Vec3f &out, const Vec3f &in) const
	{
		out.x = in.x * _data[0] + in.y * _data[4] + in.z * _data[8] + _data[12];
		out.y = in.x * _data[1] + in.y * _data[5] + in.z * _data[9] + _data[13];
		out.z = in.x * _data[2] + in.y * _data[6] + in.z * _data[10] + _data[14];
	}


	template <class T>
	inline void Matrix4<T>::transformVect(T *out, const Vec3f &in) const
	{
		out[0] = in.x * _data[0] + in.y * _data[4] + in.z * _data[8] + _data[12];
		out[1] = in.x * _data[1] + in.y * _data[5] + in.z * _data[9] + _data[13];
		out[2] = in.x * _data[2] + in.y * _data[6] + in.z * _data[10] + _data[14];
		out[3] = in.x * _data[3] + in.y * _data[7] + in.z * _data[11] + _data[15];
	}

	template <class T>
	inline void Matrix4<T>::transformVec3(T *out, const T *in) const
	{
		out[0] = in[0] * _data[0] + in[1] * _data[4] + in[2] * _data[8] + _data[12];
		out[1] = in[0] * _data[1] + in[1] * _data[5] + in[2] * _data[9] + _data[13];
		out[2] = in[0] * _data[2] + in[1] * _data[6] + in[2] * _data[10] + _data[14];
	}


	//! Transforms a plane by this matrix
	template <class T>
	inline void Matrix4<T>::transformPlane(Plane3d<f32> &plane) const
	{
		Vec3f member;
		// Transform the plane member point, i.e. rotate, translate and scale it.
		transformVect(member, plane.getMemberPoint());

		// Transform the normal by the transposed inverse of the matrix
		Matrix4<T> transposedInverse(*this, MatrixConstructor::CONST_INVERSE_TRANSPOSED);
		Vec3f normal = plane.normal;
		transposedInverse.transformVect(normal);

		plane.setPlane(member, normal);
	}

	//! Transforms a plane by this matrix
	template <class T>
	inline void Matrix4<T>::transformPlane(const Plane3d<f32> &in, Plane3d<f32> &out) const
	{
		out = in;
		transformPlane(out);
	}

	//! Transforms a axis aligned bounding box
	template <class T>
	inline void Matrix4<T>::transformBox(BoundingBox<f32> &box) const
	{
#if defined (USE_MATRIX_TEST)
		if (isIdentity())
			return;
#endif

		transformVect(box.minEdge);
		transformVect(box.maxEdge);
		box.repair();
	}

	//! Transforms a axis aligned bounding box more accurately than transformBox()
	template <class T>
	inline void Matrix4<T>::transformBoxEx(BoundingBox<f32> &box) const
	{
#if defined (USE_MATRIX_TEST)
		if (isIdentity())
			return;
#endif

		const f32 Amin[3] = { box.minEdge.x, box.minEdge.y, box.minEdge.z };
		const f32 Amax[3] = { box.maxEdge.x, box.maxEdge.y, box.maxEdge.z };

		f32 Bmin[3];
		f32 Bmax[3];

		Bmin[0] = Bmax[0] = _data[12];
		Bmin[1] = Bmax[1] = _data[13];
		Bmin[2] = Bmax[2] = _data[14];

		const Matrix4<T> &m = *this;

		for (u32 i = 0; i < 3; ++i)
		{
			for (u32 j = 0; j < 3; ++j)
			{
				const f32 a = m(j, i) * Amin[j];
				const f32 b = m(j, i) * Amax[j];

				if (a < b)
				{
					Bmin[i] += a;
					Bmax[i] += b;
				}
				else
				{
					Bmin[i] += b;
					Bmax[i] += a;
				}
			}
		}

		box.minEdge.x = Bmin[0];
		box.minEdge.y = Bmin[1];
		box.minEdge.z = Bmin[2];

		box.maxEdge.x = Bmax[0];
		box.maxEdge.y = Bmax[1];
		box.maxEdge.z = Bmax[2];
	}


	//! Multiplies this matrix by a 1x4 matrix
	template <class T>
	inline void Matrix4<T>::multiplyWith1x4Matrix(T *matrix) const
	{
		/*
		0  1  2  3
		4  5  6  7
		8  9  10 11
		12 13 14 15
		*/

		T mat[4];
		mat[0] = matrix[0];
		mat[1] = matrix[1];
		mat[2] = matrix[2];
		mat[3] = matrix[3];

		matrix[0] = _data[0] * mat[0] + _data[4] * mat[1] + _data[8] * mat[2] + _data[12] * mat[3];
		matrix[1] = _data[1] * mat[0] + _data[5] * mat[1] + _data[9] * mat[2] + _data[13] * mat[3];
		matrix[2] = _data[2] * mat[0] + _data[6] * mat[1] + _data[10] * mat[2] + _data[14] * mat[3];
		matrix[3] = _data[3] * mat[0] + _data[7] * mat[1] + _data[11] * mat[2] + _data[15] * mat[3];
	}

	template <class T>
	inline void Matrix4<T>::inverseTranslateVect(Vec3f &vect) const
	{
		vect.x = vect.x - _data[12];
		vect.y = vect.y - _data[13];
		vect.z = vect.z - _data[14];
	}

	template <class T>
	inline void Matrix4<T>::translateVect(Vec3f &vect) const
	{
		vect.x = vect.x + _data[12];
		vect.y = vect.y + _data[13];
		vect.z = vect.z + _data[14];
	}

	template <class T>
	inline bool Matrix4<T>::getInverse(Matrix4<T> &out) const
	{
		/// Calculates the inverse of this Matrix
		/// The inverse is calculated using Cramers rule.
		/// If no inverse exists then 'false' is returned.
		const Matrix4<T> &m = *this;

		f32 d = (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
			(m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
			(m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
			(m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
			(m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
			(m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0));

		if (iszero(d))
			return false;

		d = reciprocal(d);

		out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) +
			m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) +
			m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
		out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) +
			m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) +
			m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
		out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) +
			m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) +
			m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
		out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) +
			m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) +
			m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
		out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) +
			m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) +
			m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
		out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) +
			m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) +
			m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
		out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) +
			m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) +
			m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
		out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) +
			m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
			m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
		out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) +
			m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
			m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
		out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) +
			m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) +
			m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
		out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) +
			m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) +
			m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
		out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) +
			m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) +
			m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
		out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) +
			m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
			m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
		out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) +
			m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) +
			m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
		out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) +
			m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) +
			m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
		out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
			m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
			m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));
		
		return true;
	}


	//! Inverts a primitive matrix which only contains a translation and a rotation
	//! \param out: where result matrix is written to.
	template <class T>
	inline bool Matrix4<T>::getInversePrimitive(Matrix4<T> &out) const
	{
		out._data[0] = _data[0];
		out._data[1] = _data[4];
		out._data[2] = _data[8];
		out._data[3] = 0;

		out._data[4] = _data[1];
		out._data[5] = _data[5];
		out._data[6] = _data[9];
		out._data[7] = 0;

		out._data[8] = _data[2];
		out._data[9] = _data[6];
		out._data[10] = _data[10];
		out._data[11] = 0;

		out._data[12] = (T)-(_data[12] * _data[0] + _data[13] * _data[1] + _data[14] * _data[2]);
		out._data[13] = (T)-(_data[12] * _data[4] + _data[13] * _data[5] + _data[14] * _data[6]);
		out._data[14] = (T)-(_data[12] * _data[8] + _data[13] * _data[9] + _data[14] * _data[10]);
		out._data[15] = 1;

		return true;
	}

	template <class T>
	inline bool Matrix4<T>::makeInverse()
	{
		Matrix4<T> temp(MatrixConstructor::Nothing);
		if (getInverse(temp))
		{
			*this = temp;
			return true;
		}
		return false;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::operator=(const Matrix4<T> &other)
	{
		if (this == &other)
			return *this;
		memcpy(_data, other._data, 16 * sizeof(T));
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::operator=(const T &scalar)
	{
		for (s32 i = 0; i < 16; ++i)
			_data[i] = scalar;
		return *this;
	}

	template <class T>
	inline bool Matrix4<T>::operator==(const Matrix4<T> &other) const
	{
		for (s32 i = 0; i < 16; ++i)
		if (_data[i] != other._data[i])
			return false;
		return true;
	}

	template <class T>
	inline bool Matrix4<T>::operator!=(const Matrix4<T> &other) const
	{
		return !(*this == other);
	}

	// Builds a right-handed perspective projection matrix based on a field of view
	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildProjectionMatrixPerspectiveFovRH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
	{
		const f64 h = reciprocal(tan(fieldOfViewRadians * 0.5));
		const T w = static_cast<T>(h / aspectRatio);

		_data[0] = w;
		_data[1] = 0;
		_data[2] = 0;
		_data[3] = 0;

		_data[4] = 0;
		_data[5] = (T)h;
		_data[6] = 0;
		_data[7] = 0;

		_data[8] = 0;
		_data[9] = 0;
		_data[10] = (T)(zFar / (zNear - zFar)); // DirectX version
		//		_data[10] = (T)(zFar+zNear/(zNear-zFar)); // OpenGL version
		_data[11] = -1;

		_data[12] = 0;
		_data[13] = 0;
		_data[14] = (T)(zNear * zFar / (zNear - zFar)); // DirectX version
		//		_data[14] = (T)(2.0f*zNear*zFar/(zNear-zFar)); // OpenGL version
		_data[15] = 0;

		return *this;
	}

	// Builds a left-handed perspective projection matrix based on a field of view
	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildProjectionMatrixPerspectiveFovLH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
	{
		const f64 h = reciprocal(tan(fieldOfViewRadians * 0.5));
		const T w = static_cast<T>(h / aspectRatio);

		_data[0] = w;
		_data[1] = 0;
		_data[2] = 0;
		_data[3] = 0;

		_data[4] = 0;
		_data[5] = (T)h;
		_data[6] = 0;
		_data[7] = 0;

		_data[8] = 0;
		_data[9] = 0;
		_data[10] = (T)(zFar / (zFar - zNear));
		_data[11] = 1;

		_data[12] = 0;
		_data[13] = 0;
		_data[14] = (T)(-zNear * zFar / (zFar - zNear));
		_data[15] = 0;

		return *this;
	}

	// Builds a left-handed perspective projection matrix based on a field of view, with far plane culling at infinity
	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildProjectionMatrixPerspectiveFovInfinityLH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 epsilon)
	{
		const f64 h = reciprocal(tan(fieldOfViewRadians * 0.5));
		const T w = static_cast<T>(h / aspectRatio);

		_data[0] = w;
		_data[1] = 0;
		_data[2] = 0;
		_data[3] = 0;

		_data[4] = 0;
		_data[5] = (T)h;
		_data[6] = 0;
		_data[7] = 0;

		_data[8] = 0;
		_data[9] = 0;
		_data[10] = (T)(1.f - epsilon);
		_data[11] = 1;

		_data[12] = 0;
		_data[13] = 0;
		_data[14] = (T)(zNear * (epsilon - 1.f));
		_data[15] = 0;

		return *this;
	}

	// Builds a left-handed orthogonal projection matrix.
	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildProjectionMatrixOrthoLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		_data[0] = (T)(2 / widthOfViewVolume);
		_data[1] = 0;
		_data[2] = 0;
		_data[3] = 0;

		_data[4] = 0;
		_data[5] = (T)(2 / heightOfViewVolume);
		_data[6] = 0;
		_data[7] = 0;

		_data[8] = 0;
		_data[9] = 0;
		_data[10] = (T)(1 / (zFar - zNear));
		_data[11] = 0;

		_data[12] = 0;
		_data[13] = 0;
		_data[14] = (T)(zNear / (zNear - zFar));
		_data[15] = 1;
		
		return *this;
	}

	// Builds a right-handed orthogonal projection matrix.
	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildProjectionMatrixOrthoRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		_data[0] = (T)(2 / widthOfViewVolume);
		_data[1] = 0;
		_data[2] = 0;
		_data[3] = 0;

		_data[4] = 0;
		_data[5] = (T)(2 / heightOfViewVolume);
		_data[6] = 0;
		_data[7] = 0;

		_data[8] = 0;
		_data[9] = 0;
		_data[10] = (T)(1 / (zNear - zFar));
		_data[11] = 0;

		_data[12] = 0;
		_data[13] = 0;
		_data[14] = (T)(zNear / (zNear - zFar));
		_data[15] = 1;

		return *this;
	}

	// Builds a right-handed perspective projection matrix.
	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildProjectionMatrixPerspectiveRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		_data[0] = (T)(2 * zNear / widthOfViewVolume);
		_data[1] = 0;
		_data[2] = 0;
		_data[3] = 0;

		_data[4] = 0;
		_data[5] = (T)(2 * zNear / heightOfViewVolume);
		_data[6] = 0;
		_data[7] = 0;

		_data[8] = 0;
		_data[9] = 0;
		_data[10] = (T)(zFar / (zNear - zFar));
		_data[11] = -1;

		_data[12] = 0;
		_data[13] = 0;
		_data[14] = (T)(zNear * zFar / (zNear - zFar));
		_data[15] = 0;

		return *this;
	}

	// Builds a left-handed perspective projection matrix.
	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildProjectionMatrixPerspectiveLH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
	{
		_data[0] = (T)(2 * zNear / widthOfViewVolume);
		_data[1] = 0;
		_data[2] = 0;
		_data[3] = 0;

		_data[4] = 0;
		_data[5] = (T)(2 * zNear / heightOfViewVolume);
		_data[6] = 0;
		_data[7] = 0;

		_data[8] = 0;
		_data[9] = 0;
		_data[10] = (T)(zFar / (zFar - zNear));
		_data[11] = 1;

		_data[12] = 0;
		_data[13] = 0;
		_data[14] = (T)(zNear * zFar / (zNear - zFar));
		_data[15] = 0;
		
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildCameraLookAtMatrixLH(
		const Vec3f &position,
		const Vec3f &target,
		const Vec3f &upVector)
	{
		Vec3f zaxis = target - position;
		zaxis.normalize();

		Vec3f xaxis = upVector.crossProduct(zaxis);
		xaxis.normalize();

		Vec3f yaxis = zaxis.crossProduct(xaxis);

		_data[0] = (T)xaxis.x;
		_data[1] = (T)yaxis.x;
		_data[2] = (T)zaxis.x;
		_data[3] = 0;

		_data[4] = (T)xaxis.y;
		_data[5] = (T)yaxis.y;
		_data[6] = (T)zaxis.y;
		_data[7] = 0;

		_data[8] = (T)xaxis.z;
		_data[9] = (T)yaxis.z;
		_data[10] = (T)zaxis.z;
		_data[11] = 0;

		_data[12] = (T)-xaxis.dot(position);
		_data[13] = (T)-yaxis.dot(position);
		_data[14] = (T)-zaxis.dot(position);
		_data[15] = 1;

		return *this;
	}


	// Builds a right-handed look-at matrix.
	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildCameraLookAtMatrixRH(
		const Vec3f &position,
		const Vec3f &target,
		const Vec3f &upVector)
	{
		Vec3f zaxis = position - target;
		zaxis.normalize();

		Vec3f xaxis = upVector.crossProduct(zaxis);
		xaxis.normalize();

		Vec3f yaxis = zaxis.crossProduct(xaxis);

		_data[0] = (T)xaxis.x;
		_data[1] = (T)yaxis.x;
		_data[2] = (T)zaxis.x;
		_data[3] = 0;

		_data[4] = (T)xaxis.y;
		_data[5] = (T)yaxis.y;
		_data[6] = (T)zaxis.y;
		_data[7] = 0;

		_data[8] = (T)xaxis.z;
		_data[9] = (T)yaxis.z;
		_data[10] = (T)zaxis.z;
		_data[11] = 0;

		_data[12] = (T)-xaxis.dot(position);
		_data[13] = (T)-yaxis.dot(position);
		_data[14] = (T)-zaxis.dot(position);
		_data[15] = 1;
		
		return *this;
	}

	// creates a new matrix as interpolated matrix from this and the passed one.
	template <class T>
	inline Matrix4<T> Matrix4<T>::interpolate(const Matrix4<T> &b, f32 time) const
	{
		Matrix4<T> mat(MatrixConstructor::Nothing);
		for (u32 i = 0; i < 16; i += 4)
		{
			mat._data[i + 0] = (T)(_data[i + 0] + (b._data[i + 0] - _data[i + 0]) * time);
			mat._data[i + 1] = (T)(_data[i + 1] + (b._data[i + 1] - _data[i + 1]) * time);
			mat._data[i + 2] = (T)(_data[i + 2] + (b._data[i + 2] - _data[i + 2]) * time);
			mat._data[i + 3] = (T)(_data[i + 3] + (b._data[i + 3] - _data[i + 3]) * time);
		}
		return mat;
	}

	// returns transposed matrix
	template <class T>
	inline Matrix4<T> Matrix4<T>::getTransposed() const
	{
		Matrix4<T> t(MatrixConstructor::Nothing);
		getTransposed(t);
		return t;
	}

	// returns transposed matrix
	template <class T>
	inline void Matrix4<T>::getTransposed(Matrix4<T> &o) const
	{
		o[0] = _data[0];
		o[1] = _data[4];
		o[2] = _data[8];
		o[3] = _data[12];

		o[4] = _data[1];
		o[5] = _data[5];
		o[6] = _data[9];
		o[7] = _data[13];

		o[8] = _data[2];
		o[9] = _data[6];
		o[10] = _data[10];
		o[11] = _data[14];

		o[12] = _data[3];
		o[13] = _data[7];
		o[14] = _data[11];
		o[15] = _data[15];
	}

	//! Builds a matrix that rotates from one vector to another
	/** \param from: vector to rotate from
	\param to: vector to rotate to

	http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm
	*/
	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildRotateFromTo(const Vec3f &from, const Vec3f &to)
	{
		// unit vectors
		Vec3f f(from);
		Vec3f t(to);
		f.normalize();
		t.normalize();

		// axis multiplication by sin
		Vec3f vs(t.crossProduct(f));

		// axis of rotation
		Vec3f v(vs);
		v.normalize();

		// cosinus angle
		T ca = f.dot(t);

		Vec3f vt(v * (1 - ca));

		_data[0] = vt.x * v.x + ca;
		_data[5] = vt.y * v.y + ca;
		_data[10] = vt.z * v.z + ca;

		vt.x *= v.y;
		vt.z *= v.x;
		vt.y *= v.z;

		_data[1] = vt.x - vs.z;
		_data[2] = vt.z + vs.y;
		_data[3] = 0;

		_data[4] = vt.x + vs.z;
		_data[6] = vt.y - vs.x;
		_data[7] = 0;

		_data[8] = vt.z - vs.y;
		_data[9] = vt.y + vs.x;
		_data[11] = 0;

		_data[12] = 0;
		_data[13] = 0;
		_data[14] = 0;
		_data[15] = 1;

		return *this;
	}

	//! Builds a matrix which rotates a source vector to a look vector over an arbitrary axis
	/** \param camPos: viewer position in world coord
	\param center: object position in world-coord, rotation pivot
	\param translation: object final translation from center
	\param axis: axis to rotate about
	\param from: source vector to rotate from
	*/
	template <class T>
	inline void Matrix4<T>::buildAxisAlignedBillboard(
		const Vec3f &camPos,
		const Vec3f &center,
		const Vec3f &translation,
		const Vec3f &axis,
		const Vec3f &from)
	{
		// axis of rotation
		Vec3f up = axis;
		up.normalize();
		const Vec3f forward = (camPos - center).normalize();
		const Vec3f right = up.crossProduct(forward).normalize();

		// correct look vector
		const Vec3f look = right.crossProduct(up);

		// rotate from to
		// axis multiplication by sin
		const Vec3f vs = look.crossProduct(from);

		// cosinus angle
		const f32 ca = from.dot(look);

		Vec3f vt(up * (1.f - ca));

		_data[0] = static_cast<T>(vt.x * up.x + ca);
		_data[5] = static_cast<T>(vt.y * up.y + ca);
		_data[10] = static_cast<T>(vt.z * up.z + ca);

		vt.x *= up.y;
		vt.z *= up.x;
		vt.y *= up.z;

		_data[1] = static_cast<T>(vt.x - vs.z);
		_data[2] = static_cast<T>(vt.z + vs.y);
		_data[3] = 0;

		_data[4] = static_cast<T>(vt.x + vs.z);
		_data[6] = static_cast<T>(vt.y - vs.x);
		_data[7] = 0;

		_data[8] = static_cast<T>(vt.z - vs.y);
		_data[9] = static_cast<T>(vt.y + vs.x);
		_data[11] = 0;

		setRotationCenter(center, translation);
	}


	//! Builds a combined matrix which translate to a center before rotation and translate afterwards
	template <class T>
	inline void Matrix4<T>::setRotationCenter(const Vec3f &center, const Vec3f &translation)
	{
		_data[12] = -_data[0] * center.x - _data[4] * center.y - _data[8] * center.z + (center.x - translation.x);
		_data[13] = -_data[1] * center.x - _data[5] * center.y - _data[9] * center.z + (center.y - translation.y);
		_data[14] = -_data[2] * center.x - _data[6] * center.y - _data[10] * center.z + (center.z - translation.z);
		_data[15] = (T) 1.0;
	}

	/*!
	Generate texture coordinates as linear functions so that:
	u = Ux*x + Uy*y + Uz*z + Uw
	v = Vx*x + Vy*y + Vz*z + Vw
	The matrix M for this case is:
	Ux  Vx  0  0
	Uy  Vy  0  0
	Uz  Vz  0  0
	Uw  Vw  0  0
	*/

	template <class T>
	inline Matrix4<T> &Matrix4<T>::buildTextureTransform(f32 rotateRad,
		const Vec2df &rotatecenter,
		const Vec2df &translate,
		const Vec2df &scale)
	{
		const f32 c = cosf(rotateRad);
		const f32 s = sinf(rotateRad);

		_data[0] = (T)(c * scale.x);
		_data[1] = (T)(s * scale.y);
		_data[2] = 0;
		_data[3] = 0;

		_data[4] = (T)(-s * scale.x);
		_data[5] = (T)(c * scale.y);
		_data[6] = 0;
		_data[7] = 0;

		_data[8] = (T)(c * scale.x * rotatecenter.x + -s * rotatecenter.y + translate.x);
		_data[9] = (T)(s * scale.y * rotatecenter.x + c * rotatecenter.y + translate.y);
		_data[10] = 1;
		_data[11] = 0;

		_data[12] = 0;
		_data[13] = 0;
		_data[14] = 0;
		_data[15] = 1;
	
		return *this;
	}


	// rotate about z axis, center (0.5, 0.5)
	template <class T>
	inline Matrix4<T> &Matrix4<T>::setTextureRotationCenter(f32 rotateRad)
	{
		const f32 c = cosf(rotateRad);
		const f32 s = sinf(rotateRad);
		_data[0] = (T)c;
		_data[1] = (T)s;

		_data[4] = (T)-s;
		_data[5] = (T)c;

		_data[8] = (T)(0.5f * (s - c) + 0.5f);
		_data[9] = (T)(-0.5f * (s + c) + 0.5f);
		return *this;
	}


	template <class T>
	inline Matrix4<T> &Matrix4<T>::setTextureTranslate(f32 x, f32 y)
	{
		_data[8] = (T)x;
		_data[9] = (T)y;
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setTextureTranslateTransposed(f32 x, f32 y)
	{
		_data[2] = (T)x;
		_data[6] = (T)y;
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setTextureScale(f32 sx, f32 sy)
	{
		_data[0] = (T)sx;
		_data[5] = (T)sy;
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setTextureScaleCenter(f32 sx, f32 sy)
	{
		_data[0] = (T)sx;
		_data[5] = (T)sy;
		_data[8] = (T)(0.5f - 0.5f * sx);
		_data[9] = (T)(0.5f - 0.5f * sy);
		return *this;
	}

	template <class T>
	inline Matrix4<T> &Matrix4<T>::setData(const T *data)
	{
		memcpy(_data, data, 16 * sizeof(T));
		return *this;
	}

	template <class T>
	inline bool Matrix4<T>::equals(const Matrix4<T> &other, const T tolerance) const
	{
		for (s32 i = 0; i < 16; ++i)
		if (!equals(_data[i], other._data[i], tolerance))
			return false;
		return true;
	}

	template <class T>
	inline Matrix4<T> operator*(const T scalar, const Matrix4<T> &mat)
	{
		return mat * scalar;
	}

	typedef Matrix4<f32> Matrix4f;
}

#endif