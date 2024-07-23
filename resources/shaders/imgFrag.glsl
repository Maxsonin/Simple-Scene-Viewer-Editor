#version 330 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D u_img;

void main()
{
    vec4 imgColor = texture(u_img, TextureCoordinates);
    if(imgColor.a < 0.1)
        discard;
    FragColor = imgColor;
}