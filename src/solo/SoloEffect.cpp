/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloEffect.h"
#include "SoloDevice.h"
#include "SoloFileSystem.h"
#include "gl/SoloOpenGLEffect.h"
#include "vk/SoloVulkanEffect.h"
#include "null/SoloNullEffect.h"

using namespace solo;

auto Effect::loadFromFiles(Device *device, const str &vsPath, const str &fsPath) -> sptr<Effect>
{
    auto vsBytes = device->getFileSystem()->readBytes(vsPath);
    auto fsBytes = device->getFileSystem()->readBytes(fsPath);

    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLEffect>(
                vsBytes.data(), vsBytes.size(),
                fsBytes.data(), fsBytes.size()
            );
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanEffect::createFromSource(
                device,
                vsBytes.data(), vsBytes.size(), vsPath,
                fsBytes.data(), fsBytes.size(), fsPath
            );
#endif
        default:
            return std::make_shared<NullEffect>();
    }
}

auto Effect::loadFromFile(Device *device, const str &path) -> sptr<Effect>
{
    auto text = device->getFileSystem()->readText(path);

    const auto vertTagStartIdx = text.find("// VERTEX");
    SL_PANIC_IF(vertTagStartIdx == std::string::npos, SL_FMT("Vertex shader not found in ", path));

    const auto fragTagStartIdx = text.find("// FRAGMENT");
    SL_PANIC_IF(vertTagStartIdx == std::string::npos, SL_FMT("Fragment shader not found in ", path));

    const auto vertShaderStartIdx = vertTagStartIdx + std::strlen("// VERTEX");
    const auto vertShaderEndIdx = fragTagStartIdx > vertTagStartIdx ? fragTagStartIdx - 1 : text.size() - 1;
    const auto vsBytes = text.c_str() + vertShaderStartIdx;
    const auto vsSize = vertShaderEndIdx - vertShaderStartIdx + 1;

    const auto fragShaderStartIdx = fragTagStartIdx + std::strlen("// FRAGMENT");
    const auto fragShaderEndIdx = vertTagStartIdx > fragTagStartIdx ? vertTagStartIdx - 1 : text.size() - 1;
    const auto fsBytes = text.c_str() + fragShaderStartIdx;
    const auto fsSize = fragShaderEndIdx - fragShaderStartIdx + 1;

    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLEffect>(vsBytes, vsSize, fsBytes, fsSize);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanEffect::createFromSource(device, vsBytes, vsSize, path, fsBytes, fsSize, path);
#endif
        default:
            return std::make_shared<NullEffect>();
    }
}

auto Effect::createFromSource(
    Device *device,
    const void *vsSrc, u32 vsSrcLen,
    const void *fsSrc, u32 fsSrcLen) -> sptr<Effect>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLEffect>(vsSrc, vsSrcLen, fsSrc, fsSrcLen);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return VulkanEffect::createFromSource(device, vsSrc, vsSrcLen, "vert-src", fsSrc, fsSrcLen, "frag-src");
#endif
        default:
            return std::make_shared<NullEffect>();
    }
}
