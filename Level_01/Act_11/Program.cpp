#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#include <glad/glad.h>

#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

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
     *
     * @param center_ 중심 좌표.
     */
    explicit Spiral(const glm::vec2 center_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Spiral() noexcept;

    /**
     * @brief 스파이럴을 그립니다.
     *
     * @param shader_    사용할 셰이더.
     * @param deltaTime_ 시간 변화량.
     */
    void Draw(const Shader& shader_,
              const float   deltaTime_) noexcept;
private:
    /**
     * @brief 중심점.
     */
    static constexpr float pitch = 28.0f;

    /**
     * @brief 회전 수.
     */
    static constexpr float turns = 3.0f;

    /**
     * @brief 시계 방향 여부.
     */
    static constexpr float spacing = 8.0f;

    /**
     * @brief 시작 각도.
     */
    static constexpr float start = 0.0f;

    /**
     * @brief 정점 배열 객체.
     */
    unsigned int vao = 0;

    /**
     * @brief 정점 버퍼 객체.
     */
    unsigned int vbo = 0;

    /**
     * @brief 현재 위치.
     */
    glm::vec2 position;

    /**
     * @brief 점들.
     */
    std::vector<glm::vec2> points;

    /**
     * @brief 현재 진행률.
     */
    float progress = 0.0f;
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
 * @brief 스파이럴 점들을 생성합니다.
 *
 * @param pitch_         피치.
 * @param turns_         회전 수.
 * @param spacing_       점 간격.
 * @param start_         시작 각도.
 * @param shouldReverse_ 시계 반대 방향 여부.
 *
 * @return std::vector<glm::vec2> 생성된 점들.
 */
static std::vector<glm::vec2> GenerateSpiralPoints(const float     pitch_,
                                                   const float     turns_,
                                                   const float     spacing_,
                                                   const float     start_,
                                                   const bool      shouldReverse_);

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
 * @brief 원주율.
 */
static constexpr float PI  = 3.14159265358979323846f;

/**
 * @brief 스파이럴을 선으로 그릴지 여부. (true: 선, false: 점)
 */
static constinit bool isLineMode = false;

/**
 * @brief 월드 내 스파이럴들.
 */
static std::vector<std::unique_ptr<Spiral>> spirals;

/**
 * @brief 월드 내 있을 수 있는 스파이럴의 최대 갯수.
 */
static std::size_t maxSpiralCount = 5;

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

        for (std::unique_ptr<Spiral>& spiral : spirals)
        {
            spiral->Draw(shader, deltaTime);
        }

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

Spiral::Spiral(const glm::vec2 center_) noexcept
    : position(center_)
{
    std::uniform_int_distribution<int> distReverse(0, 1);

    if (static_cast<bool>(distReverse(gen)))
    {
        std::vector<glm::vec2> left  = GenerateSpiralPoints(pitch, turns, spacing, start, true);
        std::vector<glm::vec2> right = GenerateSpiralPoints(pitch, turns, spacing, start, false);

        std::ranges::reverse(right);

        for (auto& p : right) { p.x = 170 - p.x; }

        points.insert(points.end(), left.begin(), left.end());
        points.insert(points.end(), right.begin(), right.end());
    }
    else
    {
        std::vector<glm::vec2> left  = GenerateSpiralPoints(pitch, turns, spacing, start, true);
        std::vector<glm::vec2> right = GenerateSpiralPoints(pitch, turns, spacing, start, false);

        for (auto& p : left) { p.x = 170 - p.x; }

        std::ranges::reverse(left);

        points.insert(points.end(), right.begin(), right.end());
        points.insert(points.end(), left.begin(), left.end());
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), points.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

    glBindVertexArray(0);
}

Spiral::~Spiral() noexcept
{
    if (vbo)
    {
        glDeleteBuffers(1, &vbo);
    }

    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
    }
}

void Spiral::Draw(const Shader& shader_,
                  const float   deltaTime_) noexcept
{
    constexpr float thetaMax = 2.0f * PI * turns;

    if ((progress += 2.5f * deltaTime_) > thetaMax)
    {
        progress = thetaMax;
    }

    const size_t maxCount = (progress / thetaMax) * points.size();
    if (maxCount == 0)
    {
        return;
    }

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

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

    glBindVertexArray(vao);
    glPointSize(3.0f);
    glDrawArrays(isLineMode ? GL_LINE_STRIP : GL_POINTS, 0, static_cast<GLsizei>(maxCount));
    glBindVertexArray(0);
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
            isLineMode = !isLineMode;
            std::cout << (isLineMode ? "[Info] Line mode.\n" : "[Info] Point mode.\n");

            break;
        }
        case GLFW_KEY_1:
        {
            if (spirals.size() >= 1)
            {
                std::cout << std::format("[Warning] Cannot set max spiral count to {:d} because there are already {:d} spirals.\n",
                        1, static_cast<int>(spirals.size()));
                break;
            }

            maxSpiralCount = 1;
            spirals.reserve(1);

            std::cout << std::format("[Info] Max spiral count set to {:d}.\n", static_cast<int>(maxSpiralCount));

            break;
        }
        case GLFW_KEY_2:
        {
            if (spirals.size() >= 2)
            {
                std::cout << std::format("[Warning] Cannot set max spiral count to {:d} because there are already {:d} spirals.\n",
                        2, static_cast<int>(spirals.size()));
                break;
            }

            maxSpiralCount = 2;
            spirals.reserve(2);

            std::cout << std::format("[Info] Max spiral count set to {:d}.\n", static_cast<int>(maxSpiralCount));

            break;
        }
        case GLFW_KEY_3:
        {
            if (spirals.size() >= 3)
            {
                std::cout << std::format("[Warning] Cannot set max spiral count to {:d} because there are already {:d} spirals.\n",
                       3, static_cast<int>(spirals.size()));
                break;
            }

            maxSpiralCount = 3;
            spirals.reserve(3);

            std::cout << std::format("[Info] Max spiral count set to {:d}.\n", static_cast<int>(maxSpiralCount));

            break;
        }
        case GLFW_KEY_4:
        {
            if (spirals.size() >= 4)
            {
                std::cout << std::format("[Warning] Cannot set max spiral count to {:d} because there are already {:d} spirals.\n",
                     4, static_cast<int>(spirals.size()));
                break;
            }

            maxSpiralCount = 4;
            spirals.reserve(4);

            std::cout << std::format("[Info] Max spiral count set to {:d}.\n", static_cast<int>(maxSpiralCount));

            break;
        }
        case GLFW_KEY_5:
        {
            if (spirals.size() >= 5)
            {
                std::cout << std::format("[Warning] Cannot set max spiral count to {:d} because there are already {:d} spirals.\n",
                     5, static_cast<int>(spirals.size()));
                break;
            }

            maxSpiralCount = 5;
            spirals.reserve(5);

            std::cout << std::format("[Info] Max spiral count set to {:d}.\n", static_cast<int>(maxSpiralCount));

            break;
        }
        case GLFW_KEY_C:
        {
            spirals.clear();
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
        if (spirals.size() >= maxSpiralCount)
        {
            std::cout << std::format("[Warning] Cannot create more than {:d} spirals.\n", static_cast<int>(maxSpiralCount));
            return;
        }

        double x_, y_;
        glfwGetCursorPos(window_, &x_, &y_);

        const float ndcX = static_cast<float>(x_) - WINDOW_WIDTH / 2.0f;
        const float ndcY = (WINDOW_HEIGHT / 2.0f) - static_cast<float>(y_);

        const glm::vec2 cursorPosition = {ndcX, ndcY};

        spirals.emplace_back(std::make_unique<Spiral>(cursorPosition));

        std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
        backgroundColor = {distColor(gen), distColor(gen), distColor(gen)};
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

std::vector<glm::vec2> GenerateSpiralPoints(const float     pitch_,
                                            const float     turns_,
                                            const float     spacing_,
                                            const float     start_,
                                            const bool      shouldReverse_)
{
    std::vector<glm::vec2> points;

    const float k         = pitch_ / (2.0f * PI);
    const float direction = shouldReverse_ ? -1.0f : 1.0f;

    const float thetaMax = 2.0f * PI * turns_;
    float       theta    = 0.0f;

    while (theta <= thetaMax)
    {
        const float range = k * theta;
        const float angle = start_ + (direction * theta);

        const float x = range * std::cos(angle);
        const float y = range * std::sin(angle);

        points.emplace_back(x, y);

        theta += spacing_ / std::sqrt(range * range + k * k);
    }

    return points;
}