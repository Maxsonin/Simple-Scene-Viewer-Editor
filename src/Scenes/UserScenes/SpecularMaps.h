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
        float posX, posY, posZ;

        Texture planksTexture, planksSpecularTex;

        Shader lightShader, reflectiveObjShader;
        VertexArray planeVAO, lightVAO;
        VertexBuffer planeVBO, lightVBO;
        IndexBuffer planeEBO, lightEBO;

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

        GLfloat cubeVertices[192] =
        {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f
        };

        GLuint cubeIndices[36] = {
            0, 1, 2, 2, 3, 0,         // back face
            4, 5, 6, 6, 7, 4,         // front face
            8, 9, 10, 10, 11, 8,      // left face
            12, 13, 14, 14, 15, 12,   // right face
            16, 17, 18, 18, 19, 16,   // bottom face
            20, 21, 22, 22, 23, 20    // top face
        };

    public:
        SpecularMaps(ApplicationSettings* appSet)
            : Scene(appSet),
            posX(0.0f), posY(1.0f), posZ(0.0f),
            planksTexture("./resources/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
            planksSpecularTex("./resources/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE)
        {
            GL_CHECK(lightShader = Shader("./shaders/lightVert.glsl", "./shaders/lightFrag.glsl"));
            GL_CHECK(reflectiveObjShader = Shader("./shaders/lightReflectiveObjVert.glsl", "./shaders/lightReflectiveObjFrag.glsl"));

            lightVBO = VertexBuffer(cubeVertices, sizeof(cubeVertices));
            lightEBO = IndexBuffer(cubeIndices, sizeof(cubeIndices));
            lightShader.Bind();
            lightVAO.Bind(); lightVBO.Bind(); lightEBO.Bind();
            lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)0); // Vertex Positions

            planeVBO = VertexBuffer(planeVertices, sizeof(planeVertices));
            planeEBO = IndexBuffer(planeIndices, sizeof(planeIndices));
            reflectiveObjShader.Bind();
            planeVAO.Bind(); planeVBO.Bind(); planeEBO.Bind();
            planeVAO.LinkAttrib(planeVBO, 0, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void*)0);                     // Vertex Coordinats
            planeVAO.LinkAttrib(planeVBO, 1, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // Normals
            planeVAO.LinkAttrib(planeVBO, 2, 3, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // Color
            planeVAO.LinkAttrib(planeVBO, 3, 2, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat))); // Texture Coordinates

            planksTexture.texUnit(reflectiveObjShader, "u_Material.diffuse", 0); planksTexture.Bind();
            planksSpecularTex.texUnit(reflectiveObjShader, "u_Material.specular", 1); planksSpecularTex.Bind();
        }

        ~SpecularMaps() override {}

        void OnUpdate(float deltaTime) override
        { }

        void OnRender() override
        {
            // MVP
            glm::mat4 model(1.0f);
            glm::mat4 view = m_ApplicationSettings->application->getCamera().GetViewMatrix();
            float FOV = m_ApplicationSettings->application->getCamera().getFOV();
            float aspectRatio = float(m_ApplicationSettings->application->getWindowWidth()) / m_ApplicationSettings->application->getWindowHeight();
            glm::mat4 perspective = glm::perspective(glm::radians(FOV), aspectRatio, 0.01f, 1000.f);

            lightShader.Bind();
            glm::vec3 lightPos = { posX, posY, posZ };
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            lightShader.setMat4("u_ModelMatrix", model);
            lightShader.setMat4("u_ViewMatrix", view);
            lightShader.setMat4("u_Projection", perspective);
            GL_CHECK(renderer.DrawWithEBO(lightVAO, lightEBO, lightShader));


            reflectiveObjShader.Bind();

            // Light Related Stuff
            glm::vec3 viewPos = m_ApplicationSettings->application->getCamera().GetPostion();
            reflectiveObjShader.setVec3("u_ViewPos", viewPos);
            reflectiveObjShader.setVec3("u_Light.position", lightPos);
            reflectiveObjShader.setVec3("u_Light.ambient", 0.2f, 0.2f, 0.2f);
            reflectiveObjShader.setVec3("u_Light.diffuse", 0.5f, 0.5f, 0.5f);
            reflectiveObjShader.setVec3("u_Light.specular", 0.5f, 0.5f, 0.5f);
            reflectiveObjShader.setFloat("u_Material.shininess", 16.0f);

            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(4.5f, 0.0f, 4.5f));
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            reflectiveObjShader.setMat4("u_ModelMatrix", model);
            reflectiveObjShader.setMat4("u_ViewMatrix", view);
            reflectiveObjShader.setMat4("u_Projection", perspective);
            GL_CHECK(renderer.DrawWithEBO(planeVAO, planeEBO, reflectiveObjShader));

            glCheckError();
        }

        void OnImGuirender() override
        {
            ImGui::Begin("Scene Settings");
            ImGui::SliderFloat("Light Position X", &posX, -10.0f, 10.0f);
            ImGui::SliderFloat("Light Position Y", &posY, -10.0f, 10.0f);
            ImGui::SliderFloat("Light Position Z", &posZ, -10.0f, 10.0f);
            ImGui::End();
        }
    };
}
