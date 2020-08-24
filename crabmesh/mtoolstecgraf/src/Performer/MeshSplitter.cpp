#include "Performer/MeshSplitter.h"

#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Element.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

using namespace Performer;

Performer::MeshSplitter::MeshSplitter()
{
	this->mesh = NULL;

	this->shouldMakeBoundaries = false;
	this->duplicate = true;
}

Performer::MeshSplitter::~MeshSplitter()
{

}

void Performer::MeshSplitter::setShouldMakeBoundaries(bool shouldMakeBoundaries)
{
	this->shouldMakeBoundaries = shouldMakeBoundaries;
}

void Performer::MeshSplitter::setDuplicate(bool duplicate)
{
	this->duplicate = duplicate;
}

bool Performer::MeshSplitter::isDuplicate() const
{
	return this->duplicate;
}

void Performer::MeshSplitter::setMesh(const Data::Mesh *mesh)
{
	this->mesh = mesh;
}

const Data::MeshList &Performer::MeshSplitter::getMeshes() const
{
	return this->meshes;
}

const Data::BoundaryList &Performer::MeshSplitter::getBoundaries() const
{
	return this->boundaries;
}

void Performer::MeshSplitter::execute()
{
	if ((!this->mesh) || (this->mesh->verticesSize() == 0))
	{
		return;
	}

	this->selected = this->mesh->vBegin();

	while (this->numVerticesInMeshes() != this->mesh->verticesSize())
	{
		Data::Vertex *v = this->select();

		if (!v)
		{
			break;
		}

		Data::Mesh *mesh = this->makeMesh();

		this->meshes.push_back(mesh);

		this->split(v, mesh);

		this->selected++;
	}

	if (this->meshes.size() == 1)
	{
		this->meshes.front()->clear();

		delete this->meshes.front();

		this->meshes.pop_front();

		return;
	}

	this->copy();
}

ULInt Performer::MeshSplitter::numVerticesInMeshes() const
{
	ULInt count = 0;

	for (Data::MeshList::const_iterator iter = this->meshes.begin();
		 iter != this->meshes.end(); iter++)
	{
		count += (*iter)->verticesSize();
	}

	return count;
}

bool Performer::MeshSplitter::inAnyMesh(const Data::Vertex *v) const
{
	for (Data::MeshList::const_iterator iter = this->meshes.begin();
		 iter != this->meshes.end(); iter++)
	{
		if ((*iter)->have(v))
		{
			return true;
		}
	}

	return false;
}

bool Performer::MeshSplitter::inAnyMesh(const Data::FrontElement *fe) const
{
	for (Data::MeshList::const_iterator iter = this->meshes.begin();
		 iter != this->meshes.end(); iter++)
	{
		if ((*iter)->have(fe))
		{
			return true;
		}
	}

	return false;
}

bool Performer::MeshSplitter::inAnyMesh(const Data::Element *e) const
{
	for (Data::MeshList::const_iterator iter = this->meshes.begin();
		 iter != this->meshes.end(); iter++)
	{
		if ((*iter)->have(e))
		{
			return true;
		}
	}

	return false;
}

Data::Vertex *Performer::MeshSplitter::select() const
{
	for (; this->selected != this->mesh->vEnd(); this->selected++)
	{
		Data::Vertex *v = (*this->selected);

		if (!this->inAnyMesh(v))
		{
			return v;
		}
	}

	return NULL;
}

void Performer::MeshSplitter::addToVertices(Data::Vertex *v, Data::Mesh *mesh, Data::VertexList &vertices)
{
	if (mesh->have(v))
	{
		return;
	}

	mesh->add(v);

	Data::ElementList eadjs = this->mesh->adjacency(v);

	for (Data::ElementList::iterator iter = eadjs.begin();
		 iter != eadjs.end(); iter++)
	{
		Data::Element *e = (*iter);

		if (mesh->have(e))
		{
			continue;
		}

		mesh->add(e);

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			if (e->getVertex(i) == v)
			{
				continue;
			}

			vertices.push_back(e->getVertex(i));
		}
	}

	Data::FrontElementList fadjs = this->mesh->findAdjacentFrontElements(v);

	for (Data::FrontElementList::iterator iter = fadjs.begin();
		 iter != fadjs.end(); iter++)
	{
		Data::FrontElement *fe = (*iter);

		if (mesh->have(fe))
		{
			continue;
		}

		mesh->add(fe);

		for (UInt i = 0; i < fe->numVertices(); i++)
		{
			if (fe->getVertex(i) == v)
			{
				continue;
			}

			vertices.push_back(fe->getVertex(i));
		}
	}
}

void Performer::MeshSplitter::split(Data::Vertex *v, Data::Mesh *mesh)
{
	Data::VertexList vertices;

	vertices.push_back(v);

	while (!vertices.empty())
	{
		Data::Vertex *v = vertices.front();

		vertices.pop_front();

		this->addToVertices(v, mesh, vertices);
	}
}

void Performer::MeshSplitter::copy()
{
#if USE_GUI
	Int i = 0;
#endif //#if USE_GUI

	for (Data::MeshList::iterator iter = this->meshes.begin();
		 iter != this->meshes.end(); iter++)
	{
#if USE_GUI
		Data::pallete(i++, this->r, this->g, this->b);
#endif //#if USE_GUI

		this->copy((*iter));
	}
}

void Performer::MeshSplitter::copy(Data::Mesh *mesh)
{
	if (this->duplicate)
	{
		VMap vMap;
		FEMap feMap;
		EMap eMap;

		VSet vInBoundary;
		FESet feInBoundary;

#if USE_C__11
		vMap.reserve(mesh->verticesSize());
		feMap.reserve(mesh->frontElementsSize());
		eMap.reserve(mesh->size());
#endif //#if USE_C__11

		for (Data::Mesh::VertexIterator iter = mesh->vBegin();
			 iter != mesh->vEnd(); iter = mesh->erase(iter))
		{
			Data::Vertex *v = this->makeVertex((*iter), vMap);

#if USE_GUI
			v->setColor(this->r, this->g, this->b);
#endif //#if USE_GUI

			vMap[v->getId()] = v;

			bool inner = this->mesh->haveInner((*iter));

			if (!inner)
			{
				vInBoundary.insert(v);
			}
		}

		for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
			 iter != mesh->feEnd(); iter = mesh->erase(iter))
		{
			Data::FrontElement *fe = this->makeFrontElement((*iter), vMap, feMap);

#if USE_GUI
			fe->setColor(this->r, this->g, this->b);
#endif //#if USE_GUI

			feMap[fe->getId()] = fe;

			if ((this->shouldMakeBoundaries) && ((this->mesh->adjacency((*iter)).size() <= 1)))
			{
				feInBoundary.insert(fe);
			}
		}

		for (Data::Mesh::ElementIterator iter = mesh->eBegin();
			 iter != mesh->eEnd(); iter = mesh->erase(iter))
		{
			Data::Element *e = this->makeElement((*iter), vMap, feMap, eMap);

#if USE_GUI
			e->setColor(this->r, this->g, this->b);
#endif //#if USE_GUI

			eMap[e->getId()] = e;
		}

		Data::Boundary *boundary = NULL;

		if (this->shouldMakeBoundaries)
		{
			boundary = this->makeBoundary();

			this->boundaries.push_back(boundary);
		}

		for (VMap::iterator iter = vMap.begin();
			 iter != vMap.end(); iter++)
		{
			Data::Vertex *v = (*iter).second;

			bool inner = (vInBoundary.find(v) == vInBoundary.end());

			mesh->add(v, inner);

			if ((boundary) && (!inner))
			{
				boundary->add(v);
			}
		}

		for (FEMap::iterator iter = feMap.begin();
			 iter != feMap.end(); iter++)
		{
			Data::FrontElement *fe = (*iter).second;

			mesh->add(fe);

			if ((boundary) && (feInBoundary.find(fe) != feInBoundary.end()))
			{
				boundary->add(fe);
			}
		}

		for (EMap::iterator iter = eMap.begin();
			 iter != eMap.end(); iter++)
		{
			mesh->add((*iter).second);
		}

		//vInBoundary.clear();
		//feInBoundary.clear();
	}
	else
	{
		if (this->shouldMakeBoundaries)
		{
			Data::Boundary *boundary = NULL;

			boundary = this->makeBoundary();

			this->boundaries.push_back(boundary);

			for (Data::Mesh::VertexIterator iter = mesh->vBegin();
				 iter != mesh->vEnd(); iter++)
			{
				Data::Vertex *v = (*iter);

#if USE_GUI
				v->setColor(this->r, this->g, this->b);
#endif //#if USE_GUI

				if (this->mesh->haveInner(v))
				{
					boundary->add(v);
				}
			}

			for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
				 iter != mesh->feEnd(); iter++)
			{
				Data::FrontElement *fe = (*iter);

#if USE_GUI
				fe->setColor(this->r, this->g, this->b);
#endif //#if USE_GUI

				if (this->mesh->adjacency((*iter)).size() <= 1)
				{
					boundary->add(fe);
				}
			}
		}
#if USE_GUI
		else
		{
			for (Data::Mesh::VertexIterator iter = mesh->vBegin();
				 iter != mesh->vEnd(); iter++)
			{
				(*iter)->setColor(this->r, this->g, this->b);
			}

			for (Data::Mesh::FrontElementIterator iter = mesh->feBegin();
				 iter != mesh->feEnd(); iter = mesh->erase(iter))
			{
				(*iter)->setColor(this->r, this->g, this->b);
			}
		}

		for (Data::Mesh::ElementIterator iter = mesh->eBegin();
			 iter != mesh->eEnd(); iter++)
		{
			(*iter)->setColor(this->r, this->g, this->b);
		}
#endif //#if USE_GUI
	}
}
