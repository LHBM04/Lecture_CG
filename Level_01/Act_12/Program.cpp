#include "Program.h"

#include <format>
#include <fstream>
#include <iostream>
#include <ostream>

#include <glad/glad.h>

#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

#include <spdlog/spdlog.h>

#include "Shader.h"

Program::Program() noexcept
{
    // GLFW 초기화 및 OpenGL 컨텍스트 생성
    {
        SPDLOG_INFO("Initializing OpenGL context...");

        if (!glfwInit())
        {
            SPDLOG_CRITICAL("Failed to initialize GLFW");
            std::exit(EXIT_FAILURE);
        }

        constexpr unsigned int CONTEXT_VERSION_MAJOR = 4;
        constexpr unsigned int CONTEXT_VERSION_MINOR = 5;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        constexpr unsigned int      WINDOW_WIDTH  = 800;
        constexpr unsigned int      WINDOW_HEIGHT = 600;
        constexpr const char* const WINDOW_TITLE  = "Level 01 - Act 12";

        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);

        if (!window)
        {
            SPDLOG_CRITICAL("Failed to create GLFW window");
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }

        glfwSetWindowPos(window, 100, 100);
        glfwMakeContextCurrent(window);

        SPDLOG_INFO("Initialized OpenGL context successfully!");
    }
    // GLAD 초기화
    {
        SPDLOG_INFO("Initialized OpenGL successfully...");

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            SPDLOG_CRITICAL("Failed to initialize GLAD");
            glfwDestroyWindow(window);
            window = nullptr;
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }

        SPDLOG_INFO("Initialized OpenGL successfully!");
    }
    // 셰이더 생성
    {
        const std::string vertexShaderFile   = GetFile("Vertex.glsl");
        const char* const vertexShaderSource = vertexShaderFile.c_str();

        const std::string fragmentShaderFile   = GetFile("Fragment.glsl");
        const char* const fragmentShaderSource = fragmentShaderFile.c_str();

        shader = new Shader(vertexShaderSource, fragmentShaderSource);
    }
}

Program::~Program() noexcept
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

void Program::Run() noexcept
{
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->Use();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    SPDLOG_INFO("Close the application.");
}

std::string GetFile(const std::filesystem::path& path_)
{
    std::ifstream file(path_, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        return "";
    }

    file.seekg(0, std::ios::end);
    const long long size = file.tellg();
    if (size == -1)
    {
        return "";
    }

    std::string result(size, '\0');

    file.seekg(0, std::ios::beg);
    file.read(result.data(), size);

    return result;
}
