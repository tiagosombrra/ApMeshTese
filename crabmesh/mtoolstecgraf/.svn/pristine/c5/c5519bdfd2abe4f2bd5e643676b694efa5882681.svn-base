#include "Tools/EdgeBoundaryAdapter.h"

#include "MJMesh/Triangle3D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/ElementSurface.h"
#include "MJMesh/Boundary.h"
#include "Tools/AdjacenciesIdentifier.h"
#include "Tools/CracksIdentifier.h"

Tools::EdgeBoundaryAdapter::EdgeBoundaryAdapter(Data::Boundary *boundary) :
	Performer::EdgeBoundaryAdapter::EdgeBoundaryAdapter(boundary),
	Tools::Tool::Tool()
{
	this->noCrack = false;
	this->cracks = NULL;
}

Tools::EdgeBoundaryAdapter::EdgeBoundaryAdapter(Data::Mesh *surface) :
	Performer::EdgeBoundaryAdapter::EdgeBoundaryAdapter(surface),
	Tools::Tool::Tool()
{
	this->noCrack = false;
	this->cracks = NULL;
}

Tools::EdgeBoundaryAdapter::~EdgeBoundaryAdapter()
{
	if (this->cracks)
	{
		delete this->cracks;
	}
}

void Tools::EdgeBoundaryAdapter::setNoCrack(bool noCrack)
{
	this->noCrack = noCrack;
}

/*void Tools::EdgeBoundaryAdapter::adjustTopologyEdge2D(Data::FrontElement *element, Data::FrontElementList &newElements) const
{
	static_cast<MJMesh::Vertex *>(newElements.back()->getVertex(1))->remove(element);
	static_cast<MJMesh::Vertex *>(element->getVertex(1))->add(element);

#if USE_POOL
	static_cast<MJMesh::Edge2D *>(element->getShape())->recalcAttribs();
#else
	static_cast<MJMesh::Edge2D *>(element->getShape())->clearAttribs();
#endif //#if USE_POOL
}*/

void Tools::EdgeBoundaryAdapter::adjustTopologyTriangle3D(Data::/*Front*/Element *element, Data::Vertex *mids[3], Data::/*Front*/ElementList &newElements) const
{
	UInt count = 0;

	for (UInt i = 0; i < 3; i++)
	{
		if (mids[i])
		{
			count++;
		}
	}

	if (count == 0)
	{
		return;
	}

	if (count == 3)
	{
		Data::/*Front*/ElementList::iterator it = newElements.end();

		it--;
		it--;
		it--;

		if (this->isBoundary())
        {
            static_cast<MJMesh::Vertex *>((*it)->getVertex(1))->remove(static_cast<MJMesh::FrontElement *>(element));
            it++;
            static_cast<MJMesh::Vertex *>((*it)->getVertex(2))->remove(static_cast<MJMesh::FrontElement *>(element));

            static_cast<MJMesh::Vertex *>(element->getVertex(1))->add(static_cast<MJMesh::FrontElement *>(element));
            static_cast<MJMesh::Vertex *>(element->getVertex(2))->add(static_cast<MJMesh::FrontElement *>(element));

#if USE_POOL
            static_cast<MJMesh::Triangle3D *>(element->getShape())->recalcAttribs();
#else
            static_cast<MJMesh::Triangle3D *>(element->getShape())->clearAttribs();
#endif //#if USE_POOL
        }
        else
        {
            static_cast<MJMesh::Vertex *>((*it)->getVertex(1))->remove(element);
            it++;
            static_cast<MJMesh::Vertex *>((*it)->getVertex(2))->remove(element);

            static_cast<MJMesh::Vertex *>(element->getVertex(1))->add(element);
            static_cast<MJMesh::Vertex *>(element->getVertex(2))->add(element);
        }

		return;
	}

	UInt dif = 3;

	for (UInt i = 0; i < 3; i++)
	{
		if (((count == 1) && (mids[i])) ||
			((count == 2) && (!mids[i])))
		{
			dif = i;

			break;
		}
	}

	if (count == 1)
	{
		Data::/*Front*/Element *newElement = newElements.back();

		if (this->isBoundary())
        {
            static_cast<MJMesh::Vertex *>(newElement->getVertex(1))->remove(static_cast<MJMesh::FrontElement *>(element));

            static_cast<MJMesh::Vertex *>(element->getVertex((dif+1)%3))->add(static_cast<MJMesh::FrontElement *>(element));

#if USE_POOL
            static_cast<MJMesh::Triangle3D *>(element->getShape())->recalcAttribs();
#else
            static_cast<MJMesh::Triangle3D *>(element->getShape())->clearAttribs();
#endif //#if USE_POOL
        }
        else
        {
            static_cast<MJMesh::Vertex *>(newElement->getVertex(1))->remove(element);

            static_cast<MJMesh::Vertex *>(element->getVertex((dif+1)%3))->add(element);
        }

		return;
	}

	if (count == 2)
	{
		if (this->isBoundary())
        {
            static_cast<MJMesh::Vertex *>(element->getVertex(dif))->add(static_cast<MJMesh::FrontElement *>(element));
            static_cast<MJMesh::Vertex *>(element->getVertex((dif+1)%3))->add(static_cast<MJMesh::FrontElement *>(element));
        }
        else
        {
            static_cast<MJMesh::Vertex *>(element->getVertex(dif))->add(element);
            static_cast<MJMesh::Vertex *>(element->getVertex((dif+1)%3))->add(element);
        }

		Data::/*Front*/ElementList::iterator it = newElements.end();

		it--;
		it--;

		Data::/*Front*/Element *newElement = (*it);

		if (this->isBoundary())
        {
            static_cast<MJMesh::Vertex *>(newElement->getVertex(0))->remove(static_cast<MJMesh::FrontElement *>(element));
            static_cast<MJMesh::Vertex *>(newElement->getVertex(1))->remove(static_cast<MJMesh::FrontElement *>(element));

#if USE_POOL
            static_cast<MJMesh::Triangle3D *>(element->getShape())->recalcAttribs();
#else
            static_cast<MJMesh::Triangle3D *>(element->getShape())->clearAttribs();
#endif //#if USE_POOL
        }
        else
        {
            static_cast<MJMesh::Vertex *>(newElement->getVertex(0))->remove(element);
            static_cast<MJMesh::Vertex *>(newElement->getVertex(1))->remove(element);
        }

		return;
	}
}

bool Tools::EdgeBoundaryAdapter::adapt(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const
{
	this->addMeasure(1, 0);

	return Performer::EdgeBoundaryAdapter::adapt(element, newElements, newVertices);
}

bool Tools::EdgeBoundaryAdapter::adaptTriangle3D(Data::/*Front*/Element *element,
	Data::Vertex *vs[3], Data::Vertex *mids[3], Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const
{
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

	Data::Vertex *ov[element->numVertices()];

	if (other)
	{
		for (UInt i = 0; i < element->numVertices(); i++)
		{
			ov[i] = element->getVertex(i);
		}
	}

	if (!Performer::EdgeBoundaryAdapter::adaptTriangle3D(element, vs, mids, newElements, newVertices))
	{
		return false;
	}

	/*if (this->dimension == 2)
	{
		this->adjustTopologyEdge2D(element, newElements);

		if (other)
		{
			MJMesh::Vertex *mid = static_cast<MJMesh::Vertex *>(this->makeVertex(*element->getVertex(1)->getPoint()));

			newVertices.push_back(mid);

			this->adaptEdge2D(other, mid, newElements);

			this->adjustTopologyEdge2D(other, newElements);

			this->cracks[other].done = true;

			this->cracks.erase(iter);
		}

		return true;
	}*/

	if (this->dimension == 3)
	{
		if (element->numVertices() == 3)
		{
			this->adjustTopologyTriangle3D(element, mids, newElements);

			if (other)
			{
#if USE_C__11
				EBoolHashMap::const_iterator iter2 = this->refinements.find(other);
#else
				EBoolMap::const_iterator iter2 = this->refinements.find(other);
#endif //#if USE_C__11

				if (iter2 == this->refinements.end())
				{
					return false;
				}

				//Data::Vertex *vs[3];

				/*for (UInt i = 0; i < 3; i++)
				{
					vs[i] = other->getVertex(i);
				}*/

				//Data::Vertex *mids[3];

				for (UInt i = 0; i < 3; i++)
				{
					if (!(*iter2).second[i])
					{
						mids[i] = NULL;

						continue;
					}

					ULInt id1 = vs[i]->getId();
					ULInt id2 = vs[(i+1)%3]->getId();

					mids[i] = this->mids[id1][id2];

					if (!mids[i])
					{
						Data::Point3D pt;

						ULInt eid1 = 0, eid2 = 0;

						for (UInt j = 0; j < element->numVertices(); j++)
						{
							if (ov[j]->getPoint()->match(vs[i]->getPoint()))
							{
								eid1 = ov[j]->getId();
							}

							if (ov[j]->getPoint()->match(vs[(i+1)%3]->getPoint()))
							{
								eid2 = ov[j]->getId();
							}

							if ((eid1 != 0) && (eid2 != 0))
							{
								break;
							}
						}

#if USE_C__11
						VertexMatrixHashMap::iterator it = this->mids.find(eid1);
#else
						VertexMatrixMap::iterator it = this->mids.find(eid1);
#endif //#if USE_C__11

						if (it == this->mids.end())
						{
							pt = *this->mids[eid2][eid1]->getPoint();
						}
						else
						{
#if USE_C__11
							std::unordered_map<ULInt, Data::Vertex *>::iterator it2 = (*it).second.find(eid2);
#else
							std::map<ULInt, Data::Vertex *>::iterator it2 = (*it).second.find(eid2);
#endif //#if USE_C__11

							if (it2 == (*it).second.end())
							{
								pt = *this->mids[eid2][eid1]->getPoint();
							}
							else
							{
								//When the crack geometry is correct, this should
								//  be the case. By "correct", I mean that the triangle
								//  normals point to different directions
								//pt = *this->mids[eid1][eid2]->getPoint();
								pt = *(*it2).second->getPoint();
							}
						}

						this->mids[id2][id1] = mids[i] = this->makeVertex(pt);

						newVertices.push_back(mids[i]);
					}
				}

				//this->adaptTriangle3D(other, vs, mids, newElements);
				Performer::EdgeBoundaryAdapter::adaptTriangle3D(other, ov, mids, newElements, newVertices);

				this->adjustTopologyTriangle3D(other, mids, newElements);

				//this->cracks[other].done = true;
				(*this->cracks)[other].data = this->cracks;

				this->cracks->erase(iter);
			}

			return true;
		}
	}

	return false;
}

void Tools::EdgeBoundaryAdapter::execute()
{
	this->startMeasure();

	this->identifyCracks();

	Performer::EdgeBoundaryAdapter::execute();

	if (this->cracks)
	{
		delete this->cracks;

		this->cracks = NULL;
	}

	this->endMeasure();
}

bool Tools::EdgeBoundaryAdapter::addToElements(Data::/*Front*/Element *fel) const
{
	this->addMeasure(1, 0);

	return Performer::EdgeBoundaryAdapter::addToElements(fel);
}

void Tools::EdgeBoundaryAdapter::identifyQualities()
{
	this->addMeasure(0, static_cast<UInt>(this->getMesh()->size()));

	Performer::EdgeBoundaryAdapter::identifyQualities();
}

Performer::AdjacenciesIdentifier *Tools::EdgeBoundaryAdapter::makeAdjacenciesIdentifier() const
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

Performer::CracksIdentifier *Tools::EdgeBoundaryAdapter::makeCracksIdentifier() const
{
	Tools::CracksIdentifier *identifier = new Tools::CracksIdentifier(this);

	identifier->setBoundary(this->getBoundary());

	return identifier;
}

void Tools::EdgeBoundaryAdapter::identifyCracks()
{
	//this->cracks.clear();

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

void Tools::EdgeBoundaryAdapter::identifyRefinements()
{
	this->addMeasure(0, static_cast<UInt>(this->elements.size()));

	Performer::EdgeBoundaryAdapter::identifyRefinements();
}

bool Tools::EdgeBoundaryAdapter::shouldRefine(const Data::/*Front*/Element *fel)
{
	this->addMeasure(1, 0);

	if (!Performer::EdgeBoundaryAdapter::shouldRefine(fel))
	{
		return false;
	}

	Performer::CracksIdentifier::CrackMap::iterator iter = this->cracks ?
		this->cracks->find(const_cast<Data::/*Front*/Element *>(fel)) : Performer::CracksIdentifier::CrackMap::iterator();

	if ((!this->cracks) || (iter == this->cracks->end()))
	{
		return true;
	}

	Data::/*Front*/Element *other = ((*iter).second.fel1 == fel) ? (*iter).second.fel2 : (*iter).second.fel1;

	Data::BoolVector otherrefs(3, false);

#if USE_C__11
	std::pair<EBoolHashMap::iterator, bool> p2 = this->refinements.insert(std::make_pair(other, otherrefs));
#else
	std::pair<EBoolMap::iterator, bool> p2 = this->refinements.insert(std::make_pair(other, otherrefs));
#endif //#if USE_C__11

	UInt i = other->numVertices();

	for (i = 0; i < other->numVertices(); i++)
	{
		Data::Vertex *v = other->getVertex(i);

		if (v->getPoint()->match(fel->getVertex(0)->getPoint()))
		{
			break;
		}
	}

	Data::BoolVector refs = this->refinements[const_cast<Data::/*Front*/Element *>(fel)];

	for (UInt j = 0; j < fel->numVertices(); j++)
	{
		UInt k = (i + other->numVertices() - j)%other->numVertices();

		(*p2.first).second[k] = (((*p2.first).second[k]) || (refs[j]));

		if (!(*p2.first).second[k])
		{
			continue;
		}

		Data::Vertex *n = other->getVertex((k+1)%other->numVertices());
		Data::/*Front*/Element *adj = this->adjacencies[other][other->getVertex(k)];

		UInt l = other->numVertices();

		for (l = 0; l < other->numVertices(); l++)
		{
			if (adj->getVertex(l) == n)
			{
				break;
			}
		}

		Data::BoolVector adjref(adj->numVertices(), false);

		adjref[l] = true;

#if USE_C__11
		std::pair<EBoolHashMap::iterator, bool> p3 = this->refinements.insert(std::make_pair(adj, adjref));
#else
		std::pair<EBoolMap::iterator, bool> p3 = this->refinements.insert(std::make_pair(adj, adjref));
#endif //#if USE_C__11

		if (!p3.second)
		{
			(*p3.first).second[l] = true;
		}
	}

	return true;
}

void Tools::EdgeBoundaryAdapter::executeAdaptation()
{
	this->addMeasure(0, static_cast<UInt>(this->refinements.size()) + 1);

	Performer::EdgeBoundaryAdapter::executeAdaptation();

	this->addMeasure(1, 0);
}

Data::Vertex *Tools::EdgeBoundaryAdapter::makeVertex(const Data::Point &pt) const
{
	Data::Point *p = NULL;

	/*if (this->dimension == 2)
	{
		p = new Data::Point2D(pt);
	}
	else */if (this->dimension == 3)
	{
		p = new Data::Point3D(pt);
	}
	else
	{
#if USE_EXPERIMENTAL_3
        return NULL;
#else
		p = new Data::Point(pt);
#endif //USE_EXPERIMENTAL_3
	}

	return new MJMesh::Vertex(p);
}

Data::/*Front*/Element *Tools::EdgeBoundaryAdapter::makeElement(const Data::VertexVector &vertices) const
{
	/*if (this->dimension == 2)
	{
		MJMesh::FrontElement2D *element = new MJMesh::FrontElement2D(
			static_cast<MJMesh::Vertex *>(vertices[0]),
			static_cast<MJMesh::Vertex *>(vertices[1]));

		return element;
	}*/

	if (this->dimension == 3)
	{
		if (vertices.size() == 3)
		{
			if (this->isBoundary())
            {
                return new MJMesh::FrontElement3D(
                    static_cast<MJMesh::Vertex *>(vertices[0]),
                    static_cast<MJMesh::Vertex *>(vertices[1]),
                    static_cast<MJMesh::Vertex *>(vertices[2]));
            }
            else
            {
                return new MJMesh::ElementSurface(
                    static_cast<MJMesh::Vertex *>(vertices[0]),
                    static_cast<MJMesh::Vertex *>(vertices[1]),
                    static_cast<MJMesh::Vertex *>(vertices[2]));
            }
		}
	}

	return NULL;
}
