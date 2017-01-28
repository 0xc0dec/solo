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


void registerTransform(CppBindModule<LuaBinding> &module)
{
    // TODO script transform callbacks

    // Transform
    auto transform = module.beginExtendClass<Transform, Component>("Transform");
    
    REGISTER_METHOD(transform, Transform, getParent);
    REGISTER_METHOD(transform, Transform, setParent);

    REGISTER_METHOD(transform, Transform, getChild);
    REGISTER_METHOD(transform, Transform, getChildrenCount);
    REGISTER_METHOD(transform, Transform, clearChildren);

    REGISTER_METHOD(transform, Transform, getWorldScale);
    REGISTER_METHOD(transform, Transform, getLocalScale);

    REGISTER_METHOD(transform, Transform, getWorldRotation);
    REGISTER_METHOD(transform, Transform, getLocalRotation);

    REGISTER_METHOD(transform, Transform, getWorldPosition);
    REGISTER_METHOD(transform, Transform, getLocalPosition);

    REGISTER_METHOD(transform, Transform, getWorldUp);
    REGISTER_METHOD(transform, Transform, getLocalUp);

    REGISTER_METHOD(transform, Transform, getWorldDown);
    REGISTER_METHOD(transform, Transform, getLocalDown);

    REGISTER_METHOD(transform, Transform, getWorldLeft);
    REGISTER_METHOD(transform, Transform, getLocalLeft);

    REGISTER_METHOD(transform, Transform, getWorldRight);
    REGISTER_METHOD(transform, Transform, getLocalRight);

    REGISTER_METHOD(transform, Transform, getWorldForward);
    REGISTER_METHOD(transform, Transform, getLocalForward);

    REGISTER_METHOD(transform, Transform, getWorldBack);
    REGISTER_METHOD(transform, Transform, getLocalBack);

    REGISTER_METHOD(transform, Transform, translateLocal);
    REGISTER_METHOD(transform, Transform, scaleLocal);

    REGISTER_METHOD(transform, Transform, setLocalPosition);
    REGISTER_METHOD(transform, Transform, setLocalScale);

    REGISTER_METHOD(transform, Transform, rotate);
    REGISTER_METHOD(transform, Transform, rotateByAxisAngle);
    
    REGISTER_METHOD(transform, Transform, setLocalRotation);
    REGISTER_METHOD(transform, Transform, setLocalAxisAngleRotation);
    
    REGISTER_METHOD(transform, Transform, lookAt);

    REGISTER_METHOD(transform, Transform, getMatrix);
    REGISTER_METHOD(transform, Transform, getWorldMatrix);
    REGISTER_METHOD(transform, Transform, getWorldViewMatrix);
    REGISTER_METHOD(transform, Transform, getWorldViewProjMatrix);
    REGISTER_METHOD(transform, Transform, getInvTransposedWorldMatrix);
    REGISTER_METHOD(transform, Transform, getInvTransposedWorldViewMatrix);
    
    REGISTER_METHOD(transform, Transform, transformPoint);
    REGISTER_METHOD(transform, Transform, transformDirection);
    
    transform.endClass();
}