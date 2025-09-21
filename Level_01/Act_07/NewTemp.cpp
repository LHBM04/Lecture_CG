#include <algorithm>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

struct Block
{
    /**
     * @brief 해당 사각형의 최소 좌표를 반환합니다.
     *
     * @return glm::vec2 최소 좌표.
     */
    [[nodiscard]]
    constexpr inline glm::vec2 GetMin() const noexcept
    {
        return { position.x - size.x * 0.5f, position.y - size.y * 0.5f };
    }

    /**
     * @brief 해당 사각형의 최대 좌표를 반환합니다.
     *
     * @return glm::vec2 최대 좌표.
     */
    [[nodiscard]]
    constexpr inline glm::vec2 GetMax() const noexcept
    {
        return { position.x + size.x * 0.5f, position.y + size.y * 0.5f };
    }

    /**
     * @brief 해당 사각형과 지정한 점이 접촉하는지 여부를 반환합니다.
     *
     * @param point_ 지정할 점.
     *
     * @return bool 접촉 여부.
     */
    [[nodiscard]]
    constexpr inline bool IsInteract(const glm::vec2& point_) const noexcept
    {
        const glm::vec2 min = GetMin();
        const glm::vec2 max = GetMax();

        return (point_.x >= min.x && point_.x <= max.x) &&
               (point_.y >= min.y && point_.y <= max.y);
    }

    inline void Render() const noexcept
    {
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS);
        {
            const glm::vec2 min = Rect::GetMin();
            const glm::vec2 max = Rect::GetMax();

            glVertex2f(min.x, min.y);
            glVertex2f(max.x, min.y);
            glVertex2f(max.x, max.y);
            glVertex2f(min.x, max.y);
        }
        glEnd();
    }

    /**
     * @brief 블럭 식별을 위한 변수.
     */
    unsigned char id;

    /**
     * @brief 위치.
     */
    glm::vec2 position;

    /**
     * @brief 크기.
     */
    glm::vec2 size;

    /**
     * @brief 색상.
     */
    glm::vec3 color;
};

/**
* @brief 키와 상호작용할 때 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param key_ 눌린 키.
 * @param scancode_ 스캔 코드.
 * @param action_ 키 액션.
 * @param mods_ 수정자 키 상태.
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
 * @param mods_ 버튼 상태.
 */
static void OnButtonInteracted(GLFWwindow* const window_,
                               const int         button_,
                               const int         action_,
                               const int         mods_) noexcept;

/**
 * @brief 커서의 위치가 변경되었을 때 호출됩니다.
 *
 * @param window_ 윈도우.
 * @param x_ 마우스 X 좌표.
 * @param y_ 마우스 Y 좌표.
 */
static void OnCursorMoved(GLFWwindow* const window_,
                          const double      x_,
                          const double      y_) noexcept;

/**
 * @brief 게임을 초기화합니다.
 */
static void Initialize() noexcept;

/**
 * @brief 매 프레임마다 호출됩니다.
 */
static void Update() noexcept;

/**
 * @brief 게임을 렌더링합니다.
 */
static void Render() noexcept;

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
static constexpr const char* const WINDOW_TITLE = "Eraser Simulator";

/**
 * @brief GL 메이저 버전.
 */
constexpr unsigned char CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief GL 마이너 버전.
 */
constexpr unsigned char CONTEXT_MINOR_VERSION = 5;

static constexpr unsigned int GRID_COUNTS = 10;
static constexpr unsigned int GRID_WIDTH_SIZE = 10;
static constexpr unsigned int GRID_HEIGHT_SIZE = 10;
static constexpr unsigned int BLOCK_COUNTS = 10;

/**
 * @brief 문제.
 */
static std::vector<Block> question;

/**
 * @brief 답안.
 */
static std::vector<Block> answer;

/**
 * @brief 드래그 중인 도형.
 */
static Block* dragged = nullptr;

/**
 * @brief 게임 종료 여부.
 */
static bool isGameOver = false;

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

    Initialize();

    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void OnKeyInteracted(GLFWwindow* const window_,
                     const int         key_,
                     const int         scancode_,
                     const int         action_,
                     const int         mods_) noexcept
{

}

void OnButtonInteracted(GLFWwindow* const window_,
                        const int         button_,
                        const int         action_,
                        const int         mods_) noexcept
{

}

void OnCursorMoved(GLFWwindow* const window_,
                   const double      x_,
                   const double      y_) noexcept
{
    if (isGameOver)
    {
        return;
    }

    if (dragged != nullptr)
    {
        dragged->position = { x_ };
    }
}

void Initialize() noexcept
{
    for (int count = 0; count < 10; ++count)
    {
        Block newShape;
        newShape.size     = { 50.0f, 50.0f };
        newShape.position = { static_cast<float>(100 + count * 60), static_cast<float>(WINDOW_HEIGHT / 2) };

        bool positionFound = false;

        while(!positionFound)
        {
            newShape.position.y = static_cast<float>(std::rand() % (WINDOW_HEIGHT - static_cast<int>(newShape.size.y)) + newShape.size.y * 0.5f);
            positionFound = true;

            if (newShape.gridPos.x + newShape.size.x > GRID_SIZE ||
                newShape.gridPos.y + newShape.size.y > GRID_SIZE)
            {
                positionFound = false;
                continue;
            }

            for(const Block& other : question)
            {
                if (newShape.position.x < other.position.x + other.size.x * 0.5f &&
                    newShape.position.x + newShape.size.x * 0.5f > other.position.x &&
                    newShape.position.y < other.position.y + other.size.y * 0.5f &&
                    newShape.position.y + newShape.size.y * 0.5f > other.position.y)
                {
                    positionFound = false;
                    break;
                }
            }
        }

        question.push_back(newShape);
    }
}

void Update() noexcept
{

}

void Render() noexcept
{
    glColor3f(0.2, 0.2, 0.2);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    {
        for (unsigned int i = 0; i <= 20; ++i)
        {
            glVertex2f(i * (WINDOW_WIDTH / 20.0f), 0);
            glVertex2f(i * (WINDOW_WIDTH / 20.0f), WINDOW_HEIGHT);

            glVertex2f(0, i * (WINDOW_HEIGHT / 20.0f));
            glVertex2f(WINDOW_WIDTH, i * (WINDOW_HEIGHT / 20.0f));
        }
    }
    glEnd();

    std::ranges::for_each(question, [](const Block& block) { block.Render(); });
    std::ranges::for_each(answer, [](const Block& block) { block.Render(); });
}