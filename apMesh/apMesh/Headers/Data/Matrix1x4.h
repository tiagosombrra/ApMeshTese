#ifndef MATRIX1X4_H
#define MATRIX1X4_H

#include "../Numerical/IMatrix.h"
#include "../Numerical/TMatrix.h"

class Matrix1x4 : public Numerical::TMatrix<double, 1,4>
{
public:
    Matrix1x4();
};

#endif // MATRIX1X4_H
