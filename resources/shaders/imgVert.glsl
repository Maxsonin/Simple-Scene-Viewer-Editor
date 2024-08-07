#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoordinates;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

out vec2 TextureCoordinates;

void main()
{    
    TextureCoordinates = aTextureCoordinates;

    gl_Position = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(aPos, 1.0);
}
