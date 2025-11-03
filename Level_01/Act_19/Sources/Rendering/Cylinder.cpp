#include "Cylinder.h"

Cylinder::Cylinder(const float baseRadius_,
                   const float topRadius_,
                   const float height_,
                   const int   slices_,
                   const int   stacks_) noexcept
    : Model()
    , baseRadius(baseRadius_)
    , topRadius(topRadius_)
    , height(height_)
    , slices(slices_)
    , stacks(stacks_)
{
}

void Cylinder::Render() const noexcept
{
    GLUquadric* const model = GetNativeModel();
    if (model == nullptr)
    {
        return;
    }

    gluCylinder(model, baseRadius, topRadius, height, slices, stacks);
}