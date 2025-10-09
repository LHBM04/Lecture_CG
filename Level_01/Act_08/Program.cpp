#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
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
     */
    explicit Shader(const char* const vertexSource_,
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

    /**
     * @brief 소멸자.
     */
    ~Shader() noexcept
    {
        if (isCompiled)
        {
            glDeleteProgram(programID);
        }
    }

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
 * @brief 도형의 타입을 정의합니다.
 */
enum class DrawMode
{
    /**
     * @brief 도형이 없음.
     */
    None,

    /**
     * @brief 점.
     */
    Dot,

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
};

/**
 * @class Shape
 *
 * @brief 도형을 정의합니다.
 */
class Shape
{
public:
    /**
     * @brief 생성자.
     */
    constexpr explicit Shape(const std::vector<float>&        vertices_,
                             const std::vector<unsigned int>& indices_) noexcept
        : vao(0)
        , vbo(0)
        , ebo(0)
        , currentPosition(0.0f)
        , size(10.0f)
        , color(1.0f)
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);

        if (!indices_.empty())
        {
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);
            drawCount = indices_.size();
        }
        else
        {
            drawCount = vertices_.size() / 2;
        }

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    /**
     * @brief 소멸자.
     */
    virtual ~Shape() noexcept
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

    /**
     * @brief 해당 도형의 위치를 반환합니다.
     */
    [[nodiscard]]
    constexpr const glm::vec2& GetPosition() const noexcept
    {
        return currentPosition;
    }

    /**
     * @brief 해당 도형의 위치를 지정한 위치로 설정합니다.
     *
     * @param position_ 지정할 위치.
     */
    inline void SetPosition(const glm::vec2& position_) noexcept
    {
        currentPosition = position_;
    }

    /**
     * @brief 해당 도형의 크기를 반환합니다.
     *
     * @return float 도형의 크기.
     */
    [[nodiscard]]
    constexpr float GetSize() const noexcept
    {
        return size;
    }

    /**
     * @brief 해당 도형의 크기를 지정한 크기로 설정합니다.
     *
     * @param size_ 지정할 크기.
     */
    inline void SetSize(const float size_) noexcept
    {
        size = size_;
    }

    /**
     * @brief 해당 도형의 색상을 반환합니다.
     *
     * @return glm::vec3 도형의 색상.
     */
    [[nodiscard]]
    constexpr const glm::vec3& GetColor() const noexcept
    {
        return color;
    }

    /**
     * @brief 해당 도형의 색상을 지정한 색상으로 설정합니다.
     *
     * @param color_ 지정할 색상.
     */
    inline void SetColor(const glm::vec3& color_) noexcept
    {
        color = color_;
    }

    /**
     * @brief 해당 도형과 지정한 점이 접촉하는지 여부를 반환합니다.
     *
     * @param position_ 지정할 점.
     *
     * @return bool 접촉 여부.
     */
    [[nodiscard]]
    virtual constexpr bool IsInteract(const glm::vec2& position_) const noexcept = 0;

    /**
     * @brief 해당 도형을 그립니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    virtual inline void Draw(const Shader& shader_) const noexcept = 0;
protected:
    /**
     * @brief 해당 도형의 정점 배열 객체.
     */
    unsigned int vao;

    /**
     * @brief 해당 도형의 정점 버퍼 객체.
     */
    unsigned int vbo;

    /**
     * @brief 해당 도형의 요소 배열 객체.
     */
    unsigned int ebo;

    /**
     * @brief 해당 도형의 타입.
     */
    glm::vec2 currentPosition{};

    /**
     * @brief 해당 도형의 크기.
     */
    float size{};

    /**
     * @brief 해당 도형의 색상.
     */
    glm::vec3 color{};

    /**
     * @brief 그릴 정점의 개수.
     */
    std::size_t drawCount{};
};

/**
 * @class 점을 정의합니다.
 *
 * @brief 점 도형을 정의합니다.
 */
class Dot final : public Shape
{
public:
    explicit Dot() noexcept
        : Shape({ 0.0f, 0.0f },
                { })
    {
    }

    /**
     * @brief 해당 점과 지정한 점이 접촉하는지 여부를 반환합니다.
     *
     * @param position_ 지정할 점.
     *
     * @return bool 접촉 여부.
     */
    [[nodiscard]]
    virtual constexpr bool IsInteract(const glm::vec2& position_) const noexcept override
    {
        const float halfSize = size * 0.5f;

        return position_.x >= currentPosition.x - halfSize &&
               position_.x <= currentPosition.x + halfSize &&
               position_.y >= currentPosition.y - halfSize &&
               position_.y <= currentPosition.y + halfSize;
    }

    /**
     * @brief 해당 점을 그립니다.
     */
    virtual inline void Draw(const Shader& shader_) const noexcept override
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(currentPosition, 0.0f));
        model = glm::scale(model, glm::vec3(size, size, 1.0f));

        const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
        glUniform3fv(colorLoc, 1, &color[0]);

        glUniform3fv(colorLoc, 1, &color[0]);
        glBindVertexArray(vao);
        glPointSize(size);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
    };
};

/**
 * @class Line
 *
 * @brief 선 도형을 정의합니다.
 */
class Line final : public Shape
{
public:
    explicit Line() noexcept
    : Shape({ -0.5f, 0.0f, 0.5f, 0.0f },
            { 0, 1 })
    {
    }

    /**
     * @brief 해당 점과 지정한 점이 접촉하는지 여부를 반환합니다.
     *
     * @param position_ 지정할 점.
     *
     * @return bool 접촉 여부.
     */
    [[nodiscard]]
    virtual constexpr bool IsInteract(const glm::vec2& position_) const noexcept override
    {
        const float halfSize = size * 0.5f;

        return position_.x >= currentPosition.x - halfSize &&
               position_.x <= currentPosition.x + halfSize &&
               position_.y >= currentPosition.y - halfSize &&
               position_.y <= currentPosition.y + halfSize;
    }

    /**
     * @brief 해당 점을 그립니다.
     */
    virtual inline void Draw(const Shader& shader_) const noexcept override
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(currentPosition, 0.0f));
        model = glm::scale(model, glm::vec3(size, size, 1.0f));

        const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
        glUniform3fv(colorLoc, 1, &color[0]);

        glBindVertexArray(vao);
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
};

/**
 * @class Triangle
 *
 * @brief 삼각형 도형을 정의합니다.
 */
class Triangle final : public Shape
{
public:
    explicit Triangle() noexcept
        : Shape({ 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f },
                { 0, 1, 2 })
    {
    }

    /**
     * @brief 해당 점과 지정한 점이 접촉하는지 여부를 반환합니다.
     *
     * @param position_ 지정할 점.
     *
     * @return bool 접촉 여부.
     */
    [[nodiscard]]
    virtual constexpr bool IsInteract(const glm::vec2& position_) const noexcept override
    {
        const float halfSize = size * 0.5f;

        return (position_.x >= currentPosition.x - halfSize) &&
               (position_.x <= currentPosition.x + halfSize) &&
               (position_.y >= currentPosition.y - halfSize) &&
               (position_.y <= currentPosition.y + halfSize);
    }

    /**
     * @brief 해당 점을 그립니다.
     */
    virtual inline void Draw(const Shader& shader_) const noexcept override
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
};

class Rectangle final : public Shape
{
public:
    explicit Rectangle() noexcept
        : Shape({ 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f },
                { 0, 1, 2, 2, 3, 0 })
    {
    }

    /**
     * @brief 해당 점과 지정한 점이 접촉하는지 여부를 반환합니다.
     *
     * @param position_ 지정할 점.
     *
     * @return bool 접촉 여부.
     */
    [[nodiscard]]
    virtual constexpr bool IsInteract(const glm::vec2& position_) const noexcept override
    {
        const float halfSize = size * 0.5f;

        return position_.x >= currentPosition.x - halfSize &&
               position_.x <= currentPosition.x + halfSize &&
               position_.y >= currentPosition.y - halfSize &&
               position_.y <= currentPosition.y + halfSize;
    }

    /**
     * @brief 해당 점을 그립니다.
     */
    virtual inline void Draw(const Shader& shader_) const noexcept override
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(currentPosition, 0.0f));
        model = glm::scale(model, glm::vec3(size, size, 1.0f));

        const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
        glUniform3fv(colorLoc, 1, &color[0]);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
};

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
static void OnButtonInteracted(GLFWwindow* window_,
                               int         button_,
                               int         action_,
                               int         mods_) noexcept;

/**
 * @brief 커서가 움직일 때 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param x_      마우스 X 좌표.
 * @param y_      마우스 Y 좌표.
 */
static void OnCursorMoved(GLFWwindow* window_,
                          double      x_,
                          double      y_) noexcept;

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
static constexpr const char* const WINDOW_TITLE = "Level 01 - Act 08";

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
 * @brief 현재 그릴 수 있는 도형의 타입.
 */
static DrawMode currentMode = DrawMode::None;

/**
 * @brief 만들 수 있는 도형의 최대 개수.
 */
static constexpr std::size_t MAX_SHAPES = 10;

/**
 * @brief 현재 월드 내 도형들.
 */
static std::vector<std::unique_ptr<Shape>> shapes;

/**
 * @brief 현재 선택된 도형.
 */
static Shape* selectedShape = nullptr;

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          WINDOW_TITLE,
                                          nullptr,
                                          nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        ::glfwTerminate();
        return -1;
    }

    glfwSetWindowPos(window, 100, 100);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetKeyCallback(window, OnKeyInteracted);
    glfwSetMouseButtonCallback(window, OnButtonInteracted);
    glfwSetCursorPosCallback(window, OnCursorMoved);

    const std::string vertexShaderFile   = GetFile("Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile   = GetFile("Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    Shader globalShader(vertexShaderSource,
                        fragmentShaderSource);

    glm::mat4 projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 1.0f);

    // 셰이더를 한 번만 사용하고 프로젝션 행렬을 설정합니다.
    globalShader.Use();
    GLint projLoc = glGetUniformLocation(globalShader.GetProgramID(), "u_Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        for (const std::unique_ptr<Shape>& shape : shapes)
        {
            shape->Draw(globalShader);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    shapes.clear();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

std::string GetFile(const std::filesystem::path& path_)
{
    std::ifstream file(path_, std::ios::in | std::ios::binary);

    if (!file)
    {
        throw std::runtime_error("Could not open file");
    }

    return { std::istreambuf_iterator<char>(file),
             std::istreambuf_iterator<char>() };
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
            if (currentMode != DrawMode::Dot)
            {
                currentMode = DrawMode::Dot;
                std::cout << "[Notice] Point mode activated.\n";
            }
            else
            {
                currentMode = DrawMode::None;
                std::cout << "[Notice] Point mode deactivated.\n";
            }

            break;
        }
        case GLFW_KEY_L:
        {
            if (currentMode != DrawMode::Line)
            {
                currentMode = DrawMode::Line;
                std::cout << "[Notice] Line mode activated.\n";
            }
            else
            {
                currentMode = DrawMode::None;
                std::cout << "[Notice] Line mode deactivated.\n";
            }

            break;
        }
        case GLFW_KEY_T:
        {
            if (currentMode != DrawMode::Triangle)
            {
                currentMode = DrawMode::Triangle;
                std::cout << "[Notice] Triangle mode activated.\n";
            }
            else
            {
                currentMode = DrawMode::None;
                std::cout << "[Notice] Triangle mode deactivated.\n";
            }

            break;
        }
        case GLFW_KEY_R:
        {
            if (currentMode != DrawMode::Rectangle)
            {
                currentMode = DrawMode::Rectangle;
                std::cout << "[Notice] Rectangle mode activated.\n";
            }
            else
            {
                currentMode = DrawMode::None;
                std::cout << "[Notice] Rectangle mode deactivated.\n";
            }

            break;
        }
        case GLFW_KEY_W:
        {
            if (selectedShape)
            {
                selectedShape->SetPosition(selectedShape->GetPosition() + glm::vec2(0.0f, 10.0f));
                std::cout << "[Info] Moved selected shape up.\n";
            }
            else
            {
                std::cout << "[Warning] No shape selected to move.\n";
            }

            break;
        }
        case GLFW_KEY_S:
        {
            if (selectedShape)
            {
                selectedShape->SetPosition(selectedShape->GetPosition() + glm::vec2(0.0f, -10.0f));
                std::cout << "[Info] Moved selected shape up.\n";
            }
            else
            {
                std::cout << "[Warning] No shape selected to move.\n";
            }

            break;
        }
        case GLFW_KEY_A:
        {
            if (selectedShape)
            {
                selectedShape->SetPosition(selectedShape->GetPosition() + glm::vec2(-10.0f, 0.0f));
                std::cout << "[Info] Moved selected shape up.\n";
            }
            else
            {
                std::cout << "[Warning] No shape selected to move.\n";
            }

            break;
        }
        case GLFW_KEY_D:
        {
            if (selectedShape)
            {
                selectedShape->SetPosition(selectedShape->GetPosition() + glm::vec2(10.0f, 0.0f));
                std::cout << "[Info] Moved selected shape up.\n";
            }
            else
            {
                std::cout << "[Warning] No shape selected to move.\n";
            }

            break;
        }
        case GLFW_KEY_U:
        {
            if (selectedShape)
            {
                selectedShape->SetPosition(selectedShape->GetPosition() + glm::vec2(-10.0f, 10.0f));
                std::cout << "[Info] Moved selected shape up.\n";
            }
            else
            {
                std::cout << "[Warning] No shape selected to move.\n";
            }

            break;
        }
        case GLFW_KEY_J:
        {
            if (selectedShape)
            {
                selectedShape->SetPosition(selectedShape->GetPosition() + glm::vec2(10.0f, 10.0f));
                std::cout << "[Info] Moved selected shape up.\n";
            }
            else
            {
                std::cout << "[Warning] No shape selected to move.\n";
            }

            break;
        }
        case GLFW_KEY_H:
        {
            if (selectedShape)
            {
                selectedShape->SetPosition(selectedShape->GetPosition() + glm::vec2(-10.0f, -10.0f));
                std::cout << "[Info] Moved selected shape up.\n";
            }
            else
            {
                std::cout << "[Warning] No shape selected to move.\n";
            }

            break;
        }
        case GLFW_KEY_K:
        {
            if (selectedShape)
            {
                selectedShape->SetPosition(selectedShape->GetPosition() + glm::vec2(10.0f, -10.0f));
                std::cout << "[Info] Moved selected shape up.\n";
            }
            else
            {
                std::cout << "[Warning] No shape selected to move.\n";
            }

            break;
        }
        case GLFW_KEY_C:
        {
            if (!shapes.empty())
            {
                shapes.clear();
                selectedShape = nullptr;
                std::cout << "[Notice] All shapes cleared.\n";
            }
            else
            {
                std::cout << "[Warning] No shapes to clear.\n";
            }

            break;
        }
        case GLFW_KEY_Q:
        {
            glfwSetWindowShouldClose(window_, true);
            std::cout << "[Notice] Program has closed.\n";
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
        switch (currentMode)
        {
            case DrawMode::None:
            {
                for (const std::unique_ptr<Shape>& shape : shapes)
                {
                    if (shape->IsInteract(cursorPosition))
                    {
                        selectedShape = shape.get();

                        std::cout << std::format("[Info] Shape selected at position ");
                        std::cout << std::format("({:.1f}, {:.1f}).\n", selectedShape->GetPosition().x, selectedShape->GetPosition().y);

                        break;
                    }
                }

                break;
            }
            case DrawMode::Dot:
            {
                if (shapes.size() >= MAX_SHAPES)
                {
                    std::cout << "[Warning] Cannot create more shapes!\n";
                    break;
                }

                std::unique_ptr<Dot> newDot = std::make_unique<Dot>();
                newDot->SetPosition(cursorPosition);
                newDot->SetSize(10.0f);
                newDot->SetColor({ 1.0f, 0.0f, 0.0f });

                shapes.push_back(std::move(newDot));

                std::cout << std::format("[Info] Point created at ({:.1f}, {:.1f}).\n", cursorPosition.x, cursorPosition.y);

                break;
            }
            case DrawMode::Line:
            {
                if (shapes.size() >= MAX_SHAPES)
                {
                    std::cout << "[Warning] Cannot create more shapes!\n";
                    break;
                }

                std::unique_ptr<Line> newLine = std::make_unique<Line>();
                newLine->SetPosition(cursorPosition);
                newLine->SetSize(75.0f);
                newLine->SetColor({ 0.0f, 1.0f, 0.0f });

                shapes.push_back(std::move(newLine));

                std::cout << std::format("[Info] Line created at ({:.1f}, {:.1f}).\n", cursorPosition.x, cursorPosition.y);

                break;
            }
            case DrawMode::Triangle:
            {
                if (shapes.size() >= MAX_SHAPES)
                {
                    std::cout << "[Warning] Cannot create more shapes!\n";
                    break;
                }

                std::unique_ptr<Triangle> newTriangle = std::make_unique<Triangle>();
                newTriangle->SetPosition(cursorPosition);
                newTriangle->SetSize(50.0f);
                newTriangle->SetColor({ 0.0f, 0.0f, 1.0f });

                shapes.push_back(std::move(newTriangle));

                std::cout << std::format("[Info] Triangle created at ({:.1f}, {:.1f}).\n", cursorPosition.x, cursorPosition.y);

                break;
            }
            case DrawMode::Rectangle:
            {
                if (shapes.size() >= MAX_SHAPES)
                {
                    std::cout << "[Warning] Cannot create more shapes!\n";
                    break;
                }

                std::unique_ptr<Rectangle> newRectangle = std::make_unique<Rectangle>();
                newRectangle->SetPosition(cursorPosition);
                newRectangle->SetSize(50.0f);
                newRectangle->SetColor({ 1.0f, 1.0f, 1.0f });

                shapes.push_back(std::move(newRectangle));

                std::cout << std::format("[Info] Rectangle created at ({:.1f}, {:.1f}).\n", cursorPosition.x, cursorPosition.y);

                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void OnCursorMoved(GLFWwindow* const window_,
                   const double      x_,
                   const double      y_) noexcept
{
    glm::ivec2 windowSize;
    glfwGetWindowSize(window_, &windowSize.x, &windowSize.y);

    cursorPosition.x = static_cast<float>(x_);
    cursorPosition.y = static_cast<float>(windowSize.y - y_);
}