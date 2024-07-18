#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    vec4 diffuseColor = texture(texture_diffuse1, TexCoords);
    vec4 specularColor = texture(texture_specular1, TexCoords);
    vec4 color = diffuseColor * specularColor;
    FragColor = vec4(color.rgb, 1.0);
}
