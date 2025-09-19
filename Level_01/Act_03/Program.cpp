#include <ctime>
#include <format>
#include <iostream>
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
void OnDisplay();

/**
 * @brief 키보드와 상호작용할 때 호출됩니다.
 *
 * @param key_ 눌린 키.
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
void OnKeyDown(unsigned char key_, int x_, int y_);

/**
 * @brief 마우스와 상호작용할 때 호출됩니다.
 *
 * @param button_ 버튼.
 * @param state_ 버튼 상태.
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
void OnMouseClick(int button_, int state_, int x_, int y_);

/**
 * @brief 마우스가 움직일 때 호출됩니다.
 *
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
void OnCursorMoved(int x_, int y_);

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
static Rect* currentRect;

int main(int   argc_,
         char** argv_)
{
    std::srand(static_cast<float>(std::time(nullptr)));

    glutInit(&argc_, argv_);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Program");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        return EXIT_FAILURE;
    }

    glutDisplayFunc(OnDisplay);
    glutKeyboardFunc(OnKeyDown);
    glutMouseFunc(OnMouseClick);
    glutMotionFunc(OnCursorMoved);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void OnDisplay()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& [min, max, color] : rects)
    {
        glColor3f(color.r, color.g, color.b);
        glRectf(min.x, min.y, max.x, max.y);
    }

    glutSwapBuffers();
}


void OnKeyDown(unsigned char key_, int x_, int y_)
{
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

            const glm::vec2 pos   = { static_cast<float>(std::rand() % 800),
                                      static_cast<float>(std::rand() % 600) };
            const glm::vec2 min   = { pos.x - Rect::SCALE_MULTIPLIER / 2.0f,
                                      pos.y - Rect::SCALE_MULTIPLIER / 2.0f };
            const glm::vec2 max   = { pos.x + Rect::SCALE_MULTIPLIER / 2.0f,
                                      pos.y + Rect::SCALE_MULTIPLIER / 2.0f };
            const glm::vec3 color = { static_cast<float>(std::rand() % 256) / 255.0f,
                                      static_cast<float>(std::rand() % 256) / 255.0f,
                                      static_cast<float>(std::rand() % 256) / 255.0f };

            rects.push_back({ min, max, color });
            currentCreateCounts++;

            std::cout << std::format("[Info] new rectangle created at ({:.1f}, {:.1f}) ", pos.x, pos.y);
            std::cout << std::format("Total Counts: {:d}\n", currentCreateCounts);

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

void OnMouseClick(int button_, int state_, int x_, int y_)
{
    const float mouseX = static_cast<float>(x_);
    const float mouseY = static_cast<float>(600 - y_);

    if (button_ == GLUT_LEFT_BUTTON)
    {
        if (state_ == GLUT_DOWN)
        {
            for (auto& rect : rects)
            {
                if (mouseX >= rect.min.x && mouseX <= rect.max.x &&
                    mouseY >= rect.min.y && mouseY <= rect.max.y)
                {
                    currentRect = &rect;
                    std::cout << "[Info] The rectangle selected!\n";
                    break;
                }
            }
        }
        else if (state_ == GLUT_UP)
        {
            auto toErase = rects.end();

            for (auto iter = rects.begin(); iter != rects.end(); ++iter)
            {
                if (&(*iter) == currentRect)
                {
                    continue;
                }

                if (!(currentRect->max.x < iter->min.x || currentRect->min.x > iter->max.x ||
                      currentRect->max.y < iter->min.y || currentRect->min.y > iter->max.y))
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
    else if (button_ == GLUT_RIGHT_BUTTON)
    {
        if (state_ == GLUT_DOWN)
        {
            if (rects.size() + 2 >= MAX_RECT_COUNTS)
            {
                std::cerr << "[Oops!] If you devied rectangle, has been overflow!\n";
                return;
            }

            auto toErase = rects.end();

            for (auto iter = rects.begin(); iter != rects.end(); ++iter)
            {
                if (mouseX >= iter->min.x && mouseX <= iter->max.x &&
                    mouseY >= iter->min.y && mouseY <= iter->max.y)
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

    glutPostRedisplay();
}

void OnCursorMoved(int x_, int y_)
{
    if (currentRect == nullptr)
    {
        return;
    }

    const float x      = static_cast<float>(x_);
    const float y      = static_cast<float>(600 - y_);
    const float width  = currentRect->max.x - currentRect->min.x;
    const float height = currentRect->max.y - currentRect->min.y;

    currentRect->min.x = x - width  / 2.0f;
    currentRect->min.y = y - height / 2.0f;
    currentRect->max.x = x + width  / 2.0f;
    currentRect->max.y = y + height / 2.0f;

    std::cout << std::format("[Info] Current Position: ({:.1f}, {:.1f})\n", x, y);

    glutPostRedisplay();
}