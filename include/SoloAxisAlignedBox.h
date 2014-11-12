#ifndef __SOLO_AXIS_ALIGNED_BOX_H__
#define __SOLO_AXIS_ALIGNED_BOX_H__

#include "SoloCommon.h"
#include "SoloMath.h"
#include "SoloVector3.h"
#include "SoloMatrix4.h"

namespace solo
{
	class AxisAlignedBox
	{
	public:
		enum Extent
		{
			EXTENT_NULL,
			EXTENT_FINITE,
			EXTENT_INFINITE
		};

	protected:
		Vector3 _min;
		Vector3 _max;
		Extent _extent;
		mutable Vector3* _corners;

	public:
		/*
		   1-----2
		  /|    /|
		 / |   / |
		5-----4  |
		|  0--|--3
		| /   | /
		|/    |/
		6-----7
		*/
		typedef enum
		{
			FAR_LEFT_BOTTOM = 0,
			FAR_LEFT_TOP = 1,
			FAR_RIGHT_TOP = 2,
			FAR_RIGHT_BOTTOM = 3,
			NEAR_RIGHT_BOTTOM = 7,
			NEAR_LEFT_BOTTOM = 6,
			NEAR_LEFT_TOP = 5,
			NEAR_RIGHT_TOP = 4
		} CornerEnum;

		inline AxisAlignedBox() : _min(Vector3::ZERO), _max(Vector3::UNIT_SCALE), _corners(nullptr)
		{
			// Default to a null box 
			setMinimum(-0.5, -0.5, -0.5);
			setMaximum(0.5, 0.5, 0.5);
			_extent = EXTENT_NULL;
		}

		inline AxisAlignedBox(Extent e)
			: _min(Vector3::ZERO), _max(Vector3::UNIT_SCALE), _corners(nullptr)
		{
			setMinimum(-0.5, -0.5, -0.5);
			setMaximum(0.5, 0.5, 0.5);
			_extent = e;
		}

		inline AxisAlignedBox(const AxisAlignedBox& box)
			: _min(Vector3::ZERO), _max(Vector3::UNIT_SCALE), _corners(nullptr)
		{
			if (box.isNull())
				setNull();
			else if (box.isInfinite())
				setInfinite();
			else
				setExtents(box._min, box._max);
		}

		inline AxisAlignedBox(const Vector3& min, const Vector3& max)
			: _min(Vector3::ZERO), _max(Vector3::UNIT_SCALE), _corners(nullptr)
		{
			setExtents(min, max);
		}

		inline AxisAlignedBox(f32 mx, f32 my, f32 mz, f32 Mx, f32 My, f32 Mz)
			: _min(Vector3::ZERO), _max(Vector3::UNIT_SCALE), _corners(nullptr)
		{
			setExtents(mx, my, mz, Mx, My, Mz);
		}

		AxisAlignedBox& operator=(const AxisAlignedBox& other)
		{
			// Specifically override to avoid copying _corners
			if (other.isNull())
				setNull();
			else if (other.isInfinite())
				setInfinite();
			else
				setExtents(other._min, other._max);

			return *this;
		}

		~AxisAlignedBox()
		{
			if (_corners)
				delete _corners;
		}

		inline const Vector3& minimum(void) const
		{
			return _min;
		}

		inline Vector3& minimum(void)
		{
			return _min;
		}

		inline const Vector3& maximum(void) const
		{
			return _max;
		}

		inline Vector3& maximum(void)
		{
			return _max;
		}

		inline void setMinimum(const Vector3& vec)
		{
			_extent = EXTENT_FINITE;
			_min = vec;
		}

		inline void setMinimum(f32 x, f32 y, f32 z)
		{
			_extent = EXTENT_FINITE;
			_min.x = x;
			_min.y = y;
			_min.z = z;
		}

		inline void setMinimumX(f32 x)
		{
			_min.x = x;
		}

		inline void setMinimumY(f32 y)
		{
			_min.y = y;
		}

		inline void setMinimumZ(f32 z)
		{
			_min.z = z;
		}

		inline void setMaximum(const Vector3& vec)
		{
			_extent = EXTENT_FINITE;
			_max = vec;
		}

		inline void setMaximum(f32 x, f32 y, f32 z)
		{
			_extent = EXTENT_FINITE;
			_max.x = x;
			_max.y = y;
			_max.z = z;
		}

		inline void setMaximumX(f32 x)
		{
			_max.x = x;
		}

		inline void setMaximumY(f32 y)
		{
			_max.y = y;
		}

		inline void setMaximumZ(f32 z)
		{
			_max.z = z;
		}

		inline void setExtents(const Vector3& min, const Vector3& max)
		{
			assert((min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
				"The minimum corner of the box must be less than or equal to maximum corner");

			_extent = EXTENT_FINITE;
			_min = min;
			_max = max;
		}

		inline void setExtents(f32 mx, f32 my, f32 mz, f32 Mx, f32 My, f32 Mz)
		{
			assert((mx <= Mx && my <= My && mz <= Mz) &&
				"The minimum corner of the box must be less than or equal to maximum corner");

			_extent = EXTENT_FINITE;

			_min.x = mx;
			_min.y = my;
			_min.z = mz;

			_max.x = Mx;
			_max.y = My;
			_max.z = Mz;
		}

		/** Returns a pointer to an array of 8 corner points, useful for
		collision vs. non-aligned objects.
		@remarks
		If the order of these corners is important, they are as
		follows: The 4 points of the minimum Z face (note that
		because Ogre uses right-handed coordinates, the minimum Z is
		at the 'back' of the box) starting with the minimum point of
		all, then anticlockwise around this face (if you are looking
		onto the face from outside the box). Then the 4 points of the
		maximum Z face, starting with maximum point of all, then
		anticlockwise around this face (looking onto the face from
		outside the box). Like this:
		<pre>
		   1-----2
		  /|    /|
		 / |   / |
		5-----4  |
		|  0--|--3
		| /   | /
		|/    |/
		6-----7
		</pre>
		@remarks as this implementation uses a static member, make sure to use your own copy !
		*/
		inline const Vector3* allCorners(void) const
		{
			assert((_extent == EXTENT_FINITE) && "Can't get corners of a null or infinite AAB");

			// The order of these items is, using right-handed co-ordinates:
			// Minimum Z face, starting with Min(all), then anticlockwise
			//   around face (looking onto the face)
			// Maximum Z face, starting with Max(all), then anticlockwise
			//   around face (looking onto the face)
			// Only for optimization/compatibility.
			if (!_corners)
				_corners = new Vector3();

			_corners[0] = _min;
			_corners[1].x = _min.x;
			_corners[1].y = _max.y;
			_corners[1].z = _min.z;
			_corners[2].x = _max.x;
			_corners[2].y = _max.y;
			_corners[2].z = _min.z;
			_corners[3].x = _max.x;
			_corners[3].y = _min.y;
			_corners[3].z = _min.z;

			_corners[4] = _max;
			_corners[5].x = _min.x;
			_corners[5].y = _max.y;
			_corners[5].z = _max.z;
			_corners[6].x = _min.x;
			_corners[6].y = _min.y;
			_corners[6].z = _max.z;
			_corners[7].x = _max.x;
			_corners[7].y = _min.y;
			_corners[7].z = _max.z;

			return _corners;
		}

		Vector3 corner(CornerEnum cornerToGet) const
		{
			switch (cornerToGet)
			{
			case FAR_LEFT_BOTTOM:
				return _min;
			case FAR_LEFT_TOP:
				return Vector3(_min.x, _max.y, _min.z);
			case FAR_RIGHT_TOP:
				return Vector3(_max.x, _max.y, _min.z);
			case FAR_RIGHT_BOTTOM:
				return Vector3(_max.x, _min.y, _min.z);
			case NEAR_RIGHT_BOTTOM:
				return Vector3(_max.x, _min.y, _max.z);
			case NEAR_LEFT_BOTTOM:
				return Vector3(_min.x, _min.y, _max.z);
			case NEAR_LEFT_TOP:
				return Vector3(_min.x, _max.y, _max.z);
			case NEAR_RIGHT_TOP:
				return _max;
			default:
				return Vector3();
			}
		}

		friend std::ostream& operator<<(std::ostream& o, const AxisAlignedBox aab)
		{
			switch (aab._extent)
			{
			case EXTENT_NULL:
				o << "AxisAlignedBox(null)";
				return o;

			case EXTENT_FINITE:
				o << "AxisAlignedBox(min=" << aab._min << ", max=" << aab._max << ")";
				return o;

			case EXTENT_INFINITE:
				o << "AxisAlignedBox(infinite)";
				return o;

			default: // shut up compiler
				assert(false && "Never reached");
				return o;
			}
		}

		void merge(const AxisAlignedBox& other)
		{
			// Do nothing if rhs null, or this is infinite
			if ((other._extent == EXTENT_NULL) || (_extent == EXTENT_INFINITE))
				return;
			// Otherwise if other is infinite, make this infinite, too
			if (other._extent == EXTENT_INFINITE)
				_extent = EXTENT_INFINITE;
			// Otherwise if current null, just take other
			else if (_extent == EXTENT_NULL)
				setExtents(other._min, other._max);
			// Otherwise merge
			else
			{
				Vector3 min = _min;
				Vector3 max = _max;
				max.makeCeil(other._max);
				min.makeFloor(other._min);

				setExtents(min, max);
			}
		}

		inline void merge(const Vector3& point)
		{
			switch (_extent)
			{
			case EXTENT_NULL: // if null, use this point
				setExtents(point, point);
				return;

			case EXTENT_FINITE:
				_max.makeCeil(point);
				_min.makeFloor(point);
				return;

			case EXTENT_INFINITE: // if infinite, makes no difference
				return;
			}

			assert(false && "Never reached");
		}

		/** Transforms the box according to the matrix supplied.
		@remarks
		By calling this method you get the axis-aligned box which
		surrounds the transformed version of this box. Therefore each
		corner of the box is transformed by the matrix, then the
		extents are mapped back onto the axes to produce another
		AABB. Useful when you have a local AABB for an object which
		is then transformed.
		*/
		inline void transform(const Matrix4& matrix)
		{
			// Do nothing if current null or infinite
			if (_extent != EXTENT_FINITE)
				return;

			Vector3 oldMin, oldMax, currentCorner;

			// Getting the old values so that we can use the existing merge method.
			oldMin = _min;
			oldMax = _max;

			// reset
			setNull();

			// We sequentially compute the corners in the following order :
			// 0, 6, 5, 1, 2, 4 ,7 , 3
			// This sequence allows us to only change one member at a time to get at all corners.

			// For each one, we transform it using the matrix
			// Which gives the resulting point and merge the resulting point.

			// First corner 
			// min min min
			currentCorner = oldMin;
			merge(matrix * currentCorner);

			// min,min,max
			currentCorner.z = oldMax.z;
			merge(matrix * currentCorner);

			// min max max
			currentCorner.y = oldMax.y;
			merge(matrix * currentCorner);

			// min max min
			currentCorner.z = oldMin.z;
			merge(matrix * currentCorner);

			// max max min
			currentCorner.x = oldMax.x;
			merge(matrix * currentCorner);

			// max max max
			currentCorner.z = oldMax.z;
			merge(matrix * currentCorner);

			// max min max
			currentCorner.y = oldMin.y;
			merge(matrix * currentCorner);

			// max min min
			currentCorner.z = oldMin.z;
			merge(matrix * currentCorner);
		}

		/** Transforms the box according to the affine matrix supplied.
		@remarks
		By calling this method you get the axis-aligned box which
		surrounds the transformed version of this box. Therefore each
		corner of the box is transformed by the matrix, then the
		extents are mapped back onto the axes to produce another
		AABB. Useful when you have a local AABB for an object which
		is then transformed.
		@note
		The matrix must be an affine matrix. @see Matrix4::isAffine.
		*/
		void transformAffine(const Matrix4& m)
		{
			assert(m.isAffine());

			// Do nothing if current null or infinite
			if (_extent != EXTENT_FINITE)
				return;

			Vector3 centre = center();
			Vector3 halfSize = this->halfSize();

			Vector3 newCentre = m.transformAffine(centre);
			Vector3 newHalfSize(
				Math::Abs(m[0][0]) * halfSize.x + Math::Abs(m[0][1]) * halfSize.y + Math::Abs(m[0][2]) * halfSize.z,
				Math::Abs(m[1][0]) * halfSize.x + Math::Abs(m[1][1]) * halfSize.y + Math::Abs(m[1][2]) * halfSize.z,
				Math::Abs(m[2][0]) * halfSize.x + Math::Abs(m[2][1]) * halfSize.y + Math::Abs(m[2][2]) * halfSize.z);

			setExtents(newCentre - newHalfSize, newCentre + newHalfSize);
		}

		inline void setNull()
		{
			_extent = EXTENT_NULL;
		}

		inline bool isNull(void) const
		{
			return (_extent == EXTENT_NULL);
		}

		bool isFinite(void) const
		{
			return (_extent == EXTENT_FINITE);
		}

		inline void setInfinite()
		{
			_extent = EXTENT_INFINITE;
		}

		bool isInfinite(void) const
		{
			return (_extent == EXTENT_INFINITE);
		}

		inline bool intersects(const AxisAlignedBox& b2) const
		{
			// Early-fail for nulls
			if (this->isNull() || b2.isNull())
				return false;

			// Early-success for infinites
			if (this->isInfinite() || b2.isInfinite())
				return true;

			// Use up to 6 separating planes
			if (_max.x < b2._min.x)
				return false;
			if (_max.y < b2._min.y)
				return false;
			if (_max.z < b2._min.z)
				return false;

			if (_min.x > b2._max.x)
				return false;
			if (_min.y > b2._max.y)
				return false;
			if (_min.z > b2._max.z)
				return false;

			// otherwise, must be intersecting
			return true;
		}

		inline AxisAlignedBox intersection(const AxisAlignedBox& b2) const
		{
			if (this->isNull() || b2.isNull())
			{
				return AxisAlignedBox();
			}
			if (this->isInfinite())
			{
				return b2;
			}
			if (b2.isInfinite())
			{
				return *this;
			}

			Vector3 intMin = _min;
			Vector3 intMax = _max;

			intMin.makeCeil(b2.minimum());
			intMax.makeFloor(b2.maximum());

			// Check intersection isn't null
			if (intMin.x < intMax.x &&
				intMin.y < intMax.y &&
				intMin.z < intMax.z)
			{
				return AxisAlignedBox(intMin, intMax);
			}

			return AxisAlignedBox();
		}

		f32 volume(void) const
		{
			switch (_extent)
			{
			case EXTENT_NULL:
				return 0.0f;

			case EXTENT_FINITE:
				{
					Vector3 diff = _max - _min;
					return diff.x * diff.y * diff.z;
				}

			case EXTENT_INFINITE:
				return Math::POS_INFINITY;

			default: // shut up compiler
				assert(false && "Never reached");
				return 0.0f;
			}
		}

		inline void scale(const Vector3& s)
		{
			// Do nothing if current null or infinite
			if (_extent != EXTENT_FINITE)
				return;

			// NB assumes centered on origin
			Vector3 min = _min * s;
			Vector3 max = _max * s;
			setExtents(min, max);
		}

		bool intersects(const Sphere& s) const
		{
			return Math::intersects(s, *this);
		}

		bool intersects(const Plane& p) const
		{
			return Math::intersects(p, *this);
		}

		bool intersects(const Vector3& v) const
		{
			switch (_extent)
			{
			case EXTENT_NULL:
				return false;

			case EXTENT_FINITE:
				return(v.x >= _min.x && v.x <= _max.x &&
					v.y >= _min.y && v.y <= _max.y &&
					v.z >= _min.z && v.z <= _max.z);

			case EXTENT_INFINITE:
				return true;

			default: // shut up compiler
				assert(false && "Never reached");
				return false;
			}
		}

		Vector3 center(void) const
		{
			assert((_extent == EXTENT_FINITE) && "Can't get center of a null or infinite AAB");

			return Vector3(
				(_max.x + _min.x) * 0.5f,
				(_max.y + _min.y) * 0.5f,
				(_max.z + _min.z) * 0.5f);
		}

		Vector3 size(void) const
		{
			switch (_extent)
			{
			case EXTENT_NULL:
				return Vector3::ZERO;

			case EXTENT_FINITE:
				return _max - _min;

			case EXTENT_INFINITE:
				return Vector3(
					Math::POS_INFINITY,
					Math::POS_INFINITY,
					Math::POS_INFINITY);

			default: // shut up compiler
				assert(false && "Never reached");
				return Vector3::ZERO;
			}
		}

		Vector3 halfSize(void) const
		{
			switch (_extent)
			{
			case EXTENT_NULL:
				return Vector3::ZERO;

			case EXTENT_FINITE:
				return (_max - _min) * 0.5;

			case EXTENT_INFINITE:
				return Vector3(
					Math::POS_INFINITY,
					Math::POS_INFINITY,
					Math::POS_INFINITY);

			default: // shut up compiler
				assert(false && "Never reached");
				return Vector3::ZERO;
			}
		}

		bool contains(const Vector3& v) const
		{
			if (isNull())
				return false;
			if (isInfinite())
				return true;

			return _min.x <= v.x && v.x <= _max.x &&
				_min.y <= v.y && v.y <= _max.y &&
				_min.z <= v.z && v.z <= _max.z;
		}

		f32 distance(const Vector3& v) const
		{
			if (this->contains(v))
				return 0;

			f32 maxDist = std::numeric_limits<f32>::min();

			if (v.x < _min.x)
				maxDist = std::max(maxDist, _min.x - v.x);
			if (v.y < _min.y)
				maxDist = std::max(maxDist, _min.y - v.y);
			if (v.z < _min.z)
				maxDist = std::max(maxDist, _min.z - v.z);

			if (v.x > _max.x)
				maxDist = std::max(maxDist, v.x - _max.x);
			if (v.y > _max.y)
				maxDist = std::max(maxDist, v.y - _max.y);
			if (v.z > _max.z)
				maxDist = std::max(maxDist, v.z - _max.z);

			return maxDist;
		}

		bool contains(const AxisAlignedBox& other) const
		{
			if (other.isNull() || this->isInfinite())
				return true;

			if (this->isNull() || other.isInfinite())
				return false;

			return this->_min.x <= other._min.x &&
				this->_min.y <= other._min.y &&
				this->_min.z <= other._min.z &&
				other._max.x <= this->_max.x &&
				other._max.y <= this->_max.y &&
				other._max.z <= this->_max.z;
		}

		bool operator==(const AxisAlignedBox& rhs) const
		{
			if (this->_extent != rhs._extent)
				return false;

			if (!this->isFinite())
				return true;

			return this->_min == rhs._min &&
				this->_max == rhs._max;
		}

		bool operator!=(const AxisAlignedBox& rhs) const
		{
			return !(*this == rhs);
		}

		static const AxisAlignedBox BOX_NULL;
		static const AxisAlignedBox BOX_INFINITE;
	};
}

#endif