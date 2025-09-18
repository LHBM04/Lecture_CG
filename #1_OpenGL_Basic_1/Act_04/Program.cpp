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
 * @brief 지정한 두 사각형이 충돌하는지 여부를 반환합니다.
 * @param left_ 지정할 첫 번째 사각형.
 * @param right_ 지정할 두 번째 사각형.
 * @return bool 두 사각형이 충돌하면 true, 그렇지 않으면 false.
 */
[[nodiscard]]
bool IsCollide(const Rect& left_,
               const Rect& right_) noexcept;

/**
 * @brief 지정한 위치가 지정한 영역 내부에 포함되는지 여부를 반환합니다.
 * @param area_ 지정할 영역.
 * @param point_ 지정할 위치.
 * @return
 */
[[nodiscard]]
bool IsContain(const glm::vec2& point_,
               const Rect&      area_) noexcept;

/**
 * @brief 지정한 사각형을 그립니다.
 *
 * @param rect_ 그릴 사각형.
 * @param shouldFill_ 내부 채색 여부.
 */
void Draw(const Rect& rect_,
          bool        shouldFill_) noexcept;

/**
 * @brief 윈도우가 그려질 때 호출됩니다.
 */
void OnDisplay() noexcept;

/**
 * @brief 키가 눌렸을 때 호출됩니다.
 *
 * @param key_ 눌린 키.
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
void OnKeyPressed(unsigned char key_,
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
void OnButtonClicked(int button_,
                     int state_,
                     int x_,
                     int y_);

/**
 * @brief 마우스 커서가 움직일 때 호출됩니다.
 *
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
void OnCursorMoved(int x_,
                   int y_);

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
 * @brief 현재 드래그 중인지에 대한 여부.
 */
static bool isDragging = false;

/**
 * @brief 추가할 새 사각형.
 */
static Rect* newRect = nullptr;

/**
 * @brief 드래그 시작 X 좌표.
 */
static glm::vec2 startPosition = {0.0f, 0.0f};

/**
 * @brief 드래그 종료 X 좌표.
 */
static glm::vec2 endPosition   = {0.0f, 0.0f};

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
    glutKeyboardFunc(OnKeyPressed);
    glutMouseFunc(OnButtonClicked);
    glutMotionFunc(OnCursorMoved);
    glutMainLoop();
}

bool IsCollide(const Rect& left_, const Rect& right_) noexcept
{
    return !(left_.min.x > right_.max.x ||
             left_.max.x < right_.min.x ||
             left_.min.y > right_.max.y ||
             left_.max.y < right_.min.y);
}

bool IsContain(const glm::vec2& point_, const Rect& area_) noexcept
{
    return (point_.x >= area_.min.x && point_.x <= area_.max.x &&
            point_.y >= area_.min.y && point_.y <= area_.max.y);
}

void Draw(const glm::vec2& min_,
          const glm::vec2& max_,
          const glm::vec3& color_,
          bool  onlyLine_) noexcept
{
    glColor3f(color_.r, color_.g, color_.b);

    glBegin(onlyLine_ ? GL_LINE_LOOP : GL_QUADS);

    glVertex2f(min_.x, min_.y);
    glVertex2f(max_.x, min_.y);
    glVertex2f(max_.x, max_.y);
    glVertex2f(min_.x, max_.y);

    glEnd();
}

void Draw(const Rect& rect_) noexcept
{
    Draw(rect_.min, rect_.max, rect_.color, false);
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

    if (isDragging)
    {
        glm::vec2 center = startPosition;
        glm::vec2 drag   = endPosition - startPosition;
        glm::vec2 half   = glm::abs(drag) * 2.0f;

        glm::vec2 min   = center - half;
        glm::vec2 max   = center + half;
        glm::vec3 color = { 1.0f, 1.0f, 1.0f };

        Draw(min, max, color, false);
    }

    glutSwapBuffers();
}

void OnKeyPressed(unsigned char key_,
                  int           x_,
                  int           y_)
{
    switch (key_)
    {
        case '1':
        {
            break;
        }
        case '2':
        {
            break;
        }
        case '3':
        {
            break;
        }
        case '4':
        {
            break;
        }
        case '5':
        {
            break;
        }
        case 's': [[fallthrough]];
        case 'S':
        {
            break;
        }
        case 'm': [[fallthrough]];
        case 'M':
        {
            break;
        }
        case 'r': [[fallthrough]];
        case 'R':
        {
            rects.pop_back();
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
            isDragging = true;

            startPosition = { static_cast<float>(x_),
                                 static_cast<float>(WINDOW_HEIGHT - y_) };
            endPosition      = startPosition;
        }
        else if (state_ == GLUT_UP)
        {
            isDragging = false;

            const glm::vec2 drag = endPosition - startPosition;
            const glm::vec2 half = glm::abs(drag) * 2.0f;

            const glm::vec2 min = startPosition - half;
            const glm::vec2 max = startPosition + half;

            const glm::vec3 color = { static_cast<float>(std::rand() % 256) / 255.0f,
                                      static_cast<float>(std::rand() % 256) / 255.0f,
                                      static_cast<float>(std::rand() % 256) / 255.0f };

            rects.push_back({ min, max, color });
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
    if (!isDragging)
    {
        return;
    }

    endPosition = { static_cast<float>(x_),
                       static_cast<float>(WINDOW_HEIGHT - y_) };

    glutPostRedisplay();
}