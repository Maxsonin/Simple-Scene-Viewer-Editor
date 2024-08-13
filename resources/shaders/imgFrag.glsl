#version 330 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D u_img;

const float near = 0.001;
const float far  = 200.0;

float linearizeDepth(float depth)
{
    return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness, float offset)
{
    float zVal = linearizeDepth(depth);
    return (1.0 / (1.0 + exp(-steepness * (zVal - offset))));
}

void main()
{
    vec4 imgColor = texture(u_img, TextureCoordinates);
    if (imgColor.a < 0.1) discard;

    float depth = logisticDepth(gl_FragCoord.z, 0.5, 5.0);
    vec3 depthColor = mix(imgColor.rgb, vec3(0.3, 0.3, 0.3), depth);

    FragColor = vec4(depthColor, imgColor.a);
}
