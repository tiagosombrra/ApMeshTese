#include "Performer/PrincipalComponentsBasisFinder.h"

#if USE_NUMERICAL

#include "Data/Boundary.h"
#include "Numerical/ColumnMatrix.h"
#include "Numerical/RowMatrix.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/TransformationMatrix.h"

Performer::PrincipalComponentsBasisFinder::PrincipalComponentsBasisFinder() :
    Performer::Builder::Builder()
{
    this->matrix = NULL;
}

Performer::PrincipalComponentsBasisFinder::~PrincipalComponentsBasisFinder()
{

}

Numerical::BasisChangeMatrix *Performer::PrincipalComponentsBasisFinder::getMatrix() const
{
    return this->matrix;
}

void Performer::PrincipalComponentsBasisFinder::setBoundary(const Data::Boundary *boundary)
{
    this->pca.setBoundary(boundary);
}

const Data::Boundary *Performer::PrincipalComponentsBasisFinder::getBoundary() const
{
    return this->pca.getBoundary();
}

void Performer::PrincipalComponentsBasisFinder::setMode(enum Performer::CovarianceMatrixFinder::Mode mode)
{
    this->pca.setMode(mode);
}

enum Performer::CovarianceMatrixFinder::Mode Performer::PrincipalComponentsBasisFinder::getMode() const
{
    return this->pca.getMode();
}

void Performer::PrincipalComponentsBasisFinder::setFrontElementCentroids(bool frontElementCentroids)
{
    this->pca.setFrontElementCentroids(frontElementCentroids);
}

bool Performer::PrincipalComponentsBasisFinder::isFrontElementCentroids() const
{
    return this->pca.isFrontElementCentroids();
}

void Performer::PrincipalComponentsBasisFinder::setWeighted(bool weighted)
{
    this->pca.setWeighted(weighted);
}

bool Performer::PrincipalComponentsBasisFinder::isWeighted() const
{
    return this->pca.isWeighted();
}

Numerical::RowMatrix *Performer::PrincipalComponentsBasisFinder::getEigenvalues() const
{
    return this->pca.getEigenvalues();
}

Numerical::DenseMatrix *Performer::PrincipalComponentsBasisFinder::getEigenvectors() const
{
    return this->pca.getEigenvectors();
}

Numerical::ColumnMatrix *Performer::PrincipalComponentsBasisFinder::getMid() const
{
    return this->pca.getMid();
}

void Performer::PrincipalComponentsBasisFinder::execute()
{
    this->pca.execute();

    Numerical::RowMatrix *eigenvalues = this->getEigenvalues();
    Numerical::DenseMatrix *eigenvectors = this->getEigenvectors();
    Numerical::ColumnMatrix *mid = this->getMid();

    if ((!mid) || (!eigenvalues) || (!eigenvectors))
    {
        return;
    }

    if (this->getBoundary()->dimension() == 2)
    {
        Data::Real pt[2] = {mid->getElement(0), mid->getElement(1)};
        Data::Real vecs[2][2];

        UInt order[2] = {0, 1};

        if (eigenvalues->getElement(1) > eigenvalues->getElement(0) + Data::getTolerance())
        {
            order[0] = 1;
            order[1] = 0;
        }

        for (UInt i = 0; i < 2; i++)
        {
            UInt k = order[i];

            for (UInt j = 0; j < 2; j++)
            {
                vecs[i][j] = eigenvectors->getElement(j, k);
            }
        }

        this->matrix = new Numerical::BasisChangeMatrix(vecs[0], vecs[1], pt);
    }
    else
    {
        Data::Real pt[3] = {mid->getElement(0), mid->getElement(1), mid->getElement(2)};
        Data::Real vecs[3][3];

        UInt order[3] = {0, 1, 2};

        for (UInt j = 0; j < 3; j++)
        {
            UInt k = (j+1)%3;
            UInt k2 = (j+2)%3;

            if ((eigenvalues->getElement(j) > eigenvalues->getElement(k) + Data::getTolerance()) &&
                (eigenvalues->getElement(j) > eigenvalues->getElement(k2) + Data::getTolerance()))
            {
                order[0] = j;

                if (eigenvalues->getElement(k2) > eigenvalues->getElement(k) + Data::getTolerance())
                {
                    order[1] = k2;
                    order[2] = k;
                }
                else
                {
                    order[1] = k;
                    order[2] = k2;
                }

                break;
            }
        }

        for (UInt i = 0; i < 3; i++)
        {
            UInt k = order[i];

            for (UInt j = 0; j < 3; j++)
            {
                vecs[i][j] = eigenvectors->getElement(j, k);
            }
        }

        this->matrix = new Numerical::BasisChangeMatrix(vecs[0], vecs[1], vecs[2], pt);
    }
}

#endif //#if USE_NUMERICAL
