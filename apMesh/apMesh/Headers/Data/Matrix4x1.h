#ifndef MATRIX4X1_H
#define MATRIX4X1_H

#include "../Numerical/IMatrix.h"
#include "../Numerical/TMatrix.h"

class Matrix4x1 : public Numerical::TMatrix<double, 4,1>
{
public:
    Matrix4x1();
};

#endif // MATRIX4X1_H
