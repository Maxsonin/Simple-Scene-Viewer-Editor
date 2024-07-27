#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include "Shaders/Shader.h"

class Texture
{
public:
	GLuint m_RendererID = 0;
	GLuint m_Unit = 0;

	std::string m_TypeOfUse = "none";

	std::string m_FilePath;

	Texture() = default;
	Texture(const char* imagePath, GLuint slot);
	~Texture();

	void PassTextureToShader(Shader& shader, const char* uniform);
	static unsigned int TextureFromFile(const char* imagePath);
	void Bind();
	void Unbind();
};
#endif