#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include <gl/glew.h>

#include <gl/freeglut_std.h>
#include <gl/freeglut_ext.h>

#include <spdlog/spdlog.h>

#include "Graphics/Camera.h"
#include "Graphics/Shader.h"
#include "Objects/Object.h"

/**
 * @brief 타이머 이벤트가 발생할 때 호출됩니다.
 *
 * @param value_ 타이머 값.
 */
static void OnTimer(const int value_) noexcept;

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
static void OnResize(const int width_,
                     const int height_) noexcept;

/**
 * @brief 키와 상호작용할 때 호출됩니다.
 *
 * @param key_ 눌린 키.
 * @param x_   마우스 커서의 X 좌표.
 * @param y_   마우스 커서의 Y 좌표.
 */
static void OnKeyInteracted(const unsigned char key_,
                            const int           x_,
                            const int           y_) noexcept;

/**
 * @brief 버튼과 상호작용할 떄 호출됩니다.
 *
 * @param button_ 눌린 버튼.
 * @param state_  버튼 상태.
 * @param x_      마우스 커서의 X 좌표.
 * @param y_      마우스 커서의 Y 좌표.
 */
static void OnButtonInteracted(const int button_,
                               const int state_,
                               const int x_,
                               const int y_) noexcept;

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
static constinit int windowWidth  = 800;

/**
 * @brief 애플리케이션 높이.
 */
static constinit int windowHeight = 600;

/**
 * @brief 윈도우 타이틀
 */
static constexpr const char* WINDOW_TITLE = "Level 01 - Act 19";

/**
 * @brief 애플리케이션에서 사용할 전역 셰이더.
 */
static std::unique_ptr<Shader> globalShader = nullptr;

/**
 * @brief 애플리케이션에서 사용할 전역 카메라.
 */
static std::unique_ptr<Camera> globalCamera = nullptr;

/**
 * @brief 테스트 오브젝트.
 */
static std::unique_ptr<Object> testObject = nullptr;

int main(int argc_, char** argv_)
{
    {
        glutInit(&argc_, argv_);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutInitWindowSize(windowWidth, windowHeight);
        glutCreateWindow(WINDOW_TITLE);

        glutTimerFunc(16, OnTimer, 0);
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

        globalCamera = std::make_unique<Camera>(glm::vec3{0.0f, 0.0f, 5.0f},
                                                glm::vec3{0.0f, 0.0f, -1.0f},
                                                glm::vec3{0.0f, 1.0f, 0.0f});
        globalCamera->SetProjection(Camera::ProjectionType::Perspective);
        globalCamera->SetAspectRatio(static_cast<float>(windowWidth) / static_cast<float>(windowHeight));

        testObject = std::make_unique<Object>();
    }
    glutMainLoop();
}

void OnTimer(const int value_) noexcept
{
    static std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

    const std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    const std::chrono::duration<float> deltaTime = currentTime - lastTime;

    // TODO: 여기에 업데이트 코드를 작성하세요.
    testObject->Update(deltaTime.count());

    lastTime = currentTime;

    glutPostRedisplay();
    glutTimerFunc(16, OnTimer, 0);
}

void OnDisplay() noexcept
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    globalShader->Use();
    globalCamera->PreRender(*globalShader);
    testObject->Render(*globalShader);

    glutSwapBuffers();
}

void OnResize(const int width_,
              const int height_) noexcept
{
    windowWidth  = width_;
    windowHeight = height_;

    glViewport(0, 0, width_, height_);

    if (globalCamera != nullptr)
    {
        const float aspectRatio = static_cast<float>(width_) / static_cast<float>(height_);
        globalCamera->SetAspectRatio(aspectRatio);
    }
}

void OnKeyInteracted(unsigned char key_,
                     int           x_,
                     int           y_) noexcept
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