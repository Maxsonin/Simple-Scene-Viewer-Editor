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

        Model backpack;

    public:
        BackpackModel(ApplicationSettings* appSet)
            : Scene(appSet)
        {
            GL_CHECK(defaultShader = Shader("./resources/shaders/lightReflectiveObjVert.glsl", "./resources/shaders/lightReflectiveObjFrag.glsl"));

            GL_CHECK(backpack = Model("./resources/objects/backpack/backpack.obj"));

            isDirectionLightEnabled = true;

            glCheckError();
        }

        ~BackpackModel() override {}

        void OnUpdate(float deltaTime) override {}

        void OnRender() override
        {
            defaultShader.Bind();

            UpdateBasicSettings(defaultShader);

            // Initialize back pack shininess
            defaultShader.setFloat("u_Material.shininess", 32.0f);

            GL_CHECK(backpack.Draw(defaultShader));

            RenderBasicElements(defaultShader);

            defaultShader.Unbind();

            glCheckError();
        }


        void OnImGuirender() override
        {
            ImGui::Begin("Scene Settings");

            if (ImGui::CollapsingHeader("Lightning"))
            {
                ImGui::Checkbox("Enable Direction Light ", &isDirectionLightEnabled);

                for (size_t i = 0; i < pointLights.size(); i++)
                {
                    pointLights[i].AddLightSettings(i);
                }

                for (size_t i = 0; i < spotLights.size(); i++)
                {
                    spotLights[i].AddLightSettings(i);
                }

                if (isDirectionLightEnabled)
                {
                    directionLight.AddLightSettings();
                }
            }

            ImGui::End();
        }
    };
}
