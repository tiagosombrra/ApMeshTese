#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "../Numerical/IMatrix.h"
#include "../Numerical/TMatrix.h"

class Matrix4x4 : public Numerical::TMatrix<double, 4,4>
{
public:
    Matrix4x4();
    Matrix4x4 transpose() const;

};

#endif // MATRIX4X4_H
