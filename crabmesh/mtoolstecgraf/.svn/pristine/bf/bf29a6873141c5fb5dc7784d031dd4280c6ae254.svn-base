#include "PMJMesh/MeshBackTracker.h"

#include "PMJMesh/BackTrackingFrontAdvancer.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"

PMJMesh::MeshBackTracker::MeshBackTracker(UInt dimension, Data::Mesh *mesh,
	Data::Boundary *boundary, Data::BoxTree *tree, Data::Front *front,
	bool meshSorted, UInt maxBackTracks) :
	MJMesh::MeshBackTracker(dimension, mesh, boundary, tree, front,
		meshSorted, maxBackTracks)
{

}

PMJMesh::MeshBackTracker::~MeshBackTracker()
{

}

MJMesh::BackTrackingFrontAdvancer *PMJMesh::MeshBackTracker::makeBackTrackingFrontAdvancer() const
{
	PMJMesh::BackTrackingFrontAdvancer *btfa = new PMJMesh::BackTrackingFrontAdvancer(
		this->dimension, const_cast<MJMesh::Boundary *>(static_cast<const MJMesh::Boundary *>(this->boundary)),
		this->tree, const_cast<MJMesh::Mesh *>(static_cast<const MJMesh::Mesh *>(this->mesh)),
		const_cast<MJMesh::Front *>(static_cast<const MJMesh::Front *>(this->front)),
		false, false, this->maxBackTracks, MJMesh::BackTrackingFrontAdvancer::OPTIMIZATION);

	btfa->setIdManager(this->idManager);

	return btfa;
}

void PMJMesh::MeshBackTracker::clearBtfa()
{
	delete this->btfa;

	this->btfa = NULL;
}
