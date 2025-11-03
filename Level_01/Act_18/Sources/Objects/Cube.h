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
};

#endif // !GUARD_CUBE_H