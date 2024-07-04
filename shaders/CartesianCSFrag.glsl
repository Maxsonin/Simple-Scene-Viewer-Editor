#version 330 core

out vec4 FragColor;

in vec3 ourColor;

float near = .001f;
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
    float depth = logisticDepth(gl_FragCoord.z);
    FragColor = vec4(ourColor, 1.0f) * (1.0f - depth) + vec4(depth * vec3(0.3f, 0.3f, 0.3f), 1.0f);
}
