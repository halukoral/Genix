#version 450 core

layout(location=0) in vec3 localPosition;
layout(location=0) out vec4 color;

layout(binding=0) uniform samplerCube envTexture;

void main()
{
    vec3 envVector = normalize(localPosition);
    color = textureLod(envTexture, envVector, 0);
}
