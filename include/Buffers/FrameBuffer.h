#ifndef FRAME_BUFFER
#define FRAME_BUFFER

#include <glad/glad.h>
#include <iostream>
#include <Application/ApplicationSettings.h>
#include <Application/Application.h>

class FrameBuffer
{
public:
	GLuint m_RendererID;
	unsigned int framebufferTexture;
	unsigned int RBO;

	float quadVertices[24] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	FrameBuffer(ApplicationSettings* applicationSettings)
	{
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		int width = applicationSettings->application->getWindowWidth();
		int height = applicationSettings->application->getWindowHeight();

		glGenTextures(1, &framebufferTexture);
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER : Framebuffer is not complete!\n";
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void Bind()   { glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID); }
	void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
};
#endif
