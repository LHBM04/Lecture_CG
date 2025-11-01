#ifndef GUARD_MESH_H
#define GUARD_MESH_H

#include <vector>

#include <gl/glew.h>

class Mesh final
{
public:
    /**
     * @brief 생성자.
     */
    explicit Mesh(const std::vector<GLfloat>& vertices_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Mesh() noexcept;

    /**
     * @brief 메쉬를 바인딩합니다.
     */
    inline void Bind() const noexcept;

    /**
     * @brief 메쉬 바인딩을 해제합니다.
     */
    inline void Unbind() const noexcept;

    /**
     * @brief 메쉬를 렌더링합니다.
     */
    void Render() const noexcept;

private:
    /**
     * @brief 정점 배열 객체.
     */
    GLuint vao = 0;

    /**
     * @brief 정점 버퍼 객체.
     */
    GLuint vbo = 0;

    /**
     * @brief 요소 배열 객체.
     */
    GLuint ebo = 0;

    /**
     * @brief 정점 개수.
     */
    GLsizei vertexCount = 0;

    /**
     * @brief 요소 개수.
     */
    GLsizei elementCount = 0;
};

inline void Mesh::Bind() const noexcept
{
    glBindVertexArray(vao);
}

inline void Mesh::Unbind() const noexcept
{
    glBindVertexArray(0);
}

#endif // !GUARD_MESH_H