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

using namespace solo;


void registerFrameBuffer(CppBindModule<LuaBinding> &module)
{
    auto fb = module.beginClass<FrameBuffer>("FrameBuffer");
    REG_STATIC_METHOD(fb, FrameBuffer, create);
    REG_METHOD(fb, FrameBuffer, setAttachments);
    REG_METHOD(fb, FrameBuffer, getSize);
    fb.endClass();
}


void registerLogger(CppBindModule<LuaBinding> &module)
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


void registerScene(CppBindModule<LuaBinding> &module)
{
    auto scene = module.beginClass<Scene>("Scene");
    
    REG_STATIC_METHOD(scene, Scene, create);
    REG_METHOD(scene, Scene, getDevice);
    REG_METHOD(scene, Scene, createNode);
    REG_METHOD(scene, Scene, visit);
    REG_METHOD(scene, Scene, visitByTags);
    
    scene.endClass();
}


void registerDevice(CppBindModule<LuaBinding> &module)
{
    auto device = module.beginClass<Device>("Device");
    REG_METHOD(device, Device, getLogger);
    REG_METHOD(device, Device, getWindowTitle);
    REG_METHOD(device, Device, setWindowTitle);
    device.endClass();
}
