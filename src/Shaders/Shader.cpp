#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    ID = glCreateProgram();
    if (ID == 0) { throw std::runtime_error("An error occurs while creating the program object"); }

    std::string vertexShaderSource = LoadShaderAsString(vertexFile);
    std::string fragmentShaderSource = LoadShaderAsString(fragmentFile);

    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);
    GLint linkStatus;
    glGetProgramiv(ID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) 
    {
        GLint logLength;
        glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(logLength, ' ');
        glGetProgramInfoLog(ID, logLength, nullptr, log.data());
        throw std::runtime_error("Failed to link program: \n" + log);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string Shader::LoadShaderAsString(const std::string& filename)
{
    std::ifstream fileStream(filename, std::ios::in | std::ios::binary);
    if (!fileStream.is_open()) { throw std::runtime_error("Failed to open file: " + filename); }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

GLuint Shader::CompileShader(GLenum type, const std::string& source)
{
    GLuint shaderObj = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderObj, 1, &src, nullptr);
    glCompileShader(shaderObj);

    GLint compileStatus;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) 
    {
        GLint logLength;
        glGetShaderiv(shaderObj, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(logLength, ' ');

        if (logLength > 0)
        {
            glGetShaderInfoLog(shaderObj, logLength, nullptr, &log[0]);
            std::cerr << "Failed to compile shader : \n" + log + "\n";
            exit(1);
        }
    }

    return shaderObj;
}

