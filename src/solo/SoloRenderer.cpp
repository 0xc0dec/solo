/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#include "SoloRenderer.h"
#include "SoloDevice.h"
#include "SoloEnums.h"
#include "SoloEffect.h"
#include "SoloMaterial.h"
#include "gl/SoloOpenGLRenderer.h"
#include "vk/SoloVulkanRenderer.h"

using namespace solo;

// TODO Move into separate file
static const auto ERROR_EFFECT_SRC = R"(
{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            sl_Position = "vec4"
        },

        outputs = {
        },

        code = [[
            void main()
            {
                gl_Position = #matrices:wvp# * sl_Position;
                SL_FIX_Y#gl_Position#;
            }
        ]]
    },

    fragment = {
        samplers = {
        },

        outputs = {
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            void main()
            {
                fragColor = vec4(1, 0, 1, 1);
            }
        ]]
    }
})";

auto Renderer::fromDevice(Device *device) -> sptr<Renderer>
{
    switch (device->mode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLRenderer>(device);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanRenderer>(device);
#endif
    	default:
    		panic("Unknown device mode");
    		return nullptr;
    }
}

void Renderer::renderFrame(const std::function<void()> &render)
{
    beginFrame();
    render();
    endFrame();
}

Renderer::Renderer(Device *device)
{
	const auto errorEffect = Effect::fromDescription(device, ERROR_EFFECT_SRC);
	errorMaterial_ = Material::fromEffect(device, errorEffect);
	errorMaterial_->bindParameter("matrices:wvp", ParameterBinding::WorldViewProjectionMatrix);
}
