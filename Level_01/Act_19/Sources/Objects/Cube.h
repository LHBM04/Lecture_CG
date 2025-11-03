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
     * @brief 매 프레임마다 호출됩니다.
     */
    virtual void Update() noexcept override;

    /**
     * @brief 렌더링 시 호출됩니다.
     */
    virtual void Render(const Shader& shader_) const noexcept override;
};

#endif // !GUARD_CUBE_H