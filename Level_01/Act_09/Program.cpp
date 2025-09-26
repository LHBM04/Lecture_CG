#include <array>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
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
     */
    explicit Shader(const char* const vertexSource_,
                    const char* const fragmentSource_) noexcept
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource_, nullptr);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
 * @class Triangle
 *
 * @brief 삼각형 도형을 정의합니다.
 */
class Triangle final
{
public:
    explicit Triangle(const glm::vec2 position_,
                      const float     size_,
                      const glm::vec3 color_) noexcept
        : vao(0)
        , vbo(0)
        , ebo(0)
        , position(position_)
        , size(size_)
        , color(color_)
    {
        constexpr std::array<GLfloat, 6> vertices_
        {
            0.0f,  0.5f,
           -0.5f, -0.5f,
            0.5f, -0.5f
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

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }

    ~Triangle() noexcept
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    /**
     * @brief 해당 삼각형을 그립니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    inline void Draw(const Shader& shader_) const noexcept
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        model = glm::scale(model, glm::vec3(size, size, 1.0f));

        const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
        glUniform3fv(colorLoc, 1, &color[0]);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

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
    glm::vec2 position;

    /**
     * @brief 해당 도형의 크기.
     */
    float size;

    /**
     * @brief 해당 도형의 색상.
     */
    glm::vec3 color;
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
 * @brief 선으로 그릴지 여부.
 */
static bool shouldDrawWithLines = false;

/**
 * @struct Quadrant
 *
 * @brief 나눌 사분면의 위치, 크기를 정의합니다.
 */
struct Quadrant
{
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
};

/**
 * @brief 사분면들.
 */
static std::array<Quadrant, 4> quadrants
{
    Quadrant{ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f }, // Top-left (1사분면, index 0)
    Quadrant{ 0.0f,                WINDOW_HEIGHT / 2.0f, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f }, // Top-right (2사분면, index 1)
    Quadrant{ 0.0f,                0.0f,                 WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f }, // Bottom-left (3사분면, index 2)
    Quadrant{ WINDOW_WIDTH / 2.0f, 0.0f,                 WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f }  // Bottom-right (4사분면, index 3)
};
/**
 * @brief 각 사분면에 위치하는 삼각형들.
 */
static std::array<std::vector<Triangle>, 4> triangles;

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

    glfwSetKeyCallback(window, OnKeyInteracted);
    glfwSetMouseButtonCallback(window, OnButtonInteracted);
    glfwSetCursorPosCallback(window, OnCursorMoved);

    const std::string vertexShaderFile   = GetFile("Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile   = GetFile("Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    Shader shader(vertexShaderSource, fragmentShaderSource);
    shader.Use();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, shouldDrawWithLines ? GL_LINE : GL_FILL);

        for (const auto& triangle : triangles)
        {
            std::ranges::for_each(triangle, [&shader](const Triangle& tri) {
                tri.Draw(shader);
            });
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
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
            for (auto& triangleList : triangles)
            {
                triangleList.clear();
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

// OnButtonInteracted 함수 수정
void OnButtonInteracted(GLFWwindow* const window_,
                        const int         button_,
                        const int         action_,
                        const int         mods_) noexcept
{
    if (action_ != GLFW_PRESS)
    {
        return;
    }

    // 마우스 좌표를 NDC(-1 to 1)로 변환
    const float ndcX = (cursorPosition.x / WINDOW_WIDTH  * 2.0f) - 1.0f;
    const float ndcY = (cursorPosition.y / WINDOW_HEIGHT * 2.0f) - 1.0f;
    const glm::vec2 position = { ndcX, ndcY };

    // NDC에 적합한 작은 크기 값
    constexpr float     size     = 0.2f;
    constexpr glm::vec3 color    = { 1.0f, 1.0f, 1.0f };

    if (button_ == GLFW_MOUSE_BUTTON_LEFT)
    {
        for (std::size_t index = 0; index < quadrants.size(); ++index)
        {
            if (quadrants.at(index).IsInteract(cursorPosition))
            {
                triangles.at(index).clear();
                triangles.at(index).emplace_back(position, size, color);

                std::cout << std::format("[Info] Triangle created at position ");
                std::cout << std::format("({:.1f}, {:.1f}) in Quadrant {}.\n", cursorPosition.x, cursorPosition.y, index + 1);
                break;
            }
        }
    }
    else if (button_ == GLFW_MOUSE_BUTTON_RIGHT)
    {
        for (std::size_t index = 0; index < quadrants.size(); ++index)
        {
            if (quadrants.at(index).IsInteract(cursorPosition))
            {
                if (triangles.at(index).size() >= 4)
                {
                    std::cout << "[Warning] Cannot create more triangles in this quadrant!\n";
                    break;
                }

                triangles.at(index).emplace_back(position, size, color);

                std::cout << std::format("[Info] Triangle created at position ");
                std::cout << std::format("({:.1f}, {:.1f}) in Quadrant {}.\n", cursorPosition.x, cursorPosition.y, index + 1);
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
    cursorPosition.y = static_cast<float>(WINDOW_HEIGHT - y_);
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