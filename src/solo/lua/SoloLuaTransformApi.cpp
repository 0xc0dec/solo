/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloTransform.h"
#include "SoloLuaCommon.h"

using namespace solo;

void registerTransformApi(CppBindModule<LuaBinding> &module)
{
    // TODO script transform callbacks

    auto transform = BEGIN_CLASS_EXTEND(module, Transform, Component);
    REG_METHOD(transform, Transform, parent);
    REG_METHOD_NULLABLE_1ST_ARG(transform, Transform, setParent, Transform*);
    REG_METHOD(transform, Transform, child);
    REG_METHOD(transform, Transform, childrenCount);
    REG_METHOD(transform, Transform, clearChildren);
    REG_METHOD(transform, Transform, worldScale);
    REG_METHOD(transform, Transform, localScale);
    REG_METHOD(transform, Transform, worldRotation);
    REG_METHOD(transform, Transform, localRotation);
    REG_METHOD(transform, Transform, worldPosition);
    REG_METHOD(transform, Transform, localPosition);
    REG_METHOD(transform, Transform, worldUp);
    REG_METHOD(transform, Transform, localUp);
    REG_METHOD(transform, Transform, worldDown);
    REG_METHOD(transform, Transform, localDown);
    REG_METHOD(transform, Transform, worldLeft);
    REG_METHOD(transform, Transform, localLeft);
    REG_METHOD(transform, Transform, worldRight);
    REG_METHOD(transform, Transform, localRight);
    REG_METHOD(transform, Transform, worldForward);
    REG_METHOD(transform, Transform, localForward);
    REG_METHOD(transform, Transform, worldBack);
    REG_METHOD(transform, Transform, localBack);
    REG_METHOD(transform, Transform, translateLocal);
    REG_METHOD(transform, Transform, scaleLocal);
    REG_METHOD(transform, Transform, setLocalPosition);
    REG_METHOD(transform, Transform, setLocalScale);
    REG_METHOD(transform, Transform, rotate);
    REG_METHOD(transform, Transform, rotateByAxisAngle);
    REG_METHOD(transform, Transform, setLocalRotation);
    REG_METHOD(transform, Transform, setLocalAxisAngleRotation);
    REG_METHOD(transform, Transform, lookAt);
    REG_METHOD(transform, Transform, matrix);
    REG_METHOD(transform, Transform, worldMatrix);
    REG_METHOD(transform, Transform, worldViewMatrix);
    REG_METHOD(transform, Transform, worldViewProjMatrix);
    REG_METHOD(transform, Transform, invTransposedWorldMatrix);
    REG_METHOD(transform, Transform, invTransposedWorldViewMatrix);
    REG_METHOD(transform, Transform, transformPoint);
    REG_METHOD(transform, Transform, transformDirection);
    REG_PTR_EQUALITY(transform, Transform);
    transform.endClass();
}