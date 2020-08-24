#include "Performer/CrackRemover.h"

#include "Data/Mesh.h"

using namespace Performer;

Performer::CrackRemover::CrackRemover() :
	Performer::Builder()
{
	this->setBoundary(NULL);
	this->setMesh(NULL);
	this->anyCrackRemoved = false;
	this->useTolerance = false;
	this->cracks = NULL;
	this->identifier = NULL;
}

Performer::CrackRemover::~CrackRemover()
{
	if (this->cracks)
	{
		delete this->cracks;
	}

	if (this->identifier)
	{
		delete this->identifier;
	}
}

void Performer::CrackRemover::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::CrackRemover::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

void Performer::CrackRemover::setUseTolerance(bool useTolerance)
{
	this->useTolerance = useTolerance;
}

bool Performer::CrackRemover::isAnyCrackRemoved() const
{
	return this->anyCrackRemoved;
}

bool Performer::CrackRemover::isCrackFel(const Data::FrontElement *fel) const
{
	return (this->cracks->find(const_cast<Data::FrontElement *>(fel)) != this->cracks->end());
}

void Performer::CrackRemover::findAdjacentsNotInCrack(Data::FrontElementList &adjs) const
{
	for (Data::FrontElementList::iterator iter = adjs.begin();
		 iter != adjs.end();)
	{
		Data::FrontElement *adj = (*iter);

		if (this->isCrackFel(adj))
		{
			iter = adjs.erase(iter);

			continue;
		}

		++iter;
	}
}

void Performer::CrackRemover::replace(Data::FrontElement *fel, Data::Vertex *oldV, Data::Vertex *newV) const
{
	fel->replace(oldV, newV);
}

void Performer::CrackRemover::replace(Data::Element *e, Data::Vertex *oldV, Data::Vertex *newV) const
{
	e->replace(oldV, newV);
}

CracksIdentifier *Performer::CrackRemover::makeCracksIdentifier() const
{
	CracksIdentifier *identifier = new CracksIdentifier();

	identifier->setBoundary(this->boundary);
	identifier->setUseTolerance(this->useTolerance);

	return identifier;
}

void Performer::CrackRemover::execute()
{
	if (!this->boundary)
	{
		return;
	}

	this->anyCrackRemoved = false;

	this->identifier = this->makeCracksIdentifier();

	this->identifier->execute();

	this->cracks = identifier->getCracks();

	if (!this->cracks)
	{
		delete this->identifier;

		this->identifier = NULL;

		return;
	}

	this->anyCrackRemoved = true;

	this->removeCracks();

	delete this->cracks;

	this->cracks = NULL;

	delete this->identifier;

	this->identifier = NULL;
}

void Performer::CrackRemover::removeCrack(CracksIdentifier::CrackMap::iterator &iter)
{
	if ((*iter).second.data != NULL)
	{
		return;
	}

	Data::/*Front*/Element *fel = (*iter).second.fel1;
	Data::/*Front*/Element *other = (*iter).second.fel2;

	(*this->cracks)[(fel == (*iter).first) ? other : fel].data =
		(*iter).second.data = this->cracks; //just to make data != NULL

	this->boundary->remove(static_cast<Data::FrontElement *>(fel));
	this->boundary->remove(static_cast<Data::FrontElement *>(other));

	if (this->mesh)
	{
		this->mesh->remove(static_cast<Data::FrontElement *>(fel));
		this->mesh->remove(static_cast<Data::FrontElement *>(other));
	}

	for (UInt i = 0; i < fel->numVertices(); i++)
	{
		Data::Vertex *fv = fel->getVertex(i);

		Data::Vertex *ov = NULL;

		for (UInt j = 0; j < other->numVertices(); j++)
		{
			if ((other->getVertex(j)) && (this->identifier->equal(other->getVertex(j), fv)))
			{
				ov = other->getVertex(j);

				break;
			}
		}

		Data::FrontElementList adjs = this->boundary->adjacency(ov);

		this->findAdjacentsNotInCrack(adjs);

		if (adjs.empty())
		{
			continue;
		}

		fel->setVertex(i, NULL);

		if (fv == ov)
		{
			this->replace(other, ov, NULL);

			continue;
		}

		for (Data::FrontElementList::iterator iter2 = adjs.begin();
			 iter2 != adjs.end(); iter2++)
		{
			this->replace((*iter2), ov, fv);
		}

		if (this->mesh)
		{
			Data::FrontElementList madjs = this->mesh->findAdjacentFrontElements(ov);

			this->findAdjacentsNotInCrack(adjs);

			for (Data::FrontElementList::iterator iter2 = madjs.begin();
				 iter2 != madjs.end(); iter2++)
			{
				this->replace((*iter2), ov, fv);
			}

			Data::ElementList eadjs = this->mesh->adjacency(ov);

			for (Data::ElementList::iterator iter2 = eadjs.begin();
				 iter2 != eadjs.end(); iter2++)
			{
				this->replace((*iter2), ov, fv);
			}
		}
	}
}

void Performer::CrackRemover::removeCracks()
{
	for (CracksIdentifier::CrackMap::iterator iter = this->cracks->begin();
		 iter != this->cracks->end(); iter++)
	{
		this->removeCrack(iter);
	}

#if USE_C__11
	Data::VertexHash vertices;
#else
	Data::VertexSet vertices;
#endif //#if USE_C__11

	while (!this->cracks->empty())
	{
		CracksIdentifier::CrackMap::iterator it = this->cracks->begin();

		ECrack crack = (*it).second;

		Data::/*Front*/Element *fel = crack.fel1;
		Data::/*Front*/Element *other = crack.fel2;

		this->cracks->erase(((*it).first == fel) ? other : fel);
		this->cracks->erase(it);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			if (fel->getVertex(i))
			{
				vertices.insert(fel->getVertex(i));
			}

			if (other->getVertex(i))
			{
				vertices.insert(other->getVertex(i));
			}
		}

		delete fel;
		delete other;
	}

#if USE_C__11
	for (Data::VertexHash::iterator iter = vertices.begin();
#else
	for (Data::VertexSet::iterator iter = vertices.begin();
#endif //#if USE_C__11
		 iter != vertices.end(); ++iter)
	{
		Data::Vertex *v = (*iter);

		this->boundary->remove(v);

		if (this->mesh)
		{
			this->mesh->remove(v);
		}

		delete v;
	}
}
