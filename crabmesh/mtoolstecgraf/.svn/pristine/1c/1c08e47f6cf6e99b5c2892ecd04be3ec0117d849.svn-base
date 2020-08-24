#include "TemplateMJMesh/MainDrive.h"

#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/GeometryFrontAdvancer.h"
#include "TemplateMJMesh/TemplateMeshGenerator.h"

using namespace TemplateMJMesh;

const Performer::Status TemplateMJMesh::MainDrive::templateMeshGenerated =
	Performer::Status(TemplateMJMesh::MainDrive::TEMPLATE_MESH_GENERATED, "template mesh generated");

TemplateMJMesh::MainDrive::MainDrive() :
	MJMesh::MainDrive::MainDrive()
{
	this->templateMeshGenerator = NULL;
}

TemplateMJMesh::MainDrive::~MainDrive()
{
	this->clear();
}

void TemplateMJMesh::MainDrive::execute()
{
	if ((this->status.getType() == MJMesh::MainDrive::success.getType()) ||
		(this->status.getType() >= MJMesh::MainDrive::error.getType()))
	{
		return;
	}

#if USE_TIME_MEASUREMENTS
	Real start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

	switch (this->status.getType())
	{
	case MJMesh::MainDrive::NOT_INIT:
		if (!this->executeInit())
		{
			break;
		}

	case MJMesh::MainDrive::INIT:
		//begin adjusting boundary

		if (!this->executeBoundaryBuilder(true))
		{
			break;
		}

		//end adjusting boundary
	case MJMesh::MainDrive::BOUNDARY_BUILT:
#if USE_NUMERICAL
        //begin hotelling transform
        if (!this->executeHotellingTransform())
		{
			break;
		}

#if USE_GUI
		if ((this->isHotellingTransformEnabled()) && (Data::isStepByStep())) break;
#endif //#if USE_GUI

        //end hotelling transform
    case MJMesh::MainDrive::HOTELLING_TRANSFORM_APPLIED:
#endif //#if USE_NUMERICAL
		//begin adjusting tree

		if (!this->executeTreeGenerator(true))
		{
			break;
		}

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::TREE_GENERATED:
		if (!this->executeTreeMinRefiner(true))
		{
			break;
		}

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::TREE_MIN_REFINED:
		if (!this->executeTreeNeighborRefiner(true))
		{
			break;
		}

		//end adjusting tree

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::TREE_NEIGHBOR_REFINED:
		//begin mesh generation

		if (!this->executeTemplateMeshGenerator())
		{
			break;
		}

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case TemplateMJMesh::MainDrive::TEMPLATE_MESH_GENERATED:
		if (!this->executeFrontGeometryAdvancer())
		{
			break;
		}

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::FRONT_GEOMETRY_ADVANCED:
		if (!this->executeFrontTopologyAdvancer())
		{
			break;
		}

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::FRONT_TOPOLOGY_ADVANCED:
		if (!this->executeFrontBackTrackingAdvancer())
		{
			break;
		}

		//end mesh generation

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::FRONT_BACK_TRACKING_ADVANCED:
		//begin improvement

		if (!this->executeMeshImprover())
		{
			break;
		}

		//end improvement

#if USE_GUI
		if ((this->status.getType() != MJMesh::MainDrive::meshImproved.getType()) &&
			(Data::isStepByStep()))
		{
			break;
		}
#endif //#if USE_GUI

	case MJMesh::MainDrive::MESH_IMPROVED:
#if USE_NUMERICAL
        //begin hotelling back transform
        if (!this->executeHotellingBackTransform())
		{
			break;
		}

#if USE_GUI
		if ((this->isHotellingTransformEnabled()) && (Data::isStepByStep())) break;
#endif //#if USE_GUI

        //end hotelling back transform
	case MJMesh::MainDrive::HOTELLING_BACK_TRANSFORM_APPLIED:
#endif //#if USE_NUMERICAL
#if USE_TIME_MEASUREMENTS
		{
			Real end = Data::time();
			std::cout << "Runtime = " << end - start << " seconds" << std::endl;
		}
#endif //#if USE_TIME_MEASUREMENTS

		if (!this->executeFinalize())
		{
			break;
		}
	}

#if USE_TIME_MEASUREMENTS
	Real end = Data::time();
	this->runtime += end - start;
#endif //#if USE_TIME_MEASUREMENTS
}

void TemplateMJMesh::MainDrive::clear()
{
	MJMesh::MainDrive::clear();

	if (this->templateMeshGenerator)
	{
		delete this->templateMeshGenerator;

		this->templateMeshGenerator = NULL;
	}
}

bool TemplateMJMesh::MainDrive::executeTemplateMeshGenerator()
{
	if (!this->templateMeshGenerator)
	{
		this->templateMeshGenerator = this->makeTemplateMeshGenerator();

		this->templateMeshGenerator->setIdManager(this->idManager);
	}

	this->templateMeshGenerator->execute();

	if (!this->front)
	{
		this->front = static_cast<MJMesh::Front *>(static_cast<TemplateMJMesh::TemplateMeshGenerator *>(this->templateMeshGenerator)->getFront());
	}

	if (!this->mesh)
	{
		this->mesh = static_cast<MJMesh::Mesh *>(this->templateMeshGenerator->getMesh());
	}

	if (this->templateMeshGenerator->getStatus().getType() == this->templateMeshGenerator->success.getType())
	{
		this->status = TemplateMJMesh::MainDrive::templateMeshGenerated;

		delete this->templateMeshGenerator;

		this->templateMeshGenerator = NULL;
	}
	else if (this->templateMeshGenerator->getStatus().getType() >= this->templateMeshGenerator->error.getType())
	{
		this->status = MJMesh::MainDrive::error;

		delete this->templateMeshGenerator;

		this->templateMeshGenerator = NULL;

		return false;
	}

	return true;
}

Performer::TemplateMeshGenerator *TemplateMJMesh::MainDrive::makeTemplateMeshGenerator() const
{
	TemplateMJMesh::TemplateMeshGenerator *templateMeshGenerator = new TemplateMJMesh::TemplateMeshGenerator();

	templateMeshGenerator->setBoundary(this->boundary);
	templateMeshGenerator->setTree(this->tree);

	return templateMeshGenerator;
}

MJMesh::GeometryFrontAdvancer *TemplateMJMesh::MainDrive::makeFrontGeometryAdvancer() const
{
	MJMesh::GeometryFrontAdvancer *geometryAdvancer = MJMesh::MainDrive::makeFrontGeometryAdvancer();

	geometryAdvancer->setMesh(this->mesh);

	if (this->usingSearchTrees)
	{
		Data::GeometricSearchTree *vstree = geometryAdvancer->makeVertexSearchTree(this->front);
		Data::GeometricSearchTree *festree = geometryAdvancer->makeFrontElementSearchTree(this->front);

		geometryAdvancer->setVSTree(vstree);
		geometryAdvancer->setFESTree(festree);

		this->front->setVSTree(vstree);
		this->front->setFESTree(festree);
	}

	geometryAdvancer->setFront(this->front);

	return geometryAdvancer;
}
