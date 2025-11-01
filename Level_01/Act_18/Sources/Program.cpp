#include <filesystem>
#include <fstream>
#include <memory>

#include <gl/glew.h>

#include <gl/freeglut_std.h>
#include <gl/freeglut_ext.h>
#include <glfw/glfw3.h>
#include <glm/fwd.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <spdlog/spdlog.h>

#include "Graphics/Shader.h"

#include "Objects/Object.h"
#include "Objects/Axes.h"
#include "Objects/Cube.h"
#include "Objects/Cone.h"

/**
 * @brief 윈도우가 그려질 때 호출됩니다.
 */
static void OnDisplay() noexcept;

/**
 * @brief 윈도우 크기가 변경될 때 호출됩니다.
 *
 * @param width_  새로운 너비.
 * @param height_ 새로운 높이.
 */
static void OnResize(int width_,
                     int height_) noexcept;

/**
 * @brief 키와 상호작용할 때 호출됩니다.
 *
 * @param key_ 눌린 키.
 * @param x_   마우스 커서의 X 좌표.
 * @param y_   마우스 커서의 Y 좌표.
 */
static void OnKeyInteracted(unsigned char key_,
                            int           x_,
                            int           y_) noexcept;

/**
 * @brief 버튼과 상호작용할 떄 호출됩니다.
 *
 * @param button_ 눌린 버튼.
 * @param state_  버튼 상태.
 * @param x_      마우스 커서의 X 좌표.
 * @param y_      마우스 커서의 Y 좌표.
 */
static void OnButtonInteracted(int button_,
                               int state_,
                               int x_,
                               int y_) noexcept;

/**
 * @brief 지정한 경로 내 파일을 읽어옵니다.
 *
 * @param path_ 지정할 경로.
 *
 * @return std::string 파일 내용.
 */
static std::string ReadFile(const std::filesystem::path& path_) noexcept;

/**
 * @brief 애플리케이션 너비.
 */
static constinit int windowWidth = 800;

/**
 * @brief 애플리케이션 높이.
 */
static constinit int WINDOW_HEIGHT = 600;

/**
 * @brief 윈도우 타이틀
 */
static constexpr const char* WINDOW_TITLE = "Level 01 - Act 18";

/**
 * @brief 애플리케이션에서 사용할 전역 셰이더.
 */
static std::unique_ptr<Shader> globalShader = nullptr;

/**
 * @brief 월드 내 큐브 오브젝트.
 */
static std::unique_ptr<Cube> cube = nullptr;

int main(int    argc_,
         char** argv_)
{
    // System Init
    {
        glutInit(&argc_, argv_);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(windowWidth, WINDOW_HEIGHT);
        glutCreateWindow(WINDOW_TITLE);

        glutDisplayFunc(OnDisplay);
        glutReshapeFunc(OnResize);
        glutKeyboardFunc(OnKeyInteracted);
        glutMouseFunc(OnButtonInteracted);

        if (glewInit() != GLEW_OK)
        {
            spdlog::error("Failed to initialize GLEW!");
            return -1;
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_DEPTH_TEST);
    }
    // Resources Loading
    {
        const std::string vertexShaderFile   = ReadFile("Resources/Shaders/Vertex.glsl");
        const char* const vertexShaderSource = vertexShaderFile.c_str();

        const std::string fragmentShaderFile   = ReadFile("Resources/Shaders/Fragment.glsl");
        const char* const fragmentShaderSource = fragmentShaderFile.c_str();

        globalShader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);

        cube = std::make_unique<Cube>();
        cube->rotation = glm::vec3{45.0f, 45.0f, 0.0f};
    }
    glutMainLoop();

    return 0;
}

void OnDisplay() noexcept
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float previousTime = static_cast<float>(glfwGetTime());

    if (globalShader)
    {
        // Update
        {
            const float currentTime  = static_cast<float>(glfwGetTime());
            const float deltaTime    = currentTime - previousTime;

            cube->Update(deltaTime);

            previousTime = currentTime;
        }
        // Render
        {
            globalShader->Use();

            constexpr float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(WINDOW_HEIGHT);
            constexpr float orthoSize   = 2.0f;

            glm::mat4 projection = glm::ortho(
                -orthoSize * aspectRatio, // left
                 orthoSize * aspectRatio, // right
                -orthoSize,               // bottom
                 orthoSize,               // top
                -100.0f,                  // near
                 100.0f                   // far
            );

            const GLint projLoc = glGetUniformLocation(globalShader->GetProgramID(), "u_Projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            cube->Render(*globalShader);
        }
    }

    glutSwapBuffers();
}

void OnResize(const int width_,
              const int height_) noexcept
{
    windowWidth = width_;
    WINDOW_HEIGHT = height_;
    glViewport(0, 0, width_, height_);
}

void OnKeyInteracted(const unsigned char key_,
                     const int           x_,
                     const int           y_) noexcept
{
    switch (key_)
    {
        case 'q':
        {
            glutLeaveMainLoop();
            break;
        }
        default:
        {
            break;
        }
    }
}

void OnButtonInteracted(const int button_,
                        const int state_,
                        const int x_,
                        const int y_) noexcept
{

}

std::string ReadFile(const std::filesystem::path& path_) noexcept
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