#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>
#include <iostream>

class IndexBuffer
{
public:
	GLuint m_RendererID;
	unsigned int m_Count;

	IndexBuffer() = default;
	IndexBuffer(const GLuint* indices, GLsizeiptr size)
		: m_Count(size / sizeof(GLuint))
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Delete()
	{
		glDeleteBuffers(1, &m_RendererID);;
	}

	inline GLuint GetCount() const { return m_Count; }

	void Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};

#endif