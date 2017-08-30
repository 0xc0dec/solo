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
#include "SoloRay.h"
#include "SoloLuaCommon.h"

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace solo;

static void registerVector2_2(CppBindModule<LuaBinding> &module)
{
    auto v = BEGIN_CLASS_RENAMED(module, glm::vec2, "Wector2");
    REG_CTOR(v, float, float);
    REG_VARIABLE(v, glm::vec2, x);
    REG_VARIABLE(v, glm::vec2, y);
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec2 *v1, const glm::vec2 &v2) { return glm::angle(*v1, v2); }, "angle");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec2 *v1, const glm::vec2 &v2) { return glm::distance(*v1, v2); }, "distance");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec2 *v1, const glm::vec2 &v2) { return glm::dot(*v1, v2); }, "dot");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec2 *v1) { return glm::length(*v1); }, "length");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec2 *v1) { return glm::normalize(*v1); }, "normalized");
    REG_META_METHOD(v, "__add", [](const glm::vec2 &v1, const glm::vec2 &v2) { return v1 + v2; });
    REG_META_METHOD(v, "__sub", [](const glm::vec2 &v1, const glm::vec2 &v2) { return v1 - v2; });
    REG_META_METHOD(v, "__mul", [](const glm::vec2 &v, float f) { return v * f; });
    REG_META_METHOD(v, "__div", [](const glm::vec2 &v, float f) { return v / f; });
    REG_META_METHOD(v, "__unm", [](const glm::vec2 &v) { return -v; });
    v.endClass();
}

static void registerVector3_2(CppBindModule<LuaBinding> &module)
{
    auto v = BEGIN_CLASS_RENAMED(module, glm::vec3, "Wector3");
    REG_CTOR(v, float, float, float);
    REG_VARIABLE(v, glm::vec3, x);
    REG_VARIABLE(v, glm::vec3, y);
    REG_VARIABLE(v, glm::vec3, z);
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec3 *v1, const glm::vec3 &v2) { return glm::angle(*v1, v2); }, "angle");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec3 *v1, const glm::vec3 &v2) { return glm::distance(*v1, v2); }, "distance");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec3 *v1, const glm::vec3 &v2) { return glm::dot(*v1, v2); }, "dot");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec3 *v1, const glm::vec3 &v2) { return glm::cross(*v1, v2); }, "cross");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec3 *v1) { return glm::length(*v1); }, "length");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec3 *v1) { return glm::normalize(*v1); }, "normalized");
    REG_META_METHOD(v, "__add", [](const glm::vec3 &v1, const glm::vec3 &v2) { return v1 + v2; });
    REG_META_METHOD(v, "__sub", [](const glm::vec3 &v1, const glm::vec3 &v2) { return v1 - v2; });
    REG_META_METHOD(v, "__mul", [](const glm::vec3 &v, float f) { return v * f; });
    REG_META_METHOD(v, "__div", [](const glm::vec3 &v, float f) { return v / f; });
    REG_META_METHOD(v, "__unm", [](const glm::vec3 &v) { return -v; });
    v.endClass();
}

static void registerVector4_2(CppBindModule<LuaBinding> &module)
{
    auto v = BEGIN_CLASS_RENAMED(module, glm::vec4, "Wector4");
    REG_CTOR(v, float, float, float, float);
    REG_VARIABLE(v, glm::vec4, x);
    REG_VARIABLE(v, glm::vec4, y);
    REG_VARIABLE(v, glm::vec4, z);
    REG_VARIABLE(v, glm::vec4, w);
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec4 *v1, const glm::vec4 &v2) { return glm::angle(*v1, v2); }, "angle");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec4 *v1, const glm::vec4 &v2) { return glm::distance(*v1, v2); }, "distance");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec4 *v1, const glm::vec4 &v2) { return glm::dot(*v1, v2); }, "dot");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec4 *v1) { return glm::length(*v1); }, "length");
    REG_FREE_FUNC_AS_METHOD_RENAMED(v, [](glm::vec4 *v1) { return glm::normalize(*v1); }, "normalized");
    REG_META_METHOD(v, "__add", [](const glm::vec4 &v1, const glm::vec4 &v2) { return v1 + v2; });
    REG_META_METHOD(v, "__sub", [](const glm::vec4 &v1, const glm::vec4 &v2) { return v1 - v2; });
    REG_META_METHOD(v, "__mul", [](const glm::vec4 &v, float f) { return v * f; });
    REG_META_METHOD(v, "__div", [](const glm::vec4 &v, float f) { return v / f; });
    REG_META_METHOD(v, "__unm", [](const glm::vec4 &v) { return -v; });
    v.endClass();
}

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
    REG_METHOD(matrix, TransformMatrix, transformRay);
    REG_METHOD(matrix, TransformMatrix, decompose);
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
    registerVector2_2(module);
    registerVector3(module);
    registerVector3_2(module);
    registerVector4(module);
    registerVector4_2(module);
    registerQuaternion(module);
    registerMatrix(module);
    registerTransformMatrix(module);
    registerRay(module);
}
