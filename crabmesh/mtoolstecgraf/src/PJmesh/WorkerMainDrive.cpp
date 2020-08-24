#include "PJmesh/WorkerMainDrive.h"

#include "Data/Box.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "PJmesh/MeshGenerator.h"
#include "typeinfo"

#if USE_NEW_FRONT_UPDATER
#include "PMJMesh/Front.h"
#endif //#if USE_NEW_FRONT_UPDATER

#if USE_THREAD_COLOR
#include <typeinfo>
#endif //#if USE_THREAD_COLOR

PJmesh::WorkerMainDrive::WorkerMainDrive(Parallel::Communicator *comm) :
	PMJMesh::WorkerMainDrive(comm),
	Jmesh::MainDrive()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeToJmeshConversion = 0.0;
	this->timeFromJmeshConversion = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
}

PJmesh::WorkerMainDrive::~WorkerMainDrive()
{
	if ((this->Jmesh::MainDrive::mesh) &&
		(this->PMJMesh::WorkerMainDrive::mesh) &&
		(this->Jmesh::MainDrive::mesh == this->PMJMesh::WorkerMainDrive::mesh))
	{
		this->Jmesh::MainDrive::mesh = NULL;
	}
}

void PJmesh::WorkerMainDrive::setVersion(int version)
{
	this->version = version;
}

void PJmesh::WorkerMainDrive::setFormat(int format)
{
	this->format = format;
}

void PJmesh::WorkerMainDrive::setFactor(double factor)
{
	this->factor = factor;
}

void PJmesh::WorkerMainDrive::setTol(double tol)
{
	this->tol = tol;
}

void PJmesh::WorkerMainDrive::setInternal(int internal)
{
	this->internal = internal;
}

void PJmesh::WorkerMainDrive::setMesh(MJMesh::Mesh *mesh)
{
	this->Jmesh::MainDrive::mesh = this->MJMesh::MainDrive::mesh = mesh;
}

bool PJmesh::WorkerMainDrive::loadFile()
{
	return Jmesh::MainDrive::loadFile();
}

bool PJmesh::WorkerMainDrive::saveFile() const
{
	return Jmesh::MainDrive::saveFile();
}

bool PJmesh::WorkerMainDrive::loadInputFile(const std::string &inputFileName)
{
	return Jmesh::MainDrive::loadInputFile(inputFileName);
}

bool PJmesh::WorkerMainDrive::saveInputFile(const std::string &inputFileName) const
{
	return Jmesh::MainDrive::saveInputFile(inputFileName);
}

bool PJmesh::WorkerMainDrive::loadOutputFile(const std::string &outputFileName)
{
	return Jmesh::MainDrive::loadOutputFile(outputFileName);
}

bool PJmesh::WorkerMainDrive::saveOutputFile(const std::string &outputFileName) const
{
	return Jmesh::MainDrive::saveOutputFile(outputFileName);
}

void PJmesh::WorkerMainDrive::execute()
{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeRefinement = 0.0;
	this->timeTrees = 0.0;
	this->timeGeometry = 0.0;
	this->timeTopology = 0.0;
	this->timeBackTracking = 0.0;
	this->timeClassify = 0.0;
	this->timeImprover = 0.0;
	this->timeToJmeshConversion = 0.0;
	this->timeFromJmeshConversion = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	this->PMJMesh::WorkerMainDrive::status = this->init;

	if (!this->executeMeshGenerator())
	{
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

	if (!this->executeTreeMinRefiner(true))
	{
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

	if (!this->executeTreeClassifier())
	{
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

	if (!this->executeTreeDisrefiner())
	{
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

bool PJmesh::WorkerMainDrive::executeMeshGenerator()
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

	this->Jmesh::MainDrive::mesh = this->MJMesh::MainDrive::mesh;

#if USE_NEW_FRONT_UPDATER
	this->oldFront = new PMJMesh::Front(this->front->isSorted());

	this->oldFront->set(this->front);
#else
	this->oldFront = this->front->getAllElements();
	this->newFront.clear();
	this->commonFront.clear();
#endif //#if USE_NEW_FRONT_UPDATER

	PJmesh::MeshGenerator *meshGenerator = new PJmesh::MeshGenerator();

	//set input data
	meshGenerator->setFront(this->front);
	meshGenerator->setMesh(this->MJMesh::MainDrive::mesh);

	if (this->box)
	{
		meshGenerator->setUseBoxTest(true);
		meshGenerator->setBox(this->box->getMin()->getCoordsArray(), this->box->getMax()->getCoordsArray());
	}

	meshGenerator->setRangeProportion(this->rangeProportion);

	meshGenerator->setVersion(this->version);
	meshGenerator->setInternal(this->internal);
	meshGenerator->setFactor(this->factor);
	meshGenerator->setTol(this->tol);
	meshGenerator->setOffset(this->offset);
	meshGenerator->setFromone(this->fromone);

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
	this->timeToJmeshConversion = meshGenerator->timeToJmeshConversion;
	this->timeFromJmeshConversion = meshGenerator->timeFromJmeshConversion;

	tstart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	//get outsput data
	this->fromone = meshGenerator->getFromone();
	this->status = meshGenerator->getJmeshStatus();

	bool ok = (this->status != 0);

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeGeometry += Data::time() - tstart;

	this->timeBackTracking = 0.0;

	tstart = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	if (ok)
	{
#if (!USE_NEW_FRONT_UPDATER)
		this->newFront = this->front->getAllElements();
#endif //#if (!USE_NEW_FRONT_UPDATER)

		this->findCommonFront();
	}
	else
	{
#if USE_NEW_FRONT_UPDATER
		this->commonFront = static_cast<PMJMesh::Front *>(this->front);

		this->front = NULL;

		this->oldFront->clear();

		delete this->oldFront;

		this->oldFront = NULL;
#else
		this->commonFront.swap(this->oldFront);
#endif //#if USE_NEW_FRONT_UPDATER

		this->PMJMesh::WorkerMainDrive::status = this->error;

		this->PMJMesh::WorkerMainDrive::status.append(meshGenerator->getStatus());
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
