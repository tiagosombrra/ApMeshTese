#include "Performer/AdjacenciesIdentifier.h"

#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

Performer::AdjacenciesIdentifier::AdjacenciesIdentifier() :
	Performer::Builder::Builder()
{
	this->boundary = NULL;
	this->surface = NULL;
	this->dimension = 0;
	this->allowHangingVertices = false;
}

Performer::AdjacenciesIdentifier::~AdjacenciesIdentifier()
{

}

void Performer::AdjacenciesIdentifier::setBoundary(const Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::AdjacenciesIdentifier::setSurface(const Data::Mesh *surface)
{
    this->surface = surface;
}

void Performer::AdjacenciesIdentifier::setAllowHangingVertices(bool allowHangingVertices)
{
	this->allowHangingVertices = allowHangingVertices;
}

const Performer::AdjacenciesIdentifier::AdjacencyMap &Performer::AdjacenciesIdentifier::getAdjacencies() const
{
	return this->adjacencies;
}

void Performer::AdjacenciesIdentifier::getAdjacenciesSwapping(Performer::AdjacenciesIdentifier::AdjacencyMap &adjacencies)
{
	this->adjacencies.swap(adjacencies);
}

void Performer::AdjacenciesIdentifier::execute()
{
	this->adjacencies.clear();

	if ((!this->boundary) && (!this->surface))
	{
		return;
	}

#if USE_C__11
	this->adjacencies.reserve(this->boundary ? this->boundary->size() : this->surface->size());
#endif //#if USE_C__11

	this->dimension = this->boundary ? this->boundary->dimension() : this->surface->dimension();

	if (this->boundary)
    {
        for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin(), end = this->boundary->eEnd();
             iter != end; ++iter)
        {
            if (!this->identifyAdjacencies((*iter)))
            {
                this->adjacencies.clear();

                break;
            }
        }
    }
    else
    {
        for (Data::Mesh::ConstElementIterator iter = this->surface->eBegin(), end = this->surface->eEnd();
             iter != end; ++iter)
        {
            if (!this->identifyAdjacencies((*iter)))
            {
                this->adjacencies.clear();

                break;
            }
        }
    }
}

bool Performer::AdjacenciesIdentifier::identifyAdjacencies(Data::/*Front*/Element *fel)
{
	AdjacencyMap::iterator it = this->adjacencies.insert(std::make_pair(fel, VertexAdjacencyMap())).first;

	UInt numVertices = fel->numVertices();

#if USE_C__11
	(*it).second.reserve(numVertices);
#endif //#if USE_C__11

	for (UInt i = 0; i < numVertices; i++)
	{
		Data::Vertex *v = fel->getVertex(i);

		VertexAdjacencyMap::iterator it2 = (*it).second.find(v);

		if (it2 != (*it).second.end())
		{
			continue;
		}

		//Data::FrontElementList adjs = this->boundary->adjacency(v);
		Data::ElementList adjs;

		if (this->boundary)
        {
            Data::FrontElementList badjs = this->boundary->adjacency(v);

            while (!badjs.empty())
            {
                adjs.push_back(badjs.front());

                badjs.pop_front();
            }
        }
        else
        {
            adjs = this->surface->adjacency(v);
        }


		if (this->dimension == 2)
		{
			if (adjs.size() != 2)
			{
				if (this->allowHangingVertices)
				{
					(*it).second.insert(std::make_pair(v, static_cast<Data::/*Front*/Element *>(NULL)));

					continue;
				}

				return false;
			}

			Data::/*Front*/Element *adj = (adjs.front() == fel) ? adjs.back() : adjs.front();

			(*it).second.insert(std::make_pair(v, adj));

			this->adjacencies[adj][v] = fel;
		}
		else if (this->dimension == 3)
		{
			Data::Vertex *v2 = fel->getVertex((i + 1)%numVertices);

			Data::/*Front*/Element *adj = NULL;

			for (Data::/*Front*/ElementList::iterator iter2 = adjs.begin();
				 iter2 != adjs.end(); iter2++)
			{
				Data::/*Front*/Element *other = (*iter2);

				if ((other == fel) || (!other->have(v2)))
				{
					continue;
				}

				if ((adj) && (adj != other))
				{
					return false;
				}

				adj = other;
			}

			if (!adj)
			{
				if (this->allowHangingVertices)
				{
					(*it).second.insert(std::make_pair(v, static_cast<Data::/*Front*/Element *>(NULL)));

					continue;
				}

				return false;
			}

			(*it).second.insert(std::make_pair(v, adj));

			this->adjacencies[adj][v2] = fel;
		}
	}

	return true;
}
