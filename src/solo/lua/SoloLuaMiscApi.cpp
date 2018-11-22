/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloFrameBuffer.h"
#include "SoloLuaCommon.h"
#include "SoloScene.h"
#include "SoloMeshRenderer.h"
#include "SoloEffect.h"
#include "SoloFileSystem.h"
#include "SoloSpectator.h"

using namespace solo;

static void registerFrameBuffer(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, FrameBuffer);
    REG_STATIC_METHOD(binding, FrameBuffer, fromAttachments);
    REG_METHOD(binding, FrameBuffer, dimensions);
    REG_PTR_EQUALITY(binding, FrameBuffer);
    binding.endClass();
}

static void registerScene(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, Scene);
    REG_STATIC_METHOD(binding, Scene, empty);
    REG_METHOD(binding, Scene, device);
    REG_METHOD(binding, Scene, createNode);
    REG_METHOD(binding, Scene, removeNode);
    REG_METHOD(binding, Scene, removeNodeById);
    REG_METHOD(binding, Scene, visit);
    REG_METHOD(binding, Scene, visitByTags);
    REG_PTR_EQUALITY(binding, Scene);
    binding.endClass();
}

static void registerMeshRenderer(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS_EXTEND(module, MeshRenderer, Component);
    REG_METHOD(binding, MeshRenderer, render);
    REG_METHOD(binding, MeshRenderer, mesh);
    REG_METHOD_NULLABLE_1ST_ARG(binding, MeshRenderer, setMesh, sptr<Mesh>);
    REG_METHOD(binding, MeshRenderer, material);
    REG_METHOD_NULLABLE_2ND_ARG(binding, MeshRenderer, setMaterial, u32, sptr<Material>);
    REG_METHOD(binding, MeshRenderer, materialCount);
    REG_PTR_EQUALITY(binding, MeshRenderer);
    binding.endClass();
}

static void registerSpectator(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS_EXTEND(module, Spectator, Component);
    REG_METHOD(binding, Spectator, movementSpeed);
    REG_METHOD(binding, Spectator, setMovementSpeed);
    REG_METHOD(binding, Spectator, mouseSensitivity);
    REG_METHOD(binding, Spectator, setMouseSensitivity);
    REG_METHOD(binding, Spectator, rotationAcceleration);
    REG_METHOD(binding, Spectator, setRotationAcceleration);
    REG_PTR_EQUALITY(binding, Spectator);
}

static void registerEffect(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, Effect);
    REG_STATIC_METHOD(binding, Effect, fromSourceFile);
    REG_STATIC_METHOD(binding, Effect, fromDescriptionFile);
    REG_STATIC_METHOD(binding, Effect, fromSource);
    REG_PTR_EQUALITY(binding, Effect);
    binding.endClass();
}

static void registerFileSystem(CppBindModule<LuaBinding> &module)
{
    auto binding = BEGIN_CLASS(module, FileSystem);
    REG_METHOD(binding, FileSystem, readBytes);
    REG_METHOD(binding, FileSystem, writeBytes);
    REG_METHOD(binding, FileSystem, readText);
    REG_METHOD(binding, FileSystem, readLines);
    REG_METHOD(binding, FileSystem, writeLines);
    REG_METHOD(binding, FileSystem, iterateLines);
    REG_PTR_EQUALITY(binding, FileSystem);
    binding.endClass();
}

void registerMiscApi(CppBindModule<LuaBinding> &module)
{
    registerFileSystem(module);
    registerEffect(module);
    registerMeshRenderer(module);
    registerScene(module);
    registerFrameBuffer(module);
    registerSpectator(module);
}
