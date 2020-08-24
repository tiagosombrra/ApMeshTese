#include "Tools/BoundaryLaplacianSmoother.h"

#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Triangle3D.h"

using namespace Tools;

Tools::BoundaryLaplacianSmoother::BoundaryLaplacianSmoother(UInt dimension,
	Data::Boundary *boundary, Data::Mesh *mesh, bool testAdjacency, Real factor, bool sortPointsDisplacement) :
		MJMesh::LaplacianSmoother::LaplacianSmoother(dimension, mesh, testAdjacency, factor, sortPointsDisplacement),
		Tools::Tool::Tool()
{
	this->boundary = boundary;

	this->tolerance = -1.0;

	this->smoothed = false;
	this->normalTestEnabled = true;
}

Tools::BoundaryLaplacianSmoother::~BoundaryLaplacianSmoother()
{

}

void Tools::BoundaryLaplacianSmoother::setTolerance(Real tolerance)
{
	this->tolerance = tolerance;
}

void Tools::BoundaryLaplacianSmoother::setNormalTestEnabled(bool normalTestEnabled)
{
    this->normalTestEnabled = normalTestEnabled;
}

bool Tools::BoundaryLaplacianSmoother::isSmoothed() const
{
	return this->smoothed;
}

void Tools::BoundaryLaplacianSmoother::execute()
{
	if ((!this->boundary) && (!this->mesh))
	{
		this->status = Performer::MeshSmoother::ended;

		return;
	}

	this->smoothed = false;

	Real tolerance = Data::getTolerance();

	if (this->tolerance >= 0.0)
	{
		Data::setTolerance(this->tolerance);
	}

	this->setMeasure(0,
        static_cast<UInt>(this->boundary ? this->boundary->verticesSize() : this->mesh->verticesSize()) + 1);

	if (this->boundary)
    {
        for (Data::Boundary::VertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
             iter != end; iter++)
        {
            this->smooth((*iter));
        }
    }
    else if (this->mesh->isSurface())
    {
        for (Data::Mesh::VertexIterator iter = this->mesh->vBegin(), end = this->mesh->vEnd();
             iter != end; iter++)
        {
            Data::Vertex *v = (*iter);

            bool inBoundary = false;

            Data::FrontElementList fadjs = this->mesh->findAdjacentFrontElements(v);

            if (fadjs.empty())
            {
                Data::ElementList adjs = this->mesh->adjacency(v);

                if (adjs.empty())
                {
                    continue;
                }

                Data::Element *adj = adjs.front();

                for (UInt k = 0; k < adjs.size() + 2; k++)
                {
                    UInt i = adj->index(v);

                    Data::Vertex *n = adj->getVertex((i+1)%adj->numVertices());

                    Data::Element *nadj = NULL;

                    for (Data::ElementList::iterator iter2 = adjs.begin();
                         iter2 != adjs.end(); iter2++)
                    {
                        if ((*iter2) == adj)
                        {
                            continue;
                        }

                        if ((*iter2)->have(n))
                        {
                            nadj = (*iter2);

                            break;
                        }
                    }

                    if (!nadj)
                    {
                        inBoundary = true;

                        break;
                    }

                    adj = nadj;
                }
            }
            else
            {
                inBoundary = true;
            }

            if (!inBoundary)
            {
                this->smooth(v);
            }
        }
    }

#if USE_ATTRIBS
	this->clearAttribs();
#endif //#if USE_ATTRIBS

	this->endMeasure();

	Data::setTolerance(tolerance);

	this->status = Performer::MeshSmoother::ended;
}

bool Tools::BoundaryLaplacianSmoother::smooth(Data::Vertex *v, const Data::ElementList &elements, const Data::PointSet &points) const
{
	this->addMeasure(1, 0);

	if (elements.size() <= 1)
	{
		return false;
	}

	if (this->dimension == 2)
	{
		Data::Vector2D v = static_cast<Data::Edge2D *>(elements.front()->getShape())->normal();

#if USE_EXPERIMENTAL_3
		this->normal2D = v;
#else
		this->normal = v;
#endif //#if USE_EXPERIMENTAL_3

		this->worst = this->quality(elements.front());

		for (Data::ElementList::const_iterator iter = ++elements.begin();
			 iter != elements.end(); iter++)
		{
			Data::Vector2D n = static_cast<Data::Edge2D *>((*iter)->getShape())->normal();

			if (n.dot(v) <= 1.0 - Data::getTolerance())
			{
				return false;
			}

			Real q = this->quality((*iter));

			if (this->worse(q, this->worst))
			{
				this->worst = q;
			}
		}
	}
	else if (this->dimension == 3)
	{
		Data::Vector3D v = static_cast<Data::Polygon *>(elements.front()->getShape())->normal();

		//v.normalize();

#if USE_EXPERIMENTAL_3
		this->normal3D = v;
#else
		this->normal = v;
#endif //#if USE_EXPERIMENTAL_3

		this->worst = this->quality(elements.front());

		for (Data::ElementList::const_iterator iter = ++elements.begin();
			 iter != elements.end(); iter++)
		{
			if (this->normalTestEnabled)
            {
                Data::Vector3D n = static_cast<Data::Polygon *>((*iter)->getShape())->normal();

                //n.normalize();

                if (n.dot(v) <= 1.0 - Data::getTolerance())
                {
                    return false;
                }
            }

			Real q = this->quality((*iter));

			if (this->worse(q, this->worst))
			{
				this->worst = q;
			}
		}
	}
	else
	{
		return false;
	}

	bool smoothed = MJMesh::LaplacianSmoother::smooth(v, elements, points);

	if (smoothed)
	{
		this->smoothed = true;
	}

	return smoothed;
}

void Tools::BoundaryLaplacianSmoother::findDisplacement(const Data::Point *point,
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

            Real d = p->distance(point);

            for (UInt j = 0; j < this->dimension; j++)
            {
                displacement[j] += d*(p->getCoord(j) - position[j]);
            }

            den += d;
        }
	}
    else
    {
        //std::cout << "FALSE 1" << std::endl;
        for (Data::PointSet::const_iterator iter = adjacency.begin(); iter != adjacency.end(); ++iter)
        {
            Data::Point *p = (*iter);

            Real d = p->distance(point);

            for (UInt j = 0; j < this->dimension; j++)
            {
                displacement[j] += d*(p->getCoord(j) - position[j]);
            }

            den += d;
        }
    }



	den = (den > Data::getTolerance()) ? this->factor/den : 0.0;

	for (UInt j = 0; j < this->dimension; j++)
	{
		displacement[j] *= den;
	}
}

void Tools::BoundaryLaplacianSmoother::findAdjacentElements(const Data::Vertex *v, Data::ElementList &elements) const
{
    if (this->boundary)
    {
        Data::FrontElementList fels = this->boundary->adjacency(v);

        elements.clear();

        for (Data::FrontElementList::iterator iter = fels.begin();
             iter != fels.end(); iter++)
        {
            elements.push_back((*iter));
        }
    }
    else
    {
        elements = this->mesh->adjacency(v);
    }
}

Real Tools::BoundaryLaplacianSmoother::quality(const Data::Element *e) const
{
	if (this->dimension == 2)
	{
		return e->getVertex(0)->getPoint()->distance(e->getVertex(1)->getPoint());
	}

	Real a = e->getVertex(0)->getPoint()->distance(e->getVertex(1)->getPoint());
	Real b = e->getVertex(1)->getPoint()->distance(e->getVertex(2)->getPoint());
	Real c = e->getVertex(2)->getPoint()->distance(e->getVertex(0)->getPoint());

	Real s = 0.5*(a + b + c);
	Real d = (s - a)*(s - b)*(s - c);

	Real rInsc = std::sqrt(d/s);
	Real rCirc = 0.25*a*b*c/sqrt(s*d);

	return 2.0*rInsc/rCirc;
}

bool Tools::BoundaryLaplacianSmoother::worse(Real q1, Real q2) const
{
	return (q1 < q2);
}

bool Tools::BoundaryLaplacianSmoother::isAdjacencyOk(const Data::Vertex */*v*/, const Data::ElementList &elements) const
{
	for (Data::ElementList::const_iterator iter = elements.begin();
		 iter != elements.end(); ++iter)
	{
		if (this->dimension == 2)
		{
#if USE_POOL
			static_cast<MJMesh::Edge2D *>((*iter)->getShape())->recalcAttribs();
#else
			static_cast<MJMesh::Edge2D *>((*iter)->getShape())->clearAttribs();
#endif //#if USE_POOL
		}
		else if (this->boundary)
		{
#if USE_POOL
			static_cast<MJMesh::Triangle3D *>((*iter)->getShape())->recalcAttribs();
#else
			static_cast<MJMesh::Triangle3D *>((*iter)->getShape())->clearAttribs();
#endif //#if USE_POOL
		}

#if USE_EXPERIMENTAL_3
        if (this->dimension == 2)
		{
			Data::Vector2D v = static_cast<Data::Edge2D *>((*iter)->getShape())->normal();

			if ((v.dot(this->normal2D) <= 1.0 - Data::getTolerance()) ||
                (this->worse(this->quality((*iter)), this->worst)))
            {
                return false;
            }
		}
		else
		{
			Data::Vector3D v = static_cast<Data::Polygon *>((*iter)->getShape())->normal();

			if ((v.dot(this->normal3D) <= 1.0 - Data::getTolerance()) ||
                (this->worse(this->quality((*iter)), this->worst)))
            {
                return false;
            }
		}
#else
		Data::Vector v;

		if (this->dimension == 2)
		{
			v = static_cast<Data::Edge2D *>((*iter)->getShape())->normal();
		}
		else
		{
			v = static_cast<Data::Polygon *>((*iter)->getShape())->normal();

			//v.normalize();
		}

		if ((v.dot(this->normal) <= 1.0 - Data::getTolerance()) ||
			(this->worse(this->quality((*iter)), this->worst)))
		{
			return false;
		}
#endif //#if USE_EXPERIMENTAL_3
	}

	return true;
}

#if USE_ATTRIBS
void Tools::BoundaryLaplacianSmoother::clearAttribs() const
{
	if ((this->mesh) && (!this->mesh->isSurface()))
	{
		for (Data::Mesh::ConstFrontElementIterator iter = this->mesh->feBegin();
			 iter != this->mesh->feEnd(); iter++)
		{
			Data::FrontElement *fel = (*iter);

			bool found = false;

			for (UInt i = 0; i < fel->numVertices(); i++)
			{
				if (this->boundary->have(fel->getVertex(i)))
				{
					found = true;

					break;
				}
			}

			if (!found)
			{
				continue;
			}

			Data::GeometricShape *s = fel->getShape();

			if (this->dimension == 2)
			{
#if USE_POOL
				static_cast<MJMesh::Edge2D *>(s)->recalcAttribs();
#else
				static_cast<MJMesh::Edge2D *>(s)->clearAttribs();
#endif //#if USE_POOL
			}
			else
			{
#if USE_POOL
				static_cast<MJMesh::Triangle3D *>(s)->recalcAttribs();
#else
				static_cast<MJMesh::Triangle3D *>(s)->clearAttribs();
#endif //#if USE_POOL
			}
		}

		return;
	}

	if (this->boundary)
    {
        for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
             iter != end; iter++)
        {
            Data::GeometricShape *s = (*iter)->getShape();

            if (this->dimension == 2)
            {
#if USE_POOL
                static_cast<MJMesh::Edge2D *>(s)->recalcAttribs();
#else
			stat    ic_cast<MJMesh::Edge2D *>(s)->clearAttribs();
#endif //#if USE_POOL
            }
            else
            {
#if USE_POOL
                static_cast<MJMesh::Triangle3D *>(s)->recalcAttribs();
#else
                static_cast<MJMesh::Triangle3D *>(s)->clearAttribs();
#endif //#if USE_POOL
            }
        }
    }
}
#endif //#if USE_ATTRIBS
