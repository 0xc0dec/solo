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
#include "SoloRenderer.h"
#include "SoloDebugInterface.h"

using namespace solo;

void registerMiscApi(CppBindModule<LuaBinding> &module)
{
    {
        auto b = BEGIN_CLASS(module, FileSystem);
        REG_METHOD(b, FileSystem, readBytes);
        REG_METHOD(b, FileSystem, writeBytes);
        REG_METHOD(b, FileSystem, readText);
        REG_METHOD(b, FileSystem, readLines);
        REG_METHOD(b, FileSystem, writeLines);
        REG_METHOD(b, FileSystem, iterateLines);
        REG_PTR_EQUALITY(b, FileSystem);
        b.endClass();
    }

    {
        auto b = BEGIN_CLASS(module, Effect);
        REG_STATIC_METHOD(b, Effect, fromSourceFile);
        REG_STATIC_METHOD(b, Effect, fromDescriptionFile);
        REG_STATIC_METHOD(b, Effect, fromSource);
        REG_PTR_EQUALITY(b, Effect);
        b.endClass();
    }

    {
        auto b = BEGIN_CLASS_EXTEND(module, MeshRenderer, Component);
        REG_METHOD(b, MeshRenderer, render);
        REG_METHOD(b, MeshRenderer, mesh);
        REG_METHOD_NULLABLE_1ST_ARG(b, MeshRenderer, setMesh, sptr<Mesh>);
        REG_METHOD(b, MeshRenderer, material);
        REG_METHOD_NULLABLE_2ND_ARG(b, MeshRenderer, setMaterial, u32, sptr<Material>);
        REG_METHOD_NULLABLE_1ST_ARG(b, MeshRenderer, setDefaultMaterial, sptr<Material>);
        REG_METHOD(b, MeshRenderer, materialCount);
        REG_PTR_EQUALITY(b, MeshRenderer);
        b.endClass();
    }

    {
        auto b = BEGIN_CLASS(module, Scene);
        REG_STATIC_METHOD(b, Scene, empty);
        REG_METHOD(b, Scene, device);
        REG_METHOD(b, Scene, createNode);
        REG_METHOD(b, Scene, removeNode);
        REG_METHOD(b, Scene, removeNodeById);
        REG_METHOD(b, Scene, visit);
        REG_METHOD(b, Scene, visitByTags);
        REG_METHOD(b, Scene, update);
        REG_METHOD(b, Scene, render);
        REG_PTR_EQUALITY(b, Scene);
        b.endClass();
    }

    {
        auto b = BEGIN_CLASS(module, Renderer);
        REG_METHOD(b, Renderer, name);
        REG_METHOD(b, Renderer, gpuName);
        REG_PTR_EQUALITY(b, Renderer);
        b.endClass();
    }

    {
        auto b = BEGIN_CLASS(module, FrameBuffer);
        REG_STATIC_METHOD(b, FrameBuffer, fromAttachments);
        REG_METHOD(b, FrameBuffer, dimensions);
        REG_PTR_EQUALITY(b, FrameBuffer);
        b.endClass();
    }

    {
        auto b = BEGIN_CLASS_EXTEND(module, Spectator, Component);
        REG_METHOD(b, Spectator, movementSpeed);
        REG_METHOD(b, Spectator, setMovementSpeed);
        REG_METHOD(b, Spectator, mouseSensitivity);
        REG_METHOD(b, Spectator, setMouseSensitivity);
        REG_PTR_EQUALITY(b, Spectator);
    }

    {
        auto b = BEGIN_CLASS(module, WindowConfig);
        REG_CTOR(b);
        REG_FIELD(b, WindowConfig, title);
        REG_FIELD(b, WindowConfig, position);
        REG_FIELD(b, WindowConfig, pivot);
        REG_FIELD(b, WindowConfig, alpha);
        REG_FIELD(b, WindowConfig, autoResize);
        REG_FIELD(b, WindowConfig, movable);
        REG_FIELD(b, WindowConfig, decoration);
        b.endClass();
    }

    {
        auto b = BEGIN_CLASS(module, DebugInterface);
        REG_METHOD(b, DebugInterface, renderWindow);
        REG_METHOD(b, DebugInterface, renderText);
        REG_PTR_EQUALITY(b, DebugInterface);
        b.endClass();
    }
}
