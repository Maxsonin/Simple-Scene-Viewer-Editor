#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(aPos, 1.0);
}
