#include <algorithm>
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
    /**
     * @brief 위치.
     */
    glm::vec2 pos;

    /**
     * @brief 크기.
     */
    glm::vec2 size;

    /**
     * @brief 색상.
     */
    glm::vec3 color;
};

/**
 * @brief 두 사각형이 충돌하는지 검사합니다.
 *
 * @param left_ 비교할 첫 번째 사각형.
 * @param right_ 비교할 두 번째 사각형.
 *
 * @return bool 충돌 여부.
 */
static bool IsCollide(const Rect& left_,
                      const Rect& right_) noexcept;

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
 * @brief 애플리케이션 너비.
 */
static constexpr unsigned int WINDOW_WIDTH  = 800;

/**
 * @brief 애플리케이션 높이.
 */
static constexpr unsigned int WINDOW_HEIGHT = 600;

/**
 * @brief 애플리케이션 타이틀.
 */
static constexpr const char* const WINDOW_TITLE = "Animation Simulator";

/**
 * @brief GL 메이저 버전.
 */
constexpr unsigned char CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief GL 마이너 버전.
 */
constexpr unsigned char CONTEXT_MINOR_VERSION = 5;

/**
 * @brief 월드 내 있을 수 있는 사각형의 최대 갯수.
 */
static constexpr std::size_t CREATE_RECT_COUNTS = 20;

/**
 * @brief 월드 내 사각형들.
 */
static std::vector<Rect> rects;

/**
 * @brief 지우개 오브젝트.
 */
static Rect eraser;

/**
 * @brief 지우개 활성화 여부.
 */
static bool shouldEnableEraser = false;

/**
 * @brief 지금까지 지운 사각형의 갯수.
 */
static unsigned int currentEraseCounts = 0;

int main(int argc_, char** argv_)
{
    for (size_t count = 0; count < CREATE_RECT_COUNTS; ++count)
    {
        const float fx = static_cast<float>(std::rand() % (WINDOW_WIDTH - 50));
        const float fy = static_cast<float>(std::rand() % (WINDOW_HEIGHT - 50));

        rects.push_back(
        {
            {fx, fy},
            {50, 50},
            {
                static_cast<float>(std::rand() % 256) / 255.0f,
                static_cast<float>(std::rand() % 256) / 255.0f,
                static_cast<float>(std::rand() % 256) / 255.0f
            }
        });
    }

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

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (const auto&[position, size, color] : rects)
            {
                glColor3f(color.r, color.g, color.b);
                glRectf(position.x, position.y, position.x + size.x, position.y + size.y);
            }

            if (shouldEnableEraser)
            {
                glColor3f(eraser.color.r, eraser.color.g, eraser.color.b);

                if (currentEraseCounts > 0)
                {
                    glRectf(eraser.pos.x, eraser.pos.y,
                  eraser.pos.x + eraser.size.x * currentEraseCounts,
                  eraser.pos.y + eraser.size.y * currentEraseCounts);
                }
                else
                {
                    glRectf(eraser.pos.x, eraser.pos.y, (eraser.pos.x + eraser.size.x), (eraser.pos.y + eraser.size.y));
                }
            }
        }
        glfwSwapBuffers(window);
    }

    return 0;
}

bool IsCollide(const Rect& left_,
               const Rect& right_) noexcept
{
    return !(left_.pos.x + left_.size.x < right_.pos.x ||
             right_.pos.x + right_.size.x < left_.pos.x ||
             left_.pos.y + left_.size.y < right_.pos.y ||
             right_.pos.y + right_.size.y < left_.pos.y );
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
        case GLFW_KEY_R:
        {
            rects.clear();
            for (size_t count = 0; count < CREATE_RECT_COUNTS; ++count)
            {
                const float fx = static_cast<float>(std::rand() % (WINDOW_WIDTH - 50));
                const float fy = static_cast<float>(std::rand() % (WINDOW_HEIGHT - 50));

                rects.push_back(
                {
                    {fx, fy},
                    {50, 50},
                    {
                        static_cast<float>(std::rand() % 256) / 255.0f,
                        static_cast<float>(std::rand() % 256) / 255.0f,
                        static_cast<float>(std::rand() % 256) / 255.0f
                    }
                });
            }

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

void OnButtonInteracted(GLFWwindow* window_,
                        int         button_,
                        int         action_,
                        int         mods_)
{
    double mouseX, mouseY;
    glfwGetCursorPos(window_, &mouseX, &mouseY);

    if (button_ == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action_ == GLFW_PRESS)
        {
            shouldEnableEraser = true;

            const float fixedX = static_cast<float>(mouseX);
            const float fixedY = static_cast<float>(WINDOW_HEIGHT - mouseY);

            eraser.pos   = { fixedX - eraser.size.x * 0.5f,
                                fixedY - eraser.size.y * 0.5f };;
            eraser.size  = {20, 20};
            eraser.color = {0.0f, 0.0f, 0.0f};
        }
        else if (action_ == GLFW_RELEASE)
        {
            shouldEnableEraser = false;
            currentEraseCounts = 0;
        }
    }
    else if (button_ == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action_ == GLFW_PRESS)
        {
            if (rects.size() >= CREATE_RECT_COUNTS)
            {
                std::cout << "[Warning] Cannot create more rectangles!\n";
                return;
            }

            const float fx = static_cast<float>(std::rand() % (WINDOW_WIDTH - 50));
            const float fy = static_cast<float>(std::rand() % (WINDOW_HEIGHT - 50));

            rects.push_back(
            {
                {fx, fy},
                {50, 50},
                {
                    static_cast<float>(std::rand() % 256) / 255.0f,
                    static_cast<float>(std::rand() % 256) / 255.0f,
                    static_cast<float>(std::rand() % 256) / 255.0f
                }
            });
        }
    }
}

void OnCursorMoved(GLFWwindow* window_,
                   double      x_,
                   double      y_)
{
    if (!shouldEnableEraser)
    {
        return;
    }

    float fixedX = static_cast<float>(x_);
    float fixedY = static_cast<float>(WINDOW_HEIGHT - y_);

    eraser.pos = { fixedX - eraser.size.x * 0.5f,
                      fixedY - eraser.size.y * 0.5f };

    for (auto iter = rects.begin(); iter != rects.end(); ++iter)
    {
        if (IsCollide(eraser, *iter))
        {
            eraser.color = iter->color;
            currentEraseCounts++;

            rects.erase(iter);
            break;
        }
    }
}