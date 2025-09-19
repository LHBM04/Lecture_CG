#include <iostream>
#include <vector>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>

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
 * @brief 지정한 위치가 지정한 영역 내부에 포함되는지 여부를 반환합니다.
 *
 * @param area_ 지정할 영역.
 * @param point_ 지정할 위치.
 * @return
 */
[[nodiscard]]
static bool IsContain(const glm::vec2& point_,
                      const Rect&      area_) noexcept;

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
 *
 * @param value_ 사용자 정의 매개변수.
 */
static void OnTime(int value_) noexcept;

/**
 * @brief 키가 눌렸을 때 호출됩니다.
 *
 * @param key_ 눌린 키.
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
static void OnKeyPressed(unsigned char key_,
                         int           x_,
                         int           y_);

/**
 * @brief 버튼이 클릭되었을 때 호출됩니다.
 *
 * @param button_ 클릭된 버튼.
 * @param state_ 버튼 상태.
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
static void OnButtonClicked(int button_,
                            int state_,
                            int x_,
                            int y_);

/**
 * @brief 마우스 커서가 움직일 때 호출됩니다.
 *
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
static void OnCursorMoved(int x_,
                          int y_);

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
 * @brief 따라하기 여부.
 */
static bool shouldFollow = false;

/**
 * @brief 따라할 대상 인덱스.
 */
static int followTargetIndex = -1;

int main(int    argc_,
         char** argv_)
{
    glutInit(&argc_, argv_);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        return EXIT_FAILURE;
    }

    glutDisplayFunc(OnDisplay);
    glutTimerFunc(16, OnTime, 0);
    glutKeyboardFunc(OnKeyPressed);
    glutMouseFunc(OnButtonClicked);
    glutMotionFunc(OnCursorMoved);
    glutMainLoop();
}

bool IsContain(const glm::vec2& point_, const Rect& area_) noexcept
{
    return (point_.x >= area_.min.x && point_.x <= area_.max.x &&
            point_.y >= area_.min.y && point_.y <= area_.max.y);
}

void Draw(const Rect& rect_,
          bool        shouldFill_) noexcept
{
    glColor3f(rect_.color.r,
              rect_.color.g,
              rect_.color.b);

    glBegin(shouldFill_ ? GL_QUADS :
                          GL_LINE_LOOP);

    glVertex2f(rect_.min.x, rect_.min.y);
    glVertex2f(rect_.max.x, rect_.min.y);
    glVertex2f(rect_.max.x, rect_.max.y);
    glVertex2f(rect_.min.x, rect_.max.y);

    glEnd();
}

void OnDisplay() noexcept
{
    constexpr float displayWidth  = static_cast<float>(WINDOW_WIDTH);
    constexpr float displayHeight = static_cast<float>(WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, displayWidth, 0.0, displayHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    constexpr glm::vec3 backgroundColor = { 0.1f, 0.1f, 0.1f };

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const Rect& rect : rects)
    {
        Draw(rect, true);
    }

    if (newRect)
    {
        glm::vec2 center = newRect->min;
        glm::vec2 drag   = newRect->max - newRect->min;
        glm::vec2 half   = glm::abs(drag) * 2.0f;

        glm::vec2 min   = center - half;
        glm::vec2 max   = center + half;
        glm::vec3 color = { 1.0f, 1.0f, 1.0f };

        Draw({ min, max, color }, false);
    }

    glutSwapBuffers();
}

void OnTime(int value_) noexcept
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

    glutPostRedisplay();
    glutTimerFunc(16, OnTime, 0);
}


void OnKeyPressed(unsigned char key_,
                  int           x_,
                  int           y_)
{
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
            glutLeaveMainLoop();
            break;
        }
        default:
        {
            break;
        }
    }

    glutPostRedisplay();
}

void OnButtonClicked(int button_,
                     int state_,
                     int x_,
                     int y_)
{
    if (button_ == GLUT_LEFT_BUTTON)
    {
        if (state_ == GLUT_DOWN)
        {
            if (currentMoveType != MoveType::None ||
                shouldChangeColor ||
                shouldChangeSize)
            {
                return;
            }

            newRect = new Rect();

            newRect->min   = { static_cast<float>(x_), static_cast<float>(WINDOW_HEIGHT - y_) };
            newRect->max      = newRect->min;
            newRect->color = { 1.0f, 1.0f, 1.0f };
        }
        else if (state_ == GLUT_UP)
        {
            if (!newRect)
            {
                return;
            }

            const glm::vec2 center = newRect->min;
            const glm::vec2 drag   = newRect->max - newRect->min;
            const glm::vec2 half   = glm::abs(drag) * 2.0f;

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
    else if (button_ == GLUT_RIGHT_BUTTON)
    {
        auto toErase = rects.end();

        if (state_ == GLUT_DOWN)
        {
            const glm::vec2 mousePosition = { static_cast<float>(x_),
                                              static_cast<float>(WINDOW_HEIGHT - y_)};

            for (auto iter = rects.begin(); iter != rects.end(); ++iter)
            {
                if (IsContain(mousePosition, *(iter)))
                {
                    toErase = iter;
                    break;
                }
            }
        }
        else if (state_ == GLUT_UP)
        {
            if (toErase != rects.end())
            {
                rects.erase(toErase);
            }
        }
    }

    glutPostRedisplay();
}

void OnCursorMoved(int x_,
                   int y_)
{
   if (!newRect)
   {
       return;
   }

    newRect->max = { static_cast<float>(x_),
                        static_cast<float>(WINDOW_HEIGHT - y_) };
    glutPostRedisplay();
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