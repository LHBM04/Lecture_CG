// main.cpp : Archimedean Spiral (dot cloud)
// Requirements: GLFW, GLAD, GLM (OpenGL 3.3 Core)

#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::ortho
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

// ========== Minimal Shader Helper ==========
class Shader
{
public:
    explicit Shader(const char* vs, const char* fs)
    {
        GLuint v = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(v, 1, &vs, nullptr);
        glCompileShader(v);
        Check(v, "VERTEX");

        GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(f, 1, &fs, nullptr);
        glCompileShader(f);
        Check(f, "FRAGMENT");

        mProgram = glCreateProgram();
        glAttachShader(mProgram, v);
        glAttachShader(mProgram, f);
        glLinkProgram(mProgram);
        Check(mProgram, "PROGRAM", true);

        glDeleteShader(v);
        glDeleteShader(f);
    }

    ~Shader() { glDeleteProgram(mProgram); }
    void Use() const { glUseProgram(mProgram); }
    GLuint Id() const { return mProgram; }

private:
    GLuint mProgram = 0;

    static void Check(GLuint obj, const char* stage, bool isProgram = false)
    {
        GLint ok = 0;
        if (!isProgram)
        {
            glGetShaderiv(obj, GL_COMPILE_STATUS, &ok);
            if (!ok)
            {
                GLint len = 0; glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &len);
                std::string log(len, '\0');
                glGetShaderInfoLog(obj, len, nullptr, log.data());
                std::cerr << "[" << stage << " COMPILE ERROR]\n" << log << "\n";
                std::exit(EXIT_FAILURE);
            }
        }
        else
        {
            glGetProgramiv(obj, GL_LINK_STATUS, &ok);
            if (!ok)
            {
                GLint len = 0; glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &len);
                std::string log(len, '\0');
                glGetProgramInfoLog(obj, len, nullptr, log.data());
                std::cerr << "[" << stage << " LINK ERROR]\n" << log << "\n";
                std::exit(EXIT_FAILURE);
            }
        }
    }
};

// ========== Spiral Point Generator ==========
// 이유: r = k * θ (k = pitch / 2π). 균일 간격 s를 유지하려고
// Δθ ≈ s / sqrt(r^2 + (dr/dθ)^2) = s / sqrt((kθ)^2 + k^2)를 사용.
static std::vector<glm::vec2>
GenerateSpiralDots(const glm::vec2 centerPx,
                   float pitchPx,     // 팔 사이 간격 (pixels)
                   float turns,       // 회전 수
                   float spacingPx,   // 점 간 거리 (pixels)
                   float startAngle = 0.0f,
                   bool clockwise = false)
{
    std::vector<glm::vec2> pts;
    if (pitchPx <= 0.0f || spacingPx <= 0.0f || turns <= 0.0f) { return pts; }

    const float PI = 3.14159265358979323846f;
    const float k  = pitchPx / (2.0f * PI);
    const float dir = clockwise ? -1.0f : 1.0f;

    float theta = 0.0f;
    const float thetaMax = 2.0f * PI * turns;

    while (theta <= thetaMax)
    {
        const float r   = k * theta;
        const float ang = startAngle + dir * theta;

        const float x = centerPx.x + r * std::cos(ang);
        const float y = centerPx.y + r * std::sin(ang);
        pts.emplace_back(x, y);

        const float dTheta = spacingPx / std::sqrt(r * r + k * k);
        theta += dTheta;
    }
    return pts;
}

// ========== Globals ==========
static int   gWinW = 960;
static int   gWinH = 540;
static float gPointSize = 3.0f;
static bool  gAnimate = true;
static size_t gDrawCount = 0;

static std::vector<glm::vec2> gPoints; // merged left+right
static GLuint gVAO = 0, gVBO = 0;

// ========== Shaders ==========
static const char* kVS = R"(#version 330 core
layout (location = 0) in vec2 aPos;    // pixel space
uniform mat4 uMVP;                      // ortho projection
void main()
{
    gl_Position = uMVP * vec4(aPos, 0.0, 1.0);
    gl_PointSize = 1.0; // ignored in core unless gl_PointSize set via state, we use glPointSize
}
)";

static const char* kFS = R"(#version 330 core
out vec4 FragColor;
uniform vec3 uColor;
void main()
{
    FragColor = vec4(uColor, 1.0);
}
)";

// ========== Resize Callback ==========
static void OnFramebufferSize(GLFWwindow*, int w, int h)
{
    gWinW = (w > 1 ? w : 1);
    gWinH = (h > 1 ? h : 1);
    glViewport(0, 0, gWinW, gWinH);
}

// ========== Input Toggle ==========
static void OnKey(GLFWwindow* win, int key, int, int action, int)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_SPACE) { gAnimate = !gAnimate; }
        if (key == GLFW_KEY_ESCAPE) { glfwSetWindowShouldClose(win, GLFW_TRUE); }
    }
}

// ========== Buffer Upload Helper ==========
static void UploadPoints()
{
    if (gVAO == 0) { glGenVertexArrays(1, &gVAO); }
    if (gVBO == 0) { glGenBuffers(1, &gVBO); }

    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, gPoints.size() * sizeof(glm::vec2), gPoints.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

    glBindVertexArray(0);
}

// ========== Main ==========
int main()
{
    // --- GLFW/GLAD init
    if (!glfwInit()) { std::cerr << "GLFW init failed\n"; return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(gWinW, gWinH, "Spiral Dots (GLFW+GLAD+GLM)", nullptr, nullptr);
    if (!window) { std::cerr << "Window creation failed\n"; glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, OnFramebufferSize);
    glfwSetKeyCallback(window, OnKey);
    glfwSwapInterval(1); // vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD load failed\n"; return -1;
    }
    glViewport(0, 0, gWinW, gWinH);

    // --- Shader
    Shader shader(kVS, kFS);

    // --- Generate two spirals in pixel space
    // 배치 이유: 이미지처럼 화면 상단 중앙에 좌/우 대칭 스파이럴 2개.
    const float cxLeft  = gWinW * 0.35f;
    const float cxRight = gWinW * 0.65f;
    const float cy      = gWinH * 0.65f;

    const float pitch   = 28.0f; // 팔 간격(px)
    const float turns   = 3.0f;
    const float spacing = 8.0f;  // 점 간격(px)
    const float start   = 0.0f;

    auto left  = GenerateSpiralDots({cxLeft + 60,  cy}, pitch, turns, spacing, start, /*clockwise*/false);
    auto right = GenerateSpiralDots({cxRight + 60, cy}, pitch, turns, spacing, start, /*clockwise*/true);

    for (auto& p : right) { p.x = 2.0f * cxRight - p.x; }

    gPoints.reserve(left.size() + right.size());
    gPoints.insert(gPoints.end(), left.begin(), left.end());
    gPoints.insert(gPoints.end(), right.begin(), right.end());

    UploadPoints();

    // --- GL state
    glClearColor(0.02f, 0.02f, 0.03f, 1.0f);
    glEnable(GL_PROGRAM_POINT_SIZE); // 일부 드라이버에서 필요
    glPointSize(gPointSize);

    // --- Animation control
    gDrawCount = 0; // 0부터 점진적으로 증가
    const size_t total = gPoints.size();

    // --- Render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();

        // Ortho: 픽셀 좌표(0..W, 0..H)를 NDC로 투영.
        const glm::mat4 proj = glm::ortho(0.0f, (float)gWinW, 0.0f, (float)gWinH);
        glUniformMatrix4fv(glGetUniformLocation(shader.Id(), "uMVP"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniform3f(glGetUniformLocation(shader.Id(), "uColor"), 1.0f, 1.0f, 1.0f);

        glBindVertexArray(gVAO);

        if (gAnimate && gDrawCount < total)
        {
            // 프레임당 고정 증가(간단). 필요시 dt로 조절.
            gDrawCount += 8; // 속도
            if (gDrawCount > total) { gDrawCount = total; }
        }

        const GLsizei count = (GLsizei)(gAnimate ? gDrawCount : total);
        glDrawArrays(GL_POINTS, 0, count);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    // --- Cleanup
    glDeleteBuffers(1, &gVBO);
    glDeleteVertexArrays(1, &gVAO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
