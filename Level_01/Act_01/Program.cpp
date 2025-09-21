#include <cstdlib>
#include <ctime>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

/**
 * @brief 키와 상호작용했을 때 호출됩니다.
 *
 * @param window_
 * @param key_
 * @param scancode_
 * @param action_
 * @param mods_
 */
void OnKeyDown(GLFWwindow* window_,
               int         key_,
               int         scancode_,
               int         action_,
               int         mods_);

/**
 * @brief 윈도우 타이틀.
 */
static constexpr const char* const WINDOW_TITLE = "OpenGL Color Simulator";

/**
 * @brief 윈도우 너비.
 */
static constexpr int WINDOW_WIDTH  = 800;

/**
 * @brief 윈도우 높이.
 */
static constexpr int WINDOW_HEIGHT = 800;

/**
 * @brief GL 메이저 버전.
 */
constexpr unsigned char CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief GL 마이너 버전.
 */
constexpr unsigned char CONTEXT_MINOR_VERSION = 5;

/**
 * @brief 배경색.
 */
static glm::vec3 gBackgroundColor = { 1.0f, 1.0f, 1.0f };

/**
 * @brief 특정 시간마다 배경색을 변경할지 여부.
 */
static bool gShouldChangeColor = false;

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          WINDOW_TITLE,
                                          nullptr,
                                          nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetWindowPos(window, 100, 100);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return EXIT_FAILURE;
    }

    glfwSetKeyCallback(window, OnKeyDown);

    static double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        {
            const double nowTime = glfwGetTime();
            if (gShouldChangeColor && nowTime - lastTime >= 0.05) // 50ms
            {
                gBackgroundColor = { static_cast<float>(std::rand() % 256) / 255.0f,
                                        static_cast<float>(std::rand() % 256) / 255.0f,
                                        static_cast<float>(std::rand() % 256) / 255.0f };
                lastTime = nowTime;
            }

            glClearColor(gBackgroundColor.r,
                         gBackgroundColor.g,
                         gBackgroundColor.b,
                         1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void OnKeyDown(GLFWwindow* window_,
               int         key_,
               int         scancode_,
               int         action_,
               int         mods_)
{
    if (action_ != GLFW_PRESS)
    {
        return;
    }

    switch (key_)
    {
        case GLFW_KEY_C:
        {
            if (!gShouldChangeColor)
            {
                gBackgroundColor = { 0, 1, 1 };
            }
            break;
        }
        case GLFW_KEY_M:
        {
            if (!gShouldChangeColor)
            {
                gBackgroundColor = { 1, 0, 1 };
            }

            break;
        }
        case GLFW_KEY_Y:
        {
            if (!gShouldChangeColor)
            {
                gBackgroundColor = { 1, 1, 1 };
            }
            break;
        }
        case GLFW_KEY_A:
        {
            if (!gShouldChangeColor)
            {
                gBackgroundColor = { static_cast<float>(std::rand() % 256) / 255.0f,
                                        static_cast<float>(std::rand() % 256) / 255.0f,
                                        static_cast<float>(std::rand() % 256) / 255.0f };
            }

            break;
        }
        case GLFW_KEY_W:
        {
            if (!gShouldChangeColor)
            {
                gBackgroundColor = { 1, 1, 1};
            }
            break;
        }
        case GLFW_KEY_K:
        {
            if (!gShouldChangeColor)
            {
                gBackgroundColor = { 0, 0, 0};
            }
            break;
        }
        case GLFW_KEY_T:
        {
            gShouldChangeColor = true;
            break;
        }
        case GLFW_KEY_S:
        {
            gShouldChangeColor = false;
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