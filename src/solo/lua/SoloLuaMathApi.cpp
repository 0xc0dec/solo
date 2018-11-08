/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloRadians.h"
#include "SoloDegrees.h"
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
    auto binding = BEGIN_CLASS(module, Vector2);
    REG_CTOR(binding, float, float);
    binding.addProperty("x",
        static_cast<float(Vector2::*)()const>(&Vector2::x),
        [](Vector2 *v, float val) { v->x() = val; });
    binding.addProperty("y",
        static_cast<float(Vector2::*)()const>(&Vector2::y),
        [](Vector2 *v, float val) { v->y() = val; });
    REG_METHOD(binding, Vector2, isUnit);
    REG_METHOD(binding, Vector2, isZero);
    REG_METHOD(binding, Vector2, distance);
    REG_METHOD(binding, Vector2, length);
    REG_METHOD(binding, Vector2, normalized);
    REG_METHOD(binding, Vector2, normalize);
    REG_METHOD(binding, Vector2, angle);
    REG_METHOD(binding, Vector2, clamp);
    REG_METHOD(binding, Vector2, dot);
    REG_META_METHOD(binding, "__add", [](const Vector2 &v1, const Vector2 &v2) { return v1 + v2; });
    REG_META_METHOD(binding, "__sub", [](const Vector2 &v1, const Vector2 &v2) { return v1 - v2; });
    REG_META_METHOD(binding, "__mul", [](const Vector2 &v, float f) { return v * f; });
    REG_META_METHOD(binding, "__div", [](const Vector2 &v, float f) { return v / f; });
    REG_META_METHOD(binding, "__unm", [](const Vector2 &v) { return -v; });
    binding.endClass();
}

static void registerVector3(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, Vector3);
    REG_CTOR(binding, float, float, float);
    binding.addProperty("x",
        static_cast<float(Vector3::*)()const>(&Vector3::x),
        [](Vector3 *v, float val) { v->x() = val; });
    binding.addProperty("y",
        static_cast<float(Vector3::*)()const>(&Vector3::y),
        [](Vector3 *v, float val) { v->y() = val; });
    binding.addProperty("z",
        static_cast<float(Vector3::*)()const>(&Vector3::z),
        [](Vector3 *v, float val) { v->z() = val; });
    REG_METHOD(binding, Vector3, isUnit);
    REG_METHOD(binding, Vector3, isZero);
    REG_METHOD(binding, Vector3, distance);
    REG_METHOD(binding, Vector3, length);
    REG_METHOD(binding, Vector3, normalized);
    REG_METHOD(binding, Vector3, normalize);
    REG_METHOD(binding, Vector3, angle);
    REG_METHOD(binding, Vector3, clamp);
    REG_METHOD(binding, Vector3, dot);
    REG_METHOD(binding, Vector3, cross);
    REG_META_METHOD(binding, "__add", [](const Vector3 &v1, const Vector3 &v2) { return v1 + v2; });
    REG_META_METHOD(binding, "__sub", [](const Vector3 &v1, const Vector3 &v2) { return v1 - v2; });
    REG_META_METHOD(binding, "__mul", [](const Vector3 &v, float f) { return v * f; });
    REG_META_METHOD(binding, "__div", [](const Vector3 &v, float f) { return v / f; });
    REG_META_METHOD(binding, "__unm", [](const Vector3 &v) { return -v; });
    binding.endClass();
}

static void registerVector4(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, Vector4);
    REG_CTOR(binding, float, float, float, float);
    binding.addProperty("x",
        static_cast<float(Vector4::*)()const>(&Vector4::x),
        [](Vector4 *v, float val) { v->x() = val; });
    binding.addProperty("y",
        static_cast<float(Vector4::*)()const>(&Vector4::y),
        [](Vector4 *v, float val) { v->y() = val; });
    binding.addProperty("z",
        static_cast<float(Vector4::*)()const>(&Vector4::z),
        [](Vector4 *v, float val) { v->z() = val; });
    binding.addProperty("w",
        static_cast<float(Vector4::*)()const>(&Vector4::w),
        [](Vector4 *v, float val) { v->w() = val; });
    REG_METHOD(binding, Vector4, isUnit);
    REG_METHOD(binding, Vector4, isZero);
    REG_METHOD(binding, Vector4, distance);
    REG_METHOD(binding, Vector4, length);
    REG_METHOD(binding, Vector4, normalized);
    REG_METHOD(binding, Vector4, normalize);
    REG_METHOD(binding, Vector4, angle);
    REG_METHOD(binding, Vector4, clamp);
    REG_METHOD(binding, Vector4, dot);
    REG_META_METHOD(binding, "__add", [](const Vector4 &v1, const Vector4 &v2) { return v1 + v2; });
    REG_META_METHOD(binding, "__sub", [](const Vector4 &v1, const Vector4 &v2) { return v1 - v2; });
    REG_META_METHOD(binding, "__mul", [](const Vector4 &v, float f) { return v * f; });
    REG_META_METHOD(binding, "__div", [](const Vector4 &v, float f) { return v / f; });
    REG_META_METHOD(binding, "__unm", [](const Vector4 &v) { return -v; });
    binding.endClass();
}

static void registerQuaternion(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, Quaternion);
    REG_CTOR(binding);
    binding.addProperty("x",
        static_cast<float(Quaternion::*)()const>(&Quaternion::x),
        [](Quaternion *v, float val) { v->x() = val; });
    binding.addProperty("y",
        static_cast<float(Quaternion::*)()const>(&Quaternion::y),
        [](Quaternion *v, float val) { v->y() = val; });
    binding.addProperty("z",
        static_cast<float(Quaternion::*)()const>(&Quaternion::z),
        [](Quaternion *v, float val) { v->z() = val; });
    binding.addProperty("w",
        static_cast<float(Quaternion::*)()const>(&Quaternion::w),
        [](Quaternion *v, float val) { v->w() = val; });
    REG_STATIC_METHOD(binding, Quaternion, fromAxisAngle);
    REG_STATIC_METHOD(binding, Quaternion, lerp);
    REG_STATIC_METHOD(binding, Quaternion, slerp);
    REG_METHOD(binding, Quaternion, isIdentity);
    REG_METHOD(binding, Quaternion, isZero);
    REG_METHOD(binding, Quaternion, conjugate);
    REG_METHOD(binding, Quaternion, invert);
    REG_METHOD(binding, Quaternion, normalize);
    REG_METHOD(binding, Quaternion, normalized);
    REG_METHOD(binding, Quaternion, toAxisAngle);
    REG_META_METHOD(binding, "__mul", [](const Quaternion &q1, const Quaternion &q2) { return q1 * q2; });
    binding.endClass();
}

static void registerRadians(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, Radians);
    REG_CTOR(binding, float);
    REG_METHOD(binding, Radians, toRawDegrees);
    REG_METHOD(binding, Radians, toRawRadians);
    REG_FREE_FUNC_AS_STATIC_FUNC_RENAMED(binding, [](const Degrees &d) { return Radians(d); }, "fromDegrees");
    REG_FREE_FUNC_AS_STATIC_FUNC_RENAMED(binding, [](float rawDegree) { return Radians(Degrees(rawDegree)); }, "fromRawDegrees");
    REG_META_METHOD(binding, "__unm", [](const Radians &r) { return -r; });
    REG_META_METHOD(binding, "__add", [](const Radians &r1, const Radians &r2) { return r1 + r2; });
    REG_META_METHOD(binding, "__sub", [](const Radians &r1, const Radians &r2) { return r1 - r2; });
    REG_META_METHOD(binding, "__mul", [](const Radians &r, float f) { return r * f; });
    REG_META_METHOD(binding, "__div", [](const Radians &r, float f) { return r * f; });
    binding.endClass();
}

static void registerDegrees(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, Degrees);
    REG_CTOR(binding, float);
    REG_METHOD(binding, Degrees, toRawDegrees);
    REG_METHOD(binding, Degrees, toRawRadians);
    REG_META_METHOD(binding, "__unm", [](const Degrees &d) { return -d; });
    REG_META_METHOD(binding, "__add", [](const Degrees &d1, const Degrees &d2) { return d1 + d2; });
    REG_META_METHOD(binding, "__sub", [](const Degrees &d1, const Degrees &d2) { return d1 - d2; });
    REG_META_METHOD(binding, "__mul", [](const Degrees &d, float f) { return d * f; });
    REG_META_METHOD(binding, "__div", [](const Degrees &d, float f) { return d * f; });
    binding.endClass();
}

static void registerMatrix(CppBindModule<LuaBinding> &module)
{
    auto bindinb = BEGIN_CLASS(module, Matrix);
    REG_CTOR(bindinb);
    REG_STATIC_METHOD(bindinb, Matrix, identity);
    REG_METHOD(bindinb, Matrix, isIdentity);
    REG_METHOD(bindinb, Matrix, getDeterminant);
    REG_METHOD(bindinb, Matrix, invert);
    REG_METHOD(bindinb, Matrix, transpose);
    REG_STATIC_METHOD(bindinb, Matrix, createLookAt);
    REG_STATIC_METHOD(bindinb, Matrix, createPerspective);
    REG_STATIC_METHOD(bindinb, Matrix, createOrthographic);
    REG_STATIC_METHOD(bindinb, Matrix, createScale);
    REG_STATIC_METHOD(bindinb, Matrix, createRotationFromQuaternion);
    REG_STATIC_METHOD(bindinb, Matrix, createRotationFromAxisAngle);
    REG_STATIC_METHOD(bindinb, Matrix, createTranslation);
    REG_METHOD(bindinb, Matrix, scale);
    REG_METHOD(bindinb, Matrix, translation);
    REG_METHOD(bindinb, Matrix, rotation);
    REG_METHOD(bindinb, Matrix, upVector);
    REG_METHOD(bindinb, Matrix, downVector);
    REG_METHOD(bindinb, Matrix, leftVector);
    REG_METHOD(bindinb, Matrix, rightVector);
    REG_METHOD(bindinb, Matrix, forwardVector);
    REG_METHOD(bindinb, Matrix, backVector);
    REG_METHOD(bindinb, Matrix, rotateByQuaternion);
    REG_METHOD(bindinb, Matrix, rotateByAxisAngle);
    REG_METHOD(bindinb, Matrix, scaleByScalar);
    REG_METHOD(bindinb, Matrix, scaleByVector);
    REG_METHOD(bindinb, Matrix, translate);
    REG_METHOD(bindinb, Matrix, transformPoint);
    REG_METHOD(bindinb, Matrix, transformDirection);
    REG_METHOD(bindinb, Matrix, transformRay);
    REG_METHOD(bindinb, Matrix, decompose);
    REG_META_METHOD(bindinb, "__mul", [](const Matrix &m1, const Matrix &m2) { return m1 * m2; });
    bindinb.endClass();
}

static void registerRay(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, Ray);
    REG_CTOR(binding, const Vector3&, const Vector3&);
    REG_METHOD(binding, Ray, origin);
    REG_METHOD(binding, Ray, setOrigin);
    REG_METHOD(binding, Ray, direction);
    REG_METHOD(binding, Ray, setDirection);
    binding.endClass();
}

void registerMathApi(CppBindModule<LuaBinding> &module)
{
    registerRadians(module);
    registerDegrees(module);
    registerVector2(module);
    registerVector3(module);
    registerVector4(module);
    registerQuaternion(module);
    registerMatrix(module);
    registerRay(module);
}
