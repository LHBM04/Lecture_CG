#pragma once

#include "PCH.h"

/**
 * @class Shader
 *
 * @brief 셰이더를 정의합니다.
 */
class Shader final
{
public:
    /**
     * @brief 생성자.
     */
    explicit Shader() noexcept;

    /**
     * @brief 소멸자.
     */
    ~Shader() noexcept;

    /**
     * @brief 셰이더를 사용합니다.
     */
    inline void Use() const noexcept;

    /**
     * @brief 셰이더 프로그램 ID를 반환합니다.
     *
     * @return unsigned int 셰이더 프로그램 ID.
     */
    [[nodiscard]]
    constexpr unsigned int GetProgramID() const noexcept;

    /**
     * @brief 정수형 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformInt(const char* const name_,
                              const int         value_) const noexcept;

    /**
     * @brief 실수형 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformFloat(const char* const name_,
                                const float       value_) const noexcept;

    /**
     * @brief 2차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformVector2(const char* const name_,
                                  const glm::vec2& value_) const noexcept;

    /**
     * @brief 3차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformVector3(const char* const name_,
                                  const glm::vec3& value_) const noexcept;

    /**
     * @brief 4차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformVector4(const char* const name_,
                                  const glm::vec4& value_) const noexcept;

    /**
     * @brief 4x4 행렬 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    inline void SetUniformMatrix4x4(const char* const name_,
                                    const glm::mat4& value_) const noexcept;

private:
    /**
     * @brief 셰이더 프로그램 ID.
     */
    GLuint programID = 0;
};

inline void Shader::Use() const noexcept
{
    glUseProgram(programID);
}

constexpr unsigned int Shader::GetProgramID() const noexcept
{
    return programID;
}

inline void Shader::SetUniformInt(const char* const name_, const int value_) const noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniform1i(location, value_);
}

inline void Shader::SetUniformFloat(const char* const name_, const float value_) const noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniform1f(location, value_);
}

inline void Shader::SetUniformVector2(const char* const name_, const glm::vec2& value_) const noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniform2fv(location, 1, glm::value_ptr(value_));
}

inline void Shader::SetUniformVector3(const char* const name_, const glm::vec3& value_) const noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniform3fv(location, 1, glm::value_ptr(value_));
}

inline void Shader::SetUniformVector4(const char* const name_, const glm::vec4& value_) const noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniform4fv(location, 1, glm::value_ptr(value_));
}

inline void Shader::SetUniformMatrix4x4(const char* name_, const glm::mat4& value_) const noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value_));
}