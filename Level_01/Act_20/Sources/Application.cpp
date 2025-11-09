#include "Application.h"

#include <GL/glew.h>

#include <GL/freeglut_std.h>
#include <GL/freeglut_ext.h>

#include <spdlog/spdlog.h>

int Application::Run(const Config& config_) noexcept
{
	config = config_;

	glutInit(&config.argc, config.argv);

	glutInitContextVersion(4, 6);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitContextFlags(GLUT_DEBUG);

	int flag = GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA;

	if 

	if (!config.decorated)
		flag |= GLUT_BORDERLESS | GLUT_CAPTIONLESS;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_CAPTIONLESS);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(0, 0);

	if (!glutCreateWindow("Level 01 - Act 20"))
	{
		spdlog::critical("Failed to initialize Window!");
		return -1;
	}

	if (glewInit() != GLEW_OK)
	{
		spdlog::error("Failed to initialize Graphics!");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	glutTimerFunc(1000 / config.fps, OnTimer, 0);
	glutDisplayFunc(OnDisplay);
	glutCloseFunc(OnClose);

	OnLoad();
	glutMainLoop();

	return 0;
}

void Application::Quit() noexcept
{
	glutLeaveMainLoop();
}

void Application::OnLoad() noexcept
{
	if (config.onStart)
	{
		config.onStart();
	}
}

void Application::OnTimer(int value_) noexcept
{
	if (config.onUpdate)
	{
		config.onUpdate();
	}
	
	glutPostRedisplay();
	glutTimerFunc(1000 / config.fps, OnTimer, 0);
}

void Application::OnDisplay() noexcept
{
	if (config.onRender)
	{
		config.onRender();
	}

	glutSwapBuffers();
}

void Application::OnClose() noexcept
{
	if (config.onQuit)
	{
		config.onQuit();
	}
}

Application::Config Application::config = {};