#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "Application/Application.h"
#include "Debugging/Debugging.h"
#include "Lightning/DirectionLight.h"
#include "Lightning/PointLight.h"
#include "Lightning/SpotLight.h"
#include "Model/Model.h"
#include "Renderer/Renderer.h"
#include "Scenes/Scene.h"

namespace Scene
{
    class BackpackModel : public Scene
    {
    private:
        Shader defaultShader;
        VertexArray planeVAO;
        VertexBuffer planeVBO;
        IndexBuffer planeEBO;

        Renderer renderer;

        Model backpack;

        DirectionLight dirLight;
        PointLight pointLights[2];
        SpotLight spotLights[2];

    public:
        BackpackModel(ApplicationSettings* appSet)
            : Scene(appSet)
        {
            GL_CHECK(defaultShader = Shader("./resources/shaders/lightReflectiveObjVert.glsl", "./resources/shaders/lightReflectiveObjFrag.glsl"));

            GL_CHECK(backpack = Model("./resources/objects/backpack/backpack.obj"));

            glCheckError();
        }

        ~BackpackModel() override {}

        void OnUpdate(float deltaTime) override {}

        void OnRender() override
        {
            // MVP
            glm::mat4 model(1.0f);
            glm::mat4 view = m_ApplicationSettings->application->getCamera().GetViewMatrix();
            glm::mat4 perspective = m_ApplicationSettings->application->GetProjectionMatrix();

            defaultShader.Bind();

            // Initialize back pack shininess
            defaultShader.setFloat("u_Material.shininess", 32.0f);

            // Light Initialization
            glm::vec3 viewPos = m_ApplicationSettings->application->getCamera().GetPosition();
            defaultShader.setVec3("u_ViewPos", viewPos);

            // Directional light
            dirLight.SetShaderUniforms(defaultShader);

            // Point lights
            for (int i = 0; i < 2; i++) { pointLights[i].SetShaderUniforms(defaultShader, i); }

            // Spot Lights
            for (int i = 0; i < 1; i++) { spotLights[i].SetShaderUniforms(defaultShader, i); }

            model = glm::mat4(1.0f);
            defaultShader.setMat4("u_ModelMatrix", model);
            defaultShader.setMat4("u_ViewMatrix", view);
            defaultShader.setMat4("u_Projection", perspective);

            GL_CHECK(pointLights[0].Render(view, perspective, viewPos));
            GL_CHECK(pointLights[1].Render(view, perspective, viewPos));

            GL_CHECK(spotLights[0].Render(view, perspective));

            defaultShader.Bind();

            backpack.Draw(defaultShader);

            defaultShader.Unbind();

            glCheckError();
        }


        void OnImGuirender() override
        {
            ImGui::Begin("Scene Settings");
            ImGui::Text("Light Settings:");
            ImGui::Text("\tDirection Light");
            ImGui::SliderFloat("Dir light PosX:", &dirLight.direction.x, -10, 10);
            ImGui::SliderFloat("Dir light PosY:", &dirLight.direction.y, -10, 10);
            ImGui::SliderFloat("Dir light PosZ:", &dirLight.direction.z, -10, 10);

            ImGui::Text("\tPosint Light");

            ImGui::SliderFloat("Point1 light PosX:", &pointLights[0].position.x, -10, 10);
            ImGui::SliderFloat("Point1 light PosY:", &pointLights[0].position.y, -10, 10);
            ImGui::SliderFloat("Point1 light PosZ:", &pointLights[0].position.z, -10, 10);

            ImGui::SliderFloat("Point2 light PosX:", &pointLights[1].position.x, -10, 10);
            ImGui::SliderFloat("Point2 light PosY:", &pointLights[1].position.y, -10, 10);
            ImGui::SliderFloat("Point2 light PosZ:", &pointLights[1].position.z, -10, 10);

            ImGui::Text("\tSpot Light");

            ImGui::SliderFloat("Spot light DirX:", &spotLights[0].direction.x, -10, 10);
            ImGui::SliderFloat("Spot light DirY:", &spotLights[0].direction.y, -10, 10);
            ImGui::SliderFloat("Spot light DirZ:", &spotLights[0].direction.z, -10, 10);

            ImGui::SliderFloat("Spot light PosX:", &spotLights[0].position.x, -10, 10);
            ImGui::SliderFloat("Spot light PosY:", &spotLights[0].position.y, -10, 10);
            ImGui::SliderFloat("Spot light PosZ:", &spotLights[0].position.z, -10, 10);

            ImGui::End();
        }
    };
}
