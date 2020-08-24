#include "PJmesh/BackTrackingFrontAdvancer.h"

#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "PJmesh/MeshGenerator.h"

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
#include <sstream>
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

PJmesh::BackTrackingFrontAdvancer::BackTrackingFrontAdvancer(
	const MJMesh::Boundary *boundary, MJMesh::Front *front, MJMesh::Mesh *mesh) :
		MJMesh::BackTrackingFrontAdvancer(3, boundary, NULL, mesh, front, true, true, 0, MJMesh::BackTrackingFrontAdvancer::FRONT_ADVANCING)
{

}

PJmesh::BackTrackingFrontAdvancer::~BackTrackingFrontAdvancer()
{

}

void PJmesh::BackTrackingFrontAdvancer::execute()
{
	if (this->status.getType() != notInit.getType())
	{
		return;
	}

	if (this->front->size(0) + this->front->size(1) + this->front->size(2) + this->front->size(3) == 0)
	{
		this->deleteNonBoundaryFels();

		this->status = ended;

		return;
	}

	PJmesh::MeshGenerator *meshGenerator = new PJmesh::MeshGenerator();

	meshGenerator->setIdManager(this->idManager);

	meshGenerator->setBoundary(static_cast<const MJMesh::Boundary *>(this->boundary));
	meshGenerator->setFront(static_cast<MJMesh::Front *>(this->front));
	meshGenerator->setMesh(this->mesh);

	meshGenerator->execute();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	std::stringstream str;
	str << "Time for back tracking = " << meshGenerator->timeMeshGeneration << "\n";
	str << "Time for conversion = " << meshGenerator->timeToJmeshConversion << " and " << meshGenerator->timeFromJmeshConversion << "\n";
	std::cout << str.str() << std::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->status = (meshGenerator->getJmeshStatus() == 0) ? this->rejected : this->ended;

	delete meshGenerator;
}

void PJmesh::BackTrackingFrontAdvancer::deleteNonBoundaryFels()
{
	for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin(), end = this->mesh->feEnd();
		 iter != end;)
	{
		Data::FrontElement *fel = (*iter);

		if (this->boundary->have(fel))
		{
			++iter;

			continue;
		}

		iter = this->mesh->erase(iter);

		delete fel;
	}
}
