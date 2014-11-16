#include "SoloQuaternion.h"
#include "SoloMatrix3.h"
#include "SoloVector3.h"

using namespace solo;

const f32 Quaternion::epsilon = 1e-03;
const Quaternion Quaternion::ZERO(0, 0, 0, 0);
const Quaternion Quaternion::IDENTITY(1, 0, 0, 0);


void Quaternion::fromRotationMatrix(const Matrix3& rot)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".

	f32 trace = rot[0][0] + rot[1][1] + rot[2][2];
	f32 root;

	if (trace > 0.0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		root = Math::sqrt(trace + 1.0f); // 2w
		w = 0.5f * root;
		root = 0.5f / root; // 1/(4w)
		x = (rot[2][1] - rot[1][2]) * root;
		y = (rot[0][2] - rot[2][0]) * root;
		z = (rot[1][0] - rot[0][1]) * root;
	}
	else
	{
		// |w| <= 1/2
		static size_t next[3] = {1, 2, 0};
		size_t i = 0;
		if (rot[1][1] > rot[0][0])
			i = 1;
		if (rot[2][2] > rot[i][i])
			i = 2;
		size_t j = next[i];
		size_t k = next[j];

		root = Math::sqrt(rot[i][i] - rot[j][j] - rot[k][k] + 1.0f);
		f32* quat[3] = {&x, &y, &z};
		*quat[i] = 0.5f * root;
		root = 0.5f / root;
		w = (rot[k][j] - rot[j][k]) * root;
		*quat[j] = (rot[j][i] + rot[i][j]) * root;
		*quat[k] = (rot[k][i] + rot[i][k]) * root;
	}
}

void Quaternion::toRotationMatrix(Matrix3& rot) const
{
	f32 tx = x + x;
	f32 ty = y + y;
	f32 tz = z + z;
	f32 twx = tx * w;
	f32 twy = ty * w;
	f32 twz = tz * w;
	f32 txx = tx * x;
	f32 txy = ty * x;
	f32 txz = tz * x;
	f32 tyy = ty * y;
	f32 tyz = tz * y;
	f32 tzz = tz * z;

	rot[0][0] = 1.0f - (tyy + tzz);
	rot[0][1] = txy - twz;
	rot[0][2] = txz + twy;
	rot[1][0] = txy + twz;
	rot[1][1] = 1.0f - (txx + tzz);
	rot[1][2] = tyz - twx;
	rot[2][0] = txz - twy;
	rot[2][1] = tyz + twx;
	rot[2][2] = 1.0f - (txx + tyy);
}


void Quaternion::fromAngleAxis(const Radian& angle, const Vector3& axis)
{
	// assert:  axis[] is unit length
	//
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

	Radian halfAngle(angle * 0.5);
	f32 sin = Math::sin(halfAngle);
	w = Math::cos(halfAngle);
	x = sin * axis.x;
	y = sin * axis.y;
	z = sin * axis.z;
}


void Quaternion::toAngleAxis(Radian& angle, Vector3& axis) const
{
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

	f32 sqrLength = x * x + y * y + z * z;
	if (sqrLength > 0.0)
	{
		angle = Math::acos(w) * 2.0;
		f32 invLength = Math::invSqrt(sqrLength);
		axis.x = x * invLength;
		axis.y = y * invLength;
		axis.z = z * invLength;
	}
	else
	{
		// angle is 0 (mod 2*pi), so any axis will do
		angle = Radian(0.0);
		axis.x = 1.0;
		axis.y = 0.0;
		axis.z = 0.0;
	}
}


void Quaternion::fromAxes(const Vector3* axes)
{
	Matrix3 rot;

	for (size_t i = 0; i < 3; i++)
	{
		rot[0][i] = axes[i].x;
		rot[1][i] = axes[i].y;
		rot[2][i] = axes[i].z;
	}

	fromRotationMatrix(rot);
}


void Quaternion::fromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
{
	Matrix3 rot;

	rot[0][0] = xAxis.x;
	rot[1][0] = xAxis.y;
	rot[2][0] = xAxis.z;

	rot[0][1] = yAxis.x;
	rot[1][1] = yAxis.y;
	rot[2][1] = yAxis.z;

	rot[0][2] = zAxis.x;
	rot[1][2] = zAxis.y;
	rot[2][2] = zAxis.z;

	fromRotationMatrix(rot);
}


void Quaternion::toAxes(Vector3* axes) const
{
	Matrix3 rot;

	toRotationMatrix(rot);

	for (size_t i = 0; i < 3; i++)
	{
		axes[i].x = rot[0][i];
		axes[i].y = rot[1][i];
		axes[i].z = rot[2][i];
	}
}


Vector3 Quaternion::xAxis(void) const
{
	//f32 tx  = 2.0*x;
	f32 ty = 2.0f * y;
	f32 tz = 2.0f * z;
	f32 twy = ty * w;
	f32 twz = tz * w;
	f32 txy = ty * x;
	f32 txz = tz * x;
	f32 tyy = ty * y;
	f32 tzz = tz * z;

	return Vector3(1.0f - (tyy + tzz), txy + twz, txz - twy);
}


Vector3 Quaternion::yAxis(void) const
{
	f32 tx = 2.0f * x;
	f32 ty = 2.0f * y;
	f32 tz = 2.0f * z;
	f32 twx = tx * w;
	f32 twz = tz * w;
	f32 txx = tx * x;
	f32 txy = ty * x;
	f32 tyz = tz * y;
	f32 tzz = tz * z;

	return Vector3(txy - twz, 1.0f - (txx + tzz), tyz + twx);
}


Vector3 Quaternion::zAxis(void) const
{
	f32 tx = 2.0f * x;
	f32 ty = 2.0f * y;
	f32 tz = 2.0f * z;
	f32 twx = tx * w;
	f32 twy = ty * w;
	f32 txx = tx * x;
	f32 txz = tz * x;
	f32 tyy = ty * y;
	f32 tyz = tz * y;

	return Vector3(txz + twy, tyz - twx, 1.0f - (txx + tyy));
}


void Quaternion::toAxes(Vector3& xaxis, Vector3& yaxis, Vector3& zaxis) const
{
	Matrix3 rot;

	toRotationMatrix(rot);

	xaxis.x = rot[0][0];
	xaxis.y = rot[1][0];
	xaxis.z = rot[2][0];

	yaxis.x = rot[0][1];
	yaxis.y = rot[1][1];
	yaxis.z = rot[2][1];

	zaxis.x = rot[0][2];
	zaxis.y = rot[1][2];
	zaxis.z = rot[2][2];
}


Quaternion Quaternion::operator+(const Quaternion& q) const
{
	return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}


Quaternion Quaternion::operator-(const Quaternion& q) const
{
	return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}


Quaternion Quaternion::operator*(const Quaternion& q) const
{
	// NOTE:  Multiplication is not generally commutative, so in most
	// cases p*q != q*p.

	return Quaternion
		(
			w * q.w - x * q.x - y * q.y - z * q.z,
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y + y * q.w + z * q.x - x * q.z,
			w * q.z + z * q.w + x * q.y - y * q.x
		);
}


Quaternion Quaternion::operator*(f32 scalar) const
{
	return Quaternion(scalar * w, scalar * x, scalar * y, scalar * z);
}


Quaternion operator*(f32 scalar, const Quaternion& q)
{
	return Quaternion(scalar * q.w, scalar * q.x, scalar * q.y, scalar * q.z);
}


Quaternion Quaternion::operator-() const
{
	return Quaternion(-w, -x, -y, -z);
}


f32 Quaternion::dot(const Quaternion& q) const
{
	return w * q.w + x * q.x + y * q.y + z * q.z;
}


f32 Quaternion::normalLength() const
{
	return w * w + x * x + y * y + z * z;
}


Quaternion Quaternion::inverse() const
{
	f32 norm = w * w + x * x + y * y + z * z;
	if (norm > 0.0)
	{
		f32 invNorm = 1.0f / norm;
		return Quaternion(w * invNorm, -x * invNorm, -y * invNorm, -z * invNorm);
	}
	// return an invalid result to flag the error
	return ZERO;
}


Quaternion Quaternion::unitInverse() const
{
	// assert:  'this' is unit length
	return Quaternion(w, -x, -y, -z);
}


Quaternion Quaternion::exp() const
{
	// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
	// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
	// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

	Radian angle(Math::sqrt(x * x + y * y + z * z));
	f32 sin = Math::sin(angle);

	Quaternion result;
	result.w = Math::cos(angle);

	if (Math::abs(sin) >= epsilon)
	{
		f32 coeff = sin / (angle.valueRadians());
		result.x = coeff * x;
		result.y = coeff * y;
		result.z = coeff * z;
	}
	else
	{
		result.x = x;
		result.y = y;
		result.z = z;
	}

	return result;
}


Quaternion Quaternion::log() const
{
	// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
	// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
	// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

	Quaternion result;
	result.w = 0.0;

	if (Math::abs(w) < 1.0)
	{
		Radian angle(Math::acos(w));
		f32 sin = Math::sin(angle);
		if (Math::abs(sin) >= epsilon)
		{
			f32 coefc = angle.valueRadians() / sin;
			result.x = coefc * x;
			result.y = coefc * y;
			result.z = coefc * z;
			return result;
		}
	}

	result.x = x;
	result.y = y;
	result.z = z;

	return result;
}


Vector3 Quaternion::operator*(const Vector3& v) const
{
	// nVidia SDK implementation
	Vector3 uv, uuv;
	Vector3 qvec(x, y, z);
	uv = qvec.cross(v);
	uuv = qvec.cross(uv);
	uv *= (2.0f * w);
	uuv *= 2.0f;

	return v + uv + uuv;
}


bool Quaternion::equals(const Quaternion& rhs, const Radian& tolerance) const
{
	f32 cos = dot(rhs);
	Radian angle = Math::acos(cos);

	return (Math::abs(angle.valueRadians()) <= tolerance.valueRadians())
		|| Math::f32Equal(angle.valueRadians(), Math::PI, tolerance.valueRadians());
}


Quaternion Quaternion::slerp(f32 t, const Quaternion& p, const Quaternion& q, bool shortestPath)
{
	f32 cos = p.dot(q);
	Quaternion tmp;

	// Do we need to invert rotation?
	if (cos < 0.0f && shortestPath)
	{
		cos = -cos;
		tmp = -q;
	}
	else
	{
		tmp = q;
	}

	if (Math::abs(cos) < 1 - epsilon)
	{
		// Standard case (slerp)
		f32 sin = Math::sqrt(1 - Math::sqr(cos));
		Radian fAngle = Math::atan2(sin, cos);
		f32 invSin = 1.0f / sin;
		f32 coefc0 = Math::sin(fAngle * (1.0f - t)) * invSin;
		f32 coefc1 = Math::sin(fAngle * t) * invSin;
		return coefc0 * p + coefc1 * tmp;
	}

	// There are two situations:
	// 1. "p" and "q" are very close (fCos ~= +1), so we can do a linear
	//    interpolation safely.
	// 2. "p" and "q" are almost inverse of each other (fCos ~= -1), there
	//    are an infinite number of possibilities interpolation. but we haven't
	//    have method to fix this case, so just use linear interpolation here.
	Quaternion result = (1.0f - t) * p + t * tmp;
	// taking the complement requires renormalisation
	result.normalise();
	return result;
}


Quaternion Quaternion::slerpExtraSpins(f32 t, const Quaternion& p, const Quaternion& q, int extraSpins)
{
	f32 cos = p.dot(q);
	Radian angle(Math::acos(cos));

	if (Math::abs(angle.valueRadians()) < epsilon)
		return p;

	f32 sin = Math::sin(angle);
	Radian phase(Math::PI * extraSpins * t);
	f32 invSin = 1.0f / sin;
	f32 coeff0 = Math::sin(angle * (1.0f - t) - phase) * invSin;
	f32 coeff1 = Math::sin(angle * t + phase) * invSin;
	return coeff0 * p + coeff1 * q;
}


void Quaternion::intermediate(const Quaternion& q0, const Quaternion& q1, const Quaternion& q2, Quaternion& a, Quaternion& b)
{
	// assert:  q0, q1, q2 are unit quaternions

	Quaternion q0Inv = q0.unitInverse();
	Quaternion q1Inv = q1.unitInverse();
	Quaternion p0 = q0Inv * q1;
	Quaternion p1 = q1Inv * q2;
	Quaternion arg = 0.25 * (p0.log() - p1.log());
	Quaternion minusArg = -arg;

	a = q1 * arg.exp();
	b = q1 * minusArg.exp();
}


Quaternion Quaternion::squad(f32 t, const Quaternion& p, const Quaternion& a, const Quaternion& b, const Quaternion& q, bool shortestPath)
{
	f32 slerpT = 2.0f * t * (1.0f - t);
	Quaternion slerpP = slerp(t, p, q, shortestPath);
	Quaternion slerpQ = slerp(t, a, b);
	return slerp(slerpT, slerpP, slerpQ);
}


f32 Quaternion::normalise(void)
{
	f32 len = normalLength();
	f32 factor = 1.0f / Math::sqrt(len);
	*this = *this * factor;
	return len;
}


Radian Quaternion::roll(bool reprojectAxis) const
{
	if (reprojectAxis)
	{
		// roll = atan2(localx.y, localx.x)
		// pick parts of xAxis() implementation that we need
		//			f32 tx  = 2.0*x;
		f32 ty = 2.0f * y;
		f32 tz = 2.0f * z;
		f32 twz = tz * w;
		f32 txy = ty * x;
		f32 tyy = ty * y;
		f32 tzz = tz * z;

		// Vector3(1.0-(tyy+tzz), txy+twz, txz-twy);

		return Radian(Math::atan2(txy + twz, 1.0f - (tyy + tzz)));
	}
	return Radian(Math::atan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z));
}


Radian Quaternion::pitch(bool reprojectAxis) const
{
	if (reprojectAxis)
	{
		// pitch = atan2(localy.z, localy.y)
		// pick parts of yAxis() implementation that we need
		f32 tx = 2.0f * x;
		//			f32 ty  = 2.0f*y;
		f32 tz = 2.0f * z;
		f32 twx = tx * w;
		f32 txx = tx * x;
		f32 tyz = tz * y;
		f32 tzz = tz * z;

		// Vector3(txy-twz, 1.0-(txx+tzz), tyz+twx);
		return Radian(Math::atan2(tyz + twx, 1.0f - (txx + tzz)));
	}
	// internal version
	return Radian(Math::atan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z));
}


Radian Quaternion::yaw(bool reprojectAxis) const
{
	if (reprojectAxis)
	{
		// yaw = atan2(localz.x, localz.z)
		// pick parts of zAxis() implementation that we need
		f32 tx = 2.0f * x;
		f32 ty = 2.0f * y;
		f32 tz = 2.0f * z;
		f32 twy = ty * w;
		f32 txx = tx * x;
		f32 txz = tz * x;
		f32 tyy = ty * y;

		// Vector3(txz+twy, tyz-twx, 1.0-(txx+tyy));

		return Radian(Math::atan2(txz + twy, 1.0f - (txx + tyy)));
	}
	// internal version
	return Radian(Math::asin(-2 * (x * z - w * y)));
}


Quaternion Quaternion::nlerp(f32 t, const Quaternion& p, const Quaternion& q, bool shortestPath)
{
	Quaternion result;
	f32 cos = p.dot(q);
	if (cos < 0.0f && shortestPath)
		result = p + t * ((-q) - p);
	else
		result = p + t * (q - p);
	result.normalise();
	return result;
}