#include <array>
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
     * @brief 해당 오브젝트를 초기화합니다.
     */
    virtual void Reset() noexcept = 0;

    /**
     * @brief 해당 오브젝트를 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(float deltaTime_) noexcept = 0;

    /**
     * @brief 해당 오브젝트를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    virtual void Render(const Shader& shader_) const noexcept = 0;

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

    /**
     * @brief Y축 회전 트리거.
     */
    bool triggerYAnimation = false;
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

    /**
     * @brief 해당 큐브를 초기화합니다.
     */
    virtual void Reset() noexcept override;

    /**
     * @brief 해당 큐브를 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(float deltaTime_) noexcept override;

    /**
     * @brief 해당 큐브를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    virtual void Render(const Shader& shader_) const noexcept override;

    /**
     * @brief T 애니메이션 트리거.
     */
    bool triggerTAnimation = false;

    /**
     * @brief F 애니메이션 트리거.
     */
    bool triggerFAnimation = false;

    /**
     * @brief S 애니메이션 트리거.
     */
    bool triggerSAnimation = false;

    /**
     * @brief B 애니메이션 트리거.
     */
    bool triggerBAnimation = false;
private:
    /**
     * @brief 윗면의 회전 각도.
     */
    float topAngleRad = 0.0f;

    /**
     * @brief 앞면 애니메이션 진행도.
     */
    float frontPhase = 0.0f;

    /**
     * @brief 앞면의 회전 각도.
     */
    float frontAngleRad = 0.0f;

    /**
     * @brief 옆면의 회전 각도.
     */
    float sideAngleRad = 0.0f;

    /**
     * @brief 뒷면 애니메이션 진행도.
     */
    float backPhase = 0.0f;

    /**
     * @brief 뒷면의 크기 조절 비율.
     */
    float backScale = 1.0f;
};

/**
 * @class Pyramid
 *
 * @brief 피라미드를 정의합니다.
 */
class Pyramid final
    : public Object
{
public:
    /**
     * @brief 생성자.
     */
    Pyramid() noexcept;

    /**
     * @brief 해당 피라미드를 초기화합니다.
     */
    virtual void Reset() noexcept override;

    /**
     * @brief 해당 피라미드를 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(float deltaTime_) noexcept override;

    /**
     * @brief 해당 피라미드를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    virtual void Render(const Shader& shader_) const noexcept override;

    /**
     * @brief O 애니메이션 트리거.
     */
    bool triggerOAnimation = false;

    /**
     * @brief R 애니메이션 트리거.
     */
    bool triggerRAnimation = false;
private:
    /**
     * @brief 각 면의 상태.
     */
    struct FaceState
    {
        /**
         * @brief 회전 각도.
         */
        float angle;

        /**
         * @brief 크기 조절 비율.
         */
        float phase;
    };

    /**
     * @brief 네 면의 상태.
     */
    FaceState faces[4];
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
     * @brief 해당 좌표축을 초기화합니다.
     */
    virtual void Reset() noexcept;

    /**
     * @brief 해당 좌표축을 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(float deltaTime_) noexcept override;

    /**
     * @brief 해당 좌표축을 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    virtual void Render(const Shader& shader_) const noexcept override;
};

/**
 * @brief 부분 렌더링의 대상을 정의합니다.
 */
enum class TargetObject
{
    /**
     * @brief 큐브.
     */
    Cube,

    /**
     * @brief 피라미드.
     */
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
static constexpr const char* WINDOW_TITLE = "Level 01 - Act 17";

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

    axes = new Axes();
    axes->rotation = {30.0f, -30.0f, 0.0f};

    objects.at(0) = new Cube();
    objects.at(1) = new Pyramid();

    for (Object* const object : objects)
    {
        object->position = {0.0f, 0.0f, 0.0f};
        object->rotation = {30.0f, -30.0f, 0.0f};
        object->scale    = {1.0f, 1.0f, 1.0f};
    }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (!glfwWindowShouldClose(window))
    {
        Object* const cube    = objects.at(0);
        Object* const pyramid = objects.at(1);

        Object* const targetObject = (currentTarget == TargetObject::Cube) ? cube : pyramid;

        // 업데이트.
        {
            glfwPollEvents();

            static float lastTime = static_cast<float>(glfwGetTime());

            const float currentTime = static_cast<float>(glfwGetTime());
            const float deltaTime   = currentTime - lastTime;

            TranslateObject(deltaTime);
            RotateObject(deltaTime);
            ScaleObject(deltaTime);

            axes->Update(deltaTime);
            targetObject->Update(deltaTime);

            lastTime = currentTime;
        }
        // 렌더링
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

            axes->Render(shader);
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
    Reset();
}

void Cube::Reset() noexcept
{
    topAngleRad   = 0.0f;
    frontPhase    = 0.0f;
    frontAngleRad = 0.0f;
    sideAngleRad  = 0.0f;
    backPhase     = 0.0f;
    backScale     = 1.0f;
}

void Cube::Update(const float deltaTime_) noexcept
{
    if (triggerYAnimation)
    {
        rotation.y += 90.0f * deltaTime_;
    }

    // 윗면 회전 애니메이션
    if (triggerTAnimation)
    {
        constexpr float goalDegree = 90.0f;
        topAngleRad += glm::radians(goalDegree) * deltaTime_;
    }

    // 앞면 여닫기 애니메이션.
    if (triggerFAnimation)
    {
        constexpr float frontMaxDeg = 90.0f;
        constexpr float frontSpeed  = 1.5f;
        frontPhase += frontSpeed * deltaTime_;

        const float normalized = (std::sin(frontPhase) + 1.0f) * 0.5f;
        frontAngleRad = glm::radians(frontMaxDeg * normalized);
    }

    // 두 옆면 회전 애니메이션.
    if (triggerSAnimation)
    {
        constexpr float sideSpeedDegPerSec = 120.0f;
        sideAngleRad += glm::radians(sideSpeedDegPerSec) * deltaTime_;
    }

    // 뒷면 펄스 애니메이션.
    if (triggerBAnimation)
    {
        constexpr float backSpeed = 2.0f;
        backPhase += backSpeed * deltaTime_;

        const float normalized = (std::sin(backPhase) + 1.0f) * 0.5f;

        constexpr float backMinScale = 0.0f;
        constexpr float backMaxScale = 1.0f;

        backScale = backMinScale + (backMaxScale - backMinScale) * normalized;
    }
}

void Cube::Render(const Shader& shader_) const noexcept
{
    glm::mat4 objectModel = {1.0f};
    objectModel = glm::rotate(objectModel, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    objectModel = glm::rotate(objectModel, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    objectModel = glm::rotate(objectModel, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    objectModel = glm::scale(objectModel, scale);

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");

    glBindVertexArray(vao);

    for (int face = 0; face < 6; ++face)
    {
        glm::mat4 faceLocal = {1.0f};

        if (face == 4)
        {
            constexpr glm::vec3 pivot = glm::vec3(0.0f, 0.5f, 0.0f);
            faceLocal = glm::translate(faceLocal, pivot);
            faceLocal = glm::rotate(faceLocal, topAngleRad, glm::vec3(0.0f, 0.0f, 1.0f));
            faceLocal = glm::translate(faceLocal, -pivot);
        }
        else if (face == 0)
        {
            constexpr glm::vec3 pivot = glm::vec3(0.0f, 0.5f, 0.5f);
            faceLocal = glm::translate(faceLocal, pivot);
            faceLocal = glm::rotate(faceLocal, -frontAngleRad, glm::vec3(1.0f, 0.0f, 0.0f));
            faceLocal = glm::translate(faceLocal, -pivot);
        }
        else if (face == 2 || face == 3)
        {
            constexpr glm::vec3 pivot = glm::vec3(-0.5f, 0.0f, 0.0f);
            faceLocal = glm::translate(faceLocal, pivot);
            faceLocal = glm::rotate(faceLocal, sideAngleRad, glm::vec3(1.0f, 0.0f, 0.0f));
            faceLocal = glm::translate(faceLocal, -pivot);
        }
        else if (face == 1)
        {
            constexpr glm::vec3 pivot = glm::vec3(0.0f, 0.0f, -0.5f);
            faceLocal = glm::translate(faceLocal, pivot);
            faceLocal = glm::scale(faceLocal, glm::vec3(backScale, backScale, 1.0f));
            faceLocal = glm::translate(faceLocal, -pivot);
        }

        const glm::mat4 faceModel = glm::translate(glm::mat4{1.0f}, position) * objectModel * faceLocal;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(faceModel));
        glDrawArrays(GL_TRIANGLES, face * 6, 6);
    }

    glBindVertexArray(0);
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
    Reset();
}

void Pyramid::Reset() noexcept
{
    for (int i = 0; i < 4; ++i)
    {
        faces[i].angle = 0.0f;
        faces[i].phase = static_cast<float>(i) * 0.6f;
    }
}

void Pyramid::Update(const float deltaTime_) noexcept
{
    if (triggerYAnimation)
    {
        rotation.y += 90.0f * deltaTime_;
    }

    if (triggerOAnimation)
    {
        constexpr float foldSpeed  = 1.2f;
        constexpr float maxFoldDeg = 233.0f;

        static float commonPhase = 0.0f;
        commonPhase -= foldSpeed * deltaTime_;

        const float normalized = (std::sin(commonPhase) + 1.0f) * 0.5f;
        const float angleRad   = glm::radians(maxFoldDeg * normalized);

        for (auto& [angle, phase] : faces)
        {
            phase = commonPhase;
            angle = angleRad;
        }
    }

    if (triggerRAnimation)
    {
        constexpr float faceSpeed = 2.0f;
        static int activeFace = 0;
        static float phase = 0.0f;
        static bool peaked = false;

        phase += faceSpeed * deltaTime_;

        const float normalized = (std::sin(phase) + 1.0f) * 0.5f;
        const float angleRad   = glm::radians(120.0f * normalized);

        for (int i = 0; i < 4; ++i)
        {
            faces[i].angle = (i == activeFace) ? angleRad : 0.0f;
        }

        if (!peaked && normalized > 0.95f)
        {
            peaked = true;
        }

        if (peaked && normalized < 0.05f)
        {
            activeFace = (activeFace + 1) % 4;
            peaked = false;
        }
    }
}

void Pyramid::Render(const Shader& shader_) const noexcept
{
    glm::mat4 model = {1.0f};
    model = glm::translate(model, position);
    model = glm::rotate(model,  glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model,  glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model,  glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");

    glBindVertexArray(vao);

    for (int face = 0; face < 5; ++face)
    {
        glm::mat4 faceLocal = {1.0f};

        if (face >= 0 && face <= 3)
        {
            glm::vec3 pivot;
            glm::vec3 axis;

            float angle = faces[face].angle;

            switch (face)
            {
                case 0:
                    pivot = glm::vec3(0.0f, -0.5f, -0.5f);
                    axis  = glm::vec3(1.0f, 0.0f, 0.0f);
                    angle = -angle; // fold outward
                    break;
                case 1:
                    pivot = glm::vec3(0.5f, -0.5f, 0.0f);
                    axis  = glm::vec3(0.0f, 0.0f, 1.0f);
                    angle = -angle; // fold outward
                    break;
                case 2:
                    pivot = glm::vec3(0.0f, -0.5f, 0.5f);
                    axis  = glm::vec3(1.0f, 0.0f, 0.0f);
                    // keep angle positive so it mirrors outward
                    break;
                case 3:
                    pivot = glm::vec3(-0.5f, -0.5f, 0.0f);
                    axis  = glm::vec3(0.0f, 0.0f, 1.0f);
                    // keep angle positive so it mirrors outward
                    break;
                default:
                    pivot = glm::vec3(0.0f);
                    axis  = glm::vec3(0.0f, 1.0f, 0.0f);
                    break;
            }

            faceLocal = glm::translate(faceLocal, pivot);
            faceLocal = glm::rotate(faceLocal, angle, axis);
            faceLocal = glm::translate(faceLocal, -pivot);
        }

        // Use the already-translated/rotated/scaled model; do not reapply position translation.
        const glm::mat4 faceModel = model * faceLocal;

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &faceModel[0][0]);
        glDrawArrays(GL_TRIANGLES, face * 3, (face == 4) ? 6 : 3);
    }

    glBindVertexArray(0);
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

void Axes::Reset() noexcept
{
    const Object* const cube    = objects.at(0);
    const Object* const pyramid = objects.at(1);

    const Object* const targetObject = (currentTarget == TargetObject::Cube) ? cube : pyramid;

    position = targetObject->position;
    rotation = targetObject->rotation;
    scale    = targetObject->scale;
}

void Axes::Update(const float deltaTime_) noexcept
{
    const Object* const cube    = objects.at(0);
    const Object* const pyramid = objects.at(1);

    const Object* const targetObject = (currentTarget == TargetObject::Cube) ? cube : pyramid;

    position = targetObject->position;
    rotation = targetObject->rotation;
    scale    = targetObject->scale;
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
            case GLFW_KEY_H:
            {
                if (shouldCull)
                {
                    shouldCull = false;
                    //glDisable(GL_CULL_FACE);
                    glDisable(GL_DEPTH_TEST);
                }
                else
                {
                    shouldCull = true;
                    // glEnable(GL_CULL_FACE);
                    // glCullFace(GL_BACK);
                    glEnable(GL_DEPTH_TEST);
                }

                break;
            }
            case GLFW_KEY_U:
            {
                static bool shouldCull = true;

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
            case GLFW_KEY_P:
            {
                if (currentTarget == TargetObject::Cube)
                {
                    currentTarget = TargetObject::Pyramid;
                }
                else if (currentTarget == TargetObject::Pyramid)
                {
                    currentTarget = TargetObject::Cube;
                }

                break;
            }
            case GLFW_KEY_Y:
            {
                break;
            }
            case GLFW_KEY_C:
            {
                currentTarget = TargetObject::Cube;
                axes->Reset();
                objects.at(0)->Reset();
                break;
            }
            case GLFW_KEY_Q:
            {
                glfwSetWindowShouldClose(window_, false);
                break;
            }
            default:
            {
                break;
            }
        }

        if (currentTarget == TargetObject::Cube)
        {
            Cube* const cube = static_cast<Cube*>(objects.at(0));

            switch (key_)
            {
                case GLFW_KEY_Y:
                {
                    cube->triggerYAnimation = !cube->triggerYAnimation;
                    break;
                }
                case GLFW_KEY_T:
                {
                    cube->triggerTAnimation = !cube->triggerTAnimation;
                    break;
                }
                case GLFW_KEY_F:
                {
                    cube->triggerFAnimation = !cube->triggerFAnimation;
                    break;
                }
                case GLFW_KEY_S:
                {
                    cube->triggerSAnimation = !cube->triggerSAnimation;
                    break;
                }
                case GLFW_KEY_B:
                {
                    cube->triggerBAnimation = !cube->triggerBAnimation;
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else if (currentTarget == TargetObject::Pyramid)
        {
            Pyramid* const pyramid = static_cast<Pyramid*>(objects.at(1));

            switch (key_)
            {
                case GLFW_KEY_Y:
                {
                    pyramid->triggerYAnimation = !pyramid->triggerYAnimation;
                    break;
                }
                case GLFW_KEY_O:
                {
                    pyramid->triggerOAnimation = !pyramid->triggerOAnimation;
                    break;
                }
                case GLFW_KEY_R:
                {
                    pyramid->triggerRAnimation = !pyramid->triggerRAnimation;
                    break;
                }
                default:
                {
                    break;
                }
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

    const glm::vec3 currentPosition = target->position;

    constexpr float translationSpeed = 1.0f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        target->position = currentPosition + glm::vec3{0.0f, translationSpeed * deltaTime_, 0.0f};
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        target->position = currentPosition + glm::vec3{0.0f, -translationSpeed * deltaTime_, 0.0f};
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        target->position = currentPosition + glm::vec3{-translationSpeed * deltaTime_, 0.0f, 0.0f};
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        target->position = currentPosition + glm::vec3{translationSpeed * deltaTime_, 0.0f, 0.0f};
    }
}

void RotateObject(float deltaTime_) noexcept
{
    Object* const target = (currentTarget == TargetObject::Cube) ? objects.at(0) : objects.at(1);

    const glm::vec3 currentRotation = target->rotation;

    constexpr float rotationSpeed = 90.0f;

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        target->rotation = currentRotation + glm::vec3{-rotationSpeed * deltaTime_, 0.0f, 0.0f};
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        target->rotation = currentRotation + glm::vec3{rotationSpeed * deltaTime_, 0.0f, 0.0f};
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        target->rotation = currentRotation + glm::vec3{0.0f, -rotationSpeed * deltaTime_, 0.0f};
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        target->rotation = currentRotation + glm::vec3{0.0f, rotationSpeed * deltaTime_, 0.0f};
    }
}

void ScaleObject(float deltaTime_) noexcept
{
    Object* const target = (currentTarget == TargetObject::Cube) ? objects.at(0) : objects.at(1);

    const glm::vec3 currentScale = target->scale;

    constexpr float scaleFactor = 2.0f;

    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {
        target->scale = currentScale + glm::vec3{scaleFactor * deltaTime_,
                                                 scaleFactor * deltaTime_,
                                                 scaleFactor * deltaTime_};
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {
        target->scale = currentScale + glm::vec3{-scaleFactor * deltaTime_,
                                                 -scaleFactor * deltaTime_,
                                                 -scaleFactor * deltaTime_};
    }
}
