#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

/**
 * @struct Rect
 *
 * @brief 사각형을 정의합니다.
 */
struct Rect
{
    /**
     * @brief 해당 사각형의 최소 좌표를 반환합니다.
     *
     * @return glm::vec2 최소 좌표.
     */
    [[nodiscard]]
    constexpr inline glm::vec2 GetMin() const noexcept
    {
        return { position.x - size.x * 0.5f, position.y - size.y * 0.5f };
    }

    /**
     * @brief 해당 사각형의 최대 좌표를 반환합니다.
     *
     * @return glm::vec2 최대 좌표.
     */
    [[nodiscard]]
    constexpr inline glm::vec2 GetMax() const noexcept
    {
        return { position.x + size.x * 0.5f, position.y + size.y * 0.5f };
    }

    /**
     * @brief 해당 사각형과 지정한 점이 접촉하는지 여부를 반환합니다.
     *
     * @param point_ 지정할 점.
     *
     * @return bool 접촉 여부.
     */
    [[nodiscard]]
    constexpr inline bool IsInteract(const glm::vec2& point_) const noexcept
    {
        const glm::vec2 min = GetMin();
        const glm::vec2 max = GetMax();

        return (point_.x >= min.x && point_.x <= max.x) &&
               (point_.y >= min.y && point_.y <= max.y);
    }

    /**
     * @brief 해당 사각형을 업데이트합니다.
     * @param deltaTime_
     */
    void Update(const float deltaTime_) noexcept
    {
        if (!glm::all(glm::epsilonEqual(direction, glm::vec2(0, 0), 1e-6f)))
        {
            constexpr float shrinkSpeed = 75.0f;
            float shrinkAmount = shrinkSpeed * deltaTime_;
            size -= glm::vec2(shrinkAmount, shrinkAmount);

            if (size.x < 0.0f || size.y < 0.0f)
            {
                size = glm::vec2(0.0f);
            }

            constexpr float speed = 300.0f;
            position += direction * (speed * deltaTime_);
        }
    }

    /**
     * @brief 해당 사각형을 그립니다.
     */
    void Render() const noexcept
    {
        ::glColor3f(color.r, color.g, color.b);
        ::glBegin(GL_QUADS);
        {
            const glm::vec2 min = Rect::GetMin();
            const glm::vec2 max = Rect::GetMax();

            glVertex2f(min.x, min.y);
            glVertex2f(max.x, min.y);
            glVertex2f(max.x, max.y);
            glVertex2f(min.x, max.y);
        }
        ::glEnd();
    }

    /**
     * @brief 위치.
     */
    glm::vec2 position;

    /**
     * @brief 크기.
     */
    glm::vec2 size;

    /**
     * @brief 색상.
     */
    glm::vec3 color;

    /**
     * @brief 이동 방향.
     */
    glm::vec2 direction;
};

enum class EffectType
{
    /**
     * @brief 직각 이동.
     */
    Orthogonal,

    /**
     * @brief 대각 이동.
     */
    Diagonal,

    /**
     * @brief 대칭 이동.
     */
    Symmetrical,

    /**
     * @brief 모든 방향으로.
     */
    All,
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
 * @param mods_ 버튼 상태.
 */
static void OnButtonInteracted(GLFWwindow* const window_,
                               const int         button_,
                               const int         action_,
                               const int         mods_) noexcept;

/**
 * @brief 커서의 위치가 변경되었을 때 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
static void OnCursorMoved(GLFWwindow* const window_,
                          const double      x_,
                          const double      y_) noexcept;

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
static constexpr const char* const WINDOW_TITLE = "Effect Simulator";

/**
 * @brief GL 메이저 버전.
 */
constexpr unsigned char CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief GL 마이너 버전.
 */
constexpr unsigned char CONTEXT_MINOR_VERSION = 5;

/**
 * @brief 현재 커서 위치.
 */
static glm::vec2 cursorPosition;

/**
 * @brief 월드 내 모든 사각형.
 */
static std::vector<Rect> rects;

int main(int, char**)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (std::size_t count = 0; count < 20; ++count)
    {
        const glm::vec2 position   = { static_cast<float>(std::rand() % (WINDOW_WIDTH - 50)),
                                       static_cast<float>(std::rand() % (WINDOW_HEIGHT - 50)) };
        const glm::vec2 size       = { static_cast<float>((std::rand() % 100) + 20),
                                       static_cast<float>((std::rand() % 100) + 20) };
        const glm::vec3 color      = { static_cast<float>(std::rand() % 256) / 255.0f,
                                       static_cast<float>(std::rand() % 256) / 255.0f,
                                       static_cast<float>(std::rand() % 256) / 255.0f };

        rects.push_back({ position, size, color, {0.0f, 0.0f} });
    }

    if (!::glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_MAJOR_VERSION);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_MINOR_VERSION);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    ::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    ::GLFWwindow* window = ::glfwCreateWindow(WINDOW_WIDTH,
                                              WINDOW_HEIGHT,
                                              WINDOW_TITLE,
                                              nullptr,
                                              nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        ::glfwTerminate();
        return -1;
    }

    ::glfwSetWindowPos(window, 100, 100);
    ::glfwMakeContextCurrent(window);

    if (!::gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    ::glfwSetKeyCallback(window, OnKeyInteracted);
    ::glfwSetMouseButtonCallback(window, OnButtonInteracted);
    ::glfwSetCursorPosCallback(window, OnCursorMoved);

    float lastTime = static_cast<float>(glfwGetTime());

    while (!::glfwWindowShouldClose(window))
    {
        ::glfwPollEvents();

        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        ::glMatrixMode(GL_PROJECTION);
        ::glLoadIdentity();
        ::glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1, 1);

        ::glMatrixMode(GL_MODELVIEW);
        ::glLoadIdentity();

        ::glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        ::glClear(GL_COLOR_BUFFER_BIT);

        for (Rect& rect : rects)
        {
            rect.Update(deltaTime);
            rect.Render();
        }

        ::glfwSwapBuffers(window);
    }

    ::glfwTerminate();
    return 0;
}

void OnKeyInteracted(GLFWwindow* const window_,
                     const int         key_,
                     const int         scancode_,
                     const int         action_,
                     const int         mods_) noexcept
{
    if (key_ == GLFW_KEY_Q &&
        action_ == GLFW_PRESS)
    {
        ::glfwSetWindowShouldClose(window_, true);
    }
}

void OnButtonInteracted(GLFWwindow* const window_,
                        const int         button_,
                        const int         action_,
                        const int         mods_) noexcept
{
    if (button_ == GLFW_MOUSE_BUTTON_LEFT &&
        action_ == GLFW_PRESS)
    {
        auto toErase = rects.end();

        for (auto iter = rects.begin(); iter != rects.end(); ++iter)
        {
            if (iter->IsInteract(cursorPosition))
            {
                toErase = iter;
                break;
            }
        }

        if (toErase != rects.end())
        {
            const glm::vec2 center = toErase->position;
            const glm::vec2 size   = toErase->size;
            const glm::vec3 color  = toErase->color;

            rects.erase(toErase);

            const EffectType effect = static_cast<EffectType>(std::rand() % 4);

            switch (effect)
            {
                case EffectType::Orthogonal:
                {
                    constexpr glm::vec2 directions[4] = {
                        {0.0f,  1.0f},
                        {0.0f, -1.0f},
                        {-1.0f, 0.0f},
                        {1.0f,  0.0f}
                    };

                    for (auto direction : directions)
                    {
                        rects.push_back({ center, size, color, direction });
                    }
                    break;
                }
                case EffectType::Diagonal:
                {
                    constexpr glm::vec2 directions[4] = {
                        {  1.0f,  1.0f },
                        {  1.0f, -1.0f },
                        { -1.0f, -1.0f },
                        { -1.0f,  1.0f }
                    };

                    for (auto direction : directions)
                    {
                        rects.push_back({ center, size, color, direction });
                    }
                    break;
                }
                case EffectType::Symmetrical:
                {
                    // 하나의 방향만 랜덤으로 선택
                    constexpr glm::vec2 directions[4] = {
                        {  0.0f,  1.0f },
                        {  0.0f, -1.0f },
                        { -1.0f,  0.0f },
                        {  1.0f,  0.0f }
                    };

                    glm::vec2 chosenDir = directions[std::rand() % 4];

                    for (int i = 0; i < 4; ++i) // 사각형 4개
                    {
                        rects.push_back({ center, size, color, chosenDir });
                    }
                    break;
                }
                case EffectType::All:
                {
                    constexpr glm::vec2 directions[8] = {
                        { 0.0f,  1.0f}, {  0.0f, -1.0f},
                        {-1.0f,  0.0f}, {  1.0f,  0.0f},
                        { 1.0f,  1.0f}, {  1.0f, -1.0f},
                        {-1.0f, -1.0f}, {- 1.0f,  1.0f}
                    };

                    for (auto direction : directions)
                    {
                        rects.push_back({ center, size, color, direction });
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
}

void OnCursorMoved(GLFWwindow* const  window_,
                   const double       x_,
                   const double       y_) noexcept
{
    cursorPosition = { static_cast<float>(x_),
                          static_cast<float>(WINDOW_HEIGHT - y_) };
}