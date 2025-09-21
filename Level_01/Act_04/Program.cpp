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
    glm::vec2 min;

    /**
     * @brief 크기.
     */
    glm::vec2 max;

    /**
     * @brief 색상.
     */
    glm::vec3 color;
};

/**
 * @brief 이동 방식을 정의합니다.
 */
enum class MoveType
{
    /**
     * @brief 이동하지 않음.
     */
    None,

    /**
     * @brief 대각선으로 이동.
     */
    MoveDiagonally,

    /**
     * @brief 지그재그로 이동.
     */
    MoveZigzag,
};

/**
 * @brief 지정한 사각형을 그립니다.
 *
 * @param rect_ 그릴 사각형.
 * @param shouldFill_ 내부 채색 여부.
 */
static void Draw(const Rect& rect_,
                 bool        shouldFill_) noexcept;

/**
 * @brief 윈도우가 그려질 때 호출됩니다.
 */
void OnDisplay() noexcept;

/**
 * @brief 특정 시간이 지날 때마다 호출됩니다.
 */
static void OnUpdate() noexcept;

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
 * @brief 지정한 사각형을 대각선 방향으로 이동시킵니다.
 *
 * @param rect_ 지정할 사각형.
 * @param direction_ 이동 방향.
 */
static void MoveDiagonally(Rect&      rect_,
                           glm::vec2& direction_);

/**
 * @brief 지정한 사각형을 지그재그 방향으로 이동시킵니다.
 *
 * @param rect_ 지정할 사각형.
 * @param direction_ 이동 방향.
 */
static void MoveZigzag(Rect&      rect_,
                       glm::vec2& direction_);

/**
 * @brief 윈도우 너비.
 */
static constexpr int WINDOW_WIDTH  = 800;

/**
 * @brief 윈도우 높이.
 */
static constexpr int WINDOW_HEIGHT = 600;

/**
 * @brief 윈도우 제목.
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
 * @brief 사각형들.
 */
static std::vector<Rect> rects;

/**
 * @brief 사각형들의 본래 위치.
 */
static std::vector<glm::vec2> originalPositions;

/**
 * @brief 추가할 새 사각형.
 */
static Rect* newRect = nullptr;

/**
 * @brief 움직임 방식.
 */
static MoveType currentMoveType = MoveType::None;

/**
 * @brief 크기 변경 여부.
 */
static bool shouldChangeSize = false;

/**
 * @brief 색상 변경 여부.
 */
static bool shouldChangeColor = false;

/**
 * @brief 따라할 대상 인덱스.
 */
static int followTargetIndex = -1;

int main(int    argc_,
         char** argv_)
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
            const double nowTime = glfwGetTime();
            if (nowTime - lastTime >= 0.05)
            {
                OnUpdate();
                lastTime = nowTime;
            }

            OnDisplay();
        }
        glfwSwapBuffers(window);
    }

    return 0;
}

void Draw(const Rect& rect_,
          bool        shouldFill_) noexcept
{
    glBegin(shouldFill_ ? GL_QUADS : GL_LINE_LOOP);
    {
        glColor3f(rect_.color.r,rect_.color.g, rect_.color.b);
        glVertex2f(rect_.min.x, rect_.min.y);
        glVertex2f(rect_.max.x, rect_.min.y);
        glVertex2f(rect_.max.x, rect_.max.y);
        glVertex2f(rect_.min.x, rect_.max.y);
    }
    glEnd();
}

void OnDisplay() noexcept
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const Rect& rect : rects)
    {
        Draw(rect, true);
    }

    if (newRect != nullptr)
    {
        Draw(*(newRect), false);
    }
}

void OnUpdate() noexcept
{
    switch (currentMoveType)
    {
        case MoveType::MoveDiagonally:
        {
            static std::vector<glm::vec2> directions(rects.size(), glm::vec2(1.0f, 1.0f));

            if (directions.size() != rects.size())
            {
                directions.assign(rects.size(), glm::vec2(1.0f, 1.0f));
            }

            for (std::size_t index = 0; index < rects.size(); ++index)
            {
                MoveDiagonally(rects[index], directions[index]);
            }

            break;
        }
        case MoveType::MoveZigzag:
        {
            static std::vector<glm::vec2> directions(rects.size(), glm::vec2(1.0f, 1.0f));

            if (directions.size() != rects.size())
            {
                directions.assign(rects.size(), glm::vec2(1.0f, 1.0f));
            }

            for (std::size_t index = 0; index < rects.size(); ++index)
            {
                MoveZigzag(rects[index], directions[index]);
            }

            break;
        }
        case MoveType::None: [[fallthrough]];
        default:
        {
            break;
        }
    }

    if (shouldChangeColor)
    {
        for (auto& rect : rects)
        {
            rect.color = { static_cast<float>(std::rand() % 256) / 255.0f,
                              static_cast<float>(std::rand() % 256) / 255.0f,
                              static_cast<float>(std::rand() % 256) / 255.0f };
        }
    }

    if (shouldChangeSize)
    {
        static std::vector<glm::vec2> sizeDirections(rects.size(), glm::vec2(-1.0f, -1.0f));

        for (std::size_t index = 0; index < rects.size(); ++index)
        {
            const glm::vec2 center = (rects[index].min + rects[index].max) * 0.5f;
            const glm::vec2 size   = rects[index].max - rects[index].min;

            if (size.x < 20.0f ||
                size.y < 20.0f)
            {
                sizeDirections[index] = glm::vec2(1.0f, 1.0f);
            }
            else if (size.x > 200.0f ||
                     size.y > 200.0f)
            {
                sizeDirections[index] = glm::vec2(-1.0f, -1.0f);
            }

            const glm::vec2 newHalf = (size * 0.5f) * ((sizeDirections[index].x > 0) ? 1.02f : 0.98f);
            rects[index].min = center - newHalf;
            rects[index].max = center + newHalf;
        }
    }

    if (followTargetIndex != -1)
    {
        for (size_t index = 0; index < rects.size(); ++index)
        {
            if (index == followTargetIndex)
            {
                continue;
            }

            const glm::vec2 targetCenter   = (rects[followTargetIndex].min + rects[followTargetIndex].max) * 0.5f;
            const glm::vec2 followerCenter = (rects[index].min + rects[index].max) * 0.5f;
            const glm::vec2 offset         = targetCenter - followerCenter;

            rects[index].min += offset * 0.1f;
            rects[index].max += offset * 0.1f;
        }
    }
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
        case '1':
        {
            if (rects.empty())
            {
                break;
            }

            currentMoveType = currentMoveType == MoveType::MoveDiagonally ? MoveType::None :
                                                                            MoveType::MoveDiagonally;
            break;
        }
        case '2':
        {
            if (rects.empty())
            {
                break;
            }

            currentMoveType = currentMoveType == MoveType::MoveZigzag ? MoveType::None :
                                                                        MoveType::MoveZigzag;
            break;
        }
        case '3':
        {
            if (rects.empty())
            {
                break;
            }

            static std::vector<glm::vec2> originalMin;
            static std::vector<glm::vec2> originalMax;

            if (!shouldChangeSize)
            {
                shouldChangeSize = true;

                const std::size_t rectsCount = rects.size();
                originalMin.reserve(rects.size());
                originalMax.reserve(rects.size());

                for (std::size_t index = 0; index < rectsCount; ++index)
                {
                    originalMin.push_back(rects[index].min);
                    originalMax.push_back(rects[index].max);
                }
            }
            else
            {
                shouldChangeSize = false;

                for (std::size_t index = 0; index < originalMin.size(); ++index)
                {
                    rects[index].min = originalMin[index];
                    rects[index].max = originalMax[index];
                }

                originalMin.clear();
                originalMax.clear();
            }

            break;
        }
        case '4':
        {
            if (rects.empty())
            {
                break;
            }

            static std::vector<glm::vec3> originalColors;

            if (!shouldChangeColor)
            {
                shouldChangeColor = true;

                const std::size_t rectsCount = rects.size();
                originalColors.reserve(rects.size());

                for (std::size_t index = 0; index < rectsCount; ++index)
                {
                    originalColors.push_back(rects[index].color);
                }
            }
            else
            {
                shouldChangeColor = false;

                for (std::size_t index = 0; index < originalColors.size(); ++index)
                {
                    rects[index].color = originalColors[index];
                }

                originalColors.clear();
            }

            break;
        }
        case '5':
        {
            if (rects.empty())
            {
                break;
            }

            followTargetIndex = followTargetIndex == -1 ? std::rand() % rects.size() : -1;
            break;
        }
        case 's': [[fallthrough]];
        case 'S':
        {
            currentMoveType   = MoveType::None;
            shouldChangeColor = false;
            shouldChangeSize  = false;
            followTargetIndex = -1;

            break;
        }
        case 'm': [[fallthrough]];
        case 'M':
        {
            currentMoveType   = MoveType::None;
            shouldChangeColor = false;
            shouldChangeSize  = false;
            followTargetIndex = -1;

            for (std::size_t index = 0; index < rects.size(); ++index)
            {
                const glm::vec2 center = originalPositions[index];
                const glm::vec2 size   = rects[index].max - rects[index].min;
                rects[index].min = center - size * 0.5f;
                rects[index].max = center + size * 0.5f;
            }

            break;
        }
        case 'r': [[fallthrough]];
        case 'R':
        {
            currentMoveType   = MoveType::None;
            shouldChangeColor = false;
            shouldChangeSize  = false;
            followTargetIndex = -1;

            rects.clear();
            originalPositions.clear();
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
    if (button_ == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action_ == GLFW_PRESS)
        {
            if (currentMoveType != MoveType::None ||
                shouldChangeColor ||
                shouldChangeSize)
            {
                return;
            }

            double mouseX, mouseY;
            glfwGetCursorPos(window_, &mouseX, &mouseY);
            float fixedY = static_cast<float>(WINDOW_HEIGHT - mouseY);

            newRect = new Rect();
            newRect->min   = { mouseX, fixedY };
            newRect->max      = newRect->min;
            newRect->color = { 1.0f, 1.0f, 1.0f };
        }
        else if (action_ == GLFW_RELEASE)
        {
            if (!newRect)
            {
                return;
            }

            const glm::vec2 center = newRect->min;
            const glm::vec2 drag   = newRect->max - newRect->min;
            const glm::vec2 half   = glm::abs(drag);

            newRect->min      = center - half;
            newRect->max      = center + half;
            newRect->color = { static_cast<float>(std::rand() % 256) / 255.0f,
                                  static_cast<float>(std::rand() % 256) / 255.0f,
                                  static_cast<float>(std::rand() % 256) / 255.0f };

            rects.push_back(*(newRect));
            originalPositions.push_back(center);

            delete newRect;
            newRect = nullptr;
        }
    }
}

void OnCursorMoved(GLFWwindow* window_,
                   double      x_,
                   double      y_)
{
   if (!newRect)
   {
       return;
   }

    newRect->max = { x_, WINDOW_HEIGHT - y_ };
}

void MoveDiagonally(Rect&      rect_,
                    glm::vec2& direction_)
{
    const float width  = rect_.max.x - rect_.min.x;
    const float height = rect_.max.y - rect_.min.y;

    rect_.min += direction_ * 10.0f;
    rect_.max += direction_ * 10.0f;

    if (rect_.min.x < 0)
    {
        rect_.min.x  = 0;
        rect_.max.x  = width;
        direction_.x = -direction_.x;
    }
    else if (rect_.max.x > WINDOW_WIDTH)
    {
        rect_.max.x  = WINDOW_WIDTH;
        rect_.min.x  = WINDOW_WIDTH - width;
        direction_.x = -direction_.x;
    }

    if (rect_.min.y < 0)
    {
        rect_.min.y  = 0;
        rect_.max.y  = height;
        direction_.y = -direction_.y;
    }
    else if (rect_.max.y > WINDOW_HEIGHT)
    {
        rect_.max.y  = WINDOW_HEIGHT;
        rect_.min.y  = WINDOW_HEIGHT - height;
        direction_.y = -direction_.y;
    }
}

void MoveZigzag(Rect&      rect_,
                glm::vec2& direction_)
{
    const float width  = rect_.max.x - rect_.min.x;
    const float height = rect_.max.y - rect_.min.y;

    rect_.min += direction_ * 10.0f;
    rect_.max += direction_ * 10.0f;

    if (rect_.min.x < 0)
    {
        rect_.min.x  = 0;
        rect_.max.x  = width;
        direction_.x = -direction_.x;
        direction_.y = std::abs(direction_.y);
    }
    else if (rect_.max.x > WINDOW_WIDTH)
    {
        rect_.max.x  = WINDOW_WIDTH;
        rect_.min.x  = WINDOW_WIDTH - width;
        direction_.x = -direction_.x;
        direction_.y = -std::abs(direction_.y);
    }

    if (rect_.min.y < 0)
    {
        rect_.min.y  = 0;
        rect_.max.y  = height;
        direction_.y = std::abs(direction_.y);
    }
    else if (rect_.max.y > WINDOW_HEIGHT)
    {
        rect_.max.y  = WINDOW_HEIGHT;
        rect_.min.y  = WINDOW_HEIGHT - height;
        direction_.y = -std::abs(direction_.y);
    }
}