#include "Performer/EdgeBoundaryAdapter.h"

#include "Data/Point3D.h"
#include "Data/Vector3D.h"
#include "Data/Triangle3D.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"
#include "Performer/IdManager.h"

using namespace Performer;

Performer::EdgeBoundaryAdapter::EdgeBoundaryAdapter(Data::Boundary *boundary) :
	Performer::BoundaryAdapter::BoundaryAdapter(boundary)
{
	this->dimension = 0;
	this->sizeProportion = 4.0;
	this->qualityThreshold = 0.2;
	this->ok = true;
	this->adapted = false;
	this->mesh = NULL;
}

Performer::EdgeBoundaryAdapter::EdgeBoundaryAdapter(Data::Mesh *surface) :
	Performer::BoundaryAdapter::BoundaryAdapter(surface)
{
	this->dimension = 0;
	this->sizeProportion = 4.0;
	this->qualityThreshold = 0.2;
	this->ok = true;
	this->adapted = false;
	this->mesh = NULL;
}

Performer::EdgeBoundaryAdapter::~EdgeBoundaryAdapter()
{

}

void Performer::EdgeBoundaryAdapter::setSizeProportion(Real sizeProportion)
{
	this->sizeProportion = sizeProportion;
}

void Performer::EdgeBoundaryAdapter::setQualityThreshold(Real qualityThreshold)
{
	this->qualityThreshold = qualityThreshold;
}

void Performer::EdgeBoundaryAdapter::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

bool Performer::EdgeBoundaryAdapter::isOk() const
{
	return this->ok;
}

bool Performer::EdgeBoundaryAdapter::isAdapted() const
{
	return this->adapted;
}

void Performer::EdgeBoundaryAdapter::execute()
{
	if ((!this->getMesh()) || (this->getMesh()->size() == 0) || (this->sizeProportion <= 1.0))
	{
		return;
	}

	this->dimension = this->getMesh()->dimension();

	if (/*(this->dimension != 2) && */(this->dimension != 3))
	{
		return;
	}

	this->adapted = false;
	this->ok = true;

	if (!this->identifyAdjacencies())
	{
		this->adjacencies.clear();
		this->elements.clear();
		this->refinements.clear();

		this->ok = false;

		return;
	}

	this->identifyQualities();

	this->identifyRefinements();

	this->executeAdaptation();
}

bool Performer::EdgeBoundaryAdapter::adaptTriangle3D(Data::/*Front*/Element *element,
	Data::Vertex *vs[3], Data::Vertex *mids[3], Data::/*Front*/ElementList &newElements, Data::VertexList &/*newVertices*/) const
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
		return false;
	}

	if (count == 3)
	{
		element->setVertex(1, mids[0]);
		element->setVertex(2, mids[2]);

		Data::VertexVector vertices(3);
		vertices[0] = mids[0];
		vertices[1] = vs[1];
		vertices[2] = mids[1];

		Data::/*Front*/Element *newElement = this->makeElement(vertices);
		newElements.push_back(newElement);

		vertices[0] = mids[2];
		vertices[1] = mids[1];
		vertices[2] = vs[2];

		newElement = this->makeElement(vertices);
		newElements.push_back(newElement);

		vertices[0] = mids[0];
		vertices[1] = mids[1];
		vertices[2] = mids[2];

		newElement = this->makeElement(vertices);
		newElements.push_back(newElement);

		return true;
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
		element->setVertex((dif+1)%3, mids[dif]);

		Data::VertexVector vertices(3);

		vertices[0] = mids[dif];
		vertices[1] = vs[(dif+1)%3];
		vertices[2] = vs[(dif+2)%3];

		Data::/*Front*/Element *newElement = this->makeElement(vertices);
		newElements.push_back(newElement);

		return true;
	}

	if (count == 2)
	{
		element->setVertex(dif, mids[(dif+2)%3]);
		element->setVertex((dif+1)%3, mids[(dif+1)%3]);

		if (vs[(dif+2)%3]->getPoint()->distance(vs[dif]->getPoint()) >= vs[(dif+2)%3]->getPoint()->distance(vs[(dif+1)%3]->getPoint()))
		{
			Data::VertexVector vertices(3);

			vertices[0] = vs[dif];
			vertices[1] = vs[(dif+1)%3];
			vertices[2] = mids[(dif+2)%3];

			Data::/*Front*/Element *newElement = this->makeElement(vertices);
			newElements.push_back(newElement);

			vertices[0] = vs[(dif+1)%3];
			vertices[1] = mids[(dif+1)%3];
			vertices[2] = mids[(dif+2)%3];

			newElement = this->makeElement(vertices);
			newElements.push_back(newElement);
		}
		else
		{
			Data::VertexVector vertices(3);

			vertices[0] = vs[dif];
			vertices[1] = vs[(dif+1)%3];
			vertices[2] = mids[(dif+1)%3];

			Data::/*Front*/Element *newElement = this->makeElement(vertices);
			newElements.push_back(newElement);

			vertices[0] = vs[dif];
			vertices[1] = mids[(dif+1)%3];
			vertices[2] = mids[(dif+2)%3];

			newElement = this->makeElement(vertices);
			newElements.push_back(newElement);
		}

		return true;
	}

	return false;
}

bool Performer::EdgeBoundaryAdapter::adapt(Data::/*Front*/Element *element,
	Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const
{
	/*if (this->dimension == 2)
	{
		Data::Vertex *mid = this->makeVertex(element->getShape()->mid());

		newVertices.push_back(mid);

		this->adaptEdge2D(element, mid, newElements);

		return true;
	}*/

	if (this->dimension == 3)
	{
		if (element->numVertices() == 3)
		{
#if USE_C__11
			EBoolHashMap::const_iterator iter = this->refinements.find(element);
#else
			EBoolMap::const_iterator iter = this->refinements.find(element);
#endif //#if USE_C__11

			if (iter == this->refinements.end())
			{
				return false;
			}

			Data::Vertex *vs[3];

			for (UInt i = 0; i < 3; i++)
			{
				vs[i] = element->getVertex(i);
			}

			Data::Vertex *mids[3];

			for (UInt i = 0; i < 3; i++)
			{
				if (!(*iter).second[i])
				{
					mids[i] = NULL;

					continue;
				}

				ULInt id1 = vs[i]->getId();
				ULInt id2 = vs[(i+1)%3]->getId();

				mids[i] = this->mids[id1][id2];

				if (!mids[i])
				{
					Data::Vector3D vec(vs[i]->getPoint(), vs[(i+1)%3]->getPoint());
					vec.multiply(0.5);

#if USE_EXPERIMENTAL_3
					Data::Point3D pt = *vs[i]->getPoint();
#else
					Data::Point pt = *vs[i]->getPoint();
#endif //#if USE_EXPERIMENTAL_3
					pt.sum(vec);

					this->mids[id2][id1] = mids[i] = this->makeVertex(pt);

					newVertices.push_back(mids[i]);
				}
			}

			return this->adaptTriangle3D(element, vs, mids, newElements, newVertices);
		}
	}

	return false;
}

Data::Vertex *Performer::EdgeBoundaryAdapter::makeVertex(const Data::Point &pt) const
{
	if (this->dimension == 2)
	{
		return new Data::Vertex(new Data::Point2D(pt));
	}

	if (this->dimension == 3)
	{
		return new Data::Vertex(new Data::Point3D(pt));
	}

#if USE_EXPERIMENTAL_3
	return NULL;
#else
	return new Data::Vertex(new Data::Point(pt));
#endif //#if USE_EXPERIMENTAL_3
}

Real Performer::EdgeBoundaryAdapter::quality(const Data::Triangle *t) const
{
	Real a = t->getP1()->distance(t->getP2());
	Real b = t->getP2()->distance(t->getP3());
	Real c = t->getP3()->distance(t->getP1());

	Real s = 0.5*(a + b + c);
	Real d = (s - a)*(s - b)*(s - c);

	Real rInsc = std::sqrt(d/s);
	Real rCirc = 0.25*a*b*c/sqrt(s*d);

	return 2.0*rInsc/rCirc;
}

bool Performer::EdgeBoundaryAdapter::shouldRefine(const Data::/*Front*/Element *fel)
{
	UInt numPts = fel->numVertices();

	Real sizes[numPts];
	Real minsize = REAL_MAX;

	for (UInt i = 0; i < numPts; i++)
	{
		sizes[i] = fel->getVertex(i)->getPoint()->distance(fel->getVertex((i+1)%numPts)->getPoint());

		if (sizes[i] < minsize)
		{
			minsize = sizes[i];
		}
	}

	Data::BoolVector refine(numPts);
	bool any = false;

	Real threshold = minsize*this->sizeProportion;

	for (UInt i = 0; i < numPts; i++)
	{
		refine[i] = (sizes[i] > threshold);

		any = (any || refine[i]);
	}

	if (!any)
	{
		return false;
	}

#if USE_C__11
	std::pair<EBoolHashMap::iterator, bool> p = this->refinements.insert(std::make_pair(const_cast<Data::/*Front*/Element *>(fel), refine));
#else
	std::pair<EBoolMap::iterator, bool> p = this->refinements.insert(std::make_pair(const_cast<Data::/*Front*/Element *>(fel), refine));
#endif //#if USE_C__11

	for (UInt i = 0; i < numPts; i++)
	{
		if (!p.second)
		{
			(*p.first).second[i] = refine[i] = (((*p.first).second[i]) || (refine[i]));
		}

		if (!refine[i])
		{
			continue;
		}

		Data::Vertex *n = fel->getVertex((i+1)%numPts);
		Data::/*Front*/Element *adj = this->adjacencies[const_cast<Data::/*Front*/Element *>(fel)][fel->getVertex(i)];

		if (!adj)
        {
            continue;
        }

		UInt j = numPts;

		for (j = 0; j < numPts; j++)
		{
			if (adj->getVertex(j) == n)
			{
				break;
			}
		}

		Data::BoolVector adjref(adj->numVertices(), false);

		adjref[j] = true;

#if USE_C__11
		std::pair<EBoolHashMap::iterator, bool> p2 = this->refinements.insert(std::make_pair(adj, adjref));
#else
		std::pair<EBoolMap::iterator, bool> p2 = this->refinements.insert(std::make_pair(adj, adjref));
#endif //#if USE_C__11

		if (!p2.second)
		{
			(*p2.first).second[j] = true;
		}
	}

	return true;
}

void Performer::EdgeBoundaryAdapter::removeFromElements(const Data::/*Front*/Element *fel)
{
	this->refinements.erase(const_cast<Data::/*Front*/Element *>(fel));

	Real q = this->quality(static_cast<Data::Triangle *>(fel->getShape()));

	if (q > this->qualityThreshold)
	{
		return;
	}

	this->elements.remove(const_cast<Data::/*Front*/Element *>(fel));
}

AdjacenciesIdentifier *Performer::EdgeBoundaryAdapter::makeAdjacenciesIdentifier() const
{
	AdjacenciesIdentifier *identifier = new AdjacenciesIdentifier();

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

bool Performer::EdgeBoundaryAdapter::identifyAdjacencies()
{
	AdjacenciesIdentifier *identifier = this->makeAdjacenciesIdentifier();

	identifier->execute();

	identifier->getAdjacenciesSwapping(this->adjacencies);

	delete identifier;

	return !this->adjacencies.empty();
}

bool Performer::EdgeBoundaryAdapter::addToElements(Data::/*Front*/Element *fel) const
{
	Real q = this->quality(static_cast<Data::Triangle *>(fel->getShape()));

	if (q > this->qualityThreshold)
	{
		return false;
	}

	this->elements.push_back(fel);

	return true;
}

void Performer::EdgeBoundaryAdapter::identifyQualities()
{
    if (this->isBoundary())
    {
        for (Data::Boundary::ConstFrontElementIterator iter = this->getBoundary()->eBegin(), end = this->getBoundary()->eEnd();
             iter != end; iter++)
        {
            this->addToElements((*iter));
        }
    }
    else
    {
        for (Data::Mesh::ConstElementIterator iter = this->getSurface()->eBegin(), end = this->getSurface()->eEnd();
             iter != end; iter++)
        {
            this->addToElements((*iter));
        }
    }
}

void Performer::EdgeBoundaryAdapter::identifyRefinements()
{
	while (!this->elements.empty())
	{
		Data::/*Front*/Element *fel = this->elements.front();

		this->elements.pop_front();

		this->shouldRefine(fel);
	}
}

void Performer::EdgeBoundaryAdapter::executeAdaptation()
{
	Data::/*Front*/ElementList newElements;
	Data::VertexList newVertices;

#if USE_C__11
	this->mids.reserve(this->getMesh()->verticesSize());
#endif //#if USE_C__11

	while (!this->refinements.empty())
	{
#if USE_C__11
		EBoolHashMap::iterator iter = this->refinements.begin();
#else
		EBoolMap::iterator iter = this->refinements.begin();
#endif //#if USE_C__11

		Data::/*Front*/Element *fel = (*iter).first;

		if (this->adapt(fel, newElements, newVertices))
		{
			this->adapted = true;
		}

		this->refinements.erase(iter);
	}

	UInt n = this->isBoundary() ? 1 : 2;

	while (!newElements.empty())
	{
		Data::/*Front*/Element *element = newElements.front();

		newElements.pop_front();

		element->setId(this->idManager->next(n));

		this->add(element);

		if (this->mesh)
		{
			this->mesh->add(static_cast<Data::FrontElement *>(element));
		}
	}

	while (!newVertices.empty())
	{
		Data::Vertex *vertex = newVertices.front();

		newVertices.pop_front();

		vertex->setId(this->idManager->next(0));

		this->add(vertex);

		if (this->mesh)
		{
			this->mesh->add(vertex, false);
		}
	}

	this->mids.clear();
}
