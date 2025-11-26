#include "Mesh.h"

Mesh::Mesh(const std::vector<Mesh::Vertex>& vertices_,
           const std::vector<GLuint>&       indices_) noexcept
    : vao(0)
    , vbo(0)
    , ebo(0)
    , vertices(vertices_)
    , indices(indices_)
    , isInitialized(true)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(Vertex) * vertices.size(),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * indices.size(),
        indices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
        reinterpret_cast<void*>(offsetof(Mesh::Vertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
        reinterpret_cast<void*>(offsetof(Mesh::Vertex, color)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
        reinterpret_cast<void*>(offsetof(Mesh::Vertex, normal)));

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    if (isInitialized)
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
}

void Mesh::Render(const GLenum renderMode_) const noexcept
{
    if (!isInitialized)
    {
        return;
    }

    glBindVertexArray(vao);
    glDrawElements(renderMode_, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}