#version 450

layout (binding = 0) uniform Variables
{
    float leftSeparator;
    float rightSeparator;
} variables;

layout (binding = 1) uniform sampler2D mainTex;

layout (location = 0) in vec2 uv0;

layout (location = 0) out vec4 fragColor;

const float offset[5] = float[](0.0, 0.002, 0.004, 0.006, 0.008);
const float weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
    if (uv0.x >= variables.leftSeparator && uv0.x <= variables.rightSeparator)
    {
        fragColor = texture(mainTex, uv0, 1) * weight[0];
        for (int i = 1; i < 5; i++)
        {
            fragColor += texture(mainTex, uv0 + vec2(0, offset[i]), 1) * weight[i];
            fragColor += texture(mainTex, uv0 - vec2(0, offset[i]), 1) * weight[i];
        }
    }
    else
        fragColor = texture(mainTex, uv0);
}