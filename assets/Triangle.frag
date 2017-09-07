#version 450

layout (set = 0, binding = 0) uniform sampler2D colorSampler;

layout (location = 0) in vec2 inTexCoord;

layout (location = 0) out vec3 outFragColor;

void main()
{
    vec4 color = texture(colorSampler, inTexCoord, 1);
    outFragColor = color.rgb;
}