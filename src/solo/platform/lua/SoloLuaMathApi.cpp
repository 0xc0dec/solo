/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
    auto vector2 = module.beginClass<Vector2>("Vector2");
    
    vector2.addConstructor(LUA_ARGS(float, float));
    
    REG_VARIABLE(vector2, Vector2, x);
    REG_VARIABLE(vector2, Vector2, y);
    
    REG_METHOD(vector2, Vector2, isUnit);
    REG_METHOD(vector2, Vector2, isZero);

    REG_STATIC_METHOD(vector2, Vector2, angle);
    REG_METHOD(vector2, Vector2, clamp);

    REG_METHOD(vector2, Vector2, distance);
    REG_METHOD(vector2, Vector2, distanceSquared);

    REG_STATIC_OVERLOADED_METHOD(vector2, Vector2, dot, "dot", float, , const Vector2&, const Vector2&);
    REG_OVERLOADED_METHOD(vector2, Vector2, dot, "dot", float, const, const Vector2&);
    
    REG_METHOD(vector2, Vector2, length);
    REG_METHOD(vector2, Vector2, lengthSquared);
    
    REG_METHOD(vector2, Vector2, normalize);
    REG_METHOD(vector2, Vector2, normalized);

    vector2.endClass();
}


static void registerVector3(CppBindModule<LuaBinding> &module)
{
    auto vector3 = module.beginClass<Vector3>("Vector3");
    
    vector3.addConstructor(LUA_ARGS(float, float, float));

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
    
    REG_STATIC_OVERLOADED_METHOD(vector3, Vector3, dot, "dot", float, , const Vector3&, const Vector3&);
    REG_OVERLOADED_METHOD(vector3, Vector3, dot, "dot", float, const, const Vector3&);
    
    REG_METHOD(vector3, Vector3, length);
    REG_METHOD(vector3, Vector3, lengthSquared);
    
    REG_METHOD(vector3, Vector3, normalize);
    REG_METHOD(vector3, Vector3, normalized);

    vector3.endClass();
}


static void registerVector4(CppBindModule<LuaBinding> &module)
{
    auto vector4 = module.beginClass<Vector4>("Vector4");
    
    vector4.addConstructor(LUA_ARGS(float, float, float, float));
    
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
    
    REG_STATIC_OVERLOADED_METHOD(vector4, Vector4, dot, "dot", float, , const Vector4&, const Vector4&);
    REG_OVERLOADED_METHOD(vector4, Vector4, dot, "dot", float, const, const Vector4&);
    
    REG_METHOD(vector4, Vector4, length);
    REG_METHOD(vector4, Vector4, lengthSquared);
    
    REG_METHOD(vector4, Vector4, normalize);
    REG_METHOD(vector4, Vector4, normalized);

    vector4.endClass();
}


static void registerQuaternion(CppBindModule<LuaBinding> &module)
{
    auto q = module.beginClass<Quaternion>("Quaternion");
    
    q.addConstructor(LUA_ARGS());

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
    
    q.endClass();
}


static void registerRadian(CppBindModule<LuaBinding> &module)
{
    auto radian = module.beginClass<Radian>("Radian");
    radian.addConstructor(LUA_ARGS(float));
    REG_METHOD(radian, Radian, toRawDegree);
    REG_METHOD(radian, Radian, toRawRadian);
    radian.endClass();
}


static void registerDegree(CppBindModule<LuaBinding> &module)
{
    auto degree = module.beginClass<Degree>("Degree");
    degree.addConstructor(LUA_ARGS(float));
    REG_METHOD(degree, Degree, toRawDegree);
    REG_METHOD(degree, Degree, toRawRadian);
    degree.endClass();
}


static void registerMatrix(CppBindModule<LuaBinding> &module)
{
    auto matrix = module.beginClass<Matrix>("Matrix");
    
    matrix.addConstructor(LUA_ARGS());
    
    REG_STATIC_METHOD(matrix, Matrix, identity);
    REG_STATIC_METHOD(matrix, Matrix, getSize);
    
    REG_METHOD(matrix, Matrix, isIdentity);
    
    REG_METHOD(matrix, Matrix, getDeterminant);
    
    REG_METHOD(matrix, Matrix, invert);
    
    REG_METHOD(matrix, Matrix, transpose);
    
    REG_METHOD(matrix, Matrix, resetToIdentity);
    REG_METHOD(matrix, Matrix, resetToZero);
    
    matrix.endClass();
}


static void registerTransformMatrix(CppBindModule<LuaBinding> &module)
{
    auto matrix = module.beginClass<TransformMatrix>("TransformMatrix");
    
    matrix.addConstructor(LUA_ARGS());

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
    auto bb = module.beginClass<BoundingBox>("BoundingBox");
    
    bb.addConstructor(LUA_ARGS(const Vector3&, const Vector3&));

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
    auto bs = module.beginClass<BoundingSphere>("BoundingSphere");
    
    REG_VARIABLE(bs, BoundingSphere, center);
    REG_VARIABLE(bs, BoundingSphere, radius);
    
    bs.addConstructor(LUA_ARGS(const Vector3&, float));
    
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
    auto plane = module.beginClass<Plane>("Plane");

    plane.addConstructor(LUA_ARGS(const Vector3&, float));

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
    auto ray = module.beginClass<Ray>("Ray");
    
    ray.addConstructor(LUA_ARGS(const Vector3&, const Vector3&));
    
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
    auto f = module.beginClass<Frustum>("Frustum");
    
    f.addConstructor(LUA_ARGS());
    
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


void registerMath(CppBindModule<LuaBinding> &module)
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
