#include "PMTools2/WorkerMainDrive.h"

#if USE_MTOOLS

#include "Data/OStream.h"

#include "Data/BoxTree.h"
#include "Parallel/Communicator.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/MeshImprover.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/WorkerMeshImprover.h"
#include "PMJMesh2/AuxiliaryFrontGenerator.h"
#include "PMTools2/MeshGenerator.h"

using namespace PMTools2;

PMTools2::WorkerMainDrive::WorkerMainDrive(Parallel::Communicator *comm, UInt status) :
    PMTools::WorkerMainDrive::WorkerMainDrive(comm, status)
{
    this->createInnerFrontInDecomposition = false;
    this->createCrackInInnerFront = false;
    this->displaceVerticesInInnerFront = false;
    this->fixateFront = true;
    this->shouldAdvanceFront = true;

    this->useUniformSize = true;

    this->frontGenerator = NULL;
}

PMTools2::WorkerMainDrive::~WorkerMainDrive()
{
    if (this->frontGenerator)
    {
        delete this->frontGenerator;

        this->frontGenerator = NULL;
    }
}

void PMTools2::WorkerMainDrive::setCreateInnerFrontInDecomposition(bool createInnerFrontInDecomposition)
{
    this->createInnerFrontInDecomposition = createInnerFrontInDecomposition;
}

void PMTools2::WorkerMainDrive::setCreateCrackInInnerFront(bool createCrackInInnerFront)
{
    this->createCrackInInnerFront = createCrackInInnerFront;
}

void PMTools2::WorkerMainDrive::setDisplaceVerticesInInnerFront(bool displaceVerticesInInnerFront)
{
    this->displaceVerticesInInnerFront = displaceVerticesInInnerFront;
}

void PMTools2::WorkerMainDrive::setFixateFront(bool fixateFront)
{
    this->fixateFront = fixateFront;
}

void PMTools2::WorkerMainDrive::setShouldAdvanceFront(bool shouldAdvanceFront)
{
    this->shouldAdvanceFront = shouldAdvanceFront;
}

void PMTools2::WorkerMainDrive::setUseUniformSize(bool useUniformSize)
{
    this->useUniformSize = useUniformSize;
}

PMTools::MeshGenerator *PMTools2::WorkerMainDrive::makeMeshGenerator() const
{
    return new PMTools2::MeshGenerator();
}

void PMTools2::WorkerMainDrive::executeFrontGenerator()
{
    if ((this->PMJMesh::WorkerMainDrive::front->size() > 0) || (!this->createInnerFrontInDecomposition))
    {
        return;
    }

    if (!this->frontGenerator)
    {
        this->frontGenerator = this->makeFrontGenerator();
    }

    this->frontGenerator->execute();
}

void PMTools2::WorkerMainDrive::executeFrontMerger()
{
    if (!this->frontGenerator)
    {
        return;
    }

    this->frontGenerator->execute();

    delete this->frontGenerator;

    this->frontGenerator = NULL;
}

bool PMTools2::WorkerMainDrive::executeMeshGenerator()
{
    /*if (!this->shouldAdvanceFront)
    {
        return true;
    }*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real tstart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	/*if (this->MJMesh::MainDrive::mesh)
	{
		this->MJMesh::MainDrive::mesh->clear();
	}
	else*/
    if (!this->MJMesh::MainDrive::mesh)
	{
		this->MJMesh::MainDrive::mesh = new MJMesh::Mesh();
	}

	this->MTools::MainDrive::mesh = this->MJMesh::MainDrive::mesh;

#if USE_NEW_FRONT_UPDATER
	if (!this->oldFront)
    {
        this->oldFront = new PMJMesh::Front(this->PMJMesh::WorkerMainDrive::front->isSorted());

        this->oldFront->set(this->PMJMesh::WorkerMainDrive::front);
    }
#else
	this->oldFront = this->PMJMesh::WorkerMainDrive::front->getAllElements();
	this->newFront.clear();
	this->commonFront.clear();
#endif //#if USE_NEW_FRONT_UPDATER

    this->executeFrontGenerator();

	PMTools::MeshGenerator *meshGenerator = this->makeMeshGenerator();

	//set input data
	meshGenerator->setWorkerTask(true);

	if (!this->shouldAdvanceFront)
    {
        meshGenerator->setWorkerTask(false);
    }

	meshGenerator->setEnableBackTracking(this->enableBackTracking);
	meshGenerator->setBoundary(static_cast<const MJMesh::Boundary *>(this->PMJMesh::WorkerMainDrive::boundary));
	meshGenerator->setFront(this->PMJMesh::WorkerMainDrive::front);
	meshGenerator->setMesh(this->MJMesh::MainDrive::mesh);

	meshGenerator->setUseBoxTest(this->testingBox);

	//if (this->box)
	if (this->testingBox)
	{
		//meshGenerator->setUseBoxTest(true);
		meshGenerator->setBox(*this->box);
		meshGenerator->setTreeBox(*static_cast<Data::Box *>(this->PMJMesh::WorkerMainDrive::tree->getShape()));
	}

	meshGenerator->setRangeProportion(this->rangeProportion);

	meshGenerator->setFtree(this->ftree);

	static_cast<PMTools2::MeshGenerator *>(meshGenerator)->setUseUniformRefinement(this->MJMesh::MainDrive::usingUniformRefinement);
    static_cast<PMTools2::MeshGenerator *>(meshGenerator)->setUseUniformSize(this->useUniformSize);
    meshGenerator->setDeleteFels(true);

	meshGenerator->setIdManager(this->idManager);

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeGeometry = Data::time() - tstart;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

#if USE_TIME_MEASUREMENTS
	Real start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

	meshGenerator->execute();

#if USE_TIME_MEASUREMENTS
	Real end = Data::time();
	this->PMJMesh::WorkerMainDrive::runtime += end - start;
#endif //#if USE_TIME_MEASUREMENTS

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeGeometry += meshGenerator->timeMeshGeneration;
	this->timeImprover += meshGenerator->timeMeshImprovement;
	this->timeOtherStuff += meshGenerator->timeOtherStuff;
	this->timeToMToolsConversion = meshGenerator->timeToMToolsConversion;
	this->timeFromMToolsConversion = meshGenerator->timeFromMToolsConversion;

	tstart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	/*//debug
	Data::cout() << "proc " << this->comm->rank() << " boundary->debug before executeFrontMerger, " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
	this->PMJMesh::WorkerMainDrive::boundary->debug();
	//endebug*/

	this->executeFrontMerger();

	/*//debug
	Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executeFrontMerger, " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
	this->PMJMesh::WorkerMainDrive::boundary->debug();
	//endebug*/

	bool ok = (meshGenerator->getStatus().getType() == meshGenerator->success.getType());

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeGeometry += Data::time() - tstart;

	this->timeBackTracking = 0.0;

	tstart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	if (ok)
	{
#if (!USE_NEW_FRONT_UPDATER)
		this->newFront = this->PMJMesh::WorkerMainDrive::front->getAllElements();
#endif //#if (!USE_NEW_FRONT_UPDATER)

		this->findCommonFront();

		/*//debug
		Data::cout() << "proc " << this->comm->rank() << " boundary->debug after findCommonFront, " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
		this->PMJMesh::WorkerMainDrive::boundary->debug();
		//endebug*/
	}
	else
	{
#if USE_NEW_FRONT_UPDATER
		this->commonFront = static_cast<PMJMesh::Front *>(this->PMJMesh::WorkerMainDrive::front);

		this->PMJMesh::WorkerMainDrive::front = NULL;

		this->oldFront->clear();

		delete this->oldFront;

		this->oldFront = NULL;
#else
		this->commonFront.swap(this->oldFront);
#endif //#if USE_NEW_FRONT_UPDATER

		this->PMJMesh::WorkerMainDrive::status = this->error;

		this->PMJMesh::WorkerMainDrive::status.append(meshGenerator->getStatus().getMessage());
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeBackTracking += Data::time() - tstart;

	tstart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	/*//debug
	Data::cout() << "proc " << this->comm->rank() << " boundary->debug before delete meshGenerator, " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
	this->PMJMesh::WorkerMainDrive::boundary->debug();
	//endebug*/

	delete meshGenerator;

	/*//debug
	Data::cout() << "proc " << this->comm->rank() << " boundary->debug after delete meshGenerator, " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
	this->PMJMesh::WorkerMainDrive::boundary->debug();
	//endebug*/

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeGeometry += Data::time() - tstart;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return ok;
}

PMJMesh2::AuxiliaryFrontGenerator *PMTools2::WorkerMainDrive::makeFrontGenerator()
{
    PMJMesh2::AuxiliaryFrontGenerator *generator = new PMJMesh2::AuxiliaryFrontGenerator();

    generator->setDimension(this->dimension);
    generator->setCreateCrackInInnerFront(this->createCrackInInnerFront);
    generator->setDisplaceVerticesInInnerFront(this->displaceVerticesInInnerFront);
    generator->setRangeProportion(this->rangeProportion);

    generator->setBox(this->box);
    generator->setTree(this->PMJMesh::WorkerMainDrive::tree);
    generator->setFront(static_cast<PMJMesh::Front *>(this->PMJMesh::WorkerMainDrive::front));

#if USE_DELETE_FELS
    generator->setOldFront(static_cast<PMJMesh::Front *>(this->oldFront));
#endif //#if USE_DELETE_FELS

    generator->setIdManager(this->idManager);

    return generator;
}

#endif //#if USE_MTOOLS
