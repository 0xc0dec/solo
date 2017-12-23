/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloCamera.h"
#include "SoloRay.h" // really needed
#include "SoloLuaCommon.h"

using namespace solo;

void registerCameraApi(CppBindModule<LuaBinding> &module)
{
    auto camera = BEGIN_CLASS_EXTEND(module, Camera, Component);
    REG_METHOD(camera, Camera, renderFrame);
    REG_METHOD(camera, Camera, getTransform);
    REG_METHOD(camera, Camera, getRenderTarget);
    REG_METHOD_NULLABLE_1ST_ARG(camera, Camera, setRenderTarget, sptr<FrameBuffer>);
    REG_METHOD(camera, Camera, getClearColor);
    REG_METHOD(camera, Camera, setClearColor);
    REG_METHOD(camera, Camera, hasColorClearing);
    REG_METHOD(camera, Camera, setColorClearing);
    REG_METHOD(camera, Camera, getViewport);
    REG_METHOD(camera, Camera, setViewport);
    REG_METHOD(camera, Camera, isPerspective);
    REG_METHOD(camera, Camera, setPerspective);
    REG_METHOD(camera, Camera, getZNear);
    REG_METHOD(camera, Camera, setZNear);
    REG_METHOD(camera, Camera, getZFar);
    REG_METHOD(camera, Camera, setZFar);
    REG_METHOD(camera, Camera, getFOV);
    REG_METHOD(camera, Camera, setFOV);
    REG_METHOD(camera, Camera, getOrthoSize);
    REG_METHOD(camera, Camera, setOrthoSize);
    REG_METHOD(camera, Camera, windowPointToWorldRay);
    REG_METHOD(camera, Camera, getAspectRatio);
    REG_METHOD(camera, Camera, getViewMatrix);
    REG_METHOD(camera, Camera, getInvViewMatrix);
    REG_METHOD(camera, Camera, getProjectionMatrix);
    REG_METHOD(camera, Camera, getViewProjectionMatrix);
    REG_METHOD(camera, Camera, getInvViewProjectionMatrix);
    camera.endClass();
}
