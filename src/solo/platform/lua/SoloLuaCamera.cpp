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
#include "SoloRenderContext.h"
#include "SoloLuaCommon.h"

using namespace solo;


void registerRenderContext(CppBindModule<LuaBinding> &module)
{
    auto rc = module.beginClass<RenderContext>("RenderContext");
    REGISTER_VARIABLE(rc, RenderContext, camera);
    rc.endClass();
}


void registerCamera(CppBindModule<LuaBinding> &module)
{
    auto camera = module.beginExtendClass<Camera, Component>("Camera");
    
    REGISTER_METHOD(camera, Camera, render);
    
    REGISTER_METHOD(camera, Camera, getTransform);
    
    REGISTER_METHOD(camera, Camera, getRenderTarget);
    REGISTER_METHOD(camera, Camera, setRenderTarget);

    REGISTER_METHOD(camera, Camera, getClearColor);
    REGISTER_METHOD(camera, Camera, setClearColor);

    REGISTER_METHOD(camera, Camera, isClearColorEnabled);
    REGISTER_METHOD(camera, Camera, setClearColorEnabled);

    REGISTER_METHOD(camera, Camera, isClearDepthEnabled);
    REGISTER_METHOD(camera, Camera, setClearDepthEnabled);

    REGISTER_METHOD(camera, Camera, getViewport);
    REGISTER_METHOD(camera, Camera, setViewport);

    REGISTER_METHOD(camera, Camera, isPerspective);
    REGISTER_METHOD(camera, Camera, setPerspective);

    REGISTER_METHOD(camera, Camera, getNear);
    REGISTER_METHOD(camera, Camera, setNear);

    REGISTER_METHOD(camera, Camera, getFar);
    REGISTER_METHOD(camera, Camera, setFar);

    REGISTER_METHOD(camera, Camera, getFOV);
    REGISTER_METHOD(camera, Camera, setFOV);

    REGISTER_METHOD(camera, Camera, getWidth);
    REGISTER_METHOD(camera, Camera, setWidth);

    REGISTER_METHOD(camera, Camera, getHeight);
    REGISTER_METHOD(camera, Camera, setHeight);

    REGISTER_METHOD(camera, Camera, getAspectRatio);
    REGISTER_METHOD(camera, Camera, setAspectRatio);

    REGISTER_METHOD(camera, Camera, getViewMatrix);
    REGISTER_METHOD(camera, Camera, getInvViewMatrix);
    REGISTER_METHOD(camera, Camera, getProjectionMatrix);
    REGISTER_METHOD(camera, Camera, getViewProjectionMatrix);
    REGISTER_METHOD(camera, Camera, getInvViewProjectionMatrix);
    
    camera.endClass();
}