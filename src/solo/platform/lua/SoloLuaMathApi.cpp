/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloRadian.h"
#include "SoloDegree.h"
#include "SoloVector2.h"
#include "SoloVector3.h"
#include "SoloVector4.h"
#include "SoloQuaternion.h"
#include "SoloMatrix.h"
#include "SoloRay.h"
#include "SoloLuaCommon.h"

using namespace solo;

static void registerVector2(CppBindModule<LuaBinding> &module)
{
    auto v = BEGIN_CLASS(module, Vector2);
    REG_CTOR(v, float, float);
    REG_VARIABLE(v, Vector2, x);
    REG_VARIABLE(v, Vector2, y);
    REG_METHOD(v, Vector2, isUnit);
    REG_METHOD(v, Vector2, isZero);
    REG_METHOD(v, Vector2, distance);
    REG_METHOD(v, Vector2, distanceSquared);
    REG_METHOD(v, Vector2, length);
    REG_METHOD(v, Vector2, lengthSquared);
    REG_METHOD(v, Vector2, normalized);
    REG_METHOD(v, Vector2, normalize);
    REG_METHOD(v, Vector2, angle);
    REG_METHOD(v, Vector2, clamp);
    REG_METHOD(v, Vector2, dot);
    REG_META_METHOD(v, "__add", [](const Vector2 &v1, const Vector2 &v2) { return v1 + v2; });
    REG_META_METHOD(v, "__sub", [](const Vector2 &v1, const Vector2 &v2) { return v1 - v2; });
    REG_META_METHOD(v, "__mul", [](const Vector2 &v, float f) { return v * f; });
    REG_META_METHOD(v, "__div", [](const Vector2 &v, float f) { return v / f; });
    REG_META_METHOD(v, "__unm", [](const Vector2 &v) { return -v; });
    v.endClass();
}

static void registerVector3(CppBindModule<LuaBinding> &module)
{
    auto v = BEGIN_CLASS(module, Vector3);
    REG_CTOR(v, float, float, float);
    REG_VARIABLE(v, Vector3, x);
    REG_VARIABLE(v, Vector3, y);
    REG_VARIABLE(v, Vector3, z);
    REG_METHOD(v, Vector3, isUnit);
    REG_METHOD(v, Vector3, isZero);
    REG_METHOD(v, Vector3, distance);
    REG_METHOD(v, Vector3, distanceSquared);
    REG_METHOD(v, Vector3, length);
    REG_METHOD(v, Vector3, lengthSquared);
    REG_METHOD(v, Vector3, normalized);
    REG_METHOD(v, Vector3, normalize);
    REG_METHOD(v, Vector3, angle);
    REG_METHOD(v, Vector3, clamp);
    REG_METHOD(v, Vector3, dot);
    REG_METHOD(v, Vector3, cross);
    REG_META_METHOD(v, "__add", [](const Vector3 &v1, const Vector3 &v2) { return v1 + v2; });
    REG_META_METHOD(v, "__sub", [](const Vector3 &v1, const Vector3 &v2) { return v1 - v2; });
    REG_META_METHOD(v, "__mul", [](const Vector3 &v, float f) { return v * f; });
    REG_META_METHOD(v, "__div", [](const Vector3 &v, float f) { return v / f; });
    REG_META_METHOD(v, "__unm", [](const Vector3 &v) { return -v; });
    v.endClass();
}

static void registerVector4(CppBindModule<LuaBinding> &module)
{
    auto v = BEGIN_CLASS(module, Vector4);
    REG_CTOR(v, float, float, float, float);
    REG_VARIABLE(v, Vector4, x);
    REG_VARIABLE(v, Vector4, y);
    REG_VARIABLE(v, Vector4, z);
    REG_VARIABLE(v, Vector4, w);
    REG_METHOD(v, Vector4, isUnit);
    REG_METHOD(v, Vector4, isZero);
    REG_METHOD(v, Vector4, distance);
    REG_METHOD(v, Vector4, distanceSquared);
    REG_METHOD(v, Vector4, length);
    REG_METHOD(v, Vector4, lengthSquared);
    REG_METHOD(v, Vector4, normalized);
    REG_METHOD(v, Vector4, normalize);
    REG_METHOD(v, Vector4, angle);
    REG_METHOD(v, Vector4, clamp);
    REG_METHOD(v, Vector4, dot);
    REG_META_METHOD(v, "__add", [](const Vector4 &v1, const Vector4 &v2) { return v1 + v2; });
    REG_META_METHOD(v, "__sub", [](const Vector4 &v1, const Vector4 &v2) { return v1 - v2; });
    REG_META_METHOD(v, "__mul", [](const Vector4 &v, float f) { return v * f; });
    REG_META_METHOD(v, "__div", [](const Vector4 &v, float f) { return v / f; });
    REG_META_METHOD(v, "__unm", [](const Vector4 &v) { return -v; });
    v.endClass();
}

static void registerQuaternion(CppBindModule<LuaBinding> &module)
{
    auto q = BEGIN_CLASS(module, Quaternion);
    REG_CTOR(q);
    REG_VARIABLE(q, Quaternion, x);
    REG_VARIABLE(q, Quaternion, y);
    REG_VARIABLE(q, Quaternion, z);
    REG_VARIABLE(q, Quaternion, w);
    REG_STATIC_METHOD(q, Quaternion, createFromAxisAngle);
    REG_STATIC_METHOD(q, Quaternion, lerp);
    REG_STATIC_METHOD(q, Quaternion, slerp);
    REG_STATIC_METHOD(q, Quaternion, squad);
    REG_METHOD(q, Quaternion, isIdentity);
    REG_METHOD(q, Quaternion, isZero);
    REG_METHOD(q, Quaternion, conjugate);
    REG_METHOD(q, Quaternion, inverse);
    REG_METHOD(q, Quaternion, normalize);
    REG_METHOD(q, Quaternion, normalized);
    REG_METHOD(q, Quaternion, toAxisAngle);
    REG_META_METHOD(q, "__mul", [](const Quaternion &q1, const Quaternion &q2) { return q1 * q2; });
    q.endClass();
}

static void registerRadian(CppBindModule<LuaBinding> &module)
{
    auto radian = BEGIN_CLASS(module, Radian);
    REG_CTOR(radian, float);
    REG_METHOD(radian, Radian, toRawDegree);
    REG_METHOD(radian, Radian, toRawRadian);
    REG_META_METHOD(radian, "__unm", [](const Radian &r) { return -r; });
    REG_META_METHOD(radian, "__add", [](const Radian &r1, const Radian &r2) { return r1 + r2; });
    REG_META_METHOD(radian, "__sub", [](const Radian &r1, const Radian &r2) { return r1 - r2; });
    REG_META_METHOD(radian, "__mul", [](const Radian &r, float f) { return r * f; });
    REG_META_METHOD(radian, "__div", [](const Radian &r, float f) { return r * f; });
    radian.endClass();
}

static void registerDegree(CppBindModule<LuaBinding> &module)
{
    auto degree = BEGIN_CLASS(module, Degree);
    REG_CTOR(degree, float);
    REG_METHOD(degree, Degree, toRawDegree);
    REG_METHOD(degree, Degree, toRawRadian);
    REG_META_METHOD(degree, "__unm", [](const Degree &d) { return -d; });
    REG_META_METHOD(degree, "__add", [](const Degree &d1, const Degree &d2) { return d1 + d2; });
    REG_META_METHOD(degree, "__sub", [](const Degree &d1, const Degree &d2) { return d1 - d2; });
    REG_META_METHOD(degree, "__mul", [](const Degree &d, float f) { return d * f; });
    REG_META_METHOD(degree, "__div", [](const Degree &d, float f) { return d * f; });
    degree.endClass();
}

static void registerMatrix(CppBindModule<LuaBinding> &module)
{
    auto matrix = BEGIN_CLASS(module, Matrix);
    REG_CTOR(matrix);
    REG_STATIC_METHOD(matrix, Matrix, identity);
    REG_STATIC_METHOD(matrix, Matrix, getSize);
    REG_METHOD(matrix, Matrix, isIdentity);
    REG_METHOD(matrix, Matrix, getDeterminant);
    REG_METHOD(matrix, Matrix, invert);
    REG_METHOD(matrix, Matrix, transpose);
    REG_STATIC_METHOD(matrix, Matrix, createLookAt);
    REG_STATIC_METHOD(matrix, Matrix, createPerspective);
    REG_STATIC_METHOD(matrix, Matrix, createOrthographic);
    REG_STATIC_METHOD(matrix, Matrix, createScale);
    REG_STATIC_METHOD(matrix, Matrix, createRotationFromQuaternion);
    REG_STATIC_METHOD(matrix, Matrix, createRotationFromAxisAngle);
    REG_STATIC_METHOD(matrix, Matrix, createRotationX);
    REG_STATIC_METHOD(matrix, Matrix, createRotationY);
    REG_STATIC_METHOD(matrix, Matrix, createRotationZ);
    REG_STATIC_METHOD(matrix, Matrix, createTranslation);
    REG_METHOD(matrix, Matrix, getScale);
    REG_METHOD(matrix, Matrix, getTranslation);
    REG_METHOD(matrix, Matrix, getRotation);
    REG_METHOD(matrix, Matrix, getUpVector);
    REG_METHOD(matrix, Matrix, getDownVector);
    REG_METHOD(matrix, Matrix, getLeftVector);
    REG_METHOD(matrix, Matrix, getRightVector);
    REG_METHOD(matrix, Matrix, getForwardVector);
    REG_METHOD(matrix, Matrix, getBackVector);
    REG_METHOD(matrix, Matrix, rotateByQuaternion);
    REG_METHOD(matrix, Matrix, rotateByAxisAngle);
    REG_METHOD(matrix, Matrix, rotateX);
    REG_METHOD(matrix, Matrix, rotateY);
    REG_METHOD(matrix, Matrix, rotateZ);
    REG_METHOD(matrix, Matrix, scaleByScalar);
    REG_METHOD(matrix, Matrix, scaleByVector);
    REG_METHOD(matrix, Matrix, translate);
    REG_METHOD(matrix, Matrix, transformPoint);
    REG_METHOD(matrix, Matrix, transformDirection);
    REG_METHOD(matrix, Matrix, transformRay);
    REG_METHOD(matrix, Matrix, decompose);
    REG_META_METHOD(matrix, "__mul", [](const Matrix &m1, const Matrix &m2) { return m1 * m2; });
    matrix.endClass();
}

static void registerRay(CppBindModule<LuaBinding> &module)
{
    auto ray = BEGIN_CLASS(module, Ray);
    REG_CTOR(ray, const Vector3&, const Vector3&);
    REG_METHOD(ray, Ray, getOrigin);
    REG_METHOD(ray, Ray, setOrigin);
    REG_METHOD(ray, Ray, getDirection);
    REG_METHOD(ray, Ray, setDirection);
    ray.endClass();
}

void registerMathApi(CppBindModule<LuaBinding> &module)
{
    registerRadian(module);
    registerDegree(module);
    registerVector2(module);
    registerVector3(module);
    registerVector4(module);
    registerQuaternion(module);
    registerMatrix(module);
    registerRay(module);
}
