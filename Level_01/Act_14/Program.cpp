#include <array>
#include <filesystem>
#include <fstream>
#include <ios>

#include <glad/glad.h>

#include <glfw/glfw3.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>

/**
 * @class Shader
 *
 * @brief 셰이더를 정의합니다.
 */
class Shader final
{
public:
    /**
     * @brief 생성자.
     *
     * @param vertexSource_   버텍스 셰이더 소스 코드.
     * @param fragmentSource_ 프래그먼트 셰이더 소스 코드
     */
    explicit Shader(const char* vertexSource_,
                    const char* fragmentSource_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Shader() noexcept;

    /**
     * @brief 셰이더를 사용합니다.
     */
    inline void Use() const noexcept
    {
        if (programID != 0)
        {
            glUseProgram(programID);
        }
    }

    /**
     * @brief 셰이더 프로그램 ID를 반환합니다.
     *
     * @return unsigned int 셰이더 프로그램 ID.
     */
    [[nodiscard]]
    constexpr unsigned int GetProgramID() const noexcept
    {
        return programID;
    }
private:
    /**
     * @brief 셰이더 프로그램 ID.
     */
    GLuint programID = 0;
};

class Triangle final
{
public:
    /**
     * @brief 생성자.
     *
     * @param position_ 삼각형 위치.
     * @param rotation_ 삼각형 회전.
     */
    explicit Triangle(const glm::vec2& position_,
                      const glm::vec3& rotation_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Triangle() noexcept;

    /**
     * @brief 해당 삼각형을 업데이트합니다.
     *
     * @param deltaTime_ 시간 변화량.
     */
    void Update(float deltaTime_) noexcept;

    /**
     * @brief 해당 삼각형을 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    void Render(const Shader& shader_) const noexcept;

    /**
     * @brief 애니메이션 진행률.
     */
    float progress = 0.0f;
private:
    /**
     * @brief 정점 배열을 업데이트합니다.
     *
     * @param deltaTime_ 시간 변화량.
     */
    void UpdateVertices(float deltaTime_) noexcept;

    /**
     * @brief 위치를 업데이트합니다.
     *
     * @param deltaTime_ 시간 변화량.
     */
    void UpdatePosition(float deltaTime_) noexcept;

    /**
     * @brief 정점 배열 객체.
     */
    GLuint vao = 0;

    /**
     * @brief 정점 버퍼 객체.
     */
    GLuint vbo = 0;

    /**
     * @brief 요소 배열 객체.
     */
    GLuint ebo = 0;

    /**
     * @brief 위치.
     */
    glm::vec2 position;

    /**
     * @brief 크기.
     */
    glm::vec2 scale = {100.0f, 50.0f};

    /**
     * @brief 회전.
     */
    glm::vec3 rotation;

    /**
     * @brief 색상.
     */
    glm::vec3 color = {1.0f, 1.0f, 0.0f};

    /**
     * @brief 정점 데이터.
     */
    std::array<float, 6> vertices{};
};

/**
 * @enum Rotation
 *
 * @brief 회전 방향을 정의합니다.
 */
enum class Rotation
{
    /**
     * @brief 없음.
     */
    None,

    /**
     * @brief 시계 방향.
     */
    CW,

    /**
     * @brief 반시계 방향.
     */
    CCW,
};

/**
 * @brief 키와 상호작용할 때 호출됩니다.
 *
 * @param window_   윈도우.
 * @param key_      눌린 키.
 * @param scancode_ 스캔 코드.
 * @param action_   키 액션.
 * @param mods_     키 상태.
 */
void OnKeyInteracted(GLFWwindow* window_,
                     int         key_,
                     int         scancode_,
                     int         action_,
                     int         mods_) noexcept;

/**
 * @brief 버튼과 상호작용할 떄 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param button_ 클릭된 버튼.
 * @param action_ 버튼 액션.
 * @param mods_   버튼 상태.
 */
void OnButtonInteracted(GLFWwindow* window_,
                        int         button_,
                        int         action_,
                        int         mods_) noexcept;

/**
 * @brief 마우스 커서가 이동할 때 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param x_      마우스 커서의 X 좌표.
 * @param y_      마우스 커서의 Y 좌표.
 */
void OnCursorMoved(GLFWwindow* window_,
                   double      x_,
                   double      y_) noexcept;

/**
 * @brief 디버깅 메세지가 발생했을 때 호출됩니다.
 *
 * @param source_    메시지 출처.
 * @param type_      메시지 유형.
 * @param id_        메시지 ID.
 * @param severity_  메시지 심각도.
 * @param length_    메시지 길이.
 * @param message_   메시지 내용.
 * @param userParam_ 사용자 매개변수.
 */
void OnDebugMessage(GLenum       source_,
                    GLenum       type_,
                    GLuint       id_,
                    GLenum       severity_,
                    GLsizei      length_,
                    const char*  message_,
                    const void*  userParam_) noexcept;

/**
 * @brief 지정한 경로 내 파일을 읽어옵니다.
 *
 * @param path_ 지정할 경로.
 *
 * @return std::string 파일 내용.
 */
static std::string ReadFile(const std::filesystem::path& path_) noexcept;

/**
 * @brief GLFW 윈도우 핸들.
 */
static GLFWwindow* window = nullptr;

/**
 * @brief 애플리케이션 너비.
 */
static constexpr unsigned int WINDOW_WIDTH  = 1200;

/**
 * @brief 애플리케이션 높이.
 */
static constexpr unsigned int WINDOW_HEIGHT = 600;

/**
 * @brief OpenGL 컨텍스트 주 버전
 */
static constexpr int CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief OpenGL 컨텍스트 부 버전
 */
static constexpr int CONTEXT_MINOR_VERSION = 5;

/**
 * @brief 윈도우 타이틀
 */
static constexpr const char* WINDOW_TITLE = "Level 01 - Act 14";

/**
 * @brief 최대 시간 배율.
 */
static constexpr float MAX_TIME_SCALE = 30.0f;

/**
 * @brief 최소 시간 배율.
 */
static constexpr float MIN_TIME_SCALE = 1.0f;

/**
 * @brief 시간 배율.
 */
static float timeScale = 1.0f;

/**
 * @brief 월드 내 최대 삼각형 갯수.
 */
static constexpr std::size_t MAX_TRIANGLE_COUNT = 4;

/**
 * @brief 월드 내 모든 삼각형.
 */
static std::vector<Triangle> triangles;

/**
 * @brief 회전 방식.
 */
static Rotation rotateMode = Rotation::None;

/**
 * @brief 접힘 모드 활성화 여부.
 */
static bool foldMode = false;

int main()
{
    if (!glfwInit())
    {
        SPDLOG_CRITICAL("");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              WINDOW_TITLE,
                              nullptr,
                              nullptr);

    if (!window)
    {
        SPDLOG_CRITICAL("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, OnKeyInteracted);
    glfwSetMouseButtonCallback(window, OnButtonInteracted);
    glfwSetCursorPosCallback(window, OnCursorMoved);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        SPDLOG_CRITICAL("Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OnDebugMessage, nullptr);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    const std::string vertexShaderFile   = ReadFile("Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile   = ReadFile("Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    const Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.Use();

    triangles.reserve(MAX_TRIANGLE_COUNT);

    triangles.emplace_back(glm::vec2(0.0f, 150.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    triangles.emplace_back(glm::vec2(-150.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    triangles.emplace_back(glm::vec2(150.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    triangles.emplace_back(glm::vec2(0.0f, -150.0f), glm::vec3(0.0f, -1.0f, 0.0f));

    static float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        // 업데이트.
        {
            glfwPollEvents();

            const float currentTime = static_cast<float>(glfwGetTime());
            const float deltaTime   = (currentTime - lastTime) * timeScale;

            for (Triangle& triangle : triangles)
            {
                triangle.Update(deltaTime);
            }

            lastTime = currentTime;
        }
        // 렌더링
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (const Triangle& triangle : triangles)
            {
                triangle.Render(shader);
            }

            glfwSwapBuffers(window);
        }
    }

    return 0;
}

Shader::Shader(const char* const vertexSource_,
               const char* const fragmentSource_) noexcept
{
    GLint success;
    char infoLog[512];

    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource_, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
        SPDLOG_CRITICAL("Vertex shader compilation failed: {}", infoLog);
        glDeleteShader(vertexShader);
        return;
    }

    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource_, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
        SPDLOG_CRITICAL("Fragment shader compilation failed: {}", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, sizeof(infoLog), nullptr, infoLog);
        SPDLOG_CRITICAL("Shader program linking failed: {}", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(programID);
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() noexcept
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}

Triangle::Triangle(const glm::vec2& position_,
                   const glm::vec3& rotation_) noexcept
    : position{position_}
    , rotation{rotation_}
    , vertices{0.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f}
{
    constexpr std::array<GLuint, 3> indices
    {
        0, 1, 2
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(GLfloat)), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Triangle::~Triangle() noexcept
{
    if (vao != 0)
    {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
    }
    if (ebo != 0)
    {
        glDeleteBuffers(1, &ebo);
    }
}

void Triangle::Update(const float deltaTime_) noexcept
{
    UpdatePosition(deltaTime_);
    UpdateVertices(deltaTime_);
}

void Triangle::Render(const Shader& shader_) const noexcept
{
    glm::mat4 model = {1.0f};
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, std::atan2(rotation.y, rotation.x) + glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    glm::mat4 projection = glm::ortho(
       -static_cast<float>(WINDOW_WIDTH)  / 2.0f, // 왼쪽
        static_cast<float>(WINDOW_WIDTH)  / 2.0f, // 오른쪽
       -static_cast<float>(WINDOW_HEIGHT) / 2.0f, // 아래
        static_cast<float>(WINDOW_HEIGHT) / 2.0f, // 위
       -1.0f,                                     // near
        1.0f                                      // far
        );

    const GLint projLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
    glUniform3fv(colorLoc, 1, &color[0]);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Triangle::UpdateVertices(const float deltaTime_) noexcept
{
    std::array<float, 6> targetVertices = { };

    if (foldMode)
    {
        targetVertices =
        {
            0.0f, -2.5f,
           -1.0f, -1.0f,
            1.0f, -1.0f
        };
    }
    else
    {
        targetVertices =
        {
            0.0f,  1.0f,
           -1.0f, -1.0f,
            1.0f, -1.0f
         };
    }

    for (std::size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i] = vertices[i] * (1.0f - progress) + targetVertices[i] * progress;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_DYNAMIC_DRAW);

    if (constexpr float guard = 0.25f; (progress += deltaTime_ * guard) >= 1.0f)
    {
        vertices = targetVertices;
        progress = 0.0f;
    }
}

void Triangle::UpdatePosition(const float deltaTime_) noexcept
{
    if (rotateMode == Rotation::None)
    {
        return;
    }

    const float radius = glm::length(position);
    float theta = atan2(position.y, position.x);

    if (rotateMode == Rotation::CW)
    {
        theta -= deltaTime_;
    }
    else
    {
        theta += deltaTime_;
    }

    position = {radius * cos(theta), radius * sin(theta)};
    rotation = {cos(theta), sin(theta), 0.0f};
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
            rotateMode = Rotation::CW;
            SPDLOG_INFO("Rotation: Clock Wise");
            break;
        }
        case GLFW_KEY_T:
        {
            rotateMode = Rotation::CCW;
            SPDLOG_INFO("Rotation: Counter Clock Wise");
            break;
        }
        case GLFW_KEY_UP:
        {
            if ((timeScale += 1.0f) > MAX_TIME_SCALE)
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
            if ((timeScale -= 1.0f) < MIN_TIME_SCALE)
            {
                timeScale = MIN_TIME_SCALE;
                SPDLOG_WARN("Time scale cannot be less than or equal to zero.");
                break;
            }

            SPDLOG_INFO("Time scale set to {:.1f}.", timeScale);
            break;
        }
        case GLFW_KEY_S:
        {
            static float previousTimeScale = timeScale;

            if (timeScale != 0.0f)
            {
                previousTimeScale = timeScale;
                timeScale = 0.0f;
                SPDLOG_INFO("Time paused.");
            }
            else
            {
                timeScale = previousTimeScale;
                SPDLOG_INFO("Time resumed.");
            }

            break;
        }
        case GLFW_KEY_Q:
        {
            glfwSetWindowShouldClose(window_, true);
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
    if (action_ == GLFW_PRESS)
    {
        if (!foldMode)
        {
            foldMode = true;
            SPDLOG_INFO("Fold mode activated.");
        }
        else
        {
            foldMode = false;
            SPDLOG_INFO("Fold mode inactivated.");
        }

        for (Triangle& triangle : triangles)
        {
            triangle.progress = 0.0f;
        }
    }
}

void OnCursorMoved(GLFWwindow* window_,
                   double      x_,
                   double      y_) noexcept
{

}

void OnDebugMessage(GLenum      source_,
                    GLenum      type_,
                    GLuint      id_,
                    GLenum      severity_,
                    GLsizei     length_,
                    const char* message_,
                    const void* userParam_) noexcept
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