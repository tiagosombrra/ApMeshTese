#include "PJmesh/MainDrive.h"

#include "Data/BoxTree.h"
#include "Parallel/Communicator.h"
#include "PJmesh/InputData.h"
#include "PJmesh/Communicator.h"
#include "PJmesh/TreeGenerator.h"
#include "PJmesh/GlobalTaskManager.h"
#include "PJmesh/BackTrackingFrontAdvancer.h"
#include "PMJMesh/BoundaryBuilder.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Boundary.h"

PJmesh::MainDrive::MainDrive(Parallel::Communicator *comm) :
	Jmesh::MainDrive(),
	PMJMesh::MainDrive(comm)
{
	///differences between constructor, cli and gui:
	///                                   constructor  cli    gui
	///setParallel()                      true         false  user
	///setSkippingInnerInDecomposition()  true         true   false
	///setRemoveLayersInLoad()            3            3      0
	///setHeightDependentInLoad()         true         true   false
	///setTreeFactor()                    1.0          file   user

	this->setAlgorithm(PMJMesh::GLOBAL_SHIFT);
	this->setMinRefinementPostponed(true);
	this->setPercentageNonWorkingTask(0.0);

#if USE_NUMERICAL
	this->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL
	this->setUsingUniformRefinement(false);
	this->setParallel(true);
	this->setLoadBalancerType(Parallel::DEMAND_PARALLEL_DYNAMIC);
	this->setPartitionerType(PMJMesh::BOX_LOAD);
	this->setUsePartitioningThreshold(false);
	this->setRemoveLayersInLoad(3);
	this->setHeightDependentInLoad(true);
	this->setSkippingInnerInDecomposition(true);
	this->setRangeProportion(1.0);
	this->setSmoothingLayer(1);
	this->setUpdateType(PMJMesh::EMPTY_NO_BOUNDARY);
	this->setShiftType(PMJMesh::LATERAL);

	if (comm)
	{
		this->setComm(comm);
	}
	else
	{
		if (this->comm)
		{
			delete this->comm;

			this->comm = NULL;
		}

		this->setComm(this->makeCommunicator());
	}

	//if ((this->comm) && (!this->comm->isInitialized()))
	if (this->comm)
	{
		this->comm->init();
	}

	this->dimension = 3;
}

PJmesh::MainDrive::~MainDrive()
{
	this->clear();
}

#if USE_NUMERICAL
void PJmesh::MainDrive::setHotellingTransformEnabled(bool hotellingTransformEnabled)
{
    this->PMJMesh::MainDrive::hotellingTransformEnabled = this->Jmesh::MainDrive::hotellingTransformEnabled = hotellingTransformEnabled;
}

bool PJmesh::MainDrive::isHotellingTransformEnabled() const
{
    return this->PMJMesh::MainDrive::hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void PJmesh::MainDrive::setUsingUniformRefinement(bool usingUniformRefinement)
{
	this->PMJMesh::MainDrive::usingUniformRefinement = this->Jmesh::MainDrive::usingUniformRefinement = usingUniformRefinement;
}

bool PJmesh::MainDrive::isUsingUniformRefinement() const
{
	return this->PMJMesh::MainDrive::usingUniformRefinement;
}

const Data::Boundary *PJmesh::MainDrive::getBoundary() const
{
	return Jmesh::MainDrive::getBoundary();
}

void PJmesh::MainDrive::clear()
{
	this->Jmesh::MainDrive::boundary = NULL;
	this->Jmesh::MainDrive::mesh = NULL;
	this->Jmesh::MainDrive::tree = NULL;

	Jmesh::MainDrive::clear();
	PMJMesh::MainDrive::clear();
}

bool PJmesh::MainDrive::loadFile()
{
#if (!USE_WORKER_FILE_LOADER)
	if (this->comm->isMaster())
#endif //#if (!USE_WORKER_FILE_LOADER)
	{
		return Jmesh::MainDrive::loadFile();
	}

	return false;
}

bool PJmesh::MainDrive::saveFile() const
{
	if (this->comm->isMaster())
	{
		//return Jmesh::MainDrive::saveFile();
		return PMJMesh::MainDrive::saveFile();
	}

	return false;
}

void PJmesh::MainDrive::execute()
{
	if ((this->Performer::MainDrive::status.getType() == PMJMesh::MainDrive::success.getType()) ||
		(this->Performer::MainDrive::status.getType() >= PMJMesh::MainDrive::error.getType()))
	{
		return;
	}

#if USE_TIME_MEASUREMENTS
	Real start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

	switch (this->Performer::MainDrive::status.getType())
	{
	case PMJMesh::MainDrive::NOT_INIT:

		if (!this->executeInit())
		{
			break;
		}

#if USE_GUI
		//if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case PMJMesh::MainDrive::INIT:

		//begin adjusting boundary

		if (!this->executeBoundaryBuilder(true))
		{
			break;
		}

		//end adjusting boundary

#if USE_GUI
		//if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case PMJMesh::MainDrive::BOUNDARY_BUILT:

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

		//end adjusting tree

/*#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
		if (this->comm->isMaster())
		{
			Real end = Data::time();
			std::cout << "Time for generating tree = " << end - start << " seconds" << std::endl;
		}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))*/

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case PMJMesh::MainDrive::TREE_GENERATED:

		//begin mesh generation

		if (!this->executeFrontGeometryAdvancer())
		{
			break;
		}

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case PMJMesh::MainDrive::FRONT_GEOMETRY_ADVANCED:

		if (!this->executeFrontBackTrackingAdvancer())
		{
			this->status = 0;

			break;
		}

		this->status = 1;

		//end mesh generation

#if USE_GUI
		if ((this->dimension == 3) && (Data::isStepByStep())) break;
#endif //#if USE_GUI

	case PMJMesh::MainDrive::FRONT_BACK_TRACKING_ADVANCED:

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
		if (this->comm->isMaster())
		{
			Real end = Data::time();
			std::cout << "Runtime = " << end - start << " seconds" << std::endl;
		}
#endif //#if USE_TIME_MEASUREMENTS

		if (!this->PMJMesh::MainDrive::executeFinalize())
		{
			break;
		}
	}

#if USE_TIME_MEASUREMENTS
	Real end = Data::time();
	this->PMJMesh::MainDrive::runtime += end - start;
#endif //#if USE_TIME_MEASUREMENTS
}

std::string PJmesh::MainDrive::qualityString() const
{
	return PMJMesh::MainDrive::qualityString();
}

bool PJmesh::MainDrive::loadInputFile(const std::string &inputFileName)
{
	bool ret = Jmesh::MainDrive::loadInputFile(inputFileName);

	if (ret)
	{
		this->dimension = 3;
	}

	return ret;
}

bool PJmesh::MainDrive::saveInputFile(const std::string &outputFileName) const
{
	//return Jmesh::MainDrive::saveInputFile(outputFileName);
	return PMJMesh::MainDrive::saveInputFile(outputFileName);
}

bool PJmesh::MainDrive::loadOutputFile(const std::string &inputFileName)
{
	return Jmesh::MainDrive::loadOutputFile(inputFileName);
}

bool PJmesh::MainDrive::saveOutputFile(const std::string &outputFileName) const
{
	//return Jmesh::MainDrive::saveOutputFile(outputFileName);
	return PMJMesh::MainDrive::saveOutputFile(outputFileName);
}

bool PJmesh::MainDrive::executeInit()
{
	fprintf (stderr, "Model Creation:") ;
	fprintf( stderr, "\n" );

#if DSP_TIME
	init_time = clock( );
	init_gbl_time = clock( );
	fprintf( stderr, "\n" );
	fprintf( stderr, "	Building look-up table................" );
	fprintf( stderr, "\n" );
#endif

#if DSP_TIME
	fprintf( stderr,"\n") ;
	tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
	fprintf( stderr,"\t\t Initial front size: %8d fac.\n", num_face);
	fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.", (double)tot_time);
	fprintf( stderr,"\n") ;
#endif

#if USE_TIME_MEASUREMENTS
	this->Jmesh::MainDrive::runtime = 0.0;
	this->PMJMesh::MainDrive::runtime = 0.0;
#endif //#if USE_TIME_MEASUREMENTS

	if (this->Jmesh::MainDrive::boundaryBuilder)
	{
		this->PMJMesh::MainDrive::boundaryBuilder = this->Jmesh::MainDrive::boundaryBuilder;
	}

	if ((this->partitionerType == PMJMesh::BSP_NUMBER) || (this->partitionerType == PMJMesh::SMART_BSP_NUMBER))
	{
		useUniformLevel = 0;
	}
	else
	{
		useUniformLevel = 1;
	}

	return PMJMesh::MainDrive::executeInit();
}

bool PJmesh::MainDrive::executeBoundaryBuilder(bool del)
{
	if (this->Jmesh::MainDrive::boundaryBuilder)
	{
		this->PMJMesh::MainDrive::boundaryBuilder = this->Jmesh::MainDrive::boundaryBuilder;
	}
	else
	{
		this->Jmesh::MainDrive::boundaryBuilder = this->PMJMesh::MainDrive::boundaryBuilder;
	}

	if (this->comm->isSlave())
	{
		this->fillBoundary();
	}

	bool ret = Jmesh::MainDrive::executeBoundaryBuilder(false);

	this->idManager = this->makeIdManager(static_cast<MJMesh::BoundaryBuilder *>(this->PMJMesh::MainDrive::boundaryBuilder)->getIdManager());

	if (del)
	{
		delete this->Jmesh::MainDrive::boundaryBuilder;

		this->Jmesh::MainDrive::boundaryBuilder = NULL;
		this->PMJMesh::MainDrive::boundaryBuilder = NULL;
	}

	this->PMJMesh::MainDrive::boundary = this->Jmesh::MainDrive::boundary;

	this->Performer::MainDrive::status = this->boundaryBuilt;

	return ret;
}

#if USE_NUMERICAL
bool PJmesh::MainDrive::executeHotellingTransform()
{
	return MJMesh::MainDrive::executeHotellingTransform();
}
#endif //#if USE_NUMERICAL

bool PJmesh::MainDrive::executeTreeGenerator(bool del)
{
#if USE_TIMING
	this->treeGenerationTime = 0.0;

	Real start = Data::time();
#endif //#if USE_TIMING

	status = POct3DGeneration( version, internal, factor, tol,
                              offset, num_node, num_face,
                              nodes, tnodes, faces, &num_gen_nodes,
                              &generated_nodes, &tgenerated_nodes,
                              &num_elems, &elems,
                              this->PMJMesh::MainDrive::usingUniformRefinement ? 1 : 0, NULL, NULL ) ;

	Jmesh::MainDrive::executeTreeGenerator(false);

#if USE_TIMING
	this->treeGenerationTime += Data::time() - start;
#endif //#if USE_TIMING

	this->minRefinementLevel = static_cast<bool>(useUniformLevel) ? static_cast<UInt>(uniformLevel) : 0;

	this->PMJMesh::MainDrive::tree = this->Jmesh::MainDrive::tree;

	if (del)
	{
		delete this->Jmesh::MainDrive::treeGenerator;

		this->Jmesh::MainDrive::treeGenerator = NULL;
	}

	this->executeSendRuntimeInfo();

	this->Performer::MainDrive::status = this->treeGenerated;

	return true;
}

bool PJmesh::MainDrive::executeFrontGeometryAdvancer()
{
	bool ret = PMJMesh::MainDrive::executeFrontGeometryAdvancer();

	//if (ret)
	{
		this->Jmesh::MainDrive::mesh = this->PMJMesh::MainDrive::mesh;
	}

	return ret;
}

bool PJmesh::MainDrive::executeFrontBackTrackingAdvancer()
{
	bool ret = PMJMesh::MainDrive::executeFrontBackTrackingAdvancer();

	if ((this->comm->isMaster()) && (this->PMJMesh::MainDrive::tree))
	{
#if USE_GUI
		//to prevent interface errors
		Data::BoxTree *tree = this->PMJMesh::MainDrive::tree;

		this->PMJMesh::MainDrive::tree = NULL;

		delete tree;
#else
		delete this->PMJMesh::MainDrive::tree;

		this->PMJMesh::MainDrive::tree = NULL;
#endif //#if USE_GUI

		this->Jmesh::MainDrive::tree = this->PMJMesh::MainDrive::tree;

		this->usingSearchTrees = false;
	}

	return ret;
}

#if USE_NUMERICAL
bool PJmesh::MainDrive::executeHotellingBackTransform()
{
	return MJMesh::MainDrive::executeHotellingBackTransform();
}
#endif //#if USE_NUMERICAL

#if (!USE_WORKER_FILE_LOADER)
PMJMesh::InputData *PJmesh::MainDrive::makeInputData() const
{
	return new PJmesh::InputData();
}

PMJMesh::InputData *PJmesh::MainDrive::makeAndFillInputData() const
{
	PJmesh::InputData *input = static_cast<PJmesh::InputData *>(PMJMesh::MainDrive::makeAndFillInputData());

	input->setVer(this->ver);

	input->setVersion(version);
	input->setFormat(format);
	input->setFactor(factor);
	input->setTol(tol);
	input->setInternal(internal);

	input->setNumNode(num_node);
	input->setNodes(nodes);
	input->setTnodes(tnodes);
	input->setNumFace(num_face);
	input->setFaces(faces);

	return input;
}

void PJmesh::MainDrive::fillWithInputData(const PMJMesh::InputData *input)
{
	PMJMesh::MainDrive::fillWithInputData(input);

	const PJmesh::InputData *inp = static_cast<const PJmesh::InputData *>(input);

	this->setVer(inp->getVer());

	version = inp->getVersion();
	format = inp->getFormat();
	factor = inp->getFactor();
	tol = inp->getTol();
	internal = inp->getInternal();

	num_node = inp->getNumNode();
	nodes = inp->getNodes();
	tnodes = inp->getTnodes();
	num_face = inp->getNumFace();
	faces = inp->getFaces();
}
#endif //#if (!USE_WORKER_FILE_LOADER)

Parallel::Communicator *PJmesh::MainDrive::makeCommunicator() const
{
	return new PJmesh::Communicator();
}

Performer::BoundaryBuilder *PJmesh::MainDrive::makeBoundaryBuilder() const
{
	//return PMJMesh::MainDrive::makeBoundaryBuilder();
	return Jmesh::MainDrive::makeBoundaryBuilder();
}

Performer::TreeGenerator *PJmesh::MainDrive::makeTreeGenerator() const
{
	return new PJmesh::TreeGenerator(this->Jmesh::MainDrive::boundary, Msh3DTree()
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		, static_cast<PJmesh::Communicator *>(this->comm)
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		);
}

PMJMesh::TaskManager *PJmesh::MainDrive::makeTaskManager() const
{
	///TODO replace all these parameters with just the InputData

	switch (this->algorithm)
	{
	case PMJMesh::GLOBAL_SHIFT:
		return new PJmesh::GlobalTaskManager(version, format, factor, tol, internal,
			this->parallel, this->dimension,
			this->PMJMesh::MainDrive::boundary, this->PMJMesh::MainDrive::tree,
			this->percentageNonWorkingTask, this->heightDependentInLoad, this->removeLayersInLoad,
			this->smoothInSlavesEnabled, this->numImproves, this->smoothingFactor,
#if (!USE_DELETE_FELS)
			this->unsmoothingLayer,
#endif //#if (!USE_DELETE_FELS)
			this->smoothingLayer, this->localBackTrackingEnabled,
			this->meshSorted, this->maxBackTracks, this->partitionerType,
			this->updateType, this->shiftType,
			this->usePartitioningThreshold, this->partitioningThreshold,
			this->boundarySorted, this->usingSearchTrees, this->rangeProportion,
			this->minRefinementLevel, this->skippingInnerInDecomposition,
			this->createInnerFrontInDecomposition,
			this->loadBalancerType, this->balancer,
			this->comm);

	case PMJMesh::GLOBAL_SHIFT_WITH_ADJACENCY:
		return NULL;
		//return new AdjacentGlobalTaskManager();

	case PMJMesh::LOCAL_SHIFT:
		return NULL;
		//return new LocalTaskManager();

	case PMJMesh::LOCAL_SHIFT_WITH_COVARIANCE:
		return NULL;
		//return new CovarianceLocalTaskManager();

	case PMJMesh::A_PRIORI_INTERFACE_MESH:
		return NULL;
		//return new APrioriTaskManager(this->parallel, this->dimension,
		//	this->boundary, this->tree, this->percentageNonWorkingTask,
		//	this->smoothInSlavesEnabled, this->numImproves, this->smoothingFactor,
		//	this->unsmoothingLayer, this->smoothingLayer, this->localBackTrackingEnabled,
		//	this->meshSorted, this->maxBackTracks, this->updateType, this->position,
		//	this->usePartitioningThreshold, this->partitioningThreshold, this->boundarySorted,
		//	this->usingSearchTrees, this->minRefinementLevel, this->loadBalancerType,
		//	this->balancer, this->comm);
	}

	return NULL;
}

MJMesh::BackTrackingFrontAdvancer *PJmesh::MainDrive::makeFrontBackTrackingAdvancer() const
{
	return new PJmesh::BackTrackingFrontAdvancer(
		this->PMJMesh::MainDrive::boundary, this->front, this->PMJMesh::MainDrive::mesh);
}
