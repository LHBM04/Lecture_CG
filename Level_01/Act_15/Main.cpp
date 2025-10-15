#include <memory>

#include <glad/glad.h>

#include "Framework/Graphics/Mesh.h"
#include "Framework/Graphics/Model.h"
#include "Framework/Graphics/Shader.h"

#include "Framework/Misc/File.h"
#include "glfw/glfw3.h"
#include "spdlog/spdlog.h"

/**
 * @class Object
 *
 * @brief 3D 객체를 정의합니다.
 */
class Object
{
public:
    /**
     * @brief 생성자.
     */
    explicit Object() noexcept;

    /**
     * @brief 해당 객체를 렌더링합니다.
     */
    void Render() const noexcept;
private:
    /**
     * @brief 해당 객체의 모델.
     */
    std::unique_ptr<Model> model;

    /**
     * @brief 해당 객체의 메쉬.
     */
    std::unique_ptr<Mesh> mesh;

    /**
     * @brief 해당 객체의 셰이더.
     */
    std::unique_ptr<Shader> shader;
};

/**
 * @brief 애플리케이션 너비.
 */
static constexpr unsigned int WINDOW_WIDTH  = 1200;

/**
 * @brief 애플리케이션 높이.
 */
static constexpr unsigned int WINDOW_HEIGHT = 600;

/**
 * @brief 애플리케이션 타이틀.
 */
static constexpr const char* const WINDOW_TITLE = "Level 01 - Act 15";

/**
 * @brief GL 메이저 버전.
 */
static constexpr unsigned char CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief GL 마이너 버전.
 */
static constexpr unsigned char CONTEXT_MINOR_VERSION = 5;

/**
 * @brief GLFW 윈도우 핸들.
 */
static GLFWwindow* window = nullptr;

/**
 * @brief 배경 색상.
 */
static constexpr glm::vec3 backgroundColor = {0.1f, 0.1f, 0.1f};

int main()
{
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              WINDOW_TITLE,
                              nullptr,
                              nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        SPDLOG_CRITICAL("Failed to initialize GLAD\n");
        return -1;
    }

    Object object;

    while (!glfwWindowShouldClose(window))
    {
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // object.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Object::Object() noexcept
{
    // 셰이더 초기화.
    {
        const std::string vertexShaderFile   = ReadFile("Resources/Shaders/Vertex.glsl");
        const char* const vertexShaderSource = vertexShaderFile.c_str();

        const std::string fragmentShaderFile   = ReadFile("Resources/Shaders/Fragment.glsl");
        const char* const fragmentShaderSource = fragmentShaderFile.c_str();

        shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
    }
    // 버퍼 객체 초기화.
    {
        const std::string modelFile   = ReadFile("Resources/Objects/Cube.obj");
        const char* const modelSource = modelFile.c_str();

        model = std::make_unique<Model>(modelSource);
        mesh  = std::make_unique<Mesh>(modelSource);
    }
}

void Object::Render() const noexcept
{
    shader->Use();

    mesh->Render();
}