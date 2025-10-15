#include "Shape.h"
#include "Shape.h"

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Application.h"
#include "glm/gtc/type_ptr.hpp"

Shape::Shape(const Shape::Type type_,
             const glm::vec2   position_) noexcept
    : type(type_)
    , position(position_)
    , scale(100.0f)
    , direction({0.0f, 0.0f})
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const std::array<float, 10>& vertices = SHAPES_VERTICES.at(type_);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_DYNAMIC_DRAW);

    const std::vector<unsigned int>& indices = SHAPE_INDICES.at(type_);

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
    if (direction.x == 0.0f && direction.y == 0.0f)
    {
        return;
    }

    constexpr float moveSpeed = 30.0f;
    position += direction * (deltaTime_ * moveSpeed);

    const float halfWidth  = 0.5f * scale;
    const float halfHeight = 0.5f * scale;

    constexpr float halfWorldWidth  = WINDOW_WIDTH  * 0.5f;
    constexpr float halfWorldHeight = WINDOW_HEIGHT * 0.5f;

    if (position.x - halfWidth < -halfWorldWidth ||
        position.x + halfWidth >  halfWorldWidth)
    {
        direction.x = -direction.x;

        const float max =  halfWorldWidth - halfWidth;
        const float min = -halfWorldWidth + halfWidth;
        position.x = glm::clamp(position.x, min, max);
    }

    if (position.y - halfHeight < -halfWorldHeight ||
        position.y + halfHeight >  halfWorldHeight)
    {
        direction.y = -direction.y;

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

    float angle = 0.0f;
    if (direction.x != 0.0f || direction.y != 0.0f)
    {
        angle = std::atan2(direction.y, direction.x) + glm::radians(270.0f);
    }

    glm::mat4 model = {1.0f};
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, scale, 1.0f));

    glm::mat4 projection = glm::ortho(
       -static_cast<float>(WINDOW_WIDTH)  / 2.0f, // 왼쪽
        static_cast<float>(WINDOW_WIDTH)  / 2.0f, // 오른쪽
       -static_cast<float>(WINDOW_HEIGHT) / 2.0f, // 아래
        static_cast<float>(WINDOW_HEIGHT) / 2.0f, // 위
       -1.0f,                                     // near
        1.0f                                      // far
   );

    const GLint projLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    const GLint modelLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    const GLint colorLoc = glGetUniformLocation(shader_.GetProgramID(), "u_Color");
    glUniform3fv(colorLoc, 1, &color[0]);

    glBindVertexArray(vao);

    if (type == Shape::Type::Dot)
    {
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices.size()));
        glPointSize(1);
    }
    else if (type == Shape::Type::Line)
    {
        const GLsizei indexCount = static_cast<GLsizei>(indices.size());
        if (indexCount > 0)
        {
            glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr);
        }
        else
        {
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.size()));
        }

        glLineWidth(1);
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
