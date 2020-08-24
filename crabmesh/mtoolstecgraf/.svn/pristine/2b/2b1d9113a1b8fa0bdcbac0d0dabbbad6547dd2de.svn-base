#include "Performer/SizeBoundaryAdapter.h"

#include "Performer/IdManager.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"
#include "Data/FrontElement.h"
#include "Data/Vertex.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"

using namespace Performer;

Performer::SizeBoundaryAdapter::SizeBoundaryAdapter(Data::Boundary *boundary) :
	Performer::BoundaryAdapter::BoundaryAdapter(boundary)
{
	this->dimension = 0;
	this->maxAdaptations = 0;
	this->sizeProportion = 1.0;
	this->ok = true;
	this->adapted = false;
	this->mesh = NULL;
}

Performer::SizeBoundaryAdapter::SizeBoundaryAdapter(Data::Mesh *surface) :
	Performer::BoundaryAdapter::BoundaryAdapter(surface)
{
	this->dimension = 0;
	this->maxAdaptations = 0;
	this->sizeProportion = 1.0;
	this->ok = true;
	this->adapted = false;
	this->mesh = NULL;
}

Performer::SizeBoundaryAdapter::~SizeBoundaryAdapter()
{

}

void Performer::SizeBoundaryAdapter::setMaxAdaptations(UInt maxAdaptations)
{
	this->maxAdaptations = maxAdaptations;
}

void Performer::SizeBoundaryAdapter::setSizeProportion(Real sizeProportion)
{
	this->sizeProportion = sizeProportion;
}

void Performer::SizeBoundaryAdapter::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

bool Performer::SizeBoundaryAdapter::isOk() const
{
	return this->ok;
}

bool Performer::SizeBoundaryAdapter::isAdapted() const
{
	return this->adapted;
}

bool Performer::SizeBoundaryAdapter::adaptEdge2D(Data::/*Front*/Element *element, Data::Vertex *mid, Data::/*Front*/ElementList &newElements) const
{
	Data::Vertex *v = element->getVertex(1);
	element->setVertex(1, mid);

	Data::VertexVector vertices(2);
	vertices[0] = mid;
	vertices[1] = v;

	Data::/*Front*/Element *newElement = this->makeElement(vertices);

	newElements.push_back(newElement);

	Data::/*Front*/Element *adj = this->adjacencies[element][v];

	this->adjacencies[element].erase(v);
	this->adjacencies[element][mid] = newElement;

	this->adjacencies[newElement][mid] = element;
	this->adjacencies[newElement][v] = adj;

	this->adjacencies[adj][v] = newElement;

	return true;
}

bool Performer::SizeBoundaryAdapter::adaptTriangle3D(Data::/*Front*/Element *element, Data::Vertex *mid, Data::/*Front*/ElementList &newElements) const
{
	Data::Vertex *v0 = element->getVertex(0);
	Data::Vertex *v1 = element->getVertex(1);
	Data::Vertex *v2 = element->getVertex(2);

	element->setVertex(2, mid);

	Data::VertexVector vertices(3);
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = mid;

	Data::/*Front*/Element *ne1 = this->makeElement(vertices);
	newElements.push_back(ne1);

	vertices[0] = v2;
	vertices[1] = v0;
	vertices[2] = mid;

	Data::/*Front*/Element *ne2 = this->makeElement(vertices);
	newElements.push_back(ne2);

	Data::/*Front*/Element *adj1 = this->adjacencies[element][v1];
	Data::/*Front*/Element *adj2 = this->adjacencies[element][v2];

	this->adjacencies[element].erase(v2);
	this->adjacencies[element][v1] = ne1;
	this->adjacencies[element][mid] = ne2;

	this->adjacencies[ne1][v1] = adj1;
	this->adjacencies[ne1][v2] = ne2;
	this->adjacencies[ne1][mid] = element;

	this->adjacencies[ne2][v0] = element;
	this->adjacencies[ne2][mid] = ne1;
	this->adjacencies[ne2][v2] = adj2;

	if (adj1)
    {
        this->adjacencies[adj1][v2] = ne1;
    }

	if (adj2)
    {
        this->adjacencies[adj2][v0] = ne2;
    }

	return true;
}

bool Performer::SizeBoundaryAdapter::adapt(Data::/*Front*/Element *element, Data::/*Front*/ElementList &newElements, Data::VertexList &newVertices) const
{
	if (this->dimension == 2)
	{
#if USE_EXPERIMENTAL_3
		Data::Vertex *mid = this->makeVertex(element->getShape()->mid2D());
#else
		Data::Vertex *mid = this->makeVertex(element->getShape()->mid());
#endif //#if USE_EXPERIMENTAL_3

		newVertices.push_back(mid);

		this->adaptEdge2D(element, mid, newElements);

		return true;
	}

	if (this->dimension == 3)
	{
		if (element->numVertices() == 3)
		{
#if USE_EXPERIMENTAL_3
			Data::Vertex *mid = this->makeVertex(element->getShape()->mid3D());
#else
			Data::Vertex *mid = this->makeVertex(element->getShape()->mid());
#endif //#if USE_EXPERIMENTAL_3

			newVertices.push_back(mid);

			return this->adaptTriangle3D(element, mid, newElements);
		}
	}

	return false;
}

void Performer::SizeBoundaryAdapter::execute()
{
	if ((!this->getMesh()) || (this->getMesh()->size() == 0) || (this->sizeProportion <= 1.0))
	{
		return;
	}

	this->dimension = this->getMesh()->dimension();

	this->adapted = false;
	this->ok = true;

	if (!this->identifyAdjacencies())
	{
		this->adjacencies.clear();

		this->ok = false;

		return;
	}

	UInt numAdaptations = 0;

	do
	{
		this->toAdapt.clear();

		this->checkAdjacenciesSizes();

		if (this->toAdapt.empty())
		{
			break;
		}

		this->executeAdaptation();

		this->adapted = true;

		numAdaptations++;
	}
	while ((this->maxAdaptations == 0) || (numAdaptations < this->maxAdaptations));
}

AdjacenciesIdentifier *Performer::SizeBoundaryAdapter::makeAdjacenciesIdentifier() const
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

bool Performer::SizeBoundaryAdapter::identifyAdjacencies()
{
	AdjacenciesIdentifier *identifier = this->makeAdjacenciesIdentifier();

	identifier->execute();

	identifier->getAdjacenciesSwapping(this->adjacencies);

	delete identifier;

	return !this->adjacencies.empty();
}

void Performer::SizeBoundaryAdapter::checkAdjacenciesSizes(const AdjacenciesIdentifier::AdjacencyMap::iterator &iter)
{
	Data::/*Front*/Element *fel = (*iter).first;

	Real felSize = fel->getShape()->size();

	for (AdjacenciesIdentifier::VertexAdjacencyMap::iterator iter2 = (*iter).second.begin();
		 iter2 != (*iter).second.end(); iter2++)
	{
		Data::/*Front*/Element *other = (*iter2).second;

		Real otherSize = other->getShape()->size();

		if (felSize > this->sizeProportion*otherSize)
		{
			this->toAdapt.push_back(fel);

			break;
		}
	}
}

void Performer::SizeBoundaryAdapter::checkAdjacenciesSizes()
{
	for (AdjacenciesIdentifier::AdjacencyMap::iterator iter = this->adjacencies.begin();
		 iter != this->adjacencies.end(); iter++)
	{
		this->checkAdjacenciesSizes(iter);
	}
}

void Performer::SizeBoundaryAdapter::executeAdaptation()
{
	Data::/*Front*/ElementList newElements;
	Data::VertexList newVertices;

	for (Data::/*Front*/ElementList::iterator iter = this->toAdapt.begin();
		 iter != this->toAdapt.end(); iter++)
	{
		this->adapt((*iter), newElements, newVertices);
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
}

Data::Vertex *Performer::SizeBoundaryAdapter::makeVertex(const Data::Point &pt) const
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
