#ifndef GUARD_MESH_H
#define GUARD_MESH_H

#include <gl/freeglut_std.h>

class Model
{
public:
    /**
     * @brief 생성자.
     */
    explicit Model() noexcept;

    /**
     * @brief 소멸자.
     */
    virtual ~Model() noexcept;

    /**
     * @brief 모델을 렌더링합니다.
     */
    virtual void Render() const noexcept = 0;

    /**
     * @brief
     *
     * @return GLUquadricObj*
     */
    [[nodiscard]]
    inline GLUquadricObj* GetNativeModel() const noexcept;

private:
    /**
     * @brief
     */
    GLUquadricObj* nativeModel;
};

inline GLUquadricObj* Model::GetNativeModel() const noexcept
{
    return nativeModel;
}

#endif // !GUARD_MESH_H