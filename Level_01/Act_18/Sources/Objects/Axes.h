#ifndef GUARD_AXES_H
#define GUARD_AXES_H

#include "Object.h"

class Axes final
    : public Object
{
public:
    /**
     * @brief 생성자.
     */
    explicit Axes() noexcept;

    /**
     * @brief 해당 오브젝트를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    virtual void Render(const Shader& shader_) const noexcept override;
};

#endif // !GUARD_AXES_H