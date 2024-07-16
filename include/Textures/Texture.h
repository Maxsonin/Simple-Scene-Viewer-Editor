#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "../Shaders/Shader.h"
#include <stb_image/stb_image.h>
#include <glad/glad.h>

class Texture
{
public:
	GLuint m_RendererID;
	GLenum m_Type;
	GLuint m_Unit;
	std::string m_FilePath;

	Texture() = default;
	Texture(const char* imagePath, GLenum texType, GLuint slot, GLenum pixelType);
	~Texture();

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
};
#endif