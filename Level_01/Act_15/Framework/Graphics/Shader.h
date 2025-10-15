#ifndef GUARD_SHADER_H
#define GUARD_SHADER_H

#include <glad/glad.h>

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
     *
     * @param vertexSource_   버텍스 셰이더 소스 코드.
     * @param fragmentSource_ 프래그먼트 셰이더 소스 코드
     */
    explicit Shader(const char* vertexSource_,
                    const char* fragmentSource_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Shader() noexcept;

    /**
     * @brief 셰이더 프로그램 ID를 반환합니다.
     *
     * @return GLuint 셰이더 프로그램 ID.
     */
    [[nodiscard]]
    constexpr GLuint GetProgramID() const noexcept
    {
        return programID;
    }

    /**
     * @brief 해당 셰이더를 사용합니다.
     */
    inline void Use() const noexcept
    {
        if (programID != 0)
        {
            glUseProgram(programID);
        }
    }
private:
    /**
     * @brief 셰이더 프로그램 ID.
     */
    GLuint programID;
};

#endif // !GUARD_SHADER_H