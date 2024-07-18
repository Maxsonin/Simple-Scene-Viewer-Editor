#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal; // not particularly normalized
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main()
{
    FragPos  = vec3(u_ModelMatrix * vec4(aPos, 1.0));
    Normal   = aNormal; // Normal = mat3(transpose(inverse(u_ModelMatrix))) * aNormal;
    TexCoord = aTexCoord;

    gl_Position = u_Projection * u_ViewMatrix * vec4(FragPos, 1.0);
}
