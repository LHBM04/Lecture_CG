#ifndef GUARD_MESH_H
#define GUARD_MESH_H

#include <glad/glad.h>

/**
 * @struct Mesh
 *
 * @brief 메시를 정의합니다.
 */
class Mesh final
{
public:
    /**
     * @brief 생성자.
     *
     * @param source_ 메시 소스.
     */
    explicit Mesh(const char* source_) noexcept;

    /**
     * @brief 소멸자.
     */
    ~Mesh() noexcept;

    /**
     * @brief 해당 메시를 그립니다.
     */
    void Render() const noexcept;
private:
    /**
     * @brief 정점 데이터 접두사
     */
    static constexpr char vertexPrefix = 'v';

    /**
     * @brief 면 데이터 접두사.
     */
    static constexpr char facePrefix = 'f';

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
     * @brief 요소 개수.
     */
    GLsizei indexCount = 0;
};

#endif // !GUARD_MESH_H