#include "Shader.h"

#include <filesystem>
#include <fstream>
#include <print>
#include <sstream>

Shader::Shader() noexcept
{
    // 따로 유틸 함수 만들기 귀찮아서 람다로 대체
    auto getFileContents = [] (const std::filesystem::path& path) -> std::string
    {
	    std::ifstream     fileStream(path);
        std::stringstream buffer;

        buffer << fileStream.rdbuf();

        return buffer.str();
	};

	constexpr const char* shadersPath = "Resources/Shaders/";

	for (const auto& entry : std::filesystem::directory_iterator(shadersPath))
    {
        const std::filesystem::path filePath = entry.path();
        const std::string extension          = filePath.extension().string();

        GLuint shaderID = 0;
        if (extension == ".vert")
        {
            shaderID = glCreateShader(GL_VERTEX_SHADER);
        }
        else if (extension == ".frag")
        {
            shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        }
        else
        {
            continue;
        }
        
        const std::string sourceCode = getFileContents(filePath);
        const char*       sourceCStr = sourceCode.c_str();
        
        glShaderSource(shaderID, 1, &sourceCStr, nullptr);
        glCompileShader(shaderID);

        GLint compileStatus;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus != GL_TRUE)
        {
            char infoLog[512];
            glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
            std::print("셰이더 컴파일 오류 ({}): {}\n", filePath.string(), infoLog);
            glDeleteShader(shaderID);
            continue;
        }
        
        if (programID == 0)
        {
            programID = glCreateProgram();
        }
        
        glAttachShader(programID, shaderID);
        glDeleteShader(shaderID);
    }

    if (programID != 0)
    {
        glLinkProgram(programID);

        GLint linkStatus;
        glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
            char infoLog[512];
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);
            std::print("셰이더 링크 오류: {}\n", infoLog);

            glDeleteProgram(programID);
            programID = 0;
        }
    }
}

Shader::~Shader() noexcept
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}