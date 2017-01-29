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

#include "SoloCamera.h"
#include "SoloLuaCommon.h"
#include "SoloRenderContext.h"

using namespace solo;


void registerCameraApi(CppBindModule<LuaBinding> &module)
{
    auto camera = BEGIN_CLASS_EXTEND(module, Camera, Component);
    REG_METHOD(camera, Camera, renderFrame);
    REG_METHOD(camera, Camera, getTransform);
    REG_METHOD(camera, Camera, getRenderTarget);
    REG_METHOD(camera, Camera, setRenderTarget);
    REG_METHOD(camera, Camera, getClearColor);
    REG_METHOD(camera, Camera, setClearColor);
    REG_METHOD(camera, Camera, isClearColorEnabled);
    REG_METHOD(camera, Camera, setClearColorEnabled);
    REG_METHOD(camera, Camera, isClearDepthEnabled);
    REG_METHOD(camera, Camera, setClearDepthEnabled);
    REG_METHOD(camera, Camera, getViewport);
    REG_METHOD(camera, Camera, setViewport);
    REG_METHOD(camera, Camera, isPerspective);
    REG_METHOD(camera, Camera, setPerspective);
    REG_METHOD(camera, Camera, getNear);
    REG_METHOD(camera, Camera, setNear);
    REG_METHOD(camera, Camera, getFar);
    REG_METHOD(camera, Camera, setFar);
    REG_METHOD(camera, Camera, getFOV);
    REG_METHOD(camera, Camera, setFOV);
    REG_METHOD(camera, Camera, getWidth);
    REG_METHOD(camera, Camera, setWidth);
    REG_METHOD(camera, Camera, getHeight);
    REG_METHOD(camera, Camera, setHeight);
    REG_METHOD(camera, Camera, getAspectRatio);
    REG_METHOD(camera, Camera, setAspectRatio);
    REG_METHOD(camera, Camera, getViewMatrix);
    REG_METHOD(camera, Camera, getInvViewMatrix);
    REG_METHOD(camera, Camera, getProjectionMatrix);
    REG_METHOD(camera, Camera, getViewProjectionMatrix);
    REG_METHOD(camera, Camera, getInvViewProjectionMatrix);
    camera.endClass();
}