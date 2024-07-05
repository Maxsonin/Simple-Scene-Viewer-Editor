#pragma once

#include <glad/glad.h>
#include "../Scene.h"
#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Debuging.h"

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

        struct DirLight
        {
            glm::vec3 direction;

            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
        };

        struct PointLight
        {
            glm::vec3 position;

            float constant;
            float linear;
            float quadratic;

            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
        };

        struct SpotLight
        {
            glm::vec3 position;
            glm::vec3 direction;
            float innerCutOff;
            float outerCutOff;

            float constant;
            float linear;
            float quadratic;

            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
        };

        Material plank;

        DirLight dirLight;
        PointLight pointLights[2];
        SpotLight spotLight;

        // Array of Point Lights
        glm::vec3 pointLightPos[2] = { glm::vec3(1.0f, 0.6f, 0.0f),
                                       glm::vec3(1.0f, 0.0f, 0.0f) };

        glm::vec3 pointLightColors[2] = { glm::vec3(0.0f, 1.0f, 0.0f),
                                          glm::vec3(1.0f, 1.0f, 0.0f) };

    public:
        SpecularMaps(ApplicationSettings* appSet)
            : Scene(appSet),
            plank{
              Texture("./resources/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
              Texture("./resources/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE),
              32.0f
            }
        {
            GL_CHECK(defaultShader = Shader("./shaders/lightReflectiveObjVert.glsl", "./shaders/lightReflectiveObjFrag.glsl"));

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

            // Directional light
            dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
            dirLight.ambient = glm::vec3(0.3f, 0.24f, 0.14f);
            dirLight.diffuse = glm::vec3(0.7f, 0.42f, 0.26f);
            dirLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);

            // Point Light 1
            pointLights[0].position = pointLightPos[0];
            pointLights[0].constant = 1.0f;
            pointLights[0].linear = 0.09f;
            pointLights[0].quadratic = 0.032;
            pointLights[0].ambient = pointLightColors[0] * 0.1f;
            pointLights[0].diffuse = pointLightColors[0];
            pointLights[0].specular = pointLightColors[0];

            // Point Light 2
            pointLights[1].position = pointLightPos[1];
            pointLights[1].constant = 1.0f;
            pointLights[1].linear = 0.09f;
            pointLights[1].quadratic = 0.032;
            pointLights[1].ambient = pointLightColors[1] * 0.1f;
            pointLights[1].diffuse = pointLightColors[1];
            pointLights[1].specular = pointLightColors[1];

            // Spot Light
            spotLight.position = glm::vec3(0.0f, 1.0f, 2.0f);
            spotLight.direction = glm::vec3(0.0f, -1.0f, -1.0f);
            spotLight.innerCutOff = glm::cos(glm::radians(12.5f));
            spotLight.outerCutOff = glm::cos(glm::radians(15.0f));

            spotLight.constant = 1.0f;
            spotLight.linear = 0.09f;
            spotLight.quadratic = 0.032f;

            spotLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
            spotLight.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
            spotLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

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
            defaultShader.setVec3("dirLight.direction", dirLight.direction);
            defaultShader.setVec3("dirLight.ambient", dirLight.ambient);
            defaultShader.setVec3("dirLight.diffuse", dirLight.diffuse);
            defaultShader.setVec3("dirLight.specular", dirLight.specular);

            // Point lights
            for (int i = 0; i < 2; i++) {
                std::string number = std::to_string(i);
                defaultShader.setVec3("pointLights[" + number + "].position", pointLights[i].position);
                defaultShader.setFloat("pointLights[" + number + "].constant", pointLights[i].constant);
                defaultShader.setFloat("pointLights[" + number + "].linear", pointLights[i].linear);
                defaultShader.setFloat("pointLights[" + number + "].quadratic", pointLights[i].quadratic);
                defaultShader.setVec3("pointLights[" + number + "].ambient", pointLights[i].ambient);
                defaultShader.setVec3("pointLights[" + number + "].diffuse", pointLights[i].diffuse);
                defaultShader.setVec3("pointLights[" + number + "].specular", pointLights[i].specular);
            }

            defaultShader.setVec3("spotLight.position", spotLight.position);
            defaultShader.setVec3("spotLight.direction", spotLight.direction);
            defaultShader.setFloat("spotLight.innerCutOff", spotLight.innerCutOff);
            defaultShader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
            defaultShader.setFloat("spotLight.constant", spotLight.constant);
            defaultShader.setFloat("spotLight.linear", spotLight.linear);
            defaultShader.setFloat("spotLight.quadratic", spotLight.quadratic);
            defaultShader.setVec3("spotLight.ambient", spotLight.ambient);
            defaultShader.setVec3("spotLight.diffuse", spotLight.diffuse);
            defaultShader.setVec3("spotLight.specular", spotLight.specular);

            // Render the Plane
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(4.5f, 1.0f, 4.5f));
            defaultShader.setMat4("u_ModelMatrix", model);
            defaultShader.setMat4("u_ViewMatrix", view);
            defaultShader.setMat4("u_Projection", perspective);
            GL_CHECK(renderer.DrawWithEBO(planeVAO, planeEBO, defaultShader));

            defaultShader.Unbind();

            glCheckError();
        }


        void OnImGuirender() override
        {
            ImGui::Begin("Scene Settings");
            ImGui::Text("Light Settings:");
            ImGui::SliderFloat("Dir light PosX:", &dirLight.direction.x, -10, 10);
            ImGui::SliderFloat("Dir light PosY:", &dirLight.direction.y, -10, 10);
            ImGui::SliderFloat("Dir light PosZ:", &dirLight.direction.z, -10, 10);

            ImGui::SliderFloat("Point1 light PosX:", &dirLight.direction.x, -10, 10);
            ImGui::SliderFloat("Point1 light PosY:", &dirLight.direction.y, -10, 10);
            ImGui::SliderFloat("Point1 light PosZ:", &dirLight.direction.z, -10, 10);

            ImGui::SliderFloat("Point2 light PosX:", &dirLight.direction.x, -10, 10);
            ImGui::SliderFloat("Point2 light PosY:", &dirLight.direction.y, -10, 10);
            ImGui::SliderFloat("Point2 light PosZ:", &dirLight.direction.z, -10, 10);

            ImGui::End();
        }
    };
}
