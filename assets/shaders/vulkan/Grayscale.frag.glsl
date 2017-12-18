#version 450

layout (binding = 0) uniform Variables
{
    float leftSeparator;
    float rightSeparator;
} variables;

layout (binding = 1) uniform sampler2D mainTex;

layout (location = 0) in vec2 uv0;

layout (location = 0) out vec4 fragColor;

void main()
{
    vec4 color = texture(mainTex, uv0, 1);
    if (uv0.x >= variables.leftSeparator && uv0.x <= variables.rightSeparator)
    {
        float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
        color = vec4(gray, gray, gray, 1);
    }
    fragColor = color;
}