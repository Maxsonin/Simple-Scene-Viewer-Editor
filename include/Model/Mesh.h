#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "Shaders/Shader.h"
#include "Debugging/Debugging.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Renderer/Renderer.h"

struct Tex
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    std::vector<Vertex>       m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<Tex>          m_Textures;
    VertexArray m_VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Tex> textures)
    {
        m_Vertices = vertices; m_Indices = indices; m_Textures = textures;

        setupMesh();
    }

    // Render the mesh
    void Draw(Shader& shader)
    {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            //std::cout << "UNIT: " << i << std::endl;
            std::string number;
            std::string name = m_Textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);
            else if (name == "texture_height")
                number = std::to_string(heightNr++);
             
            if (name != "texture_normal" && name != "texture_height")
            {
                // now set the sampler to the correct texture unit
                int location = glGetUniformLocation(shader.m_RendererID, ("u_Material." + name + number).c_str());
                if (location < 0) 
                {
                    std::cout << "Error in location for: " << "u_Material." + name + number << std::endl;
                }
                glUniform1i(location, i);
                // and finally bind the texture
                glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
                //std::cout << m_Textures[i].id << std::endl;
            }
            glCheckError();
        }

        m_VAO.Bind();
        GL_CHECK(Renderer::DrawWithTriangles(m_VAO, m_EBO, shader));
        m_VAO.Unbind();

        for (size_t i = 0; i < m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glCheckError();
    }

private:
    VertexBuffer m_VBO;
    IndexBuffer  m_EBO;

    // Initializes all the buffer objects/arrays
    void setupMesh()
    {
        GL_CHECK(m_VBO = VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex)));
        GL_CHECK(m_EBO = IndexBuffer(m_Indices.data(), m_Indices.size() * sizeof(unsigned int)));

        m_VAO.Bind(); m_VBO.Bind(); m_EBO.Bind();

        // Set the vertex attribute pointers
        m_VAO.LinkAttrib(m_VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);                           // Vertex Positions
        m_VAO.LinkAttrib(m_VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));    // Vertex Normals
        m_VAO.LinkAttrib(m_VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); // Vertex Texture Coordinates

        m_VAO.Unbind();

        glCheckError();
    }
};
#endif