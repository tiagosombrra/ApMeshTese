#include "Numerical/EigenvectorFinder.h"

#if USE_NUMERICAL

#include "Numerical/ColumnMatrix.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/GaussianEliminationLinearSystemSolver.h"
#include "Numerical/GaussSeidelLinearSystemSolver.h"

using namespace Data;
using namespace Numerical;

Numerical::EigenvectorFinder::EigenvectorFinder() :
    Performer::Builder::Builder()
{
    this->matrix = NULL;
    this->eigenvalue = 0.0;
    this->eigenvector = NULL;
}

Numerical::EigenvectorFinder::~EigenvectorFinder()
{

}

void Numerical::EigenvectorFinder::setMatrix(const IMatrix *matrix)
{
    this->matrix = matrix;
}

const IMatrix *Numerical::EigenvectorFinder::getMatrix() const
{
    return this->matrix;
}

void Numerical::EigenvectorFinder::setEigenvalue(Real eigenvalue)
{
    this->eigenvalue = eigenvalue;
}

Real Numerical::EigenvectorFinder::getEigenvalue() const
{
    return this->eigenvalue;
}

IColumnMatrix *Numerical::EigenvectorFinder::getEigenvector() const
{
    return this->eigenvector;
}

void Numerical::EigenvectorFinder::execute()
{
    if ((!this->matrix) || (!this->matrix->isSquare()))
    {
        return;
    }

    /*IMatrix *m = this->matrix->clone();

    for (UInt i = 0; i < m->numCols(); i++)
    {
        m->setElement(i, i, m->getElement(i, i) - this->eigenvalue);
    }*/

    DenseMatrix m(this->matrix->numRows(), this->matrix->numCols());

    for (UInt i = 0; i < m.numRows(); i++)
    {
        for (UInt j = 0; j < m.numCols(); j++)
        {
            m.setElement(i, j, this->matrix->getElement(i, j) - ((i == j) ? this->eigenvalue : 0.0));
        }
    }

    ColumnMatrix cts(m.numCols());
    //ColumnMatrix guess(m->numCols());

    for (UInt i = 0; i < cts.numRows(); i++)
    {
        cts.setElement(i, 0.0);
        //guess.setElement(i, 1.0);
    }

    GaussianEliminationLinearSystemSolver solver;

    solver.setMatrix(&m);
    solver.setConstants(&cts);
    //solver.setTolerance(solver.getTolerance()*0.1);
    solver.setPivoting(LinearSystemSolver::PARTIAL);
    solver.setCopy(false);
    solver.setSkip(true);

    solver.execute();

    /*GaussSeidelLinearSystemSolver solver;
    solver.setMatrix(m);
    solver.setConstants(&cts);
    solver.setGuess(&guess);

    solver.execute();*/

    this->eigenvector = solver.getUnknowns();

    Real size = 0.0;

    for (UInt i = 0; i < this->eigenvector->numRows(); i++)
    {
        Real v = this->eigenvector->getElement(i);

        size += v*v;
    }

    size = std::sqrt(size);

    for (UInt i = 0; i < this->eigenvector->numRows(); i++)
    {
        this->eigenvector->setElement(i, this->eigenvector->getElement(i)/size);
    }

    //delete m;
}

#endif //#if USE_NUMERICAL
