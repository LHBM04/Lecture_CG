#include "Shader.h"

#include <spdlog/spdlog.h>

#include "../Core/File.h"

Shader::Shader(const char* const vertexSource_,
               const char* const fragmentSource_) noexcept
{
    GLint success;
    char infoLog[512];

    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource_, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);

        spdlog::critical("Vertex shader compilation failed: {}", infoLog);

        glDeleteShader(vertexShader);

        return;
    }

    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource_, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);

        spdlog::critical("Fragment shader compilation failed: {}", infoLog);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, sizeof(infoLog), nullptr, infoLog);

        spdlog::critical("Shader program linking failed: {}", infoLog);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(programID);

        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() noexcept
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}