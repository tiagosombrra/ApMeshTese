#include "Tools/SizeBoundaryAdapter.h"

#include "MJMesh/Edge2D.h"
#include "MJMesh/Triangle3D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/ElementSurface.h"
#include "MJMesh/Vertex.h"
#include "Tools/CracksIdentifier.h"
#include "Tools/AdjacenciesIdentifier.h"

Tools::SizeBoundaryAdapter::SizeBoundaryAdapter(MJMesh::Boundary *boundary) :
	Performer::SizeBoundaryAdapter::SizeBoundaryAdapter(boundary),
	Tools::Tool::Tool()
{
	this->noCrack = false;
	this->cracks = NULL;
}

Tools::SizeBoundaryAdapter::SizeBoundaryAdapter(MJMesh::Mesh *surface) :
	Performer::SizeBoundaryAdapter::SizeBoundaryAdapter(surface),
	Tools::Tool::Tool()
{
	this->noCrack = false;
	this->cracks = NULL;
}

Tools::SizeBoundaryAdapter::~SizeBoundaryAdapter()
{
	if (this->cracks)
	{
		this->cracks = NULL;
	}
}

void Tools::SizeBoundaryAdapter::setNoCrack(bool noCrack)
{
	this->noCrack = noCrack;
}

void Tools::SizeBoundaryAdapter::adjustTopologyEdge2D(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements) const
{
	if (this->isBoundary())
    {
        static_cast<MJMesh::Vertex *>(newElements.back()->getVertex(1))->remove(static_cast<MJMesh::FrontElement *>(element));
        static_cast<MJMesh::Vertex *>(element->getVertex(1))->add(static_cast<MJMesh::FrontElement *>(element));
    }
    else
    {
        static_cast<MJMesh::Vertex *>(newElements.back()->getVertex(1))->remove(element);
        static_cast<MJMesh::Vertex *>(element->getVertex(1))->add(element);
    }

#if USE_POOL
	static_cast<MJMesh::Edge2D *>(element->getShape())->recalcAttribs();
#else
	static_cast<MJMesh::Edge2D *>(element->getShape())->clearAttribs();
#endif //#if USE_POOL
}

void Tools::SizeBoundaryAdapter::adjustTopologyTriangle3D(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements) const
{
	if (this->isBoundary())
    {
        static_cast<MJMesh::Vertex *>((newElements.back())->getVertex(0))->remove(static_cast<MJMesh::FrontElement *>(element));
        static_cast<MJMesh::Vertex *>(element->getVertex(2))->add(static_cast<MJMesh::FrontElement *>(element));

#if USE_POOL
        static_cast<MJMesh::Triangle3D *>(element->getShape())->recalcAttribs();
#else
        static_cast<MJMesh::Triangle3D *>(element->getShape())->clearAttribs();
#endif //#if USE_POOL
    }
    else
    {
        static_cast<MJMesh::Vertex *>((newElements.back())->getVertex(0))->remove(element);
        static_cast<MJMesh::Vertex *>(element->getVertex(2))->add(element);
    }
}

bool Tools::SizeBoundaryAdapter::adapt(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const
{
	this->addMeasure(1, 0);

	Performer::CracksIdentifier::CrackMap::iterator iter = this->cracks ?
		this->cracks->find(element) : Performer::CracksIdentifier::CrackMap::iterator();

	if ((this->cracks) && (iter != this->cracks->end()) && ((*iter).second.data != NULL))
	{
		this->cracks->erase(iter);

		return false;
	}

	Data::/*Front*/Element *other = NULL;

	if ((this->cracks) && (iter != this->cracks->end()))
	{
		other = ((*iter).second.fel1 == element) ? (*iter).second.fel2 : (*iter).second.fel1;
	}

	if (!Performer::SizeBoundaryAdapter::adapt(element, newElements, newVertices))
	{
		return false;
	}

	if (this->dimension == 2)
	{
		this->adjustTopologyEdge2D(element, newElements);

		if (other)
		{
			MJMesh::Vertex *mid = static_cast<MJMesh::Vertex *>(this->makeVertex(*element->getVertex(1)->getPoint()));

			newVertices.push_back(mid);

			this->adaptEdge2D(other, mid, newElements);

			this->adjustTopologyEdge2D(other, newElements);

			(*this->cracks)[other].data = this->cracks;

			this->cracks->erase(iter);
		}

		return true;
	}

	if (this->dimension == 3)
	{
		if (element->numVertices() == 3)
		{
			this->adjustTopologyTriangle3D(element, newElements);

			if (other)
			{
				Data::Vertex *mid = this->makeVertex((*element->getVertex(2)->getPoint()));

				newVertices.push_back(mid);

				this->adaptTriangle3D(other, mid, newElements);

				this->adjustTopologyTriangle3D(other, newElements);

				(*this->cracks)[other].data = this->cracks;

				this->cracks->erase(iter);
			}

			return true;
		}
	}

	return false;
}

void Tools::SizeBoundaryAdapter::execute()
{
	this->startMeasure();

	Performer::SizeBoundaryAdapter::execute();

	this->endMeasure();
}

Performer::AdjacenciesIdentifier *Tools::SizeBoundaryAdapter::makeAdjacenciesIdentifier() const
{
	Tools::AdjacenciesIdentifier *identifier = new Tools::AdjacenciesIdentifier(this);

	if (this->isBoundary())
    {
        identifier->setBoundary(this->getBoundary());
    }
    else
    {
        identifier->setSurface(this->getSurface());

        identifier->setAllowHangingVertices(true);
    }

	return identifier;
}

void Tools::SizeBoundaryAdapter::checkAdjacenciesSizes(const Performer::AdjacenciesIdentifier::AdjacencyMap::iterator &iter)
{
	this->addMeasure(1, 0);

	Performer::SizeBoundaryAdapter::checkAdjacenciesSizes(iter);
}

void Tools::SizeBoundaryAdapter::checkAdjacenciesSizes()
{
	this->identifyCracks();

	this->addMeasure(0, static_cast<UInt>(this->adjacencies.size()) + 1);

	Performer::SizeBoundaryAdapter::checkAdjacenciesSizes();

	if (this->cracks)
	{
		delete this->cracks;

		this->cracks = NULL;
	}

	this->addMeasure(1, 0);
}

void Tools::SizeBoundaryAdapter::executeAdaptation()
{
	this->addMeasure(0, static_cast<UInt>(this->toAdapt.size()) + 1);

	Performer::SizeBoundaryAdapter::executeAdaptation();

	this->addMeasure(1, 0);
}

Performer::CracksIdentifier *Tools::SizeBoundaryAdapter::makeCracksIdentifier() const
{
	Tools::CracksIdentifier *identifier = new Tools::CracksIdentifier(this);

	identifier->setBoundary(this->getBoundary());

	return identifier;
}

void Tools::SizeBoundaryAdapter::identifyCracks()
{
	if (this->noCrack)
	{
		return;
	}

	if ((!this->isBoundary()) || (!this->getBoundary()))
	{
		return;
	}

	Performer::CracksIdentifier *identifier = this->makeCracksIdentifier();

	identifier->execute();

	this->cracks = identifier->getCracks();

	delete identifier;
}

Data::Vertex *Tools::SizeBoundaryAdapter::makeVertex(const Data::Point &pt) const
{
	Data::Point *p = NULL;

	if (this->dimension == 2)
	{
		p = new Data::Point2D(pt);
	}
	else if (this->dimension == 3)
	{
		p = new Data::Point3D(pt);
	}
	else
	{
#if USE_EXPERIMENTAL_3
        return NULL;
#else
		p = new Data::Point(pt);
#endif //#if USE_EXPERIMENTAL_3
	}

	return new MJMesh::Vertex(p);
}

Data::/*Front*/Element *Tools::SizeBoundaryAdapter::makeElement(const Data::VertexVector &vertices) const
{
	if (this->dimension == 2)
	{
		MJMesh::FrontElement2D *element = new MJMesh::FrontElement2D(
			static_cast<MJMesh::Vertex *>(vertices[0]),
			static_cast<MJMesh::Vertex *>(vertices[1]));

		return element;
	}

	if (this->dimension == 3)
	{
		if (vertices.size() == 3)
		{
			if (this->isBoundary())
            {
                MJMesh::FrontElement3D *element = new MJMesh::FrontElement3D(
                    static_cast<MJMesh::Vertex *>(vertices[0]),
                    static_cast<MJMesh::Vertex *>(vertices[1]),
                    static_cast<MJMesh::Vertex *>(vertices[2]));

                return element;
            }
            else
            {
                MJMesh::ElementSurface *element = new MJMesh::ElementSurface(
                    static_cast<MJMesh::Vertex *>(vertices[0]),
                    static_cast<MJMesh::Vertex *>(vertices[1]),
                    static_cast<MJMesh::Vertex *>(vertices[2]));

                return element;
            }
		}
	}

	return NULL;
}
