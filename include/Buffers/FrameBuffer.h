#ifndef FRAME_BUFFER
#define FRAME_BUFFER

#include <glad/glad.h>
#include <iostream>

#include "Application/Application.h"
#include "Debugging/Debugging.h"

class FrameBuffer
{
public:
    GLuint m_RendererID;
    unsigned int framebufferTexture;
    unsigned int RBO;
    unsigned int samples;

    float quadVertices[24] =
    {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    FrameBuffer(int width, int height, int sampleCount = 0)
        : samples(sampleCount)
    {
        Initialize(width, height, samples);
    }

    ~FrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &framebufferTexture);
        glDeleteRenderbuffers(1, &RBO);
    }

    void Reset(int width, int height, int newSampleCount = 0)
    {
        // Delete existing resources
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &framebufferTexture);
        glDeleteRenderbuffers(1, &RBO);

        // Recreate the FBO with the new sample count
        m_RendererID = 0;
        framebufferTexture = 0;
        RBO = 0;
        samples = newSampleCount;

        Initialize(width, height, samples);
    }

    void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID); }
    void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

private:
    void Initialize(int width, int height, int sampleCount)
    {
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        CreateAttachments(width, height, sampleCount);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER : Framebuffer is not complete!\n";
            exit(1);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void CreateAttachments(int width, int height, int sampleCount)
    {
        if (sampleCount > 0)
        {
            // Multisampled framebuffer
            glGenTextures(1, &framebufferTexture);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, sampleCount, GL_RGB, width, height, GL_TRUE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);
        }
        else
        {
            // Non-multisampled framebuffer
            glGenTextures(1, &framebufferTexture);
            glBindTexture(GL_TEXTURE_2D, framebufferTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);
        }

        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        if (sampleCount > 0)
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleCount, GL_DEPTH24_STENCIL8, width, height);
        else
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    }
};

#endif
