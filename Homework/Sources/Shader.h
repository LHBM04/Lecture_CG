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
     * @brief 셰이더를 초기화합니다.
     */
    static bool Initialize() noexcept;

    /**
	 * @brief 셰이더를 해제합니다.
     */
    static void Release() noexcept;

    /**
     * @brief 셰이더 프로그램 ID를 반환합니다.
     *
     * @return unsigned int 셰이더 프로그램 ID.
     */
    [[nodiscard]]
    static constexpr unsigned int GetProgramID() noexcept;

    /**
     * @brief 정수형 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    static inline void SetUniformInt(const char* const name_,
                                     const int         value_) noexcept;

    /**
     * @brief 실수형 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    static inline void SetUniformFloat(const char* const name_,
                                       const float       value_) noexcept;

    /**
     * @brief 2차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    static inline void SetUniformVector2(const char* const name_,
                                         const glm::vec2& value_) noexcept;

    /**
     * @brief 3차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    static inline void SetUniformVector3(const char* const name_,
                                         const glm::vec3& value_) noexcept;

    /**
     * @brief 4차원 벡터 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    static inline void SetUniformVector4(const char* const name_,
                                         const glm::vec4& value_) noexcept;

    /**
     * @brief 4x4 행렬 유니폼 변수를 설정합니다.
     *
     * @param name_  유니폼 변수 이름.
     * @param value_ 설정할 값.
     */
    static inline void SetUniformMatrix4x4(const char* const name_,
                                           const glm::mat4& value_) noexcept;

private:
#pragma region Deleted Functions
    Shader() = delete;
    ~Shader() = delete;

    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;

    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;
    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;
#pragma endregion

	/**
	 * @brief 셰이더를 컴파일합니다.
     * 
	 * @param type_   컴파일 타입
	 * @param source_ 컴파일할 세이더 소스
     * 
	 * @return GLuint 셰이더
	 */
	[[nodiscard]]
    static GLuint CompileShader(GLenum           type_,
		                        std::string_view source_) noexcept;

    /**
     * @brief 셰이더가 사용 중인지 여부.
	 */
    static bool isUsing;

    /**
     * @brief 셰이더 프로그램 ID.
     */
    static GLuint programID;
};

constexpr unsigned int Shader::GetProgramID() noexcept
{
    return programID;
}

inline void Shader::SetUniformInt(const char* const name_, const int value_) noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniform1i(location, value_);
}

inline void Shader::SetUniformFloat(const char* const name_, const float value_) noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniform1f(location, value_);
}

inline void Shader::SetUniformVector2(const char* const name_, const glm::vec2& value_) noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniform2fv(location, 1, glm::value_ptr(value_));
}

inline void Shader::SetUniformVector3(const char* const name_, const glm::vec3& value_) noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniform3fv(location, 1, glm::value_ptr(value_));
}

inline void Shader::SetUniformVector4(const char* const name_, const glm::vec4& value_) noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniform4fv(location, 1, glm::value_ptr(value_));
}

inline void Shader::SetUniformMatrix4x4(const char* name_, const glm::mat4& value_) noexcept
{
    const GLint location = glGetUniformLocation(programID, name_);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value_));
}