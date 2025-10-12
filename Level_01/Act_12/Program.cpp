#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <random>

#include <glad/glad.h>

#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
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
    explicit Shader(const char* const vertexSource_,
                    const char* const fragmentSource_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Shader() noexcept;

    /**
     * @brief 셰이더를 사용합니다.
     */
    inline void Use() const noexcept
    {
        if (isCompiled)
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
     * @brief 해당 셰이더의 컴파일 여부.
     */
    bool isCompiled = false;

    /**
     * @brief 셰이더 프로그램 ID.
     */
    unsigned int programID;
};

/**
 * @class Shape
 *
 * @brief 도형을 정의합니다.
 */
class Shape final
{
public:
    /**
     * @enum Type
     *
     * @brief 도형의 유형을 정의합니다.
     */
    enum class Type
    {
        /**
         * @brief 선.
         */
        Line,

        /**
         * @brief 삼각형.
         */
        Triangle,

        /**
         * @brief 사각형.
         */
        Rectangle,

        /**
         * @brief 원.
         */
        Circle,
    };

    /**
     * @brief 생성자.
     *
     * @param type_ 도형의 유형.
     */
    explicit Shape(Shape::Type type_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Shape() noexcept;

    /**
     * @brief 도형의 유형을 반환합니다.
     *
     * @return Shape::Type 도형의 유형.
     */
    [[nodiscard]]
    constexpr Shape::Type GetType() const noexcept
    {
        return type;
    }

    /**
     * @brief 도형을 업데이트합니다.
     *
     * @param deltaTime_ 시간 변화량.
     */
    void Update(const float deltaTime_) noexcept;

    /**
     * @brief 도형을 그립니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    void Draw(const Shader& shader_) noexcept;

    /**
     * @brief 도형을 선으로 변경합니다.
     */
    void ToLine() noexcept;

    /**
     * @brief 도형을 삼각형으로 변경합니다.
     */
    void ToTriangle() noexcept;

    /**
     * @brief 도형을 사각형으로 변경합니다.
     */
    void ToRectangle() noexcept;

    /**
     * @brief 도형을 원으로 변경합니다.
     */
    void ToCircle() noexcept;
private:
    /**
     * @brief 도형의 모든 유형에 대한 정점 데이터.
     */
    static inline const std::unordered_map<Shape::Type, std::vector<float>> vertices
    {
        {Shape::Type::Line,      {-0.5f, 0.0f,  0.5f,  0.0f}},
        {Shape::Type::Triangle,  { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f}},
        {Shape::Type::Rectangle, {-0.5f, 0.5f,  0.5f,  0.5f, 0.5f, -0.5f, -0.5f, -0.5f}},
        {Shape::Type::Circle,    { 0.0f}},
    };

    /**
     * @brief 도형의 모든 유형에 대한 요소 데이터.
     */
    static inline const std::unordered_map<Shape::Type, std::vector<unsigned int>> indices
    {
        {Shape::Type::Line,      {0, 1}},
        {Shape::Type::Triangle,  {0, 1, 2}},
        {Shape::Type::Rectangle, {0, 1, 2, 2, 3, 0}},
        {Shape::Type::Circle,    {0}},
    };

    /**
     * @brief 정점 배열 객체.
     */
    unsigned int vao = 0;

    /**
     * @brief 정점 버퍼 객체.
     */
    unsigned int vbo = 0;

    /**
     * @brief 요소 배열 객체.
     */
    unsigned int ebo = 0;

    /**
     * @brief 해당 도형의 유형.
     */
    Shape::Type type;

    /**
     * @brief 애니메이션 여부.
     */
    bool isAnimating = false;
};

/**
 * @brief OpenGL 디버그 메시지 콜백 함수.
 *
 * @param source_    메시지 출처.
 * @param type_      메시지 유형.
 * @param id_        메시지 ID.
 * @param severity_  메시지 심각도.
 * @param length_    메시지 길이.
 * @param message_   메시지 내용.
 * @param userParam_ 사용자 매개변수.
 */
static void GLAPIENTRY OnDebugMessage(const GLenum        source_,
                                      const GLenum        type_,
                                      const GLuint        id_,
                                      const GLenum        severity_,
                                      const GLsizei       length_,
                                      const GLchar* const message_,
                                      const void* const   userParam_) noexcept;

/**
 * @brief 키와 상호작용할 때 호출됩니다.
 *
 * @param window_   윈도우.
 * @param key_      눌린 키.
 * @param scancode_ 스캔 코드.
 * @param action_   키 액션.
 * @param mods_     수정자 키 상태.
 */
static void OnKeyInteracted(GLFWwindow* const window_,
                            const int         key_,
                            const int         scancode_,
                            const int         action_,
                            const int         mods_) noexcept;

/**
 * @brief 버튼과 상호작용할 떄 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param button_ 클릭된 버튼.
 * @param action_ 버튼 액션.
 * @param mods_   버튼 상태.
 */
static void OnButtonInteracted(GLFWwindow* const window_,
                               const int         button_,
                               const int         action_,
                               const int         mods_) noexcept;

/**
 * @brief 셰이더 소스 코드.
 */
static std::string GetFile(const std::filesystem::path& path_);

/**
 * @brief 지정된 경로 내 파일을 읽어옵니다.
 *
 * @param path_ 지정할 경로.
 *
 * @return std::string 파일 내용.
 */
static std::string GetFile(const std::filesystem::path& path_);

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
static constexpr const char* const WINDOW_TITLE = "Level 01 - Act 12";

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
static constinit GLFWwindow* window = nullptr;

/**
 * @brief 배경 색상.
 */
static constinit glm::vec3 backgroundColor = {0.1f, 0.1f, 0.1f};

/**
 * @brief 난수 생성기.
 */
static std::random_device rd;

/**
 * @brief 난수 생성 엔진.
 */
static std::mt19937 gen(rd());

/**
 * @brief 시간 배율.
 */
static constinit float timeScale = 1.0f;

/**
 * @brief 월드 내 있을 수 있는 도형의 최대 갯수.
 */
static constexpr std::size_t MAX_SHAPE_COUNTS = 4;

/**
 * @brief 월드 내 모든 도형.
 */
static std::array<std::unique_ptr<Shape>, MAX_SHAPE_COUNTS> shapes;

int main()
{
    SPDLOG_INFO("Initializing GLFW...");
    {
        if (!glfwInit())
        {
            SPDLOG_CRITICAL("Failed to initialize GLFW");
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
            SPDLOG_CRITICAL("Failed to create GLFW window\n");
            glfwTerminate();
            return -1;
        }

        glfwSetWindowPos(window, 100, 100);
        glfwMakeContextCurrent(window);

        glfwSetKeyCallback(window, OnKeyInteracted);
        glfwSetMouseButtonCallback(window, OnButtonInteracted);
    }
    SPDLOG_INFO("Initialized GLFW successfully!");

    SPDLOG_INFO("Initializing GLAD...");
    {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            SPDLOG_CRITICAL("Failed to initialize GLAD\n");
            return -1;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(OnDebugMessage, nullptr);
    }
    SPDLOG_INFO("Initialized GLAD successfully!");

    const std::string vertexShaderFile   = GetFile("Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile   = GetFile("Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    const Shader shader(vertexShaderSource, fragmentShaderSource);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        const float currentTime = static_cast<float>(glfwGetTime());
        static float lastTime = currentTime;
        const float deltaTime = (currentTime - lastTime) * timeScale;
        lastTime = currentTime;

        shader.Use();
        for (const auto& shape : shapes)
        {
            shape->Update(deltaTime);
            shape->Draw(shader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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
        isCompiled = false;
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
        isCompiled = false;
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
        isCompiled = false;
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    isCompiled = true;
}

Shader::~Shader() noexcept
{
    if (isCompiled)
    {
        glDeleteProgram(programID);
    }
}

Shape::Shape(Shape::Type type_) noexcept
    : type(type_)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Shape::~Shape() noexcept
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void OnDebugMessage(const GLenum        source_,
                    const GLenum        type_,
                    const GLuint        id_,
                    const GLenum        severity_,
                    const GLsizei       length_,
                    const GLchar* const message_,
                    const void* const   userParam_) noexcept
{
    if (type_ == GL_DEBUG_TYPE_ERROR)
    {
        SPDLOG_ERROR("GL ERROR: type = {:x}, severity = {:x}, message = {}\n", type_, severity_, message_);
    }
    if (type_ == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
    {
        SPDLOG_WARN("GL WARNING: type = {:x}, severity = {:x}, message = {}\n", type_, severity_, message_);
    }
    else
    {
        SPDLOG_INFO("GL INFO: type = {:x}, severity = {:x}, message = {}\n", type_, severity_, message_);
    }
}

void OnKeyInteracted(GLFWwindow* const window_,
                     const int         key_,
                     const int         scancode_,
                     const int         action_,
                     const int         mods_) noexcept
{
    if (action_ != GLFW_PRESS)
    {
        return;
    }

    switch (key_)
    {
        case GLFW_KEY_1:
        {
            for (std::unique_ptr<Shape>& shape : shapes)
            {
                if (shape->GetType() == Shape::Type::Line)
                {
                    shape->ToLine();
                }
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

void OnButtonInteracted(GLFWwindow* const window_,
                        const int         button_,
                        const int         action_,
                        const int         mods_) noexcept
{

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
