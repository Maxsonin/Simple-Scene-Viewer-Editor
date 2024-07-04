#ifndef RENDERER_H
#define RENDERER_H

#include "../Buffers/VertexArray.h"
#include "../Buffers/IndexBuffer.h"
#include "../Shaders/Shader.h"

class Renderer
{
public:
	void Clear() const;
	void DrawWithEBO(const VertexArray& vertArr, const IndexBuffer& indexBuff, const Shader& shader) const;
	void DrawWithoutEBO(const Shader& shader, GLenum type, GLint first, GLsizei count) const;
};

#endif // !RENDERER_H