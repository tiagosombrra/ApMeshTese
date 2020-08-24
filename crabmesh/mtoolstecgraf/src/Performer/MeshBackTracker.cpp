#include "Performer/MeshBackTracker.h"

#include "Data/Vertex.h"
#include "Data/Element.h"
#include "Data/FrontElement.h"
#include "Data/Mesh.h"

Performer::MeshBackTracker::MeshBackTracker(Data::Mesh *mesh, Data::Boundary *boundary,
	Data::Front *front, bool meshSorted) :
	MeshOptimizer(mesh)
{
	this->setBoundary(boundary);

	this->setFront(front);

	this->setMeshSorted(meshSorted);

	this->idManager = NULL;
}

Performer::MeshBackTracker::~MeshBackTracker()
{

}

void Performer::MeshBackTracker::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::MeshBackTracker::setFront(Data::Front *front)
{
	this->front = front;
}

void Performer::MeshBackTracker::setMeshSorted(bool meshSorted)
{
	this->meshSorted = meshSorted;
}

void Performer::MeshBackTracker::setIdManager(Performer::IdManager *idManager)
{
	this->idManager = idManager;
}

void Performer::MeshBackTracker::invert(Data::FrontElement *fel, Data::Element *e)
{
	//Data::VertexVector vertices = e->getVertices();

	//while (!vertices.empty())
	for (UInt i = 0; i < e->numVertices(); i++)
	{
		//Data::Vertex *v = vertices.back();
		Data::Vertex *v = e->getVertex(i);
		//vertices.pop_back();

		if (fel->have(v))
		{
			continue;
		}

		if (!fel->accordingToNormal(v))
		{
			fel->invert();
		}

		break;
	}
}

/*void Performer::MeshBackTracker::markAsDeleted(Data::VertexSet &deleted, Data::Vertex *v, Data::Mesh *mesh)
{
	pair<Data::VertexSet::iterator, bool> p = deleted.insert(v);

	if (p.second == true)
	{
		mesh->remove(v);
	}
}*/

Data::ElementList Performer::MeshBackTracker::sort() const
{
	if (!this->meshSorted)
	{
		//return this->mesh->getElements(true);
		//return this->mesh->getSortedElements();
		return this->mesh->getElements();
	}

	Data::ElementList sorted;

	//Data::ElementList els = this->mesh->getElements(true);
	//Data::ElementList els = this->mesh->getSortedElements();

	//while (!els.empty())
	for (Data::Mesh::ConstElementIterator iter = this->mesh->eBegin();
		 iter != this->mesh->eEnd(); iter++)
	{
		//Data::Element *e = els.front();
		Data::Element *e = (*iter);
		//els.pop_front();

		if (e->isBad())
		{
			this->insert(sorted, e);
		}
	}

	return sorted;
}
