#include "Application.h"

#include "Input.h"

int Application::Run(const Application::Configuration& configuration_) noexcept
{
	configuration = configuration_;
	
	if (!Initialize())
	{
		return -1;
	}

	Start();

	// 3. 메인 루프
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		static float previousTime = static_cast<float>(glfwGetTime());

		const float currentTime = static_cast<float>(glfwGetTime());
		const float deltaTime   = currentTime - previousTime;

		Update(deltaTime);
		Render();

		Input::Update();

		previousTime = currentTime;
	}

	Close();

	return 0;
}

void Application::Quit(const int exitCode_) noexcept
{
	std::exit(exitCode_);
}

bool Application::Initialize() noexcept
{
	// 1. GLFW 초기화 및 윈도우 생성
	{
		if (!glfwInit())
		{
			std::println("GLFW 초기화에 실패했습니다.");
			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, configuration.shouldResizable ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_DECORATED, configuration.shouldDecorate ? GLFW_TRUE : GLFW_FALSE);

		window = glfwCreateWindow(configuration.width, configuration.height, configuration.title, nullptr, nullptr);
		if (!window)
		{
			std::println("GLFW 윈도우 생성에 실패했습니다.");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);

		glfwSetKeyCallback(window, [](GLFWwindow*, int key_, int scancode_, int action_, int mod_) {
			Input::OnKeyInteract(key_, scancode_, action_, mod_);
		});
		glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button_, int action_, int mod_) {
			Input::OnMouseButtonInteract(button_, action_, mod_);
		});
		glfwSetCursorPosCallback(window, [](GLFWwindow*, double x_, double y_) {
			Input::OnCursorMove(static_cast<int>(x_), static_cast<int>(y_));
		});
	}
	// 2. GLAD 초기화
	{
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			std::println("GLAD 초기화에 실패했습니다.");
			glfwDestroyWindow(window);
			glfwTerminate();
			return false;
		}

		glEnable(GL_DEPTH_TEST);

		// glEnable(GL_CULL_FACE);
		// glCullFace(GL_BACK);
	}

	// 3. VSync 설정
	{
		if (configuration.shouldVSync)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
	}

	return true;
}

void Application::Start() noexcept
{
	if (configuration.onStart)
	{
		configuration.onStart();
	}
}

void Application::Update(const float deltaTime_) noexcept
{
	if (configuration.onUpdate)
	{
		configuration.onUpdate(deltaTime_);
	}
}

void Application::Render() noexcept
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (configuration.onRender)
	{
		configuration.onRender();
	}

	glfwSwapBuffers(window);
}

void Application::Close() noexcept
{
	if (configuration.onClose)
	{
		configuration.onClose();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

Application::Configuration Application::configuration = { };

GLFWwindow* Application::window = nullptr;