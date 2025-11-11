#include <ctime>
#include <format>
#include <iostream>
#include <vector>

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
    static constexpr float SCALE_MULTIPLIER = 25.0f;

    /**
     * @brief 최소 좌표.
     */
    glm::vec2 min;

    /**
     * @brief 최대 좌표.
     */
    glm::vec2 max;

    /**
     * @brief 색.
     */
    glm::vec3 color;
};

/**
 * @brief 윈도우가 그려질 때 호출됩니다.
 */
void OnRender();

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
 * @brief 커서의 위치가 변경되었을 때 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
static void OnCursorMoved(GLFWwindow* window_,
                          double      x_,
                          double      y_);

/**
 * @brief 윈도우 타이틀.
 */
static constexpr const char* const WINDOW_TITLE = "Level 01 - Act 03";

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
 * @brief 사각형들.
 */
static std::vector<Rect> rects;

/**
 * @brief 최대 사각형 개수.
 */
static constexpr std::size_t MAX_RECT_COUNTS = 30;

/**
 * @brief 'a' 명령어로 만들 수 있는 사각형의 최대 개수.
 */
static constexpr std::size_t CREATE_RECT_COUNTS = 10;

/**
 * @brief 현재까지 'a' 명령어로 생성된 사각형 개수.
 */
static std::size_t currentCreateCounts = 0;

/**
 * @brief 현재 선택 중인 사각형.
 */
static Rect* currentRect = nullptr;

int main(int, char**)
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
    glfwSetCursorPosCallback(window, OnCursorMoved);

    static double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (const auto& [min, max, color] : rects)
            {
                glColor3f(color.r, color.g, color.b);
                glRectf(min.x, min.y, max.x, max.y);
            }
        }
        glfwSwapBuffers(window);
    }

    return EXIT_SUCCESS;
}

void OnKeyInteracted(GLFWwindow* window_,
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
        case 'a': [[fallthrough]];
        case 'A':
        {
            if (rects.size() >= MAX_RECT_COUNTS ||
                currentCreateCounts + 1 > CREATE_RECT_COUNTS)
            {
                return;
            }

            const glm::vec2 currentPosition   = { static_cast<float>(std::rand() % 800),
                                      static_cast<float>(std::rand() % 600) };
            const glm::vec2 min   = { currentPosition.x - Rect::SCALE_MULTIPLIER / 2.0f,
                                      currentPosition.y - Rect::SCALE_MULTIPLIER / 2.0f };
            const glm::vec2 max   = { currentPosition.x + Rect::SCALE_MULTIPLIER / 2.0f,
                                      currentPosition.y + Rect::SCALE_MULTIPLIER / 2.0f };
            const glm::vec3 color = { static_cast<float>(std::rand() % 256) / 255.0f,
                                      static_cast<float>(std::rand() % 256) / 255.0f,
                                      static_cast<float>(std::rand() % 256) / 255.0f };

            rects.push_back({ min, max, color });
            currentCreateCounts++;

            std::cout << std::format("[Info] new rectangle created at ({:.1f}, {:.1f}) ", currentPosition.x, currentPosition.y);
            std::cout << std::format("Total Counts: {:d}\n", currentCreateCounts);

            break;
        }
        case 'q': [[fallthrough]];
        case 'Q':
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

void OnButtonInteracted(GLFWwindow* window_,
                        int         button_,
                        int         action_,
                        int         mods_)
{
    int width;
    int height;
    glfwGetWindowSize(window_, &width, &height);

    double mouseX;
    double mouseY;
    glfwGetCursorPos(window_, &mouseX, &mouseY);

    float fixedY = static_cast<float>(height - mouseY);

    if (button_ == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action_ == GLFW_PRESS)
        {
            for (auto& rect : rects)
            {
                if ((mouseX >= rect.min.x && mouseX <= rect.max.x) &&
                    (fixedY >= rect.min.y && fixedY <= rect.max.y))
                {
                    currentRect = &rect;
                    std::cout << "[Info] The rectangle selected!\n";
                    break;
                }
            }
        }
        else if (action_ == GLFW_RELEASE)
        {
            auto toErase = rects.end();

            for (auto iter = rects.begin(); iter != rects.end(); ++iter)
            {
                if (&(*iter) == currentRect)
                {
                    continue;
                }

                if (!((currentRect->max.x < iter->min.x || currentRect->min.x > iter->max.x) ||
                      (currentRect->max.y < iter->min.y || currentRect->min.y > iter->max.y)))
                {
                    const glm::vec2 newMin   = { std::min(currentRect->min.x, iter->min.x),
                                                 std::min(currentRect->min.y, iter->min.y) };
                    const glm::vec2 newMax   = { std::max(currentRect->max.x, iter->max.x),
                                                 std::max(currentRect->max.y, iter->max.y) };
                    const glm::vec3 newColor = { static_cast<float>(std::rand() % 256) / 255.0f,
                                                 static_cast<float>(std::rand() % 256) / 255.0f,
                                                 static_cast<float>(std::rand() % 256) / 255.0f };

                    currentRect->min   = newMin;
                    currentRect->max   = newMax;
                    currentRect->color = newColor;

                    toErase = iter;
                    break;
                }
            }

            if (toErase != rects.end())
            {
                rects.erase(toErase);
            }

            currentRect = nullptr;
        }
    }
    else if (button_ == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action_ == GLFW_PRESS)
        {
            if (rects.size() + 2 >= MAX_RECT_COUNTS)
            {
                std::cerr << "[Oops!] If you divide rectangle, overflow!\n";
                return;
            }

            auto toErase = rects.end();

            for (auto iter = rects.begin(); iter != rects.end(); ++iter)
            {
                if ((mouseX >= iter->min.x && mouseX <= iter->max.x) &&
                    (fixedY >= iter->min.y && fixedY <= iter->max.y))
                {
                    toErase = iter;
                    break;
                }
            }

            if (toErase != rects.end())
            {
                const Rect original = *toErase;

                const float width  = original.max.x - original.min.x;
                const float height = original.max.y - original.min.y;

                if (width > height)
                {
                    const float midX = original.min.x + width / 2.0f;

                    rects.push_back({
                        {original.min.x, original.min.y},
                        {midX, original.max.y},
                        {static_cast<float>(std::rand() % 256) / 255.0f,
                         static_cast<float>(std::rand() % 256) / 255.0f,
                         static_cast<float>(std::rand() % 256) / 255.0f}
                    });

                    rects.push_back({
                        {midX, original.min.y},
                        {original.max.x, original.max.y},
                        {static_cast<float>(std::rand() % 256) / 255.0f,
                         static_cast<float>(std::rand() % 256) / 255.0f,
                         static_cast<float>(std::rand() % 256) / 255.0f}
                    });
                }
                else
                {
                    const float midY = original.min.y + height / 2.0f;

                    rects.push_back({
                        {original.min.x, original.min.y},
                        {original.max.x, midY},
                        {static_cast<float>(std::rand() % 256) / 255.0f,
                         static_cast<float>(std::rand() % 256) / 255.0f,
                         static_cast<float>(std::rand() % 256) / 255.0f}
                    });

                    rects.push_back({
                        {original.min.x, midY},
                        {original.max.x, original.max.y},
                        {static_cast<float>(std::rand() % 256) / 255.0f,
                         static_cast<float>(std::rand() % 256) / 255.0f,
                         static_cast<float>(std::rand() % 256) / 255.0f}
                    });
                }

                std::cout << "[Info] Rectangle divided into two new rectangles!\n";
                rects.erase(toErase);
            }
        }
    }
}

void OnCursorMoved(GLFWwindow* window_,
                   double      x_,
                   double      y_)
{
    if (currentRect == nullptr)
    {
        return;
    }

    int width  = 0;
    int height = 0;
    glfwGetWindowSize(window_, &width, &height);

    float fixedX = static_cast<float>(x_);
    float fixedY = static_cast<float>(height - y_);

    const float rectWidth  = currentRect->max.x - currentRect->min.x;
    const float rectHeight = currentRect->max.y - currentRect->min.y;

    currentRect->min = { fixedX - rectWidth / 2.0f, fixedY - rectHeight / 2.0f };
    currentRect->max = { fixedX + rectWidth / 2.0f, fixedY + rectHeight / 2.0f };

    std::cout << std::format("[Info] Current Position: ({:.1f}, {:.1f})\n", fixedX, fixedY);
}