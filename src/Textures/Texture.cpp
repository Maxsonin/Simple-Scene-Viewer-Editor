#include <iostream>

#include "Textures/Texture.h"

Texture::Texture(const char* imagePath, GLenum texType, GLuint slot, GLenum pixelType)
    : m_FilePath(imagePath), m_Type(texType), m_Unit(slot)
{
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(imagePath, &widthImg, &heightImg, &numColCh, 0);

    if (!bytes)
    {
        std::cerr << "Failed to load texture: " << imagePath << std::endl;
        return;
    }

    GLenum format;
    if (numColCh == 4)
    {
        format = GL_RGBA;
    }
    else if (numColCh == 3)
    {
        format = GL_RGB;
    }
    else if (numColCh == 1)
    {
        format = GL_RED;
    }
    else
    {
        std::cerr << "Unsupported number of color channels: " << numColCh << std::endl;
        stbi_image_free(bytes);
        return;
    }

    glGenTextures(1, &m_RendererID);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(texType, m_RendererID);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(texType, 0, format, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(texType);

    if (glGetError() != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error occurred during texture initialization: " << imagePath << std::endl;
        stbi_image_free(bytes);
        glBindTexture(texType, 0);
        return;
    }

    stbi_image_free(bytes);
    glBindTexture(texType, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture::texUnit(Shader& shader, const char* uniformName, GLuint unit)
{
    shader.Bind();
    GLuint texUni = glGetUniformLocation(shader.ID, uniformName);
    if (texUni == -1)
    {
        std::cout << "ERROR: Uniform \"" << uniformName << "\" not found in shader." << std::endl;
    }
    else
    {
        glUniform1i(texUni, unit);
    }
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + m_Unit);
	glBindTexture(m_Type, m_RendererID);
}

void Texture::Unbind()
{
	glBindTexture(m_Type, 0);
}