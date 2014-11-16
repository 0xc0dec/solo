#include "SoloMath.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloPlane.h"
#include "SoloRay.h"
#include "SoloSphere.h"
#include "SoloMatrix4.h"


using namespace solo;

const f32 Math::POS_INFINITY = std::numeric_limits<f32>::infinity();
const f32 Math::NEG_INFINITY = -std::numeric_limits<f32>::infinity();
const f32 Math::PI = f32(4.0 * std::atan(1.0));
const f32 Math::TWO_PI = f32(2.0 * PI);
const f32 Math::HALF_PI = f32(0.5 * PI);
const f32 Math::deg2Rad = PI / f32(180.0);
const f32 Math::rad2Deg = f32(180.0) / PI;
const f32 Math::LOG2 = log(f32(2.0));

u32 Math::_trigTableSize;
Math::AngleUnit Math::_angleUnit;

f32 Math::_trigTableFactor;
f32* Math::_sinTable = nullptr;
f32* Math::_tanTable = nullptr;


Math::Math(unsigned int trigTableSize)
{
	_angleUnit = AngleUnit::Degree;
	_trigTableSize = trigTableSize;
	_trigTableFactor = _trigTableSize / TWO_PI;

	_sinTable = new f32[_trigTableSize];
	_tanTable = new f32[_trigTableSize];

	_buildTrigTables();
}


Math::~Math()
{
	delete[] _sinTable;
	delete[] _tanTable;
}


void Math::_buildTrigTables(void)
{
	// Build trig lookup tables
	// Could get away with building only PI sized Sin table but simpler this 
	// way. Who cares, it'll ony use an extra 8k of memory anyway and I like 
	// simplicity.
	f32 angle;
	for (u32 i = 0; i < _trigTableSize; ++i)
	{
		angle = TWO_PI * i / _trigTableSize;
		_sinTable[i] = sin(angle);
		_tanTable[i] = tan(angle);
	}
}


f32 Math::_fromSinTable(f32 val)
{
	// Convert range to index values, wrap if required
	int idx;
	if (val >= 0)
	{
		idx = int(val * _trigTableFactor) % _trigTableSize;
	}
	else
	{
		idx = _trigTableSize - (int(-val * _trigTableFactor) % _trigTableSize) - 1;
	}

	return _sinTable[idx];
}


f32 Math::_fromTanTable(f32 val)
{
	// Convert range to index values, wrap if required
	int idx = int(val *= _trigTableFactor) % _trigTableSize;
	return _tanTable[idx];
}


int Math::sign(int val)
{
	return (val > 0 ? +1 : (val < 0 ? -1 : 0));
}


Radian Math::acos(f32 val)
{
	if (-1.0 < val)
	{
		if (val < 1.0)
			return Radian(acos(val));
		return Radian(0.0);
	}
	return Radian(PI);
}


Radian Math::asin(f32 val)
{
	if (-1.0 < val)
	{
		if (val < 1.0)
			return Radian(asin(val));
		return Radian(HALF_PI);
	}
	return Radian(-HALF_PI);
}


f32 Math::sign(f32 val)
{
	if (val > 0.0)
		return 1.0;

	if (val < 0.0)
		return -1.0;

	return 0.0;
}


f32 Math::invSqrt(f32 val)
{
	return f32(1. / sqrt(val));
}


f32 Math::randomUnit()
{
	return rand() / f32(RAND_MAX);
}


f32 Math::randomRange(f32 low, f32 high)
{
	return (high - low) * randomUnit() + low;
}


f32 Math::randomSymmetric()
{
	return 2.0f * randomUnit() - 1.0f;
}


void Math::setAngleUnit(AngleUnit unit)
{
	_angleUnit = unit;
}


Math::AngleUnit Math::angleUnit(void)
{
	return _angleUnit;
}


f32 Math::angleUnitsToRadians(f32 angleunits)
{
	if (_angleUnit == AngleUnit::Degree)
		return angleunits * deg2Rad;
	return angleunits;
}


f32 Math::radiansToAngleUnits(f32 radians)
{
	if (_angleUnit == AngleUnit::Degree)
		return radians * rad2Deg;
	return radians;
}


f32 Math::angleUnitsToDegrees(f32 angleunits)
{
	if (_angleUnit == AngleUnit::Degree)
		return angleunits * rad2Deg;
	return angleunits;
}


f32 Math::degreesToAngleUnits(f32 degrees)
{
	if (_angleUnit == AngleUnit::Degree)
		return degrees * deg2Rad;
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
	zeroDot[0] = f32Equal(dot[0], 0.0f, 1e-3);

	v1 = c - b;
	v2 = p - b;

	dot[1] = v1.crossProduct(v2);
	zeroDot[1] = f32Equal(dot[1], 0.0f, 1e-3);

	// Compare signs (ignore colinear / coincident points)
	if (!zeroDot[0] && !zeroDot[1] && sign(dot[0]) != sign(dot[1]))
		return false;

	v1 = a - c;
	v2 = p - c;

	dot[2] = v1.crossProduct(v2);
	zeroDot[2] = f32Equal(dot[2], 0.0f, 1e-3);
	// Compare signs (ignore colinear / coincident points)
	if ((!zeroDot[0] && !zeroDot[2] && sign(dot[0]) != sign(dot[2])) ||
		(!zeroDot[1] && !zeroDot[2] && sign(dot[1]) != sign(dot[2])))
	{
		return false;
	}


	return true;
}


bool Math::pointInTri3D(const Vector3& p, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& normal)
{
	// Winding must be consistent from all edges for point to be inside
	Vector3 v1, v2;
	f32 dot[3];
	bool zeroDot[3];

	v1 = b - a;
	v2 = p - a;

	// Note we don't care about normalisation here since sign is all we need
	// It means we don't have to worry about magnitude of cross products either
	dot[0] = v1.cross(v2).dot(normal);
	zeroDot[0] = f32Equal(dot[0], 0.0f, 1e-3);

	v1 = c - b;
	v2 = p - b;

	dot[1] = v1.cross(v2).dot(normal);
	zeroDot[1] = f32Equal(dot[1], 0.0f, 1e-3);

	// Compare signs (ignore colinear / coincident points)
	if (!zeroDot[0] && !zeroDot[1] && sign(dot[0]) != sign(dot[1]))
		return false;

	v1 = a - c;
	v2 = p - c;

	dot[2] = v1.cross(v2).dot(normal);
	zeroDot[2] = f32Equal(dot[2], 0.0f, 1e-3);
	// Compare signs (ignore colinear / coincident points)
	if ((!zeroDot[0] && !zeroDot[2] && sign(dot[0]) != sign(dot[2])) ||
		(!zeroDot[1] && !zeroDot[2] && sign(dot[1]) != sign(dot[2])))
	{
		return false;
	}


	return true;
}


bool Math::f32Equal(f32 a, f32 b, f32 tolerance)
{
	if (fabs(b - a) <= tolerance)
		return true;
	return false;
}


Pair<bool, f32> Math::intersects(const Ray& ray, const Plane& plane)
{
	f32 denom = plane.normal.dot(ray.getDirection());
	if (abs(denom) < std::numeric_limits<f32>::epsilon())
	{
		// Parallel
		return Pair<bool, f32>(false, 0.0f);
	}
	f32 nom = plane.normal.dot(ray.getOrigin()) + plane.d;
	f32 t = -(nom / denom);
	return Pair<bool, f32>(t >= 0, t);
}


Pair<bool, f32> Math::intersects(const Ray& ray, const Array<Plane>& planes, bool normalIsOutside)
{
	List<Plane> planesList;
	for (auto i = planes.cbegin(); i != planes.cend(); ++i)
		planesList.push_back(*i);
	return intersects(ray, planesList, normalIsOutside);
}


Pair<bool, f32> Math::intersects(const Ray& ray, const List<Plane>& planes, bool normalIsOutside)
{
	List<Plane>::const_iterator planeit, planeitend;
	planeitend = planes.end();
	bool allInside = true;
	Pair<bool, f32> ret;
	Pair<bool, f32> end;
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
			Pair<bool, f32> planeRes = ray.intersects(plane);
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
			Pair<bool, f32> planeRes = ray.intersects(plane);
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


Pair<bool, f32> Math::intersects(const Ray& ray, const Sphere& sphere, bool discardInside)
{
	const Vector3& raydir = ray.getDirection();
	// Adjust ray origin relative to sphere center
	const Vector3& rayorig = ray.getOrigin() - sphere.getCenter();
	f32 radius = sphere.getRadius();

	// Check origin inside first
	if (rayorig.squaredLength() <= radius * radius && discardInside)
	{
		return Pair<bool, f32>(true, 0.0f);
	}

	// Mmm, quadratics
	// Build coeffs which can be used with std quadratic solver
	// ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
	f32 a = raydir.dot(raydir);
	f32 b = 2 * rayorig.dot(raydir);
	f32 c = rayorig.dot(rayorig) - radius * radius;

	// Calc determinant
	f32 d = (b * b) - (4 * a * c);
	if (d < 0)
	{
		// No intersection
		return Pair<bool, f32>(false, 0.0f);
	}

	// BTW, if d=0 there is one intersection, if d > 0 there are 2
	// But we only want the closest one, so that's ok, just use the 
	// '-' version of the solver
	f32 t = (-b - sqrt(d)) / (2 * a);
	if (t < 0)
		t = (-b + sqrt(d)) / (2 * a);
	return Pair<bool, f32>(true, t);
}


Pair<bool, f32> Math::intersects(const Ray& ray, const AxisAlignedBox& box)
{
	if (box.isNull()) return Pair<bool, f32>(false, 0.0f);
	if (box.isInfinite()) return Pair<bool, f32>(true, 0.0f);

	f32 lowt = 0.0f;
	f32 t;
	bool hit = false;
	Vector3 hitpoint;
	const Vector3& min = box.minimum();
	const Vector3& max = box.maximum();
	const Vector3& rayorig = ray.getOrigin();
	const Vector3& raydir = ray.getDirection();

	// Check origin inside first
	if (rayorig > min && rayorig < max)
	{
		return Pair<bool, f32>(true, 0.0f);
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

	return Pair<bool, f32>(hit, lowt);
}


bool Math::intersects(const Ray& ray, const AxisAlignedBox& box, f32* d1, f32* d2)
{
	if (box.isNull())
		return false;

	if (box.isInfinite())
	{
		if (d1) *d1 = 0;
		if (d2) *d2 = POS_INFINITY;
		return true;
	}

	const Vector3& min = box.minimum();
	const Vector3& max = box.maximum();
	const Vector3& rayorig = ray.getOrigin();
	const Vector3& raydir = ray.getDirection();

	Vector3 absDir;
	absDir[0] = abs(raydir[0]);
	absDir[1] = abs(raydir[1]);
	absDir[2] = abs(raydir[2]);

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

	f32 start = 0, end = POS_INFINITY;

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

Pair<bool, f32> Math::intersects(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& normal,
								bool positiveSide, bool negativeSide)
{
	//
	// Calculate intersection with plane.
	//
	f32 t;
	{
		f32 denom = normal.dot(ray.getDirection());

		// Check intersect side
		if (denom > +std::numeric_limits<f32>::epsilon())
		{
			if (!negativeSide)
				return Pair<bool, f32>(false, 0.0f);
		}
		else if (denom < -std::numeric_limits<f32>::epsilon())
		{
			if (!positiveSide)
				return Pair<bool, f32>(false, 0.0f);
		}
		else
		{
			// Parallel or triangle area is close to zero when
			// the plane normal not normalised.
			return Pair<bool, f32>(false, 0.0f);
		}

		t = normal.dot(a - ray.getOrigin()) / denom;

		if (t < 0)
		{
			// Intersection is behind origin
			return Pair<bool, f32>(false, 0.0f);
		}
	}

	//
	// Calculate the largest area projection plane in X, Y or Z.
	//
	size_t i0, i1;
	{
		f32 n0 = abs(normal[0]);
		f32 n1 = abs(normal[1]);
		f32 n2 = abs(normal[2]);

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
				return Pair<bool, f32>(false, 0.0f);
		}
		else
		{
			if (alpha > tolerance || beta > tolerance || alpha + beta < area - tolerance)
				return Pair<bool, f32>(false, 0.0f);
		}
	}

	return Pair<bool, f32>(true, t);
}

Pair<bool, f32> Math::intersects(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, bool positiveSide, bool negativeSide)
{
	Vector3 normal = calculateFaceNormalBasicWithoutNormalize(a, b, c);
	return intersects(ray, a, b, c, normal, positiveSide, negativeSide);
}

bool Math::intersects(const Sphere& sphere, const AxisAlignedBox& box)
{
	if (box.isNull()) return false;
	if (box.isInfinite()) return true;

	// Use splitting planes
	const Vector3& center = sphere.getCenter();
	f32 radius = sphere.getRadius();
	const Vector3& min = box.minimum();
	const Vector3& max = box.maximum();

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
	return (abs(plane.getDistance(sphere.getCenter())) <= sphere.getRadius());
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
	Vector3 normal = side1.cross(side0);
	normal.normalize();
	//Now we use a formula to calculate the tangent. 
	f32 deltaV0 = v1 - v2;
	f32 deltaV1 = v3 - v1;
	Vector3 tangent = deltaV1 * side0 - deltaV0 * side1;
	tangent.normalize();
	//Calculate binormal
	f32 deltaU0 = u1 - u2;
	f32 deltaU1 = u3 - u1;
	Vector3 binormal = deltaU1 * side0 - deltaU0 * side1;
	binormal.normalize();
	//Now, we take the cross product of the tangents to get a vector which 
	//should point in the same direction as our normal calculated above. 
	//If it points in the opposite direction (the dot product between the normals is less than zero), 
	//then we need to reverse the s and t tangents. 
	//This is because the triangle has been mirrored when going from tangent space to object space.
	//reverse tangents if necessary
	Vector3 tangentCross = tangent.cross(binormal);
	if (tangentCross.dot(normal) < 0.0f)
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
	Vector3 normal = calculateFaceNormalBasic(v1, v2, v3);
	// Now set up the w (distance of tri from origin
	return Vector4(normal.x, normal.y, normal.z, -(normal.dot(v1)));
}

Vector3 Math::calculateFaceNormalBasic(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	Vector3 normal = (v2 - v1).cross(v3 - v1);
	normal.normalize();
	return normal;
}

Vector4 Math::calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	Vector3 normal = calculateFaceNormalBasicWithoutNormalize(v1, v2, v3);
	// Now set up the w (distance of tri from origin)
	return Vector4(normal.x, normal.y, normal.z, -(normal.dot(v1)));
}


Vector3 Math::calculateFaceNormalBasicWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	Vector3 normal = (v2 - v1).cross(v3 - v1);
	return normal;
}


f32 Math::gaussianDistribution(f32 x, f32 offset, f32 scale)
{
	f32 nom = exp(-sqr(x - offset) / (2 * sqr(scale)));
	f32 denom = scale * sqrt(2 * PI);
	return nom / denom;
}


Matrix4 Math::makeViewMatrix(const Vector3& position, const Quaternion& orientation, const Matrix4* reflectMatrix)
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
	orientation.toRotationMatrix(rot);

	// Make the translation relative to new axes
	Matrix3 rotT = rot.transpose();
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
	Vector3 max = aabb.maximum();
	Vector3 min = aabb.minimum();

	Vector3 magnitude = max;
	magnitude.makeCeil(-max);
	magnitude.makeCeil(min);
	magnitude.makeCeil(-min);

	return magnitude.length();
}