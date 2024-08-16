#include "Shaders/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& shaderName, const char* vertexFile, const char* fragmentFile)
    :m_ShaderProgramName(shaderName)
{
    m_RendererID = glCreateProgram();
    if (m_RendererID == 0) 
    {
        std::cerr << FATAL_ERROR("ERROR: Unable to create Shader Program: Name - " + shaderName + ", ID - " + std::to_string(m_RendererID));
        exit(EXIT_FAILURE);
    }

    std::string vertexShaderSource = LoadShaderAsString(vertexFile);
    std::string fragmentShaderSource = LoadShaderAsString(fragmentFile);

    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);

    glLinkProgram(m_RendererID);
    GLint linkStatus;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) 
    {
        GLint logLength;
        glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(logLength, ' ');
        glGetProgramInfoLog(m_RendererID, logLength, nullptr, log.data());

        std::cerr << FATAL_ERROR("ERROR: Failed to link Shader Program: Name - " + shaderName + "\nLog: " + log);
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string Shader::LoadShaderAsString(const std::string& filename)
{
    std::ifstream fileStream(filename, std::ios::in | std::ios::binary);
    if (!fileStream.is_open()) { throw std::runtime_error(FATAL_ERROR("ERROR: Failed to open file: " + filename)); }

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
            std::cerr << FATAL_ERROR("ERROR: Failed to COMPILE Shader: Shader Program - " + m_ShaderProgramName + ", Shader Type - " + std::to_string(type) + "\nLog: " + log);
            exit(EXIT_FAILURE);
        }
    }

    return shaderObj;
}

