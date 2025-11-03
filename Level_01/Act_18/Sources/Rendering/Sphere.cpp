#include "Sphere.h"

Sphere::Sphere(const float radius_,
               const int   slices_,
               const int   stacks_) noexcept
    : radius(radius_)
    , slices(slices_)
    , stacks(stacks_)
{

}

void Sphere::Render() const noexcept
{
    const GLUquadric* const model = GetNativeModel();
    if (model == nullptr)
    {
        return;
    }

    gluSphere(const_cast<GLUquadric*>(model), radius, slices, stacks);
}
