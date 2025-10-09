#ifndef GUARD_PROGRAM_H
#define GUARD_PROGRAM_H

#include <filesystem>
#include <string>

class GLFWwindow;
class Shader;

class Program
{
public:
    /**
     * @brief 생성자.
     */
    explicit Program() noexcept;

    /**
     * @brief 소멸자.
     */
    ~Program() noexcept;

    /**
     * @brief 프로그램을 실행합니다.
     */
    void Run() noexcept;
private:
    /**
     * @brief 키와 상호작용할 때 호출됩니다.
     *
     * @param window_   윈도우.
     * @param key_      눌린 키.
     * @param scancode_ 스캔 코드.
     * @param action_   키 액션.
     * @param mods_     수정자 키 상태.
     */
    static void OnKeyInteracted(GLFWwindow* const window_,
                                const int         key_,
                                const int         scancode_,
                                const int         action_,
                                const int         mods_) noexcept;

    /**
     * @brief 버튼과 상호작용할 떄 호출됩니다.
     *
     * @param window_ 윈도우.
     * @param button_ 클릭된 버튼.
     * @param action_ 버튼 액션.
     * @param mods_   버튼 상태.
     */
    static void OnButtonInteracted(GLFWwindow* const window_,
                                   const int         button_,
                                   const int         action_,
                                   const int         mods_) noexcept;

    /**
     * @brief 커서가 움직일 때 호출됩니다.
     *
     * @param window_ 윈도우.
     * @param x_      마우스 X 좌표.
     * @param y_      마우스 Y 좌표.
     */
    static void OnCursorMoved(GLFWwindow* const window_,
                              const double      x_,
                              const double      y_) noexcept;

    /**
     * @brief GLFW 윈도우 핸들.
     */
    GLFWwindow* window;

    /**
     * @brief 셰이더.
     */
    Shader* shader;
};

/**
 * @brief 지정한 경로의 파일을 읽어옵니다.
 *
 * @param path_ 지정할 파일 경로.
 *
 * @return std::string 파일 텍스트.
 */
static std::string GetFile(const std::filesystem::path& path_);

#endif // !GUARD_PROGRAM_H