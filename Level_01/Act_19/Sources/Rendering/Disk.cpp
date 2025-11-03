#include "Disk.h"

Disk::Disk(const float innerRadius_,
           const float outerRadius_,
           const int   slices_,
           const int   loops_) noexcept
    : Model()
    , innerRadius(innerRadius_)
    , outerRadius(outerRadius_)
    , slices(slices_)
    , loops(loops_)
{
}

void Disk::Render() const noexcept
{
    GLUquadric* const model = GetNativeModel();
    if (model == nullptr)
    {
        return;
    }

    gluDisk(model, innerRadius, outerRadius, slices, loops);
}