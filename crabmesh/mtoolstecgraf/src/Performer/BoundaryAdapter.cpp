#include "Performer/BoundaryAdapter.h"

#include "Data/Boundary.h"
#include "Data/Mesh.h"

using namespace Performer;

Performer::BoundaryAdapter::BoundaryAdapter(Data::Boundary *boundary) :
	Performer::Builder::Builder()
{
    this->boundary = true;
	this->setBoundary(boundary);

	this->idManager = NULL;
}

Performer::BoundaryAdapter::BoundaryAdapter(Data::Mesh *surface) :
	Performer::Builder::Builder()
{
    this->boundary = false;
	this->setSurface(surface);

	this->idManager = NULL;
}

Performer::BoundaryAdapter::~BoundaryAdapter()
{

}

void Performer::BoundaryAdapter::setBoundary(Data::Boundary *boundary)
{
	this->mesh = boundary;
	this->boundary = true;
}

Data::Boundary *Performer::BoundaryAdapter::getBoundary() const
{
	return static_cast<Data::Boundary *>(this->mesh);
}

void Performer::BoundaryAdapter::setSurface(Data::Mesh *surface)
{
	this->mesh = surface->isSurface() ? surface : NULL;
	this->boundary = false;
}

Data::Mesh *Performer::BoundaryAdapter::getSurface() const
{
	return static_cast<Data::Mesh *>(this->mesh);
}

Data::AbstractMesh *Performer::BoundaryAdapter::getMesh() const
{
    return this->mesh;
}

void Performer::BoundaryAdapter::setIdManager(IdManager *idManager)
{
	this->idManager = idManager;
}

IdManager *Performer::BoundaryAdapter::getIdManager() const
{
	return this->idManager;
}

bool Performer::BoundaryAdapter::isBoundary() const
{
    return this->boundary;
}

bool Performer::BoundaryAdapter::isSurface() const
{
    return !this->boundary;
}

void Performer::BoundaryAdapter::add(Data::Vertex *v) const
{
    if (this->boundary)
    {
        this->getBoundary()->add(v);
    }
    else
    {
        this->getSurface()->add(v, false);
    }
}

void Performer::BoundaryAdapter::add(Data::Element *e) const
{
    if (this->boundary)
    {
        this->getBoundary()->add(static_cast<Data::FrontElement *>(e));
    }
    else
    {
        this->getSurface()->add(e);
    }
}

void Performer::BoundaryAdapter::remove(Data::Vertex *v) const
{
    if (this->boundary)
    {
        this->getBoundary()->remove(v);
    }
    else
    {
        this->getSurface()->remove(v);
    }
}

void Performer::BoundaryAdapter::remove(Data::Element *e) const
{
    if (this->boundary)
    {
        this->getBoundary()->remove(static_cast<Data::FrontElement *>(e));
    }
    else
    {
        this->getSurface()->remove(e);
    }
}

Data::ElementList Performer::BoundaryAdapter::adjacency(const Data::Vertex *v) const
{
    if (this->isSurface())
    {
        return this->getSurface()->adjacency(v);
    }

    Data::ElementList adjs;

    Data::FrontElementList badjs = this->getBoundary()->adjacency(v);

    while (!badjs.empty())
    {
        adjs.push_back(badjs.front());

        badjs.pop_front();
    }

    return adjs;
}
