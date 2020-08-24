#include "PMTools/WorkerMainDrive.h"

#if USE_MTOOLS

#include "Data/OStream.h"
#include "MJMesh/Boundary.h"

#include "Data/BoxTree.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "Parallel/Communicator.h"
#include "PMTools/MeshGenerator.h"

#if USE_NEW_FRONT_UPDATER
#include "PMJMesh/Front.h"
#endif //#if USE_NEW_FRONT_UPDATER

#if USE_THREAD_COLOR
#include <typeinfo>
#endif //#if USE_THREAD_COLOR

PMTools::WorkerMainDrive::WorkerMainDrive(Parallel::Communicator *comm, UInt status) :
	PMJMesh::WorkerMainDrive(comm, status),
	MTools::MainDrive()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeOtherStuff = 0.0;
	this->timeToMToolsConversion = 0.0;
	this->timeFromMToolsConversion = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->enableBackTracking = true;
	this->ftree = NULL;
}

PMTools::WorkerMainDrive::~WorkerMainDrive()
{
	if ((this->MTools::MainDrive::mesh) &&
		(this->PMJMesh::WorkerMainDrive::mesh) &&
		(this->MTools::MainDrive::mesh == this->PMJMesh::WorkerMainDrive::mesh))
	{
		this->MTools::MainDrive::mesh = NULL;
	}
}

void PMTools::WorkerMainDrive::setMesh(MJMesh::Mesh *mesh)
{
	this->MTools::MainDrive::mesh = this->MJMesh::MainDrive::mesh = mesh;
}

void PMTools::WorkerMainDrive::setEnableBackTracking(bool enableBackTracking)
{
	this->enableBackTracking = enableBackTracking;
}

void PMTools::WorkerMainDrive::setFtree(const FTools::OctTree *ftree)
{
	this->ftree = ftree;
}

void PMTools::WorkerMainDrive::setUsingUniformRefinement(bool usingUniformRefinement)
{
    this->PMJMesh::WorkerMainDrive::setUsingUniformRefinement(usingUniformRefinement);
    this->MTools::MainDrive::setUsingUniformRefinement(usingUniformRefinement);
}

bool PMTools::WorkerMainDrive::loadFile()
{
	return MTools::MainDrive::loadFile();
}

bool PMTools::WorkerMainDrive::saveFile() const
{
	return MTools::MainDrive::saveFile();
}

bool PMTools::WorkerMainDrive::loadInputFile(const std::string &inputFileName)
{
	return MTools::MainDrive::loadInputFile(inputFileName);
}

bool PMTools::WorkerMainDrive::saveInputFile(const std::string &inputFileName) const
{
	return MTools::MainDrive::saveInputFile(inputFileName);
}

bool PMTools::WorkerMainDrive::loadOutputFile(const std::string &outputFileName)
{
	return MTools::MainDrive::loadOutputFile(outputFileName);
}

bool PMTools::WorkerMainDrive::saveOutputFile(const std::string &outputFileName) const
{
	return MTools::MainDrive::saveOutputFile(outputFileName);
}

PMTools::MeshGenerator *PMTools::WorkerMainDrive::makeMeshGenerator() const
{
    return new PMTools::MeshGenerator();
}

void PMTools::WorkerMainDrive::execute()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeRefinement = 0.0;
	this->timeTrees = 0.0;
	this->timeGeometry = 0.0;
	this->timeTopology = 0.0;
	this->timeBackTracking = 0.0;
	this->timeClassify = 0.0;
	this->timeImprover = 0.0;
	this->timeOtherStuff = 0.0;
	this->timeToMToolsConversion = 0.0;
	this->timeFromMToolsConversion = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->PMJMesh::WorkerMainDrive::status = this->init;

	if (!this->executeMeshGenerator())
	{
		/*//debug
		Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executeMeshGenerator (false), " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
		this->PMJMesh::WorkerMainDrive::boundary->debug();
		//endebug*/

#if USE_THREAD_COLOR
		if (typeid(this->idManager) == typeid(Performer::SequentialIdManager *))
		{
			this->colorizeMesh(0);
		}
		else
		{
			this->colorizeMesh();
		}
#endif //#if USE_THREAD_COLOR

		return;
	}

	/*//debug
	Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executeMeshGenerator (true), " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
	this->PMJMesh::WorkerMainDrive::boundary->debug();
	//endebug*/

	if (!this->executeTreeMinRefiner(true))
	{
		/*//debug
		Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executeTreeMinRefiner (false), " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
		this->PMJMesh::WorkerMainDrive::boundary->debug();
		//endebug*/

#if USE_THREAD_COLOR
		if (typeid(this->idManager) == typeid(Performer::SequentialIdManager *))
		{
			this->colorizeMesh(0);
		}
		else
		{
			this->colorizeMesh();
		}
#endif //#if USE_THREAD_COLOR

		return;
	}

	/*//debug
	Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executeTreeMinRefiner (true), " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
	this->PMJMesh::WorkerMainDrive::boundary->debug();
	//endebug*/

	if (!this->executeTreeClassifier())
	{
		/*//debug
		Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executeTreeClassifier (false), " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
		this->PMJMesh::WorkerMainDrive::boundary->debug();
		//endebug*/

#if USE_THREAD_COLOR
		if (typeid(this->idManager) == typeid(Performer::SequentialIdManager *))
		{
			this->colorizeMesh(0);
		}
		else
		{
			this->colorizeMesh();
		}
#endif //#if USE_THREAD_COLOR

		return;
	}

	/*//debug
	Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executeTreeClassifier (true), " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
	this->PMJMesh::WorkerMainDrive::boundary->debug();
	//endebug*/

	if (!this->executeTreeDisrefiner())
	{
		/*//debug
		Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executeTreeDisrefiner (false), " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
		this->PMJMesh::WorkerMainDrive::boundary->debug();
		//endebug*/

#if USE_THREAD_COLOR
		if (typeid(this->idManager) == typeid(Performer::SequentialIdManager *))
		{
			this->colorizeMesh(0);
		}
		else
		{
			this->colorizeMesh();
		}
#endif //#if USE_THREAD_COLOR

		return;
	}

	/*//debug
	Data::cout() << "proc " << this->comm->rank() << " boundary->debug after executeTreeDisrefiner (true), " << this->PMJMesh::WorkerMainDrive::boundary << Data::endl;
	this->PMJMesh::WorkerMainDrive::boundary->debug();
	//endebug*/

#if USE_THREAD_COLOR
	if (typeid(this->idManager) == typeid(Performer::SequentialIdManager *))
	{
		this->colorizeMesh(0);
	}
	else
	{
		this->colorizeMesh();
	}
#endif //#if USE_THREAD_COLOR

	this->PMJMesh::WorkerMainDrive::status = this->success;
}

bool PMTools::WorkerMainDrive::executeMeshGenerator()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real tstart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	if (this->MJMesh::MainDrive::mesh)
	{
		this->MJMesh::MainDrive::mesh->clear();
	}
	else
	{
		this->MJMesh::MainDrive::mesh = new MJMesh::Mesh();
	}

	this->MTools::MainDrive::mesh = this->MJMesh::MainDrive::mesh;

#if USE_NEW_FRONT_UPDATER
	this->oldFront = new PMJMesh::Front(this->PMJMesh::WorkerMainDrive::front->isSorted());

	this->oldFront->set(this->PMJMesh::WorkerMainDrive::front);
#else
	this->oldFront = this->PMJMesh::WorkerMainDrive::front->getAllElements();
	this->newFront.clear();
	this->commonFront.clear();
#endif //#if USE_NEW_FRONT_UPDATER

	PMTools::MeshGenerator *meshGenerator = this->makeMeshGenerator();

	//set input data
	meshGenerator->setWorkerTask(true);
	meshGenerator->setEnableBackTracking(this->enableBackTracking);
	meshGenerator->setFront(this->PMJMesh::WorkerMainDrive::front);
	meshGenerator->setMesh(this->MJMesh::MainDrive::mesh);

	if (this->box)
	{
		meshGenerator->setUseBoxTest(true);
		meshGenerator->setBox(*this->box);
		meshGenerator->setTreeBox(*static_cast<Data::Box *>(this->PMJMesh::WorkerMainDrive::tree->getShape()));
	}

	meshGenerator->setRangeProportion(this->rangeProportion);

	meshGenerator->setFtree(this->ftree);

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

	delete meshGenerator;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeGeometry += Data::time() - tstart;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return ok;
}

#endif //#if USE_MTOOLS
