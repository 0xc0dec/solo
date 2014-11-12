#ifndef __SOLO_MATH_H__
#define __SOLO_MATH_H__

#include <cmath>
#include "SoloCommon.h"

namespace solo
{
	class AxisAlignedBox;
	class Degree;
	class Vector2;
	class Vector3;
	class Vector4;
	class Ray;
	class Sphere;
	class Plane;
	class Quaternion;
	class Matrix4;

	class Radian
	{
	public:
		explicit Radian(f32 r = 0) : _value(r)
		{
		}

		Radian(const Degree& d);

		Radian& operator =(const f32& f)
		{
			_value = f;
			return *this;
		}

		Radian& operator =(const Radian& r)
		{
			_value = r._value;
			return *this;
		}

		Radian& operator =(const Degree& d);

		f32 valueDegrees() const; // see bottom of this file
		f32 valueRadians() const
		{
			return _value;
		}

		f32 valueAngleUnits() const;

		const Radian& operator +() const
		{
			return *this;
		}

		Radian operator +(const Radian& r) const
		{
			return Radian(_value + r._value);
		}

		Radian operator +(const Degree& d) const;

		Radian& operator +=(const Radian& r)
		{
			_value += r._value;
			return *this;
		}

		Radian& operator +=(const Degree& d);

		Radian operator -() const
		{
			return Radian(-_value);
		}

		Radian operator -(const Radian& r) const
		{
			return Radian(_value - r._value);
		}

		Radian operator -(const Degree& d) const;

		Radian& operator -=(const Radian& r)
		{
			_value -= r._value;
			return *this;
		}

		Radian& operator -=(const Degree& d);

		Radian operator *(f32 f) const
		{
			return Radian(_value * f);
		}

		Radian operator *(const Radian& f) const
		{
			return Radian(_value * f._value);
		}

		Radian& operator *=(f32 f)
		{
			_value *= f;
			return *this;
		}

		Radian operator /(f32 f) const
		{
			return Radian(_value / f);
		}

		Radian& operator /=(f32 f)
		{
			_value /= f;
			return *this;
		}

		bool operator <(const Radian& r) const
		{
			return _value < r._value;
		}

		bool operator <=(const Radian& r) const
		{
			return _value <= r._value;
		}

		bool operator ==(const Radian& r) const
		{
			return _value == r._value;
		}

		bool operator !=(const Radian& r) const
		{
			return _value != r._value;
		}

		bool operator >=(const Radian& r) const
		{
			return _value >= r._value;
		}

		bool operator >(const Radian& r) const
		{
			return _value > r._value;
		}

		inline friend std::ostream& operator <<(std::ostream& o, const Radian& v)
		{
			o << "Radian(" << v.valueRadians() << ")";
			return o;
		}

	private:
		f32 _value;
	};

	class Degree
	{
	public:
		explicit Degree(f32 d = 0) : _value(d)
		{
		}

		Degree(const Radian& r) : _value(r.valueDegrees())
		{
		}

		Degree& operator =(const f32& f)
		{
			_value = f;
			return *this;
		}

		Degree& operator =(const Degree& d)
		{
			_value = d._value;
			return *this;
		}

		Degree& operator =(const Radian& r)
		{
			_value = r.valueDegrees();
			return *this;
		}

		f32 valueDegrees() const
		{
			return _value;
		}

		f32 valueRadians() const; // see bottom of this file
		f32 valueAngleUnits() const;

		const Degree& operator +() const
		{
			return *this;
		}

		Degree operator +(const Degree& d) const
		{
			return Degree(_value + d._value);
		}

		Degree operator +(const Radian& r) const
		{
			return Degree(_value + r.valueDegrees());
		}

		Degree& operator +=(const Degree& d)
		{
			_value += d._value;
			return *this;
		}

		Degree& operator +=(const Radian& r)
		{
			_value += r.valueDegrees();
			return *this;
		}

		Degree operator -() const
		{
			return Degree(-_value);
		}

		Degree operator -(const Degree& d) const
		{
			return Degree(_value - d._value);
		}

		Degree operator -(const Radian& r) const
		{
			return Degree(_value - r.valueDegrees());
		}

		Degree& operator -=(const Degree& d)
		{
			_value -= d._value;
			return *this;
		}

		Degree& operator -=(const Radian& r)
		{
			_value -= r.valueDegrees();
			return *this;
		}

		Degree operator *(f32 f) const
		{
			return Degree(_value * f);
		}

		Degree operator *(const Degree& f) const
		{
			return Degree(_value * f._value);
		}

		Degree& operator *=(f32 f)
		{
			_value *= f;
			return *this;
		}

		Degree operator /(f32 f) const
		{
			return Degree(_value / f);
		}

		Degree& operator /=(f32 f)
		{
			_value /= f;
			return *this;
		}

		bool operator <(const Degree& d) const
		{
			return _value < d._value;
		}

		bool operator <=(const Degree& d) const
		{
			return _value <= d._value;
		}

		bool operator ==(const Degree& d) const
		{
			return _value == d._value;
		}

		bool operator !=(const Degree& d) const
		{
			return _value != d._value;
		}

		bool operator >=(const Degree& d) const
		{
			return _value >= d._value;
		}

		bool operator >(const Degree& d) const
		{
			return _value > d._value;
		}

		inline friend std::ostream& operator <<(std::ostream& o, const Degree& v)
		{
			o << "Degree(" << v.valueDegrees() << ")";
			return o;
		}

	private:
		f32 _value;
	};


	class Angle
	{
	public:
		explicit Angle(f32 angle) : _value(angle)
		{
		}

		operator Radian() const;
		operator Degree() const;

	private:
		f32 _value;
	};

	// these functions could not be defined within the class definition of class
	// Radian because they required class Degree to be defined
	inline Radian::Radian(const Degree& d) : _value(d.valueRadians())
	{
	}

	inline Radian& Radian::operator =(const Degree& d)
	{
		_value = d.valueRadians();
		return *this;
	}

	inline Radian Radian::operator +(const Degree& d) const
	{
		return Radian(_value + d.valueRadians());
	}

	inline Radian& Radian::operator +=(const Degree& d)
	{
		_value += d.valueRadians();
		return *this;
	}

	inline Radian Radian::operator -(const Degree& d) const
	{
		return Radian(_value - d.valueRadians());
	}

	inline Radian& Radian::operator -=(const Degree& d)
	{
		_value -= d.valueRadians();
		return *this;
	}


	class Math
	{
	public:
		/** The angular units used by the API. This functionality is now deprecated in favor
		of discreet angular unit types ( see Degree and Radian above ). The only place
		this functionality is actually still used is when parsing files. Search for
		usage of the Angle class for those instances
		*/
		enum AngleUnit
		{
			AU_DEGREE,
			AU_RADIAN
		};

	protected:
		// angle units used by the api
		static AngleUnit msAngleUnit;

		/// Size of the trig tables as determined by constructor.
		static int mTrigTableSize;

		/// Radian -> index factor value ( mTrigTableSize / 2 * PI )
		static f32 mTrigTableFactor;
		static f32* mSinTable;
		static f32* mTanTable;

		/** Private function to build trig tables.
		*/
		void buildTrigTables();

		static f32 SinTable(f32 fValue);
		static f32 TanTable(f32 fValue);
	public:

		/** Default constructor.
		@param
		trigTableSize Optional parameter to set the size of the
		tables used to implement Sin, Cos, Tan
		*/
		Math(unsigned int trigTableSize = 4096);

		/** Default destructor.
		*/
		~Math();

		static inline int IAbs(int iValue)
		{
			return (iValue >= 0 ? iValue : -iValue);
		}

		static inline int ICeil(float fValue)
		{
			return int(ceil(fValue));
		}

		static inline int IFloor(float fValue)
		{
			return int(floor(fValue));
		}

		static int ISign(int iValue);

		/** Absolute value function
		@param
		fValue The value whose absolute value will be returned.
		*/
		static inline f32 Abs(f32 fValue)
		{
			return f32(fabs(fValue));
		}

		/** Absolute value function
		@param
		fValue The value, in degrees, whose absolute value will be returned.
		*/
		static inline Degree Abs(const Degree& dValue)
		{
			return Degree(fabs(dValue.valueDegrees()));
		}

		/** Absolute value function
		@param
		fValue The value, in radians, whose absolute value will be returned.
		*/
		static inline Radian Abs(const Radian& rValue)
		{
			return Radian(fabs(rValue.valueRadians()));
		}

		/** Arc cosine function
		@param
		fValue The value whose arc cosine will be returned.
		*/
		static Radian ACos(f32 fValue);

		/** Arc sine function
		@param
		fValue The value whose arc sine will be returned.
		*/
		static Radian ASin(f32 fValue);

		/** Arc tangent function
		@param
		fValue The value whose arc tangent will be returned.
		*/
		static inline Radian ATan(f32 fValue)
		{
			return Radian(atan(fValue));
		}

		/** Arc tangent between two values function
		@param
		fY The first value to calculate the arc tangent with.
		@param
		fX The second value to calculate the arc tangent with.
		*/
		static inline Radian ATan2(f32 fY, f32 fX)
		{
			return Radian(atan2(fY, fX));
		}

		/** Ceiling function
		Returns the smallest following integer. (example: Ceil(1.1) = 2)

		@param
		fValue The value to round up to the nearest integer.
		*/
		static inline f32 Ceil(f32 fValue)
		{
			return f32(ceil(fValue));
		}

		static inline bool isNaN(f32 f)
		{
			// std::isnan() is C99, not supported by all compilers
			// However NaN always fails this next test, no other number does.
			return f != f;
		}

		/** Cosine function.
		@param
		fValue Angle in radians
		@param
		useTables If true, uses lookup tables rather than
		calculation - faster but less accurate.
		*/
		static inline f32 Cos(const Radian& fValue, bool useTables = false)
		{
			return (!useTables) ? f32(cos(fValue.valueRadians())) : SinTable(fValue.valueRadians() + HALF_PI);
		}

		/** Cosine function.
		@param
		fValue Angle in radians
		@param
		useTables If true, uses lookup tables rather than
		calculation - faster but less accurate.
		*/
		static inline f32 Cos(f32 fValue, bool useTables = false)
		{
			return (!useTables) ? f32(cos(fValue)) : SinTable(fValue + HALF_PI);
		}

		static inline f32 Exp(f32 fValue)
		{
			return f32(exp(fValue));
		}

		/** Floor function
		Returns the largest previous integer. (example: Floor(1.9) = 1)

		@param
		fValue The value to round down to the nearest integer.
		*/
		static inline f32 Floor(f32 fValue)
		{
			return f32(floor(fValue));
		}

		static inline f32 Log(f32 fValue)
		{
			return f32(log(fValue));
		}

		/// Stored value of log(2) for frequent use
		static const f32 LOG2;

		static inline f32 Log2(f32 fValue)
		{
			return f32(log(fValue) / LOG2);
		}

		static inline f32 LogN(f32 base, f32 fValue)
		{
			return f32(log(fValue) / log(base));
		}

		static inline f32 Pow(f32 fBase, f32 fExponent)
		{
			return f32(pow(fBase, fExponent));
		}

		static f32 Sign(f32 fValue);

		static inline Radian Sign(const Radian& rValue)
		{
			return Radian(Sign(rValue.valueRadians()));
		}

		static inline Degree Sign(const Degree& dValue)
		{
			return Degree(Sign(dValue.valueDegrees()));
		}

		/** Sine function.
		@param
		fValue Angle in radians
		@param
		useTables If true, uses lookup tables rather than
		calculation - faster but less accurate.
		*/
		static inline f32 Sin(const Radian& fValue, bool useTables = false)
		{
			return (!useTables) ? f32(sin(fValue.valueRadians())) : SinTable(fValue.valueRadians());
		}

		/** Sine function.
		@param
		fValue Angle in radians
		@param
		useTables If true, uses lookup tables rather than
		calculation - faster but less accurate.
		*/
		static inline f32 Sin(f32 fValue, bool useTables = false)
		{
			return (!useTables) ? f32(sin(fValue)) : SinTable(fValue);
		}

		/** Squared function.
		@param
		fValue The value to be squared (fValue^2)
		*/
		static inline f32 Sqr(f32 fValue)
		{
			return fValue * fValue;
		}

		/** Square root function.
		@param
		fValue The value whose square root will be calculated.
		*/
		static inline f32 Sqrt(f32 fValue)
		{
			return f32(sqrt(fValue));
		}

		/** Square root function.
		@param
		fValue The value, in radians, whose square root will be calculated.
		@return
		The square root of the angle in radians.
		*/
		static inline Radian Sqrt(const Radian& fValue)
		{
			return Radian(sqrt(fValue.valueRadians()));
		}

		/** Square root function.
		@param
		fValue The value, in degrees, whose square root will be calculated.
		@return
		The square root of the angle in degrees.
		*/
		static inline Degree Sqrt(const Degree& fValue)
		{
			return Degree(sqrt(fValue.valueDegrees()));
		}

		/** Inverse square root i.e. 1 / Sqrt(x), good for vector
		normalisation.
		@param
		fValue The value whose inverse square root will be calculated.
		*/
		static f32 InvSqrt(f32 fValue);

		/** Generate a random number of unit length.
		@return
		A random number in the range from [0,1].
		*/
		static f32 UnitRandom();

		/** Generate a random number within the range provided.
		@param
		fLow The lower bound of the range.
		@param
		fHigh The upper bound of the range.
		@return
		A random number in the range from [fLow,fHigh].
		*/
		static f32 RangeRandom(f32 fLow, f32 fHigh);

		/** Generate a random number in the range [-1,1].
		@return
		A random number in the range from [-1,1].
		*/
		static f32 SymmetricRandom();

		/** Tangent function.
		@param
		fValue Angle in radians
		@param
		useTables If true, uses lookup tables rather than
		calculation - faster but less accurate.
		*/
		static inline f32 Tan(const Radian& fValue, bool useTables = false)
		{
			return (!useTables) ? f32(tan(fValue.valueRadians())) : TanTable(fValue.valueRadians());
		}

		/** Tangent function.
		@param
		fValue Angle in radians
		@param
		useTables If true, uses lookup tables rather than
		calculation - faster but less accurate.
		*/
		static inline f32 Tan(f32 fValue, bool useTables = false)
		{
			return (!useTables) ? f32(tan(fValue)) : TanTable(fValue);
		}

		static inline f32 DegreesToRadians(f32 degrees)
		{
			return degrees * fDeg2Rad;
		}

		static inline f32 RadiansToDegrees(f32 radians)
		{
			return radians * fRad2Deg;
		}

		/** These functions used to set the assumed angle units (radians or degrees)
		expected when using the Angle type.
		@par
		You can set this directly after creating a new Root, and also before/after resource creation,
		depending on whether you want the change to affect resource files.
		*/
		static void setAngleUnit(AngleUnit unit);
		/** Get the unit being used for angles. */
		static AngleUnit getAngleUnit(void);

		/** Convert from the current AngleUnit to radians. */
		static f32 AngleUnitsToRadians(f32 units);
		/** Convert from radians to the current AngleUnit . */
		static f32 RadiansToAngleUnits(f32 radians);
		/** Convert from the current AngleUnit to degrees. */
		static f32 AngleUnitsToDegrees(f32 units);
		/** Convert from degrees to the current AngleUnit. */
		static f32 DegreesToAngleUnits(f32 degrees);

		/** Checks whether a given point is inside a triangle, in a
		2-dimensional (Cartesian) space.
		@remarks
		The vertices of the triangle must be given in either
		trigonometrical (anticlockwise) or inverse trigonometrical
		(clockwise) order.
		@param
		p The point.
		@param
		a The triangle's first vertex.
		@param
		b The triangle's second vertex.
		@param
		c The triangle's third vertex.
		@return
		If the point resides in the triangle, <b>true</b> is
		returned.
		@par
		If the point is outside the triangle, <b>false</b> is
		returned.
		*/
		static bool pointInTri2D(const Vector2& p, const Vector2& a, const Vector2& b, const Vector2& c);

		/** Checks whether a given 3D point is inside a triangle.
		@remarks
		The vertices of the triangle must be given in either
		trigonometrical (anticlockwise) or inverse trigonometrical
		(clockwise) order, and the point must be guaranteed to be in the
		same plane as the triangle
		@param
		p The point.
		@param
		a The triangle's first vertex.
		@param
		b The triangle's second vertex.
		@param
		c The triangle's third vertex.
		@param
		normal The triangle plane's normal (passed in rather than calculated
		on demand since the caller may already have it)
		@return
		If the point resides in the triangle, <b>true</b> is
		returned.
		@par
		If the point is outside the triangle, <b>false</b> is
		returned.
		*/
		static bool pointInTri3D(const Vector3& p, const Vector3& a,
								const Vector3& b, const Vector3& c, const Vector3& normal);
		/** Ray / plane intersection, returns boolean result and distance. */
		static Pair<bool, f32> intersects(const Ray& ray, const Plane& plane);

		/** Ray / sphere intersection, returns boolean result and distance. */
		static Pair<bool, f32> intersects(const Ray& ray, const Sphere& sphere,
												bool discardInside = true);

		/** Ray / box intersection, returns boolean result and distance. */
		static Pair<bool, f32> intersects(const Ray& ray, const AxisAlignedBox& box);

		/** Ray / box intersection, returns boolean result and two intersection distance.
		@param
		ray The ray.
		@param
		box The box.
		@param
		d1 A f32 pointer to retrieve the near intersection distance
		from the ray origin, maybe <b>null</b> which means don't care
		about the near intersection distance.
		@param
		d2 A f32 pointer to retrieve the far intersection distance
		from the ray origin, maybe <b>null</b> which means don't care
		about the far intersection distance.
		@return
		If the ray is intersects the box, <b>true</b> is returned, and
		the near intersection distance is return by <i>d1</i>, the
		far intersection distance is return by <i>d2</i>. Guarantee
		<b>0</b> <= <i>d1</i> <= <i>d2</i>.
		@par
		If the ray isn't intersects the box, <b>false</b> is returned, and
		<i>d1</i> and <i>d2</i> is unmodified.
		*/
		static bool intersects(const Ray& ray, const AxisAlignedBox& box, f32* d1, f32* d2);

		/** Ray / triangle intersection, returns boolean result and distance.
		@param
		ray The ray.
		@param
		a The triangle's first vertex.
		@param
		b The triangle's second vertex.
		@param
		c The triangle's third vertex.
		@param
		normal The triangle plane's normal (passed in rather than calculated
		on demand since the caller may already have it), doesn't need
		normalised since we don't care.
		@param
		positiveSide Intersect with "positive side" of the triangle
		@param
		negativeSide Intersect with "negative side" of the triangle
		@return
		If the ray is intersects the triangle, a pair of <b>true</b> and the
		distance between intersection point and ray origin returned.
		@par
		If the ray isn't intersects the triangle, a pair of <b>false</b> and
		<b>0</b> returned.
		*/
		static Pair<bool, f32> intersects(const Ray& ray, const Vector3& a,
												const Vector3& b, const Vector3& c, const Vector3& normal,
												bool positiveSide = true, bool negativeSide = true);

		/** Ray / triangle intersection, returns boolean result and distance.
		@param
		ray The ray.
		@param
		a The triangle's first vertex.
		@param
		b The triangle's second vertex.
		@param
		c The triangle's third vertex.
		@param
		positiveSide Intersect with "positive side" of the triangle
		@param
		negativeSide Intersect with "negative side" of the triangle
		@return
		If the ray is intersects the triangle, a pair of <b>true</b> and the
		distance between intersection point and ray origin returned.
		@par
		If the ray isn't intersects the triangle, a pair of <b>false</b> and
		<b>0</b> returned.
		*/
		static Pair<bool, f32> intersects(const Ray& ray, const Vector3& a,
												const Vector3& b, const Vector3& c,
												bool positiveSide = true, bool negativeSide = true);

		/** Sphere / box intersection test. */
		static bool intersects(const Sphere& sphere, const AxisAlignedBox& box);

		/** Plane / box intersection test. */
		static bool intersects(const Plane& plane, const AxisAlignedBox& box);

		/** Ray / convex plane list intersection test.
		@param ray The ray to test with
		@param plaeList List of planes which form a convex volume
		@param normalIsOutside Does the normal point outside the volume
		*/
		static Pair<bool, f32> intersects(const Ray& ray, const Array<Plane>& planeList, bool normalIsOutside);
		/** Ray / convex plane list intersection test.
		@param ray The ray to test with
		@param plaeList List of planes which form a convex volume
		@param normalIsOutside Does the normal point outside the volume
		*/
		static Pair<bool, f32> intersects(const Ray& ray, const List<Plane>& planeList, bool normalIsOutside);

		/** Sphere / plane intersection test.
		@remarks NB just do a plane.getDistance(sphere.getCenter()) for more detail!
		*/
		static bool intersects(const Sphere& sphere, const Plane& plane);

		/** Compare 2 f32s, using tolerance for inaccuracies.
		*/
		static bool f32Equal(f32 a, f32 b, f32 tolerance = std::numeric_limits<f32>::epsilon());

		/** Calculates the tangent space vector for a given set of positions / texture coords. */
		static Vector3 calculateTangentSpaceVector(const Vector3& position1, const Vector3& position2, const Vector3& position3,
			f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3);

		/** Build a reflection matrix for the passed in plane. */
		static Matrix4 buildReflectionMatrix(const Plane& p);
		/** Calculate a face normal, including the w component which is the offset from the origin. */
		static Vector4 calculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/** Calculate a face normal, no w-information. */
		static Vector3 calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/** Calculate a face normal without normalize, including the w component which is the offset from the origin. */
		static Vector4 calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/** Calculate a face normal without normalize, no w-information. */
		static Vector3 calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);

		/** Generates a value based on the Gaussian (normal) distribution function
		with the given offset and scale parameters.
		*/
		static f32 gaussianDistribution(f32 x, f32 offset = 0.0f, f32 scale = 1.0f);

		/** Clamp a value within an inclusive range. */
		template <typename T>
		static T Clamp(T val, T minval, T maxval)
		{
			assert(minval <= maxval && "Invalid clamp range");
			return std::max(std::min(val, maxval), minval);
		}

		static Matrix4 makeViewMatrix(const Vector3& position, const Quaternion& orientation, const Matrix4* reflectMatrix = 0);

		/** Get a bounding radius value from a bounding box. */
		static f32 boundingRadiusFromAABB(const AxisAlignedBox& aabb);

		static const f32 POS_INFINITY;
		static const f32 NEG_INFINITY;
		static const f32 PI;
		static const f32 TWO_PI;
		static const f32 HALF_PI;
		static const f32 fDeg2Rad;
		static const f32 fRad2Deg;
	};

	inline f32 Radian::valueDegrees() const
	{
		return Math::RadiansToDegrees(_value);
	}

	inline f32 Radian::valueAngleUnits() const
	{
		return Math::RadiansToAngleUnits(_value);
	}

	inline f32 Degree::valueRadians() const
	{
		return Math::DegreesToRadians(_value);
	}

	inline f32 Degree::valueAngleUnits() const
	{
		return Math::DegreesToAngleUnits(_value);
	}

	inline Angle::operator Radian() const
	{
		return Radian(Math::AngleUnitsToRadians(_value));
	}

	inline Angle::operator Degree() const
	{
		return Degree(Math::AngleUnitsToDegrees(_value));
	}

	inline Radian operator * (f32 a, const Radian& b)
	{
		return Radian(a * b.valueRadians());
	}

	inline Radian operator / (f32 a, const Radian& b)
	{
		return Radian(a / b.valueRadians());
	}

	inline Degree operator * (f32 a, const Degree& b)
	{
		return Degree(a * b.valueDegrees());
	}

	inline Degree operator / (f32 a, const Degree& b)
	{
		return Degree(a / b.valueDegrees());
	}
}

#endif