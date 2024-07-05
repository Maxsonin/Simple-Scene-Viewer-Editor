#version 330 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D img;

void main()
{
    FragColor = texture(img, TextureCoordinates);
}