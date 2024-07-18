#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Shaders/Shader.h"

class DirectionLight
{
public:
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionLight()
        : direction{ 0.0f, 0.0f, 0.0f }, // Can be { -0.2f, -1.0f, -0.3f } after enabling
        ambient{ 0.1f, 0.1f, 0.1f },
        diffuse{ 1.0f, 1.0f, 1.0f },
        specular{ 1.0f, 1.0f, 1.0f }
    { }

    void SetShaderUniforms(Shader& shader) const
    {
        shader.setVec3("dirLight.direction", direction);
        shader.setVec3("dirLight.ambient", ambient);
        shader.setVec3("dirLight.diffuse", diffuse);
        shader.setVec3("dirLight.specular", specular);
    }
};
