#version 450 core
layout (location = 0) in vec3 a_Position;

out vec3 WorldPos;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    WorldPos = a_Position;
    gl_Position =  u_Projection * u_View * vec4(WorldPos, 1.0);
}