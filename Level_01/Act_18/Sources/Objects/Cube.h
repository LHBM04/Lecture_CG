#ifndef GUARD_CUBE_H
#define GUARD_CUBE_H

#include "Object.h"

class Cube final
    : public Object
{
public:
    /**
    * @brief 생성자.
    */
    explicit Cube() noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Cube() noexcept override;

    /**
     * @brief 해당 큐브를 업데이트합니다.
     *
     * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격.
     */
    virtual void Update(float deltaTime_) noexcept override;

    /**
     * @brief 해당 큐브를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    virtual void Render(const Shader& shader_) const noexcept override;
};

#endif // !GUARD_CUBE_H