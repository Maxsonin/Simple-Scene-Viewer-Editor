#include <iostream>

#include "Textures/Texture.h"

Texture::Texture(const char* imagePath, GLuint slot)
    : m_FilePath(imagePath), m_Unit(slot)
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
    glActiveTexture(GL_TEXTURE0 + m_Unit);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, format, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (glGetError() != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error occurred during texture initialization: " << imagePath << std::endl;
        stbi_image_free(bytes);
        glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::TextureFromFile(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Tex failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture::PassTextureToShader(Shader& shader, const char* uniformName)
{
    shader.Bind();
    GLuint textureLocation = glGetUniformLocation(shader.m_RendererID, uniformName);
    if (textureLocation == -1)
        std::cerr << "ERROR: Uniform \"" << uniformName << "\" not found in shader." << std::endl;
    else
        glUniform1i(textureLocation, m_Unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + m_Unit);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}