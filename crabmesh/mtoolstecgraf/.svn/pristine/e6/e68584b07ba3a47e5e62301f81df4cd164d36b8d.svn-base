#include "Tools/UniformBoundaryAdapter.h"

#include "MJMesh/Edge2D.h"
#include "MJMesh/Triangle3D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/ElementSurface.h"
#include "MJMesh/Vertex.h"
#include "Tools/CracksIdentifier.h"

Tools::UniformBoundaryAdapter::UniformBoundaryAdapter(MJMesh::Boundary *boundary) :
	Performer::UniformBoundaryAdapter::UniformBoundaryAdapter(boundary),
	Tools::Tool::Tool()
{
	this->noCrack = false;
	this->cracks = NULL;
}

Tools::UniformBoundaryAdapter::UniformBoundaryAdapter(MJMesh::Mesh *surface) :
	Performer::UniformBoundaryAdapter::UniformBoundaryAdapter(surface),
	Tools::Tool::Tool()
{
	this->noCrack = false;
	this->cracks = NULL;
}

Tools::UniformBoundaryAdapter::~UniformBoundaryAdapter()
{
	if (this->cracks)
	{
		delete this->cracks;
	}
}

void Tools::UniformBoundaryAdapter::setNoCrack(bool noCrack)
{
	this->noCrack = noCrack;
}

void Tools::UniformBoundaryAdapter::adjustTopologyEdge2D(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements) const
{
	static_cast<MJMesh::Vertex *>(newElements.back()->getVertex(1))->remove(static_cast<MJMesh::FrontElement *>(element));
	static_cast<MJMesh::Vertex *>(element->getVertex(1))->add(static_cast<MJMesh::FrontElement *>(element));

#if USE_POOL
	static_cast<MJMesh::Edge2D *>(element->getShape())->recalcAttribs();
#else
	static_cast<MJMesh::Edge2D *>(element->getShape())->clearAttribs();
#endif //#if USE_POOL
}

void Tools::UniformBoundaryAdapter::adjustTopologyTriangle3D(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements) const
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
}

bool Tools::UniformBoundaryAdapter::adapt(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const
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

	Data::Vertex *ov[element->numVertices()];

	if (other)
	{
		for (UInt i = 0; i < element->numVertices(); i++)
		{
			ov[i] = element->getVertex(i);
		}
	}

	if (!Performer::UniformBoundaryAdapter::adapt(element, newElements, newVertices))
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
				Data::Vertex *vs[3];

				for (UInt i = 0; i < 3; i++)
				{
					vs[i] = other->getVertex(i);
				}

				Data::Vertex *mids[3] = {NULL, NULL, NULL};

				for (UInt i = 0; i < 3; i++)
				{
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

						mids[i] = this->makeVertex(pt);

						this->mids[id2][id1] = mids[i];
						this->mids[id1][id2] = mids[i];

						newVertices.push_back(mids[i]);
					}
				}

				this->adaptTriangle3D(other, vs, mids, newElements);

				this->adjustTopologyTriangle3D(other, newElements);

				(*this->cracks)[other].data = this->cracks;

				this->cracks->erase(iter);
			}

			return true;
		}
	}

	return false;
}

void Tools::UniformBoundaryAdapter::execute()
{
	this->setMeasure(0, 1);

	this->identifyCracks();

	this->addMeasure(0, static_cast<UInt>(this->getMesh()->size()));

	Performer::UniformBoundaryAdapter::execute();

	if (this->cracks)
	{
		delete this->cracks;

		this->cracks = NULL;
	}

	this->endMeasure();
}

Performer::CracksIdentifier *Tools::UniformBoundaryAdapter::makeCracksIdentifier() const
{
	Tools::CracksIdentifier *identifier = new Tools::CracksIdentifier(this);

	identifier->setBoundary(this->getBoundary());

	return identifier;
}

void Tools::UniformBoundaryAdapter::identifyCracks()
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

Data::Vertex *Tools::UniformBoundaryAdapter::makeVertex(const Data::Point &pt) const
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

Data::/*Front*/Element *Tools::UniformBoundaryAdapter::makeElement(const Data::VertexVector &vertices) const
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
