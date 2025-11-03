#include "Model.h"

Model::Model() noexcept
    : nativeModel{gluNewQuadric()}
{
    gluQuadricDrawStyle(nativeModel, GLU_FILL);
    gluQuadricNormals(nativeModel, GLU_SMOOTH);
    gluQuadricOrientation(nativeModel, GLU_OUTSIDE);
}

Model::~Model() noexcept
{
    if (nativeModel != nullptr)
    {
        gluDeleteQuadric(nativeModel);
    }
}