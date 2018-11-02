/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloFrameBuffer.h"
#include "SoloLuaCommon.h"
#include "SoloScene.h"
#include "SoloMeshRenderer.h"
#include "SoloEffect.h"
#include "SoloFileSystem.h"
#include "SoloMesh.h"
#include "SoloSpectator.h"
#include "SoloAsyncHandle.h"
#include "SoloTexture.h"

using namespace solo;

static void registerFrameBuffer(CppBindModule<LuaBinding> &module)
{
    auto fb = BEGIN_CLASS(module, FrameBuffer);
    REG_STATIC_METHOD(fb, FrameBuffer, create);
    REG_METHOD(fb, FrameBuffer, dimensions);
    fb.endClass();
}

static void registerScene(CppBindModule<LuaBinding> &module)
{
    auto scene = BEGIN_CLASS(module, Scene);
    REG_STATIC_METHOD(scene, Scene, create);
    REG_METHOD(scene, Scene, device);
    REG_METHOD(scene, Scene, createNode);
    REG_METHOD(scene, Scene, removeNode);
    REG_METHOD(scene, Scene, removeNodeById);
    REG_METHOD(scene, Scene, visit);
    REG_METHOD(scene, Scene, visitByTags);
    scene.endClass();
}

static void registerMeshRenderer(CppBindModule<LuaBinding> &module)
{
    auto renderer = BEGIN_CLASS_EXTEND(module, MeshRenderer, Component);
    REG_METHOD(renderer, MeshRenderer, render);
    REG_METHOD(renderer, MeshRenderer, mesh);
    REG_METHOD_NULLABLE_1ST_ARG(renderer, MeshRenderer, setMesh, sptr<Mesh>);
    REG_METHOD(renderer, MeshRenderer, material);
    REG_METHOD_NULLABLE_2ND_ARG(renderer, MeshRenderer, setMaterial, u32, sptr<Material>);
    REG_METHOD(renderer, MeshRenderer, materialCount);
    renderer.endClass();
}

static void registerSpectator(CppBindModule<LuaBinding> &module)
{
    auto spec = BEGIN_CLASS_EXTEND(module, Spectator, Component);
    REG_METHOD(spec, Spectator, movementSpeed);
    REG_METHOD(spec, Spectator, setMovementSpeed);
    REG_METHOD(spec, Spectator, mouseSensitivity);
    REG_METHOD(spec, Spectator, setMouseSensitivity);
    REG_METHOD(spec, Spectator, rotationAcceleration);
    REG_METHOD(spec, Spectator, setRotationAcceleration);
}

static void registerEffect(CppBindModule<LuaBinding> &module)
{
    auto eff = BEGIN_CLASS(module, Effect);
    REG_STATIC_METHOD(eff, Effect, fromSourceFile);
    REG_STATIC_METHOD(eff, Effect, fromDescriptionFile);
    REG_STATIC_METHOD(eff, Effect, fromSource);
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
    auto h1 = BEGIN_CLASS_RENAMED(module, AsyncHandle<Texture2D>, "Texture2DAsyncHandle");
    REG_METHOD(h1, AsyncHandle<Texture2D>, done);
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
    registerFrameBuffer(module);
    registerSpectator(module);
    registerAsyncHandles(module);
}
