#include "Performer/BoundaryCreator.h"

#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Element.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"
#include "Performer/IdManager.h"

using namespace Performer;

Performer::BoundaryCreator::BoundaryCreator(Data::Mesh *mesh) :
	Performer::Builder::Builder()
{
	this->setMesh(mesh);
	this->setIdManager(NULL);
	this->boundary = NULL;
	this->onlyFind = false;
	this->onlyVertices = false;
}

Performer::BoundaryCreator::~BoundaryCreator()
{

}

void Performer::BoundaryCreator::setIdManager(Performer::IdManager *idManager)
{
	this->idManager = idManager;
}

IdManager *Performer::BoundaryCreator::getIdManager() const
{
	return this->idManager;
}

void Performer::BoundaryCreator::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

void Performer::BoundaryCreator::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}

Data::Boundary *Performer::BoundaryCreator::getBoundary() const
{
	return this->boundary;
}

void Performer::BoundaryCreator::setOnlyVertices(bool onlyVertices)
{
    this->onlyVertices = onlyVertices;
}

void Performer::BoundaryCreator::execute()
{
	if (!this->mesh)
	{
		return;
	}

	if ((this->boundary) && (this->boundary->size() != 0) && (this->mesh->frontElementsSize() != 0))
    {
        return;
    }

    this->onlyFind = false;

    if ((this->boundary) && (this->boundary->size() == 0) && (this->mesh->frontElementsSize() != 0))
    {
        this->onlyFind = true;
    }

	UInt dimension = this->mesh->dimension();

	if (!this->boundary)
	{
		this->boundary = this->makeBoundary();
	}

	if (dimension == 2)
	{
		this->execute2D();
	}
	else if (dimension == 3)
	{
		this->execute3D();
	}
}

UInt Performer::BoundaryCreator::numFaces(UInt numVertices) const
{
	if (numVertices == 4)
	{
		return 4;
	}

	if (numVertices == 5)
    {
        return 5;
    }

    if (numVertices == 6)
    {
        return 5;
    }

	if (numVertices == 8)
	{
		return 6;
	}

	return 0;
}

void Performer::BoundaryCreator::face(UInt i, UInt numVertices, Data::UIntVector &vertices) const
{
	if (numVertices == 4)
	{
		vertices.resize(3);

		if (i == 0)
		{
			vertices[0] = 0;
			vertices[1] = 1;
			vertices[2] = 2;

			return;
		}

		if (i == 1)
		{
			vertices[0] = 0;
			vertices[1] = 3;
			vertices[2] = 1;

			return;
		}

		if (i == 2)
		{
			vertices[0] = 1;
			vertices[1] = 3;
			vertices[2] = 2;

			return;
		}

		if (i == 3)
		{
			vertices[0] = 2;
			vertices[1] = 3;
			vertices[2] = 0;

			return;
		}

		return;
	}

	if (numVertices == 5)
    {
        if (i == 0)
        {
            vertices.resize(4);
        }
        else
        {
            vertices.resize(3);
        }

        if (i == 0)
        {
            vertices[0] = 0;
            vertices[1] = 1;
            vertices[2] = 2;
            vertices[3] = 3;

            return;
        }

        if (i == 1)
        {
            vertices[0] = 0;
            vertices[1] = 4;
            vertices[2] = 1;

            return;
        }

        if (i == 2)
        {
            vertices[0] = 1;
            vertices[1] = 4;
            vertices[2] = 2;

            return;
        }

        if (i == 3)
        {
            vertices[0] = 2;
            vertices[1] = 4;
            vertices[2] = 3;

            return;
        }

        if (i == 4)
        {
            vertices[0] = 3;
            vertices[1] = 4;
            vertices[2] = 0;

            return;
        }
    }

    if (numVertices == 6)
    {
        if ((i == 0) || (i == 1))
        {
            vertices.resize(3);
        }
        else
        {
            vertices.resize(4);
        }

        if (i == 0)
        {
            vertices[0] = 0;
            vertices[1] = 1;
            vertices[2] = 2;

            return;
        }

        if (i == 1)
        {
            vertices[0] = 3;
            vertices[1] = 5;
            vertices[2] = 4;

            return;
        }

        if (i == 2)
        {
            vertices[0] = 0;
            vertices[1] = 3;
            vertices[2] = 4;
            vertices[3] = 1;

            return;
        }

        if (i == 3)
        {
            vertices[0] = 1;
            vertices[1] = 4;
            vertices[2] = 5;
            vertices[3] = 2;

            return;
        }

        if (i == 4)
        {
            vertices[0] = 2;
            vertices[1] = 5;
            vertices[2] = 3;
            vertices[3] = 0;

            return;
        }
    }

	if (numVertices == 8)
	{
		vertices.resize(4);

		if (i == 0)
		{
			vertices[0] = 0;
			vertices[1] = 1;
			vertices[2] = 2;
			vertices[3] = 3;

			return;
		}

		if (i == 1)
		{
			vertices[0] = 4;
			vertices[1] = 7;
			vertices[2] = 6;
			vertices[3] = 5;

			return;
		}

		if (i == 2)
		{
			vertices[0] = 0;
			vertices[1] = 3;
			vertices[2] = 7;
			vertices[3] = 4;

			return;
		}

		if (i == 3)
		{
			vertices[0] = 1;
			vertices[1] = 5;
			vertices[2] = 6;
			vertices[3] = 2;

			return;
		}

		if (i == 4)
		{
			vertices[0] = 0;
			vertices[1] = 4;
			vertices[2] = 5;
			vertices[3] = 1;

			return;
		}

		if (i == 5)
		{
			vertices[0] = 2;
			vertices[1] = 6;
			vertices[2] = 7;
			vertices[3] = 3;

			return;
		}

		return;
	}

	vertices.clear();
}

Data::FrontElement *Performer::BoundaryCreator::makeOrFindFrontElement(const Data::VertexVector &vertices, Data::Element *e) const
{
    if (this->onlyFind)
    {
        for (Data::VertexVector::const_iterator iter = vertices.begin();
             iter != vertices.end(); iter++)
        {
            Data::Vertex *v = (*iter);

            Data::FrontElementList adjs = this->mesh->findAdjacentFrontElements(v);

            while (!adjs.empty())
            {
                Data::FrontElement *adj = adjs.front();

                adjs.pop_front();

                bool foundAll = true;

                for (Data::VertexVector::const_iterator iter2 = vertices.begin();
                     iter2 != vertices.end(); iter2++)
                {
                    if (iter == iter2)
                    {
                        continue;
                    }

                    if (!adj->have((*iter2)))
                    {
                        foundAll = false;

                        break;
                    }
                }

                if (foundAll)
                {
                    return adj;
                }
            }
        }
    }

    return this->makeFrontElement(vertices, e);
}

void Performer::BoundaryCreator::execute2D(Data::Element *e, DoneList2D &done)
{
	UInt numVertices = e->numVertices();

	for (UInt i = 0; i < numVertices; i++)
	{
		Data::Vertex *v = e->getVertex(i);
		Data::Vertex *n = e->getVertex((i+1)%numVertices);

		if (done[v].find(n) != done[v].end())
		{
			continue;
		}

		done[v].insert(n);
		done[n].insert(v);

		bool found = false;

		Data::ElementList adjs = this->mesh->adjacency(v);

		for (Data::ElementList::iterator iter2 = adjs.begin();
			 iter2 != adjs.end(); iter2++)
		{
			Data::Element *adj = (*iter2);

			if (adj == e)
			{
				continue;
			}

			if (adj->have(n))
			{
				found = true;

				break;
			}
		}

		if (found)
		{
			continue;
		}

		Data::VertexVector vertices(2, static_cast<Data::Vertex *>(NULL));

		vertices[0] = v;
		vertices[1] = n;

		if (!this->onlyVertices)
        {
            Data::FrontElement *fel = this->makeOrFindFrontElement(vertices, e);

            if (!this->mesh->have(fel))
            {
                fel->setId(this->idManager->next());

                this->mesh->add(fel);
            }

            this->boundary->add(fel);
        }

		if (!this->boundary->have(v))
		{
			this->boundary->add(v);

			if (this->mesh->haveInner(v))
            {
                this->mesh->removeInner(v);
            }
		}

		if (!this->boundary->have(n))
		{
			this->boundary->add(n);

			if (this->mesh->haveInner(n))
            {
                this->mesh->removeInner(n);
            }
		}
	}
}

void Performer::BoundaryCreator::execute2D()
{
	DoneList2D done;

#if USE_C__11
	done.reserve(this->mesh->verticesSize());
#endif //#if USE_C__11

	for (Data::Mesh::ConstElementIterator iter = this->mesh->eBegin();
		 iter != this->mesh->eEnd(); iter++)
	{
		this->execute2D((*iter), done);
	}
}

void Performer::BoundaryCreator::execute3D(Data::Element *e, DoneList3D &done)
{
	UInt numVertices = e->numVertices();

	UInt numFaces = this->numFaces(numVertices);

	for (UInt i = 0; i < numFaces; i++)
	{
		Data::UIntVector faceVertices;

		this->face(i, numVertices, faceVertices);

		Data::VertexVector vertices(faceVertices.size(), NULL);

		for (UInt i = 0; i < vertices.size(); i++)
		{
			vertices[i] = e->getVertex(faceVertices[i]);
		}

		Data::VertexVector aux(vertices);

		std::sort(aux.begin(), aux.end(), Data::Identifiable::less);

		if (!done.insert(aux).second)
		{
			continue;
		}

		bool found = false;

		Data::ElementList adjs = this->mesh->adjacency(vertices[0]);

		for (Data::ElementList::iterator iter2 = adjs.begin();
			 iter2 != adjs.end(); iter2++)
		{
			Data::Element *adj = (*iter2);

			if (adj == e)
			{
				continue;
			}

			bool haveAll = true;

			for (UInt i = 1; i < vertices.size(); i++)
			{
				if (!adj->have(vertices[i]))
				{
					haveAll = false;

					break;
				}
			}

			if (haveAll)
			{
				found = true;

				break;
			}
		}

		if (found)
		{
			continue;
		}

		if (!this->onlyVertices)
        {
            Data::FrontElement *fel = this->makeOrFindFrontElement(vertices, e);

            if (!this->mesh->have(fel))
            {
                fel->setId(this->idManager->next());

                this->mesh->add(fel);
            }

            this->boundary->add(fel);
        }

		for (UInt i = 0; i < vertices.size(); i++)
		{
			if (!this->boundary->have(vertices[i]))
			{
				this->boundary->add(vertices[i]);

				if (this->mesh->haveInner(vertices[i]))
                {
                    this->mesh->removeInner(vertices[i]);
                }
			}
		}
	}
}

void Performer::BoundaryCreator::execute3D()
{
	DoneList3D done;

#if USE_C__11
	done.reserve(this->mesh->verticesSize());
#endif //#if USE_C__11

	for (Data::Mesh::ConstElementIterator iter = this->mesh->eBegin();
		 iter != this->mesh->eEnd(); iter++)
	{
		this->execute3D((*iter), done);
	}
}
