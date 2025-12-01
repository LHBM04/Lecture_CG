#include "Shader.h"

#include <filesystem>
#include <fstream>
#include <print>
#include <sstream>

Shader::Shader(const char* vertex_, const char* fragment_) noexcept
{
    // 1. 파일 읽기 람다
    auto getFileContents = [] (const std::filesystem::path& path) -> std::string
    {
        std::ifstream     fileStream(path);
        if (!fileStream.is_open())
        {
            std::println("셰이더 파일을 열 수 없습니다: {}", path.string());
            return "";
        }
        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        return buffer.str();
    };

    // 2. 프로그램 생성
    programID = glCreateProgram();

    // 3. 버텍스 셰이더 컴파일
    unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
    {
        std::string vertexCode = getFileContents(vertex_);
        const char* vShaderCode = vertexCode.c_str();
        glShaderSource(vertexID, 1, &vShaderCode, nullptr);
        glCompileShader(vertexID);

        // 컴파일 에러 체크 (필수)
        int success;
        char infoLog[512];
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexID, 512, nullptr, infoLog);
            std::println("VERTEX SHADER 컴파일 오류 ({}):\n{}", vertex_, infoLog);
        }
    }

    // 4. 프래그먼트 셰이더 컴파일
    unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    {
        std::string fragmentCode = getFileContents(fragment_);
        const char* fShaderCode = fragmentCode.c_str();
        glShaderSource(fragmentID, 1, &fShaderCode, nullptr);
        glCompileShader(fragmentID);

        // 컴파일 에러 체크
        int success;
        char infoLog[512];
        glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentID, 512, nullptr, infoLog);
            std::println("FRAGMENT SHADER 컴파일 오류 ({}):\n{}", fragment_, infoLog);
        }
    }

    // 5. 셰이더 프로그램 링크 (이 부분이 빠져 있었습니다!)
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
    glLinkProgram(programID);

    // 링크 에러 체크
    int success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::println("SHADER PROGRAM 링크 오류:\n{}", infoLog);
    }

    // 6. 셰이더 객체 삭제 (링크 후에는 필요 없음)
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}
Shader::~Shader() noexcept
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}