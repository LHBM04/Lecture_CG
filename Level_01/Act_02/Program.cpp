#include <array>
#include <ctime>
#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

/**
 * @struct Rect
 *
 * @brief 사각형을 정의합니다.
 */
struct Rect
{
    /**
     * @brief 죄측 최소 위치.
     */
    glm::vec2 min;

    /**
     * @brief 우측 최대 위치.
     */
    glm::vec2 max;

    /**
     * @brief 색상.
     */
    glm::vec3 color;
};

/**
 * @brief 키와 상호작용할 때 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param key_ 눌린 키.
 * @param scancode_ 스캔 코드.
 * @param action_ 키 액션.
 * @param mods_ 수정자 키 상태.
 */
static void OnKeyInteracted(GLFWwindow* window_,
                            int         key_,
                            int         scancode_,
                            int         action_,
                            int         mods_);

/**
 * @brief 버튼과 상호작용할 떄 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param button_ 클릭된 버튼.
 * @param action_ 버튼 액션.
 * @param mods_ 버튼 상태.
 */
static void OnButtonInteracted(GLFWwindow* window_,
                               int         button_,
                               int         action_,
                               int         mods_);

/**
 * @brief 윈도우 타이틀.
 */
static constexpr const char* const WINDOW_TITLE = "Level 01 - Act 02";

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
 * @brief 뒤에 올 사각형.
 */
static std::array<Rect, 4> backs
{
    Rect{ {  0.0f,  0.0f }, { 1.0f, 1.0f }, { 0.25f, 0.0f,  0.0f  } },
    Rect{ { -1.0f,  0.0f }, { 0.0f, 1.0f }, { 0.0f,  0.25f, 0.0f  } },
    Rect{ { -1.0f, -1.0f }, { 0.0f, 0.0f }, { 0.0f,  0.0f,  0.25f } },
    Rect{ {  0.0f, -1.0f }, { 1.0f, 0.0f }, { 0.25f, 0.25f, 0.0f  } }
};

/**
 * @brief 앞에 올 사각형들.
 */
static std::array<Rect, 4> fronts
{
    Rect{ {  0.0f,  0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
    Rect{ { -1.0f,  0.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
    Rect{ { -1.0f, -1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
    Rect{ {  0.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } }
};

/**
 * @brief 크기 조절 스케일러.
 */
static constexpr float SCALE_MULTIPLIER = 0.05f;

/**
 * @brief 최소 크기.
 */
static constexpr float MIN_SIZE = 0.1f;

/**
 * @brief 최대 크기.
 */
static constexpr float MAX_SIZE = 1.0f;

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
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

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        {
            glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (const auto& [min, max, color] : backs)
            {
                glColor3f(color.r, color.g, color.b);
                glRectf(min.x, min.y, max.x, max.y);
            }

            for (const auto& [min, max, color] : fronts)
            {
                glColor3f(color.r, color.g, color.b);
                glRectf(min.x, min.y, max.x, max.y);
            }
        }
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void OnKeyInteracted(GLFWwindow* window_,
                     int         key_,
                     int         scancode_,
                     int         action_,
                     int         mods_)
{
    if (key_ == GLFW_KEY_Q && action_ == GLFW_RELEASE)
    {
        glfwSetWindowShouldClose(window_, true);
    }
}

void OnButtonInteracted(GLFWwindow* window_,
                        int         button_,
                        int         action_,
                        int         mods_)
{
    if (action_ != GLFW_PRESS)
    {
        return;
    }

    int width, height;
    glfwGetWindowSize(window_, &width, &height);

    double mouseX, mouseY;
    glfwGetCursorPos(window_, &mouseX, &mouseY);

    float ndcX = static_cast<float>( (mouseX / width)  * 2.0 - 1.0 );
    float ndcY = static_cast<float>( 1.0 - (mouseY / height) * 2.0 );

    bool isInteracted = false;

    for (std::size_t index = 0; index < 4; ++index)
    {
        Rect& front = fronts[index];

        if ((ndcX >= front.min.x && ndcX <= front.max.x) &&
            (ndcY >= front.min.y && ndcY <= front.max.y))
        {
            isInteracted = true;

            if (button_ == GLFW_MOUSE_BUTTON_LEFT)
            {
                front.color = { static_cast<float>(std::rand() % 256) / 255.0f,
                                   static_cast<float>(std::rand() % 256) / 255.0f,
                                   static_cast<float>(std::rand() % 256) / 255.0f };

                break;
            }
            if (button_ == GLFW_MOUSE_BUTTON_RIGHT)
            {
                if ((front.max.x - front.min.x > MIN_SIZE * 2.0f) &&
                    (front.max.y - front.min.y > MIN_SIZE * 2.0f))
                {
                    front.min += glm::vec2(SCALE_MULTIPLIER);
                    front.max -= glm::vec2(SCALE_MULTIPLIER);
                }

                break;
            }
        }

        if (!isInteracted)
        {
            Rect& back  = backs[index];

            if ((ndcX >= back.min.x && ndcX <= back.max.x) &&
                (ndcY >= back.min.y && ndcY <= back.max.y))
            {
                if (button_ == GLFW_MOUSE_BUTTON_LEFT)
                {
                    back.color = { static_cast<float>(std::rand() % 256) / 255.0f,
                                      static_cast<float>(std::rand() % 256) / 255.0f,
                                      static_cast<float>(std::rand() % 256) / 255.0f };

                    break;
                }
                if (button_ == GLFW_MOUSE_BUTTON_RIGHT)
                {
                    if ((front.max.x - front.min.x < MAX_SIZE) &&
                        (front.max.y - front.min.y < MAX_SIZE))
                    {
                        front.min -= glm::vec2(SCALE_MULTIPLIER);
                        front.max += glm::vec2(SCALE_MULTIPLIER);
                    }

                    break;
                }
            }
        }
    }
}
