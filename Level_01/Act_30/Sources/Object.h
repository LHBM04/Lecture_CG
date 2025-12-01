#pragma once

#include "PCH.h"

#include "Shader.h"

class Mesh;
class Texture;

/**
 * @brief 
 */
class Object
{
public:
	explicit Object(Mesh* mesh_, Texture* texture_) noexcept;

	/**
	 * @brief 해당 객체를 업데이트합니다.
     * 
	 * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격
	 */
	virtual void Update(const float deltaTime_) noexcept;

    /**
     * @brief 해당 객체를 렌더링합니다.
     * 
     * @param shader_ 사용할 셰이더
     */
    virtual void Render(const Shader& shader_) const noexcept;

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
     * @brief 해당 오브젝트의 크기를 반환합니다.
     *
     * @return glm::vec3 해당 오브젝트의 크기
     */
    [[nodiscard]]
    inline constexpr Object* const GetParent() const noexcept;

    /**
     * @brief 해당 오브젝트의 크기를 설정합니다.
     *
     * @param scale_ 설정할 크기
     */
    inline void SetParent(Object* const parent_) noexcept;

    /**
     * @brief 해당 오브젝트의 모델 행렬을 반환합니다.
     * 
	 * @return glm::mat4 해당 오브젝트의 모델 행렬
     */
    [[nodiscard]]
	inline glm::mat4 GetModelMatrix() const noexcept;

private:
	/**
	 * @brief 해당 객체의 위치.
	 */
	glm::vec3 position = glm::vec3(0.0f);

	/**
	 * @brief 해당 객체의 회전.
	 */
    glm::vec3 rotation = glm::vec3(0.0f);

	/**
	 * @brief 해당 객체의 크기.
	 */
	glm::vec3 scale = glm::vec3(1.0f);

	/**
	 * @brief 해당 객체의 상위 객체.
	 */
	Object* parent = nullptr;

	/**
	 * @brief 해당 객체의 메쉬.
	 */
	Mesh* mesh = nullptr;

	/**
	 * @brief 해당 객체의 텍스처.
	 */
	Texture* texture = nullptr;
};

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

inline constexpr Object* const Object::GetParent() const noexcept
{
    return parent;
}

inline void Object::SetParent(Object* const parent_) noexcept
{
    parent = parent_;
}

inline glm::mat4 Object::GetModelMatrix() const noexcept
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    if (parent != nullptr)
    {
        model = parent->GetModelMatrix() * model;
    }

    return model;
}