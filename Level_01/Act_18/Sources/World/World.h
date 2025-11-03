#ifndef GUARD_WORLD_H
#define GUARD_WORLD_H

#include <memory>
#include <vector>

#include "../Rendering/Camera.h"

#include "../Objects/Object.h"

class World final
{
public:
    /**
     * @brief 생성자.
     */
    explicit World() noexcept;

    /**
     * @brief 소멸자.
     */
    ~World();

    /**
     * @brief 월드에 오브젝트를 추가합니다.
     *
     * @param object_ 추가할 오브젝트.
     */
    void AddObject(std::unique_ptr<Object>&& object_) noexcept;

    /**
     * @brief 고정된 시간 간격으로 호출됩니다.
     *
     * @param fixedDeltaTime_ 이전 고정 업데이트와 현재 고정 업데이트 사이의 간격.
     */
    void FixedUpdate(float fixedDeltaTime_) const noexcept;

    /**
     * @brief 매 프레임마다 호출됩니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    void Update(float deltaTime_) const noexcept;

    /**
     * @brief 렌더링 후에 호출됩니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    void LateUpdate(float deltaTime_) const noexcept;

    /**
     * @brief 해당 월드를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    void Render(const Shader& shader_) const noexcept;

private:
    /**
     * @brief 월드 내 카메라.
     */
    std::unique_ptr<Camera> camera;

    /**
     * @brief 월드 내 오브젝트들.
     */
    std::vector<std::unique_ptr<Object>> objects;
};

#endif // !GUARD_WORLD_H