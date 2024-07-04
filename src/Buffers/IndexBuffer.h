#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>

class IndexBuffer
{
public:
	GLuint m_RendererID;
	unsigned int m_Count;

	IndexBuffer() = default;
	IndexBuffer(const GLuint* indices, GLsizeiptr size);

	void Bind() const;
	void Unbind() const;
	void Delete();

	inline GLuint GetCount() const { return m_Count; }
};

#endif