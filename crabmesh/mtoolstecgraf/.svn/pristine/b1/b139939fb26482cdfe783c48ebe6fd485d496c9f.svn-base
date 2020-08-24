#include "Tools/EdgeSwapBoundaryAdapter.h"

#include "Data/Box.h"
#include "MJMesh/Triangle3D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "Tools/CracksIdentifier.h"
#include "Tools/AdjacenciesIdentifier.h"

Tools::EdgeSwapBoundaryAdapter::EdgeSwapBoundaryAdapter(Data::Boundary *boundary) :
	Performer::EdgeSwapBoundaryAdapter::EdgeSwapBoundaryAdapter(boundary)
{
	this->noCrack = false;
	this->cracks = NULL;
}

Tools::EdgeSwapBoundaryAdapter::EdgeSwapBoundaryAdapter(Data::Mesh *surface) :
	Performer::EdgeSwapBoundaryAdapter::EdgeSwapBoundaryAdapter(surface)
{
	this->noCrack = false;
	this->cracks = NULL;
}

Tools::EdgeSwapBoundaryAdapter::~EdgeSwapBoundaryAdapter()
{
	if (this->cracks)
	{
		delete this->cracks;
	}
}

void Tools::EdgeSwapBoundaryAdapter::setNoCrack(bool noCrack)
{
	this->noCrack = noCrack;
}

void Tools::EdgeSwapBoundaryAdapter::execute()
{
	this->startMeasure();

	this->identifyCracks();

	Performer::EdgeSwapBoundaryAdapter::execute();

	if (this->cracks)
	{
		delete this->cracks;

		this->cracks = NULL;
	}

	this->endMeasure();
}

void Tools::EdgeSwapBoundaryAdapter::adjustTopology(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj,
	Data::Vertex *v1, Data::Vertex *v2, Data::Vertex *v3, Data::Vertex *v4) const
{
	if (this->isBoundary())
    {
        static_cast<MJMesh::Vertex *>(v1)->add(static_cast<MJMesh::FrontElement *>(adj));
        static_cast<MJMesh::Vertex *>(v2)->remove(static_cast<MJMesh::FrontElement *>(adj));
        static_cast<MJMesh::Vertex *>(v3)->add(static_cast<MJMesh::FrontElement *>(fel));
        static_cast<MJMesh::Vertex *>(v4)->remove(static_cast<MJMesh::FrontElement *>(fel));

#if USE_POOL
        static_cast<MJMesh::Triangle3D *>(fel->getShape())->recalcAttribs();
        static_cast<MJMesh::Triangle3D *>(adj->getShape())->recalcAttribs();
#else
        static_cast<MJMesh::Triangle3D *>(fel->getShape())->clearAttribs();
        static_cast<MJMesh::Triangle3D *>(adj->getShape())->clearAttribs();
#endif //#if USE_POOL
    }
    else
    {
        static_cast<MJMesh::Vertex *>(v1)->add(adj);
        static_cast<MJMesh::Vertex *>(v2)->remove(adj);
        static_cast<MJMesh::Vertex *>(v3)->add(fel);
        static_cast<MJMesh::Vertex *>(v4)->remove(fel);
    }
}

bool Tools::EdgeSwapBoundaryAdapter::adapt(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const
{
	this->addMeasure(1, 0);

	return Performer::EdgeSwapBoundaryAdapter::adapt(element, newElements, newVertices);
}

bool Tools::EdgeSwapBoundaryAdapter::swap(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj,
	Data::Vertex *v1, Data::Vertex *v2, Data::Vertex *v3, Data::Vertex *v4) const
{
	Performer::CracksIdentifier::CrackMap::iterator iter = this->cracks ?
		this->cracks->find(fel) : Performer::CracksIdentifier::CrackMap::iterator();

	if ((this->cracks) && (iter != this->cracks->end()) && ((*iter).second.data != NULL))
	{
		this->cracks->erase(iter);

		return false;
	}

	Data::/*Front*/Element *other = NULL;

	if ((this->cracks) && (iter != this->cracks->end()))
	{
		other = ((*iter).second.fel1 == fel) ? (*iter).second.fel2 : (*iter).second.fel1;
	}

	Data::Vertex *ov1, *ov2, *ov3, *ov4;
	ov1 = ov2 = ov3 = ov4 = NULL;
	Data::/*Front*/Element *oadj = NULL;

	if (other)
	{
		UInt iv1 = 3;

		for (UInt i = 0; i < 3; i++)
		{
			if (other->getVertex(i)->getPoint()->match(v1->getPoint()))
			{
				iv1 = i;

				break;
			}
		}

		ov1 = other->getVertex(iv1);
		ov2 = other->getVertex((iv1+1)%3);
		ov4 = other->getVertex((iv1+2)%3);
		ov3 = NULL;

		oadj = this->adjacencies[other][ov2];

		for (UInt i = 0; i < 3; i++)
		{
			Data::Vertex *v = oadj->getVertex(i);

			if ((v != v2) && (v != v4))
			{
				ov3 = v;

				break;
			}
		}

		Performer::CracksIdentifier::CrackMap::iterator adjiter = this->cracks ?
			this->cracks->find(adj) : Performer::CracksIdentifier::CrackMap::iterator();

		if ((adjiter == this->cracks->end()) ||
			((((*adjiter).second.fel1 == adj) ? (*iter).second.fel2 : (*iter).second.fel1) != oadj))
		{
			return false;
		}

		this->cracks->erase(adjiter);

		(*this->cracks)[oadj].data = this->cracks;
	}

	bool swapped = false;

	if (Performer::EdgeSwapBoundaryAdapter::swap(fel, adj, v1, v2, v3, v4))
	{
		this->adjustTopology(fel, adj, v1, v2, v3, v4);

		swapped = true;
	}

	if (other)
	{
		if (Performer::EdgeSwapBoundaryAdapter::swap(other, oadj, ov1, ov2, ov3, ov4))
		{
			this->adjustTopology(other, oadj, ov1, ov2, ov3, ov4);

			swapped = true;
		}

		//this->cracks[other].done = true;
		(*this->cracks)[other].data = this->cracks;

		//this->cracks.erase(iter);
		this->cracks->erase(iter);
	}

	return swapped;
}

Performer::AdjacenciesIdentifier *Tools::EdgeSwapBoundaryAdapter::makeAdjacenciesIdentifier() const
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

bool Tools::EdgeSwapBoundaryAdapter::addToElements(const Data::/*Front*/Element *fel) const
{
	this->addMeasure(1, 0);

	return Performer::EdgeSwapBoundaryAdapter::addToElements(fel);
}

bool Tools::EdgeSwapBoundaryAdapter::removeFromElements(const Data::/*Front*/Element *fel) const
{
	bool removed = Performer::EdgeSwapBoundaryAdapter::removeFromElements(fel);

	if (removed)
	{
		this->addMeasure(1, 0);
	}

	return removed;
}

void Tools::EdgeSwapBoundaryAdapter::identifyQualities()
{
	this->addMeasure(0, static_cast<UInt>(this->getMesh()->size()));

	Performer::EdgeSwapBoundaryAdapter::identifyQualities();
}

void Tools::EdgeSwapBoundaryAdapter::executeAdaptation()
{
	this->addMeasure(0, static_cast<UInt>(this->elements.size()) + 1);

	Performer::EdgeSwapBoundaryAdapter::executeAdaptation();

	this->addMeasure(1, 0);
}

Performer::CracksIdentifier *Tools::EdgeSwapBoundaryAdapter::makeCracksIdentifier() const
{
	Tools::CracksIdentifier *identifier = new Tools::CracksIdentifier(this);

	identifier->setBoundary(this->getBoundary());

	return identifier;
}

void Tools::EdgeSwapBoundaryAdapter::identifyCracks()
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
