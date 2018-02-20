/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloMaterial.h"
#include "SoloTexture.h"
#include "SoloDevice.h"
#include "gl/SoloOpenGLMaterial.h"
#include "vk/SoloVulkanMaterial.h"
#include "null/SoloNullMaterial.h"

using namespace solo;

static const char * const skyboxPrefabEffect = R"(
{
    vertex = {
        uniformBuffers = {
            matrices = {
                worldView = "mat4",
                proj = "mat4"
            }
        },

        inputs = {
            sl_Position = "vec4"
        },

        outputs = {
            eyeDir = "vec3"
        },

        code = [[
            void main()
            {
                vec4 pos = sl_Position;
                SL_FIX_Y#pos#;

                mat4 invProjMatrix = inverse(#matrices:proj#);
                mat3 invModelViewMatrix = inverse(mat3(#matrices:worldView#));
                vec3 unprojected = (invProjMatrix * pos).xyz;
                eyeDir = invModelViewMatrix * unprojected;
                gl_Position = sl_Position;
                SL_FIX_Y#eyeDir#;
            }
        ]]
    },

    fragment = {
        samplers = {
            mainTex = "samplerCube"
        },

        outputs = {
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            void main()
            {
                fragColor = texture(mainTex, eyeDir);
            }
        ]]
    }
})";

static const char * const fontPrefabEffect = R"(
{
    vertex = {
        uniformBuffers = {
            matrices = {
                wvp = "mat4"
            }
        },

        inputs = {
            sl_Position = "vec4",
            sl_TexCoord = "vec2"
        },

        outputs = {
            uv = "vec2"
        },

        code = [[
            void main()
            {
                gl_Position = #matrices:wvp# * sl_Position;
                uv = sl_TexCoord;
                SL_FIX_Y#gl_Position#;
            }
        ]]
    },

    fragment = {
        samplers = {
            mainTex = "sampler2D"
        },

        outputs = {
            fragColor = { type = "vec4", target = 0 }
        },

        code = [[
            void main()
            {
                vec4 c = texture(mainTex, uv);
                fragColor = vec4(c.r, c.r, c.r, c.r);
            }
        ]]
    }
})";

auto Material::create(Device *device, sptr<Effect> effect) -> sptr<Material>
{
    switch (device->getMode())
    {
#ifdef SL_OPENGL_RENDERER
        case DeviceMode::OpenGL:
            return std::make_shared<OpenGLMaterial>(effect);
#endif
#ifdef SL_VULKAN_RENDERER
        case DeviceMode::Vulkan:
            return std::make_shared<VulkanMaterial>(effect);
#endif
        default:
            return std::make_shared<NullMaterial>();
    }
}

auto Material::createFromPrefab(Device *device, MaterialPrefab prefab) -> sptr<Material>
{
    switch (prefab)
    {
        case MaterialPrefab::Font:
        {
            const auto effect = Effect::createFromDescription(device, fontPrefabEffect);
            auto material = create(device, effect);
            material->bindParameter("matrices:wvp", BindParameterSemantics::WorldViewProjectionMatrix);
            return material;
        }

        case MaterialPrefab::Skybox:
        {
            const auto effect = Effect::createFromDescription(device, skyboxPrefabEffect);
            auto material = create(device, effect);
            material->bindParameter("matrices:proj", BindParameterSemantics::ProjectionMatrix);
            material->bindParameter("matrices:worldView", BindParameterSemantics::WorldViewMatrix);
            return material;    
        }

        default:
            return panic<nullptr_t>("Unknown material prefab");
    }
}

void Material::setBlendFactors(BlendFactor srcFactor, BlendFactor dstFactor)
{
    srcBlendFactor = srcFactor;
    dstBlendFactor = dstFactor;
}
