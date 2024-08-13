#version 330 core

out vec4 FragColor;

in vec3 ourColor;

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
    float depth = logisticDepth(gl_FragCoord.z, 0.5, 5.0);
    FragColor = vec4(ourColor, 1.0) * (1.0 - depth) + vec4(depth * vec3(0.3, 0.3, 0.3), 1.0);
}
