#pragma once

#include <glad/glad.h>
#include <iostream>
#include <imgui.h>

inline GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__) 

inline void checkOpenGLError_(const char* stmt, const char* fname, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error " << err << ", at " << stmt << " " << fname << ":" << line << std::endl;
        exit(EXIT_FAILURE);
    }
}

#define GL_CHECK(stmt) do { \
        stmt; \
        checkOpenGLError_(#stmt, __FILE__, __LINE__); \
    } while (0)

//ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);   // Red color
//ImVec4 greenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green color
//ImVec4 blueColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);  // Blue color

//ImGui::TextColored(redColor, "This text is red.");
//ImGui::TextColored(greenColor, "This text is green.");
//ImGui::TextColored(blueColor, "This text is blue.");