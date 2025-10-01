#include <iostream>
#include <random>
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
            const glm::vec2 min = GetMin();
            const glm::vec2 max = GetMax();

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
 *
 * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
 */
static void Update(const float deltaTime_) noexcept;

/**
 * @brief 게임을 렌더링합니다.
 */
static void Render() noexcept;

/**
 * @brief 게임 종료 여부 검사.
 */
static void CheckComplete() noexcept;

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
static constexpr const char* const WINDOW_TITLE = "Level 01 - Act 07";

/**
 * @brief GL 메이저 버전.
 */
constexpr unsigned char CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief GL 마이너 버전.
 */
constexpr unsigned char CONTEXT_MINOR_VERSION = 5;

/**
 * @brief 커서 위치.
 */
static glm::vec2 cursorPosition = { 0, 0 };

/**
 * @brief 격자 크기.
 */
static constexpr int GRID_SIZE = 10;

/**
 * @brief 화면의 절반(왼쪽 패널)의 너비.
 */
static constexpr int PANEL_PIXEL_WIDTH = WINDOW_WIDTH / 2;

/**
 * @brief 셀 너비.
 */
static constexpr float CELL_WIDTH = static_cast<float>(PANEL_PIXEL_WIDTH) / GRID_SIZE;

/**
 * @brief 셀 높이.
 */
static constexpr float CELL_HEIGHT = static_cast<float>(WINDOW_HEIGHT) / GRID_SIZE;

/**
 * @brief 블럭 개수.
 */
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
        // Update.
        {
            float currentTime = static_cast<float>(glfwGetTime());
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            Update(deltaTime);
        }
        // Render.
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            Render();
        }
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
    if (action_ != GLFW_PRESS)
    {
        return;
    }

    switch (key_)
    {
        case GLFW_KEY_R:
        {
            Initialize();
            break;
        }
        case GLFW_KEY_Q:
        {
            glfwSetWindowShouldClose(window_, true);
            break;
        }
        case GLFW_KEY_SPACE:
        {
            for (Block& userBlock : answer)
            {
                const Block* targetBlock = nullptr;

                for (const auto& q_block : question)
                {
                    if (userBlock.id == q_block.id)
                    {
                        targetBlock = &q_block;
                        break;
                    }
                }

                if (targetBlock)
                {
                    glm::vec2 hintPosition = targetBlock->position;
                    hintPosition.x += PANEL_PIXEL_WIDTH;

                    constexpr float THRESHOLD = 0.5f;
                    if (glm::distance(userBlock.position, hintPosition) >= THRESHOLD)
                    {
                        userBlock.position = hintPosition;

                        CheckComplete();
                        return;
                    }
                }
            }
            break;
        }
    }
}

void OnButtonInteracted(GLFWwindow* const window_,
                        const int         button_,
                        const int         action_,
                        const int         mods_) noexcept
{
    if (isGameOver)
    {
        return;
    }

    static glm::vec2 offset;

    if (button_ == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action_ == GLFW_PRESS)
        {
            for (Block& block : answer)
            {
                if (block.IsInteract(cursorPosition))
                {
                    dragged = &block;
                    break;
                }
            }
        }
        else if (action_ == GLFW_RELEASE)
        {
            if (!dragged)
            {
                return;
            }

            const glm::vec2 currentTopLeft = dragged->GetMin();

            int gridX = static_cast<int>(round(currentTopLeft.x / CELL_WIDTH));
            int gridY = static_cast<int>(round(currentTopLeft.y / CELL_HEIGHT));

            const int blockGridWidth  = static_cast<int>(dragged->size.x / CELL_WIDTH);
            const int blockGridHeight = static_cast<int>(dragged->size.y / CELL_HEIGHT);

            gridX = std::max(GRID_SIZE, gridX);
            gridX = std::min(gridX, 2 * GRID_SIZE - blockGridWidth);
            gridY = std::max(0, std::min(gridY, GRID_SIZE - blockGridHeight));

            dragged->position.x = (gridX * CELL_WIDTH) + dragged->size.x * 0.5f;
            dragged->position.y = (gridY * CELL_HEIGHT) + dragged->size.y * 0.5f;

            dragged = nullptr;

            CheckComplete();
        }
    }
}

void OnCursorMoved(GLFWwindow* const window_,
                   const double      x_,
                   const double      y_) noexcept
{
    if (isGameOver)
    {
        return;
    }

    const glm::vec2 lastCursorPosition = cursorPosition;
    cursorPosition = { static_cast<float>(x_), static_cast<float>(WINDOW_HEIGHT - y_) };

    if (dragged != nullptr)
    {
        // int gridX = static_cast<int>(cursorPosition.x / CELL_WIDTH);
        // int gridY = static_cast<int>(cursorPosition.y / CELL_HEIGHT);

        // float targetX = (gridX * CELL_WIDTH) + (CELL_WIDTH * 0.5f);
        // float targetY = (gridY * CELL_HEIGHT) + (CELL_HEIGHT * 0.5f);

        // dragged->position = { targetX, targetY };
        glm::vec2 delta = cursorPosition - lastCursorPosition;
        dragged->position += delta;
    }
}

void Initialize() noexcept
{
    isGameOver = false;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> colorDist(0.3f, 1.0f);
    std::uniform_int_distribution<int> sizeDist(1, 3);
    std::uniform_int_distribution<int> posDist(0, GRID_SIZE - 3);

    question.clear();
    for (unsigned int count = 0; count < BLOCK_COUNTS; ++count)
    {
        Block newBlock{};
        newBlock.id     = count;
        newBlock.size= { sizeDist(gen) * CELL_WIDTH, sizeDist(gen) * CELL_WIDTH };

        bool positionFound = false;

        while(!positionFound)
        {
            const glm::vec2 gridPos = { posDist(gen), posDist(gen) };

            newBlock.position = { (gridPos.x * CELL_WIDTH) + newBlock.size.x * 0.5f,
                                     (gridPos.y * CELL_HEIGHT) + newBlock.size.y * 0.5f };

            positionFound = true;

            if (newBlock.GetMax().x > PANEL_PIXEL_WIDTH ||
                newBlock.GetMax().y > WINDOW_HEIGHT)
            {
                positionFound = false;
                continue;
            }

            for(const Block& other : question)
            {
                if ((newBlock.GetMin().x < other.GetMax().x) &&
                    (newBlock.GetMax().x > other.GetMin().x) &&
                    (newBlock.GetMin().y < other.GetMax().y) &&
                    (newBlock.GetMax().y > other.GetMin().y))
                {
                    positionFound = false;
                    break;
                }
            }
        }

        newBlock.color = { colorDist(gen), colorDist(gen), colorDist(gen) };
        question.push_back(newBlock);
    }

    answer.assign(question.begin(), question.end());

    std::uniform_int_distribution<int> userPosDistX(0, GRID_SIZE - 3);
    std::uniform_int_distribution<int> userPosDistY(0, GRID_SIZE - 3);

    for (Block& block : answer)
    {
        bool isFound = false;

        while (!isFound)
        {
            const glm::vec2 gridPos = { userPosDistX(gen), userPosDistY(gen) };

            block.position = { PANEL_PIXEL_WIDTH + (gridPos.x * CELL_WIDTH) + block.size.x * 0.5f,
                                  (gridPos.y * CELL_HEIGHT) + block.size.y * 0.5f };

            isFound = true;

            for (const Block& other : answer)
            {
                if (block.id == other.id)
                {
                    continue;
                }

                if ((block.GetMin().x < other.GetMax().x) &&
                    (block.GetMax().x > other.GetMin().x) &&
                    (block.GetMin().y < other.GetMax().y) &&
                    (block.GetMax().y > other.GetMin().y))
                {
                    isFound = false;
                    break;
                }
            }
        }
    }

    dragged = nullptr;
}

void Update(const float deltaTime_) noexcept
{

}

void Render() noexcept
{
    // 격자.
    {
        glColor3f(0.2, 0.2, 0.2);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        {
            for (unsigned int index = 0; index <= GRID_SIZE * 2; ++index)
            {
                glVertex2f(index * CELL_WIDTH, 0);
                glVertex2f(index * CELL_WIDTH, WINDOW_HEIGHT);
            }

            for (unsigned int index = 0; index <= GRID_SIZE; ++index)
            {
                glVertex2f(0, index * CELL_HEIGHT);
                glVertex2f(WINDOW_WIDTH, index * CELL_HEIGHT);
            }
        }
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        {
            glVertex2f(WINDOW_WIDTH / 2.0f, 0);
            glVertex2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT);
        }
        glEnd();

    }
    // 블록.
    {
        for (const Block& block : question)
        {
            block.Render();
        }

        for (const Block& block : answer)
        {
            block.Render();
        }
    }
    {
        if (isGameOver)
        {
            glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
            glRectf(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
    }
}

void CheckComplete() noexcept
{
    for (const auto& ans_block : answer)
    {
        bool foundMatch = false;

        for (const auto& q_block : question)
        {
            if (ans_block.id == q_block.id)
            {
                glm::vec2 converted_pos = ans_block.position;
                converted_pos.x -= PANEL_PIXEL_WIDTH;

                constexpr float THRESHOLD = 1.0f;

                if (glm::distance(converted_pos, q_block.position) < THRESHOLD)
                {
                    foundMatch = true;
                }
                break;
            }
        }
        if (!foundMatch)
        {
            isGameOver = false;
            return;
        }
    }

    std::cout << "All blocks are in place! Game Over." << std::endl;

    isGameOver = true;
}