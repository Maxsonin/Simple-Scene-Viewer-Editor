#pragma once

#include <glad/glad.h>
#include "../Scene.h"
#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Debuging.h"
#include "../../Lightning/PointLight.h"
#include "../../Lightning/DirectionLight.h"
#include "../../Lightning/SpotLight.h"

#include "../include/Model/Model.h"

namespace Scene
{
    class BackpackModel : public Scene
    {
    private:
        Shader defaultShader;
        Model ourModel;

    public:
        BackpackModel(ApplicationSettings* appSet)
            : Scene(appSet)
        {
            GL_CHECK(defaultShader = Shader("./resources/shaders/ModelVert.glsl", "./resources/shaders/ModelFrag.glsl"));

            std::cout << "Loading Model...\n";
            GL_CHECK(ourModel = Model("./resources/objects/backpack/backpack.obj"));
            std::cout << "Model Loaded Successfully\n";

            glCheckError();
        }

        ~BackpackModel() override {}

        void OnUpdate(float deltaTime) override {}

        void OnRender() override
        {
            // MVP
            glm::mat4 model(1.0f);
            glm::mat4 view = m_ApplicationSettings->application->getCamera().GetViewMatrix();
            float FOV = m_ApplicationSettings->application->getCamera().getFOV();
            float aspectRatio = float(m_ApplicationSettings->application->getWindowWidth()) / m_ApplicationSettings->application->getWindowHeight();
            glm::mat4 perspective = glm::perspective(glm::radians(FOV), aspectRatio, 0.01f, 1000.f);

            defaultShader.Bind();
            defaultShader.setMat4("u_ModelMatrix", model);
            defaultShader.setMat4("u_ViewMatrix", view);
            defaultShader.setMat4("u_Projection", perspective);

            GL_CHECK(ourModel.Draw(defaultShader));
            glCheckError();

            defaultShader.Unbind();
            glCheckError();
        }



        void OnImGuirender() override
        {
            ImGui::Begin("Scene Settings");

            ImGui::End();
        }
    };
}
