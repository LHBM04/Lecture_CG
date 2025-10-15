#include <array>
#include <cmath>
#include <filesystem>
#include <format>
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
        return direction;
    }

    /**
     * @brief 해당 삼각형의 이동 방향을 설정합니다.
     *
     * @param velocity_ 설정할 이동 방향.
     */
    inline void SetDirection(const glm::vec2& velocity_) noexcept
    {
        direction = velocity_;
    }

    /**
     * @brief 해당 삼각형이 지정한 점과 접촉하는지 여부를 반환합니다.
     *
     * @return bool 접촉 여부.
     */
    [[nodiscard]]
    constexpr bool IsInteract(const glm::vec2 point_) const noexcept
    {
        return (point_.x >= position.x - size * 0.25f) &&
               (point_.x <= position.x + size * 0.25f) &&
               (point_.y >= position.y - size * 0.25f) &&
               (point_.y <= position.y + size * 0.75f);
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
    glm::vec2 direction;
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
private:
    /**
     * @brief 각 삼각형의 속도 벡터.
     */
    std::vector<glm::vec2> velocities;
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
private:
    static constexpr float speed = 200.0f;
    static constexpr float verticalStep = 80.0f;
    static constexpr float horizontalLength = 500.0f;
    static constexpr int   maxRows = 6;
    /**
     * @struct State
     * 
     * @brief 삼각형의 이동 정보를 정의합니다.
     */
    struct State
    {
        /**
         * @brief 애니메이션 시작 위치.
         */
        glm::vec2 originPosition;

        /**
         * @brief 현재 위치.
         */
        glm::vec2 currentPosition;

        /**
         * @brief 현재 방향.
         */
        glm::vec2 currentDirection;

        /**
         * @brief 현재 구간에 남은 거리.
         */
        float remaining; // 현재 구간에 남은 거리

        /**
         * @brief 현재 줄.
         */
        int row;         

        /**
         * @brief 현재 진행 방향 (true=→, false=←)
         */
        bool isGoingRight;
    };

    /**
     * @brief 각 삼각형들의 상태.
     */
    std::vector<State> states;
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
private:
    /**
	 * @struct State
     * 
     * @brief 삼각형의 이동 정보를 정의합니다.
     */
    struct State
    {
        glm::vec2 origin;
        glm::vec2 currentPosition;
        glm::vec2 currentDirection; 
        float segLeft; 
        int turnCount; 
        int lenFactor;
    };
    
    /**
	 * @brief 각 코너 간의 거리(픽셀).
     */
    static constexpr float StepLen = 40.0f;

    /**
     * @brief 픽셀/초
     */
    static constexpr float Speed = 160.0f;

    /**
     * @brief 각 삼각형들의 상태.
     */
    std::vector<State> states;
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
private:
    struct State
    {
        glm::vec2 origin;
        float theta;
        float k;
        float r0;
        float speed;
    };
    
    std::vector<State> states;
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
static std::string ReadFile(const std::filesystem::path& path_);

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
static constexpr const char* const WINDOW_TITLE = "Level 01 - Act 10";

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

static float lastTime = 0.0f; // <<-- 여기로 이동 및 초기화

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

    const std::string vertexShaderFile   = ReadFile("Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile   = ReadFile("Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    const Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.Use();

    lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (currentAnimation != nullptr)
        {
            const float currentTime = static_cast<float>(glfwGetTime());
            const float deltaTime   = (currentTime - lastTime) * timeScale;

            currentAnimation->Update(deltaTime);

            lastTime = currentTime;
        }

        shader.Use();
        for (const auto& triangle : triangles)
        {
            triangle->Draw(shader);
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
    , direction(0.0f, 0.0f)
{
    constexpr std::array<float, 6> vertices
    {
         0.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f
    };

    constexpr std::array<unsigned int, 3> indices
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
    float angle = 0.0f;
    if (direction.x != 0.0f || direction.y != 0.0f)
    {
        angle = std::atan2(direction.y, direction.x) + glm::radians(270.0f);
    }

    glm::mat4 model = {1.0f};
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, size, 1.0f));

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
    if (velocities.size() != triangles.size())
    {
        std::uniform_real_distribution<float> directionDist(-1.0f, 1.0f);
        std::uniform_real_distribution<float> speedDist(100.0f, 250.0f);

        const float states = speedDist(gen);

        velocities.resize(triangles.size(), glm::vec2(directionDist(gen) * states, directionDist(gen) * states));
    }

    for (std::size_t index = 0; index < triangles.size(); ++index)
    {
        glm::vec2 currentPosition = triangles.at(index)->GetPosition();
        glm::vec2 direction = velocities.at(index);

        currentPosition += direction * deltaTime_;

        const float size = triangles.at(index)->GetSize();

        const float halfTriangleWidth  = 0.5f * size;
        const float halfTriangleHeight = 0.5f * size;

        constexpr float halfWorldWidth  = WINDOW_WIDTH  / 2.0f;
        constexpr float halfWorldHeight = WINDOW_HEIGHT / 2.0f;

        if (currentPosition.x - halfTriangleWidth < -halfWorldWidth ||
            currentPosition.x + halfTriangleWidth >  halfWorldWidth)
        {
            direction.x = -direction.x;

            const float max =  halfWorldWidth - halfTriangleWidth;
            const float min = -halfWorldWidth + halfTriangleWidth;
            currentPosition.x = glm::clamp(currentPosition.x, min, max); // x를 클램프
        }

        if (currentPosition.y - halfTriangleHeight < -halfWorldHeight ||
            currentPosition.y + halfTriangleHeight >  halfWorldHeight)
        {
            direction.y = -direction.y;

            const float max =  halfWorldHeight - halfTriangleHeight;
            const float min = -halfWorldHeight + halfTriangleHeight;
            currentPosition.y = glm::clamp(currentPosition.y, min, max);
        }

        triangles.at(index)->SetPosition(currentPosition);
        triangles.at(index)->SetDirection(direction);
        velocities.at(index) = direction;
    }
}

void ZigzagAnimation::Update(const float deltaTime_) noexcept
{
    if (states.size() != triangles.size())
    {
        states.clear();
        states.reserve(triangles.size());

        for (const auto& t : triangles)
        {
            State state = { };
            state.originPosition = t->GetPosition();
            state.currentPosition = state.originPosition;
            state.currentDirection = glm::vec2(1.0f, 0.0f);
            state.remaining = horizontalLength;
            state.row = 0;
            state.isGoingRight = true;

            states.push_back(state);

            t->SetPosition(state.currentPosition);
            t->SetDirection(state.currentDirection * speed);
        }
    }

    for (std::size_t i = 0; i < triangles.size(); ++i)
    {
        auto& state = states[i];
        auto& triangle = triangles[i];

        if (state.currentDirection.x != 0.0f)
        {
            state.currentPosition.x += state.currentDirection.x * speed * deltaTime_;

            const float horizontalBoundary = WINDOW_WIDTH / 2.0f;
            const float triangleSize = triangle->GetSize();
            bool hasHitWall = false;

            if (state.currentDirection.x > 0 && state.currentPosition.x > horizontalBoundary - triangleSize) // 오른쪽 벽
            {
                state.currentPosition.x = horizontalBoundary - triangleSize; // 벽에 붙이기
                hasHitWall = true;
            }
            else if (state.currentDirection.x < 0 && state.currentPosition.x < -horizontalBoundary + triangleSize) // 왼쪽 벽
            {
                state.currentPosition.x = -horizontalBoundary + triangleSize; // 벽에 붙이기
                hasHitWall = true;
            }

            if (hasHitWall)
            {
                state.currentDirection = glm::vec2(0.0f, -1.0f);
                state.remaining = verticalStep;
            }
        }
        else
        {
            float distanceToMove = speed * deltaTime_;

            if (distanceToMove < state.remaining)
            {
                state.currentPosition.y += state.currentDirection.y * distanceToMove;
                state.remaining -= distanceToMove;
            }
            else
            {
                state.currentPosition.y += state.currentDirection.y * state.remaining;
                state.remaining = 0;

                state.isGoingRight = !state.isGoingRight;
                state.currentDirection = glm::vec2(state.isGoingRight ? 1.0f : -1.0f, 0.0f);
                ++state.row;
            }
        }

        const float boundary = WINDOW_HEIGHT / 2.0f;
        const float triangleSize = triangle->GetSize();

        if (state.currentPosition.y < -boundary - triangleSize)
        {
            state.currentPosition.y = boundary + triangleSize;
        }

        triangle->SetPosition(state.currentPosition);
        triangle->SetDirection(state.currentDirection * speed);
    }
}

void OrthogonalAnimation::Update(const float deltaTime_) noexcept
{
    auto rot90 = [](const glm::vec2& v) -> glm::vec2 { return glm::vec2(-v.y, v.x); };

    if (states.size() != triangles.size())
    {
        states.clear();
        states.reserve(triangles.size());

        for (const auto& triangle : triangles)
        {
            State state = { };
            state.origin           = triangle->GetPosition();    // 각 삼각형 현재 위치를 중심으로 시작
            state.currentPosition  = state.origin;
            state.currentDirection = glm::vec2(1.0f, 0.0f); // +X 방향에서 시작
            state.turnCount        = 0;
            state.lenFactor        = 1;
            state.segLeft          = StepLen * static_cast<float>(state.lenFactor);
            states.push_back(state);

            triangle->SetDirection(state.currentDirection * Speed);
        }
    }

    for (std::size_t i = 0; i < triangles.size(); ++i)
    {
        auto& st = states[i];
        float dist = Speed * deltaTime_;

        while (dist > 0.0f)
        {
            if (dist < st.segLeft)
            {
                st.currentPosition += st.currentDirection * dist;
                st.segLeft = st.segLeft - dist;
                dist = 0.0f;
            }
            else
            {
                st.currentPosition += st.currentDirection * st.segLeft;
                dist -= st.segLeft;

                st.currentDirection = rot90(st.currentDirection);
                ++st.turnCount;

                if ((st.turnCount % 2) == 0)
                {
                    ++st.lenFactor;
                }
                st.segLeft = StepLen * static_cast<float>(st.lenFactor);
            }
        }

        triangles[i]->SetPosition(st.currentPosition);
        triangles[i]->SetDirection(st.currentDirection * Speed);

        constexpr float halfW     = WINDOW_WIDTH  * 0.5f;
        const float halfH     = WINDOW_HEIGHT * 0.5f;
        const float maxExtent = std::max(halfW, halfH) * 1.2f;

        if (glm::length(st.currentPosition - st.origin) > maxExtent)
        {
            st.currentPosition  = st.origin;
            st.currentDirection = glm::vec2(1.0f, 0.0f);
            st.turnCount        = 0;
            st.lenFactor        = 1;
            st.segLeft          = StepLen * float(st.lenFactor);
        }
    }
}

void SpiralAnimation::Update(const float deltaTime_) noexcept
{
    constexpr float kStep = 24.0f;                                          // k: 한 바퀴당 반경 증가량(픽셀/라디안)에 비례 (실감상 16~48 적당)
    constexpr float speed = 160.0f;                                         // 거의 일정한 화면상 속도(픽셀/초)
    constexpr float r0 = 0.0f;                                              // 시작 반경
    constexpr float maxOut = std::max(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.7f;

    if (states.size() != triangles.size())
    {
        states.clear();
        states.reserve(triangles.size());

        // 각 삼각형을 현재 위치를 중심으로 시작
        for (const auto& t : triangles)
        {
            State st{};
            st.origin = t->GetPosition();
            st.theta  = 0.0f;
            st.k      = kStep;
            st.r0     = r0;
            st.speed  = speed;
            states.push_back(st);

            // 초기 위치/방향 반영
            const float r   = st.r0 + st.k * st.theta;
            const float cs  = std::cos(st.theta);
            const float sn  = std::sin(st.theta);
            const glm::vec2 currentPosition = st.origin + glm::vec2(r * cs, r * sn);
            t->SetPosition(currentPosition);

            // 초기 접선 방향 (d/dθ [r(θ)(cosθ, sinθ)] = (r'cosθ - r sinθ, r' sinθ + r cosθ))
            const float rp  = st.k;
            glm::vec2 tangent(rp*cs - r*sn, rp*sn + r*cs);
            if (glm::dot(tangent, tangent) > 0.0f) tangent = glm::normalize(tangent);
            t->SetDirection(tangent * st.speed);
        }
    }

    // 업데이트
    for (std::size_t i = 0; i < triangles.size(); ++i)
    {
        auto& st = states[i];
        auto& tri = triangles[i];

        // 현재 r와 호길이 미분에 따른 theta 증가율
        const float r   = st.r0 + st.k * st.theta;
        const float dsd = std::sqrt(r*r + st.k*st.k); // ds/dθ
        float dtheta_dt = 0.0f;
        if (dsd > 0.0f)
        {
            dtheta_dt = st.speed / dsd;
        }

        st.theta += dtheta_dt * deltaTime_;

        // 새 좌표
        const float cs  = std::cos(st.theta);
        const float sn  = std::sin(st.theta);
        const float r2  = st.r0 + st.k * st.theta;
        const glm::vec2 currentPosition = st.origin + glm::vec2(r2 * cs, r2 * sn);
        tri->SetPosition(currentPosition);

        // 접선 벡터(정규화) → velocity 동기화(시각적 회전 재사용)
        const float rp  = st.k;
        glm::vec2 tangent(rp*cs - r2*sn, rp*sn + r2*cs);
        if (glm::dot(tangent, tangent) > 0.0f) tangent = glm::normalize(tangent);
        tri->SetDirection(tangent * st.speed);

        // 너무 멀리 나가면 리셋(원점에서 다시 시작)
        if (glm::length(currentPosition - st.origin) > maxOut)
        {
            st.theta = 0.0f;
        }
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

        const glm::vec2 currentPosition = cursorPosition;

        std::uniform_real_distribution<float> sizeDist(10.0f, 50.0f);
        const float size = sizeDist(gen);

        std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
        const glm::vec3 color = glm::vec3(colorDist(gen), colorDist(gen), colorDist(gen));

        triangles.push_back(std::make_unique<Triangle>(currentPosition, size, color));

        std::cout << std::format("[Info] New triangle created at ({:.1f}, {:.1f}). ", currentPosition.x, currentPosition.y)
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

std::string ReadFile(const std::filesystem::path& path_)
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
        triangle->SetDirection({ velDist(gen), velDist(gen) });
    }
    lastTime = static_cast<float>(glfwGetTime());
    currentAnimation = std::make_unique<TAnimation>();
}

void StopAnimation() noexcept
{
    currentAnimation.reset();

    for (const std::unique_ptr<Triangle>& triangle : triangles)
    {
        triangle->SetDirection({ 0.0f, 0.0f });
    }
}