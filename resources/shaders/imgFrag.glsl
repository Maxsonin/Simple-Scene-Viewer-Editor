#version 330 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D u_img;

void main()
{
    FragColor = texture(u_img, TextureCoordinates);
}