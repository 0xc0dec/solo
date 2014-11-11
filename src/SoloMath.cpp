#include "SoloMath.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloPlane.h"
#include "SoloRay.h"

using namespace solo;

const f32 Math::POS_INFINITY = std::numeric_limits<f32>::infinity();
const f32 Math::NEG_INFINITY = -std::numeric_limits<f32>::infinity();
const f32 Math::PI = f32(4.0 * atan(1.0));
const f32 Math::TWO_PI = f32(2.0 * PI);
const f32 Math::HALF_PI = f32(0.5 * PI);
const f32 Math::fDeg2Rad = PI / f32(180.0);
const f32 Math::fRad2Deg = f32(180.0) / PI;
const f32 Math::LOG2 = log(f32(2.0));

int Math::mTrigTableSize;
Math::AngleUnit Math::msAngleUnit;

f32 Math::mTrigTableFactor;
f32* Math::mSinTable = nullptr;
f32* Math::mTanTable = nullptr;


Math::Math(unsigned int trigTableSize)
{
	msAngleUnit = AU_DEGREE;
	mTrigTableSize = trigTableSize;
	mTrigTableFactor = mTrigTableSize / Math::TWO_PI;

	mSinTable = new f32[mTrigTableSize];
	mTanTable = new f32[mTrigTableSize];

	buildTrigTables();
}


Math::~Math()
{
	delete[] mSinTable;
	delete[] mTanTable;
}


void Math::buildTrigTables(void)
{
	// Build trig lookup tables
	// Could get away with building only PI sized Sin table but simpler this 
	// way. Who cares, it'll ony use an extra 8k of memory anyway and I like 
	// simplicity.
	f32 angle;
	for (int i = 0; i < mTrigTableSize; ++i)
	{
		angle = Math::TWO_PI * i / mTrigTableSize;
		mSinTable[i] = sin(angle);
		mTanTable[i] = tan(angle);
	}
}


f32 Math::SinTable(f32 fValue)
{
	// Convert range to index values, wrap if required
	int idx;
	if (fValue >= 0)
	{
		idx = int(fValue * mTrigTableFactor) % mTrigTableSize;
	}
	else
	{
		idx = mTrigTableSize - (int(-fValue * mTrigTableFactor) % mTrigTableSize) - 1;
	}

	return mSinTable[idx];
}


f32 Math::TanTable(f32 fValue)
{
	// Convert range to index values, wrap if required
	int idx = int(fValue *= mTrigTableFactor) % mTrigTableSize;
	return mTanTable[idx];
}


int Math::ISign(int iValue)
{
	return (iValue > 0 ? +1 : (iValue < 0 ? -1 : 0));
}


Radian Math::ACos(f32 fValue)
{
	if (-1.0 < fValue)
	{
		if (fValue < 1.0)
			return Radian(acos(fValue));
		return Radian(0.0);
	}
	return Radian(PI);
}


Radian Math::ASin(f32 fValue)
{
	if (-1.0 < fValue)
	{
		if (fValue < 1.0)
			return Radian(asin(fValue));
		return Radian(HALF_PI);
	}
	return Radian(-HALF_PI);
}


f32 Math::Sign(f32 fValue)
{
	if (fValue > 0.0)
		return 1.0;

	if (fValue < 0.0)
		return -1.0;

	return 0.0;
}


f32 Math::InvSqrt(f32 fValue)
{
	return f32(1. / sqrt(fValue));
}


f32 Math::UnitRandom()
{
	return rand() / f32(RAND_MAX);
}


f32 Math::RangeRandom(f32 fLow, f32 fHigh)
{
	return (fHigh - fLow) * UnitRandom() + fLow;
}


f32 Math::SymmetricRandom()
{
	return 2.0f * UnitRandom() - 1.0f;
}


void Math::setAngleUnit(Math::AngleUnit unit)
{
	msAngleUnit = unit;
}


Math::AngleUnit Math::getAngleUnit(void)
{
	return msAngleUnit;
}


f32 Math::AngleUnitsToRadians(f32 angleunits)
{
	if (msAngleUnit == AU_DEGREE)
		return angleunits * fDeg2Rad;
	else
		return angleunits;
}


f32 Math::RadiansToAngleUnits(f32 radians)
{
	if (msAngleUnit == AU_DEGREE)
		return radians * fRad2Deg;
	else
		return radians;
}


f32 Math::AngleUnitsToDegrees(f32 angleunits)
{
	if (msAngleUnit == AU_RADIAN)
		return angleunits * fRad2Deg;
	else
		return angleunits;
}


f32 Math::DegreesToAngleUnits(f32 degrees)
{
	if (msAngleUnit == AU_RADIAN)
		return degrees * fDeg2Rad;
	else
		return degrees;
}


bool Math::pointInTri2D(const Vector2& p, const Vector2& a, const Vector2& b, const Vector2& c)
{
	// Winding must be consistent from all edges for point to be inside
	Vector2 v1, v2;
	f32 dot[3];
	bool zeroDot[3];

	v1 = b - a;
	v2 = p - a;

	// Note we don't care about normalisation here since sign is all we need
	// It means we don't have to worry about magnitude of cross products either
	dot[0] = v1.crossProduct(v2);
	zeroDot[0] = Math::f32Equal(dot[0], 0.0f, 1e-3);


	v1 = c - b;
	v2 = p - b;

	dot[1] = v1.crossProduct(v2);
	zeroDot[1] = Math::f32Equal(dot[1], 0.0f, 1e-3);

	// Compare signs (ignore colinear / coincident points)
	if (!zeroDot[0] && !zeroDot[1]
		&& Math::Sign(dot[0]) != Math::Sign(dot[1]))
	{
		return false;
	}

	v1 = a - c;
	v2 = p - c;

	dot[2] = v1.crossProduct(v2);
	zeroDot[2] = Math::f32Equal(dot[2], 0.0f, 1e-3);
	// Compare signs (ignore colinear / coincident points)
	if ((!zeroDot[0] && !zeroDot[2]
			&& Math::Sign(dot[0]) != Math::Sign(dot[2])) ||
		(!zeroDot[1] && !zeroDot[2]
			&& Math::Sign(dot[1]) != Math::Sign(dot[2])))
	{
		return false;
	}


	return true;
}


bool Math::pointInTri3D(const Vector3& p, const Vector3& a,
						const Vector3& b, const Vector3& c, const Vector3& normal)
{
	// Winding must be consistent from all edges for point to be inside
	Vector3 v1, v2;
	f32 dot[3];
	bool zeroDot[3];

	v1 = b - a;
	v2 = p - a;

	// Note we don't care about normalisation here since sign is all we need
	// It means we don't have to worry about magnitude of cross products either
	dot[0] = v1.crossProduct(v2).dotProduct(normal);
	zeroDot[0] = Math::f32Equal(dot[0], 0.0f, 1e-3);


	v1 = c - b;
	v2 = p - b;

	dot[1] = v1.crossProduct(v2).dotProduct(normal);
	zeroDot[1] = Math::f32Equal(dot[1], 0.0f, 1e-3);

	// Compare signs (ignore colinear / coincident points)
	if (!zeroDot[0] && !zeroDot[1]
		&& Math::Sign(dot[0]) != Math::Sign(dot[1]))
	{
		return false;
	}

	v1 = a - c;
	v2 = p - c;

	dot[2] = v1.crossProduct(v2).dotProduct(normal);
	zeroDot[2] = Math::f32Equal(dot[2], 0.0f, 1e-3);
	// Compare signs (ignore colinear / coincident points)
	if ((!zeroDot[0] && !zeroDot[2]
			&& Math::Sign(dot[0]) != Math::Sign(dot[2])) ||
		(!zeroDot[1] && !zeroDot[2]
			&& Math::Sign(dot[1]) != Math::Sign(dot[2])))
	{
		return false;
	}


	return true;
}


bool Math::f32Equal(f32 a, f32 b, f32 tolerance)
{
	if (fabs(b - a) <= tolerance)
		return true;
	else
		return false;
}


std::pair<bool, f32> Math::intersects(const Ray& ray, const Plane& plane)
{
	f32 denom = plane.normal.dotProduct(ray.getDirection());
	if (Math::Abs(denom) < std::numeric_limits<f32>::epsilon())
	{
		// Parallel
		return std::pair<bool, f32>(false, 0);
	}
	else
	{
		f32 nom = plane.normal.dotProduct(ray.getOrigin()) + plane.d;
		f32 t = -(nom / denom);
		return std::pair<bool, f32>(t >= 0, t);
	}
}


std::pair<bool, f32> Math::intersects(const Ray& ray, const std::vector<Plane>& planes, bool normalIsOutside)
{
	std::list<Plane> planesList;
	for (auto i = planes.cbegin(); i != planes.cend(); ++i)
		planesList.push_back(*i);
	return intersects(ray, planesList, normalIsOutside);
}


std::pair<bool, f32> Math::intersects(const Ray& ray, const std::list<Plane>& planes, bool normalIsOutside)
{
	std::list<Plane>::const_iterator planeit, planeitend;
	planeitend = planes.end();
	bool allInside = true;
	std::pair<bool, f32> ret;
	std::pair<bool, f32> end;
	ret.first = false;
	ret.second = 0.0f;
	end.first = false;
	end.second = 0;

	// derive side
	// NB we don't pass directly since that would require Plane::Side in 
	// interface, which results in recursive includes since Math is so fundamental
	Plane::Side outside = normalIsOutside ? Plane::POSITIVE_SIDE : Plane::NEGATIVE_SIDE;

	for (planeit = planes.begin(); planeit != planeitend; ++planeit)
	{
		const Plane& plane = *planeit;
		// is origin outside?
		if (plane.getSide(ray.getOrigin()) == outside)
		{
			allInside = false;
			// Test single plane
			std::pair<bool, f32> planeRes =
				ray.intersects(plane);
			if (planeRes.first)
			{
				// Ok, we intersected
				ret.first = true;
				// Use the most distant result since convex volume
				ret.second = std::max(ret.second, planeRes.second);
			}
			else
			{
				ret.first = false;
				ret.second = 0.0f;
				return ret;
			}
		}
		else
		{
			std::pair<bool, f32> planeRes =
				ray.intersects(plane);
			if (planeRes.first)
			{
				if (!end.first)
				{
					end.first = true;
					end.second = planeRes.second;
				}
				else
				{
					end.second = std::min(planeRes.second, end.second);
				}
			}
		}
	}

	if (allInside)
	{
		// Intersecting at 0 distance since inside the volume!
		ret.first = true;
		ret.second = 0.0f;
		return ret;
	}

	if (end.first)
	{
		if (end.second < ret.second)
		{
			ret.first = false;
			return ret;
		}
	}
	return ret;
}


std::pair<bool, f32> Math::intersects(const Ray& ray, const Sphere& sphere,
									bool discardInside)
{
	const Vector3& raydir = ray.getDirection();
	// Adjust ray origin relative to sphere center
	const Vector3& rayorig = ray.getOrigin() - sphere.getCenter();
	f32 radius = sphere.getRadius();

	// Check origin inside first
	if (rayorig.squaredLength() <= radius * radius && discardInside)
	{
		return std::pair<bool, f32>(true, 0);
	}

	// Mmm, quadratics
	// Build coeffs which can be used with std quadratic solver
	// ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
	f32 a = raydir.dotProduct(raydir);
	f32 b = 2 * rayorig.dotProduct(raydir);
	f32 c = rayorig.dotProduct(rayorig) - radius * radius;

	// Calc determinant
	f32 d = (b * b) - (4 * a * c);
	if (d < 0)
	{
		// No intersection
		return std::pair<bool, f32>(false, 0);
	}

	// BTW, if d=0 there is one intersection, if d > 0 there are 2
	// But we only want the closest one, so that's ok, just use the 
	// '-' version of the solver
	f32 t = (-b - Math::Sqrt(d)) / (2 * a);
	if (t < 0)
		t = (-b + Math::Sqrt(d)) / (2 * a);
	return std::pair<bool, f32>(true, t);
}


std::pair<bool, f32> Math::intersects(const Ray& ray, const AxisAlignedBox& box)
{
	if (box.isNull()) return std::pair<bool, f32>(false, 0);
	if (box.isInfinite()) return std::pair<bool, f32>(true, 0);

	f32 lowt = 0.0f;
	f32 t;
	bool hit = false;
	Vector3 hitpoint;
	const Vector3& min = box.getMinimum();
	const Vector3& max = box.getMaximum();
	const Vector3& rayorig = ray.getOrigin();
	const Vector3& raydir = ray.getDirection();

	// Check origin inside first
	if (rayorig > min && rayorig < max)
	{
		return std::pair<bool, f32>(true, 0);
	}

	// Check each face in turn, only check closest 3
	// Min x
	if (rayorig.x <= min.x && raydir.x > 0)
	{
		t = (min.x - rayorig.x) / raydir.x;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max x
	if (rayorig.x >= max.x && raydir.x < 0)
	{
		t = (max.x - rayorig.x) / raydir.x;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Min y
	if (rayorig.y <= min.y && raydir.y > 0)
	{
		t = (min.y - rayorig.y) / raydir.y;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max y
	if (rayorig.y >= max.y && raydir.y < 0)
	{
		t = (max.y - rayorig.y) / raydir.y;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Min z
	if (rayorig.z <= min.z && raydir.z > 0)
	{
		t = (min.z - rayorig.z) / raydir.z;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.y >= min.y && hitpoint.y <= max.y &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max z
	if (rayorig.z >= max.z && raydir.z < 0)
	{
		t = (max.z - rayorig.z) / raydir.z;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.y >= min.y && hitpoint.y <= max.y &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}

	return std::pair<bool, f32>(hit, lowt);
}


bool Math::intersects(const Ray& ray, const AxisAlignedBox& box, f32* d1, f32* d2)
{
	if (box.isNull())
		return false;

	if (box.isInfinite())
	{
		if (d1) *d1 = 0;
		if (d2) *d2 = Math::POS_INFINITY;
		return true;
	}

	const Vector3& min = box.getMinimum();
	const Vector3& max = box.getMaximum();
	const Vector3& rayorig = ray.getOrigin();
	const Vector3& raydir = ray.getDirection();

	Vector3 absDir;
	absDir[0] = Math::Abs(raydir[0]);
	absDir[1] = Math::Abs(raydir[1]);
	absDir[2] = Math::Abs(raydir[2]);

	// Sort the axis, ensure check minimise floating error axis first
	int imax = 0, imid = 1, imin = 2;
	if (absDir[0] < absDir[2])
	{
		imax = 2;
		imin = 0;
	}
	if (absDir[1] < absDir[imin])
	{
		imid = imin;
		imin = 1;
	}
	else if (absDir[1] > absDir[imax])
	{
		imid = imax;
		imax = 1;
	}

	f32 start = 0, end = Math::POS_INFINITY;

#define _CALC_AXIS(i) \
	do { \
		f32 denom = 1 / raydir[i]; \
		f32 newstart = (min[i] - rayorig[i]) * denom; \
		f32 newend = (max[i] - rayorig[i]) * denom; \
		if (newstart > newend) std::swap(newstart, newend); \
		if (newstart > end || newend < start) return false; \
		if (newstart > start) start = newstart; \
		if (newend < end) end = newend; \
	} while (0)

	// Check each axis in turn
	_CALC_AXIS(imax);

	if (absDir[imid] < std::numeric_limits<f32>::epsilon())
	{
		// Parallel with middle and minimise axis, check bounds only
		if (rayorig[imid] < min[imid] || rayorig[imid] > max[imid] ||
			rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
			return false;
	}
	else
	{
		_CALC_AXIS(imid);

		if (absDir[imin] < std::numeric_limits<f32>::epsilon())
		{
			// Parallel with minimise axis, check bounds only
			if (rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
				return false;
		}
		else
		{
			_CALC_AXIS(imin);
		}
	}
#undef _CALC_AXIS

	if (d1) *d1 = start;
	if (d2) *d2 = end;

	return true;
}

std::pair<bool, f32> Math::intersects(const Ray& ray, const Vector3& a,
									const Vector3& b, const Vector3& c, const Vector3& normal,
									bool positiveSide, bool negativeSide)
{
	//
	// Calculate intersection with plane.
	//
	f32 t;
	{
		f32 denom = normal.dotProduct(ray.getDirection());

		// Check intersect side
		if (denom > +std::numeric_limits<f32>::epsilon())
		{
			if (!negativeSide)
				return std::pair<bool, f32>(false, 0);
		}
		else if (denom < -std::numeric_limits<f32>::epsilon())
		{
			if (!positiveSide)
				return std::pair<bool, f32>(false, 0);
		}
		else
		{
			// Parallel or triangle area is close to zero when
			// the plane normal not normalised.
			return std::pair<bool, f32>(false, 0);
		}

		t = normal.dotProduct(a - ray.getOrigin()) / denom;

		if (t < 0)
		{
			// Intersection is behind origin
			return std::pair<bool, f32>(false, 0);
		}
	}

	//
	// Calculate the largest area projection plane in X, Y or Z.
	//
	size_t i0, i1;
	{
		f32 n0 = Math::Abs(normal[0]);
		f32 n1 = Math::Abs(normal[1]);
		f32 n2 = Math::Abs(normal[2]);

		i0 = 1;
		i1 = 2;
		if (n1 > n2)
		{
			if (n1 > n0) i0 = 0;
		}
		else
		{
			if (n2 > n0) i1 = 0;
		}
	}

	//
	// Check the intersection point is inside the triangle.
	//
	{
		f32 u1 = b[i0] - a[i0];
		f32 v1 = b[i1] - a[i1];
		f32 u2 = c[i0] - a[i0];
		f32 v2 = c[i1] - a[i1];
		f32 u0 = t * ray.getDirection()[i0] + ray.getOrigin()[i0] - a[i0];
		f32 v0 = t * ray.getDirection()[i1] + ray.getOrigin()[i1] - a[i1];

		f32 alpha = u0 * v2 - u2 * v0;
		f32 beta = u1 * v0 - u0 * v1;
		f32 area = u1 * v2 - u2 * v1;

		// epsilon to avoid float precision error
		const f32 EPSILON = 1e-6f;

		f32 tolerance = -EPSILON * area;

		if (area > 0)
		{
			if (alpha < tolerance || beta < tolerance || alpha + beta > area - tolerance)
				return std::pair<bool, f32>(false, 0);
		}
		else
		{
			if (alpha > tolerance || beta > tolerance || alpha + beta < area - tolerance)
				return std::pair<bool, f32>(false, 0);
		}
	}

	return std::pair<bool, f32>(true, t);
}

std::pair<bool, f32> Math::intersects(const Ray& ray, const Vector3& a,
									const Vector3& b, const Vector3& c,
									bool positiveSide, bool negativeSide)
{
	Vector3 normal = calculateBasicFaceNormalWithoutNormalize(a, b, c);
	return intersects(ray, a, b, c, normal, positiveSide, negativeSide);
}

bool Math::intersects(const Sphere& sphere, const AxisAlignedBox& box)
{
	if (box.isNull()) return false;
	if (box.isInfinite()) return true;

	// Use splitting planes
	const Vector3& center = sphere.getCenter();
	f32 radius = sphere.getRadius();
	const Vector3& min = box.getMinimum();
	const Vector3& max = box.getMaximum();

	// Arvo's algorithm
	f32 s, d = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (center.ptr()[i] < min.ptr()[i])
		{
			s = center.ptr()[i] - min.ptr()[i];
			d += s * s;
		}
		else if (center.ptr()[i] > max.ptr()[i])
		{
			s = center.ptr()[i] - max.ptr()[i];
			d += s * s;
		}
	}
	return d <= radius * radius;
}

bool Math::intersects(const Plane& plane, const AxisAlignedBox& box)
{
	return (plane.getSide(box) == Plane::BOTH_SIDE);
}

bool Math::intersects(const Sphere& sphere, const Plane& plane)
{
	return (
		Math::Abs(plane.getDistance(sphere.getCenter()))
		<= sphere.getRadius());
}

Vector3 Math::calculateTangentSpaceVector(
	const Vector3& position1, const Vector3& position2, const Vector3& position3,
	f32 u1, f32 v1, f32 u2, f32 v2, f32 u3, f32 v3)
{
	//side0 is the vector along one side of the triangle of vertices passed in, 
	//and side1 is the vector along another side. Taking the cross product of these returns the normal.
	Vector3 side0 = position1 - position2;
	Vector3 side1 = position3 - position1;
	//Calculate face normal
	Vector3 normal = side1.crossProduct(side0);
	normal.normalise();
	//Now we use a formula to calculate the tangent. 
	f32 deltaV0 = v1 - v2;
	f32 deltaV1 = v3 - v1;
	Vector3 tangent = deltaV1 * side0 - deltaV0 * side1;
	tangent.normalise();
	//Calculate binormal
	f32 deltaU0 = u1 - u2;
	f32 deltaU1 = u3 - u1;
	Vector3 binormal = deltaU1 * side0 - deltaU0 * side1;
	binormal.normalise();
	//Now, we take the cross product of the tangents to get a vector which 
	//should point in the same direction as our normal calculated above. 
	//If it points in the opposite direction (the dot product between the normals is less than zero), 
	//then we need to reverse the s and t tangents. 
	//This is because the triangle has been mirrored when going from tangent space to object space.
	//reverse tangents if necessary
	Vector3 tangentCross = tangent.crossProduct(binormal);
	if (tangentCross.dotProduct(normal) < 0.0f)
	{
		tangent = -tangent;
		binormal = -binormal;
	}

	return tangent;
}

Matrix4 Math::buildReflectionMatrix(const Plane& p)
{
	return Matrix4(
		-2 * p.normal.x * p.normal.x + 1, -2 * p.normal.x * p.normal.y, -2 * p.normal.x * p.normal.z, -2 * p.normal.x * p.d,
		-2 * p.normal.y * p.normal.x, -2 * p.normal.y * p.normal.y + 1, -2 * p.normal.y * p.normal.z, -2 * p.normal.y * p.d,
		-2 * p.normal.z * p.normal.x, -2 * p.normal.z * p.normal.y, -2 * p.normal.z * p.normal.z + 1, -2 * p.normal.z * p.d,
		0, 0, 0, 1);
}

Vector4 Math::calculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	Vector3 normal = calculateBasicFaceNormal(v1, v2, v3);
	// Now set up the w (distance of tri from origin
	return Vector4(normal.x, normal.y, normal.z, -(normal.dotProduct(v1)));
}

Vector3 Math::calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
	normal.normalise();
	return normal;
}

Vector4 Math::calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	Vector3 normal = calculateBasicFaceNormalWithoutNormalize(v1, v2, v3);
	// Now set up the w (distance of tri from origin)
	return Vector4(normal.x, normal.y, normal.z, -(normal.dotProduct(v1)));
}


Vector3 Math::calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
	return normal;
}


f32 Math::gaussianDistribution(f32 x, f32 offset, f32 scale)
{
	f32 nom = Math::Exp(
		-Math::Sqr(x - offset) / (2 * Math::Sqr(scale)));
	f32 denom = scale * Math::Sqrt(2 * Math::PI);

	return nom / denom;
}


Matrix4 Math::makeViewMatrix(const Vector3& position, const Quaternion& orientation,
							const Matrix4* reflectMatrix)
{
	Matrix4 viewMatrix;

	// View matrix is:
	//
	//  [ Lx  Uy  Dz  Tx  ]
	//  [ Lx  Uy  Dz  Ty  ]
	//  [ Lx  Uy  Dz  Tz  ]
	//  [ 0   0   0   1   ]
	//
	// Where T = -(Transposed(Rot) * Pos)

	// This is most efficiently done using 3x3 Matrices
	Matrix3 rot;
	orientation.ToRotationMatrix(rot);

	// Make the translation relative to new axes
	Matrix3 rotT = rot.Transpose();
	Vector3 trans = -rotT * position;

	// Make final matrix
	viewMatrix = Matrix4::IDENTITY;
	viewMatrix = rotT; // fills upper 3x3
	viewMatrix[0][3] = trans.x;
	viewMatrix[1][3] = trans.y;
	viewMatrix[2][3] = trans.z;

	// Deal with reflections
	if (reflectMatrix)
	{
		viewMatrix = viewMatrix * (*reflectMatrix);
	}

	return viewMatrix;
}


f32 Math::boundingRadiusFromAABB(const AxisAlignedBox& aabb)
{
	Vector3 max = aabb.getMaximum();
	Vector3 min = aabb.getMinimum();

	Vector3 magnitude = max;
	magnitude.makeCeil(-max);
	magnitude.makeCeil(min);
	magnitude.makeCeil(-min);

	return magnitude.length();
}