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
const float STAGE_SIZE = 20.0f;
const float ROBOT_SCALE = 1.0f;

// --- 쉐이더 소스 (Vertex & Fragment) ---
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
struct Obstacle {
    glm::vec3 position;
    glm::vec3 size; // Half-extents or full scale
    glm::vec3 color;
};

struct Robot {
    glm::vec3 position;
    float rotationY; // 로봇이 바라보는 방향 (각도)
    float speed;
    float verticalVelocity; // 점프용
    bool isJumping;

    // 애니메이션 상태
    float walkTime;
};

struct Camera {
    glm::vec3 position;
    glm::vec3 target;
    float orbitAngle;
};

// --- 전역 상태 변수 ---
Robot robot;
Camera camera;
std::vector<Obstacle> obstacles;
bool isDoorOpen = false;
float doorSlideHeight = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 큐브 렌더링을 위한 VAO, VBO
unsigned int cubeVAO, cubeVBO;

// --- 유틸리티 함수 선언 ---
unsigned int createShader(const char* vShader, const char* fShader);
void initCube();
void drawCube(unsigned int shaderProgram, glm::mat4 model, glm::vec3 color);
void resetSimulation();
void processInput(GLFWwindow* window);
bool checkCollision(glm::vec3 newPos, float& groundHeight);

// --- 메인 함수 ---
int main()
{
    // 1. GLFW 초기화
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Robot Simulation", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 2. GLAD 초기화
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // 컬링 설정: 무대의 내부는 보여야 하므로, 일반적인 Back-face culling을 켜되
    // 무대 벽을 그릴 때 Normal을 안쪽으로 뒤집거나, 그리는 순서를 관리해야 함.
    // 여기서는 편의상 Culling을 끄거나, 각 벽을 별도의 큐브로 그려서 해결함.
    // (사양서: "카메라가 회전해도 항상 내부가 보임") -> Back Face Culling을 켜고 벽을 바깥에서 감싸는 형태가 아니라 '판'으로 조립.
    // glEnable(GL_CULL_FACE);

    // 3. 리소스 초기화
    unsigned int shaderProgram = createShader(vertexShaderSource, fragmentShaderSource);
    initCube();
    resetSimulation();

    // 랜덤 시드
    std::srand(static_cast<unsigned int>(glfwGetTime()));

    // 4. 렌더 루프
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 입력 처리 및 로직 업데이트
        processInput(window);

        // 문 애니메이션 처리
        float targetDoorHeight = isDoorOpen ? 10.0f : 0.0f;
        doorSlideHeight += (targetDoorHeight - doorSlideHeight) * 2.0f * deltaTime; // 부드러운 보간

        // 로봇 중력 및 점프 물리
        float groundHeight = 0.0f;
        if (robot.isJumping || robot.position.y > 0.0f) {
            robot.verticalVelocity -= 9.8f * deltaTime; // 중력
            robot.position.y += robot.verticalVelocity * deltaTime;

            // 충돌 체크 (바닥 높이 결정)
            checkCollision(robot.position, groundHeight);

            if (robot.position.y <= groundHeight) {
                robot.position.y = groundHeight;
                robot.verticalVelocity = 0.0f;
                robot.isJumping = false;
            }
        }

        // 화면 클리어
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // --- View & Projection Matrix ---
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // 카메라 궤도 공전 (Orbit) 로직 적용
        float camX = sin(camera.orbitAngle) * 30.0f + camera.position.x;
        float camZ = cos(camera.orbitAngle) * 30.0f + camera.position.z;
        glm::vec3 finalCamPos = glm::vec3(camX, 20.0f + camera.position.y, camZ); // 약간 위에서

        glm::mat4 view = glm::lookAt(finalCamPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        // --- 1. 무대 그리기 (5개의 벽 + 문) ---
        // 무대의 각 면은 얇은 육면체(판)로 구현하여 내부 공간 확보
        float wallThick = 0.5f;
        float halfSize = STAGE_SIZE / 2.0f;

        // 바닥 (회색)
        drawCube(shaderProgram, glm::translate(glm::mat4(1.0f), glm::vec3(0, -wallThick / 2, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(STAGE_SIZE, wallThick, STAGE_SIZE)), glm::vec3(0.3f, 0.3f, 0.3f));

        // 뒷면 (파랑)
        drawCube(shaderProgram, glm::translate(glm::mat4(1.0f), glm::vec3(0, halfSize, -halfSize - wallThick / 2)) * glm::scale(glm::mat4(1.0f), glm::vec3(STAGE_SIZE, STAGE_SIZE, wallThick)), glm::vec3(0.2f, 0.2f, 0.8f));

        // 왼쪽면 (초록)
        drawCube(shaderProgram, glm::translate(glm::mat4(1.0f), glm::vec3(-halfSize - wallThick / 2, halfSize, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(wallThick, STAGE_SIZE, STAGE_SIZE)), glm::vec3(0.2f, 0.8f, 0.2f));

        // 오른쪽면 (노랑)
        drawCube(shaderProgram, glm::translate(glm::mat4(1.0f), glm::vec3(halfSize + wallThick / 2, halfSize, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(wallThick, STAGE_SIZE, STAGE_SIZE)), glm::vec3(0.8f, 0.8f, 0.2f));

        // 천장 (하늘색, 선택사항) - 내부를 보기 위해 생략하거나 그릴 수 있음. 여기선 개방감 위해 생략.

        // 앞면 (빨강) - 슬라이딩 도어
        // o/O 키를 누르면 위로 올라감 (doorSlideHeight 적용)
        drawCube(shaderProgram, glm::translate(glm::mat4(1.0f), glm::vec3(0, halfSize + doorSlideHeight, halfSize + wallThick / 2)) * glm::scale(glm::mat4(1.0f), glm::vec3(STAGE_SIZE, STAGE_SIZE, wallThick)), glm::vec3(0.8f, 0.2f, 0.2f));


        // --- 2. 장애물 그리기 ---
        for (const auto& obs : obstacles) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(obs.position.x, obs.position.y + obs.size.y / 2, obs.position.z));
            model = glm::scale(model, obs.size);
            drawCube(shaderProgram, model, obs.color);
        }

        // --- 3. 로봇 그리기 (계층적 모델링) ---
        // 로봇 기준 행렬
        glm::mat4 robotModel = glm::mat4(1.0f);
        // 로봇의 중심점(발바닥 기준)이 (0,0,0)에 오도록 오프셋 고려. 몸통 높이만큼 띄움.
        float legLength = 1.5f;
        float bodySize = 1.5f;

        robotModel = glm::translate(robotModel, glm::vec3(robot.position.x, robot.position.y + legLength + bodySize / 2, robot.position.z));
        robotModel = glm::rotate(robotModel, glm::radians(robot.rotationY), glm::vec3(0, 1, 0));

        // 애니메이션 각도 계산
        float swingAngle = 0.0f;
        float speedFactor = robot.speed / 2.0f; // 속도에 따라 각도 변화
        if (robot.speed > 0.1f) {
            swingAngle = glm::sin(robot.walkTime * 10.0f) * glm::radians(45.0f * speedFactor);
        }

        // 3-1. 몸통
        drawCube(shaderProgram, glm::scale(robotModel, glm::vec3(1.0f, 1.5f, 0.8f)), glm::vec3(0.5f, 0.5f, 0.5f));

        // 3-2. 머리
        glm::mat4 headModel = glm::translate(robotModel, glm::vec3(0, 1.25f, 0));
        drawCube(shaderProgram, glm::scale(headModel, glm::vec3(0.8f, 0.8f, 0.8f)), glm::vec3(0.9f, 0.7f, 0.7f));

        // 3-3. 코 (앞뒤 구분용)
        glm::mat4 noseModel = glm::translate(headModel, glm::vec3(0, 0, 0.4f)); // 머리 앞쪽
        drawCube(shaderProgram, glm::scale(noseModel, glm::vec3(0.2f, 0.2f, 0.2f)), glm::vec3(1.0f, 0.0f, 0.0f)); // 빨간 코

        // 3-4. 팔 (왼쪽/오른쪽 다른색)
        // 왼쪽 팔
        glm::mat4 lArm = glm::translate(robotModel, glm::vec3(-0.8f, 0.5f, 0));
        lArm = glm::rotate(lArm, swingAngle, glm::vec3(1, 0, 0)); // 어깨 관절 회전
        lArm = glm::translate(lArm, glm::vec3(0, -0.6f, 0)); // 피벗 포인트 조정
        drawCube(shaderProgram, glm::scale(lArm, glm::vec3(0.4f, 1.2f, 0.4f)), glm::vec3(0.2f, 0.2f, 0.8f));

        // 오른쪽 팔
        glm::mat4 rArm = glm::translate(robotModel, glm::vec3(0.8f, 0.5f, 0));
        rArm = glm::rotate(rArm, -swingAngle, glm::vec3(1, 0, 0)); // 반대 위상
        rArm = glm::translate(rArm, glm::vec3(0, -0.6f, 0));
        drawCube(shaderProgram, glm::scale(rArm, glm::vec3(0.4f, 1.2f, 0.4f)), glm::vec3(0.8f, 0.2f, 0.2f));

        // 3-5. 다리 (왼쪽/오른쪽 다른색)
        // 다리는 몸통 아래에서 시작
        // 왼쪽 다리
        glm::mat4 lLeg = glm::translate(robotModel, glm::vec3(-0.3f, -0.75f, 0));
        lLeg = glm::rotate(lLeg, -swingAngle, glm::vec3(1, 0, 0)); // 팔과 반대 위상
        lLeg = glm::translate(lLeg, glm::vec3(0, -0.75f, 0));
        drawCube(shaderProgram, glm::scale(lLeg, glm::vec3(0.4f, 1.5f, 0.4f)), glm::vec3(0.2f, 0.8f, 0.8f));

        // 오른쪽 다리
        glm::mat4 rLeg = glm::translate(robotModel, glm::vec3(0.3f, -0.75f, 0));
        rLeg = glm::rotate(rLeg, swingAngle, glm::vec3(1, 0, 0));
        rLeg = glm::translate(rLeg, glm::vec3(0, -0.75f, 0));
        drawCube(shaderProgram, glm::scale(rLeg, glm::vec3(0.4f, 1.5f, 0.4f)), glm::vec3(0.8f, 0.2f, 0.8f));


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

// --- 함수 구현부 ---

void resetSimulation() {
    // 로봇 초기화
    robot.position = glm::vec3(0, 0, 0);
    robot.rotationY = 0.0f;
    robot.speed = 2.0f;
    robot.verticalVelocity = 0.0f;
    robot.isJumping = false;
    robot.walkTime = 0.0f;

    // 카메라 초기화
    camera.position = glm::vec3(0, 0, 0);
    camera.orbitAngle = 0.0f;

    // 장애물 랜덤 생성
    obstacles.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDist(-8.0f, 8.0f);
    std::uniform_real_distribution<float> sizeDist(1.0f, 3.0f);
    std::uniform_real_distribution<float> colorDist(0.2f, 1.0f);

    for (int i = 0; i < 5; ++i) {
        Obstacle obs;
        obs.position = glm::vec3(posDist(gen), 0, posDist(gen));
        // 로봇 시작 위치 근처는 피함
        if (glm::length(obs.position) < 3.0f) obs.position.x += 5.0f;

        float h = sizeDist(gen);
        obs.size = glm::vec3(sizeDist(gen), h, sizeDist(gen)); // y는 높이
        obs.color = glm::vec3(colorDist(gen), colorDist(gen), colorDist(gen));
        obstacles.push_back(obs);
    }

    isDoorOpen = false;
    doorSlideHeight = 0.0f;
}

// AABB 충돌 검사 (XZ 평면 + 높이 체크)
bool checkCollision(glm::vec3 nextPos, float& groundHeight) {
    float robotRadius = 0.5f; // 근사치
    groundHeight = 0.0f;
    bool collided = false;

    for (const auto& obs : obstacles) {
        // AABB 계산
        float minX = obs.position.x - obs.size.x / 2;
        float maxX = obs.position.x + obs.size.x / 2;
        float minZ = obs.position.z - obs.size.z / 2;
        float maxZ = obs.position.z + obs.size.z / 2;

        // XZ 평면에서 로봇이 장애물 내부에 있는지 확인
        if (nextPos.x + robotRadius > minX && nextPos.x - robotRadius < maxX &&
            nextPos.z + robotRadius > minZ && nextPos.z - robotRadius < maxZ) {

            // 장애물 위에 있는지 확인 (현재 로봇의 y가 장애물 높이보다 높거나 같으면 '위'로 간주)
            // 약간의 오차(0.1f)를 두어 자연스럽게 올라가게 함
            if (nextPos.y >= obs.size.y - 0.5f) {
                if (obs.size.y > groundHeight) {
                    groundHeight = obs.size.y;
                }
            }
            else {
                // 장애물 벽에 부딪힘
                return true;
            }
        }
    }
    return false; // 벽 충돌 없음 (바닥 높이만 갱신됨)
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // 리셋
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        resetSimulation();

    // 문 개폐 (토글은 키 콜백이 좋지만, 여기선 간단히 처리)
    static bool oPressed = false;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        if (!oPressed) {
            isDoorOpen = !isDoorOpen;
            oPressed = true;
        }
    }
    else {
        oPressed = false;
    }

    // 로봇 이동
    glm::vec3 direction(0.0f);
    bool isMoving = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { direction.z = 1.0f; robot.rotationY = 0.0f; isMoving = true; }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { direction.z = -1.0f; robot.rotationY = 180.0f; isMoving = true; }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { direction.x = 1.0f; robot.rotationY = 90.0f; isMoving = true; }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { direction.x = -1.0f; robot.rotationY = -90.0f; isMoving = true; }

    // 속도 조절
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) robot.speed += 1.0f * deltaTime; // + 키
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) robot.speed = std::max(0.1f, robot.speed - 1.0f * deltaTime);

    // 점프
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && !robot.isJumping) {
        robot.verticalVelocity = 5.0f; // 점프 강도
        robot.isJumping = true;
    }

    if (isMoving) {
        robot.walkTime += deltaTime * robot.speed;

        glm::vec3 moveDir = glm::vec3(sin(glm::radians(robot.rotationY)), 0, cos(glm::radians(robot.rotationY)));
        glm::vec3 nextPos = robot.position + moveDir * robot.speed * deltaTime;

        // 경계 체크 (반전)
        float limit = STAGE_SIZE / 2.0f - 1.0f;
        if (nextPos.x > limit || nextPos.x < -limit || nextPos.z > limit || nextPos.z < -limit) {
            robot.rotationY += 180.0f; // 뒤로 돎
            // 뒤로 돌아서 걷는 로직은 다음 프레임부터 적용됨
        }
        else {
            // 장애물 충돌 체크 (벽인지 바닥인지)
            float dummyH;
            if (!checkCollision(nextPos, dummyH)) {
                robot.position.x = nextPos.x;
                robot.position.z = nextPos.z;
            }
        }
    }
    else {
        robot.walkTime = 0; // 정지 시 애니메이션 리셋
    }

    // 카메라 이동
    float camSpeed = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.position.z -= camSpeed; // Z (대문자 shift)
        else camera.position.z += camSpeed; // z
    }
    // (대소문자 구분은 GLFW에서 modifier로 처리하나 편의상 같은 키로 매핑)
    // 실제 z/Z 구분을 위해선 callback을 써야하나, 여기선 방향키처럼 Z를 앞뒤로 해석하여 구현.
    // 문제의 "z/Z: 앞/뒤" -> 그냥 토글이 아니므로 z 누르면 앞으로, shift+z(Z) 누르면 뒤로 등으로 해석 가능하지만
    // 통상적으로 w/s 처럼 z/x에 방향을 할당합니다. 여기선 z=앞, shift+z=뒤 로 간주하지 않고,
    // 키 입력을 단순히 분리합니다. 여기서는 기능 구현을 위해 z: Zoom In, x: Pan 등으로 단순화합니다.

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) camera.position.z -= camSpeed; // Forward
    // Shift 키 등 조합이 없으면 키코드가 같으므로 로직상 하나로 처리되거나 별도 콜백 필요. 
    // 여기서는 z:앞, c:뒤 처럼 키를 나누지 않고, 문제 그대로 구현하려 노력:
    // 그러나 GLFW polling은 대소문자 구분이 안되므로, Z키 하나로 앞뒤를 토글하거나 modifier 확인 필요.
    // --> 수정: 직관성을 위해 Z: 앞, LEFT_CONTROL + Z : 뒤로 구현.
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.position.z -= camSpeed;
        else camera.position.z += camSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.position.x -= camSpeed;
        else camera.position.x += camSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.orbitAngle -= camSpeed * 0.5f;
        else camera.orbitAngle += camSpeed * 0.5f;
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

unsigned int createShader(const char* vShader, const char* fShader) {
    // Vertex Shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShader, NULL);
    glCompileShader(vertex);

    // Fragment Shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShader, NULL);
    glCompileShader(fragment);

    // Program
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}