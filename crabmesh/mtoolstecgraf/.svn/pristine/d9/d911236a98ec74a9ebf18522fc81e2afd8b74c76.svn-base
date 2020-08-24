#include "Performer/PrincipalComponentsAnalyzer.h"

#if USE_NUMERICAL

#include "Data/Boundary.h"
#include "Numerical/ColumnMatrix.h"
#include "Numerical/SymmetricMatrix.h"
#include "Numerical/JacobiEigensystemSolver.h"

using namespace Performer;

Performer::PrincipalComponentsAnalyzer::PrincipalComponentsAnalyzer() :
    Performer::Builder::Builder()
{
    this->eigenvalues = NULL;
    this->eigenvectors = NULL;

    this->mid = NULL;
    this->midGot = false;
}

Performer::PrincipalComponentsAnalyzer::~PrincipalComponentsAnalyzer()
{
    if (!this->midGot)
    {
        delete this->mid;
    }
}

void Performer::PrincipalComponentsAnalyzer::setBoundary(const Data::Boundary *boundary)
{
    this->cmf.setBoundary(boundary);
}

const Data::Boundary *Performer::PrincipalComponentsAnalyzer::getBoundary() const
{
    return this->cmf.getBoundary();
}

void Performer::PrincipalComponentsAnalyzer::setMode(enum Performer::CovarianceMatrixFinder::Mode mode)
{
    this->cmf.setMode(mode);
}

enum Performer::CovarianceMatrixFinder::Mode Performer::PrincipalComponentsAnalyzer::getMode() const
{
    return this->cmf.getMode();
}

void Performer::PrincipalComponentsAnalyzer::setFrontElementCentroids(bool frontElementCentroids)
{
    this->cmf.setFrontElementCentroids(frontElementCentroids);
}

bool Performer::PrincipalComponentsAnalyzer::isFrontElementCentroids() const
{
    return this->cmf.isFrontElementCentroids();
}

void Performer::PrincipalComponentsAnalyzer::setWeighted(bool weighted)
{
    this->cmf.setWeighted(weighted);
}

bool Performer::PrincipalComponentsAnalyzer::isWeighted() const
{
    return this->cmf.isWeighted();
}

Numerical::RowMatrix *Performer::PrincipalComponentsAnalyzer::getEigenvalues() const
{
    return this->eigenvalues;
}

Numerical::DenseMatrix *Performer::PrincipalComponentsAnalyzer::getEigenvectors() const
{
    return this->eigenvectors;
}

Numerical::ColumnMatrix *Performer::PrincipalComponentsAnalyzer::getMid() const
{
    this->midGot = true;

    return this->mid;
}

void Performer::PrincipalComponentsAnalyzer::execute()
{
    if (!this->getBoundary())
    {
        return;
    }

    this->cmf.execute();

    Numerical::SymmetricMatrix *matrix = cmf.getMatrix();

    Real mid[3] = {0.0, 0.0, 0.0};

    this->cmf.getMid(mid);

    UInt dim = this->getBoundary()->dimension();

    if ((!this->midGot) && (this->mid))
    {
        delete this->mid;
    }

    this->mid = new Numerical::ColumnMatrix(dim);

    for (UInt i = 0; i < dim; i++)
    {
        this->mid->setElement(i, mid[i]);
    }

    this->midGot = false;

    Numerical::JacobiEigensystemSolver jacobi;

    jacobi.setMatrix(matrix);

    jacobi.execute();

    this->eigenvalues = jacobi.getEigenvalues();

    this->eigenvectors = jacobi.getEigenvectors();

    delete matrix;
}

#endif //#if USE_NUMERICAL
