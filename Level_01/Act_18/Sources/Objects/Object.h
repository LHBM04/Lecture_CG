#ifndef GUARD_OBJECTS_H
#define GUARD_OBJECTS_H

#include <vector>

#include <gl/glew.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../Graphics/Shader.h"

/**
 * @class Object
 *
 * @brief 오브젝트를 정의합니다.
 */
class Object
{
public:
    /**
     * @brief 생성자.
     *
     * @param vertices_ 정점 배열.
     */
    explicit Object(const std::vector<GLfloat>& vertices_) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Object() noexcept;

    /**
     * @brief 해당 오브젝트를 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(float deltaTime_) noexcept = 0;

    /**
     * @brief 해당 오브젝트를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    virtual void Render(const Shader& shader_) const noexcept = 0;

    /**
     * @brief 정점 개수.
     */
    GLsizei vertexCount = 0;

    /**
     * @brief 요소 개수.
     */
    GLsizei elementCount = 0;

    /**
     * @brief 위치.
     */
    glm::vec3 position = {0.0f, 0.0f, 0.0f};

    /**
     * @brief 회전.
     */
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};

    /**
     * @brief 크기.
     */
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

protected:
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
};

#endif // !GUARD_OBJECTS_H