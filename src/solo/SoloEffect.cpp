/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "SoloScriptRuntime.h"
#include "gl/SoloOpenGLEffect.h"
#include "vk/SoloVulkanEffect.h"
#include "null/SoloNullEffect.h"

using namespace solo;

auto Effect::loadFromSourceFile(Device *device, const str &path) -> sptr<Effect>
{
    const auto source = device->getFileSystem()->readText(path);
	return createFromSource(device, source);
}

auto Effect::loadFromDescriptionFile(Device* device, const str& path) -> sptr<Effect>
{
	const auto desc = device->getFileSystem()->readText(path);
	return createFromDescription(device, desc);
}

auto Effect::createFromDescription(Device* device, const str& description) -> sptr<Effect>
{
	const auto source = device->getScriptRuntime()->eval("sl.generateEffectSource(" + description + ")");
	return createFromSource(device, source);
}

auto Effect::createFromSource(Device* device, const str& source) -> sptr<Effect>
{
	const auto vertTagStartIdx = source.find("// VERTEX");
    panicIf(vertTagStartIdx == std::string::npos, SL_FMT("Vertex shader not found in ", source));

    const auto fragTagStartIdx = source.find("// FRAGMENT");
    panicIf(vertTagStartIdx == std::string::npos, SL_FMT("Fragment shader not found in ", source));

    const auto vertShaderStartIdx = vertTagStartIdx + std::strlen("// VERTEX");
    const auto vertShaderEndIdx = fragTagStartIdx > vertTagStartIdx ? fragTagStartIdx - 1 : source.size() - 1;
    const auto vsBytes = source.c_str() + vertShaderStartIdx;
    const auto vsSize = vertShaderEndIdx - vertShaderStartIdx + 1;

    const auto fragShaderStartIdx = fragTagStartIdx + std::strlen("// FRAGMENT");
    const auto fragShaderEndIdx = vertTagStartIdx > fragTagStartIdx ? vertTagStartIdx - 1 : source.size() - 1;
    const auto fsBytes = source.c_str() + fragShaderStartIdx;
    const auto fsSize = fragShaderEndIdx - fragShaderStartIdx + 1;

    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLEffect>(vsBytes, vsSize, fsBytes, fsSize);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanEffect::createFromSources(device, vsBytes, vsSize, "source", fsBytes, fsSize, "source");
#endif
        default:
            return std::make_shared<NullEffect>();
    }
}

auto Effect::createFromShaderSources(Device *device, const void *vsSrc, u32 vsSrcLen, const void *fsSrc, u32 fsSrcLen) -> sptr<Effect>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLEffect>(vsSrc, vsSrcLen, fsSrc, fsSrcLen);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanEffect::createFromSources(device, vsSrc, vsSrcLen, "vert-src", fsSrc, fsSrcLen, "frag-src");
#endif
        default:
            return std::make_shared<NullEffect>();
    }
}
