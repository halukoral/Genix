#version 450 core
layout (location = 0) in vec3 a_Position;

out vec3 WorldPos;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    WorldPos = a_Position;
    
    mat4 rotView = mat4(mat3(u_View));
    vec4 clipPos = u_Projection * rotView * vec4(WorldPos, 1.0);
    
    gl_Position = clipPos.xyww;
}