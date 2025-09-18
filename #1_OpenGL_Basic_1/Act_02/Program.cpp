#include <array>
#include <random>
#include <iostream>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>

/**
 * @brief 애플리케이션 타이틀.
 */
static constexpr const char* const WINDOW_TITLE = "OpenGL Program";

/**
 * @brief 애플리케이션 너비.
 */
static constexpr int WINDOW_WIDTH  = 800;

/**
 * @brief 애플리케이션 높이.
 */
static constexpr int WINDOW_HEIGHT = 800;

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
 * @brief 윈도우가 렌더될 떄 호출됩니다.
 */
void OnDisplay();

/**
 * @brief 키보드의 키가 눌렸을 때 호출됩니다.
 *
 * @param key_ 눌린 키.
 * @param x_ 마우스의 X 좌표.
 * @param y_ 마우스의 Y 좌표.
 */
void OnKeyDown(unsigned char key_,
                int           x_,
                int           y_);

/**
 * @brief 마우스의 버튼을 눌렀을 때 호출됩니다.
 *
 * @param button_ 클릭된 버튼.
 * @param state_ 버튼 상태.
 * @param x_ 마우스의 X 좌표.
 * @param y_ 마우스의 Y 좌표.
 */
void OnMouseClick(int button_,
                  int state_,
                  int x_,
                  int y_);

/**
 * @brief 시드 값을 위한 랜덤 디바이스.
 */
static std::random_device rd;

/**
 * @brief 난수 생성 엔진.
 */
static std::mt19937 gen(rd());

/**
 * @brief 난수 생성 범위.
 */
static std::uniform_int_distribution<> distrib(0, 255);

/**
 * @brief 뒤에 올 사각형.
 */
static std::array backs
{
    Rect{ {  0.0f,  0.0f }, { 1.0f, 1.0f }, { 0.25f, 0.0f,  0.0f  } },
    Rect{ { -1.0f,  0.0f }, { 0.0f, 1.0f }, { 0.0f,  0.25f, 0.0f  } },
    Rect{ { -1.0f, -1.0f }, { 0.0f, 0.0f }, { 0.0f,  0.0f,  0.25f } },
    Rect{ {  0.0f, -1.0f }, { 1.0f, 0.0f }, { 0.25f, 0.25f, 0.0f  } }
};

/**
 * @brief 앞에 올 사각형들.
 */
static std::array fronts
{
    Rect{ {  0.0f,  0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
    Rect{ { -1.0f,  0.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
    Rect{ { -1.0f, -1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
    Rect{ {  0.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } }
};

/**
 * @brief 크기 조절 스케일러.
 */
static constexpr float SCALER = 0.05f;

/**
 * @brief 최소 크기.
 */
static constexpr float MIN_SIZE = 0.1f;

/**
 * @brief 최대 크기.
 */
static constexpr float MAX_SIZE = 1.0f;

int main(int    argc_,
         char** argv_)
{
    glutInit(&argc_, argv_);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);

    glutDisplayFunc(OnDisplay);
    glutKeyboardFunc(OnKeyDown);
    glutMouseFunc(OnMouseClick);
    glutMainLoop();
}

void OnDisplay()
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

    glutSwapBuffers();
}

void OnKeyDown(unsigned char key_,
                int           x_,
                int           y_)
{
    if (key_ == 'q' || key_ == 'Q')
    {
        glutLeaveMainLoop();
    }
}


void OnMouseClick(int button_,
             int state_,
             int x_,
             int y_)
{
    if (state_ != GLUT_DOWN)
    {
        return;
    }

    const float mouseX = (static_cast<float>(x_) / WINDOW_WIDTH) * 2.0f - 1.0f;
    const float mouseY = 1.0f - (static_cast<float>(y_) / WINDOW_HEIGHT) * 2.0f;

    bool isInteracted = false;

    for (std::size_t index = 0; index < 4; ++index)
    {
        Rect& front = fronts[index];

        if (mouseX >= front.min.x && mouseX <= front.max.x &&
            mouseY >= front.min.y && mouseY <= front.max.y)
        {
            isInteracted = true;

            if (button_ == GLUT_LEFT_BUTTON)
            {
                front.color =
                {
                    static_cast<float>(distrib(gen)) / 255.0f,
                    static_cast<float>(distrib(gen)) / 255.0f,
                    static_cast<float>(distrib(gen)) / 255.0f
                };

                break;
            }
            if (button_ == GLUT_RIGHT_BUTTON)
            {
                if ((front.max.x - front.min.x > MIN_SIZE * 2.0f) &&
                    (front.max.y - front.min.y > MIN_SIZE * 2.0f))
                {
                    front.min += glm::vec2(SCALER);
                    front.max -= glm::vec2(SCALER);
                }

                break;
            }
        }

        if (!isInteracted)
        {
            Rect& back  = backs[index];

            if (mouseX >= back.min.x && mouseX <= back.max.x &&
                mouseY >= back.min.y && mouseY <= back.max.y)
            {
                if (button_ == GLUT_LEFT_BUTTON)
                {
                    back.color =
                    {
                        static_cast<float>(distrib(gen)) / 255.0f,
                        static_cast<float>(distrib(gen)) / 255.0f,
                        static_cast<float>(distrib(gen)) / 255.0f
                    };

                    break;
                }
                if (button_ == GLUT_RIGHT_BUTTON)
                {
                    if ((front.max.x - front.min.x < MAX_SIZE) &&
                        (front.max.y - front.min.y < MAX_SIZE))
                    {
                        front.min -= glm::vec2(SCALER);
                        front.max += glm::vec2(SCALER);
                    }

                    break;
                }
            }
        }
    }

    glutPostRedisplay();
}