#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"

class VertexArray
{
public:
	GLuint m_RendererID;

	VertexArray();

	void Bind() const;
	void LinkAttrib(VertexBuffer& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset);
	void Unbind() const;
	void Delete();
};

#endif