#include "Performer/CracksIdentifier.h"

#include "Data/Point.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"

using namespace Performer;

Performer::CracksIdentifier::CracksIdentifier() :
	Performer::Builder::Builder()
{
	this->setBoundary(NULL);
	this->useTolerance = false;
	this->cracks = NULL;
}

Performer::CracksIdentifier::~CracksIdentifier()
{

}

void Performer::CracksIdentifier::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}


void Performer::CracksIdentifier::setUseTolerance(bool useTolerance)
{
	this->useTolerance = useTolerance;
}

Performer::CracksIdentifier::CrackMap *Performer::CracksIdentifier::getCracks() const
{
	return this->cracks;
}

bool Performer::CracksIdentifier::equal(const Data::Vertex *other, const Data::Vertex *vertex) const
{
	return ((other == vertex) ||
			 (other->getPoint() == vertex->getPoint()) ||
			 (this->useTolerance ? other->getPoint()->equal(*vertex->getPoint()) : other->getPoint()->match(*vertex->getPoint())));
}

bool Performer::CracksIdentifier::equal(const Data::FrontElement *other, const Data::FrontElement *fel) const
{
	if (other->numVertices() != fel->numVertices())
	{
		return false;
	}

	bool found[other->numVertices()];

	for (UInt i = 0; i < other->numVertices(); i++)
	{
		found[i] = false;
	}

	for (UInt i = 0; i < fel->numVertices(); i++)
	{
		Data::Vertex *fv = fel->getVertex(i);

		for (UInt j = 0; j < other->numVertices(); j++)
		{
			if (found[j])
			{
				continue;
			}

			Data::Vertex *ov = other->getVertex(j);

			if (this->equal(ov, fv))
			{
				found[j] = true;

				break;
			}
		}
	}

	for (UInt i = 0; i < other->numVertices(); i++)
	{
		if (!found[i])
		{
			return false;
		}
	}

	return true;
}

Data::FrontElement *Performer::CracksIdentifier::findCrackFel(
	Data::Boundary::FrontElementIterator iter,
	const Data::FrontElement *fel) const
{
	for (++iter; iter != this->boundary->eEnd(); ++iter)
	{
		Data::FrontElement *other = (*iter);

		if (this->equal(other, fel))
		{
			return other;
		}
	}

	return NULL;
}

void Performer::CracksIdentifier::checkForCrack(Data::Boundary::FrontElementIterator iter, Data::FrontElement *fel)
{
	if ((this->cracks) && (this->cracks->find(fel) != this->cracks->end()))
	{
		return;
	}

	Data::FrontElement *other = this->findCrackFel(iter, fel);

	if (!other)
	{
		return;
	}

	if (!this->cracks)
	{
		this->cracks = new CrackMap();
	}

	ECrack crack;

	crack.fel1 = fel;
	crack.fel2 = other;
	crack.data = NULL;

	(*this->cracks)[fel] = (*this->cracks)[other] = crack;
}

void Performer::CracksIdentifier::execute()
{
	if (!this->boundary)
	{
		return;
	}

	for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin();
		 iter != this->boundary->eEnd(); ++iter)
	{
		this->checkForCrack(iter, (*iter));
	}
}
