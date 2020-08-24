#include "Performer/CovarianceMatrixFinder.h"

#if USE_NUMERICAL

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"
#include "Numerical/SymmetricMatrix.h"

using namespace Data;
using namespace Performer;

Performer::CovarianceMatrixFinder::CovarianceMatrixFinder() :
    Performer::Builder::Builder()
{
    this->boundary = NULL;
#if USE_EXACT
    this->mode = this->PSEUDO_EXACT;
#else
    this->mode = this->ACCUMULATE;
#endif //#if USE_EXACT
    this->matrix = NULL;

    this->frontElementCentroids = false;
    this->weighted = false;

    for (UInt i = 0; i < 3; i++)
    {
        this->mid[i] = 0.0;
    }
}

Performer::CovarianceMatrixFinder::~CovarianceMatrixFinder()
{

}

void Performer::CovarianceMatrixFinder::setBoundary(const Data::Boundary *boundary)
{
    this->boundary = boundary;
}

const Data::Boundary *Performer::CovarianceMatrixFinder::getBoundary() const
{
    return this->boundary;
}

void Performer::CovarianceMatrixFinder::setMode(enum Performer::CovarianceMatrixFinder::Mode mode)
{
    this->mode = mode;
}

enum Performer::CovarianceMatrixFinder::Mode Performer::CovarianceMatrixFinder::getMode() const
{
    return this->mode;
}

void Performer::CovarianceMatrixFinder::setFrontElementCentroids(bool frontElementCentroids)
{
    this->frontElementCentroids = frontElementCentroids;
}

bool Performer::CovarianceMatrixFinder::isFrontElementCentroids() const
{
    return this->frontElementCentroids;
}

void Performer::CovarianceMatrixFinder::setWeighted(bool weighted)
{
    this->weighted = weighted;
}

bool Performer::CovarianceMatrixFinder::isWeighted() const
{
    return this->weighted;
}

Numerical::SymmetricMatrix *Performer::CovarianceMatrixFinder::getMatrix() const
{
    return this->matrix;
}

void Performer::CovarianceMatrixFinder::getMid(Real mid[3]) const
{
    for (UInt i = 0; i < 3; i++)
    {
        mid[i] = this->mid[i];
    }
}

Real Performer::CovarianceMatrixFinder::weight(const Data::Vertex *v) const
{
    if (!this->weighted)
    {
        return 1.0;
    }

    Data::FrontElementList adjs = this->boundary->adjacency(v);

    Real sum = 0.0/*, size = static_cast<Real>(adjs.size())*/;

    while (!adjs.empty())
    {
        Data::FrontElement *fel = adjs.front();

        adjs.pop_front();

        sum += this->weight(fel)/static_cast<Real>(fel->numVertices());
    }

    return sum;
}

Real Performer::CovarianceMatrixFinder::weight(const Data::FrontElement *fel) const
{
    return (!this->weighted) ? 1.0 : fel->getShape()->size();
}

void Performer::CovarianceMatrixFinder::centroid(const Data::FrontElement *fel, UInt dimension, Real pt[]) const
{
    Data::GeometricShape *shape = fel->getShape();

    for (UInt i = 0; i < dimension; i++)
    {
        pt[i] = 0.0;
    }

    UInt npts = shape->numPoints();

    for (UInt i = 0; i < npts; i++)
    {
        for (UInt j = 0; j < dimension; j++)
        {
            pt[j] += shape->getPoint(i)->getCoord(j);
        }
    }

    for (UInt i = 0; i < dimension; i++)
    {
        pt[i] /= static_cast<Real>(npts);
    }
}

void Performer::CovarianceMatrixFinder::average(UInt dimension, Real average[], const Real pt[], Real weight, Real n) const
{
    if (this->mode == this->NORMAL)
    {
        for (UInt i = 0; i < dimension; i++)
        {
            average[i] += pt[i]*weight;
        }
    }
    else if (this->mode == this->ACCUMULATE)
    {
        for (UInt i = 0; i < dimension; i++)
        {
            average[i] = (n*average[i] + pt[i]*weight)/(n + 1.0*weight);
        }
    }
}

void Performer::CovarianceMatrixFinder::endAverage(UInt dimension, Real average[], Real size) const
{
    if (this->mode == this->NORMAL)
    {
        for (UInt i = 0; i < dimension; i++)
        {
            average[i] /= size;
        }
    }
    else if (this->mode == this->ACCUMULATE)
    {

    }
}

void Performer::CovarianceMatrixFinder::covariance(UInt dimension, Real average[], const Real pt[], Real weight, Real n) const
{
    Real d[dimension];

    for (UInt i = 0; i < dimension; i++)
    {
        d[i] = pt[i] - average[i];
    }

    if (this->mode == this->NORMAL)
    {
        for (UInt i = 0; i < dimension; i++)
        {
            for (UInt j = i; j < dimension; j++)
            {
                this->matrix->setElement(i, j, this->matrix->getElement(i, j) + d[i]*d[j]*weight);
            }
        }
    }
    else if (this->mode == this->ACCUMULATE)
    {
        for (UInt i = 0; i < dimension; i++)
        {
            for (UInt j = i; j < dimension; j++)
            {
                this->matrix->setElement(i, j,
                    (n*this->matrix->getElement(i, j) + d[i]*d[j]*weight)/(n + 1.0*weight));
            }
        }
    }
}

void Performer::CovarianceMatrixFinder::endCovariance(UInt dimension, Real size) const
{
    if (this->mode == this->NORMAL)
    {
        for (UInt i = 0; i < dimension; i++)
        {
            for (UInt j = i; j < dimension; j++)
            {
                this->matrix->setElement(i, j, this->matrix->getElement(i, j)/size);
            }
        }
    }
    else if (this->mode == this->ACCUMULATE)
    {

    }
}

#if USE_EXACT
Data::EReal Performer::CovarianceMatrixFinder::eweight(const Data::Vertex *v) const
{
    if (!this->weighted)
    {
        return 1.0;
    }

    Data::FrontElementList adjs = this->boundary->adjacency(v);

    Data::EReal sum = 0.0, size = static_cast<Real>(adjs.size());

    while (!adjs.empty())
    {
        Data::FrontElement *fel = adjs.front();

        adjs.pop_front();

        sum += this->eweight(fel)/static_cast<Real>(fel->numVertices());
    }

    return sum;
}

Data::EReal Performer::CovarianceMatrixFinder::eweight(const Data::FrontElement *fel) const
{
    return this->weight(fel);
}

void Performer::CovarianceMatrixFinder::centroid(const Data::FrontElement *fel, UInt dimension, Data::EReal pt[]) const
{
    Data::GeometricShape *shape = fel->getShape();

    for (UInt i = 0; i < dimension; i++)
    {
        pt[i] = 0.0;
    }

    UInt npts = shape->numPoints();

    for (UInt i = 0; i < npts; i++)
    {
        for (UInt j = 0; j < dimension; j++)
        {
            pt[j] += shape->getPoint(i)->getCoord(j);
        }
    }

    for (UInt i = 0; i < dimension; i++)
    {
        //pt[i] /= static_cast<Real>(npts);
        pt[i] *= 1.0/static_cast<Real>(npts);
    }
}

void Performer::CovarianceMatrixFinder::average(UInt dimension, Data::EReal average[], const Real pt[], const Data::EReal &weight) const
{
    Data::EReal pt2[dimension];

    for (UInt i = 0; i < dimension; i++)
    {
        pt2[i] = pt[i];
    }

    this->average(dimension, average, pt2, weight);
}

void Performer::CovarianceMatrixFinder::average(UInt dimension, Data::EReal average[], const Data::EReal pt[], const Data::EReal &weight) const
{
    for (UInt i = 0; i < dimension; i++)
    {
        average[i] += pt[i]*weight;
    }
}

void Performer::CovarianceMatrixFinder::endAverage(UInt dimension, Data::EReal average[], const Data::EReal &size) const
{
    for (UInt i = 0; i < dimension; i++)
    {
        //average[i] /= size;
        average[i] *= 1.0/size;
    }
}

void Performer::CovarianceMatrixFinder::covariance(UInt dimension, Data::EReal matrix[], Data::EReal average[], const Real pt[], const Data::EReal &weight) const
{
    Data::EReal pt2[dimension];

    for (UInt i = 0; i < dimension; i++)
    {
        pt2[i] = pt[i];
    }

    this->covariance(dimension, matrix, average, pt2, weight);
}

void Performer::CovarianceMatrixFinder::covariance(UInt dimension, Data::EReal matrix[], Data::EReal average[], const Data::EReal pt[], const Data::EReal &weight) const
{
    Data::EReal d[dimension];

    for (UInt i = 0; i < dimension; i++)
    {
        d[i] = pt[i] - average[i];
    }

    for (UInt i = 0; i < dimension; i++)
    {
        for (UInt j = i; j < dimension; j++)
        {
            matrix[i*dimension + j] += d[i]*d[j]*weight;

            if (matrix[i*dimension + j].size() > 5)
            {
                Data::EReal r = matrix[i*dimension + j];

                matrix[i*dimension + j] = r;
            }
        }
    }
}

void Performer::CovarianceMatrixFinder::endCovariance(UInt dimension, Data::EReal matrix[], const Data::EReal &size) const
{
    for (UInt i = 0; i < dimension; i++)
    {
        for (UInt j = i; j < dimension; j++)
        {
            //matrix[i*dimension + j] /= size;
            matrix[i*dimension + j] *= 1.0/size;
        }
    }
}
#endif //#if USE_EXACT

Numerical::SymmetricMatrix *Performer::CovarianceMatrixFinder::makeMatrix(UInt dimension) const
{
    Numerical::SymmetricMatrix *matrix = new Numerical::SymmetricMatrix(dimension);

    for (UInt i = 0; i < dimension; i++)
    {
        for (UInt j = i; j < dimension; j++)
        {
            matrix->setElement(i, j, 0.0);
        }
    }

    return matrix;
}

void Performer::CovarianceMatrixFinder::execute()
{
    this->matrix = NULL;

    if (!this->boundary)
    {
        return;
    }

    UInt dimension = this->boundary->dimension();

    Real size = this->frontElementCentroids ?
        static_cast<Real>(this->boundary->size()) :
        static_cast<Real>(this->boundary->verticesSize());

    this->matrix = this->makeMatrix(dimension);

    //std::cout << (this->frontElementCentroids ? "fels\t\t" : "vertices\t");
    //std::cout << (this->weighted ? "weighted\t" : "!weighted\t");
    //std::cout << ((this->mode == NORMAL) ? "NORMAL" : ((this->mode == ACCUMULATE) ? "ACCUMULATE" : "PSEUDO_EXACT"));
    //std::cout << std::endl;

    for (UInt i = 0; i < 3; i++)
    {
        this->mid[i] = 0.0;
    }

#if USE_EXACT
    if (this->mode == PSEUDO_EXACT)
    {
        Data::EReal n = 0.0;
        Data::EReal average[dimension];

        for (UInt i = 0; i < dimension; i++)
        {
            average[i] = 0.0;
        }

        if (this->frontElementCentroids)
        {
            Data::EReal pt[dimension];

            for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
                 iter != end; iter++)
            {
                this->centroid((*iter), dimension, pt);

                Data::EReal weight = this->eweight((*iter));

                this->average(dimension, average, pt, weight);

                //std::cout << "fel " << (*iter)->getId() << ", weight " << weight << ", n " << n << std::endl;
                //std::cout << "avg " << average[0] << ", " << average[1] << std::endl;

                n += weight;
            }
        }
        else
        {
            for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
                 iter != end; iter++)
            {
                Data::Point *p = (*iter)->getPoint();

                Data::EReal weight = this->eweight((*iter));

                this->average(dimension, average, p->getCoordsArray(), weight);

                //std::cout << "v   " << (*iter)->getId() << ", weight " << weight << ", n " << n << std::endl;

                n += weight;
            }
        }

        if (!this->weighted)
        {
            n = size;
        }

        //std::cout << "divide avg by size " << n << std::endl;

        this->endAverage(dimension, average, n);

        //std::cout << "avg " << average[0] << ", " << average[1] << std::endl;

        Data::EReal mat[dimension][dimension];

        for (UInt i = 0; i < dimension; i++)
        {
            for (UInt j = 0; j < dimension; j++)
            {
                mat[i][j] = 0.0;
            }
        }

        if (this->frontElementCentroids)
        {
            Data::EReal pt[dimension];

            for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
                 iter != end; iter++)
            {
                this->centroid((*iter), dimension, pt);

                Data::EReal weight = this->eweight((*iter));

                this->covariance(dimension, &mat[0][0], average, pt, weight);

                /*bool br = false;

                std::cout << "fel " << (*iter)->getId() << ", weight " << weight << ", n " << n << std::endl;
                std::cout << "covariance" << std::endl;
                for (UInt i = 0; i < dimension; i++)
                {
                    for (UInt j = 0; j < dimension; j++)
                    {
                        std::cout << i << " " << j << ":" << mat[i][j] << std::endl;

                        if ((std::isinf(mat[i][j])) || (std::isnan(mat[i][j]))) br = true;
                    }
                }

                if (br) break;*/
            }
        }
        else
        {
            for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
                 iter != end; iter++)
            {
                Data::Point *p = (*iter)->getPoint();

                Data::EReal weight = this->eweight((*iter));

                this->covariance(dimension, &mat[0][0], average, p->getCoordsArray(), weight);
            }
        }

        this->endCovariance(dimension, &mat[0][0], n);

        for (UInt i = 0; i < dimension; i++)
        {
            for (UInt j = i; j < dimension; j++)
            {
                this->matrix->setElement(i, j, static_cast<Real>(mat[i][j]));
            }
        }

        for (UInt i = 0; i < dimension; i++)
        {
            this->mid[i] = average[i];
        }
    }
    else
#endif //#if USE_EXACT
    {
        Real n = 0.0;
        /*Real average[dimension];

        for (UInt i = 0; i < dimension; i++)
        {
            average[i] = 0.0;
        }*/

        if (this->frontElementCentroids)
        {
            Real pt[dimension];

            for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
                 iter != end; iter++)
            {
                this->centroid((*iter), dimension, pt);

                Real weight = this->weight((*iter));

                this->average(dimension, this->mid, pt, weight, n);

                //std::cout << "fel " << (*iter)->getId() << ", weight " << weight << ", n " << n << std::endl;

                n += weight;
            }
        }
        else
        {
            for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
                 iter != end; iter++)
            {
                Data::Point *p = (*iter)->getPoint();

                Real weight = this->weight((*iter));

                this->average(dimension, this->mid, p->getCoordsArray(), weight, n);

                //std::cout << "v   " << (*iter)->getId() << ", weight " << weight << ", n " << n << std::endl;

                n += weight;
            }
        }

        if (this->weighted)
        {
            size = n;
        }

        //std::cout << "divide avg by size " << size << std::endl;

        this->endAverage(dimension, this->mid, size);

        //std::cout << "avg " << this->mid[0] << ", " << this->mid[1] << std::endl;

        n = 0.0;

        if (this->frontElementCentroids)
        {
            Real pt[dimension];

            for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
                 iter != end; iter++)
            {
                this->centroid((*iter), dimension, pt);

                Real weight = this->weight((*iter));

                this->covariance(dimension, this->mid, pt, weight, n);

                n += weight;
            }
        }
        else
        {
            for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
                 iter != end; iter++)
            {
                Data::Point *p = (*iter)->getPoint();

                Real weight = this->weight((*iter));

                this->covariance(dimension, this->mid, p->getCoordsArray(), weight, n);

                n += weight;
            }
        }

        this->endCovariance(dimension, size);
    }

    for (UInt i = 0; i < dimension; i++)
    {
        for (UInt j = i; j < dimension; j++)
        {
            Real cov = this->matrix->getElement(i, j);

            if (std::fabs(cov) <= Data::getTolerance())
            {
                this->matrix->setElement(i, j, 0.0);
            }
        }

        if (std::fabs(this->mid[i]) <= Data::getTolerance())
        {
            this->mid[i] = 0.0;
        }
    }

    /*for (UInt i = 0; i < dimension; i++)
    {
        for (UInt j = 0; j < dimension; j++)
        {
            std::cout << this->matrix->getElement(i, j);

            if (j + 1 < this->matrix->numCols())
            {
                std::cout << "\t";
            }
        }

        std::cout << std::endl;
    }*/
}

#endif //#if USE_NUMERICAL
