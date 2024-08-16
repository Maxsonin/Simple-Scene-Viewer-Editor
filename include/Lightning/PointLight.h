#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Buffers/VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"
#include "Debugging/Debugging.h"
#include "Debugging/Logger.h"

class PointLight
{
public:
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
private:
    VertexArray VAO;
    VertexBuffer VBO;
    IndexBuffer EBO;
    Shader imgShader;

    Texture img;

public:
    PointLight()
        : img("./resources/system/lightIco.png", 0),
        position{ 0.0f, 1.0f, 0.0f },
        constant(1.0f),
        linear(0.09f),
        quadratic(0.032),
        ambient{ 0.1f, 0.1f, 0.1f },
        diffuse{ 1.0f, 1.0f, 1.0f },
        specular{ 1.0f, 1.0f, 1.0f }
    {
        GLfloat vertices[] =
        {
            // positions          // texture Coords
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f
        };

        GLuint indices[] =
        {
            0, 1, 2,
            0, 2, 3
        };

        imgShader = Shader("imgShader", "./resources/shaders/imgVert.glsl", "./resources/shaders/imgFrag.glsl");

        VBO = VertexBuffer(vertices, sizeof(vertices));
        EBO = IndexBuffer(indices, sizeof(indices));

        imgShader.Bind();
        VAO.Bind(); VBO.Bind(); EBO.Bind();

        VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 5 * sizeof(GLfloat), (void*)0); // Position Coordinates
        VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // Texture Coordinates

        img.PassTextureToShader(imgShader, "u_img"); img.Bind();

        VAO.Unbind(); VBO.Unbind(); EBO.Unbind();

        imgShader.Unbind();

        glCheckError();

        Logger::WriteLog("Point Light Crated");
    }

    void Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 viewPos)
    {
        imgShader.Bind();

        glm::vec3 viewDir = glm::normalize(viewPos - position);
        float angle = atan2(viewDir.x, viewDir.z);

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);

        modelMatrix = modelMatrix * rotationMatrix;
        imgShader.setMat4("u_ModelMatrix", modelMatrix);
        imgShader.setMat4("u_ViewMatrix", viewMatrix);
        imgShader.setMat4("u_Projection", projectionMatrix);

        img.Bind();
        VAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        VAO.Unbind();
        img.Unbind();

        imgShader.Unbind();

        glCheckError();
    }


    void SetShaderUniforms(Shader& shader, int index) const
    {
        std::string baseName = "pointLights[" + std::to_string(index) + "]";
        shader.setVec3((baseName + ".position").c_str(), position);
        shader.setFloat((baseName + ".constant").c_str(), constant);
        shader.setFloat((baseName + ".linear").c_str(), linear);
        shader.setFloat((baseName + ".quadratic").c_str(), quadratic);
        shader.setVec3((baseName + ".ambient").c_str(), ambient);
        shader.setVec3((baseName + ".diffuse").c_str(), diffuse);
        shader.setVec3((baseName + ".specular").c_str(), specular);

        glCheckError();
    }

    void AddLightSettings(int i)
    {
        if (ImGui::CollapsingHeader("Point Light " + i))
        {
            if (ImGui::TreeNode("Basic Settings"))
            {
                ImGui::InputFloat("X Position", &position.x);
                ImGui::InputFloat("Y Position", &position.y);
                ImGui::InputFloat("Z Position", &position.z);
                ImGui::ColorEdit3("Diffuse Color", (float*)&diffuse);

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Advanced Settings"))
            {
                ImGui::InputFloat("Constant", &constant);
                ImGui::InputFloat("Linear", &linear);
                ImGui::InputFloat("Quadratic", &quadratic);
                ImGui::ColorEdit3("Ambient Color", (float*)&ambient);
                ImGui::ColorEdit3("Specular Color", (float*)&specular);

                ImGui::TreePop();
            }
        }
    }
};
