#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include "Debugging/ConsoleDebug.h"
#include <iostream>

class Shader
{
public:
    GLuint m_RendererID = 0;
    std::string m_ShaderProgramName; // For Debug Purpose
    std::unordered_map<std::string, GLint> m_UniformLocarionCache;

    Shader() = default;
    Shader(const std::string& shaderName, const char* vertexFile, const char* fragmentFile);

    void Bind() const   { glUseProgram(m_RendererID); }
    void Unbind() const { glUseProgram(0); }
    void Delete() { glDeleteProgram(m_RendererID); };

    #pragma region Uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value)
    {
        glUniform1i(GetUniformLocation(name), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value)
    {
        glUniform2fv(GetUniformLocation(name), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y)
    {
        glUniform2f(GetUniformLocation(name), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value)
    {
        glUniform3fv(GetUniformLocation(name), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z)
    {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value)
    {
        glUniform4fv(GetUniformLocation(name), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(GetUniformLocation(name), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat)
    {
        glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat)
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    #pragma endregion

private:
    std::string LoadShaderAsString(const std::string& filename);
    GLuint CompileShader(GLenum type, const std::string& source);

    GLint GetUniformLocation(const std::string& name) 
    {
        if (m_UniformLocarionCache.find(name) != m_UniformLocarionCache.end())
            return m_UniformLocarionCache[name];

        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
            std::cerr << ERROR("ERROR: Uniform " + name + " couldn't be created in " + m_ShaderProgramName + " Shader Program");

        m_UniformLocarionCache[name] = location;
        return location;
    }
};

#endif // !SHADER_H