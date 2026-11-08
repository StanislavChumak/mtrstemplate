#version 330 core

layout(location = 0) in vec2 aVertex;
layout(location = 1) in vec2 aUV;

layout(location = 2) in vec2 aPosition;
layout(location = 3) in vec2 aSize;
layout(location = 4) in float aRotation;
layout(location = 5) in vec2 aLbUv;
layout(location = 6) in vec2 aRtUv;
layout(location = 7) in vec4 aColor;
layout(location = 8) in float aLayer;

layout (std140) uniform UiMatrices
{
    mat4 projection;
};

out vec4 color;
out vec2 UV;

void main()
{
    color = aColor;
    UV = mix(aLbUv, aRtUv, vec2(aUV.x, 1.0 - aUV.y));

    vec2 local = aVertex * aSize;
    float s = sin(aRotation), c = cos(aRotation);
    vec2 rotated = vec2(local.x * c - local.y * s, local.x * s + local.y * c);
    vec2 centerOffset = aSize * 0.5;

    gl_Position = projection * vec4(aPosition + centerOffset + rotated, 0.0, 1.0);
}