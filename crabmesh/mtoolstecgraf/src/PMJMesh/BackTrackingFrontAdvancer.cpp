#include "PMJMesh/BackTrackingFrontAdvancer.h"

#include "MJMesh/FrontElement.h"
#include "MJMesh/Boundary.h"

PMJMesh::BackTrackingFrontAdvancer::BackTrackingFrontAdvancer(UInt dimension,
	MJMesh::Boundary *boundary, Data::BoxTree *tree, MJMesh::Mesh *mesh,
	MJMesh::Front *front, bool adjustFront, bool deleteElements, UInt maxBackTracks,
	enum MJMesh::BackTrackingFrontAdvancer::Mode mode) :
	MJMesh::BackTrackingFrontAdvancer(dimension, boundary, tree, mesh, front,
		adjustFront, deleteElements, maxBackTracks, mode)
{

}

PMJMesh::BackTrackingFrontAdvancer::~BackTrackingFrontAdvancer()
{

}

void PMJMesh::BackTrackingFrontAdvancer::invert()
{
	for (Data::FrontElementList::iterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		if (!this->boundary->have((*iter)))
		{
			static_cast<MJMesh::FrontElement *>((*iter))->invert();
		}
	}
}

void PMJMesh::BackTrackingFrontAdvancer::deleteOldMesh()
{
	this->invert();

	MJMesh::BackTrackingFrontAdvancer::deleteOldMesh();
}

void PMJMesh::BackTrackingFrontAdvancer::restoreOldMesh()
{
	this->invert();

	MJMesh::BackTrackingFrontAdvancer::restoreOldMesh();
}
