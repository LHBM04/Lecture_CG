#include <cmath>
#include <ctime>
#include <format>
#include <iostream>
#include <optional>
#include <vector>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>

/**
 * @struct Rect
 *
 * @brief 사각형을 정의합니다.
 */
struct Rect
{
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

    /**
     * @brief 이동 방향.
     */
    glm::vec2 velocity;
};

enum class AnimationType : unsigned char
{
    /**
     * @brief 비어있음.
     */
    None         = 0,

    /**
     * @brief 대각선으로 이동.
     */
    MoveDiagonal = 1 << 0,

    /**
     * @brief 지그재그로 이동.
     */
    MoveZigzag   = 1 << 1,

    /**
     * @brief 크기 변경.
     */
    SizeChange   = 1 << 2,

    /**
     * @brief 색 변경.
     */
    ColorChange  = 1 << 3,

    /**
     * @brief 따라하기.
     */
    Imitation    = 1 << 4,
};

/**
 * @brief 특정 밀리세컨드마다 호출됩니다.
 *
 * @param value_ 사용자 정의 인자.
 */
static void OnUpdate(int value_);

/**
 * @brief 화면이 그려질 때 호출됩니다.
 */
static void OnRender();

/**
 * @brief 키가 눌렸을 때 호출됩니다.
 *
 * @param key_ 눌린 키.
 * @param x_ 마우스의 X 좌표.
 * @param y_ 마우스의 Y 좌표.
 */
static void OnKeyDown(unsigned char key_,
                      int           x_,
                      int           y_);

/**
 * @brief 버튼이 눌렸을 때 호출됩니다.
 * @param button_ 눌린 버튼.
 * @param state_ 버튼 상태.
 * @param x_ 마우스의 X 좌표.
 * @param y_ 마우스의 Y 좌표.
 */
static void OnMouseClick(int button_,
                           int state_,
                           int x_,
                           int y_);

/**
 * @brief 마우스가 움직일 때 호출됩니다.
 *
 * @param x_ 마우스의 X 좌표.
 * @param y_ 마우스의 Y 좌표.
 */
static void OnMouseMove(int x_,
                        int y_);

/**
 * @brief 지정한 사각형을 대각선으로 움직입니다..
 *
 * @param rect_ 지정할 사각형.
 */
static void MoveDiagonaly(Rect& rect_);

/**
 * @brief 지정한 사각형을 지그재그로 움직입니다.
 *
 * @param rect_ 지정할 사각형.
 */
static void MoveZigZag(Rect& rect_);

/**
 * @brief 지정한 사각형의 크기를 변경합니다.
 *
 * @param rect_ 지정할 사각형.
 */
static void ChangeSize(Rect& rect_);

/**
 * @brief 지정한 사각형의 색상을 변경합니다.
 *
 * @param rect_ 지정할 사각형.
 */
static void ChangeColor(Rect& rect_);

/**
 * @brief 지정한 사각형을 특정 사각형의 애니메이션대로 움직입니다.
 *
 * @param rect_ 지정할 사각형.
 */
static void MoveImitate(Rect& rect_);

/**
 * @brief 윈도우 너비.
 */
static constexpr unsigned int WINDOW_WIDTH = 800;

/**
 * @brief 윈도우 높이.
 */
static constexpr unsigned int WINDOW_HEIGHT = 600;

/**
 * @brief 업데이트 함수 호출 여부.
 */
static bool shouldUpdate = false;

/**
 * @brief 업데이트 함수 호출 주기.
 */
static unsigned int milliseconds = 100;

/**
 * @brief 애니메이션 재생 속도.
 */
static constexpr float animationSpeed = 15.0f;

/**
 * @brief 생성할 수 있는 사각형의 최대 갯수.
 */
static std::size_t MAX_RECT_COUNTS = 5;

/**
 * @brief 그려질 도형들.
 */
static std::vector<Rect> rects;

static std::vector<glm::vec2> originalPosition;

/**
 * @brief 새로 그려질 사각형.
 */
static std::optional<Rect> newRect;

/**
 * @brief 현재 애니메이션 타입.
 */
static AnimationType currentAnimation = AnimationType::None;

/**
 * @brief 따라할 사각형의 인덱스.
 */
static std::size_t leaderRectIndex = 0;

int main(int    argc_,
         char** argv_)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    glutInit(&argc_, argv_);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Program");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        return EXIT_FAILURE;
    }

    glutDisplayFunc(OnRender);
    glutKeyboardFunc(OnKeyDown);
    glutMouseFunc(OnMouseClick);
    glutMotionFunc(OnMouseMove);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void OnUpdate(int value_)
{
    switch (currentAnimation)
    {
        case AnimationType::MoveDiagonal:
        {
            for (Rect& rect : rects)
            {
                MoveDiagonaly(rect);
            }

            break;
        }
        case AnimationType::MoveZigzag:
        {
            for (Rect& rect : rects)
            {
                MoveZigZag(rect);
            }

            break;
        }
        case AnimationType::SizeChange:
        {
            for (Rect& rect : rects)
            {
                ChangeSize(rect);
            }

            break;
        }
        case AnimationType::ColorChange:
        {
            for (Rect& rect : rects)
            {
                ChangeColor(rect);
            }

            break;
        }
        case AnimationType::Imitation:
        {
            for (Rect& rect : rects)
            {
                if (&rect == &rects[leaderRectIndex])
                {
                    continue;
                }

                MoveImitate(rect);
            }

            break;
        }
        default:
        {
            return;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(milliseconds, OnUpdate, value_);
}

void OnRender()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto&[min, max, color, _] : rects)
    {
        glColor3f(color.r, color.g, color.b);
        glRectf(min.x, min.y, max.x, max.y);
    }

    if (newRect.has_value())
    {
        glColor3f(newRect->color.r, newRect->color.g, newRect->color.b);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glRectf(newRect->min.x, newRect->min.y, newRect->max.x, newRect->max.y);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glutSwapBuffers();
}

void OnKeyDown(unsigned char key_,
               int           x_,
               int           y_)
{
    switch (key_)
    {
        case '1':
        {
            if (currentAnimation == AnimationType::MoveDiagonal)
            {
                currentAnimation = AnimationType::None;
                std::cout << "[Info] Stopped the current animation.\n";
                break;
            }

            if (originalPosition.empty())
            {
                originalPosition.resize(rects.size());
                for (std::size_t i = 0; i < rects.size(); ++i)
                {
                    originalPosition[i] = rects[i].min;
                }
            }

            for (Rect& rect : rects)
            {
                const float angle = static_cast<float>(std::rand() % 360);
                rect.velocity = { std::cos(glm::radians(angle)), std::sin(glm::radians(angle)) };
            }

            currentAnimation = AnimationType::MoveDiagonal;
            glutTimerFunc(milliseconds, OnUpdate, 0);
            std::cout << "[Info] Now Playing... Move Diagonaly Animation\n";
            break;
        }
        case '2':
        {
            if (currentAnimation == AnimationType::MoveZigzag)
            {
                currentAnimation = AnimationType::None;
                std::cout << "[Info] Stopped the current animation.\n";
                break;
            }

            for (Rect& rect : rects)
            {
                float angle = static_cast<float>(std::rand() % 360);
                rect.velocity = { std::cos(glm::radians(angle)), std::sin(glm::radians(angle)) };
            }

            currentAnimation = AnimationType::MoveZigzag;
            glutTimerFunc(milliseconds, OnUpdate, 0);
            std::cout << "[Info] Now Playing... Move ZigZag Animation\n";
            break;
        }
        case '3':
        {
            if (currentAnimation == AnimationType::SizeChange)
            {
                currentAnimation = AnimationType::None;
                std::cout << "[Info] Stopped the current animation.\n";
                break;
            }

            currentAnimation = AnimationType::SizeChange;
            glutTimerFunc(milliseconds, OnUpdate, 0);
            std::cout << "[Info] Now Playing... Size Change Animation\n";
            break;
        }
        case '4':
        {
            if (currentAnimation == AnimationType::ColorChange)
            {
                currentAnimation = AnimationType::None;
                std::cout << "[Info] Stopped the current animation.\n";
                break;
            }

            currentAnimation = AnimationType::ColorChange;
            glutTimerFunc(milliseconds, OnUpdate, 0);
            std::cout << "[Info] Now Playing... Color Change Animation\n";
            break;
        }
        case '5':
        {
            if (currentAnimation == AnimationType::Imitation)
            {
                currentAnimation = AnimationType::None;
                std::cout << "[Info] Stopped the current animation.\n";
                break;
            }

            if (rects.size() < 2)
            {
                std::cerr << "[Oops!] Need at least 2 rectangles for imitation animation!\n";
                break;
            }

            leaderRectIndex = std::rand() % rects.size();

            float angle = static_cast<float>(std::rand() % 360);
            rects[leaderRectIndex].velocity = { std::cos(glm::radians(angle)),
                                                   std::sin(glm::radians(angle)) };

            currentAnimation = AnimationType::Imitation;
            glutTimerFunc(milliseconds, OnUpdate, 0);
            std::cout << std::format("[Info] Now Playing... Imitation Animation (Rectangle {} is the leader)\n", leaderRectIndex);
            break;
        }
        case 's':
        {
            currentAnimation = AnimationType::None;
            std::cout << "[Info] Stopped the current animation.\n";
            break;
        }
        case 'm':
        {
            break;
        }
        case 'r':
        {
            if (rects.empty())
            {
                std::cerr << "[Oops!] No rects to clear!\n";
                break;
            }

            rects.pop_back();
            std::cout << "[Info] The last rectangle deleted.\n";
            break;
        }
        case 'q':
        {
            glutLeaveMainLoop();
            std::cout << "[Info] Application terminated.\n";
            break;
        }
        default:
        {
            break;
        }
    }

    glutPostRedisplay();
}

void OnMouseClick(int button_,
                  int state_,
                  int x_,
                  int y_)
{
    if (button_ == GLUT_LEFT_BUTTON)
    {
        if (state_ == GLUT_DOWN)
        {
            if (rects.size() + 1 > MAX_RECT_COUNTS)
            {
                std::cerr << "[Oops!] Too many rects!" << std::endl;
                return;
            }

            if (currentAnimation != AnimationType::None)
            {
                std::cerr << "[Oops!] Can't draw new rectangle during animation!\n";
                return;
            }

            const float fx = static_cast<float>(x_);
            const float fy = static_cast<float>(WINDOW_HEIGHT - y_);

            newRect = Rect{};
            newRect->min = {fx, fy};
            newRect->max = {fx, fy};
        }
        else if (state_ == GLUT_UP)
        {
            if (!newRect.has_value())
            {
                return;
            }

            newRect->color.r = static_cast<float>(std::rand()) / RAND_MAX;
            newRect->color.g = static_cast<float>(std::rand()) / RAND_MAX;
            newRect->color.b = static_cast<float>(std::rand()) / RAND_MAX;

            rects.push_back(newRect.value());
            newRect = std::nullopt;

            std::cout << std::format("[Info] New rectangle created. Total counts: {:d}\n", rects.size());
        }
        else
        {
            std::cerr << "[Warning] Unknown button state!\n";
        }
    }
    else if (button_ == GLUT_RIGHT_BUTTON)
    {
        if (!newRect.has_value())
        {
            return;
        }

        newRect = std::nullopt;
        std::cout << "[Info] Cancel drawing new rectangle.\n";
    }

    glutPostRedisplay();
}

void OnMouseMove(int x_,
                 int y_)
{
    if (!newRect.has_value())
    {
        return;
    }

    const float fx = static_cast<float>(x_);
    const float fy = static_cast<float>(WINDOW_HEIGHT - y_);

    newRect->max = {fx, fy};

    if (newRect->min.x > newRect->max.x)
    {
        std::swap(newRect->min.x, newRect->max.x);
    }
    if (newRect->min.y > newRect->max.y)
    {
        std::swap(newRect->min.y, newRect->max.y);
    }

    std::cout << std::format("[Trace] Drawing rectangle from ({:.1f}, {:.1f}) to ({:.1f}, {:.1f})\n",
                             newRect->min.x, newRect->min.y, newRect->max.x, newRect->max.y);

    glutPostRedisplay();
}


void MoveDiagonaly(Rect& rect_)
{
    const float width  = rect_.max.x - rect_.min.x;
    const float height = rect_.max.y - rect_.min.y;

    rect_.min += rect_.velocity * animationSpeed;
    rect_.max += rect_.velocity * animationSpeed;

    if (rect_.min.x < 0)
    {
        rect_.min.x      = 0;
        rect_.max.x      = width;
        rect_.velocity.x = -rect_.velocity.x;
    }
    else if (rect_.max.x > WINDOW_WIDTH)
    {
        rect_.max.x      = WINDOW_WIDTH;
        rect_.min.x      = WINDOW_WIDTH - width;
        rect_.velocity.x = -rect_.velocity.x;
    }

    if (rect_.min.y < 0)
    {
        rect_.min.y      = 0;
        rect_.max.y      = height;
        rect_.velocity.y = -rect_.velocity.y;
    }
    else if (rect_.max.y > WINDOW_HEIGHT)
    {
        rect_.max.y      = WINDOW_HEIGHT;
        rect_.min.y      = WINDOW_HEIGHT - height;
        rect_.velocity.y = -rect_.velocity.y;
    }
}

void MoveZigZag(Rect& rect_)
{
    const float width  = rect_.max.x - rect_.min.x;
    const float height = rect_.max.y - rect_.min.y;

    rect_.min += rect_.velocity * animationSpeed;
    rect_.max += rect_.velocity * animationSpeed;

    if (rect_.min.x < 0)
    {
        rect_.min.x      = 0;
        rect_.max.x      = width;
        rect_.velocity.x = -rect_.velocity.x;
        rect_.velocity.y = std::abs(rect_.velocity.y);
    }
    else if (rect_.max.x > WINDOW_WIDTH)
    {
        rect_.max.x      = WINDOW_WIDTH;
        rect_.min.x      = WINDOW_WIDTH - width;
        rect_.velocity.x = -rect_.velocity.x;
        rect_.velocity.y = -std::abs(rect_.velocity.y);
    }

    if (rect_.min.y < 0)
    {
        rect_.min.y      = 0;
        rect_.max.y      = height;
        rect_.velocity.y = std::abs(rect_.velocity.y);
    }
    else if (rect_.max.y > WINDOW_HEIGHT)
    {
        rect_.max.y = WINDOW_HEIGHT;
        rect_.min.y = WINDOW_HEIGHT - height;
        rect_.velocity.y = -std::abs(rect_.velocity.y);
    }
}

void ChangeSize(Rect& rect_)
{
    const float centerX = (rect_.min.x + rect_.max.x) / 2.0f;
    const float centerY = (rect_.min.y + rect_.max.y) / 2.0f;

    const float widthFactor  = 0.8f + (static_cast<float>(std::rand()) / RAND_MAX * 0.4f);
    const float heightFactor = 0.8f + (static_cast<float>(std::rand()) / RAND_MAX * 0.4f);

    float halfWidth  = (rect_.max.x - rect_.min.x) * widthFactor / 2.0f;
    float halfHeight = (rect_.max.y - rect_.min.y) * heightFactor / 2.0f;

    if (centerX - halfWidth < 0)
    {
        halfWidth = centerX;
    }
    if (centerX + halfWidth > WINDOW_WIDTH)
    {
        halfWidth = WINDOW_WIDTH - centerX;
    }

    if (centerY - halfHeight < 0)
    {
        halfHeight = centerY;
    }
    if (centerY + halfHeight > WINDOW_HEIGHT)
    {
        halfHeight = WINDOW_HEIGHT - centerY;
    }

    rect_.min.x = centerX - halfWidth;
    rect_.max.x = centerX + halfWidth;
    rect_.min.y = centerY - halfHeight;
    rect_.max.y = centerY + halfHeight;
}

void ChangeColor(Rect& rect_)
{
    rect_.color = { static_cast<float>(std::rand()) / RAND_MAX,
                       static_cast<float>(std::rand()) / RAND_MAX,
                       static_cast<float>(std::rand()) / RAND_MAX };
}

void MoveImitate(Rect& rect_)
{
    // Non-leader rectangles follow the leader
    const Rect& leader = rects[leaderRectIndex];

    // Calculate centers
    const glm::vec2 rectCenter = (rect_.min + rect_.max) * 0.5f;
    const glm::vec2 leaderCenter = (leader.min + leader.max) * 0.5f;

    // Calculate direction vector from this rectangle to the leader
    glm::vec2 direction = leaderCenter - rectCenter;

    // Only move if we're not too close to the leader
    if (glm::length(direction) > 5.0f)
    {
        direction = glm::normalize(direction);

        // Move toward the leader
        const float followSpeed = animationSpeed * 0.7f; // Slightly slower than the leader
        const glm::vec2 movement = direction * followSpeed;

        // Update rectangle position
        const float width = rect_.max.x - rect_.min.x;
        const float height = rect_.max.y - rect_.min.y;

        rect_.min += movement;
        rect_.max += movement;

        // Keep rectangle within window bounds
        if (rect_.min.x < 0)
        {
            rect_.min.x = 0;
            rect_.max.x = width;
        }
        else if (rect_.max.x > WINDOW_WIDTH)
        {
            rect_.max.x = WINDOW_WIDTH;
            rect_.min.x = WINDOW_WIDTH - width;
        }

        if (rect_.min.y < 0)
        {
            rect_.min.y = 0;
            rect_.max.y = height;
        }
        else if (rect_.max.y > WINDOW_HEIGHT)
        {
            rect_.max.y = WINDOW_HEIGHT;
            rect_.min.y = WINDOW_HEIGHT - height;
        }
    }
}


