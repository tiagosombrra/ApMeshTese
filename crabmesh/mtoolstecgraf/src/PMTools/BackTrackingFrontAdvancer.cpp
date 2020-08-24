#include "PMTools/BackTrackingFrontAdvancer.h"

#if USE_MTOOLS

#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "PMTools/MeshGenerator.h"

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
#include <sstream>
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

PMTools::BackTrackingFrontAdvancer::BackTrackingFrontAdvancer(
	const FTools::OctTree *ftree, const MJMesh::Boundary *boundary,
	MJMesh::Front *front, MJMesh::Mesh *mesh) :
		MJMesh::BackTrackingFrontAdvancer(3, boundary, NULL, mesh, front, true, true, 0, MJMesh::BackTrackingFrontAdvancer::FRONT_ADVANCING)
{
	this->ftree = ftree;
#if DEBUG_TIMING_PMTOOLS
	this->timeBackTracking = this->timeToMToolsConversion =
		this->timeFromMToolsConversion = 0.0;
#endif //#if DEBUG_TIMING_PMTOOLS
}

PMTools::BackTrackingFrontAdvancer::~BackTrackingFrontAdvancer()
{

}

void PMTools::BackTrackingFrontAdvancer::execute()
{
	if (this->status.getType() != notInit.getType())
	{
		return;
	}

	if (this->front->size(0) + this->front->size(1) + this->front->size(2) + this->front->size(3) == 0)
	{
#if DEBUG_TIMING_PMTOOLS
		Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

		this->deleteNonBoundaryFels();

		this->status = this->ended;

#if DEBUG_TIMING_PMTOOLS
		this->timeBackTracking = Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMTOOLS

		return;
	}

	PMTools::MeshGenerator *meshGenerator = new PMTools::MeshGenerator();

	meshGenerator->setIdManager(this->idManager);

	meshGenerator->setFtree(this->ftree);

	meshGenerator->setBoundary(static_cast<const MJMesh::Boundary *>(this->boundary));
	meshGenerator->setFront(static_cast<MJMesh::Front *>(this->front));
	meshGenerator->setMesh(this->mesh);
	meshGenerator->setEnableBackTracking(true);

	meshGenerator->execute();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
#if DEBUG_TIMING_PMTOOLS
	this->timeBackTracking = meshGenerator->timeMeshGeneration;
	this->timeToMToolsConversion = meshGenerator->timeToMToolsConversion;
	this->timeFromMToolsConversion = meshGenerator->timeFromMToolsConversion;
#endif //#if DEBUG_TIMING_PMTOOLS

	std::stringstream str;
	str << "Time for back tracking = " << meshGenerator->timeMeshGeneration << "\n";
	str << "Time for conversion = " << meshGenerator->timeToMToolsConversion << " and " << meshGenerator->timeFromMToolsConversion << "\n";
	std::cout << str.str() << std::endl;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->status = (meshGenerator->getStatus().getType() == meshGenerator->error.getType()) ? this->rejected : this->ended;

	delete meshGenerator;
}

void PMTools::BackTrackingFrontAdvancer::deleteNonBoundaryFels()
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

#endif //#if USE_MTOOLS
