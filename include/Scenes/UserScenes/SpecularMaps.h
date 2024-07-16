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

namespace Scene
{
    class SpecularMaps : public Scene
    {
    private:
        Shader defaultShader;
        VertexArray planeVAO;
        VertexBuffer planeVBO;
        IndexBuffer planeEBO;

        Renderer renderer;

        GLfloat planeVertices[44] =
        {    // COORDINATES       // NORMALS          // COLOR          // TexCoord 
            -1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
            -1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f,
             1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,
             1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f
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

        Material plank;

        DirectionLight dirLight;
        PointLight pointLights[2];
        SpotLight spotLights[2];

        // Array of Point Lights
        glm::vec3 pointLightPos[2] = { glm::vec3(1.0f, 0.6f, 0.0f),
                                       glm::vec3(1.0f, 0.0f, 0.0f) };

    public:
        SpecularMaps(ApplicationSettings* appSet)
            : Scene(appSet),
            plank{
              Texture("./resources/planks.png", GL_TEXTURE_2D, 0, GL_UNSIGNED_BYTE),
              Texture("./resources/planksSpec.png", GL_TEXTURE_2D, 1, GL_UNSIGNED_BYTE),
              32.0f
            }
        {
            GL_CHECK(defaultShader = Shader("./resources/shaders/lightReflectiveObjVert.glsl", "./resources/shaders/lightReflectiveObjFrag.glsl"));

            planeVBO = VertexBuffer(planeVertices, sizeof(planeVertices));
            planeEBO = IndexBuffer(planeIndices, sizeof(planeIndices));

            planeVAO.Bind(); planeVBO.Bind(); planeEBO.Bind();

            defaultShader.Bind();

            planeVAO.LinkAttrib(planeVBO, 0, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void*)0);                     // Vertex Coordinates
            planeVAO.LinkAttrib(planeVBO, 1, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // Normals
            planeVAO.LinkAttrib(planeVBO, 2, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // Color
            planeVAO.LinkAttrib(planeVBO, 3, 2, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat))); // Texture Coordinates
            planeVAO.Unbind(); planeVBO.Unbind(); planeEBO.Unbind();

            // Material Initialization
            plank.diffuse.texUnit(defaultShader, "u_Material.diffuse", 0);
            plank.specular.texUnit(defaultShader, "u_Material.specular", 1);
            defaultShader.setFloat("u_Material.shininess", plank.shininess);

            defaultShader.Unbind();

            glCheckError();
        }

        ~SpecularMaps() override {}

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

            // Bind Textures
            plank.diffuse.Bind(); plank.specular.Bind();

            // Light Initialization
            glm::vec3 viewPos = m_ApplicationSettings->application->getCamera().GetPosition();
            defaultShader.setVec3("u_ViewPos", viewPos);

            // Directional light
            dirLight.SetShaderUniforms(defaultShader);

            // Point lights
            for (int i = 0; i < 2; i++) { pointLights[i].SetShaderUniforms(defaultShader, i); }

            // Spot Lights
            for (int i = 0; i < 1; i++) { spotLights[i].SetShaderUniforms(defaultShader, i); }

            // Render the Plane
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(4.5f, 1.0f, 4.5f));
            defaultShader.setMat4("u_ModelMatrix", model);
            defaultShader.setMat4("u_ViewMatrix", view);
            defaultShader.setMat4("u_Projection", perspective);

            GL_CHECK(renderer.DrawWithEBO(planeVAO, planeEBO, defaultShader));

            GL_CHECK(pointLights[0].Render(view, perspective, viewPos));
            GL_CHECK(pointLights[1].Render(view, perspective, viewPos));

            GL_CHECK(spotLights[0].Render(view, perspective));

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
