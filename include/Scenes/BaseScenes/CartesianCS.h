#pragma once

#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

#include "Application/Application.h"
#include "Debugging/Debugging.h"
#include "Renderer/Renderer.h"
#include "Scenes/Scene.h"

namespace Scene
{
    class CartesianCS : public Scene
    {
    private:
        VertexArray VAO;

        VertexBuffer VBO;

        Shader CartesianCSshader;

        std::vector<float> vertices =
        {
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
            GL_CHECK(CartesianCSshader = Shader("./resources/shaders/CartesianCSVert.glsl", "./resources/shaders/CartesianCSFrag.glsl"));
            glCheckError();
            std::vector<float> gridVertices = CreateGridVertices(100.0f, 3.0f);
            vertices.insert(vertices.end(), gridVertices.begin(), gridVertices.end());

            VAO.Bind();
            glCheckError();
                VBO = VertexBuffer(vertices.data(), vertices.size() * sizeof(GLfloat));
                VBO.Bind();
                glCheckError();
                VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
                glCheckError();
                VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                glCheckError();
            VAO.Unbind();
            glCheckError();

            glCheckError();
        }

        ~CartesianCS() override {}

        void OnUpdate(float deltaTime) override {}

        void OnRender() override
        {
            // MVP
            glm::mat4 model(1.0f);
            glm::mat4 view = m_ApplicationSettings->application->getCamera().GetViewMatrix();
            glm::mat4 perspective = m_ApplicationSettings->application->GetProjectionMatrix();

            CartesianCSshader.Bind();
            CartesianCSshader.setMat4("u_ModelMatrix", model);
            CartesianCSshader.setMat4("u_ViewMatrix", view);
            CartesianCSshader.setMat4("u_Projection", perspective);

            VAO.Bind();
            GL_CHECK(Renderer::DrawLines(CartesianCSshader, 0, vertices.size() / 6));
            VAO.Unbind();

            CartesianCSshader.Unbind();

            glCheckError();
        }

        void OnImGuirender() override {}

        std::vector<float> CreateGridVertices(float gridSize, float step)
        {
            std::vector<GLfloat> gridVertices;

            float start = -gridSize * step / 2;
            float end = gridSize * step / 2;

            float lineLenght = gridSize * step / 2 + step;
            float lineColor = 0.4;

            for (float x = start; x <= end; x += step)
            {
                if (x != 0) // left place for the X-axis
                {
                    gridVertices.push_back(x); gridVertices.push_back(0.0f); gridVertices.push_back(lineLenght); // line start
                    gridVertices.push_back(lineColor); gridVertices.push_back(lineColor); gridVertices.push_back(lineColor); // gray color
                    gridVertices.push_back(x); gridVertices.push_back(0.0f); gridVertices.push_back(-lineLenght); // line end
                    gridVertices.push_back(lineColor); gridVertices.push_back(lineColor); gridVertices.push_back(lineColor); // gray color
                }
            }

            for (float z = start; z <= end; z += step)
            {
                if (z != 0) // left place for the Z-axis
                {
                    gridVertices.push_back(lineLenght); gridVertices.push_back(0.0f); gridVertices.push_back(z); // line start
                    gridVertices.push_back(lineColor); gridVertices.push_back(lineColor); gridVertices.push_back(lineColor); // gray color
                    gridVertices.push_back(-lineLenght); gridVertices.push_back(0.0f); gridVertices.push_back(z); // line end
                    gridVertices.push_back(lineColor); gridVertices.push_back(lineColor); gridVertices.push_back(lineColor); // gray color
                }
            }
            
            return gridVertices;
        }
    };
}
