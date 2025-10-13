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
    enum class Type : unsigned char
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
        Pentagon,
    };

    /**
     * @brief 생성자.
     *
     * @param position_ 도형의 위치.
     * @param type_     도형의 유형.
     */
    explicit Shape(const glm::vec2&   position_,
                   const Shape::Type& type_) noexcept;

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
    void Render(const Shader& shader_) noexcept;

    /**
     * @brief 도형을 다음 타입으로 변경합니다.
     */
    void NextTo() noexcept;
private:
    /**
 * @brief 도형의 모든 유형에 대한 정점 데이터.
 */
    static inline const std::unordered_map<Shape::Type, std::array<float, 10>> SHAPES_VERTICES
    {
        {Shape::Type::Line,      {-0.5f, 0.0f,  0.5f,    0.0f}},
        {Shape::Type::Triangle,  { 0.0f, 0.5f, -0.5f,   -0.5f,    0.5f,    -0.5f}},
        {Shape::Type::Rectangle, {-0.5f, 0.5f,  0.5f,    0.5f,    0.5f,    -0.5f, -0.5f, -0.5f}},
        {Shape::Type::Pentagon,  { 0.0f, 0.5f,  0.4755f, 0.1545f, 0.2939f, -0.4045f, -0.2939f, -0.4045f, -0.4755f, 0.1545f}},
    };

    /**
     * @brief 도형의 모든 유형에 대한 요소 데이터.
     */
    static inline const std::unordered_map<Shape::Type, std::vector<unsigned int>> SHAPE_INDICES
    {
        {Shape::Type::Line,      {0, 1}},
        {Shape::Type::Triangle,  {0, 1, 2}},
        {Shape::Type::Rectangle, {0, 1, 2, 2, 3, 0}},
        {Shape::Type::Pentagon,  {0, 1, 2, 0, 2, 3, 0, 3, 4}},
    };

    /**
     * @brief 도형의 모든 유형에 대한 그리기 기본형 데이터.
     */
    static inline const std::unordered_map<Shape::Type, GLenum> SHAPE_PRIMITIVES
    {
            {Shape::Type::Line,      GL_LINES},
            {Shape::Type::Triangle,  GL_TRIANGLES},
            {Shape::Type::Rectangle, GL_TRIANGLES},
            {Shape::Type::Pentagon,  GL_TRIANGLES},
        };

    /**
     * @brief 도형의 모든 유형에 대한 색상 데이터.
     */
    static inline const std::unordered_map<Shape::Type, glm::vec3> SHAPE_COLORS
    {
        {Shape::Type::Line,      {1.0f, 0.0f, 0.0f}},
        {Shape::Type::Triangle,  {0.0f, 1.0f, 0.0f}},
        {Shape::Type::Rectangle, {0.0f, 0.0f, 1.0f}},
        {Shape::Type::Pentagon,  {1.0f, 1.0f, 0.0f}},
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
     * @brief 현재 타입.
     */
    Shape::Type type;

    /**
     * @brief 현재 도형이 변신해야 할
     */
    Shape::Type goal;

    /**
     * @brief 현재 위치.
     */
    glm::vec2 position;

    /**
     * @brief 현재 크기.
     */
    glm::vec2 scale = {200.0f, 200.0f};

    /**
     * @brief 현재 색상.
     */
    glm::vec3 color;

    /**
     * @brief 현재 도형의 버텍스 데이터.
     */
    std::array<float, 10> vertices;

    /**
     * @brief 현재 도형의 인덱스 데이터.
     */
    std::vector<unsigned int> indices;

    /**
     * @brief 애니메이션 진행률.
     */
    float progress = 0.0f;
};

class Area final
{
public:
    /**
     * @brief 생성자.
     *
     * @param type_     배치할 도형의 유형.
     * @param position_ 영역의 위치.
     * @param type_     영역의 크기.
     */
    explicit Area(const glm::vec2   position_,
                  const glm::vec2   size_,
                  const Shape::Type type_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Area() noexcept;

    /**
     * @brief 해당 영역에 도형이 배치되었는지 여부를 반환합니다.
     *
     * @return bool 도형 배치 여부.
     */
    [[nodiscard]]
    inline bool HasShape() const noexcept
    {
        return shape != nullptr;
    }

    /**
     * @brief 해당 영역에 배치된 도형을 반환합니다.
     *
     * @return Shape* 도형 포인터.
     */
    [[nodiscard]]
    inline Shape* GetShape() const noexcept
    {
        return shape.get();
    }

    /**
     * @brief 영역을 업데이트합니다.
     *
     * @param deltaTime_ 시간 변화량.
     */
    void Update(const float deltaTime_) const noexcept;

    /**
     * @brief 영역을 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    void Render(const Shader& shader_) const noexcept;
private:
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
     * @brief 영역의 위치.
     */
    glm::vec2 position;

    /**
     * @brief 영역의 크기.
     */
    glm::vec2 size;

    /**
     * @brief 해당 영역에 배치된 도형.
     */
    std::unique_ptr<Shape> shape;
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
                                      const void*   userParam_) noexcept;

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
static constexpr glm::vec3 backgroundColor = {0.1f, 0.1f, 0.1f};

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
 * @brief 월드 내 포함될 수 있는 영역의 최대 갯수.
 */
static constexpr std::size_t MAX_AREA_COUNTS = 4;

/**
 * @brief 월드 내 모든 영역.
 */
static std::array<std::unique_ptr<Area>, MAX_AREA_COUNTS> areas;

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
    shader.Use();

    constexpr float halfWidth  = WINDOW_WIDTH / 2.0f;
    constexpr float halfHeight = WINDOW_HEIGHT / 2.0f;

    areas.at(0) = std::make_unique<Area>(glm::vec2{halfWidth, halfHeight}, glm::vec2{halfWidth, halfHeight}, Shape::Type::Line);
    areas.at(1) = std::make_unique<Area>(glm::vec2{0.0f,      halfHeight}, glm::vec2{halfWidth, halfHeight}, Shape::Type::Triangle);
    areas.at(2) = std::make_unique<Area>(glm::vec2{0.0f,      0.0f},       glm::vec2{halfWidth, halfHeight}, Shape::Type::Rectangle);
    areas.at(3) = std::make_unique<Area>(glm::vec2{halfWidth, 0.0f},       glm::vec2{halfWidth, halfHeight}, Shape::Type::Pentagon);

    glm::mat4 projection = glm::ortho(0.0f,
                                      static_cast<float>(WINDOW_WIDTH),
                                      0.0f,
                                      static_cast<float>(WINDOW_HEIGHT),
                                      -1.0f, 1.0f);

    const GLint projLoc = glGetUniformLocation(shader.GetProgramID(), "u_Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        const float currentTime = static_cast<float>(glfwGetTime());
        static float lastTime = currentTime;
        const float deltaTime = (currentTime - lastTime) * timeScale;
        lastTime = currentTime;

        shader.Use();

        for (const std::unique_ptr<Area>& area : areas)
        {
            area->Update(deltaTime);
            area->Render(shader);
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

Shape::Shape(const glm::vec2&   position_,
             const Shape::Type& type_) noexcept
    : position(position_)
    , type(type_)
    , goal(type_)
    , color(SHAPE_COLORS.at(type_))
    , vertices(SHAPES_VERTICES.at(type_))
    , indices(SHAPE_INDICES.at(type_))
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Shape::~Shape() noexcept
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Shape::Update(const float deltaTime_) noexcept
{
    if (type == goal)
    {
        return;
    }

    const std::array<float, 10>& currentVerticesData = SHAPES_VERTICES.at(type);
    const std::array<float, 10>& goalVertices        = SHAPES_VERTICES.at(goal);
    for (std::size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i] = currentVerticesData[i] * (1.0f - progress) + goalVertices[i] * progress;
    }

    const glm::vec3& goalColor    = SHAPE_COLORS.at(goal);
    const glm::vec3& currentColor = SHAPE_COLORS.at(type);
    color = currentColor * (1.0f - progress) + goalColor * progress;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    if ((progress += deltaTime_ * 2.0f) >= 1.0f)
    {
        type     = goal;
        vertices = SHAPES_VERTICES.at(type);
        color    = SHAPE_COLORS.at(type);
        progress = 0.0f;
    }
}

void Shape::Render(const Shader& shader_) noexcept
{
    glm::mat4 model = {};
    model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
    glUniform3fv(colorLoc, 1, &color[0]);

    glBindVertexArray(vao);

    const Shape::Type& currentDrawType = (type == goal) ? type : goal;
    const GLenum       primitive       = SHAPE_PRIMITIVES.at(currentDrawType);
    glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void Shape::NextTo() noexcept
{
    if (type != goal)
    {
        return;
    }

    goal = static_cast<Shape::Type>((static_cast<unsigned char>(type) + 1) % 4);

    indices = SHAPE_INDICES.at(goal);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

    progress = 0.0f;
}

Area::Area(const glm::vec2   position_,
           const glm::vec2   size_,
           const Shape::Type type_) noexcept
    : position(position_)
    , size(size_)
    , shape(std::make_unique<Shape>(position_ + size_ / 2.0f, type_))
{
    constexpr std::array<glm::vec2, 4> vertices
    {
        glm::vec2{-1.0f,  1.0f},
        glm::vec2{ 1.0f,  1.0f},
        glm::vec2{ 1.0f, -1.0f},
        glm::vec2{-1.0f, -1.0f}
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Area::~Area() noexcept
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Area::Update(const float deltaTime_) const noexcept
{
    if (shape)
    {
        shape->Update(deltaTime_);
    }
}

void Area::Render(const Shader& shader_) const noexcept
{
    if (shape)
    {
        shape->Render(shader_);
    }

    glm::mat4 model = { 0.0f };
    model = glm::translate(glm::mat4(1.0f), glm::vec3(position.x + size.x / 2.0f, position.y + size.y / 2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(size.x / 2.0f, size.y / 2.0f, 1.0f));

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    constexpr glm::vec3 color    = {1.0f, 1.0f, 1.0f};
    const     GLint     colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
    glUniform3fv(colorLoc, 1, &color[0]);

    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glBindVertexArray(0);
}

void OnDebugMessage(GLenum        source_,
                    GLenum        type_,
                    GLuint        id_,
                    GLenum        severity_,
                    GLsizei       length_,
                    const GLchar* message_,
                    const void*   userParam_) noexcept
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
        case GLFW_KEY_L:
        {
            for (const std::unique_ptr<Area>& area : areas)
            {
                if (area->HasShape())
                {
                    Shape* shape = area->GetShape();
                    if (shape->GetType() == Shape::Type::Line)
                    {
                        shape->NextTo();
                    }
                }
            }

            break;
        }
        case GLFW_KEY_T:
        {
            for (const std::unique_ptr<Area>& area : areas)
            {
                if (area->HasShape())
                {
                    Shape* shape = area->GetShape();
                    if (shape->GetType() == Shape::Type::Triangle)
                    {
                        shape->NextTo();
                    }
                }
            }

            break;
        }
        case GLFW_KEY_R:
        {
            for (const std::unique_ptr<Area>& area : areas)
            {
                if (area->HasShape())
                {
                    Shape* shape = area->GetShape();
                    if (shape->GetType() == Shape::Type::Rectangle)
                    {
                        shape->NextTo();
                    }
                }
            }

            break;
        }
        case GLFW_KEY_P:
        {
            for (const std::unique_ptr<Area>& area : areas)
            {
                if (area->HasShape())
                {
                    Shape* shape = area->GetShape();
                    if (shape->GetType() == Shape::Type::Pentagon)
                    {
                        shape->NextTo();
                    }
                }
            }

            break;
        }
        case GLFW_KEY_A:
        {
            for (const std::unique_ptr<Area>& area : areas)
            {
                if (area->HasShape())
                {
                    Shape* shape = area->GetShape();
                    shape->NextTo();
                }
            }

            break;
        }
        case GLFW_KEY_UP:
        {
            if (constexpr float MAX_TIME_SCALE = 30.0f; (timeScale += 1.0f) > MAX_TIME_SCALE)
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
            if (constexpr float MIN_TIME_SCALE = 1.0f; (timeScale -= 1.0f) < MIN_TIME_SCALE)
            {
                timeScale = MIN_TIME_SCALE;
                SPDLOG_WARN("Time scale cannot be less than or equal to zero.");
                break;
            }

            SPDLOG_INFO("Time scale set to {:.1f}.", timeScale);
            break;
        }
        case GLFW_KEY_ENTER:
        {
            static float previousTimeScale = timeScale;

            if (timeScale != 0.0f)
            {
                previousTimeScale = timeScale;
                timeScale = 0.0f;
                SPDLOG_INFO("Animation paused.");
            }
            else
            {
                timeScale = previousTimeScale;
                SPDLOG_INFO("Animation resumed.");
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
