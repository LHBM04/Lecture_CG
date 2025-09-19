#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class Block
{
public:
};

class Field
{
public:

};

class Game
{
public:
    static constexpr int WINDOW_WIDTH  = 1200;
    static constexpr int WINDOW_HEIGHT = 600;
private:

};

static constexpr int WINDOW_WIDTH      = 1200;
static constexpr int WINDOW_HEIGHT     = 600;
static constexpr int GRID_SIZE         = 10; // 10x10 격자
static constexpr int PANEL_GRID_WIDTH  = GRID_SIZE;
static constexpr int PANEL_GRID_HEIGHT = GRID_SIZE;
static constexpr int NUM_SHAPES        = 10; // 생성할 도형의 개수

// 화면의 절반(왼쪽 패널)의 너비
static constexpr int PANEL_PIXEL_WIDTH = WINDOW_WIDTH / 2;

// 각 격자 셀의 픽셀 크기
static constexpr float CELL_WIDTH = static_cast<float>(PANEL_PIXEL_WIDTH) / PANEL_GRID_WIDTH;
static constexpr float CELL_HEIGHT = static_cast<float>(WINDOW_HEIGHT) / PANEL_GRID_HEIGHT;

// --- 구조체 정의 ---

// 도형의 속성을 담는 구조체
struct Block
{
    int id{};
    glm::vec2 gridPos{}; // 격자 좌표 (x, y)
    glm::vec2 size{};    // 격자 단위 크기 (너비, 높이)
    glm::vec3 color{};
    bool isDragging = false;
};

// --- 전역 변수 ---
std::vector<Block> question;   // 왼쪽 문제 패널의 도형들
std::vector<Block> answer;     // 오른쪽 사용자 패널의 도형들
Block* draggedShape = nullptr; // 현재 드래그 중인 도형의 포인터
glm::vec2 dragOffset;          // 드래그 시작 시 마우스와 도형 중심 간의 오프셋
bool isGameComplete = false;   // 게임 완료 여부

// --- 함수 프로토타입 ---
void init();
void display();
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void drawGrid(float startX);
void drawShapes(const std::vector<Block>& shapes, float startX);
void drawText(float x, float y, const std::string& text, void* font);
void checkCompletion();
glm::vec2 screenToGrid(int sx, int sy, float panelStartX);

int main(int    argc_,
         char** argv_)
{
    glutInit(&argc_, argv_);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Shape Matching Game");

    glewInit();
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutMainLoop();

    return 0;
}

// --- 함수 구현 ---

// 초기화 함수
void init() {
    // 기존 데이터 초기화
    question.clear();
    answer.clear();
    isGameComplete = false;
    draggedShape = nullptr;

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // 어두운 배경색

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> colorDist(0.3f, 1.0f);
    std::uniform_int_distribution<int> sizeDist(1, 3);
    std::uniform_int_distribution<int> posDist(0, GRID_SIZE - 3); // 도형이 그리드 밖으로 나가지 않도록

    // 문제 도형 생성
    for (int i = 0; i < NUM_SHAPES; ++i)
    {
        Block newShape;
        newShape.id = i;
        newShape.size = { sizeDist(gen), sizeDist(gen) };

        // 다른 도형과 겹치지 않는 위치 찾기
        bool positionFound = false;

        while (!positionFound)
        {
            newShape.gridPos = { posDist(gen), posDist(gen) };
            positionFound = true;

            // 경계 검사
            if (newShape.gridPos.x + newShape.size.x > GRID_SIZE ||
                newShape.gridPos.y + newShape.size.y > GRID_SIZE) {
                positionFound = false;
                continue;
            }

            // 다른 도형과 겹치는지 검사
            for (const auto& shape : question)
            {
                if (newShape.gridPos.x < shape.gridPos.x + shape.size.x &&
                    newShape.gridPos.x + newShape.size.x > shape.gridPos.x &&
                    newShape.gridPos.y < shape.gridPos.y + shape.size.y &&
                    newShape.gridPos.y + newShape.size.y > shape.gridPos.y)
                {
                    positionFound = false;
                    break;
                }
            }
        }

        newShape.color = { colorDist(gen), colorDist(gen), colorDist(gen) };
        question.push_back(newShape);
    }

    // 사용자 도형 생성 (문제 도형 복사 후 위치 무작위화)
    answer = question;
    std::uniform_int_distribution<int> userPosDistX(GRID_SIZE, 2 * GRID_SIZE - 3);
    std::uniform_int_distribution<int> userPosDistY(0, GRID_SIZE - 3);

    for (auto& shape : answer) {
         bool positionFound = false;

        while(!positionFound)
         {
            shape.gridPos = { userPosDistX(gen), userPosDistY(gen) };
            positionFound = true;

            // 다른 도형과 겹치는지 검사 (사용자 패널 내에서)
            for(const Block& other : answer)
            {
                if(shape.id == other.id)
                {
                    continue;
                }

                if (shape.gridPos.x < other.gridPos.x + other.size.x &&
                    shape.gridPos.x + shape.size.x > other.gridPos.x &&
                    shape.gridPos.y < other.gridPos.y + other.size.y &&
                    shape.gridPos.y + shape.size.y > other.gridPos.y)
                {
                    positionFound = false;
                    break;
                }
            }
         }
    }
}

// 화면 그리기 콜백 함수
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0); // Y축을 아래로 향하게 설정
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 그리드 그리기
    drawGrid(0); // 왼쪽 패널
    drawGrid(PANEL_PIXEL_WIDTH); // 오른쪽 패널

    // 중앙 구분선 그리기
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(WINDOW_WIDTH / 2.0f, 0);
    glVertex2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT);
    glEnd();

    // 도형 그리기
    drawShapes(question, 0);
    drawShapes(answer, 0); // userShapes는 전체 좌표계 기준으로 그려짐

    // 게임 완료 메시지
    if (isGameComplete)
    {
        glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
        glRectf(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        constexpr float boxW = 350;
        constexpr float boxH = 150;
        constexpr float boxX = (WINDOW_WIDTH - boxW) / 2.0f;
        constexpr float boxY = (WINDOW_HEIGHT - boxH) / 2.0f;

        constexpr const char* const clearMessageHeader = "Mission Complete!";
        constexpr const char* const clearMessageBody   = "Press 'r' to restart or 'q' to quit.";

        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(boxX + 105, boxY + 80, clearMessageHeader, GLUT_BITMAP_HELVETICA_18);
        drawText(boxX + 50, boxY + 110, clearMessageBody, GLUT_BITMAP_HELVETICA_12);
    }

    glutSwapBuffers();
}

// 창 크기 변경 콜백 함수
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glutPostRedisplay();
}

// 마우스 클릭/해제 콜백 함수
void mouse(int button, int state, int x, int y)
{
    if (isGameComplete)
    {
        return;
    }

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            // 오른쪽 패널의 도형 위에서 클릭했는지 확인
            for (Block& shape : answer)
            {
                const glm::vec2 start = {  shape.gridPos.x * CELL_WIDTH,
                                           shape.gridPos.y * CELL_HEIGHT };
                const glm::vec2 end   = { start.x + shape.size.x * CELL_WIDTH,
                                          start.y + shape.size.y * CELL_HEIGHT };

                if (x >= start.x && x <= end.x &&
                    y >= start.y && y <= end.y)
                {
                    draggedShape = &shape;
                    draggedShape->isDragging = true;
                    // 마우스 위치와 도형의 좌상단 모서리 사이의 오프셋 계산
                    dragOffset = { static_cast<float>(x) - start.x,
                                      static_cast<float>(y) - start.y };
                    break;
                }
            }
        }
        else if (state == GLUT_UP)
        {
            if (draggedShape)
            {
                // 도형을 그리드에 맞추기
                float newPosX = x - dragOffset.x;
                float newPosY = y - dragOffset.y;
                int gridX = static_cast<int>(round(newPosX / CELL_WIDTH));
                int gridY = static_cast<int>(round(newPosY / CELL_HEIGHT));

                // 도형이 왼쪽 패널로 넘어가지 않도록 함
                gridX = std::max(PANEL_GRID_WIDTH, gridX);
                // 도형이 패널 경계를 벗어나지 않도록 함
                gridX = std::min(gridX, 2 * PANEL_GRID_WIDTH - (int)draggedShape->size.x);
                gridY = std::max(0, std::min(gridY, PANEL_GRID_HEIGHT - (int)draggedShape->size.y));

                draggedShape->gridPos = { (float)gridX, (float)gridY };

                draggedShape->isDragging = false;
                draggedShape = nullptr;
                checkCompletion(); // 도형을 놓을 때마다 정답 확인
                glutPostRedisplay();
            }
        }
    }
}

// 마우스 드래그 콜백 함수
void motion(int x, int y)
{
    if (draggedShape)
    {
        // 드래그 중인 도형의 위치를 픽셀 단위로 업데이트 (그리기 위함)
        // 실제 gridPos는 마우스를 놓았을 때만 업데이트
        float newPosX = x - dragOffset.x;
        float newPosY = y - dragOffset.y;

        // 임시 위치를 그리드 단위로 변환하여 저장 (화면에 부드럽게 보이기 위함)
        draggedShape->gridPos.x = newPosX / CELL_WIDTH;
        draggedShape->gridPos.y = newPosY / CELL_HEIGHT;

        glutPostRedisplay();
    }
}

// 키보드 입력 콜백 함수
void keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
        case 'q': [[fallthrough]];
        case 'Q':
        {
            glutLeaveMainLoop();
            break;
        }
        case 'r': [[fallthrough]];
        case 'R':
        {
            init();
            break;
        }
        default:
        {
            break;
        }
    }

    glutPostRedisplay();
}


// 그리드를 그리는 함수
void drawGrid(float startX)
{
    glColor3f(0.2f, 0.2f, 0.25f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i <= GRID_SIZE; ++i)
    {
        // Vertical lines
        glVertex2f(startX + i * CELL_WIDTH, 0);
        glVertex2f(startX + i * CELL_WIDTH, WINDOW_HEIGHT);
        // Horizontal lines
        glVertex2f(startX, i * CELL_HEIGHT);
        glVertex2f(startX + PANEL_PIXEL_WIDTH, i * CELL_HEIGHT);
    }
    glEnd();
}

// 도형들을 그리는 함수
void drawShapes(const std::vector<Block>& shapes, float startX)
{
    for (const auto& shape : shapes)
    {
        glColor3f(shape.color.r, shape.color.g, shape.color.b);

        float drawX = startX + shape.gridPos.x * CELL_WIDTH;
        float drawY = shape.gridPos.y * CELL_HEIGHT;
        float width = shape.size.x * CELL_WIDTH;
        float height = shape.size.y * CELL_HEIGHT;

        glRectf(drawX, drawY, drawX + width, drawY + height);

        // 드래그 중일 때 테두리 추가
        if(shape.isDragging){
            glColor3f(1.0f, 1.0f, 0.0f); // 노란색 테두리
            glLineWidth(3.0f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(drawX, drawY);
            glVertex2f(drawX + width, drawY);
            glVertex2f(drawX + width, drawY + height);
            glVertex2f(drawX, drawY + height);
            glEnd();
        }
    }
}


// 화면에 텍스트를 그리는 함수
void drawText(float x, float y, const std::string& text, void* font)
{
    glRasterPos2f(x, y);
    std::ranges::for_each(text, [&font](const char c)
    {
        glutBitmapCharacter(font, c);
    });
}

// 게임 완료 조건을 확인하는 함수
void checkCompletion() {
    if (question.size() != answer.size()) {
        isGameComplete = false;
        return;
    }
    bool allMatch = true;
    for (const auto& block : answer) {
        bool foundMatch = false;
        for (const auto& pShape : question) {
            // ID가 같고, 사용자 도형의 위치가 문제 패널의 그리드 좌표와 일치하는지 확인
            // 사용자 도형의 x좌표는 오른쪽 패널에 있으므로 GRID_SIZE만큼 빼서 비교
            if (block.id == pShape.id &&
                static_cast<int>(round(block.gridPos.x)) - GRID_SIZE == static_cast<int>(pShape.gridPos.x) &&
                static_cast<int>(round(block.gridPos.y)) == static_cast<int>(pShape.gridPos.y)) {
                foundMatch = true;
                break;
            }
        }
        if (!foundMatch) {
            allMatch = false;
            break;
        }
    }

    isGameComplete = allMatch;
}