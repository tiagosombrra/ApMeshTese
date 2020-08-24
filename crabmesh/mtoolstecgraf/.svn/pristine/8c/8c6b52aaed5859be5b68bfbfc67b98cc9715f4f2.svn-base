#include "PMJMesh/BoundaryBuilder.h"

#include "PMJMesh/Communicator.h"
#include "PMJMesh/BoundaryTransferable.h"
#include "Performer/IdManager.h"

#include "MJMesh/Boundary.h"

PMJMesh::BoundaryBuilder::BoundaryBuilder(Parallel::Communicator *comm) :
	MJMesh::BoundaryBuilder(false),
	Parallel::Builder(comm)
{
	this->setDimension(0);
}

PMJMesh::BoundaryBuilder::~BoundaryBuilder()
{

}

void PMJMesh::BoundaryBuilder::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::BoundaryBuilder::execute()
{
	if (this->comm->isSerial())
	{
		MJMesh::BoundaryBuilder::execute();

		return;
	}

	PMJMesh::BoundaryTransferable *transferable = NULL;

	if (this->comm->isMaster())
	{
		MJMesh::BoundaryBuilder::execute();

		transferable = new PMJMesh::BoundaryTransferable(this->dimension, this->getBoundary());

		this->comm->broadcast(transferable);
	}
	else
	{
	    transferable = static_cast<PMJMesh::BoundaryTransferable *>(this->comm->broadcast(this->comm->root()));

		if (this->boundary)
		{
		    delete this->boundary;
		}

		this->boundary = transferable->getBoundary();

		//non global shift
		//this->boundary->clearMaps();

		this->boundary->setSorted(this->sorted);

		if (this->idManager)
		{
		    delete this->idManager;
		}

		this->idManager = transferable->getIdManager();

		transferable->setIdManager(NULL);
	}

	delete transferable;
}
