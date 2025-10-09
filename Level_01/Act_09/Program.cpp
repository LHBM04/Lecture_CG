#include <array>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
     * @brief 컴파일 여부.
     */
    bool isCompiled = false;

    /**
     * @brief 셰이더 프로그램 ID.
     */
    unsigned int programID;
};

/**
 * @class Triangle
 *
 * @brief 삼각형 도형을 정의합니다.
 */
class Triangle final
{
public:
    /**
     * @brief 생성자.
     *
     * @param position_ 삼각형 위치.
     * @param size_     삼각형 크기.
     * @param color_    삼각형 색상.
     */
    explicit Triangle(const glm::vec2& position_,
                      const float      size_,
                      const glm::vec3& color_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Triangle() noexcept;

    /**
     * @brief 해당 삼각형을 그립니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    void Draw(const Shader& shader_) const noexcept;

private:
    /**
     * @brief 해당 도형의 정점 배열 객체.
     */
    GLuint vao;

    /**
     * @brief 해당 도형의 정점 버퍼 객체.
     */
    GLuint vbo;

    /**
     * @brief 해당 도형의 요소 배열 객체.
     */
    GLuint ebo;

    /**
     * @brief 해당 도형의 위치.
     */
    glm::vec2 currentPosition;

    /**
     * @brief 해당 도형의 크기.
     */
    float size;

    /**
     * @brief 해당 도형의 색상.
     */
    glm::vec3 color;
};

class Quadrant final
{
public:
    explicit Quadrant(const float x_,
                      const float y_,
                      const float width_,
                      const float height_) noexcept;

    ~Quadrant() noexcept;

    /**
     * @brief 해당 사분면을 초기화합니다.
     *
     * @param shouldAddOne_ 삼각형을 하나 추가할지 여부.
     */
    void Reset(const bool shouldAddOne_) noexcept;

    /**
     * @brief 해당 사분면에 삼각형을 추가합니다.
     *
     * @param position_ 삼각형 위치.
     * @param size_     삼각형 크기.
     * @param color_    삼각형 색상.
     */
    void Add(const glm::vec2& position_,
             const float      size_,
             const glm::vec3& color_) noexcept;

    /**
     * @brief 해당 사분면에 삼각형을 추가합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    void Draw(const Shader& shader_) const noexcept;

    /**
     * @brief 해당 사분면이 포화 상태인지에 대한 여부를 반환합니다.
     *
     * @return bool 포화 상태 여부.
     */
    [[nodiscard]]
    constexpr bool IsFull() const noexcept
    {
        return triangles.size() >= MAX_TRIANGLE_COUNTS;
    }

    /**
   * @brief 해당 사분면과 지정한 점이 접촉하는지 여부를 반환합니다.
   *
   * @param position_ 지정할 점.
   *
   * @return bool 접촉 여부.
   */
    [[nodiscard]]
    constexpr bool IsInteract(const glm::vec2& position_) const noexcept
    {
        return position_.x >= x && position_.x <= x + width &&
               position_.y >= y && position_.y <= y + height;
    }
private:
    /**
     * @brief 해당 사분면의 정점 배열 객체.
     */
    GLuint vao;

    /**
     * @brief 해당 사분면의 정점 버퍼 객체.
     */
    GLuint vbo;

    /**
     * @brief x.
     */
    float x;

    /**
     * @brief y.
     */
    float y;

    /**
     * @brief 너비.
     */
    float width;

    /**
     * @brief 높이.
     */
    float height;

    /**
     * @brief 최대 삼각형 개수.
     */
    static constexpr std::size_t MAX_TRIANGLE_COUNTS = 4;

    /**
     * @brief 해당 사분면에 위치하는 삼각형들.
     */
    std::vector<std::unique_ptr<Triangle>> triangles;
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
static void GLAPIENTRY OnDebugMessage(GLenum        source_,
                                       GLenum        type_,
                                       GLuint        id_,
                                       GLenum        severity_,
                                       GLsizei       length_,
                                       const GLchar* message_,
                                       const void*   userParam_);

/**
 * @brief 키와 상호작용할 때 호출됩니다.
 *
 * @param window_   윈도우.
 * @param key_      눌린 키.
 * @param scancode_ 스캔 코드.
 * @param action_   키 액션.
 * @param mods_     수정자 키 상태.
 */
static void OnKeyInteracted(GLFWwindow* window_,
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
static void OnButtonInteracted(GLFWwindow* const window_,
                               const int         button_,
                               const int         action_,
                               const int         mods_) noexcept;

/**
 * @brief 커서가 움직일 때 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param x_      마우스 X 좌표.
 * @param y_      마우스 Y 좌표.
 */
static void OnCursorMoved(GLFWwindow* const window_,
                          const double      x_,
                          const double      y_) noexcept;

/**
 * @brief 셰이더 소스 코드.
 */
static std::string GetFile(const std::filesystem::path& path_);

/**
 * @brief GLFW 윈도우.
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
 * @brief 애플리케이션 타이틀.
 */
static constexpr const char* const WINDOW_TITLE = "Level 01 - Act 09";

/**
 * @brief GL 메이저 버전.
 */
static constexpr unsigned char CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief GL 마이너 버전.
 */
static constexpr unsigned char CONTEXT_MINOR_VERSION = 5;

/**
 * @brief 커서의 위치.
 */
static glm::vec2 cursorPosition;

/**
 * @brief 난수 생성기.
 */
static std::random_device rd;

/**
 * @brief 난수 생성 엔진.
 */
static std::mt19937 gen(rd());

/**
 * @brief 선으로 그릴지 여부.
 */
static bool shouldDrawWithLines = false;

/**
 * @brief 사분면들.
 */
static std::array<std::unique_ptr<Quadrant>, 4> quadrants;

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

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OnDebugMessage, nullptr);

    glfwSetKeyCallback(window, OnKeyInteracted);
    glfwSetMouseButtonCallback(window, OnButtonInteracted);
    glfwSetCursorPosCallback(window, OnCursorMoved);

    const std::string vertexShaderFile   = GetFile("Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile   = GetFile("Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    const Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.Use();

    constexpr float halfWidth  = WINDOW_WIDTH / 2.0f;
    constexpr float halfHeight = WINDOW_HEIGHT / 2.0f;

    quadrants[0] = std::make_unique<Quadrant>(halfWidth, halfHeight, halfWidth, halfHeight);
    quadrants[1] = std::make_unique<Quadrant>(0.0f,      halfHeight, halfWidth, halfHeight);
    quadrants[2] = std::make_unique<Quadrant>(0.0f,      0.0f,       halfWidth, halfHeight);
    quadrants[3] = std::make_unique<Quadrant>(halfWidth, 0.0f,       halfWidth, halfHeight);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, shouldDrawWithLines ? GL_LINE : GL_FILL);

        for (const std::unique_ptr<Quadrant>& quadrant : quadrants)
        {
            quadrant->Draw(shader);
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

Triangle::Triangle(const glm::vec2& position_,
                   const float      size_,
                   const glm::vec3& color_) noexcept
    : vao(0)
    , vbo(0)
    , ebo(0)
    , currentPosition(position_)
    , size(size_)
    , color(color_)
{
    constexpr std::array<GLfloat, 6> vertices_
    {
         0.0f,  1.0f,
        -0.25f, -0.25f,
         0.25f, -0.25f
    };

    constexpr std::array<GLuint, 3> indices_
    {
        0, 1, 2
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(GLfloat), vertices_.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), indices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Triangle::~Triangle() noexcept
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Triangle::Draw(const Shader& shader_) const noexcept
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(currentPosition, 0.0f));
    model = glm::scale(model, glm::vec3(size, size, 1.0f));

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
    glUniform3fv(colorLoc, 1, &color[0]);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

Quadrant::Quadrant(const float x_,
                   const float y_,
                   const float width_,
                   const float height_) noexcept
    : vao(0)
    , vbo(0)
    , x(x_)
    , y(y_)
    , width(width_)
    , height(height_)
    , triangles(MAX_TRIANGLE_COUNTS)
{
    const float x2 = x_ + width_;
    const float y2 = y_ + height_;

    const std::array<glm::vec2, 4> vertices_pixels
    {
        glm::vec2{x_, y_}, // 좌측 하단
        glm::vec2{x2, y_}, // 우측 하단
        glm::vec2{x2, y2}, // 우측 상단
        glm::vec2{x_, y2} // 좌측 상단
    };

    std::array<glm::vec2, 4> vertices_ndc{ };
    for (size_t i = 0; i < vertices_pixels.size(); ++i)
    {
        vertices_ndc[i].x = (vertices_pixels[i].x / 1200 * 2.0f) - 1.0f;
        vertices_ndc[i].y = (vertices_pixels[i].y / 600 * 2.0f) - 1.0f;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices_ndc.size(), vertices_ndc.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Reset(true);
}

Quadrant::~Quadrant() noexcept
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    triangles.clear();
}

void Quadrant::Reset(const bool shouldAddOne_) noexcept
{
    triangles.clear();

    if (shouldAddOne_)
    {
        const float ndcX = ((x + width / 2.0f) / WINDOW_WIDTH * 2.0f) - 1.0f;
        const float ndcY = (((WINDOW_HEIGHT - (y + height / 2.0f))) / WINDOW_HEIGHT * 2.0f) - 1.0f;
        const glm::vec2 currentPosition{ndcX, ndcY};

        std::uniform_real_distribution<float> sizeDist(0.1f, 0.25f);
        const float size{ sizeDist(gen) };

        std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
        const glm::vec3 color{ colorDist(gen), colorDist(gen), colorDist(gen) };

        triangles.emplace_back(std::make_unique<Triangle>(currentPosition, size, color));
    }
}

void Quadrant::Add(const glm::vec2& position_,
                   const float      size_,
                   const glm::vec3& color_) noexcept
{
    if (IsFull())
    {
        triangles.erase(triangles.begin());
    }

    triangles.emplace_back(std::make_unique<Triangle>(position_, size_, color_));
}

void Quadrant::Draw(const Shader& shader_) const noexcept
{
    for (const std::unique_ptr<Triangle>& triangle : triangles)
    {
        triangle->Draw(shader_);
    }

    constexpr glm::mat4 model       = glm::mat4(1.0f);
    constexpr glm::vec3 borderColor = {1.0f, 1.0f, 1.0f};

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
    glUniform3fv(colorLoc, 1, &borderColor[0]);

    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glBindVertexArray(0);
    glLineWidth(1.0f);
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
        case GLFW_KEY_A:
        {
            shouldDrawWithLines = false;
            std::cout << "[Info] Filled mode activated.\n";
            break;
        }
        case GLFW_KEY_B:
        {
            shouldDrawWithLines = true;
            std::cout << "[Info] Line mode activated.\n";
            break;
        }
        case GLFW_KEY_C:
        {
            for (std::unique_ptr<Quadrant>& quadrant : quadrants)
            {
                quadrant->Reset(true);
            }
            std::cout << "[Info] All triangles cleared.\n";
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
    if (action_ != GLFW_PRESS)
    {
        return;
    }

    if (button_ == GLFW_MOUSE_BUTTON_LEFT)
    {
        for (const auto& quadrant : quadrants)
        {
            if (quadrant->IsInteract(cursorPosition))
            {
                quadrant->Reset(false);

                const float ndcX = (cursorPosition.x / WINDOW_WIDTH * 2.0f) - 1.0f;
                const float ndcY = ((WINDOW_HEIGHT - cursorPosition.y) / WINDOW_HEIGHT * 2.0f) - 1.0f;
                const glm::vec2 currentPosition = {ndcX, ndcY};

                std::uniform_real_distribution<float> sizeDist(0.1f, 0.25f);
                const float size{ sizeDist(gen) };

                std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
                const glm::vec3 color{ colorDist(gen), colorDist(gen), colorDist(gen) };

                quadrant->Add(currentPosition, size, color);

                break;
            }
        }
    }
    else if (button_ == GLFW_MOUSE_BUTTON_RIGHT)
    {
        for (const auto& quadrant : quadrants)
        {
            if (quadrant->IsInteract(cursorPosition))
            {
                const float ndcX = (cursorPosition.x / WINDOW_WIDTH * 2.0f) - 1.0f;
                const float ndcY = ((WINDOW_HEIGHT - cursorPosition.y) / WINDOW_HEIGHT * 2.0f) - 1.0f;
                const glm::vec2 currentPosition = {ndcX, ndcY};

                std::uniform_real_distribution<float> sizeDist(0.1f, 0.25f);
                const float size{ sizeDist(gen) };

                std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
                const glm::vec3 color{ colorDist(gen), colorDist(gen), colorDist(gen) };

                quadrant->Add(currentPosition, size, color);

                break;
            }
        }
    }
}

void OnCursorMoved(GLFWwindow* const window_,
                   const double      x_,
                   const double      y_) noexcept
{
    cursorPosition.x = static_cast<float>(x_);
    cursorPosition.y = static_cast<float>(y_);
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