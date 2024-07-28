#version 330 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D u_img;

float near = 0.001f;
float far  = 200.0f;

float linearizeDepth(float depth)
{
    return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.5f, float offset = 5.0f)
{
    float zVal = linearizeDepth(depth);
    return (1 / (1 + exp(-steepness * (zVal - offset))));
}

void main()
{
    vec4 imgColor = texture(u_img, TextureCoordinates);
    if(imgColor.a < 0.1) discard;

    float depth = logisticDepth(gl_FragCoord.z);
    vec3 depthColor = mix(imgColor.rgb, vec3(0.3, 0.3, 0.3), depth);

    FragColor = vec4(depthColor, imgColor.a);
}