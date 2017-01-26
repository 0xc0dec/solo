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
#include "SoloRay.h"
#include "SoloLuaCommon.h"

using namespace solo;
using namespace LuaIntf;


static void registerVector2(CppBindModule<LuaBinding> &module)
{
    auto vector2 = module.beginClass<Vector2>("Vector2");
    vector2.addConstructor(LUA_ARGS(float, float));
    
    REGISTER_VARIABLE(vector2, Vector2, x);
    REGISTER_VARIABLE(vector2, Vector2, y);
    
    REGISTER_STATIC_METHOD(vector2, Vector2, zero);
    REGISTER_STATIC_METHOD(vector2, Vector2, unit);
    REGISTER_STATIC_METHOD(vector2, Vector2, unitX);
    REGISTER_STATIC_METHOD(vector2, Vector2, unitY);
    
    REGISTER_METHOD(vector2, Vector2, isUnit);
    REGISTER_METHOD(vector2, Vector2, isZero);

    REGISTER_STATIC_METHOD(vector2, Vector2, angle);
    REGISTER_METHOD(vector2, Vector2, clamp);

    REGISTER_METHOD(vector2, Vector2, distance);
    REGISTER_METHOD(vector2, Vector2, distanceSquared);

    REGISTER_STATIC_OVERLOADED_METHOD(vector2, Vector2, dot, "dot", float, , const Vector2&, const Vector2&);
    REGISTER_OVERLOADED_METHOD(vector2, Vector2, dot, "dot", float, const, const Vector2&);
    
    REGISTER_METHOD(vector2, Vector2, length);
    REGISTER_METHOD(vector2, Vector2, lengthSquared);
    
    REGISTER_METHOD(vector2, Vector2, normalize);
    REGISTER_METHOD(vector2, Vector2, normalized);

    vector2.endClass();
}


static void registerVector3(CppBindModule<LuaBinding> &module)
{
    auto vector3 = module.beginClass<Vector3>("Vector3");
    vector3.addConstructor(LUA_ARGS(float, float, float));

    REGISTER_VARIABLE(vector3, Vector3, x);
    REGISTER_VARIABLE(vector3, Vector3, y);
    REGISTER_VARIABLE(vector3, Vector3, z);

    REGISTER_STATIC_METHOD(vector3, Vector3, unit);
    REGISTER_STATIC_METHOD(vector3, Vector3, unitX);
    REGISTER_STATIC_METHOD(vector3, Vector3, unitY);
    REGISTER_STATIC_METHOD(vector3, Vector3, unitZ);

    REGISTER_METHOD(vector3, Vector3, isUnit);
    REGISTER_METHOD(vector3, Vector3, isZero);

    REGISTER_STATIC_METHOD(vector3, Vector3, angle);
    REGISTER_METHOD(vector3, Vector3, clamp);

    REGISTER_STATIC_METHOD(vector3, Vector3, cross);

    REGISTER_METHOD(vector3, Vector3, distance);
    REGISTER_METHOD(vector3, Vector3, distanceSquared);
    
    REGISTER_STATIC_OVERLOADED_METHOD(vector3, Vector3, dot, "dot", float, , const Vector3&, const Vector3&);
    REGISTER_OVERLOADED_METHOD(vector3, Vector3, dot, "dot", float, const, const Vector3&);
    
    REGISTER_METHOD(vector3, Vector3, length);
    REGISTER_METHOD(vector3, Vector3, lengthSquared);
    
    REGISTER_METHOD(vector3, Vector3, normalize);
    REGISTER_METHOD(vector3, Vector3, normalized);

    vector3.endClass();
}


static void registerVector4(CppBindModule<LuaBinding> &module)
{
    auto vector4 = module.beginClass<Vector4>("Vector4");
    vector4.addConstructor(LUA_ARGS(float, float, float, float));
    
    REGISTER_VARIABLE(vector4, Vector4, x);
    REGISTER_VARIABLE(vector4, Vector4, y);
    REGISTER_VARIABLE(vector4, Vector4, z);
    REGISTER_VARIABLE(vector4, Vector4, w);
    
    REGISTER_STATIC_METHOD(vector4, Vector4, unit);
    REGISTER_STATIC_METHOD(vector4, Vector4, unitX);
    REGISTER_STATIC_METHOD(vector4, Vector4, unitY);
    REGISTER_STATIC_METHOD(vector4, Vector4, unitZ);
    REGISTER_STATIC_METHOD(vector4, Vector4, unitW);

    REGISTER_METHOD(vector4, Vector4, isUnit);
    REGISTER_METHOD(vector4, Vector4, isZero);
    
    REGISTER_STATIC_METHOD(vector4, Vector4, angle);
    
    REGISTER_METHOD(vector4, Vector4, clamp);

    REGISTER_METHOD(vector4, Vector4, distance);
    REGISTER_METHOD(vector4, Vector4, distanceSquared);
    
    REGISTER_STATIC_OVERLOADED_METHOD(vector4, Vector4, dot, "dot", float, , const Vector4&, const Vector4&);
    REGISTER_OVERLOADED_METHOD(vector4, Vector4, dot, "dot", float, const, const Vector4&);
    
    REGISTER_METHOD(vector4, Vector4, length);
    REGISTER_METHOD(vector4, Vector4, lengthSquared);
    
    REGISTER_METHOD(vector4, Vector4, normalize);
    REGISTER_METHOD(vector4, Vector4, normalized);

    vector4.endClass();
}


static void registerQuaternion(CppBindModule<LuaBinding> &module)
{
    auto q = module.beginClass<Quaternion>("Quaternion");
    q.addConstructor(LUA_ARGS());

    REGISTER_VARIABLE(q, Quaternion, x);
    REGISTER_VARIABLE(q, Quaternion, y);
    REGISTER_VARIABLE(q, Quaternion, z);
    REGISTER_VARIABLE(q, Quaternion, w);

    REGISTER_STATIC_METHOD(q, Quaternion, createFromAxisAngle);

    REGISTER_STATIC_METHOD(q, Quaternion, identity);

    REGISTER_METHOD(q, Quaternion, isIdentity);
    REGISTER_METHOD(q, Quaternion, isZero);

    REGISTER_METHOD(q, Quaternion, conjugate);
    REGISTER_METHOD(q, Quaternion, inverse);

    REGISTER_METHOD(q, Quaternion, normalize);
    REGISTER_METHOD(q, Quaternion, normalized);

    REGISTER_METHOD(q, Quaternion, toAxisAngle);
    
    REGISTER_STATIC_METHOD(q, Quaternion, lerp);
    REGISTER_STATIC_METHOD(q, Quaternion, slerp);
    REGISTER_STATIC_METHOD(q, Quaternion, squad);
    
    q.endClass();
}


static void registerRadian(CppBindModule<LuaBinding> &module)
{
    auto radian = module.beginClass<Radian>("Radian");
    radian.addConstructor(LUA_ARGS(float));
    REGISTER_METHOD(radian, Radian, toRawDegree);
    REGISTER_METHOD(radian, Radian, toRawRadian);
    radian.endClass();
}


static void registerDegree(CppBindModule<LuaBinding> &module)
{
    auto degree = module.beginClass<Degree>("Degree");
    degree.addConstructor(LUA_ARGS(float));
    REGISTER_METHOD(degree, Degree, toRawDegree);
    REGISTER_METHOD(degree, Degree, toRawRadian);
    degree.endClass();
}


static void registerMatrix(CppBindModule<LuaBinding> &module)
{
    auto matrix = module.beginClass<Matrix>("Matrix");
    matrix.addConstructor(LUA_ARGS());
    
    REGISTER_STATIC_METHOD(matrix, Matrix, identity);
    REGISTER_STATIC_METHOD(matrix, Matrix, getSize);
    
    REGISTER_METHOD(matrix, Matrix, isIdentity);
    REGISTER_METHOD(matrix, Matrix, getDeterminant);
    REGISTER_METHOD(matrix, Matrix, invert);
    REGISTER_METHOD(matrix, Matrix, transpose);
    REGISTER_METHOD(matrix, Matrix, makeIdentity);
    REGISTER_METHOD(matrix, Matrix, makeZero);
    
    matrix.endClass();
}


static void registerTransformMatrix(CppBindModule<LuaBinding> &module)
{
    auto matrix = module.beginClass<TransformMatrix>("TransformMatrix");
    matrix.addConstructor(LUA_ARGS());

    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createLookAt);
    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createPerspective);
    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createOrthographic);
    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createReflection);
    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createScale);
    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createRotationFromQuaternion);
    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createRotationFromAxisAngle);
    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createRotationX);
    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createRotationY);
    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createRotationZ);
    REGISTER_STATIC_METHOD(matrix, TransformMatrix, createTranslation);

    REGISTER_METHOD(matrix, TransformMatrix, getScale);
    REGISTER_METHOD(matrix, TransformMatrix, getTranslation);
    REGISTER_METHOD(matrix, TransformMatrix, getRotation);
    
    REGISTER_METHOD(matrix, TransformMatrix, getUpVector);
    REGISTER_METHOD(matrix, TransformMatrix, getDownVector);
    REGISTER_METHOD(matrix, TransformMatrix, getLeftVector);
    REGISTER_METHOD(matrix, TransformMatrix, getRightVector);
    REGISTER_METHOD(matrix, TransformMatrix, getForwardVector);
    REGISTER_METHOD(matrix, TransformMatrix, getBackVector);

    REGISTER_METHOD(matrix, TransformMatrix, rotateByQuaternion);
    REGISTER_METHOD(matrix, TransformMatrix, rotateByAxisAngle);
    REGISTER_METHOD(matrix, TransformMatrix, rotateX);
    REGISTER_METHOD(matrix, TransformMatrix, rotateY);
    REGISTER_METHOD(matrix, TransformMatrix, rotateZ);

    REGISTER_METHOD(matrix, TransformMatrix, scaleByScalar);
    REGISTER_METHOD(matrix, TransformMatrix, scaleByVector);

    REGISTER_METHOD(matrix, TransformMatrix, translate);

    REGISTER_METHOD(matrix, TransformMatrix, transformPoint);
    REGISTER_METHOD(matrix, TransformMatrix, transformDirection);
    REGISTER_METHOD(matrix, TransformMatrix, transformBoundingBox);
    REGISTER_METHOD(matrix, TransformMatrix, transformBoundingSphere);
    REGISTER_METHOD(matrix, TransformMatrix, transformPlane);
    REGISTER_METHOD(matrix, TransformMatrix, transformRay);
    
    REGISTER_METHOD(matrix, TransformMatrix, decompose);

    matrix.endClass();
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
}
