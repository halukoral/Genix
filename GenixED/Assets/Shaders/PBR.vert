#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout(location = 7) in int a_EntityID;

out flat int v_EntityID;
out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out vec3 ViewPos;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
    vec3 u_ViewPos;
};

uniform mat4 u_Transform;
uniform mat3 u_Normal;

void main()
{
    TexCoords = a_TexCoord;
    Normal = u_Normal * a_Normal;
    ViewPos = u_ViewPos;
    v_EntityID = a_EntityID;

    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    //WorldPos = vec3(model * vec4(a_Position, 1.0));
    //gl_Position =  projection * view * vec4(WorldPos, 1.0);
}