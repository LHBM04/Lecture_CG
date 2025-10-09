#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cmath>

static const char* kVS = R"(#version 330 core
// 최대 정점 수
const int MAXV = 5;
// 키프레임 수 (선, 삼각형, 사각형, 오각형, 선)
const int KEYN = 5;

// 각 키프레임의 5개 정점 좌표 (NDC). gl_VertexID로 인덱싱.
uniform vec2 uKey[KEYN][MAXV];

// 0..KEYN 사이를 순환하는 페이즈. floor(uPhase)가 현재 키, 그다음이 다음 키.
uniform float uPhase; // 예: 0.0=선→삼각형, 1.0=삼→사, 2.0=사→오, 3.0=오→선, 4.0==0과 동일

void main()
{
    int vi = gl_VertexID;     // 0..4
    float p = fract(uPhase);  // 구간 내 t
    int k0 = int(floor(uPhase)) % KEYN;
    int k1 = (k0 + 1) % KEYN;

    vec2 p0 = uKey[k0][vi];
    vec2 p1 = uKey[k1][vi];
    vec2 pos = mix(p0, p1, p);

    gl_Position = vec4(pos, 0.0, 1.0);
}
)";

static const char* kFS = R"(#version 330 core
out vec4 FragColor;
uniform float uPhase;
void main()
{
    // 구간별로 살짝 색상 변화 (디버깅에 유용).
    float seg = floor(uPhase);
    float t   = fract(uPhase);
    FragColor = vec4(0.2 + 0.15*seg, 0.6*(1.0-t)+0.4*t, 0.9-0.15*seg, 1.0);
}
)";

static GLuint Compile(GLenum type, const char* src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if(!ok){ char log[1024]; glGetShaderInfoLog(s,1024,nullptr,log); std::fprintf(stderr,"Shader error:\n%s\n",log); std::exit(1); }
    return s;
}
static GLuint Link(GLuint vs, GLuint fs)
{
    GLuint p = glCreateProgram();
    glAttachShader(p, vs); glAttachShader(p, fs); glLinkProgram(p);
    GLint ok; glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if(!ok){ char log[1024]; glGetProgramInfoLog(p,1024,nullptr,log); std::fprintf(stderr,"Link error:\n%s\n",log); std::exit(1); }
    return p;
}

int main()
{
    if(!glfwInit()){ std::fprintf(stderr,"GLFW init fail\n"); return 1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif
    GLFWwindow* win = glfwCreateWindow(800,600,"Line->Tri->Quad->Penta->Line",nullptr,nullptr);
    if(!win){ std::fprintf(stderr,"Window create fail\n"); glfwTerminate(); return 1; }
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ std::fprintf(stderr,"GLAD init fail\n"); return 1; }

    GLuint vs = Compile(GL_VERTEX_SHADER, kVS);
    GLuint fs = Compile(GL_FRAGMENT_SHADER, kFS);
    GLuint prog = Link(vs, fs);
    glDeleteShader(vs); glDeleteShader(fs);

    // 인덱스: 삼각형 팬 (v0 고정 중심 아님; 여기선 단순 외곽 팬)
    // (0,1,2), (0,2,3), (0,3,4)
    const unsigned int indices[] = { 0,1,2, 0,2,3, 0,3,4 };
    GLuint vao=0, ebo=0;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 키프레임 좌표 정의 (NDC) — 부족한 꼭짓점은 "겹쳐서" 채움
    // 0) 선: v0..v4 중 v0=-0.7, v1=0.7 수평선, v2=v1, v3=v1, v4=v1
    const float K0[10] = { -0.7f,0.0f,   0.7f,0.0f,   0.7f,0.0f,   0.7f,0.0f,   0.7f,0.0f };
    // 1) 삼각형: v0=-0.6,-0.4, v1=0.0,0.7, v2=0.6,-0.4, v3=v2, v4=v2
    const float K1[10] = { -0.6f,-0.4f,  0.0f,0.7f,   0.6f,-0.4f,  0.6f,-0.4f,  0.6f,-0.4f };
    // 2) 사각형: v0=-0.6,-0.4, v1=-0.6,0.6, v2=0.6,-0.4, v3=0.6,0.6, v4=v3
    const float K2[10] = { -0.6f,-0.4f, -0.6f,0.6f,   0.6f,-0.4f,  0.6f,0.6f,   0.6f,0.6f };
    // 3) 오각형: 정오각형 대략값
    const float K3[10] = {
        0.0f, 0.7f,
        -0.67f, 0.22f,
        -0.41f, -0.57f,
        0.41f, -0.57f,
        0.67f, 0.22f
    };
    // 4) 선(다시): K0와 동일
    const float K4[10] = { -0.7f,0.0f,   0.7f,0.0f,   0.7f,0.0f,   0.7f,0.0f,   0.7f,0.0f };

    // uKey 업로드: uniform 배열로(작아서 문제 없음)
    glUseProgram(prog);
    auto locKey = [&](int k, int vi, const char* base)->GLint{
        // GLSL: uKey[k][vi]
        char name[64];
        std::snprintf(name, sizeof(name), "%s[%d][%d]", base, k, vi);
        return glGetUniformLocation(prog, name);
    };
    const float* KEYS[5] = { K0, K1, K2, K3, K4 };
    for(int k=0;k<5;++k){
        for(int vi=0; vi<5; ++vi){
            GLint l = locKey(k,vi,"uKey");
            glUniform2f(l, KEYS[k][vi*2+0], KEYS[k][vi*2+1]);
        }
    }
    GLint locPhase = glGetUniformLocation(prog, "uPhase");

    glDisable(GL_CULL_FACE); // 일부 구간에서 중첩/뒤집힘 방지용

    while(!glfwWindowShouldClose(win))
    {
        glfwPollEvents();
        double tm = glfwGetTime();

        // uPhase: 0→4 순환 (각 구간 길이 = 1.0)
        float cyclesPerSec = 0.25f; // 4초에 한 바퀴
        float phase = fmodf((float)tm * cyclesPerSec, 4.0f) + 0.0f; // [0,4)

        glViewport(0,0,800,600);
        glClearColor(0.08f,0.08f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glUniform1f(locPhase, phase);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(win);
    }

    glDeleteProgram(prog);
    glDeleteBuffers(1,&ebo);
    glDeleteVertexArrays(1,&vao);
    glfwTerminate();
    return 0;
}
