#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

/**
 * @brief 배경색.
 */
static glm::vec3 gBackgroundColor = { 1.0f, 1.0f, 1.0f };

/**
 * @brief 애니메이션 트리거.
 */
static bool gShouldChangeColor = false;

void OnKeyDown(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (action != GLFW_PRESS) return;

    switch (key)
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
            glfwSetWindowShouldClose(window, true);
            break;
        }
        default:
        {
            break;
        }
    }
}
// ------------------------------------

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Program", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetWindowPos(window, 100, 100);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return EXIT_FAILURE;
    }

    glfwSetKeyCallback(window, OnKeyDown);

    static double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        const double nowTime = glfwGetTime();
        if (gShouldChangeColor && nowTime - lastTime >= 0.05) // 50ms
        {
            gBackgroundColor = { static_cast<float>(std::rand() % 256) / 255.0f,
                                    static_cast<float>(std::rand() % 256) / 255.0f,
                                    static_cast<float>(std::rand() % 256) / 255.0f };
            lastTime = nowTime;
        }

        glClearColor(gBackgroundColor.r, gBackgroundColor.g, gBackgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
