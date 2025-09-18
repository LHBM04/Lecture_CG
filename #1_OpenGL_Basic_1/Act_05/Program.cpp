#include <algorithm>
#include <iostream>
#include <vector>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>

struct Rect
{
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec3 color;
};

static bool IsCollide(const Rect& left_,
                      const Rect& right_) noexcept;

/**
 * @brief 어플리케이션을 그릴 때 호출됩니다.
 */
static void OnDisplay();

/**
 * @brief 키보드의 키가 눌렸을 때 호출됩니다.
 *
 * @param key_ 눌린 키.
 * @param x_ 마우스의 X 좌표.
 * @param y_ 마우스의 Y 좌표.
 */
static void OnKeyDowned(unsigned char key_,
                        int           x_,
                        int           y_);

/**
 * @brief 마우스 버튼이 클릭되었을 때 호출됩니다.
 *
 * @param button_ 클릭된 버튼.
 * @param state_ 버튼 상태.
 * @param x_ 마우스의 X 좌표.
 * @param y_ 마우스의 Y 좌표.
 */
static void OnButtonClicked(int button_,
                            int state_,
                            int x_,
                            int y_);

/**
 * @brief 마우스의 커서가 움직일 때 호출됩니다.
 *
 * @param x_ 마우스의 X 좌표.
 * @param y_ 마우스의 Y 좌표.
 */
static void OnCursorMoved(int x_,
                          int y_);

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
static constexpr const char* const WINDOW_TITLE = "Eraser Simulator";

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
    glutKeyboardFunc(OnKeyDowned);
    glutMouseFunc(OnButtonClicked);
    glutMotionFunc(OnCursorMoved);

    glutMainLoop();

    return EXIT_SUCCESS;
}

bool IsCollide(const Rect& left_,
               const Rect& right_) noexcept
{
    return !(left_.pos.x > right_.pos.x + right_.size.x ||
             left_.pos.x + left_.size.x < right_.pos.x  ||
             left_.pos.y > right_.pos.y + right_.size.y ||
             left_.pos.y + left_.size.y < right_.pos.y);
}

void OnDisplay()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto&[position, size, color] : rects)
    {
        glColor3f(color.r, color.g, color.b);
        glRectf(position.x, position.y,
                position.x + size.x, position.y + size.y);
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

    glutSwapBuffers();
}

void OnKeyDowned(unsigned char key_,
                 int           x_,
                 int           y_)
{
    switch (key_)
    {
        case 'r': [[fallthrough]];
        case 'R':
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
            shouldEnableEraser = true;

            const float fx = static_cast<float>(x_);
            const float fy = static_cast<float>(WINDOW_HEIGHT - y_);

            eraser.pos = {fx, fy};
            eraser.size= {20, 20};
            eraser.color = {1.0f, 1.0f, 1.0f};
        }
        else if (state_ == GLUT_UP)
        {
            shouldEnableEraser = false;
            currentEraseCounts = 0;
        }
    }
    else if (button_ == GLUT_RIGHT_BUTTON)
    {
        if (state_ == GLUT_DOWN)
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

    glutPostRedisplay();
}

void OnCursorMoved(int x_,
                   int y_)
{
    if (!shouldEnableEraser)
    {
        return;
    }

    const float fx = static_cast<float>(x_);
    const float fy = static_cast<float>(WINDOW_HEIGHT - y_);

    eraser.pos = {fx, fy};

    using Iterator = std::vector<Rect>::iterator;
    for (Iterator iter = rects.begin(); iter != rects.end(); ++iter)
    {
        if (IsCollide(eraser, *iter))
        {
            eraser.color = iter->color;
            currentEraseCounts++;

            rects.erase(iter);
            break;
        }
    }

    glutPostRedisplay();
}