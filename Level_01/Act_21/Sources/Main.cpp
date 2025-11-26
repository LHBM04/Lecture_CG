#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <random>
#include <cmath>

// --- 설정 상수 ---
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
const float STAGE_SIZE = 20.0f;     // 무대 크기
const float WALL_THICKNESS = 0.5f;  // 벽 두께
const float GRAVITY = 20.0f;        // 중력 가속도

// --- 쉐이더 소스 ---
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 objectColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(objectColor, 1.0);\n"
"}\n\0";

// --- 구조체 정의 ---
struct Ball {
    glm::vec3 position; // 로컬 좌표
    glm::vec3 velocity; // 로컬 속도
    glm::vec3 color;
    float radius;
};

struct Block {
    glm::vec3 position; // 로컬 좌표
    glm::vec3 size;     // 크기 (width, height, depth)
    glm::vec3 color;
    float velocityX;    // 블록은 X축으로만 미끄러짐
	float velocityY;    // 바닥 열릴 때 낙하용
};

// --- 전역 상태 변수 ---
float stageAngleZ = 0.0f;       // 무대 회전 각도
bool isFloorOpen = false;       // 바닥 열림 상태
float floorOpenAngle = 0.0f;    // 바닥 애니메이션용 각도

glm::vec3 cameraPos(0.0f, 0.0f, 40.0f); // 카메라 초기 위치

std::vector<Ball> balls;
std::vector<Block> blocks;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int cubeVAO, cubeVBO;
// [추가] 구 렌더링을 위한 변수
unsigned int sphereVAO, sphereVBO, sphereEBO;
unsigned int sphereIndexCount = 0;

// --- 함수 선언 ---
unsigned int createShader(const char* vShader, const char* fShader);
void initCube();
void initSphere(); // [추가] 구 초기화 함수
void drawCube(unsigned int shaderProgram, glm::mat4 model, glm::vec3 color);
void drawSphere(unsigned int shaderProgram, glm::mat4 model, glm::vec3 color); // [추가] 구 그리기 함수
void spawnBall();
void initBlocks();
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    // 1. GLFW 초기화
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Physics Simulation", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    // 2. GLAD 초기화
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 3. OpenGL 설정
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    unsigned int shaderProgram = createShader(vertexShaderSource, fragmentShaderSource);
    initCube();
    initSphere(); // [추가] 구 모델 생성
    initBlocks();

    // 4. 렌더 루프
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // --- 물리 업데이트 (Local Space) ---
        float targetFloorAngle = isFloorOpen ? -90.0f : 0.0f;
        floorOpenAngle += (targetFloorAngle - floorOpenAngle) * 5.0f * deltaTime;

        float gravityFactorX = -sin(glm::radians(stageAngleZ)) * GRAVITY;

        // 블록 물리
        for (auto& block : blocks)
        {
            if (isFloorOpen)
            {
                block.position.y -= 10 * deltaTime;
            }

            block.velocityX += gravityFactorX * deltaTime;
            block.velocityX *= 0.98f;
            block.position.x += block.velocityX * deltaTime;

            float halfStage = STAGE_SIZE / 2.0f - WALL_THICKNESS;
            float halfBlock = block.size.x / 2.0f;

            if (block.position.x - halfBlock < -halfStage) {
                block.position.x = -halfStage + halfBlock;
                block.velocityX *= -0.5f;
            }
            else if (block.position.x + halfBlock > halfStage) {
                block.position.x = halfStage - halfBlock;
                block.velocityX *= -0.5f;
            }
        }

        // 공 물리
        for (auto& ball : balls) {
            ball.position += ball.velocity * deltaTime;

            float limit = STAGE_SIZE / 2.0f - WALL_THICKNESS - ball.radius;

            if (ball.position.x > limit) { ball.position.x = limit; ball.velocity.x *= -1.0f; }
            if (ball.position.x < -limit) { ball.position.x = -limit; ball.velocity.x *= -1.0f; }

            if (ball.position.y > limit) { ball.position.y = limit; ball.velocity.y *= -1.0f; }

            if (ball.position.y < -limit) {
                if (!isFloorOpen) {
                    ball.position.y = -limit;
                    ball.velocity.y *= -1.0f;
                }
            }
        }

        // --- 렌더링 ---
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        // Scene Graph Root
        glm::mat4 stageModel = glm::mat4(1.0f);
        stageModel = glm::rotate(stageModel, glm::radians(stageAngleZ), glm::vec3(0, 0, 1));

        // 1. 무대 그리기
        float s = STAGE_SIZE;
        float t = WALL_THICKNESS;
        float hs = s / 2.0f;

        drawCube(shaderProgram, stageModel * glm::translate(glm::mat4(1.0f), glm::vec3(0, hs + t / 2, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(s + t * 2, t, s)), glm::vec3(0.2f, 0.8f, 0.2f));
        drawCube(shaderProgram, stageModel * glm::translate(glm::mat4(1.0f), glm::vec3(-hs - t / 2, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(t, s, s)), glm::vec3(0.8f, 0.2f, 0.2f));
        drawCube(shaderProgram, stageModel * glm::translate(glm::mat4(1.0f), glm::vec3(hs + t / 2, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(t, s, s)), glm::vec3(0.2f, 0.2f, 0.8f));
        drawCube(shaderProgram, stageModel * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -hs - t / 2)) * glm::scale(glm::mat4(1.0f), glm::vec3(s, s, t)), glm::vec3(0.8f, 0.8f, 0.2f));

        glm::mat4 floorModel = stageModel;
        floorModel = glm::translate(floorModel, glm::vec3(-hs, -hs, 0));
        floorModel = glm::rotate(floorModel, glm::radians(floorOpenAngle), glm::vec3(0, 0, 1));
        floorModel = glm::translate(floorModel, glm::vec3(hs, -t / 2, 0));
        drawCube(shaderProgram, glm::scale(floorModel, glm::vec3(s, t, s)), glm::vec3(0.5f, 0.5f, 0.5f));


        // 2. 블록 그리기
        for (const auto& block : blocks) {
            for (const auto& block : blocks) {
                glm::mat4 blockModel = stageModel;
                // 낙하 누적값 block.position.y 반영
                blockModel = glm::translate(
                    blockModel,
                    glm::vec3(
                        block.position.x,
                        -hs + block.size.y / 2 + block.position.y,
                        block.position.z
                    )
                );
                blockModel = glm::scale(blockModel, block.size);
                drawCube(shaderProgram, blockModel, block.color);
            }
        }

        // 3. 공 그리기 (수정됨: drawSphere 사용)
        for (const auto& ball : balls) {
            glm::mat4 ballModel = stageModel;
            ballModel = glm::translate(ballModel, ball.position);
            // initSphere는 반지름 1.0짜리 구를 생성하므로, 반지름만큼만 스케일링하면 됨
            ballModel = glm::scale(ballModel, glm::vec3(ball.radius));
            drawSphere(shaderProgram, ballModel, ball.color);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    // [추가] 구 자원 해제
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &sphereEBO);

    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

// --- 초기화 및 입력 처리 함수 구현 ---

void initBlocks() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> widthDist(2.0f, 6.0f);
    std::uniform_real_distribution<float> colorDist(0.5f, 1.0f);

    float zPositions[] = { -5.0f, 0.0f, 5.0f };

    for (int i = 0; i < 3; i++) {
        Block b;
        b.size = glm::vec3(widthDist(gen), 3.0f, 3.0f);
        b.position = glm::vec3(0.0f, 0.0f, zPositions[i]);
        b.color = glm::vec3(colorDist(gen), colorDist(gen), colorDist(gen));
        b.velocityX = 0.0f;
        blocks.push_back(b);
    }
}

void spawnBall() {
    if (balls.size() >= 5) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> velDist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> colorDist(0.2f, 1.0f);

    Ball b;
    b.position = glm::vec3(0, 0, 0);
    b.velocity = glm::vec3(velDist(gen), velDist(gen), 0.0f);
    if (glm::length(b.velocity) < 2.0f) b.velocity = glm::normalize(b.velocity) * 5.0f;

    b.radius = 0.8f;
    b.color = glm::vec3(colorDist(gen), 0.2f, 0.2f);
    balls.push_back(b);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float normalizedX = (float)xpos / SCR_WIDTH;
    float angle = (normalizedX - 0.5f) * 2.0f * 60.0f;

    if (angle < -60.0f) angle = -60.0f;
    if (angle > 60.0f) angle = 60.0f;

    stageAngleZ = angle;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) isFloorOpen = true;
        else if (action == GLFW_RELEASE) isFloorOpen = false;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        spawnBall();
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 20.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cameraPos.z -= cameraSpeed;
        else cameraPos.z += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cameraPos.y -= cameraSpeed;
        else cameraPos.y += cameraSpeed;
    }
}

void drawCube(unsigned int shaderProgram, glm::mat4 model, glm::vec3 color) {
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glUniform3fv(colorLoc, 1, &color[0]);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// [추가] 구 그리기 함수
void drawSphere(unsigned int shaderProgram, glm::mat4 model, glm::vec3 color) {
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glUniform3fv(colorLoc, 1, &color[0]);

    glBindVertexArray(sphereVAO);
    // 인덱스를 사용하여 그립니다.
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
}

void initCube() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

// [추가] 구 메시 데이터 생성 및 버퍼 할당
void initSphere() {
    const int stacks = 18;  // 가로 분할 수
    const int sectors = 36; // 세로 분할 수
    const float radius = 1.0f;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // 1. 정점 생성
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = 3.141592f / 2 - (float)i / stacks * 3.141592f; // pi/2 ~ -pi/2
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = (float)j / sectors * 2 * 3.141592f; // 0 ~ 2pi

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            // 위치 벡터만 저장 (layout 0)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    // 2. 인덱스(삼각형) 생성
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    sphereIndexCount = indices.size();

    // 3. OpenGL 버퍼 설정
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

unsigned int createShader(const char* vShader, const char* fShader) {
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShader, NULL);
    glCompileShader(vertex);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShader, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}