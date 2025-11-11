#include "Application.h"

#include <gl/glew.h>

#include <gl/freeglut_std.h>
#include <gl/freeglut_ext.h>

#include <spdlog/spdlog.h>

#include "Input.h"
#include "Time.h"

int Application::Run(const Application::Specification& specification_) noexcept
{
    specification = specification_;

    // 윈도우 초기화.
    {
        glutInit(&__argc, __argv);

        glutInitContextVersion(specification.majorVersion, specification.minorVersion);
        glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
        glutInitContextFlags(GLUT_DEBUG);

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(specification.width, specification.height);
        glutInitWindowPosition(0, 0);

        if (!glutCreateWindow(specification.name.c_str()))
        {
            spdlog::critical("Failed to initialize Window!");
            return -1;
        }
    }

    // 그래픽스 초기화.
    {
        if (glewInit() != GLEW_OK)
        {
            spdlog::error("Failed to initialize Graphics!");
            return -1;
        }

        glEnable(GL_CULL_FACE);
        //glFrontFace(GL_CW);
        // glCullFace(GL_BACK);

        glEnable(GL_DEPTH_TEST);
    }

    glutIdleFunc(OnUpdate);

    glutDisplayFunc(OnRender);
    glutReshapeFunc(OnReshape);

    glutKeyboardFunc(Input::OnKeyPressed);
    glutKeyboardUpFunc(Input::OnKeyReleased);
    glutSpecialFunc(Input::OnSpecialKeyPressed);
    glutSpecialUpFunc(Input::OnSpecialKeyReleased);
    glutMouseFunc(Input::OnMouseClicked);
    glutMotionFunc(Input::OnMouseMoved);
    glutPassiveMotionFunc(Input::OnMouseMoved);

    Time::Initialize();

    if (specification.fps > 0)
    {
        Time::SetUnscaledFixedDeltaTime(1.0f / static_cast<float>(specification.fps));
    }
    else
    {
        Time::SetUnscaledFixedDeltaTime(0.0f);
    }

    if (specification.onStart)
    {
        specification.onStart();
    }

    glutMainLoop();

    return 0;
}

void Application::Quit() noexcept
{
    glutLeaveMainLoop();
}

void Application::OnUpdate() noexcept
{
    Time::Update();

    fixedUpdateTime += Time::GetDeltaTime();

    if (const float fixedDeltaTime = Time::GetFixedDeltaTime(); fixedDeltaTime > 0.0f)
    {
        while (fixedUpdateTime >= fixedDeltaTime)
        {
            if (specification.onFixedUpdate)
            {
                specification.onFixedUpdate();
            }

            fixedUpdateTime -= fixedDeltaTime;
        }
    }

    if (specification.onUpdate)
    {
        specification.onUpdate();
    }

    Input::Update();

    glutPostRedisplay();
}

void Application::OnRender() noexcept
{
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (specification.onRender)
    {
        specification.onRender();
    }

    if (specification.onLateUpdate)
    {
        specification.onLateUpdate();
    }

    glutSwapBuffers();
}

void Application::OnReshape(const int width_,
                            const int height_) noexcept
{
    specification.width  = width_;
    specification.height = height_;

    glViewport(0, 0, width_, height_);
}

Application::Specification Application::specification = {};

float Application::fixedUpdateTime = 0.0f;