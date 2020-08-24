#include "Tools/EdgeCollapseBoundaryAdapter.h"

#include "Data/Box.h"
#include "MJMesh/Triangle3D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Boundary.h"
#include "Tools/CracksIdentifier.h"
#include "Tools/AdjacenciesIdentifier.h"

Tools::EdgeCollapseBoundaryAdapter::EdgeCollapseBoundaryAdapter(Data::Boundary *boundary, Data::Mesh *mesh) :
	Performer::EdgeCollapseBoundaryAdapter::EdgeCollapseBoundaryAdapter(boundary, mesh),
	Tools::Tool::Tool()
{
	this->noCrack = false;
	this->cracks = NULL;
	this->allowHangingVertices = true;
}

Tools::EdgeCollapseBoundaryAdapter::EdgeCollapseBoundaryAdapter(Data::Mesh *surface) :
	Performer::EdgeCollapseBoundaryAdapter::EdgeCollapseBoundaryAdapter(surface),
	Tools::Tool::Tool()
{
	this->noCrack = false;
	this->cracks = NULL;
	this->allowHangingVertices = true;
}

Tools::EdgeCollapseBoundaryAdapter::~EdgeCollapseBoundaryAdapter()
{
	if (this->cracks)
	{
		delete this->cracks;
	}
}

void Tools::EdgeCollapseBoundaryAdapter::setNoCrack(bool noCrack)
{
	this->noCrack = noCrack;
}

void Tools::EdgeCollapseBoundaryAdapter::execute()
{
	this->startMeasure();

	this->identifyCracks();

	Performer::EdgeCollapseBoundaryAdapter::execute();

	if (this->cracks)
	{
		delete this->cracks;

		this->cracks = NULL;
	}

	this->endMeasure();
}

void Tools::EdgeCollapseBoundaryAdapter::adjustTopology(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj,
	Data::Vertex *v1, Data::Vertex *v2) const
{
	Data::/*Front*/ElementList adjs = this->adjacency(v2);

	while (!adjs.empty())
	{
		Data::/*Front*/Element *fe = adjs.front();

		adjs.pop_front();

		if (this->isBoundary())
        {
            static_cast<MJMesh::Vertex *>(v2)->remove(static_cast<MJMesh::FrontElement *>(fe));
        }
        else
        {
            static_cast<MJMesh::Vertex *>(v2)->remove(fe);
        }

		if ((fe == fel) || (fe == adj))
		{
			continue;
		}

		if (this->isBoundary())
        {
            static_cast<MJMesh::Vertex *>(v1)->add(static_cast<MJMesh::FrontElement *>(fe));

#if USE_POOL
            static_cast<MJMesh::Triangle3D *>(fe->getShape())->recalcAttribs();
#else
            static_cast<MJMesh::Triangle3D *>(fe->getShape())->clearAttribs();
#endif //#if USE_POOL
        }
        else
        {
            static_cast<MJMesh::Vertex *>(v1)->add(fe);
        }
	}
}

bool Tools::EdgeCollapseBoundaryAdapter::adapt(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const
{
	this->addMeasure(1, 0);

	return Performer::EdgeCollapseBoundaryAdapter::adapt(element, newElements, newVertices);
}

bool Tools::EdgeCollapseBoundaryAdapter::collapse(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj,
	Data::Vertex *v1, Data::Vertex *v2) const
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

	Data::Vertex *ov1, *ov2;
	ov1 = ov2 = NULL;
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
		ov2 = other->getVertex((iv1+2)%3);

		oadj = this->adjacencies[other][ov2];

		Performer::CracksIdentifier::CrackMap::iterator adjiter = this->cracks->find(adj);

		if ((adjiter == this->cracks->end()) ||
			((((*adjiter).second.fel1 == adj) ? (*iter).second.fel2 : (*iter).second.fel1) != oadj))
		{
			return false;
		}

		this->cracks->erase(adjiter);

		(*this->cracks)[oadj].data = this->cracks;
	}

	bool collapsed = Performer::EdgeCollapseBoundaryAdapter::collapse(fel, adj, v1, v2);

	if (collapsed)
	{
		this->adjustTopology(fel, adj, v1, v2);

		if (other)
		{
			this->removeFromElements(oadj);

			if (Performer::EdgeCollapseBoundaryAdapter::collapse(other, oadj, ov1, ov2))
			{
				this->adjustTopology(other, oadj, ov1, ov2);

				this->deletes(other, oadj, ov1, ov2);
			}
			else
			{
				this->addToElements(oadj);
			}

			(*this->cracks)[other].data = this->cracks;

			this->cracks->erase(iter);
		}
	}

	return collapsed;
}

bool Tools::EdgeCollapseBoundaryAdapter::canCollapse(const Data::/*Front*/Element *fel, UInt i) const
{
	if ((this->allowHangingVertices) && (!Performer::EdgeCollapseBoundaryAdapter::canCollapse(fel, i)))
	{
		return false;
	}

	Performer::CracksIdentifier::CrackMap::iterator iter;

	Data::Vertex *v = fel->getVertex(i);

	Data::/*Front*/ElementList adjs = this->adjacency(v);

	bool icrack = false;
	bool inocrack = false;

	while (!adjs.empty())
	{
		Data::/*Front*/Element *fe = adjs.front();

		adjs.pop_front();

		//iter = this->cracks.find(fe);
		iter = (this->cracks) ? this->cracks->find(fe) : Performer::CracksIdentifier::CrackMap::iterator();

		//if (iter == this->cracks.end())
		if ((!this->cracks) || (iter == this->cracks->end()))
		{
			inocrack = true;
		}
		else
		{
			icrack = true;
		}
	}

	v = fel->getVertex((i+1)%fel->numVertices());

	adjs = this->adjacency(v);

	bool jcrack = false;
	bool jnocrack = false;

	while (!adjs.empty())
	{
		Data::/*Front*/Element *fe = adjs.front();

		adjs.pop_front();

		//iter = this->cracks.find(fe);
		iter = (this->cracks) ? this->cracks->find(fe) : Performer::CracksIdentifier::CrackMap::iterator();

		//if (iter == this->cracks.end())
		if ((!this->cracks) || (iter == this->cracks->end()))
		{
			jnocrack = true;
		}
		else
		{
			jcrack = true;
		}
	}

	return (((icrack) && (jcrack) && (!inocrack) && (!jnocrack)) ||
			((!icrack) && (!jcrack) && (inocrack) && (jnocrack)));
}

bool Tools::EdgeCollapseBoundaryAdapter::addToElements(Data::/*Front*/Element *fel) const
{
	this->addMeasure(1, 0);

	return Performer::EdgeCollapseBoundaryAdapter::addToElements(fel);
}

void Tools::EdgeCollapseBoundaryAdapter::identifyQualities()
{
	this->addMeasure(0, static_cast<UInt>(this->getMesh()->size()));

	Performer::EdgeCollapseBoundaryAdapter::identifyQualities();
}

void Tools::EdgeCollapseBoundaryAdapter::executeAdaptation()
{
	this->addMeasure(0, static_cast<UInt>(this->elements.size()) + 1);

	Performer::EdgeCollapseBoundaryAdapter::executeAdaptation();
}

Performer::AdjacenciesIdentifier *Tools::EdgeCollapseBoundaryAdapter::makeAdjacenciesIdentifier() const
{
	Tools::AdjacenciesIdentifier *identifier = new Tools::AdjacenciesIdentifier(this);

	if (this->isBoundary())
    {
        identifier->setBoundary(this->getBoundary());

        identifier->setAllowHangingVertices(this->allowHangingVertices);
    }
    else
    {
        identifier->setSurface(this->getSurface());

        identifier->setAllowHangingVertices(true);
    }

	return identifier;
}

Performer::CracksIdentifier *Tools::EdgeCollapseBoundaryAdapter::makeCracksIdentifier() const
{
	Tools::CracksIdentifier *identifier = new Tools::CracksIdentifier(this);

	identifier->setBoundary(this->getBoundary());

	return identifier;
}

void Tools::EdgeCollapseBoundaryAdapter::identifyCracks()
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
