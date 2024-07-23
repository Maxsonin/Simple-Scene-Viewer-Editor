#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Shaders/Shader.h"
#include <imgui.h>

class DirectionLight
{
public:
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionLight()
        : direction{ -0.2f, -1.0f, -0.3f },
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

    void AddLightSettings()
    {
        if (ImGui::CollapsingHeader("Direction Light"))
        {
            if (ImGui::TreeNode("Basic Settings"))
            {
                ImGui::InputFloat3("Direction", (float*)&direction);
                ImGui::ColorEdit3("Diffuse Color", (float*)&diffuse);

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Advanced Settings"))
            {
                ImGui::ColorEdit3("Ambient Color", (float*)&ambient);
                ImGui::ColorEdit3("Specular Color", (float*)&specular);

                ImGui::TreePop();
            }
        }
    }
};
