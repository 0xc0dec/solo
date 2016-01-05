#include "SoloQuaternion.h"
#include "SoloMath.h"

using namespace solo;


Quaternion::Quaternion(float x, float y, float z, float w):
	x(x), y(y), z(z), w(w)
{
}


Quaternion::Quaternion(const Vector3& axis, float angleRadians)
{
	*this = createFromAxisAngle(axis, angleRadians);
}


Quaternion Quaternion::identity()
{
	static Quaternion value(0.0f, 0.0f, 0.0f, 1.0f);
	return value;
}


Quaternion Quaternion::zero()
{
	static Quaternion value(0.0f, 0.0f, 0.0f, 0.0f);
	return value;
}


bool Quaternion::isIdentity() const
{
	return x == 0.0f && y == 0.0f && z == 0.0f && w == 1.0f;
}


bool Quaternion::isZero() const
{
	return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
}


Quaternion Quaternion::createFromRotationMatrix(const Matrix& m)
{
	return m.getRotation(); // TODO remove
}


Quaternion Quaternion::createFromAxisAngle(const Vector3& axis, float angleRadians)
{
	auto halfAngle = angleRadians * 0.5f;
	auto sinHalfAngle = sinf(halfAngle);
	auto normal(const_cast<Vector3&>(axis));
	normal.normalize();
	return Quaternion(normal.x * sinHalfAngle, normal.y * sinHalfAngle, normal.z * sinHalfAngle, cosf(halfAngle));
}


void Quaternion::conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}


bool Quaternion::inverse()
{
	auto n = x * x + y * y + z * z + w * w;
	if (n == 1.0f)
	{
		x = -x;
		y = -y;
		z = -z;
		return true;
	}

	// Too close to zero
	if (n < Math::EPSILON)
		return false;

	n = 1.0f / n;
	x = -x * n;
	y = -y * n;
	z = -z * n;
	w = w * n;

	return true;
}


void Quaternion::normalize()
{
	auto n = x * x + y * y + z * z + w * w;

	// Already normalized
	if (n == 1.0f)
		return;

	n = sqrt(n);
	// Too close to zero
	if (n < Math::EPSILON)
		return;

	n = 1.0f / n;
	x *= n;
	y *= n;
	z *= n;
	w *= n;
}


Quaternion Quaternion::normalized() const
{
	auto result(*this);
	result.normalize();
	return result;
}


float Quaternion::toAxisAngle(Vector3& axis) const
{
	Quaternion q(x, y, z, w);
	q.normalize();
	axis.x = q.x;
	axis.y = q.y;
	axis.z = q.z;
	axis.normalize();
	return 2.0f * acos(q.w);
}


Quaternion Quaternion::lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	if (t == 0.0f)
		return q1;

	if (t == 1.0f)
		return q2;

	auto t1 = 1.0f - t;

	return Quaternion(
		t1 * q1.x + t * q2.x,
		t1 * q1.y + t * q2.y,
		t1 * q1.z + t * q2.z,
		t1 * q1.w + t * q2.w
	);
}


Quaternion Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	if (t == 0.0f)
		return q1;

	if (t == 1.0f)
		return q2;

	if (q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w)
		return q1;

	float halfY, alpha, beta;
	float u, f1, f2a, f2b;
	float ratio1, ratio2;
	float halfSecHalfTheta, versHalfTheta;
	float sqNotU, sqU;

	auto cosTheta = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

	// As usual in all slerp implementations, we fold theta.
	alpha = cosTheta >= 0 ? 1.0f : -1.0f;
	halfY = 1.0f + alpha * cosTheta;

	// Here we bisect the interval, so we need to fold t as well.
	f2b = t - 0.5f;
	u = f2b >= 0 ? f2b : -f2b;
	f2a = u - f2b;
	f2b += u;
	u += u;
	f1 = 1.0f - u;

	// One iteration of Newton to get 1-cos(theta / 2) to good accuracy.
	halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
	halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
	versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

	// Evaluate series expansions of the coefficients.
	sqNotU = f1 * f1;
	ratio2 = 0.0000440917108f * versHalfTheta;
	ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
	ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
	ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
	ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

	sqU = u * u;
	ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
	ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
	ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
	ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;

	// Perform the bisection and resolve the folding done earlier.
	f1 *= ratio1 * halfSecHalfTheta;
	f2a *= ratio2;
	f2b *= ratio2;
	alpha *= f1 + f2a;
	beta = f1 + f2b;

	// Apply final coefficients to a and b as usual.
	auto w = alpha * q1.w + beta * q2.w;
	auto x = alpha * q1.x + beta * q2.x;
	auto y = alpha * q1.y + beta * q2.y;
	auto z = alpha * q1.z + beta * q2.z;

	// This final adjustment to the quaternion's length corrects for
	// any small constraint error in the inputs q1 and q2 But as you
	// can see, it comes at the cost of 9 additional multiplication
	// operations. If this error-correcting feature is not required,
	// the following code may be removed.
	f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
	return Quaternion(w * f1, x * f1, y * f1, z * f1);
}


Quaternion Quaternion::squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t)
{
	auto q = slerpForSquad(q1, q2, t);
	auto s = slerpForSquad(s1, s2, t);
	return slerpForSquad(q, s, 2.0f * t * (1.0f - t));
}


Quaternion Quaternion::slerpForSquad(const Quaternion& q1, const Quaternion& q2, float t)
{
	// cos(omega) = q1 * q2;
	// slerp(q1, q2, t) = (q1*sin((1-t)*omega) + q2*sin(t*omega))/sin(omega);
	// q1 = +- q2, slerp(q1,q2,t) = q1.
	// This is a straight-forward implementation of the formula of slerp. It does not do any sign switching.
	auto c = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	if (fabs(c) >= 1.0f)
		return q1;

	auto omega = acos(c);
	auto s = sqrt(1.0f - c * c);
	if (fabs(s) <= 0.00001f)
		return q1;

	auto r1 = sin((1 - t) * omega) / s;
	auto r2 = sin(t * omega) / s;
	return Quaternion(
		q1.x * r1 + q2.x * r2,
		q1.y * r1 + q2.y * r2,
		q1.z * r1 + q2.z * r2,
		q1.w * r1 + q2.w * r2
	);
}


Quaternion& Quaternion::operator*=(const Quaternion& q)
{
	auto newX = w * q.x + x * q.w + y * q.z - z * q.y;
	auto newY = w * q.y - x * q.z + y * q.w + z * q.x;
	auto newZ = w * q.z + x * q.y - y * q.x + z * q.w;
	auto newW = w * q.w - x * q.x - y * q.y - z * q.z;
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
	return *this;
}