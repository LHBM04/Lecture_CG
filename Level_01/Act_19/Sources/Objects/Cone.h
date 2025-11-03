#ifndef GUARD_CYCLINDER_H
#define GUARD_CYCLINDER_H

#include "Object.h"

class Cone final
    : public Object
{
public:
    /**
     * @brief 생성자.
     */
    explicit Cone() noexcept;

    /**
     * @brief 매 프레임마다 호출됩니다.
     */
    virtual void Update() noexcept override;

    /**
     * @brief 해당 오브젝트를 렌더링합니다.
     *
     * @param shader_ 사용할 셰이더.
     */
    virtual void Render(const Shader& shader_) const noexcept override;
};

#endif // !GUARD_CYCLINDER_H