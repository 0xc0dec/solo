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

#include "SoloFrameBuffer.h"
#include "SoloLuaCommon.h"
#include "SoloLogger.h"
#include "SoloScene.h"
#include "SoloDevice.h"
#include "SoloMeshRenderer.h"
#include "SoloEffect.h"
#include "SoloFileSystem.h"
#include "SoloAssetLoader.h"
#include "SoloRenderer.h"
#include "SoloPhysics.h"
#include "SoloRigidBody.h"
#include "SoloBoxCollider.h"
#include "SoloMesh.h"

using namespace solo;


static void registerFrameBuffer(CppBindModule<LuaBinding> &module)
{
    auto fb = module.beginClass<FrameBuffer>("FrameBuffer");
    REG_STATIC_METHOD(fb, FrameBuffer, create);
    REG_METHOD(fb, FrameBuffer, setAttachments);
    REG_METHOD(fb, FrameBuffer, getSize);
    fb.endClass();
}


static void registerLogger(CppBindModule<LuaBinding> &module)
{
    auto logger = module.beginClass<Logger>("Logger");
    REG_METHOD(logger, Logger, setTargetFile);
    REG_METHOD(logger, Logger, logDebug);
    REG_METHOD(logger, Logger, logInfo);
    REG_METHOD(logger, Logger, logWarning);
    REG_METHOD(logger, Logger, logError);
    REG_METHOD(logger, Logger, logCritical);
    logger.endClass();
}


static void registerScene(CppBindModule<LuaBinding> &module)
{
    auto scene = module.beginClass<Scene>("Scene");
    REG_STATIC_METHOD(scene, Scene, create);
    REG_METHOD(scene, Scene, getDevice);
    REG_METHOD(scene, Scene, createNode);
    REG_METHOD(scene, Scene, visit);
    REG_METHOD(scene, Scene, visitByTags);
    scene.endClass();
}


static void registerDevice(CppBindModule<LuaBinding> &module)
{
    auto device = module.beginClass<Device>("Device");
    REG_METHOD(device, Device, getWindowTitle);
    REG_METHOD(device, Device, setWindowTitle);
    REG_METHOD(device, Device, getCanvasSize);
    REG_METHOD(device, Device, saveScreenshot);
    REG_METHOD(device, Device, setCursorCaptured);
    REG_METHOD(device, Device, getLifetime);
    REG_METHOD(device, Device, getTimeDelta);
    REG_METHOD(device, Device, isWindowCloseRequested);
    REG_METHOD(device, Device, isQuitRequested);
    REG_METHOD(device, Device, isKeyPressed);
    REG_METHOD(device, Device, isKeyReleased);
    REG_METHOD(device, Device, getMouseMotion);
    REG_METHOD(device, Device, isMouseButtonDown);
    REG_METHOD(device, Device, isMouseButtonReleased);
    REG_METHOD(device, Device, update);
    REG_METHOD(device, Device, getSetup);
    REG_METHOD(device, Device, getFileSystem);
    REG_METHOD(device, Device, getAssetLoader);
    REG_METHOD(device, Device, getRenderer);
    REG_METHOD(device, Device, getPhysics);
    REG_METHOD(device, Device, getLogger);
    device.endClass();
}


static void registerMeshRenderer(CppBindModule<LuaBinding> &module)
{
    auto renderer = module.beginExtendClass<MeshRenderer, Component>("MeshRenderer");
    REG_METHOD(renderer, MeshRenderer, render);
    REG_METHOD(renderer, MeshRenderer, getMesh);
    REG_METHOD(renderer, MeshRenderer, setMesh);
    REG_METHOD(renderer, MeshRenderer, getMaterial);
    REG_METHOD(renderer, MeshRenderer, setMaterial);
    REG_METHOD(renderer, MeshRenderer, getMaterialCount);
    renderer.endClass();
}


static void registerEffect(CppBindModule<LuaBinding> &module)
{
    auto eff = module.beginClass<Effect>("Effect");
    REG_STATIC_METHOD(eff, Effect, create);
    REG_STATIC_METHOD(eff, Effect, createFromPrefab);
    eff.endClass();
}


static void registerDeviceSetup(CppBindModule<LuaBinding> &module)
{
    auto setup = module.beginClass<DeviceSetup>("DeviceSetup");
    REG_VARIABLE(setup, DeviceSetup, mode);
    REG_VARIABLE(setup, DeviceSetup, canvasWidth);
    REG_VARIABLE(setup, DeviceSetup, canvasHeight);
    REG_VARIABLE(setup, DeviceSetup, fullScreen);
    REG_VARIABLE(setup, DeviceSetup, windowTitle);
    REG_VARIABLE(setup, DeviceSetup, redBits);
    REG_VARIABLE(setup, DeviceSetup, greenBits);
    REG_VARIABLE(setup, DeviceSetup, blueBits);
    REG_VARIABLE(setup, DeviceSetup, depthBits);
    REG_VARIABLE(setup, DeviceSetup, alphaBits);
    REG_VARIABLE(setup, DeviceSetup, vsync);
    REG_VARIABLE(setup, DeviceSetup, logFilePath);
    setup.endClass();
}


static void registerFileSystem(CppBindModule<LuaBinding> &module)
{
    auto fs = module.beginClass<FileSystem>("FileSystem");
    REG_METHOD(fs, FileSystem, readBytes);
    REG_METHOD(fs, FileSystem, writeBytes);
    REG_METHOD(fs, FileSystem, readText);
    REG_METHOD(fs, FileSystem, readLines);
    REG_METHOD(fs, FileSystem, writeLines);
    REG_METHOD(fs, FileSystem, iterateLines);
    fs.endClass();
}


static void registerAsyncHandles(CppBindModule<LuaBinding> &module)
{
    auto meshAsyncHandle = module.beginClass<AsyncHandle<Mesh>>("MeshAsyncHandle");
    REG_METHOD(meshAsyncHandle, AsyncHandle<Mesh>, done);
    meshAsyncHandle.endClass();

    auto rectTexAsyncHandle = module.beginClass<AsyncHandle<RectTexture>>("RectTextureAsyncHandle");
    REG_METHOD(rectTexAsyncHandle, AsyncHandle<RectTexture>, done);
    rectTexAsyncHandle.endClass();

    auto cubeTexAsyncHandle = module.beginClass<AsyncHandle<CubeTexture>>("CubeTextureAsyncHandle");
    REG_METHOD(cubeTexAsyncHandle, AsyncHandle<CubeTexture>, done);
    cubeTexAsyncHandle.endClass();
}


static void registerAssetLoader(CppBindModule<LuaBinding> &module)
{
    auto loader = module.beginClass<AssetLoader>("AssetLoader");
    REG_METHOD(loader, AssetLoader, loadRectTexture);
    REG_METHOD(loader, AssetLoader, loadRectTextureAsync);
    REG_METHOD(loader, AssetLoader, loadCubeTexture);
    REG_METHOD(loader, AssetLoader, loadCubeTextureAsync);
    REG_METHOD(loader, AssetLoader, loadMesh);
    REG_METHOD(loader, AssetLoader, loadMeshAsync);
    REG_METHOD(loader, AssetLoader, update);
    loader.endClass();
}


static void registerRenderer(CppBindModule<LuaBinding> &module)
{
    auto renderer = module.beginClass<Renderer>("Renderer");
    REG_METHOD(renderer, Renderer, renderFrame);
    renderer.endClass();
}


static void registerRigidBodyConstructionParams(CppBindModule<LuaBinding> &module)
{
    auto params = module.beginClass<RigidBodyConstructionParameters>("RigidBodyConstructionParameters");
    params.addConstructor(LUA_ARGS());
    REG_VARIABLE(params, RigidBodyConstructionParameters, mass);
    REG_VARIABLE(params, RigidBodyConstructionParameters, friction);
    REG_VARIABLE(params, RigidBodyConstructionParameters, restitution);
    REG_VARIABLE(params, RigidBodyConstructionParameters, linearDamping);
    REG_VARIABLE(params, RigidBodyConstructionParameters, angularDamping);
    REG_VARIABLE(params, RigidBodyConstructionParameters, kinematic);
    REG_VARIABLE(params, RigidBodyConstructionParameters, linearFactor);
    REG_VARIABLE(params, RigidBodyConstructionParameters, angularFactor);
    params.endClass();
}


static void registerRigidBody(CppBindModule<LuaBinding> &module)
{
    auto rb = module.beginExtendClass<RigidBody, Component>("RigidBody");
    REG_METHOD(rb, RigidBody, setCollider);
    rb.endClass();
}


static void registerRaycastResult(CppBindModule<LuaBinding> &module)
{
    auto rcr = module.beginClass<RaycastResult>("RaycastResult");
    REG_VARIABLE(rcr, RaycastResult, body);
    REG_VARIABLE(rcr, RaycastResult, point);
    REG_VARIABLE(rcr, RaycastResult, normal);
    rcr.endClass();
}


static void registerPhysics(CppBindModule<LuaBinding> &module)
{
    auto ph = module.beginClass<Physics>("Physics");
    REG_METHOD(ph, Physics, update);
    REG_METHOD(ph, Physics, setGravity);
    REG_METHOD(ph, Physics, castRay);
    REG_METHOD(ph, Physics, castRayAll);
    ph.endClass();
}


static void registerColliders(CppBindModule<LuaBinding> &module)
{
    auto coll = module.beginClass<Collider>("Collider");
    coll.endClass();

    auto box = module.beginExtendClass<BoxCollider, Collider>("BoxCollider");
    REG_STATIC_METHOD(box, BoxCollider, create);
    box.endClass();
}


void registerOther(CppBindModule<LuaBinding> &module)
{
    registerPhysics(module);
    registerRigidBody(module);
    registerRigidBodyConstructionParams(module);
    registerRenderer(module);
    registerAssetLoader(module);
    registerFileSystem(module);
    registerDeviceSetup(module);
    registerEffect(module);
    registerMeshRenderer(module);
    registerDevice(module);
    registerScene(module);
    registerLogger(module);
    registerFrameBuffer(module);
    registerRaycastResult(module);
    registerColliders(module);
    registerAsyncHandles(module);
}
