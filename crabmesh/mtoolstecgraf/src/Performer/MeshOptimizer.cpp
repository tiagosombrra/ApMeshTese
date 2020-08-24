#include "Performer/MeshOptimizer.h"

const Performer::Status Performer::MeshOptimizer::notInit =
	Performer::Status(MeshOptimizer::NOT_INIT, "not initiated");
const Performer::Status Performer::MeshOptimizer::init =
	Performer::Status(MeshOptimizer::INIT, "initiated");
const Performer::Status Performer::MeshOptimizer::ended =
	Performer::Status(MeshOptimizer::ENDED, "ended");

Performer::MeshOptimizer::MeshOptimizer(Data::Mesh *mesh) :
	Builder()
{
	this->setMesh(mesh);

	this->status = MeshOptimizer::notInit;
}

Performer::MeshOptimizer::~MeshOptimizer()
{

}

void Performer::MeshOptimizer::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

Data::Mesh *Performer::MeshOptimizer::getMesh() const
{
	return this->mesh;
}
