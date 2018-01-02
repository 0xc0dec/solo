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
#include "SoloRenderer.h"
#include "SoloMesh.h"
#include "SoloSpectator.h"
#include "SoloAsyncHandle.h"
#include "SoloTexture.h"

using namespace solo;

static void registerFrameBuffer(CppBindModule<LuaBinding> &module)
{
    auto fb = BEGIN_CLASS(module, FrameBuffer);
    REG_STATIC_METHOD(fb, FrameBuffer, create);
    REG_METHOD(fb, FrameBuffer, setAttachments);
    REG_METHOD(fb, FrameBuffer, getDimensions);
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
    REG_METHOD(scene, Scene, removeNode);
    REG_METHOD(scene, Scene, removeNodeById);
    scene.endClass();
}

static void registerMeshRenderer(CppBindModule<LuaBinding> &module)
{
    auto renderer = BEGIN_CLASS_EXTEND(module, MeshRenderer, Component);
    REG_METHOD(renderer, MeshRenderer, render);
    REG_METHOD(renderer, MeshRenderer, getMesh);
    REG_METHOD_NULLABLE_1ST_ARG(renderer, MeshRenderer, setMesh, sptr<Mesh>);
    REG_METHOD(renderer, MeshRenderer, getMaterial);
    REG_METHOD_NULLABLE_2ND_ARG(renderer, MeshRenderer, setMaterial, u32, sptr<Material>);
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
    REG_STATIC_METHOD(eff, Effect, loadFromFile);
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

static void registerRenderer(CppBindModule<LuaBinding> &module)
{
    auto renderer = BEGIN_CLASS(module, Renderer);
    renderer.endClass();
}

static void registerAsyncHandles(CppBindModule<LuaBinding> &module)
{
    auto h1 = BEGIN_CLASS_RENAMED(module, AsyncHandle<Texture2d>, "Texture2dAsyncHandle");
    REG_METHOD(h1, AsyncHandle<Texture2d>, done);
    h1.endClass();

    auto h2 = BEGIN_CLASS_RENAMED(module, AsyncHandle<CubeTexture>, "CubeTextureAsyncHandle");
    REG_METHOD(h2, AsyncHandle<CubeTexture>, done);
    h2.endClass();

    auto h3 = BEGIN_CLASS_RENAMED(module, AsyncHandle<Mesh>, "MeshAsyncHandle");
    REG_METHOD(h3, AsyncHandle<Mesh>, done);
    h3.endClass();
}

void registerMiscApi(CppBindModule<LuaBinding> &module)
{
    registerFileSystem(module);
    registerEffect(module);
    registerMeshRenderer(module);
    registerScene(module);
    registerRenderer(module);
    registerLogger(module);
    registerFrameBuffer(module);
    registerSpectator(module);
    registerSkyboxRenderer(module);
    registerAsyncHandles(module);
}
