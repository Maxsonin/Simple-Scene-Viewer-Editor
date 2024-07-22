#ifndef RENDERER_H
#define RENDERER_H

#include "Buffers/VertexArray.h"
#include "Buffers/IndexBuffer.h"
#include "Shaders/Shader.h"

class Renderer
{
public:
	static void ClearFrame(glm::vec3 color)
	{
		glClearColor(color.r, color.g, color.b, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static void DrawWithTriangles(const VertexArray& vertArr, const IndexBuffer& indexBuff, const Shader& shader)
	{
		shader.Bind();
		vertArr.Bind(); indexBuff.Bind();
		glDrawElements(GL_TRIANGLES, indexBuff.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	static void DrawLines(const Shader& shader, GLint first, GLsizei count)
	{
		shader.Bind();
		glDrawArrays(GL_LINES, first, count);
	}
};

#endif // !RENDERER_H