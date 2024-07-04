#include "Texture.h"
#include <iostream>

Texture::Texture(const char* imagePath, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
    :m_FilePath(imagePath), m_Type(texType), m_Unit(slot)
{
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(imagePath, &widthImg, &heightImg, &numColCh, 0);

    if (!bytes)
    {
        std::cerr << "Failed to load texture: " << imagePath << std::endl;
        return;
    }

    glGenTextures(1, &m_RendererID);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(texType, m_RendererID);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
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
	GLuint texUni = glGetUniformLocation(shader.ID, uniformName);
	shader.Bind();
	glUniform1i(texUni, unit);
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