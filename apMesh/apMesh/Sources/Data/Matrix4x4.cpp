#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{

}

Matrix4x4 Matrix4x4::transpose() const
{
    Matrix4x4 transpose;
    for(int i = 0; i < this->numRows(); i++)
        for(int j = 0; j < this->numCols(); j++)
            transpose.setElement( i, j, this->getElement(j, i) );

    return transpose;

}
