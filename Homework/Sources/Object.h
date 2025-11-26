#pragma once

#include "PCH.h"

#include "Transform.h"

#include "AABB.h"

class Mesh;

/**
 * @brief 
 */
class Object
{
public:
    /**
     * @brief 생성자.
     * 
	 * @param transform_ 생성할 오븝젝트의 트랜스폼
	 * @param mesh_      생성할 오브젝트의 메쉬
     */
    explicit Object(const Transform& transform_, 
                    Mesh* const      mesh_) noexcept;

	/**
	 * @brief 소멸자.
	 */
	virtual ~Object() noexcept = default;

    /**
     * @brief 해당 객체를 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격
     */
    void Update(const float deltaTime_) noexcept;

    /**
     * @brief 해당 객체를 그립니다.
     */
    void Render() const noexcept;

    /**
     * @brief 해당 오브젝트의 위치를 반환합니다.
     *
     * @return glm::vec3 해당 오브젝트의 위치
     */
    [[nodiscard]]
    inline constexpr glm::vec3 GetPosition() const noexcept;

    /**
     * @brief 해당 오브젝트의 위치를 설정합니다.
     *
     * @param position_ 설정할 위치
     */
    inline void SetPosition(const glm::vec3& position_) noexcept;

    /**
     * @brief 해당 오브젝트의 회전을 반환합니다.
     *
     * @return glm::vec3 해당 오브젝트의 회전
     */
    [[nodiscard]]
    inline constexpr glm::vec3 GetRotation() const noexcept;

    /**
     * @brief 해당 오브젝트의 회전을 설정합니다.
     *
     * @param rotation_ 설정할 회전
     */
    inline void SetRotation(const glm::vec3& rotation_) noexcept;

    /**
     * @brief 해당 오브젝트의 크기를 반환합니다.
     *
     * @return glm::vec3 해당 오브젝트의 크기
     */
    [[nodiscard]]
    inline constexpr glm::vec3 GetScale() const noexcept;

    /**
     * @brief 해당 오브젝트의 크기를 설정합니다.
     *
     * @param scale_ 설정할 크기
     */
    inline void SetScale(const glm::vec3& scale_) noexcept;

    /**
     * @brief 해당 오브젝트의 부모 트랜스폼을 반환합니다.
     *
     * @return glm::vec3 해당 오브젝트의 부모 트랜스폼
     */
    [[nodiscard]]
    inline constexpr Transform* const GetParent() const noexcept;

    /**
     * @brief 해당 오브젝트의 부모 트랜스폼을 설정합니다.
     *
     * @param scale_ 설정할 부모 트랜스폼
     */
    inline void SetParent(Transform* const parent_) noexcept;

    /**
     * @brief 해당 오브젝트의 충돌 박스를 반환합니다.
     *
     * @return AABB 해당 오브젝트의 충돌 박스
	 */
	[[nodiscard]]
    inline AABB GetAABB() const noexcept
    {
        glm::vec3 halfScale = GetScale() * 0.5f;
        return AABB{ GetPosition() - halfScale, GetPosition() + halfScale };
    }

protected:
    /**
	 * @brief 해당 오브젝트가 업데이트될 때 호출됩니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격
	 */
    virtual void OnUpdate(const float deltaTime_) noexcept;

    /**
     * @brief 해당 오브젝트가 그려질 때 호출됩니다.
     */
    virtual void OnRender() const noexcept;

private:
    /**
     * @brief 해당 오브젝트의 트랜스폼.
	 */
    Transform transform;

    /**
     * @brief 해당 오브젝트의 메쉬.
     */
    Mesh* mesh;
};

inline constexpr glm::vec3 Object::GetPosition() const noexcept
{
    return transform.position;
}

inline void Object::SetPosition(const glm::vec3& position_) noexcept
{
    transform.position = position_;
}

inline constexpr glm::vec3 Object::GetRotation() const noexcept
{
    return transform.rotation;
}

inline void Object::SetRotation(const glm::vec3& rotation_) noexcept
{
    transform.rotation = rotation_;
}

inline constexpr glm::vec3 Object::GetScale() const noexcept
{
    return transform.scale;
}

inline void Object::SetScale(const glm::vec3& scale_) noexcept
{
    transform.scale = scale_;
}

inline constexpr Transform* const Object::GetParent() const noexcept
{
    return transform.parent;
}

inline void Object::SetParent(Transform* const parent_) noexcept
{
    transform.parent = parent_;
}