#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

/**
 * @brief 키와 상호작용할 때 호출됩니다.
 *
 * @param window_   윈도우.
 * @param key_      눌린 키.
 * @param scancode_ 스캔 코드.
 * @param action_   키 액션.
 * @param mods_     수정자 키 상태.
 */
static void OnKeyInteracted(GLFWwindow* window_,
                            int         key_,
                            int         scancode_,
                            int         action_,
                            int         mods_) noexcept;

/**
 * @brief 버튼과 상호작용할 떄 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param button_ 클릭된 버튼.
 * @param action_ 버튼 액션.
 * @param mods_   버튼 상태.
 */
static void OnButtonInteracted(GLFWwindow* window_,
                               int         button_,
                               int         action_,
                               int         mods_) noexcept;

/**
 * @brief 커서가 움직일 때 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param x_      마우스 X 좌표.
 * @param y_      마우스 Y 좌표.
 */
static void OnCursorMoved(GLFWwindow* window_,
                          double      x_,
                          double      y_) noexcept;

/**
 * @brief 셰이더 소스 코드.
 */
static std::string GetFile(const std::filesystem::path& path_);

/**
 * @brief GLFW 윈도우.
 */
static GLFWwindow* window = nullptr;

/**
 * @brief 애플리케이션 너비.
 */
static constexpr unsigned int WINDOW_WIDTH  = 1200;

/**
 * @brief 애플리케이션 높이.
 */
static constexpr unsigned int WINDOW_HEIGHT = 600;

/**
 * @brief 애플리케이션 타이틀.
 */
static constexpr const char* const WINDOW_TITLE = "Level 01 - Act 07";

/**
 * @brief GL 메이저 버전.
 */
static constexpr unsigned char CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief GL 마이너 버전.
 */
static constexpr unsigned char CONTEXT_MINOR_VERSION = 5;

/**
 * @brief 커서의 위치.
 */
static glm::vec2 cursorPosition;

/**
 * @brief 선으로 그릴지 여부.
 */
static bool shouldDrawWithLines = false;

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              WINDOW_TITLE,
                              nullptr,
                              nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(window, 100, 100);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetKeyCallback(window, OnKeyInteracted);
    glfwSetMouseButtonCallback(window, OnButtonInteracted);
    glfwSetCursorPosCallback(window, OnCursorMoved);
}

std::string GetFile(const std::filesystem::path& path_)
{
    std::ifstream file(path_, std::ios::in | std::ios::binary);

    if (!file)
    {
        throw std::runtime_error("Could not open file");
    }

    return { std::istreambuf_iterator<char>(file),
             std::istreambuf_iterator<char>() };
}

void OnKeyInteracted(GLFWwindow* const window_,
                     const int         key_,
                     const int         scancode_,
                     const int         action_,
                     const int         mods_) noexcept
{
    if (action_ != GLFW_PRESS)
    {
        return;
    }

    switch (key_)
    {
        case GLFW_KEY_A:
        {
            shouldDrawWithLines = false;
            break;
        }
        case GLFW_KEY_B:
        {
            shouldDrawWithLines = true;
            break;
        }
        case GLFW_KEY_Q:
        {
            glfwSetWindowShouldClose(window_, true);
            break;
        }
        default:
        {
            break;
        }
    }
}

void OnButtonInteracted(GLFWwindow* const window_,
                        const int         button_,
                        const int         action_,
                        const int         mods_) noexcept
{
    if (action_ != GLFW_PRESS)
    {
        return;
    }

    if (button_ == GLFW_MOUSE_BUTTON_LEFT)
    {
    }
    else if (button_ == GLFW_MOUSE_BUTTON_RIGHT)
    {
    }
}

void OnCursorMoved(GLFWwindow* const window_,
                   const double      x_,
                   const double      y_) noexcept
{
    cursorPosition.x = static_cast<float>(x_);
    cursorPosition.y = static_cast<float>(WINDOW_WIDTH - y_);
}