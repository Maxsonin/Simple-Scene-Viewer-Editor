#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "Application/Application.h"
#include "Debugging/Debugging.h"
#include "Renderer/Renderer.h"
#include "Scenes/Scene.h"
#include "Textures/Texture.h"
#include "Lightning/PointLight.h"
#include "Lightning/DirectionLight.h"
#include "Lightning/SpotLight.h"
#include "Model/Model.h"

namespace Scene
{
    class SpecularMaps : public Scene
    {
    private:
        Shader defaultShader;
        VertexArray planeVAO;
        VertexBuffer planeVBO;
        IndexBuffer planeEBO;

        GLfloat planeVertices[32] =
        {    // COORDINATES       // NORMALS          // TexCoord 
            -1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
            -1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
             1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
             1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f
        };

        GLuint planeIndices[6] =
        {
            0, 1, 2,
            0, 2, 3
        };

        struct Material
        {
            Texture diffuse;
            Texture specular;
            float   shininess;
        };

        PointLight light1;
        SpotLight spotlight1;

        Material plank;

        Model box;

    public:
        SpecularMaps(ApplicationSettings* appSet)
            : Scene(appSet),
            plank
            {
              Texture("./resources/specularMapsScene/planks.png", 0),
              Texture("./resources/specularMapsScene/planksSpec.png", 1),
              32.0f
            }
        {
            GL_CHECK(defaultShader = Shader("./resources/shaders/lightReflectiveObjVert.glsl", "./resources/shaders/lightReflectiveObjFrag.glsl"));

            GL_CHECK(box = Model("./resources/objects/box.obj"));

            planeVBO = VertexBuffer(planeVertices, sizeof(planeVertices));
            planeEBO = IndexBuffer(planeIndices, sizeof(planeIndices));

            planeVAO.Bind(); planeVBO.Bind(); planeEBO.Bind();

            defaultShader.Bind();

            planeVAO.LinkAttrib(planeVBO, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)0);                     // Vertex Coordinates
            planeVAO.LinkAttrib(planeVBO, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // Normals
            planeVAO.LinkAttrib(planeVBO, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // Texture Coordinates
            planeVAO.Unbind(); planeVBO.Unbind(); planeEBO.Unbind();

            // Material Initialization
            plank.diffuse.PassTextureToShader(defaultShader, "u_Material.texture_diffuse1");
            plank.specular.PassTextureToShader(defaultShader, "u_Material.texture_specular1");
            defaultShader.setFloat("u_Material.shininess", plank.shininess);

            light1.position = glm::vec3(2.0f, 1.0f, 0.0f);
            spotlight1.position = glm::vec3(0.0f, 4.0f, 0.0f);

            pointLights.push_back(light1);
            spotLights.push_back(spotlight1);

            defaultShader.Unbind();

            glCheckError();
        }

        ~SpecularMaps() override {}

        void OnUpdate(float deltaTime) override {}

        void OnRender() override
        {
            defaultShader.Bind();

            UpdateBasicSettings(defaultShader);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);

            model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)); defaultShader.setMat4("u_ModelMatrix", model);
            box.Draw(defaultShader);

            // Bind Textures
            plank.specular.Bind(); plank.diffuse.Bind();

            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(4.5f, 2.0f, 4.5f)); defaultShader.setMat4("u_ModelMatrix", model);
            GL_CHECK(Renderer::DrawWithTriangles(planeVAO, planeEBO, defaultShader));

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
