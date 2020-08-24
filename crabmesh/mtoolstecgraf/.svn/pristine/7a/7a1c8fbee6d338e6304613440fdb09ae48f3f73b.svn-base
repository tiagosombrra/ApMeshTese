#include "PMJMesh2/WorkerMainDrive.h"

#include "Data/OStream.h"
#include "Parallel/Communicator.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/MeshImprover.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/GeometryFrontAdvancer.h"
#include "PMJMesh/WorkerMeshImprover.h"
#include "PMJMesh2/AuxiliaryFrontGenerator.h"

using namespace PMJMesh2;

PMJMesh2::WorkerMainDrive::WorkerMainDrive(Parallel::Communicator *comm, UInt status) :
    PMJMesh::WorkerMainDrive::WorkerMainDrive(comm, status)
{
    this->createInnerFrontInDecomposition = false;
    this->createCrackInInnerFront = false;
    this->displaceVerticesInInnerFront = false;
    this->fixateFront = true;
    this->shouldAdvanceFront = true;

    this->frontGenerator = NULL;
}

PMJMesh2::WorkerMainDrive::~WorkerMainDrive()
{
    if (this->frontGenerator)
    {
        delete this->frontGenerator;

        this->frontGenerator = NULL;
    }
}

void PMJMesh2::WorkerMainDrive::setCreateInnerFrontInDecomposition(bool createInnerFrontInDecomposition)
{
    this->createInnerFrontInDecomposition = createInnerFrontInDecomposition;
}

void PMJMesh2::WorkerMainDrive::setCreateCrackInInnerFront(bool createCrackInInnerFront)
{
    this->createCrackInInnerFront = createCrackInInnerFront;
}

void PMJMesh2::WorkerMainDrive::setDisplaceVerticesInInnerFront(bool displaceVerticesInInnerFront)
{
    this->displaceVerticesInInnerFront = displaceVerticesInInnerFront;
}

void PMJMesh2::WorkerMainDrive::setFixateFront(bool fixateFront)
{
    this->fixateFront = fixateFront;
}

void PMJMesh2::WorkerMainDrive::setShouldAdvanceFront(bool shouldAdvanceFront)
{
    this->shouldAdvanceFront = shouldAdvanceFront;
}

void PMJMesh2::WorkerMainDrive::executeFrontGenerator()
{
    /*if(this->front == NULL)
    {
        std::cout << "TA NULO!" << std::endl;
        //return;
    }*/

    if ((this->front->size() > 0) || (!this->createInnerFrontInDecomposition))
    {
        return;
    }

    if (!this->frontGenerator)
    {
        this->frontGenerator = this->makeFrontGenerator();
    }

    this->frontGenerator->execute();
}

void PMJMesh2::WorkerMainDrive::executeFrontMerger()
{
    if (!this->frontGenerator)
    {
        return;
    }

    this->frontGenerator->execute();

    delete this->frontGenerator;

    this->frontGenerator = NULL;
}

bool PMJMesh2::WorkerMainDrive::executeFrontGeometryAdvancer()
{
    if (!this->shouldAdvanceFront)
    {
        return true;
    }

	if (!this->geometryAdvancer)
	{
		this->geometryAdvancer = this->makeFrontGeometryAdvancer();
	}

	if (this->geometryAdvancer->getStatus().getType() == MJMesh::GeometryFrontAdvancer::notInit.getType())
	{
		static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setBox(this->box);

		static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setFront(this->front);

		if (this->front)
		{
#if USE_NEW_FRONT_UPDATER
			if (!this->oldFront)
            {
                this->oldFront = new PMJMesh::Front(this->front->isSorted());

                this->oldFront->set(this->front);
            }

#if USE_DELETE_FELS
            static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setOldFront(this->oldFront);
#endif //#if USE_DELETE_FELS
#else
			this->oldFront = static_cast<PMJMesh::Front *>(this->front)->getAllElements();
#endif //#if USE_NEW_FRONT_UPDATER

			//Data::FrontElementList crossing = static_cast<PMJMesh::Front *>(this->front)->getCrossing();

			//this->oldFront.insert(this->oldFront.end(), crossing.begin(), crossing.end());
		}
	}

	this->executeFrontGenerator();

	this->geometryAdvancer->setIdManager(this->idManager);

	//this->crossing.clear();

	this->geometryAdvancer->execute();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeTrees += static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->timeTrees;
	this->timeGeometry += static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->timeGeometry;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	if ((this->geometryAdvancer->getStatus().getType() == MJMesh::GeometryFrontAdvancer::ended.getType()) ||
		(this->geometryAdvancer->getStatus().getType() == MJMesh::GeometryFrontAdvancer::rejected.getType()) ||
		(this->geometryAdvancer->getStatus().getType() >= MJMesh::GeometryFrontAdvancer::error.getType()))
	{
		//delete ((PMJMesh::GeometryFrontAdvancer *)this->builder)->getBox();

		static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setBox(NULL);

		//this->crossing = static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->getCrossing();

		this->mesh = static_cast<MJMesh::Mesh *>(static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->getMesh());
		this->front = static_cast<PMJMesh::Front *>(static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->getFront());

		static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setMesh(NULL);
		static_cast<PMJMesh::GeometryFrontAdvancer *>(this->geometryAdvancer)->setFront(NULL);
	}

	bool ok = (this->geometryAdvancer->getStatus().getType() < MJMesh::GeometryFrontAdvancer::error.getType());

	if (!ok)
	{
		this->status = this->errorFrontGeometryAdvancer;

		this->status.append(this->geometryAdvancer->getStatus());
	}

	delete this->geometryAdvancer;

	this->geometryAdvancer = NULL;

	return ok;
}

bool PMJMesh2::WorkerMainDrive::executeFrontTopologyAdvancer()
{
    if (!this->shouldAdvanceFront)
    {
        return true;
    }

    return PMJMesh::WorkerMainDrive::executeFrontTopologyAdvancer();
}

bool PMJMesh2::WorkerMainDrive::executeFrontBackTrackingAdvancer()
{
    if (!this->shouldAdvanceFront)
    {
        return true;
    }

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
    Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    this->executeFrontMerger();

    if (!this->front)
	{
#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
        this->timeBackTracking += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

		return true;
	}

	///TODO back tracking in slaves

	static_cast<PMJMesh::Front *>(this->front)->moveCrossingToRejected();

#if (!USE_NEW_FRONT_UPDATER)
	this->newFront = this->front->getAllElements();
#endif //#if (!USE_NEW_FRONT_UPDATER)

	//this->newFront.insert(this->newFront.end(), this->crossing.begin(), this->crossing.end());
	//this->crossing.clear();

	this->findCommonFront();

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeBackTracking += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	bool ok = true;

	if (!ok)
	{
		this->status = this->errorFrontBackTrackingAdvancer;
	}

	return ok;
}

/*bool PMJMesh2::WorkerMainDrive::executeMeshImprover()
{
	if ((!this->front) || (!this->smoothInSlavesEnabled))
	{
*//*#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		this->timeImprover = 0.0;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))*//*

		return true;
	}

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	Real start = Data::time();
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

    this->executeFrontMerger();

	this->invertNewFront();

	this->improver = this->makeMeshImprover();

	this->improver->setIdManager(this->idManager);

	this->improver->execute();

#if USE_DELETE_FELS
    while (!this->oldInnerVertices.empty())
    {
        this->mesh->addInner(this->oldInnerVertices.front());

        this->oldInnerVertices.pop_front();
    }
#endif //#if USE_DELETE_FELS

	//bool ok = (this->improver->getStatus() < this->improver->error);
	bool ok = true;

	if (!ok)
	{
		this->status = this->errorMeshImprover;

		this->status.append(this->improver->getStatus());
	}

	delete this->improver;

	this->improver = NULL;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	this->timeImprover += Data::time() - start;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	return ok;
}*/

MJMesh::GeometryFrontAdvancer *PMJMesh2::WorkerMainDrive::makeFrontGeometryAdvancer() const
{
    MJMesh::GeometryFrontAdvancer *advancer = PMJMesh::WorkerMainDrive::makeFrontGeometryAdvancer();

    if (this->mesh)
    {
        advancer->setMesh(this->mesh);
    }

    return advancer;
}

AuxiliaryFrontGenerator *PMJMesh2::WorkerMainDrive::makeFrontGenerator()
{
    AuxiliaryFrontGenerator *generator = new AuxiliaryFrontGenerator();

    generator->setDimension(this->dimension);
    generator->setCreateCrackInInnerFront(this->createCrackInInnerFront);
    generator->setDisplaceVerticesInInnerFront(this->displaceVerticesInInnerFront);
    generator->setRangeProportion(this->rangeProportion);

    generator->setBox(this->box);
    generator->setTree(this->tree);
    generator->setFront(static_cast<PMJMesh::Front *>(this->front));

#if USE_DELETE_FELS
    generator->setOldFront(static_cast<PMJMesh::Front *>(this->oldFront));
#endif //#if USE_DELETE_FELS

    generator->setIdManager(this->idManager);

    return generator;
}

MJMesh::MeshImprover *PMJMesh2::WorkerMainDrive::makeMeshImprover() const
{
    if (this->fixateFront)
    {
        return PMJMesh::WorkerMainDrive::makeMeshImprover();
    }

#if (USE_NEW_FRONT_UPDATER /*&& false*/)
	MJMesh::Boundary *boundary = new MJMesh::Boundary();

	//Data::cout() << " debug creating boundary at " << boundary << " as a copy of " << this->boundary << Data::endl;

	/*{
	    std::stringstream str;
	    str << "mesh improver boundary " << boundary << std::endl;
	    std::cout << str.str();
	}

	{
	    std::stringstream str;
	    str << "adding from boundary " << this->boundary << " to boundary " << boundary << std::endl;
	    std::cout << str.str();
	}*/

	/*for (Data::Front::AllFrontElementIterator iter = this->oldFront->allEBegin();
		 iter != this->oldFront->allEEnd(); iter++)
	{
		boundary->add((*iter));
	}

	for (Data::Front::VertexIterator iter = this->oldFront->vBegin();
		 iter != this->oldFront->vEnd(); iter++)
	{
		boundary->add((*iter));
	}*/

	boundary->setElements(this->boundary->getElements());
	boundary->setVertices(this->boundary->getVertices());

	/*{
	    std::stringstream str;
	    str << "adding from front " << this->front << " to boundary " << boundary << std::endl;
	    std::cout << str.str();
	}*/

	for (Data::Front::AllFrontElementIterator iter = this->front->allEBegin();
		 iter != this->front->allEEnd(); iter++)
	{
		if (!boundary->have((*iter)))
        {
            boundary->add((*iter));
        }
	}

	for (Data::Front::VertexIterator iter = this->front->vBegin();
		 iter != this->front->vEnd(); iter++)
	{
		if (!boundary->have((*iter)))
		{
			boundary->add((*iter));

#if USE_DELETE_FELS
            if (this->mesh->haveInner((*iter)))
            {
                this->mesh->removeInner((*iter));

                this->oldInnerVertices.push_back((*iter));
            }
#endif //#if USE_DELETE_FELS
		}
	}

	/*{
	    std::stringstream str;
	    str << "adding from common front " << this->commonFront << " to boundary " << boundary << std::endl;
	    std::cout << str.str();
	}*/

	if (this->commonFront)
    {
        for (Data::Front::AllFrontElementIterator iter = this->commonFront->allEBegin();
             iter != this->commonFront->allEEnd(); iter++)
        {
            if (!boundary->have((*iter)))
            {
                boundary->add((*iter));
            }
        }

        for (Data::Front::VertexIterator iter = this->commonFront->vBegin();
             iter != this->commonFront->vEnd(); iter++)
        {
            if (!boundary->have((*iter)))
            {
                boundary->add((*iter));

#if USE_DELETE_FELS
                if (this->mesh->haveInner((*iter)))
                {
                    this->mesh->removeInner((*iter));

                    this->oldInnerVertices.push_back((*iter));
                }
#endif //#if USE_DELETE_FELS
            }
        }
    }

#if USE_DELETE_FELS
	return new PMJMesh::WorkerMeshImprover(this->dimension, this->mesh,
		boundary, this->tree, true,
		this->numImproves, this->smoothingFactor,
		this->localBackTrackingEnabled, this->meshSorted, this->maxBackTracks);
#else
	return new PMJMesh::WorkerMeshImprover(this->dimension, this->mesh,
		this->unsmoothingLayer, boundary, this->tree, this->removeBoundaryFelsForSmoothing, true,
		this->numImproves, this->smoothingFactor,
		this->localBackTrackingEnabled, this->meshSorted, this->maxBackTracks);
#endif //#if USE_DELETE_FELS
#elif USE_NEW_FRONT_UPDATER
	Data::FrontElementList fels;
	fels.insert(fels.end(), this->boundary->eBegin(), this->boundary->eEnd());
	fels.insert(fels.end(), this->front->allEBegin(), this->front->allEEnd());
	fels.insert(fels.end(), this->commonFront->allEBegin(), this->commonFront->allEEnd());

	return new PMJMesh::WorkerMeshImprover(this->dimension, this->mesh,
		this->unsmoothingLayer, fels, this->tree, this->removeBoundaryFelsForSmoothing,
		this->numImproves, this->smoothingFactor,
		this->localBackTrackingEnabled, this->meshSorted, this->maxBackTracks);
#else
	Data::FrontElementList fels = this->boundary->getElements();
	fels.insert(fels.end(), this->newFront.begin(), this->newFront.end());
	fels.insert(fels.end(), this->commonFront.begin(), this->commonFront.end());

	return new PMJMesh::WorkerMeshImprover(this->dimension, this->mesh,
		this->unsmoothingLayer, fels, this->tree, this->removeBoundaryFelsForSmoothing,
		this->numImproves, this->smoothingFactor,
		this->localBackTrackingEnabled, this->meshSorted, this->maxBackTracks);
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
}
