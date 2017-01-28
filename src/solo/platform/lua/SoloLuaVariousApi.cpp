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

using namespace solo;


void registerFrameBuffer(CppBindModule<LuaBinding> &module)
{
    auto c = module.beginClass<FrameBuffer>("FrameBuffer");
    REG_STATIC_METHOD(c, FrameBuffer, create);
    REG_METHOD(c, FrameBuffer, setAttachments);
    REG_METHOD(c, FrameBuffer, getSize);
    c.endClass();
}


void registerLogger(CppBindModule<LuaBinding> &module)
{
    auto c = module.beginClass<Logger>("Logger");
    REG_METHOD(c, Logger, setTargetFile);
    REG_METHOD(c, Logger, logDebug);
    REG_METHOD(c, Logger, logInfo);
    REG_METHOD(c, Logger, logWarning);
    REG_METHOD(c, Logger, logError);
    REG_METHOD(c, Logger, logCritical);
    c.endClass();
}


void registerScene(CppBindModule<LuaBinding> &module)
{
    auto c = module.beginClass<Scene>("Scene");
    
    REG_STATIC_METHOD(c, Scene, create);
    REG_METHOD(c, Scene, getDevice);
    REG_METHOD(c, Scene, createNode);
    REG_METHOD(c, Scene, visit);
    REG_METHOD(c, Scene, visitByTags);
    
    c.endClass();
}


void registerDevice(CppBindModule<LuaBinding> &module)
{
    auto c = module.beginClass<Device>("Device");
    
    REG_METHOD(c, Device, getWindowTitle);
    REG_METHOD(c, Device, setWindowTitle);

    REG_METHOD(c, Device, getCanvasSize);

    REG_METHOD(c, Device, saveScreenshot);

    REG_METHOD(c, Device, setCursorCaptured);

    REG_METHOD(c, Device, getLifetime);
    REG_METHOD(c, Device, getTimeDelta);

    REG_METHOD(c, Device, isWindowCloseRequested);
    REG_METHOD(c, Device, isQuitRequested);

    REG_METHOD(c, Device, isKeyPressed);
    REG_METHOD(c, Device, isKeyReleased);

    REG_METHOD(c, Device, getMouseMotion);
    REG_METHOD(c, Device, isMouseButtonDown);
    REG_METHOD(c, Device, isMouseButtonReleased);
    
    REG_METHOD(c, Device, update);

    REG_METHOD(c, Device, getSetup);

    REG_METHOD(c, Device, getFileSystem);
    REG_METHOD(c, Device, getAssetLoader);
    REG_METHOD(c, Device, getRenderer);
    REG_METHOD(c, Device, getPhysics);
    REG_METHOD(c, Device, getLogger);
    
    c.endClass();
}


void registerMeshRenderer(CppBindModule<LuaBinding> &module)
{
    auto c = module.beginExtendClass<MeshRenderer, Component>("MeshRenderer");
    REG_METHOD(c, MeshRenderer, render);
    REG_METHOD(c, MeshRenderer, getMesh);
    REG_METHOD(c, MeshRenderer, setMesh);
    REG_METHOD(c, MeshRenderer, getMaterial);
    REG_METHOD(c, MeshRenderer, setMaterial);
    REG_METHOD(c, MeshRenderer, getMaterialCount);
    c.endClass();
}


void registerEffect(CppBindModule<LuaBinding> &module)
{
    auto c = module.beginClass<Effect>("Effect");
    REG_STATIC_METHOD(c, Effect, create);
    REG_STATIC_METHOD(c, Effect, createFromPrefab);
    c.endClass();
}


void registerDeviceSetup(CppBindModule<LuaBinding> &module)
{
    auto c = module.beginClass<DeviceSetup>("DeviceSetup");
    
    REG_VARIABLE(c, DeviceSetup, mode);
    REG_METHOD(c, DeviceSetup, withMode);

    REG_VARIABLE(c, DeviceSetup, canvasWidth);
    REG_METHOD(c, DeviceSetup, withCanvasWidth);

    REG_VARIABLE(c, DeviceSetup, canvasHeight);
    REG_METHOD(c, DeviceSetup, withCanvasHeight);

    REG_VARIABLE(c, DeviceSetup, fullScreen);
    REG_METHOD(c, DeviceSetup, withFullScreen);

    REG_VARIABLE(c, DeviceSetup, windowTitle);
    REG_METHOD(c, DeviceSetup, withWindowTitle);

    REG_VARIABLE(c, DeviceSetup, redBits);
    REG_METHOD(c, DeviceSetup, withRedBits);

    REG_VARIABLE(c, DeviceSetup, greenBits);
    REG_METHOD(c, DeviceSetup, withGreenBits);

    REG_VARIABLE(c, DeviceSetup, blueBits);
    REG_METHOD(c, DeviceSetup, withBlueBits);

    REG_VARIABLE(c, DeviceSetup, depthBits);
    REG_METHOD(c, DeviceSetup, withDepthBits);

    REG_VARIABLE(c, DeviceSetup, alphaBits);
    REG_METHOD(c, DeviceSetup, withAlphaBits);

    REG_VARIABLE(c, DeviceSetup, vsync);
    REG_METHOD(c, DeviceSetup, withVsync);

    REG_VARIABLE(c, DeviceSetup, logFilePath);
    REG_METHOD(c, DeviceSetup, withLogFilePath);

    c.endClass();
}


void registerFileSystem(CppBindModule<LuaBinding> &module)
{
    auto c = module.beginClass<FileSystem>("FileSystem");
    
    REG_METHOD(c, FileSystem, readBytes);
    REG_METHOD(c, FileSystem, writeBytes);
    
    REG_METHOD(c, FileSystem, readText);

    REG_METHOD(c, FileSystem, readLines);
    REG_METHOD(c, FileSystem, writeLines);
    
    REG_METHOD(c, FileSystem, iterateLines);

    c.endClass();
}


void registerAssetLoader(CppBindModule<LuaBinding> &module)
{
    auto c = module.beginClass<AssetLoader>("AssetLoader");
    
    REG_METHOD(c, AssetLoader, loadRectTexture);
    REG_METHOD(c, AssetLoader, loadRectTextureAsync);
    
    REG_METHOD(c, AssetLoader, loadCubeTexture);
    REG_METHOD(c, AssetLoader, loadCubeTextureAsync);

    REG_METHOD(c, AssetLoader, loadMesh);
    REG_METHOD(c, AssetLoader, loadMeshAsync);

    REG_METHOD(c, AssetLoader, update);

    c.endClass();
}