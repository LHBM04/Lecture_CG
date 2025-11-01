#ifndef GUARD_OBJECT_H
#define GUARD_OBJECT_H

#include "../Graphics/Shader.h"

#include <gl/freeglut_std.h>

#include <glm/vec3.hpp>

class Object
{
public:
    /**
     * @brief 생성자.
     */
    explicit Object() noexcept;

    /**
     * @brief 소멸자.
     */
    ~Object() noexcept;

    /**
     * @brief 해당 객체를 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    void Update(const float deltaTime_) noexcept;

    /**
     * @brief 해당 객체를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    void Render(const Shader& shader_) const noexcept;

private:
    /**
     * @brief 해당 객체의 쿼드릭 객체.
     */
    GLUquadricObj* model;

    /**
     * @brief 해당 객체의 위치.
     */
    glm::vec3 position = {0.0f, 0.0f, 0.0f};

    /**
     * @brief 해당 객체의 회전.
     */
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};

    /**
     * @brief 해당 객체의 크기.
     */
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    /**
     * @brief 해당 객체의 부모 객체.
     */
    Object* parent = nullptr;

    /**
     * @brief 해당 객체의 자식 객체들.
     */
    Object* children = nullptr;
};

#endif // !GUARD_OBJECT_H