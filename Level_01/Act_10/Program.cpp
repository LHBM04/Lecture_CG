#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#include <glad/glad.h>

#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
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

/**
 * @class Triangle
 *
 * @brief 삼각형을 정의합니다.
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
     * @brief 해당 삼각형과 지정한 점이 접촉하는지 여부를 반환합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    void Draw(const Shader& shader_) const noexcept;

    /**
     * @brief 해당 삼각형의 위치를 반환합니다.
     *
     * @return glm::vec2 해당 삼각형의 위치.
     */
    [[nodiscard]]
    constexpr glm::vec2 GetPosition() const noexcept
    {
        return position;
    }

    /**
     * @brief 해당 삼각형의 위치를 설정합니다.
     *
     * @param position_ 설정할 위치.
     */
    inline void SetPosition(const glm::vec2& position_) noexcept
    {
        position = position_;
    }

    /**
     * @brief 해당 삼각형의 크기를 반환합니다.
     *
     * @return float 해당 삼각형의 크기.
     */
    [[nodiscard]]
    constexpr float GetSize() const noexcept
    {
        return size;
    }

    /**
     * @brief 해당 삼각형의 크기를 설정합니다.
     *
     * @param size_ 설정할 크기.
     */
    inline void SetSize(const float size_) noexcept
    {
        size = size_;
    }

    /**
     * @brief 해당 삼각형의 색상을 반환합니다.
     *
     * @return glm::vec3 해당 삼각형의 색상.
     */
    [[nodiscard]]
    constexpr glm::vec3 GetColor() const noexcept
    {
        return color;
    }

    /**
     * @brief 해당 삼각형의 색상을 설정합니다.
     *
     * @param color_ 설정할 색상.
     */
    inline void SetColor(const glm::vec3& color_) noexcept
    {
        color = color_;
    }

    /**
     * @brief 해당 삼각형의 이동 방향을 반환합니다.
     *
     * @return float 해당 삼각형의 이동 방향.
     */
    [[nodiscard]]
    constexpr glm::vec2 GetVelocity() const noexcept
    {
        return velocity;
    }

    /**
     * @brief 해당 삼각형의 이동 방향을 설정합니다.
     *
     * @param velocity_ 설정할 이동 방향.
     */
    inline void SetVelocity(const glm::vec2& velocity_) noexcept
    {
        velocity = velocity_;
    }

    /**
     * @brief 해당 삼각형이 지정한 점과 접촉하는지 여부를 반환합니다.
     *
     * @return bool 접촉 여부.
     */
    [[nodiscard]]
    constexpr bool IsInteract(const glm::vec2 point_) const noexcept
    {
        return point_.x >= position.x - size * 0.25f && point_.x <= position.x + size * 0.25f &&
               point_.y >= position.y - size * 0.25f && point_.y <= position.y + size * 0.75f;
    }
private:
    /**
     * @brief 해당 삼각형의 정점 배열 객체.
     */
    unsigned int vao;

    /**
     * @brief 해당 삼각형의 정점 버퍼 객체.
     */
    unsigned int vbo;

    /**
     * @brief 해당 삼각형의 요소 배열 객체.
     */
    unsigned int ebo;

    /**
     * @brief 해당 삼각형의 위치.
     */
    glm::vec2 position;

    /**
     * @brief 해당 삼각형의 크기.
     */
    float size;

    /**
     * @brief 해당 삼각형의 색상.
     */
    glm::vec3 color;

    /**
     * @brief 해당 삼각형의 속도.
     */
    glm::vec2 velocity;
};

/**
 * @class Animation
 *
 * @brief 애니메이션을 정의합니다.
 */
class Animation
{
public:
    /**
     * @brief 소멸자.
     */
    virtual ~Animation() = default;

    /**
     * @brief 애니메이션을 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(const float deltaTime_) noexcept = 0;
};

/**
 * @class BounceAnimation
 *
 * @brief 튕기는 애니메이션을 정의합니다.
 */
class BounceAnimation final : public Animation
{
public:
    /**
     * @brief 애니메이션을 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(const float deltaTime_) noexcept override;
};

/**
 * @class DiagonalAnimation
 *
 * @brief 대각선 애니메이션을 정의합니다.
 */
class ZigzagAnimation final : public Animation
{
public:
    /**
     * @brief 애니메이션을 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(const float deltaTime_) noexcept override;
};

/**
 * @class OrthogonalAnimation
 *
 * @brief 직각 이동 애니메이션을 정의합니다.
 */
class OrthogonalAnimation final : public Animation
{
public:
    /**
     * @brief 애니메이션을 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(const float deltaTime_) noexcept override;
};

/**
 * @class SpiralAnimation
 *
 * @brief 나선형 애니메이션을 정의합니다.
 */
class SpiralAnimation final : public Animation
{
public:
    /**
     * @brief 애니메이션을 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(const float deltaTime_) noexcept override;
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
 * @brief 애니메이션을 시작합니다.
 *
 * @tparam TAnimation 시작할 애니메이션 타입.
 */
template <typename TAnimation>
    requires std::is_base_of_v<Animation, TAnimation>
static void StartAnimation();

/**
 * @brief 애니메이션을 중지합니다.
 */
static void StopAnimation() noexcept;

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
static glm::vec2 cursorPosition = glm::vec2(0, 0);

/**
 * @brief 배경 색상.
 */
static glm::vec3 backgroundColor =  glm::vec3(0.1f, 0.1f, 0.1f);

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
 * @brief 월드 내 있을 수 있는 삼각형의 최대 갯수.
 */
static constexpr std::size_t MAX_TRIANGLE_COUNTS = 10;

/**
 * @brief 월드 내 모든 삼각형.
 */
static std::vector<std::unique_ptr<Triangle>> triangles;

/**
 * @brief 현재 재생 중인 애니메이션.
 */
static std::unique_ptr<Animation> currentAnimation = nullptr;

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
    glfwSetCursorPosCallback(window, OnCursorMoved);

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

    const Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.Use();

    static float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(backgroundColor.r, backgroundColor.b, backgroundColor.g, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        for (const auto& triangle : triangles)
        {
            triangle->Draw(shader);
        }

        if (currentAnimation != nullptr)
        {
            const float currentTime = static_cast<float>(glfwGetTime());
            const float deltaTime   = (currentTime - lastTime) * timeScale;

            currentAnimation->Update(deltaTime);

            lastTime = currentTime;
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
    , position(position_)
    , size(size_)
    , color(color_)
    , velocity(0.0f, 0.0f)
{
    constexpr std::array<GLfloat, 6> vertices
    {
         0.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f
    };

    constexpr std::array<GLuint, 3> indices
    {
        0, 1, 2
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

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
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, size, 1.0f));

    glm::mat4 projection = glm::ortho(
        -static_cast<float>(WINDOW_WIDTH)  / 2.0f, // 왼쪽
         static_cast<float>(WINDOW_WIDTH)  / 2.0f, // 오른쪽
        -static_cast<float>(WINDOW_HEIGHT) / 2.0f, // 아래
         static_cast<float>(WINDOW_HEIGHT) / 2.0f, // 위
        -1.0f,  // near
         1.0f   // far
    );

    const GLint projLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
    glUniform3fv(colorLoc, 1, &color[0]);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void BounceAnimation::Update(const float deltaTime_) noexcept
{
    for (const std::unique_ptr<Triangle>& triangle : triangles)
    {
        glm::vec2   position = triangle->GetPosition();
        glm::vec2   velocity = triangle->GetVelocity();
        const float size     = triangle->GetSize();

        position += velocity * deltaTime_;

        const float halfTriW = 0.5f * size;
        const float halfTriH = 0.5f * size;

        constexpr float halfWorldW = WINDOW_WIDTH  / 2.0f;
        constexpr float halfWorldH = WINDOW_HEIGHT / 2.0f;

        if (position.x - halfTriW < -halfWorldW ||
            position.x + halfTriW >  halfWorldW)
        {
            velocity.x = -velocity.x;
            position.x = glm::clamp(position.x, -halfWorldW + halfTriW, halfWorldW - halfTriW);
        }

        if (position.y - halfTriH < -halfWorldH ||
            position.y + halfTriH >  halfWorldH)
        {
            velocity.y = -velocity.y;
            position.y = glm::clamp(position.y, -halfWorldH + halfTriH, halfWorldH - halfTriH);
        }

        triangle->SetPosition(position);
        triangle->SetVelocity(velocity);
    }
}

void ZigzagAnimation::Update(const float deltaTime_) noexcept
{
    for (const std::unique_ptr<Triangle>& triangle : triangles)
    {
        glm::vec2   position = triangle->GetPosition();
        glm::vec2   velocity = triangle->GetVelocity();
        const float size     = triangle->GetSize();

        const float halfWorldW = WINDOW_WIDTH  / 2.0f;
        const float halfWorldH = WINDOW_HEIGHT / 2.0f;
        
        position.x += velocity.x * deltaTime_;

        if (position.x - size < -halfWorldW ||
            position.x + size > halfWorldW)
        {
            velocity.x = -velocity.x;
            position.x = glm::clamp(position.x, -halfWorldW + size, halfWorldW - size);

            const float step = size * 2.0f;
            position.y -= step;
        }

        if (position.y - size < -halfWorldH)
        {
            velocity = -velocity;
        }

        triangle->SetPosition(position);
        triangle->SetVelocity(velocity);
    }
}

void OrthogonalAnimation::Update(const float deltaTime_) noexcept
{
    for (const std::unique_ptr<Triangle>& triangle : triangles)
    {
        glm::vec2   position = triangle->GetPosition();
        glm::vec2   velocity = triangle->GetVelocity();
        const float size     = triangle->GetSize();

        const float halfWorldW = WINDOW_WIDTH  / 2.0f;
        const float halfWorldH = WINDOW_HEIGHT / 2.0f;

        position.x += velocity.x * deltaTime_;
        position.y += velocity.y * deltaTime_;

        if (position.x - size < -halfWorldW ||
            position.x + size >  halfWorldW)
        {
            velocity.x = -velocity.x;
            position.x = glm::clamp(position.x, -halfWorldW + size, halfWorldW - size);
        }

        if (position.y - size < -halfWorldH ||
            position.y + size >  halfWorldH)
        {
            velocity.y = -velocity.y;
            position.y = glm::clamp(position.y, -halfWorldH + size, halfWorldH - size);
        }

        triangle->SetPosition(position);
        triangle->SetVelocity(velocity);
    }
}

void SpiralAnimation::Update(const float deltaTime_) noexcept
{
    for (const std::unique_ptr<Triangle>& triangle : triangles)
    {
        glm::vec2   position = triangle->GetPosition();
        glm::vec2   velocity = triangle->GetVelocity();
        const float size     = triangle->GetSize();

        const float halfWorldW = WINDOW_WIDTH  / 2.0f;
        const float halfWorldH = WINDOW_HEIGHT / 2.0f;

        position += velocity * deltaTime_;

        const float speed = glm::length(velocity);

        if (speed > 0)
        {
            constexpr float rotationRate = 1.0f;
            const float     angle        = rotationRate * deltaTime_;
            const float     cosA         = std::cos(angle);
            const float     sinA         = std::sin(angle);
            const float     shrinkRate = 0.98f;

            glm::vec2 newVelocity;

            newVelocity.x = (velocity.x * cosA - velocity.y * sinA) * shrinkRate;
            newVelocity.y = (velocity.x * sinA + velocity.y * cosA) * shrinkRate;

            velocity = newVelocity;
        }

        if (position.x - size < -halfWorldW ||
            position.x + size >  halfWorldW)
        {
            velocity.x = -velocity.x;
            position.x = glm::clamp(position.x, -halfWorldW + size, halfWorldW - size);
        }

        if (position.y - size < -halfWorldH ||
            position.y + size >  halfWorldH)
        {
            velocity.y = -velocity.y;
            position.y = glm::clamp(position.y, -halfWorldH + size, halfWorldH - size);
        }

        triangle->SetPosition(position);
        triangle->SetVelocity(velocity);
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
        case GLFW_KEY_1:
        {
            StartAnimation<BounceAnimation>();
            std::cout << "[Info] Bounce animation started.\n";
            break;
        }
        case GLFW_KEY_2:
        {
            StartAnimation<ZigzagAnimation>();
            std::cout << "[Info] Zigzag animation started.\n";
            break;
        }
        case GLFW_KEY_3:
        {
            StartAnimation<OrthogonalAnimation>();
            std::cout << "[Info] Orthogonal animation started.\n";
            break;
        }
        case GLFW_KEY_4:
        {
            StartAnimation<SpiralAnimation>();
            std::cout << "[Info] Spiral animation started.\n";
            break;
        }
        case GLFW_KEY_0:
        {
            StopAnimation();
            std::cout << "[Info] Animation stopped.\n";
            break;
        }
        case GLFW_KEY_UP:
        {
            timeScale += 1.0f;
            std::cout << std::format("[Info] Time scale set to {:.1f}.\n", timeScale);
            break;
        }
        case GLFW_KEY_DOWN:
        {
            if (timeScale <= 1.0f)
            {
                std::cout << "[Warning] Time scale cannot be less than or equal to zero.\n";
                break;
            }

            timeScale -= 1.0f;
            std::cout << std::format("[Info] Time scale set to {:.1f}.\n", timeScale);
            break;
        }
        case GLFW_KEY_C:
        {
            currentAnimation.reset();
            triangles.clear();
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
        if (triangles.size() >= MAX_TRIANGLE_COUNTS)
        {
            std::cout << "[Warning] Cannot create more triangles!\n";
            return;
        }

        const glm::vec2 position = cursorPosition;

        std::uniform_real_distribution<float> sizeDist(10.0f, 50.0f);
        const float size = sizeDist(gen);

        std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
        const glm::vec3 color = glm::vec3(colorDist(gen), colorDist(gen), colorDist(gen));

        triangles.push_back(std::make_unique<Triangle>(position, size, color));

        std::cout << std::format("[Info] New triangle created at ({:.1f}, {:.1f}). ", position.x, position.y)
                  << std::format("Total Counts: {:d}\n", static_cast<int>(triangles.size()));
    }
    else if (button_ == GLFW_MOUSE_BUTTON_RIGHT)
    {
        for (auto iter = triangles.begin(); iter != triangles.end(); ++iter)
        {
            if ((*iter)->IsInteract(cursorPosition))
            {
                triangles.erase(iter);

                std::cout << std::format("[Info] Triangle removed at ({:.1f}, {:.1f}). ", cursorPosition.x, cursorPosition.y)
                          << std::format("Total Counts: {:d}\n", static_cast<int>(triangles.size()) );

                return;
            }
        }
    }
}

void OnCursorMoved(GLFWwindow* const window_,
                   const double      x_,
                   const double      y_) noexcept
{
    const float ndcX = static_cast<float>(x_) - WINDOW_WIDTH / 2.0f;
    const float ndcY = (WINDOW_HEIGHT / 2.0f) - static_cast<float>(y_);

    cursorPosition = glm::vec2(ndcX, ndcY);

    // std::cout << std::format("[Trace] Cursor moved to ({:.1f}, {:.1f}).\n", cursorPosition.x, cursorPosition.y);
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

template <typename TAnimation>
    requires std::is_base_of_v<Animation, TAnimation>
void StartAnimation()
{
    std::uniform_real_distribution<float>velDist(-100.0f, 100.0f);

    for (const std::unique_ptr<Triangle>& triangle : triangles)
    {
        triangle->SetVelocity({ velDist(gen), velDist(gen) });
    }

    currentAnimation = std::make_unique<TAnimation>();
}

void StopAnimation() noexcept
{
    currentAnimation.reset();

    for (const std::unique_ptr<Triangle>& triangle : triangles)
    {
        triangle->SetVelocity({ 0.0f, 0.0f });
    }
}