#ifndef GUARD_DISK_H
#define GUARD_DISK_H

#include "Model.h"

/**
 * @class Disk
 *
 * @brief 원반 모델을 정의합니다.
 */
class Disk final
    : public Model
{
public:
    /**
     * @brief 생성자.
     *
     * @param innerRadius_ 생성할 원반의 내부 반지름.
     * @param outerRadius_ 생성할 원반의 외부 반지름.
     * @param slices_      생성할 원반의 슬라이스 수.
     * @param loops_       생성할 원반의 루프 수.
     */
    explicit Disk(float innerRadius_,
                  float outerRadius_,
                  int   slices_,
                  int   loops_) noexcept;

    /**
     * @brief 해당 원반을 렌더링합니다.
     */
    virtual void Render() const noexcept override;

private:
    /**
     * @brief 해당 원반의 내부 반지름.
     */
    float innerRadius;

    /**
     * @brief 해당 원반의 외부 반지름.
     */
    float outerRadius;

    /**
     * @brief 해당 원반의 슬라이스 수.
     */
    int slices;

    /**
     * @brief 해당 원반의 루프 수.
     */
    int loops;
};

#endif // !GUARD_DISK_H