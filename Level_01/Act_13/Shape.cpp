#include "Shape.h"

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Application.h"

Shape::Shape(const Shape::Type type_,
             const glm::vec2   position_) noexcept
    : type(type_)
    , position(position_)
    , scale(100.0f)
    , direction({0.0f, 0.0f})
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const auto vertices = SHAPES_VERTICES.at(type_);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_DYNAMIC_DRAW);

    const auto indices = SHAPE_INDICES.at(type_);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Shape::~Shape() noexcept
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Shape::Update(const float deltaTime_) noexcept
{
    // 방향 벡터가 (0,0)인 경우에만 업데이트를 건너뛰기
    if (direction.x == 0.0f && direction.y == 0.0f)
    {
        return;
    }

    constexpr float moveSpeed = 30.0f;
    position += direction * (deltaTime_ * moveSpeed);

    const float halfWidth  = 0.5f * scale;
    const float halfHeight = 0.5f * scale;

    constexpr float halfWorldWidth  = WINDOW_WIDTH  / 2.0f;
    constexpr float halfWorldHeight = WINDOW_HEIGHT / 2.0f;

    if (position.x - halfWidth < -halfWorldWidth ||
        position.x + halfWidth >  halfWorldWidth)
    {
        direction.x *= -1.0f;

        const float max =  halfWorldWidth - halfWidth;
        const float min = -halfWorldWidth + halfWidth;
        position.x = glm::clamp(position.x, min, max);
    }

    if (position.y - halfHeight < -halfWorldHeight ||
        position.y + halfHeight >  halfWorldHeight)
    {
        direction.y *= -1.0f;

        const float max =  halfWorldHeight - halfHeight;
        const float min = -halfWorldHeight + halfHeight;

        position.y = glm::clamp(position.y, min, max);
    }
}

void Shape::Render(const Shader& shader_) const noexcept
{
    const auto vertices      = SHAPES_VERTICES.at(type);
    const auto indices= SHAPE_INDICES.at(type);
    const auto color                 = SHAPE_COLORS.at(type);

    glm::mat4 model = {1.0f};
    model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, 1.0f));

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
    glUniform3fv(colorLoc, 1, &color[0]);

    glBindVertexArray(vao);

    if (type == Shape::Type::Dot)
    {
        // 포인트 크기 설정 (원복을 위해 이전값 저장)
        GLfloat prevPointSize = 1.0f;
        glGetFloatv(GL_POINT_SIZE, &prevPointSize);
        glPointSize(5.0f); // 필요 시 GL_PROGRAM_POINT_SIZE 고려

        const GLsizei vertexCount = static_cast<GLsizei>(vertices.size());
        glDrawArrays(GL_POINTS, 0, vertexCount);

        glPointSize(prevPointSize);
    }
    else if (type == Shape::Type::Line)
    {
        GLfloat prevLineWidth = 1.0f;
        glGetFloatv(GL_LINE_WIDTH, &prevLineWidth);
        glLineWidth(5.0f);

        const GLsizei indexCount = static_cast<GLsizei>(indices.size());
        if (indexCount > 0)
        {
            glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr);
        }
        else
        {
            const GLsizei vertexCount = static_cast<GLsizei>(vertices.size());
            glDrawArrays(GL_LINES, 0, vertexCount);
        }

        glLineWidth(prevLineWidth);
    }
    else
    {
        const GLsizei indexCount = static_cast<GLsizei>(indices.size());
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }

    glBindVertexArray(0);
}

void Shape::NextTo(const Shape& other_) noexcept
{
    type = static_cast<Shape::Type>((static_cast<unsigned char>(type) + static_cast<unsigned char>(other_.type) + 1) % 5);

    glBindVertexArray(vao);

    const auto& vertices = SHAPES_VERTICES.at(type);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_DYNAMIC_DRAW);

    const auto& indices = SHAPE_INDICES.at(type);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
