/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloCamera.h"
#include "SoloRay.h" // really needed
#include "SoloLuaCommon.h"

using namespace solo;

void registerCameraApi(CppBindModule<LuaBinding> &module)
{
    auto camera = BEGIN_CLASS_EXTEND(module, Camera, Component);
    REG_METHOD(camera, Camera, renderFrame);
    REG_METHOD(camera, Camera, transform);
    REG_METHOD(camera, Camera, renderTarget);
    REG_METHOD_NULLABLE_1ST_ARG(camera, Camera, setRenderTarget, sptr<FrameBuffer>);
    REG_METHOD(camera, Camera, clearColor);
    REG_METHOD(camera, Camera, setClearColor);
    REG_METHOD(camera, Camera, hasColorClearing);
    REG_METHOD(camera, Camera, setColorClearing);
    REG_METHOD(camera, Camera, viewport);
    REG_METHOD(camera, Camera, setViewport);
    REG_METHOD(camera, Camera, isPerspective);
    REG_METHOD(camera, Camera, setPerspective);
    REG_METHOD(camera, Camera, zNear);
    REG_METHOD(camera, Camera, setZNear);
    REG_METHOD(camera, Camera, zFar);
    REG_METHOD(camera, Camera, setZFar);
    REG_METHOD(camera, Camera, fieldOfView);
    REG_METHOD(camera, Camera, setFieldOfView);
    REG_METHOD(camera, Camera, orthoSize);
    REG_METHOD(camera, Camera, setOrthoSize);
    REG_METHOD(camera, Camera, windowPointToWorldRay);
    REG_METHOD(camera, Camera, aspectRatio);
    REG_METHOD(camera, Camera, viewMatrix);
    REG_METHOD(camera, Camera, invViewMatrix);
    REG_METHOD(camera, Camera, projectionMatrix);
    REG_METHOD(camera, Camera, viewProjectionMatrix);
    REG_METHOD(camera, Camera, invViewProjectionMatrix);
    camera.endClass();
}
