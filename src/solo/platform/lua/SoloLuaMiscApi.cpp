/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFrameBuffer.h"
#include "SoloLuaCommon.h"
#include "SoloLogger.h"
#include "SoloScene.h"
#include "SoloMeshRenderer.h"
#include "SoloSkyboxRenderer.h"
#include "SoloEffect.h"
#include "SoloFileSystem.h"
#include "SoloAssetLoader.h"
#include "SoloRenderer.h"
#include "SoloMesh.h"
#include "SoloRenderContext.h"
#include "SoloSpectator.h"

using namespace solo;


static auto createEffect(Device *device, const std::vector<uint8_t> &vsSrc, const std::vector<uint8_t> &fsSrc) -> sptr<Effect>
{
    return Effect::create(device, vsSrc.data(), vsSrc.size(), fsSrc.data(), fsSrc.size());
}


static void registerFrameBuffer(CppBindModule<LuaBinding> &module)
{
    auto fb = BEGIN_CLASS(module, FrameBuffer);
    REG_STATIC_METHOD(fb, FrameBuffer, create);
    REG_METHOD(fb, FrameBuffer, setAttachments);
    REG_METHOD(fb, FrameBuffer, getSize);
    fb.endClass();
}


static void registerLogger(CppBindModule<LuaBinding> &module)
{
    auto logger = BEGIN_CLASS(module, Logger);
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
    auto scene = BEGIN_CLASS(module, Scene);
    REG_STATIC_METHOD(scene, Scene, create);
    REG_METHOD(scene, Scene, getDevice);
    REG_METHOD(scene, Scene, createNode);
    REG_METHOD(scene, Scene, visit);
    REG_METHOD(scene, Scene, visitByTags);
    scene.endClass();
}


static void registerMeshRenderer(CppBindModule<LuaBinding> &module)
{
    auto renderer = BEGIN_CLASS_EXTEND(module, MeshRenderer, Component);
    REG_METHOD(renderer, MeshRenderer, render);
    REG_METHOD(renderer, MeshRenderer, getMesh);
    REG_METHOD_NULLABLE_1ST_ARG(renderer, MeshRenderer, setMesh, sptr<Mesh>);
    REG_METHOD(renderer, MeshRenderer, getMaterial);
    REG_METHOD_NULLABLE_2ND_ARG(renderer, MeshRenderer, setMaterial, uint32_t, sptr<Material>);
    REG_METHOD(renderer, MeshRenderer, getMaterialCount);
    renderer.endClass();
}


static void registerSpectator(CppBindModule<LuaBinding> &module)
{
    auto spec = BEGIN_CLASS_EXTEND(module, Spectator, Component);
    REG_METHOD(spec, Spectator, getMovementSpeed);
    REG_METHOD(spec, Spectator, setMovementSpeed);
    REG_METHOD(spec, Spectator, getMouseSensitivity);
    REG_METHOD(spec, Spectator, setMouseSensitivity);
    REG_METHOD(spec, Spectator, getRotationAcceleration);
    REG_METHOD(spec, Spectator, setRotationAcceleration);
}

static void registerSkyboxRenderer(CppBindModule<LuaBinding> &module)
{
    auto r = BEGIN_CLASS_EXTEND(module, SkyboxRenderer, Component);
    REG_METHOD_NULLABLE_1ST_ARG(r, SkyboxRenderer, setTexture, sptr<CubeTexture>);
    REG_METHOD(r, SkyboxRenderer, getTexture);
    r.endClass();
}


static void registerEffect(CppBindModule<LuaBinding> &module)
{
    auto eff = BEGIN_CLASS(module, Effect);
    REG_FREE_FUNC_AS_STATIC_FUNC_RENAMED(eff, createEffect, "create");
    REG_STATIC_METHOD(eff, Effect, createFromPrefab);
    eff.endClass();
}


static void registerFileSystem(CppBindModule<LuaBinding> &module)
{
    auto fs = BEGIN_CLASS(module, FileSystem);
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
    auto meshAsyncHandle = BEGIN_CLASS_RENAMED(module, AsyncHandle<Mesh>, "MeshAsyncHandle");
    REG_METHOD(meshAsyncHandle, AsyncHandle<Mesh>, done);
    meshAsyncHandle.endClass();

    auto rectTexAsyncHandle = BEGIN_CLASS_RENAMED(module, AsyncHandle<RectTexture>, "RectTextureAsyncHandle");
    REG_METHOD(rectTexAsyncHandle, AsyncHandle<RectTexture>, done);
    rectTexAsyncHandle.endClass();

    auto cubeTexAsyncHandle = BEGIN_CLASS_RENAMED(module, AsyncHandle<CubeTexture>, "CubeTextureAsyncHandle");
    REG_METHOD(cubeTexAsyncHandle, AsyncHandle<CubeTexture>, done);
    cubeTexAsyncHandle.endClass();
}


static void registerAssetLoader(CppBindModule<LuaBinding> &module)
{
    auto loader = BEGIN_CLASS(module, AssetLoader);
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
    auto renderer = BEGIN_CLASS(module, Renderer);
    REG_METHOD(renderer, Renderer, renderFrame);
    renderer.endClass();
}


static void registerRenderContext(CppBindModule<LuaBinding> &module)
{
    auto rc = BEGIN_CLASS(module, RenderContext);
    REG_CTOR(rc);
    REG_VARIABLE(rc, RenderContext, camera);
    rc.endClass();
}


void registerMiscApi(CppBindModule<LuaBinding> &module)
{
    registerRenderer(module);
    registerAssetLoader(module);
    registerFileSystem(module);
    registerEffect(module);
    registerMeshRenderer(module);
    registerScene(module);
    registerLogger(module);
    registerFrameBuffer(module);
    registerAsyncHandles(module);
    registerRenderContext(module);
    registerSpectator(module);
    registerSkyboxRenderer(module);
}
