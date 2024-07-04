#pragma once

#include "../Scene.h"
#include <glad/glad.h>
#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Debuging.h"
#include "../../Renderer/Renderer.h"

namespace Scene
{
    class CartesianCS : public Scene
    {
    private:
        Renderer renderer;

        VertexArray VAO;
        VertexBuffer VBO;
        Shader CartesianCSshader;

        float vertices[36] = {
            // X axis (red)
            -1000.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
             1000.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
             // Y axis (green)
             0.0f, -1000.0f,  0.0f, 0.0f, 1.0f, 0.0f,
             0.0f,  1000.0f,  0.0f, 0.0f, 1.0f, 0.0f,
             // Z axis (blue)
             0.0f,  0.0f, -1000.0f, 0.0f, 0.0f, 1.0f,
             0.0f,  0.0f,  1000.0f, 0.0f, 0.0f, 1.0f
        };

    public:
        CartesianCS(ApplicationSettings* appSet)
            : Scene(appSet)
        {
            GL_CHECK(CartesianCSshader = Shader("./shaders/CartesianCSVert.glsl", "./shaders/CartesianCSFrag.glsl"));

            CartesianCSshader.Bind();
            // Setup for axes
            VAO.Bind();
            VBO = VertexBuffer(vertices, sizeof(vertices)); VBO.Bind();
            VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
            VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            VAO.Unbind(); VBO.Unbind();

            glCheckError();
        }

        ~CartesianCS() override
        { }

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

            CartesianCSshader.Bind();
            CartesianCSshader.setMat4("u_ModelMatrix", model);
            CartesianCSshader.setMat4("u_ViewMatrix", view);
            CartesianCSshader.setMat4("u_Projection", perspective);

            // Render axes
            VAO.Bind();
            GL_CHECK(renderer.DrawWithoutEBO(CartesianCSshader, GL_LINES, 0, 6));

            glCheckError();
        }

        void OnImGuirender() override
        { }
    };
}
