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
#include "SoloTransformMatrix.h"
#include "SoloBoundingBox.h"
#include "SoloBoundingSphere.h"
#include "SoloPlane.h"
#include "SoloFrustum.h"
#include "SoloRay.h"
#include "SoloLuaCommon.h"

using namespace solo;

static void registerVector2(CppBindModule<LuaBinding> &module)
{
    auto vector2 = BEGIN_CLASS(module, Vector2);
    REG_CTOR(vector2, float, float);
    REG_VARIABLE(vector2, Vector2, x);
    REG_VARIABLE(vector2, Vector2, y);
    REG_METHOD(vector2, Vector2, isUnit);
    REG_METHOD(vector2, Vector2, isZero);
    REG_STATIC_METHOD(vector2, Vector2, angle);
    REG_METHOD(vector2, Vector2, clamp);
    REG_METHOD(vector2, Vector2, distance);
    REG_METHOD(vector2, Vector2, distanceSquared);
    REG_STATIC_METHOD_OVERLOADED(vector2, Vector2, dot, "dot", float, , const Vector2&, const Vector2&);
    REG_METHOD_OVERLOADED(vector2, Vector2, dot, "dot", float, const, const Vector2&);
    REG_METHOD(vector2, Vector2, length);
    REG_METHOD(vector2, Vector2, lengthSquared);
    REG_METHOD(vector2, Vector2, normalize);
    REG_METHOD(vector2, Vector2, normalized);
    REG_META_METHOD(vector2, "__add", [](const Vector2 &v1, const Vector2 &v2) { return v1 + v2; });
    REG_META_METHOD(vector2, "__sub", [](const Vector2 &v1, const Vector2 &v2) { return v1 - v2; });
    REG_META_METHOD(vector2, "__mul", [](const Vector2 &v, float f) { return v * f; });
    REG_META_METHOD(vector2, "__div", [](const Vector2 &v, float f) { return v / f; });
    REG_META_METHOD(vector2, "__unm", [](const Vector2 &v) { return -v; });
    vector2.endClass();
}

static void registerVector3(CppBindModule<LuaBinding> &module)
{
    auto vector3 = BEGIN_CLASS(module, Vector3);
    REG_CTOR(vector3, float, float, float);
    REG_VARIABLE(vector3, Vector3, x);
    REG_VARIABLE(vector3, Vector3, y);
    REG_VARIABLE(vector3, Vector3, z);
    REG_METHOD(vector3, Vector3, isUnit);
    REG_METHOD(vector3, Vector3, isZero);
    REG_STATIC_METHOD(vector3, Vector3, angle);
    REG_METHOD(vector3, Vector3, clamp);
    REG_STATIC_METHOD(vector3, Vector3, cross);
    REG_METHOD(vector3, Vector3, distance);
    REG_METHOD(vector3, Vector3, distanceSquared);
    REG_STATIC_METHOD_OVERLOADED(vector3, Vector3, dot, "dot", float, , const Vector3&, const Vector3&);
    REG_METHOD_OVERLOADED(vector3, Vector3, dot, "dot", float, const, const Vector3&);
    REG_METHOD(vector3, Vector3, length);
    REG_METHOD(vector3, Vector3, lengthSquared);
    REG_METHOD(vector3, Vector3, normalize);
    REG_METHOD(vector3, Vector3, normalized);
    REG_META_METHOD(vector3, "__add", [](const Vector3 &v1, const Vector3 &v2) { return v1 + v2; });
    REG_META_METHOD(vector3, "__sub", [](const Vector3 &v1, const Vector3 &v2) { return v1 - v2; });
    REG_META_METHOD(vector3, "__mul", [](const Vector3 &v, float f) { return v * f; });
    REG_META_METHOD(vector3, "__div", [](const Vector3 &v, float f) { return v / f; });
    REG_META_METHOD(vector3, "__unm", [](const Vector3 &v) { return -v; });
    vector3.endClass();
}

static void registerVector4(CppBindModule<LuaBinding> &module)
{
    auto vector4 = BEGIN_CLASS(module, Vector4);
    REG_CTOR(vector4, float, float, float, float);
    REG_VARIABLE(vector4, Vector4, x);
    REG_VARIABLE(vector4, Vector4, y);
    REG_VARIABLE(vector4, Vector4, z);
    REG_VARIABLE(vector4, Vector4, w);
    REG_METHOD(vector4, Vector4, isUnit);
    REG_METHOD(vector4, Vector4, isZero);
    REG_STATIC_METHOD(vector4, Vector4, angle);
    REG_METHOD(vector4, Vector4, clamp);
    REG_METHOD(vector4, Vector4, distance);
    REG_METHOD(vector4, Vector4, distanceSquared);
    REG_STATIC_METHOD_OVERLOADED(vector4, Vector4, dot, "dot", float, , const Vector4&, const Vector4&);
    REG_METHOD_OVERLOADED(vector4, Vector4, dot, "dot", float, const, const Vector4&);
    REG_METHOD(vector4, Vector4, length);
    REG_METHOD(vector4, Vector4, lengthSquared);
    REG_METHOD(vector4, Vector4, normalize);
    REG_METHOD(vector4, Vector4, normalized);
    REG_META_METHOD(vector4, "__add", [](const Vector4 &v1, const Vector4 &v2) { return v1 + v2; });
    REG_META_METHOD(vector4, "__sub", [](const Vector4 &v1, const Vector4 &v2) { return v1 - v2; });
    REG_META_METHOD(vector4, "__mul", [](const Vector4 &v, float f) { return v * f; });
    REG_META_METHOD(vector4, "__div", [](const Vector4 &v, float f) { return v / f; });
    REG_META_METHOD(vector4, "__unm", [](const Vector4 &v) { return -v; });
    vector4.endClass();
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
    REG_METHOD(q, Quaternion, isIdentity);
    REG_METHOD(q, Quaternion, isZero);
    REG_METHOD(q, Quaternion, conjugate);
    REG_METHOD(q, Quaternion, inverse);
    REG_METHOD(q, Quaternion, normalize);
    REG_METHOD(q, Quaternion, normalized);
    REG_METHOD(q, Quaternion, toAxisAngle);
    REG_STATIC_METHOD(q, Quaternion, lerp);
    REG_STATIC_METHOD(q, Quaternion, slerp);
    REG_STATIC_METHOD(q, Quaternion, squad);
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
    REG_METHOD(matrix, Matrix, resetToIdentity);
    REG_METHOD(matrix, Matrix, resetToZero);
    REG_META_METHOD(matrix, "__mul", [](const Matrix &m1, const Matrix &m2) { return m1 * m2; });
    matrix.endClass();
}

static void registerTransformMatrix(CppBindModule<LuaBinding> &module)
{
    auto matrix = BEGIN_CLASS(module, TransformMatrix);
    REG_CTOR(matrix);
    REG_STATIC_METHOD(matrix, TransformMatrix, createLookAt);
    REG_STATIC_METHOD(matrix, TransformMatrix, createPerspective);
    REG_STATIC_METHOD(matrix, TransformMatrix, createOrthographic);
    REG_STATIC_METHOD(matrix, TransformMatrix, createScale);
    REG_STATIC_METHOD(matrix, TransformMatrix, createRotationFromQuaternion);
    REG_STATIC_METHOD(matrix, TransformMatrix, createRotationFromAxisAngle);
    REG_STATIC_METHOD(matrix, TransformMatrix, createRotationX);
    REG_STATIC_METHOD(matrix, TransformMatrix, createRotationY);
    REG_STATIC_METHOD(matrix, TransformMatrix, createRotationZ);
    REG_STATIC_METHOD(matrix, TransformMatrix, createTranslation);
    REG_METHOD(matrix, TransformMatrix, getScale);
    REG_METHOD(matrix, TransformMatrix, getTranslation);
    REG_METHOD(matrix, TransformMatrix, getRotation);
    REG_METHOD(matrix, TransformMatrix, getUpVector);
    REG_METHOD(matrix, TransformMatrix, getDownVector);
    REG_METHOD(matrix, TransformMatrix, getLeftVector);
    REG_METHOD(matrix, TransformMatrix, getRightVector);
    REG_METHOD(matrix, TransformMatrix, getForwardVector);
    REG_METHOD(matrix, TransformMatrix, getBackVector);
    REG_METHOD(matrix, TransformMatrix, rotateByQuaternion);
    REG_METHOD(matrix, TransformMatrix, rotateByAxisAngle);
    REG_METHOD(matrix, TransformMatrix, rotateX);
    REG_METHOD(matrix, TransformMatrix, rotateY);
    REG_METHOD(matrix, TransformMatrix, rotateZ);
    REG_METHOD(matrix, TransformMatrix, scaleByScalar);
    REG_METHOD(matrix, TransformMatrix, scaleByVector);
    REG_METHOD(matrix, TransformMatrix, translate);
    REG_METHOD(matrix, TransformMatrix, transformPoint);
    REG_METHOD(matrix, TransformMatrix, transformDirection);
    REG_METHOD(matrix, TransformMatrix, transformBoundingBox);
    REG_METHOD(matrix, TransformMatrix, transformBoundingSphere);
    REG_METHOD(matrix, TransformMatrix, transformPlane);
    REG_METHOD(matrix, TransformMatrix, transformRay);
    REG_METHOD(matrix, TransformMatrix, decompose);
    matrix.endClass();
}

static void registerBoundingBox(CppBindModule<LuaBinding> &module)
{
    auto bb = BEGIN_CLASS(module, BoundingBox);
    REG_CTOR(bb, const Vector3&, const Vector3&);
    REG_VARIABLE(bb, BoundingBox, min);
    REG_VARIABLE(bb, BoundingBox, max);
    REG_METHOD(bb, BoundingBox, getCenter);
    REG_METHOD(bb, BoundingBox, getCorners);
    REG_METHOD(bb, BoundingBox, intersectsBoundingBox);
    REG_METHOD(bb, BoundingBox, intersectsBoundingSphere);
    REG_METHOD(bb, BoundingBox, intersectsFrustum);
    REG_METHOD(bb, BoundingBox, hitByRay);
    REG_METHOD(bb, BoundingBox, intersectPlane);
    REG_METHOD(bb, BoundingBox, isEmpty);
    REG_METHOD(bb, BoundingBox, mergeBoundingBox);
    REG_METHOD(bb, BoundingBox, mergeBoundingSphere);
    bb.endClass();
}

static void registerBoundingSphere(CppBindModule<LuaBinding> &module)
{
    auto bs = BEGIN_CLASS(module, BoundingSphere);
    REG_CTOR(bs, const Vector3&, float);
    REG_VARIABLE(bs, BoundingSphere, center);
    REG_VARIABLE(bs, BoundingSphere, radius);
    REG_METHOD(bs, BoundingSphere, intersectsBoundingSphere);
    REG_METHOD(bs, BoundingSphere, intersectsBoundingBox);
    REG_METHOD(bs, BoundingSphere, intersectsFrustum);
    REG_METHOD(bs, BoundingSphere, hitByRay);
    REG_METHOD(bs, BoundingSphere, intersectPlane);
    REG_METHOD(bs, BoundingSphere, isEmpty);
    REG_METHOD(bs, BoundingSphere, mergeBoundingBox);
    REG_METHOD(bs, BoundingSphere, mergeBoundingSphere);
    bs.endClass();
}

static void registerPlane(CppBindModule<LuaBinding> &module)
{
    auto plane = BEGIN_CLASS(module, Plane);
    REG_CTOR(plane, const Vector3&, float);
    REG_METHOD(plane, Plane, getNormal);
    REG_METHOD(plane, Plane, setNormal);
    REG_METHOD(plane, Plane, getDistance);
    REG_METHOD(plane, Plane, setDistance);
    REG_METHOD(plane, Plane, getDistanceToPoint);
    REG_STATIC_METHOD(plane, Plane, getCommonPoint);
    REG_METHOD(plane, Plane, intersectBoundingBox);
    REG_METHOD(plane, Plane, intersectBoundingSphere);
    REG_METHOD(plane, Plane, intersectFrustum);
    REG_METHOD(plane, Plane, intersectPlane);
    REG_METHOD(plane, Plane, intersectRay);
    REG_METHOD(plane, Plane, isParallel);
    plane.endClass();
}

static void registerRay(CppBindModule<LuaBinding> &module)
{
    auto ray = BEGIN_CLASS(module, Ray);
    REG_CTOR(ray, const Vector3&, const Vector3&);
    REG_METHOD(ray, Ray, getOrigin);
    REG_METHOD(ray, Ray, setOrigin);
    REG_METHOD(ray, Ray, getDirection);
    REG_METHOD(ray, Ray, setDirection);
    REG_METHOD(ray, Ray, hitBoundingBox);
    REG_METHOD(ray, Ray, hitBoundingSphere);
    REG_METHOD(ray, Ray, hitFrustum);
    REG_METHOD(ray, Ray, hitPlane);
    ray.endClass();
}

static void registerFrustum(CppBindModule<LuaBinding> &module)
{
    auto f = BEGIN_CLASS(module, Frustum);
    REG_CTOR(f);
    REG_METHOD(f, Frustum, getNearPlane);
    REG_METHOD(f, Frustum, getFarPlane);
    REG_METHOD(f, Frustum, getLeftPlane);
    REG_METHOD(f, Frustum, getRightPlane);
    REG_METHOD(f, Frustum, getTopPlane);
    REG_METHOD(f, Frustum, getBottomPlane);
    REG_METHOD(f, Frustum, getMatrix);
    REG_METHOD(f, Frustum, setMatrix);
    REG_METHOD(f, Frustum, getCorners);
    REG_METHOD(f, Frustum, getNearCorners);
    REG_METHOD(f, Frustum, getFarCorners);
    REG_METHOD(f, Frustum, intersectsPoint);
    REG_METHOD(f, Frustum, intersectsBoundingBox);
    REG_METHOD(f, Frustum, intersectsBoundingSphere);
    REG_METHOD(f, Frustum, hitByRay);
    REG_METHOD(f, Frustum, intersectPlane);
    f.endClass();
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
    registerTransformMatrix(module);
    registerBoundingBox(module);
    registerBoundingSphere(module);
    registerPlane(module);
    registerRay(module);
    registerFrustum(module);
}
