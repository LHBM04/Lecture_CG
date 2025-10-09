#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#include <glad/glad.h>

#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

#include <glm/glm.hpp>

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

class Spiral
{
public:
    /**
     * @brief 생성자.
     */
    explicit Spiral(const glm::vec2 center_,
                    const bool      clockwise_) noexcept;

    /**
     * @brief 스파이럴을 그립니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    void Draw(const Shader& shader_) const noexcept;
private:
    static constexpr float pitch   = 28.0f; // 팔 간격(px)
    static constexpr float turns   = 3.0f;
    static constexpr float spacing = 8.0f;  // 점 간격(px)
    static constexpr float start   = 0.0f;

    /**
     * @brief 점들.
     */
    std::vector<glm::vec2> points;
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
                                      const void* const   userParam_);

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
static constexpr const char* const WINDOW_TITLE = "Level 01 - Act 11";

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
static glm::vec3 backgroundColor = {0.1f, 0.1f, 0.1f};

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
static float timeScale = 1.0f;

/**
 * @brief 선으로 그릴지 여부. (true: 선, false: 점)
 */
static bool isLineMode = false;

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
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
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(window, 100, 100);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, OnKeyInteracted);
    glfwSetMouseButtonCallback(window, OnButtonInteracted);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OnDebugMessage, nullptr);

    const std::string vertexShaderFile   = GetFile("Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile   = GetFile("Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    static Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.Use();

    static float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        const float currentTime = static_cast<float>(glfwGetTime());
        const float deltaTime   = (currentTime - lastTime) * timeScale;

        lastTime = currentTime;

        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

Shader::Shader(const char* const vertexSource_,
               const char* const fragmentSource_) noexcept
{
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource_, nullptr);
    glCompileShader(vertexShader);

    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource_, nullptr);
    glCompileShader(fragmentShader);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    glLinkProgram(programID);

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

void OnDebugMessage(const GLenum        source_,
                    const GLenum        type_,
                    const GLuint        id_,
                    const GLenum        severity_,
                    const GLsizei       length_,
                    const GLchar* const message_,
                    const void* const   userParam_)
{
    if (type_ == GL_DEBUG_TYPE_ERROR)
    {
        std::cerr << std::format("GL ERROR: type = {:x}, severity = {:x}, message = {}\n",
                type_, severity_, message_);
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
        case GLFW_KEY_P:
        {
            break;
        }
        case GLFW_KEY_1:
        {
            break;
        }
        case GLFW_KEY_2:
        {
            break;
        }
        case GLFW_KEY_3:
        {
            break;
        }
        case GLFW_KEY_4:
        {
            break;
        }
        case GLFW_KEY_5:
        {
            break;
        }
        case GLFW_KEY_C:
        {
            break;
        }
        case GLFW_KEY_UP:
        {
            if (constexpr float MAX_TIME_SCALE = 30.0f; (timeScale += 1.0f) > MAX_TIME_SCALE)
            {
                timeScale = MAX_TIME_SCALE;
                std::cout << std::format("[Warning] Time scale cannot be greater than {:.0f}.\n", MAX_TIME_SCALE);
                break;
            }

            std::cout << std::format("[Info] Time scale set to {:.1f}.\n", timeScale);
            break;
        }
        case GLFW_KEY_DOWN:
        {
            if (constexpr float MIN_TIME_SCALE = 1.0f; (timeScale -= 1.0f) < MIN_TIME_SCALE)
            {
                timeScale = MIN_TIME_SCALE;
                std::cout << std::format("[Warning] Time scale cannot be less than or equal to zero.\n");
                break;
            }

            std::cout << std::format("[Info] Time scale set to {:.1f}.\n", timeScale);
            break;
        }
        case GLFW_KEY_ENTER:
        {
            static float previousTimeScale = timeScale;

            if (timeScale != 0.0f)
            {
                previousTimeScale = timeScale;
                timeScale = 0.0f;
                std::cout << "[Info] Animation paused.\n";
            }
            else
            {
                timeScale = previousTimeScale;
                std::cout << "[Info] Animation resumed.\n";
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
    if ((button_ == GLFW_MOUSE_BUTTON_LEFT) && (action_ == GLFW_PRESS))
    {
        double x_, y_;
        glfwGetCursorPos(window_, &x_, &y_);

        const float ndcX = static_cast<float>(x_) - WINDOW_WIDTH / 2.0f;
        const float ndcY = (WINDOW_HEIGHT / 2.0f) - static_cast<float>(y_);

        const glm::vec2 cursorPosition = {ndcX, ndcY};


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