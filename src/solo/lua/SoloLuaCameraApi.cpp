/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloCamera.h"
#include "math/SoloRay.h" // really needed
#include "SoloLuaCommon.h"

using namespace solo;

void registerCameraApi(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS_EXTEND(module, Camera, Component);
    REG_METHOD(binding, Camera, renderFrame);
    REG_METHOD(binding, Camera, transform);
    REG_METHOD(binding, Camera, renderTarget);
    REG_METHOD_NULLABLE_1ST_ARG(binding, Camera, setRenderTarget, sptr<FrameBuffer>);
    REG_METHOD(binding, Camera, clearColor);
    REG_METHOD(binding, Camera, setClearColor);
    REG_METHOD(binding, Camera, hasColorClearing);
    REG_METHOD(binding, Camera, setColorClearing);
    REG_METHOD(binding, Camera, viewport);
    REG_METHOD(binding, Camera, setViewport);
    REG_METHOD(binding, Camera, isPerspective);
    REG_METHOD(binding, Camera, setPerspective);
    REG_METHOD(binding, Camera, zNear);
    REG_METHOD(binding, Camera, setZNear);
    REG_METHOD(binding, Camera, zFar);
    REG_METHOD(binding, Camera, setZFar);
    REG_METHOD(binding, Camera, fieldOfView);
    REG_METHOD(binding, Camera, setFieldOfView);
    REG_METHOD(binding, Camera, orthoSize);
    REG_METHOD(binding, Camera, setOrthoSize);
    REG_METHOD(binding, Camera, windowPointToWorldRay);
    REG_METHOD(binding, Camera, aspectRatio);
    REG_METHOD(binding, Camera, viewMatrix);
    REG_METHOD(binding, Camera, invViewMatrix);
    REG_METHOD(binding, Camera, projectionMatrix);
    REG_METHOD(binding, Camera, viewProjectionMatrix);
    REG_METHOD(binding, Camera, invViewProjectionMatrix);
    REG_PTR_EQUALITY(binding, Camera);
    binding.endClass();
}
