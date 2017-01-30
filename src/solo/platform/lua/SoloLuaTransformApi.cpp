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

#include "SoloTransform.h"
#include "SoloLuaCommon.h"

using namespace solo;


void registerTransformApi(CppBindModule<LuaBinding> &module)
{
    // TODO script transform callbacks

    auto transform = BEGIN_CLASS_EXTEND(module, Transform, Component);
    REG_METHOD(transform, Transform, getParent);
    REG_METHOD_NULLABLE_1ST_ARG(transform, Transform, setParent, Transform*);
    REG_METHOD(transform, Transform, getChild);
    REG_METHOD(transform, Transform, getChildrenCount);
    REG_METHOD(transform, Transform, clearChildren);
    REG_METHOD(transform, Transform, getWorldScale);
    REG_METHOD(transform, Transform, getLocalScale);
    REG_METHOD(transform, Transform, getWorldRotation);
    REG_METHOD(transform, Transform, getLocalRotation);
    REG_METHOD(transform, Transform, getWorldPosition);
    REG_METHOD(transform, Transform, getLocalPosition);
    REG_METHOD(transform, Transform, getWorldUp);
    REG_METHOD(transform, Transform, getLocalUp);
    REG_METHOD(transform, Transform, getWorldDown);
    REG_METHOD(transform, Transform, getLocalDown);
    REG_METHOD(transform, Transform, getWorldLeft);
    REG_METHOD(transform, Transform, getLocalLeft);
    REG_METHOD(transform, Transform, getWorldRight);
    REG_METHOD(transform, Transform, getLocalRight);
    REG_METHOD(transform, Transform, getWorldForward);
    REG_METHOD(transform, Transform, getLocalForward);
    REG_METHOD(transform, Transform, getWorldBack);
    REG_METHOD(transform, Transform, getLocalBack);
    REG_METHOD(transform, Transform, translateLocal);
    REG_METHOD(transform, Transform, scaleLocal);
    REG_METHOD(transform, Transform, setLocalPosition);
    REG_METHOD(transform, Transform, setLocalScale);
    REG_METHOD(transform, Transform, rotate);
    REG_METHOD(transform, Transform, rotateByAxisAngle);
    REG_METHOD(transform, Transform, setLocalRotation);
    REG_METHOD(transform, Transform, setLocalAxisAngleRotation);
    REG_METHOD(transform, Transform, lookAt);
    REG_METHOD(transform, Transform, getMatrix);
    REG_METHOD(transform, Transform, getWorldMatrix);
    REG_METHOD(transform, Transform, getWorldViewMatrix);
    REG_METHOD(transform, Transform, getWorldViewProjMatrix);
    REG_METHOD(transform, Transform, getInvTransposedWorldMatrix);
    REG_METHOD(transform, Transform, getInvTransposedWorldViewMatrix);
    REG_METHOD(transform, Transform, transformPoint);
    REG_METHOD(transform, Transform, transformDirection);
    transform.endClass();
}