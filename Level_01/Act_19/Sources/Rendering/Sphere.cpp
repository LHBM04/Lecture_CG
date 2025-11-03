#include "Sphere.h"

Sphere::Sphere(const float radius_,
               const int   slices_,
               const int   stacks_) noexcept
    : Model()
    , radius(radius_)
    , slices(slices_)
    , stacks(stacks_)
{

}

void Sphere::Render() const noexcept
{
    GLUquadric* const model = GetNativeModel();
    if (model == nullptr)
    {
        return;
    }

    gluSphere(model, radius, slices, stacks);
}