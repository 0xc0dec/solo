/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloScriptRuntime.h"
#include "SoloEnums.h"
#include "gl/SoloOpenGLEffect.h"
#include "vk/SoloVulkanEffect.h"

using namespace solo;

auto Effect::fromSourceFile(Device *device, const str &path) -> sptr<Effect>
{
    const auto source = device->fileSystem()->readText(path);
    return fromSource(device, source);
}

auto Effect::fromDescriptionFile(Device* device, const str& path) -> sptr<Effect>
{
    const auto desc = device->fileSystem()->readText(path);
    return fromDescription(device, desc);
}

auto Effect::fromDescription(Device* device, const str& description) -> sptr<Effect>
{
    // This doesn't seem to restrict us to Lua scripting only. It seems quite possible to 
    // provide the same script method in other possible scripting languages in the future.
    const auto source = device->scriptRuntime()->eval("sl.generateEffectSource(" + description + ")");
    return fromSource(device, source);
}

auto Effect::fromSource(Device* device, const str& source) -> sptr<Effect>
{
    const auto vertTagStartIdx = source.find("// VERTEX");
    SL_DEBUG_PANIC(vertTagStartIdx == std::string::npos, "Vertex shader not found in ", source);

    const auto fragTagStartIdx = source.find("// FRAGMENT");
    SL_DEBUG_PANIC(vertTagStartIdx == std::string::npos, "Fragment shader not found in ", source);

    const auto vertShaderStartIdx = vertTagStartIdx + std::strlen("// VERTEX");
    const auto vertShaderEndIdx = fragTagStartIdx > vertTagStartIdx ? fragTagStartIdx - 1 : source.size() - 1;
    const auto vsBytes = source.c_str() + vertShaderStartIdx;
    const auto vsSize = vertShaderEndIdx - vertShaderStartIdx + 1;

    const auto fragShaderStartIdx = fragTagStartIdx + std::strlen("// FRAGMENT");
    const auto fragShaderEndIdx = vertTagStartIdx > fragTagStartIdx ? vertTagStartIdx - 1 : source.size() - 1;
    const auto fsBytes = source.c_str() + fragShaderStartIdx;
    const auto fsSize = fragShaderEndIdx - fragShaderStartIdx + 1;

    switch (device->mode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLEffect>(vsBytes, vsSize, fsBytes, fsSize);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanEffect::fromSources(device, vsBytes, vsSize, fsBytes, fsSize);
#endif
        default:
            SL_DEBUG_PANIC(true, "Unknown device mode");
            return nullptr;
    }
}

