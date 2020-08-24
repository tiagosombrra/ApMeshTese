#include "Performer/LaplacianSmoother.h"

#include "Data/Point.h"
#include "Data/Edge.h"
#include "Data/Vertex.h"
#include "Data/Element.h"
#include "Data/Mesh.h"

Performer::LaplacianSmoother::LaplacianSmoother(UInt dimension,
	Data::Mesh *mesh, bool testAdjacency, Real factor, bool sortPointsDisplacement) :
	MeshSmoother(mesh)
{
	this->setDimension(dimension);

	this->setTestAdjacency(testAdjacency);

	this->setFactor(factor);

	this->setSortPointsDisplacement( sortPointsDisplacement );
}

Performer::LaplacianSmoother::~LaplacianSmoother()
{

}

void Performer::LaplacianSmoother::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void Performer::LaplacianSmoother::setTestAdjacency(bool testAdjacency)
{
	this->testAdjacency = testAdjacency;
}

void Performer::LaplacianSmoother::setFactor(Real factor)
{
	this->factor = factor;
}

void Performer::LaplacianSmoother::setSortPointsDisplacement( bool sortPointsDisplacement )
{
	this->sortPointsDisplacement = sortPointsDisplacement;
}

bool Performer::LaplacianSmoother::smooth(Data::Vertex *v) const
{
	Data::PointSet points;
	Data::ElementList elements;

	this->findAdjacentElements(v, elements);
	this->findAdjacency(elements, v, points);


	return this->smooth(v, elements, points);
}

bool Performer::LaplacianSmoother::smooth(Data::Vertex *v, const Data::ElementList &elements, const Data::PointSet &points) const
{
	/*Real den = 0.0;

	Real num[this->dimension], old[this->dimension];

	for (UInt j = 0; j < this->dimension; j++)
	{
		num[j] = 0.0;
		old[j] = v->getPoint()->getCoord(j);
	}

	for (Data::PointSet::iterator iter = points.begin();
		 iter != points.end(); ++iter)
	{
		Data::Point *p = (*iter);

		for (UInt j = 0; j < this->dimension; j++)
		{
			//num[j] += weights[i]*(p->getCoord(j) - old[j]);
			num[j] += 1.0*(p->getCoord(j) - old[j]);
		}

		//den += weights[i];
		den += 1.0;
	}

	den = (den > Data::getTolerance()) ? this->factor/den : 0.0;

	for (UInt j = 0; j < this->dimension; j++)
	{
		v->getPoint()->setCoord(j, old[j] + num[j]*den);
	}*/

	Real disp[this->dimension], old[this->dimension];

	for (UInt j = 0; j < this->dimension; j++)
	{
		old[j] = v->getPoint()->getCoord(j);
	}

	this->findDisplacement(v->getPoint(), old, points, disp);

	for (UInt j = 0; j < this->dimension; j++)
	{
		v->getPoint()->setCoord(j, old[j] + disp[j]);
	}

#if USE_JMSH_DEBUG_MESSAGES
	if (this->dimension == 3)
	{
		std::cout << "markos debug, smoothing node = " << v->getId() - 1 << ", old_coord = ("
			<< old[0] << ", " << old[1] << ", " << old[2] << "), new_coord = (" <<
			v->getPoint()->getCoord(0) << ", " << v->getPoint()->getCoord(1) << ", " <<
			v->getPoint()->getCoord(2) << ")" << std::endl;
	}
#endif //#if USE_JMSH_DEBUG_MESSAGES

	if ((this->testAdjacency) && (!this->isAdjacencyOk(v, elements)))
	{
		v->getPoint()->setCoords(old);

#if USE_JMSH_DEBUG_MESSAGES
		if (this->dimension == 3)
		{
			std::cout << "markos debug, smoothing invalid" << std::endl;
		}
#endif //#if USE_JMSH_DEBUG_MESSAGES

		return false;
	}

	return true;
}

void Performer::LaplacianSmoother::findDisplacement(const Data::Point */*point*/,
	const Real position[], const Data::PointSet &adjacency, Real displacement[]) const
{
	Real den = 0.0;

	for (UInt j = 0; j < this->dimension; j++)
	{
		displacement[j] = 0.0;
	}


    ///ALTERADO POR DANIEL N.T
    if( this->sortPointsDisplacement )
    {
        Data::PointList pts;
        pts.insert(pts.end(), adjacency.begin(), adjacency.end());
        pts.sort(    [&](const Data::Point * first, const Data::Point * second) {
                         if (first->getCoord(0) != second->getCoord(0)) {
                             return first->getCoord(0) < second->getCoord(0);
                         }
                         if (first->getCoord(1) != second->getCoord(1)) {
                             return first->getCoord(1) < second->getCoord(1);
                         }
                         return first->getCoord(2) < second->getCoord(2);
                        }
                );

        for (Data::PointList::const_iterator iter = pts.begin(); iter != pts.end(); ++iter)
        {
            Data::Point *p = (*iter);

            for (UInt j = 0; j < this->dimension; j++)
            {
                displacement[j] += 1.0 *(p->getCoord(j) - position[j]);
            }

            den += 1.0;
        }
    }
    else
    {
        //std::cout << "FALSE 2" << std::endl;
        for (Data::PointSet::const_iterator iter = adjacency.begin(); iter != adjacency.end(); ++iter)
        {
            Data::Point *p = (*iter);

            for (UInt j = 0; j < this->dimension; j++)
            {
                displacement[j] += 1.0 *(p->getCoord(j) - position[j]);
            }

            den += 1.0;
        }
    }



	den = (den > Data::getTolerance()) ? this->factor/den : 0.0;

	for (UInt j = 0; j < this->dimension; j++)
	{
		displacement[j] *= den;
	}
}

void Performer::LaplacianSmoother::findAdjacency(const Data::ElementList &elements,
	const Data::Vertex *v, Data::PointSet &adjacency) const
{
	for (Data::ElementList::const_iterator iter = elements.begin();
		 iter != elements.end(); ++iter)
	{
		Data::Element *e = (*iter);

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			if (e->getVertex(i) != v)
			{
				adjacency.insert(e->getVertex(i)->getPoint());
			}
		}
	}
}

void Performer::LaplacianSmoother::findAdjacency(const Data::Vertex *v, Data::PointSet &adjacency) const
{
	Data::ElementList elements;

	this->findAdjacentElements(v, elements);

	this->findAdjacency(elements, v, adjacency);
}

void Performer::LaplacianSmoother::findAdjacentElements(const Data::Vertex *v, Data::ElementList &elements) const
{
	elements = this->mesh->adjacency(v);
}

bool Performer::LaplacianSmoother::isAdjacencyOk(const Data::Vertex */*v*/, const Data::ElementList &elements) const
{
	for (Data::ElementList::const_iterator iter = elements.begin();
		 iter != elements.end(); ++iter)
	{
		Data::Element *e = (*iter);

		if (e->getShape()->orientedSize() < Data::getTolerance())
		{
			return false;
		}
	}

	return true;
}
