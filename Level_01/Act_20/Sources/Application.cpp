#include "Application.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

int Application::Run(const Application::Specification& specification_) noexcept
{
	specification = specification_;

	if (!InitWindow())
	{
		spdlog::critical("Failed to initialize Window!");
		return false;
	}

	if (!InitGraphics())
	{
		spdlog::critical("Failed to initialize Graphics!");
		return false;
	}

	specification.onStart();

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static float lastTime = static_cast<float>(glfwGetTime());
		
		const float curTime   = static_cast<float>(glfwGetTime());
		const float deltaTime = curTime - lastTime;
		
		lastTime = curTime;
		
		specification.onUpdate(deltaTime);
		specification.onRender();
		
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	specification.onClose();
	return true;
}

void Application::Quit() noexcept
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool Application::InitWindow() noexcept
{
	if (glfwInit() != GLFW_TRUE)
	{
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, specification.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, specification.minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(
		specification.width, 
		specification.height,
		specification.name.c_str(), 
		nullptr, 
		nullptr
	);

	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	return true;
}

bool Application::InitGraphics() noexcept
{
	if (glewInit() != GLEW_OK)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	return true;
}

GLFWwindow* Application::window = nullptr;

Application::Specification Application::specification = {};