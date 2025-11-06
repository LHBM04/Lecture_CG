#ifndef GUARD_OBJECTS_H
#define GUARD_OBJECTS_H

#include <memory>

#include <glm/vec3.hpp>

#include "../Rendering/Shader.h"
#include "../Rendering/Mesh.h"

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
     * @param position_ 생성할 오브젝트의 위치
     * @param rotation_ 생성할 오브젝트의 회전
     * @param scale_    생성할 오브젝트의 크기
     */
    explicit Object(const glm::vec3& position_ = glm::vec3{0.0f, 0.0f, 0.0f},
                    const glm::vec3& rotation_ = glm::vec3{0.0f, 0.0f, 0.0f},
                    const glm::vec3& scale_    = glm::vec3{1.0f, 1.0f, 1.0f}) noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Object() noexcept;

    /**
     * @brief 해당 오브젝트를 업데이트합니다.
     */
    virtual void Update() noexcept;

    /**
     * @brief 해당 오브젝트를 렌더링합니다.
     *
     * @param shader_     사용할 셰이더
     * @param renderMode_ 렌더링 모드
     */
    virtual void Render(const Shader& shader_, const GLenum renderMode_) const noexcept;

    /**
     * @brief 해당 오브젝트의 매쉬를 반환합니다.
     *
     * @return Mesh* 해당 오브젝트의 매쉬
     */
    [[nodiscard]]
    inline Mesh* GetMesh() const noexcept;

    /**
     * @brief 해당 오브젝트의 매쉬를 설정합니다.
     *
     * @param mesh_ 설정할 매쉬.
     */
    inline void SetMesh(Mesh* mesh_) noexcept;

    /**
     * @brief 해당 오브젝트의 위치를 반환합니다.
     *
     * @return glm::vec3 해당 오브젝트의 위치.
     */
    [[nodiscard]]
    inline constexpr glm::vec3 GetPosition() const noexcept;

    /**
     * @brief 해당 오브젝트의 위치를 설정합니다.
     *
     * @param position_ 설정할 위치.
     */
    inline void SetPosition(const glm::vec3& position_) noexcept;

    /**
     * @brief 해당 오브젝트의 회전을 반환합니다.
     *
     * @return glm::vec3 해당 오브젝트의 회전.
     */
    [[nodiscard]]
    inline constexpr glm::vec3 GetRotation() const noexcept;

    /**
     * @brief 해당 오브젝트의 회전을 설정합니다.
     *
     * @param rotation_ 설정할 회전.
     */
    inline void SetRotation(const glm::vec3& rotation_) noexcept;

    /**
     * @brief 해당 오브젝트의 크기를 반환합니다.
     *
     * @return glm::vec3 해당 오브젝트의 크기.
     */
    [[nodiscard]]
    inline constexpr glm::vec3 GetScale() const noexcept;

    /**
     * @brief 해당 오브젝트의 크기를 설정합니다.
     *
     * @param scale_ 설정할 크기.
     */
    inline void SetScale(const glm::vec3& scale_) noexcept;

    /**
     * @brief 해당 오브젝트의 색상을 반환합니다.
     *
     * @return glm::vec3 해당 오브젝트의 색상.
     */
    [[nodiscard]]
    inline constexpr glm::vec3 GetColor() const noexcept;

    /**
     * @brief 해당 오브젝트의 색상을 설정합니다.
     *
     * @param color_ 설정할 색상.
     */
    inline void SetColor(const glm::vec3& color_) noexcept;

private:
    /**
     * @brief 해당 오브젝트의 매쉬.
     */
    std::unique_ptr<Mesh> mesh;

    /**
     * @brief 해당 오브젝트의 위치.
     */
    glm::vec3 position;

    /**
     * @brief 해당 오브젝트의 회전.
     */
    glm::vec3 rotation;

    /**
     * @brief 해당 오브젝트의 크기.
     */
    glm::vec3 scale;

    /**
     * @brief 해당 오브젝트의 색상.
     */
    glm::vec3 color;
};

inline Mesh* Object::GetMesh() const noexcept
{
    return mesh.get();
}

inline void Object::SetMesh(Mesh* const mesh_) noexcept
{
    mesh.reset(mesh_);
}

inline constexpr glm::vec3 Object::GetPosition() const noexcept
{
    return position;
}

inline void Object::SetPosition(const glm::vec3& position_) noexcept
{
    position = position_;
}

inline constexpr glm::vec3 Object::GetRotation() const noexcept
{
    return rotation;
}

inline void Object::SetRotation(const glm::vec3& rotation_) noexcept
{
    rotation = rotation_;
}

inline constexpr glm::vec3 Object::GetScale() const noexcept
{
    return scale;
}

inline void Object::SetScale(const glm::vec3& scale_) noexcept
{
    scale = scale_;
}

inline constexpr glm::vec3 Object::GetColor() const noexcept
{
    return color;
}

inline void Object::SetColor(const glm::vec3& color_) noexcept
{
    color = color_;
}

#endif // !GUARD_OBJECTS_H