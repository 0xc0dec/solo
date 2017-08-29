/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloCamera.h"
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
    REG_METHOD(camera, Camera, getOrthoSize);
    REG_METHOD(camera, Camera, setOrthoSize);
    REG_METHOD(camera, Camera, getAspectRatio);
    REG_METHOD(camera, Camera, setAspectRatio);
    REG_METHOD(camera, Camera, getViewMatrix);
    REG_METHOD(camera, Camera, getInvViewMatrix);
    REG_METHOD(camera, Camera, getProjectionMatrix);
    REG_METHOD(camera, Camera, getViewProjectionMatrix);
    REG_METHOD(camera, Camera, getInvViewProjectionMatrix);
    camera.endClass();
}