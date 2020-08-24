#include "Performer/MeshRefiner.h"

Performer::MeshRefiner::MeshRefiner() :
	Builder()
{
	this->mesh = NULL;
}

Performer::MeshRefiner::~MeshRefiner()
{

}

void Performer::MeshRefiner::setIdManager(Performer::IdManager *idManager)
{
	this->idManager = idManager;
}

void Performer::MeshRefiner::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

Data::Mesh *Performer::MeshRefiner::getMesh() const
{
	return this->mesh;
}
