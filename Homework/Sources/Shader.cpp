#include "Shader.h"

#include "Resources.h"

bool Shader::Initialize() noexcept
{
	GLuint vertexShader   = CompileShader(GL_VERTEX_SHADER,   "Resources/Shaders/Vertex.vert");
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, "Resources/Shaders/Fragment.frag");

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(programID);
    isUsing = true;

    return true;
}

void Shader::Release() noexcept
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}

GLuint Shader::CompileShader(GLenum           type_,
							 std::string_view path_) noexcept
{
    const std::string code = Resources::GetShader(path_);
    const GLchar* src      = code.c_str();

    GLuint shader = glCreateShader(type_);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    return shader;
}

bool Shader::isUsing = false;

GLuint Shader::programID = 0;