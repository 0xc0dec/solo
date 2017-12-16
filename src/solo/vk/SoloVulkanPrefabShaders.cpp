/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloVulkanPrefabShaders.h"

#ifdef SL_VULKAN_RENDERER

using namespace solo;

const s8 *VulkanPrefabShaders::Vertex::skybox = R"(
    #version 450

    layout (binding = 0) uniform Matrices
    {
        mat4 worldView;
        mat4 proj;
    } matrices;

    layout (location = 0) in vec4 position;

    layout (location = 0) out vec3 eyeDir;

    void main()
    {
        mat4 invProjMatrix = inverse(matrices.proj);
        mat3 invModelViewMatrix = inverse(mat3(matrices.worldView));
        vec4 mirroredPos =  vec4(position.x, -position.y, position.z, position.w);
        vec3 unprojected = (invProjMatrix * mirroredPos).xyz;
        eyeDir = invModelViewMatrix * unprojected;
        eyeDir.y = -eyeDir.y;
        gl_Position = position;
    }
)";

const s8* VulkanPrefabShaders::Vertex::font = R"(
    TODO
)";

const s8* VulkanPrefabShaders::Fragment::skybox = R"(
    #version 450

    layout (binding = 1) uniform samplerCube mainTex;

    layout (location = 0) in vec3 eyeDir;

    layout (location = 0) out vec4 fragColor;

    void main()
    {
        fragColor = texture(mainTex, eyeDir);
    }
)";

const s8* VulkanPrefabShaders::Fragment::font = R"(
    TODO
)";

#endif
