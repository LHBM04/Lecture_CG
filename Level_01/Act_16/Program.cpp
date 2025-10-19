#include <filesystem>
#include <fstream>
#include <ios>
#include <random>

#include <glad/glad.h>

#include <glfw/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

/**
 * @class Object
 *
 * @brief 오브젝트를 정의합니다.
 */
class Object
{
public:
    /**
     * @brief 생성자.
     *
     * @param vertices_ 정점 배열.
     */
    Object(std::initializer_list<GLfloat> vertices_) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Object() noexcept;

    /**
     * @brief 해당 오브젝트를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     * @param first_   첫 번째 정점 인덱스.
     * @param count_   렌더링할 정점 개수.
     */
    virtual void Render(const Shader& shader_) const noexcept;

    /**
     * @brief 위치를 반환합니다.
     *
     * @return glm::vec3 위치.
     */
    [[nodiscard]]
    constexpr glm::vec3 GetPosition() const noexcept
    {
        return position;
    }

    /**
     * @brief 회전을 반환합니다.
     *
     * @return glm::vec3 회전.
     */
    [[nodiscard]]
    constexpr glm::vec3 GetRotation() const noexcept
    {
        return rotation;
    }

    /**
     * @brief 크기를 반환합니다.
     *
     * @return glm::vec3 크기.
     */
    [[nodiscard]]
    constexpr glm::vec3 GetScale() const noexcept
    {
        return scale;
    }

    /**
     * @brief 위치를 설정합니다.
     *
     * @param position_ 설정할 위치.
     */
    inline void SetPosition(const glm::vec3& position_) noexcept
    {
        position = position_;
    }

    /**
     * @brief 회전을 설정합니다.
     *
     * @param rotation_ 설정할 회전.
     */
    inline void SetRotation(const glm::vec3& rotation_) noexcept
    {
        rotation = rotation_;
    }

    /**
     * @brief 크기를 설정합니다.
     *
     * @param scale_ 설정할 크기.
     */
    inline void SetScale(const glm::vec3& scale_) noexcept
    {
        scale = scale_;
    }

    /**
     * @brief 정점 개수를 반환합니다.
     *
     * @return GLsizei 정점 개수.
     */
    [[nodiscard]]
    constexpr GLsizei GetVertexCount() const noexcept
    {
        return vertexCount;
    }
protected:
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
     * @brief 정점 개수.
     */
    GLsizei vertexCount = 0;

    /**
     * @brief 요소 개수.
     */
    GLsizei elementCount = 0;

    /**
     * @brief 위치.
     */
    glm::vec3 position = {0.0f, 0.0f, 0.0f};

    /**
     * @brief 회전.
     */
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};

    /**
     * @brief 크기.
     */
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
};

/**
 * @class Cube
 *
 * @brief 큐브를 정의합니다.
 */
class Cube final
    : public Object
{
public:
    /**
     * @brief 생성자.
     */
    Cube() noexcept;
};

class Pyramid final
    : public Object
{
public:
    /**
     * @brief 생성자.
     */
    Pyramid() noexcept;
};

/**
 * @class Axes
 *
 * @brief 좌표축을 정의합니다.
 */
class Axes final
    : public Object
{
public:
    /**
     * @brief 생성자.
     */
    Axes() noexcept;

    /**
     * @brief 해당 오브젝트를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    virtual void Render(const Shader& shader_) const noexcept override;
};

/**
 * @brief 렌더링 모드를 정의합니다.
 */
enum class RenderMode
{
    /**
     * @brief 면 렌더링.
     */
    Solid,

    /**
     * @brief 선 렌더링.
     */
    Wire
};

/**
 * @brief 부분 렌더링의 대상을 정의합니다.
 */
enum class TargetObject
{
    Cube,
    Pyramid
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
 * @brief 오브젝트를 이동합니다.
 *
 * @param deltaTime_ 델타 시간.
 */
void TranslateObject(float deltaTime_) noexcept;

/**
 * @brief 오브젝트를 회전시킵니다.
 *
 * @param deltaTime_    델타 시간.
 */
void RotateObject(float deltaTime_) noexcept;

/**
 * @brief 오브젝트의 크기를 조절합니다.
 *
 * @param deltaTime_   델타 시간.
 */
void ScaleObject(float deltaTime_) noexcept;

/**
 * @brief GLFW 윈도우 핸들.
 */
static GLFWwindow* window = nullptr;

/**
 * @brief 애플리케이션 너비.
 */
static constexpr unsigned int WINDOW_WIDTH  = 800;

/**
 * @brief 애플리케이션 높이.
 */
static constexpr unsigned int WINDOW_HEIGHT = 600;

/**
 * @brief 윈도우 타이틀
 */
static constexpr const char* WINDOW_TITLE = "Level 01 - Act 16";

/**
 * @brief OpenGL 컨텍스트 주 버전
 */
static constexpr int CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief OpenGL 컨텍스트 부 버전
 */
static constexpr int CONTEXT_MINOR_VERSION = 5;

/**
 * @brief 좌표축 오브젝트.
 */
static Axes* axes;

/**
 * @brief 오브젝트들.
 */
static std::array<Object*, 2> objects = {nullptr};

/**
 * @brief 현재 렌더링 모드.
 */
static RenderMode currentRenderMode = RenderMode::Solid;

/**
 * @brief 부분 렌더링의 대상 오브젝트.
 */
static TargetObject currentTarget = TargetObject::Cube;

/**
 * @brief 면 컬링 활성화 여부.
 */
static bool shouldCull = true;

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

    const std::string vertexShaderFile   = ReadFile("Resources/Shaders/Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile   = ReadFile("Resources/Shaders/Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    const Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.Use();

    constexpr float aspectRatio = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
    constexpr float orthoSize   = 2.0f;

    glm::mat4 projection = glm::ortho(
        -orthoSize * aspectRatio, // left
         orthoSize * aspectRatio, // right
        -orthoSize,               // bottom
         orthoSize,               // top
        -100.0f,                  // near
         100.0f                   // far
    );

    const GLint projLoc = glGetUniformLocation(shader.GetProgramID(), "u_Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    axes = new Axes();
    axes->SetRotation({30.0f, -30.0f, 0.0f});

    objects.at(0) = new Cube();
    objects.at(1) = new Pyramid();

    for (Object* const object : objects)
    {
        object->SetPosition({0.0f, 0.0f, 0.0f});
        object->SetRotation({30.0f, -30.0f, 0.0f});
        object->SetScale({1.0f, 1.0f, 1.0f});
    }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (!glfwWindowShouldClose(window))
    {
        Object* const cube        = objects.at(0);
        Object* const tetrahedron = objects.at(1);

        Object* const targetObject = (currentTarget == TargetObject::Cube) ? cube : tetrahedron;

        // 업데이트.
        {
            glfwPollEvents();

            static float lastTime = static_cast<float>(glfwGetTime());

            const float currentTime = static_cast<float>(glfwGetTime());
            const float deltaTime   = currentTime - lastTime;

            TranslateObject(deltaTime);
            RotateObject(deltaTime);
            ScaleObject(deltaTime);

            lastTime = currentTime;
        }
        // 렌더링
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            axes->Render(shader);

            if (currentRenderMode == RenderMode::Solid)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }

            targetObject->Render(shader);

            glfwSwapBuffers(window);
        }
    }

    delete axes;

    for (const Object* const object : objects)
    {
        delete object;
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

Object::Object(const std::initializer_list<GLfloat> vertices_) noexcept
{
    constexpr size_t floatsPerVertex = 6;
    vertexCount  = static_cast<GLsizei>(vertices_.size() / floatsPerVertex);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices_.size() * sizeof(GLfloat)),
                 vertices_.begin(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 0,
                 nullptr,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          floatsPerVertex * sizeof(float),
                          nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          floatsPerVertex * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Object::~Object() noexcept
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

void Object::Render(const Shader& shader_) const noexcept
{
    glm::mat4 model = {1.0f};
    model = glm::translate(model, position);
    model = glm::rotate(model,  glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model,  glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model,  glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

Cube::Cube() noexcept
    : Object(
        {
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f
        }
    )
{
}

Pyramid::Pyramid() noexcept
    : Object(
        {
             0.0f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,

             0.0f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,

             0.0f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

             0.0f,  0.5f,  0.0f,   1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,

             0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f
        }
    )
{
}

Axes::Axes() noexcept
    : Object(
        {
            0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f
        }
    )
{
}

void Axes::Render(const Shader& shader_) const noexcept
{
    glm::mat4 model = {1.0f};
    model = glm::translate(model, position);
    model = glm::rotate(model,  glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model,  glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model,  glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale * 50.0f);

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);
}

void OnKeyInteracted(GLFWwindow* window_,
                     int         key_,
                     int         scancode_,
                     int         action_,
                     int         mods_) noexcept
{
    if (action_ == GLFW_PRESS)
    {
        switch (key_)
        {
            case GLFW_KEY_C:
            {
                currentTarget = TargetObject::Cube;
                currentRenderMode = RenderMode::Solid;
                break;
            }
            case GLFW_KEY_P:
            {
                currentTarget = TargetObject::Pyramid;
                currentRenderMode = RenderMode::Solid;
                break;
            }
            case GLFW_KEY_H:
            {
                if (shouldCull)
                {
                    shouldCull = false;
                    glDisable(GL_CULL_FACE);
                }
                else
                {
                    shouldCull = true;
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                }

                break;
            }
            case GLFW_KEY_W:
            {
                currentRenderMode = (mods_ & GLFW_MOD_SHIFT) ? RenderMode::Solid : RenderMode::Wire;
                break;
            }
            case GLFW_KEY_S:
            {
                for (Object* const object : objects)
                {
                    object->SetPosition({0.0f, 0.0f, 0.0f});
                    object->SetRotation({30.0f, -30.0f, 0.0f});
                    object->SetScale({1.0f, 1.0f, 1.0f});
                }

                break;
            }
            default:
            {
                break;
            }
        }
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

void TranslateObject(float deltaTime_) noexcept
{
    Object* const target = (currentTarget == TargetObject::Cube) ? objects.at(0) : objects.at(1);

    const glm::vec3 currentPosition = target->GetPosition();

    constexpr float translationSpeed = 1.0f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        target->SetPosition(currentPosition + glm::vec3{0.0f, translationSpeed * deltaTime_, 0.0f});
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        target->SetPosition(currentPosition + glm::vec3{0.0f, -translationSpeed * deltaTime_, 0.0f});
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        target->SetPosition(currentPosition + glm::vec3{-translationSpeed * deltaTime_, 0.0f, 0.0f});
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        target->SetPosition(currentPosition + glm::vec3{translationSpeed * deltaTime_, 0.0f, 0.0f});
    }
}

void RotateObject(float deltaTime_) noexcept
{
    Object* const target = (currentTarget == TargetObject::Cube) ? objects.at(0) : objects.at(1);

    const glm::vec3 currentRotation = target->GetRotation();

    constexpr float rotationSpeed = 90.0f;

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        target->SetRotation(currentRotation + glm::vec3{-rotationSpeed * deltaTime_, 0.0f, 0.0f});
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        target->SetRotation(currentRotation + glm::vec3{rotationSpeed * deltaTime_, 0.0f, 0.0f});
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        target->SetRotation(currentRotation + glm::vec3{0.0f, -rotationSpeed * deltaTime_, 0.0f});
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        target->SetRotation(currentRotation + glm::vec3{0.0f, rotationSpeed * deltaTime_, 0.0f});
    }
}

void ScaleObject(float deltaTime_) noexcept
{
    Object* const target = (currentTarget == TargetObject::Cube) ? objects.at(0) : objects.at(1);

    const glm::vec3 currentScale = target->GetScale();

    constexpr float scaleFactor = 2.0f;

    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {
        target->SetScale(currentScale + glm::vec3{scaleFactor * deltaTime_, scaleFactor * deltaTime_, scaleFactor * deltaTime_});
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {
        target->SetScale(currentScale + glm::vec3{-scaleFactor * deltaTime_, -scaleFactor * deltaTime_, -scaleFactor * deltaTime_});
    }
}
