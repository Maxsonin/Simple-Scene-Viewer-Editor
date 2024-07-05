#pragma once

#include "../Interfaces/Renderable.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../Buffers/VertexArray.h"
#include "../Buffers/IndexBuffer.h"
#include "../Shaders/Shader.h"
#include "../Textures/Texture.h"

class PointLight : public Renderable
{
public:
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    VertexArray VAO;
    VertexBuffer VBO;
    IndexBuffer EBO;
    Shader imgShader;

    Texture img;

    PointLight() = default;
    PointLight()
        : img("./resources/system/lightIco.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE),
        position{ 0.0f, 1.0f, 0.0f },
        constant(1.0f),
        linear(0.09f),
        quadratic(0.032),
        ambient{ 0.1f, 0.1f, 0.1f },
        diffuse{ 1.0f, 1.0f, 1.0f },
        specular{ 1.0f, 1.0f, 1.0f }

    {
        GLfloat vertices[] =
        {
            // positions          // texture Coords
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f
        };

        GLuint indices[] = 
        {
            0, 1, 2,
            0, 2, 3
        };

        imgShader = Shader("./shaders/imgVert.glsl", "./shaders/imgFrag.glsl");

        VBO = VertexBuffer(vertices, sizeof(vertices));
        EBO = IndexBuffer(indices, sizeof(indices));
        VAO.Bind(); VBO.Bind(); EBO.Bind();


        imgShader.Bind();
        VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 5 * sizeof(GLfloat), (void*)0); // Position Coordinates
        VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 5 * sizeof(GLfloat), (void*)0); // Texture Coordinates

        img.texUnit(imgShader, "u_img", 0); img.Bind();
        imgShader.Unbind();
    }

    void Render() override
    {
        
    }
    
    void Update(float deltaTime) override { }
};