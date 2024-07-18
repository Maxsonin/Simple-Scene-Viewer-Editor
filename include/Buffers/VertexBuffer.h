#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class VertexBuffer
{
public:
	GLuint m_RendererID;

	VertexBuffer() = default;
	VertexBuffer(const GLfloat* vertices, GLsizeiptr size);

	void Bind() const;
	void Unbind() const;
	void Delete();
};

#endif