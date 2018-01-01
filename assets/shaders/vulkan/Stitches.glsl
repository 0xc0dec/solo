// VERTEX

#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord0;

layout (location = 0) out vec2 uv0;

void main()
{
    gl_Position = position;
    uv0 = texCoord0;
}

// FRAGMENT

#version 450

layout (binding = 0) uniform Variables
{
    vec2 stitchCount;
    vec2 resolution;
} variables;

layout (binding = 1) uniform sampler2D mainTex;
layout (binding = 2) uniform sampler2D stitchTex;

layout (location = 0) in vec2 uv0;

layout (location = 0) out vec4 fragColor;

void main()
{
    vec2 colorBlock = floor(uv0 * variables.resolution);
    vec4 color = texture(mainTex, uv0);
    vec4 prevColor = texture(mainTex, vec2(uv0.x, uv0.y + 1.0 / variables.resolution.y));

    float stitchUvX = fract(variables.stitchCount.x * (uv0.x + pow(colorBlock.y, 2.0) / variables.resolution.x * 2.0));
    float stitchUvY = fract(variables.stitchCount.y * uv0.y);
    float nextStitchUvX = fract(variables.stitchCount.x * (uv0.x + pow(colorBlock.y + 1.0, 2.0) / variables.resolution.x * 2.0));
    if (stitchUvY > 0.5)
    {
        vec2 stitchUV = vec2(stitchUvX, stitchUvY);
        fragColor = texture(stitchTex, stitchUV) * color;
        vec2 topStitchUV = vec2(nextStitchUvX, fract(stitchUvY - 0.5));
        vec4 topStitchColor = texture(stitchTex, topStitchUV) * prevColor;
        if (topStitchColor.a > 0.05)
            fragColor = mix(fragColor, topStitchColor, 1.0 - fragColor.a);
    }
    else
    {
        vec2 stitchUV = vec2(nextStitchUvX, stitchUvY);
        fragColor = texture(stitchTex, stitchUV) * prevColor;
        vec2 bottomStitchUV = vec2(stitchUvX, fract(stitchUvY + 0.5));
        vec4 bottomStitchColor = texture(stitchTex, bottomStitchUV) * color;
        if (bottomStitchColor.a > 0.7)
            fragColor = mix(bottomStitchColor, fragColor, 1.0 - prevColor.a);
    }

    fragColor = mix(vec4(0, 0, 0, 1), fragColor, fragColor.a);
}