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
    class Grid : public Scene
    {
    private:
        Renderer renderer;

        VertexArray VAO;
        VertexBuffer VBO;
        Shader CartesianCSshader;

        std::vector<float> gridVertices;

    private:
        void generateGridVertices(float gridSize, float step)
        {
            float start = -gridSize * step / 2;
            float end = gridSize * step / 2;

            float ll = gridSize * step / 2 + step;

            float lineGrayTone = 0.4;

            for (float x = start; x <= end; x += step)
            {
                if (x != 0)
                {
                    gridVertices.push_back(x); gridVertices.push_back(ll); gridVertices.push_back(0.0f); // line start
                    gridVertices.push_back(lineGrayTone); gridVertices.push_back(lineGrayTone); gridVertices.push_back(lineGrayTone); // gray color
                    gridVertices.push_back(x); gridVertices.push_back(-ll); gridVertices.push_back(0.0f); // line end
                    gridVertices.push_back(lineGrayTone); gridVertices.push_back(lineGrayTone); gridVertices.push_back(lineGrayTone); // gray color
                }
            }

            for (float y = start; y <= end; y += step)
            {
                if (y != 0)
                {
                    gridVertices.push_back(ll); gridVertices.push_back(y); gridVertices.push_back(0.0f); // line start
                    gridVertices.push_back(lineGrayTone); gridVertices.push_back(lineGrayTone); gridVertices.push_back(lineGrayTone); // gray color
                    gridVertices.push_back(-ll); gridVertices.push_back(y); gridVertices.push_back(0.0f); // line end
                    gridVertices.push_back(lineGrayTone); gridVertices.push_back(lineGrayTone); gridVertices.push_back(lineGrayTone); // gray color
                }
            }
        }

    public:
        Grid(ApplicationSettings* appSet)
            : Scene(appSet)
        {
            GL_CHECK(CartesianCSshader = Shader("./shaders/CartesianCSVert.glsl", "./shaders/CartesianCSFrag.glsl"));

            generateGridVertices(100.0f, 3.0f);

            CartesianCSshader.Bind();
            // Setup for grid
            VAO.Bind();
            VBO = VertexBuffer(gridVertices.data(), gridVertices.size() * sizeof(float)); VBO.Bind();
            VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
            VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            VAO.Unbind(); VBO.Unbind();

            glCheckError();
        }

        ~Grid() override
        { }

        void OnUpdate(float deltaTime) override
        { }

        void OnRender() override
        {
            // MVP
            glm::mat4 model(1.0f);
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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
            GL_CHECK(renderer.DrawWithoutEBO(CartesianCSshader, GL_LINES, 0, gridVertices.size() / sizeof(float)));

            glCheckError();
        }

        void OnImGuirender() override
        { }
    };
}
