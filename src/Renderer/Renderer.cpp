#include "Renderer.h"

void Renderer::Clear() const
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawWithEBO(const VertexArray& vertArr, const IndexBuffer& indexBuff, const Shader& shader) const
{
	shader.Bind();
	vertArr.Bind(); indexBuff.Bind();
	glDrawElements(GL_TRIANGLES, indexBuff.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawWithoutEBO(const Shader& shader, GLenum type, GLint first, GLsizei count) const
{
	shader.Bind();
	glDrawArrays(type, first, count);
}
