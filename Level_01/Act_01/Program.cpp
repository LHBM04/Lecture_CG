#include <cstdio>
#include <iostream>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

/**
 * @brief 윈도우가 렌더될 떄 호출됩니다.
 */
void OnDisplay();

/**
 * @brief 윈도우의 크기가 변경될 때 호출됩니다.
 *
 * @param width_ 윈도우의 가로 크기.
 * @param height_ 윈도우의 세로 크기.
 */
void OnResize(const int width_,
              const int height_);

/**
 * @brief 키가 눌렸을 때 호출됩니다.
 *
 * @param key_ 눌린 키 값.
 * @param x_ 마우스의 X 좌표.
 * @param y_ 마우스의 Y 좌표.
 */
void OnKeyDown(const unsigned char key_,
               const int           x_,
               const int           y_);

/**
 * @brief 매 프레임마다 호출됩니다.
 */
void OnUpdate(const int value_);

static float r = 255.0f;
static float g = 255.0f;
static float b = 255.0f;

static bool trigger = false;

int main(int    argc_,
         char** argv_)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

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
    glutReshapeFunc(OnResize);
    glutKeyboardFunc(OnKeyDown);
    glutMainLoop();
}

void OnDisplay()
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

void OnResize(const int width_,
              const int height_)
{
    glViewport(0, 0, width_, height_);
}

void OnKeyDown(const unsigned char key_,
               const int           x_,
               const int           y_)
{
    switch (key_)
    {
        case 'c':
        {
            if (trigger)
            {
                break;
            }

            r = 0.0f;
            g = 1.0f;
            b = 1.0f;
            break;
        }
        case 'm':
        {
            if (trigger)
            {
                break;
            }

            r = 1.0f;
            g = 0.0f;
            b = 1.0f;
            break;
        }
        case 'y':
        {
            if (trigger)
            {
                break;
            }

            r = 1.0f;
            g = 1.0f;
            b = 0.0f;
            break;
        }
        case 'a':
        {
            if (trigger)
            {
                break;
            }

            r = static_cast<float>(std::rand() % 256) / 255.0f;
            g = static_cast<float>(std::rand() % 256) / 255.0f;
            b = static_cast<float>(std::rand() % 256) / 255.0f;

            break;
        }
        case 'w':
        {
            if (trigger)
            {
                break;
            }

            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
            break;
        }
        case 'k':
        {
            if (trigger)
            {
                break;
            }

            r = 0.0f;
            g = 0.0f;
            b = 0.0f;
            break;
        }
        case 't':
        {
            trigger = true;
            glutTimerFunc(50, OnUpdate, 1);
            break;
        }
        case 's':
        {
            trigger = false;
            break;
        }
        case 'q':
        {
            glutLeaveMainLoop();
        }
        default:
        {
            break;
        }
    }

    glutPostRedisplay();
}

void OnUpdate(const int value_)
{
    if (trigger)
    {
        r = static_cast<float>(std::rand() % 256) / 255.0f;
        g = static_cast<float>(std::rand() % 256) / 255.0f;
        b = static_cast<float>(std::rand() % 256) / 255.0f;

        glutPostRedisplay();
        glutTimerFunc(50, OnUpdate, 1);
    }
}