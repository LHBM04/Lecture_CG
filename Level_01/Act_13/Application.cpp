#include "Application.h"

#include <fstream>
#include <memory>
#include <random>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "glm/gtc/type_ptr.hpp"

#include <spdlog/spdlog.h>

#include "Shader.h"
#include "Shape.h"

/**
 * @brief GLFW 윈도우 핸들.
 */
constinit GLFWwindow* window = nullptr;

/**
 * @brief 마우스 위치.
 */
constinit glm::vec2 mousePosition = {0.0f, 0.0f};

/**
 * @brief 배경색
 */
constinit glm::vec3 backgroundColor = {0.1f, 0.1f, 0.1f};

/**
 * @brief 시간 배율.
 */
constinit float timeScale = 1.0f;

/**
 * @brief 애플리케이션 내에서 사용할 셰이더.
 */
constinit std::unique_ptr<Shader> shader = nullptr;

/**
 * @brief 난수 생성기.
 */
std::random_device rd;

/**
 * @brief 난수 생성 엔진.
 */
std::mt19937 gen(rd());

/**
 * @brief 애플리케이션 실행 직후 월드 내 포함될 도형의 개수.
 */
constexpr std::size_t INITIAL_SHAPE_COUNTS = 10;

/**
 * @brief 월드 내 모든 도형들.
 */
std::vector<std::unique_ptr<Shape>> shapes;

/**
 * @brief 현재 선택된 도형.
 */
constinit Shape* selectedShape = nullptr;

void Initialize() noexcept
{
    SPDLOG_INFO("Initializing GLFW...");
    {
        if (!glfwInit())
        {
            SPDLOG_CRITICAL("Failed to initialize GLFW");
            return;
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
            SPDLOG_CRITICAL("Failed to create GLFW window\n");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetKeyCallback(window, OnKeyInteracted);
        glfwSetMouseButtonCallback(window, OnButtonInteracted);
        glfwSetCursorPosCallback(window, OnCursorMoved);
    }
    SPDLOG_INFO("GLFW initialized successfully");

    SPDLOG_INFO("Initializing GLAD...");
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            SPDLOG_CRITICAL("Failed to initialize GLAD");
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(OnDebugMessage, nullptr);
    }
    SPDLOG_INFO("GLAD initialized successfully");

    const std::string vertexShaderFile   = GetFile("Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile   = GetFile("Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
    shader->Use();

    glm::mat4 projection = glm::ortho(0.0f,
                                      static_cast<float>(WINDOW_WIDTH),
                                      0.0f,
                                      static_cast<float>(WINDOW_HEIGHT),
                                      -1.0f, 1.0f);

    const GLint projLoc = glGetUniformLocation(shader->GetProgramID(), "u_Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Run() noexcept
{
    if (!window)
    {
        SPDLOG_CRITICAL("GLFW window is not initialized");
        std::exit(EXIT_FAILURE);
    }

    Reset();

    while (!glfwWindowShouldClose(window))
    {
        Update();
        Render();
    }
}

void Update() noexcept
{
    glfwPollEvents();

    static float lastTime = static_cast<float>(glfwGetTime());

    const float currentTime = static_cast<float>(glfwGetTime());
    const float deltaTime   = (currentTime - lastTime) * timeScale;

    for (const std::unique_ptr<Shape>& shape : shapes)
    {
        shape->Update(deltaTime);
    }

    lastTime = currentTime;
}

void Render() noexcept
{
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->Use();

    for (const std::unique_ptr<Shape>& shape : shapes)
    {
        shape->Render(*(shader.get()));
    }

    glfwSwapBuffers(window);
}

void Reset() noexcept
{
    if (shapes.size() > 0)
    {
        shapes.clear();
    }

    for (std::size_t i = 0; i < INITIAL_SHAPE_COUNTS; ++i)
    {
        std::uniform_int_distribution<int> distType(static_cast<int>(Shape::Type::Dot), static_cast<int>(Shape::Type::Pentagon));
        const Shape::Type type = static_cast<Shape::Type>(distType(gen));

        std::uniform_real_distribution<float> distPosition(300.0f, 500.0f);
        const glm::vec2 position{distPosition(gen), distPosition(gen)};

        shapes.push_back(std::make_unique<Shape>(type, position));
    }
}

void OnKeyInteracted(GLFWwindow* window_,
              int         key_,
              int         scancode_,
              int         action_,
              int         mods_) noexcept
{
    if (action_ != GLFW_PRESS)
    {
        return;
    }

    switch (key_)
    {
        case GLFW_KEY_C:
        {
            Reset();
            break;
        }
        case GLFW_KEY_UP:
        {
            if (constexpr float MAX_TIME_SCALE = 30.0f; (timeScale += 1.0f) > MAX_TIME_SCALE)
            {
                timeScale = MAX_TIME_SCALE;
                SPDLOG_WARN("[Warning] Time scale cannot be greater than {:.0f}.", MAX_TIME_SCALE);
                break;
            }

            SPDLOG_INFO("Time scale set to {:.1f}.", timeScale);
            break;
        }
        case GLFW_KEY_DOWN:
        {
            if (constexpr float MIN_TIME_SCALE = 1.0f; (timeScale -= 1.0f) < MIN_TIME_SCALE)
            {
                timeScale = MIN_TIME_SCALE;
                SPDLOG_WARN("Time scale cannot be less than or equal to zero.");
                break;
            }

            SPDLOG_INFO("Time scale set to {:.1f}.", timeScale);
            break;
        }
        case GLFW_KEY_S: [[fallthrough]];
        case GLFW_KEY_ENTER:
        {
            static float previousTimeScale = timeScale;

            if (timeScale != 0.0f)
            {
                previousTimeScale = timeScale;
                timeScale = 0.0f;
                SPDLOG_INFO("Animation paused.");
            }
            else
            {
                timeScale = previousTimeScale;
                SPDLOG_INFO("Animation resumed.");
            }

            break;
        }
        case GLFW_KEY_Q:
        {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
            break;
        }
        default:
        {
            break;
        }
    }
}

void OnButtonInteracted(GLFWwindow* window_,
                 int         button_,
                 int         action_,
                 int         mods_) noexcept
{
    if (button_ == GLFW_MOUSE_BUTTON_1)
    {
        if (action_ == GLFW_PRESS)
        {
            for (const std::unique_ptr<Shape>& shape : shapes)
            {
                if (shape->IsInteracted(mousePosition))
                {
                    selectedShape = shape.get();
                    SPDLOG_INFO("The shape is selected.");
                    break;
                }
            }
        }
        else if (action_ == GLFW_RELEASE)
        {
            if (selectedShape != nullptr)
            {
                for (const std::unique_ptr<Shape>& shape : shapes)
                {
                    if (shape.get() == selectedShape)
                    {
                        continue;
                    }

                    if (shape->IsInteracted(*(selectedShape)))
                    {
                        shape->NextTo(*(selectedShape));
                        selectedShape->Destroy();

                        break;
                    }
                }

                std::erase_if(shapes, [](const std::unique_ptr<Shape>& shape_)
                {
                    return shape_->ShouldDestroy();
                });

                selectedShape = nullptr;
                SPDLOG_INFO("The shape is released.");
            }
        }
    }
}

void OnCursorMoved(GLFWwindow* window_,
            double      x_,
            double      y_) noexcept
{
    double x, y;
    glfwGetCursorPos(window_, &x, &y);

    mousePosition.x = static_cast<float>(x);
    mousePosition.y = static_cast<float>(WINDOW_HEIGHT - y_);

    if (selectedShape != nullptr)
    {
        selectedShape->SetPosition(mousePosition);
        SPDLOG_INFO("Shape moved to ({:.1f}, {:.1f})", mousePosition.x, mousePosition.y);
    }

}

void OnDebugMessage(GLenum      source_,
                    GLenum      type_,
                    GLuint      id_,
                    GLenum      severity_,
                    GLsizei     length_,
                    const char* message_,
                    const void* userParam_) noexcept
{
    if (type_ == GL_DEBUG_TYPE_ERROR)
    {
        SPDLOG_ERROR("GL ERROR: type = 0x{:x}, severity = 0x{:x}, message = {}", type_, severity_, message_);
    }
    else if (type_ == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
    {
        SPDLOG_WARN("GL DEPRECATED: type = 0x{:x}, severity = 0x{:x}, message = {}", type_, severity_, message_);
    }
    else if (type_ == GL_DEBUG_TYPE_PERFORMANCE)
    {
        SPDLOG_WARN("GL PERFORMANCE: type = 0x{:x}, severity = 0x{:x}, message = {}", type_, severity_, message_);
    }
    else
    {
        SPDLOG_INFO("GL INFO: type = 0x{:x}, severity = 0x{:x}, message = {}", type_, severity_, message_);
    }
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
