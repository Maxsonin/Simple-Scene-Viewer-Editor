#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <glad/glad.h>

#include "Buffers/VertexBuffer.h"

class VertexArray
{
public:
	GLuint m_RendererID;

	VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	void Delete()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void LinkAttrib(VertexBuffer& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset)
	{
		VBO.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		VBO.Unbind();
	}

	void Unbind() const
	{
		glBindVertexArray(0);
	}
};

#endif