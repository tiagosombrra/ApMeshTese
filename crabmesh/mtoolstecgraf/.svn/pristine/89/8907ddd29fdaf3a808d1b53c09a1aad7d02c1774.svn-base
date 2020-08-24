#include "PMJMesh2/InputData.h"

#include "PMJMesh2/Communicator.h"

using namespace PMJMesh2;

PMJMesh2::InputData::InputData() :
	Parallel::Transferable(PMJMesh::TRANSFERABLE_INPUT_DATA)
{
	this->setInputFileName("");

	this->setDimension(0);

	this->setIdVertex(0);
	this->setIdFrontElement(0);
	this->setIdElement(0);

#if USE_NUMERICAL
    this->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL
    this->setBoundarySorted(false);
	this->setTreeFactor(0.0);
	this->setUsingSearchTrees(false);
	this->setNumImproves(0);
	this->setSmoothingFactor(0.0);
	this->setMeshSorted(false);
	this->setLocalBackTrackingEnabled(false);
	this->setMaxBackTracks(0);

    this->setNumThreads(0);
    this->setNumTasksPerThread(1);
    this->setHeightDependentInLoad(false);
    this->setRemoveLayersInLoad(0);
    this->setLoadThreshold(0.0);
    this->setInterfaceBadAngleCos(0);
    this->setInterfaceShiftDelta(0.0);
    this->setPartitionerType(GEOMETRY_BASED_BSP);
#if USE_FILE_BASED_BSP
    this->setPartitionBSPFileName("");
#endif //#if USE_FILE_BASED_BSP
    this->setInterfaceMeshGenerationType(A_POSTERIORI);
    this->setInterfaceElementSizeFactor(1.0);
    this->setSharedParallelismEnabled(false);
    this->setCreatingInnerFrontInDecomposition(false);
    this->setRangeProportion(0.0);
    this->setImprovementInSlavesEnabled(false);
    this->setImprovementLayers(0);
    this->setUpdateType(FULL);
    this->setFinalMeshState(JOINED);

	this->setTolerance(0.0);

#if USE_GUI
	this->setStepByStep(false);
#endif //#if USE_GUI
}

PMJMesh2::InputData::~InputData()
{

}

void PMJMesh2::InputData::setInputFileName(const std::string &inputFileName)
{
	this->inputFileName = inputFileName;
}

const std::string &PMJMesh2::InputData::getInputFileName() const
{
	return this->inputFileName;
}

void PMJMesh2::InputData::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

UInt PMJMesh2::InputData::getDimension() const
{
	return this->dimension;
}

void PMJMesh2::InputData::setIdVertex(ULInt idVertex)
{
	this->idVertex = idVertex;
}

ULInt PMJMesh2::InputData::getIdVertex() const
{
	return this->idVertex;
}

void PMJMesh2::InputData::setIdFrontElement(ULInt idFrontElement)
{
	this->idFrontElement = idFrontElement;
}

ULInt PMJMesh2::InputData::getIdFrontElement() const
{
	return this->idFrontElement;
}

void PMJMesh2::InputData::setIdElement(ULInt idElement)
{
	this->idElement = idElement;
}

ULInt PMJMesh2::InputData::getIdElement() const
{
	return this->idElement;
}

#if USE_NUMERICAL
void PMJMesh2::InputData::setHotellingTransformEnabled(bool hotellingTransformEnabled)
{
	this->hotellingTransformEnabled = hotellingTransformEnabled;
}

bool PMJMesh2::InputData::isHotellingTransformEnabled() const
{
	return this->hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void PMJMesh2::InputData::setBoundarySorted(bool boundarySorted)
{
	this->boundarySorted = boundarySorted;
}

bool PMJMesh2::InputData::isBoundarySorted() const
{
	return this->boundarySorted;
}

void PMJMesh2::InputData::setTreeFactor(Real treeFactor)
{
	this->treeFactor = treeFactor;
}

Real PMJMesh2::InputData::getTreeFactor() const
{
	return this->treeFactor;
}

void PMJMesh2::InputData::setUsingSearchTrees(bool usingSearchTrees)
{
	this->usingSearchTrees = usingSearchTrees;
}

bool PMJMesh2::InputData::isUsingSearchTrees() const
{
	return this->usingSearchTrees;
}

void PMJMesh2::InputData::setNumImproves(UInt numImproves)
{
	this->numImproves = numImproves;
}

UInt PMJMesh2::InputData::getNumImproves() const
{
	return this->numImproves;
}

void PMJMesh2::InputData::setSmoothingFactor(Real smoothingFactor)
{
	this->smoothingFactor = smoothingFactor;
}

Real PMJMesh2::InputData::getSmoothingFactor() const
{
	return this->smoothingFactor;
}

void PMJMesh2::InputData::setMeshSorted(bool meshSorted)
{
	this->meshSorted = meshSorted;
}

bool PMJMesh2::InputData::isMeshSorted() const
{
	return this->meshSorted;
}

void PMJMesh2::InputData::setLocalBackTrackingEnabled(bool localBackTrackingEnabled)
{
	this->localBackTrackingEnabled = localBackTrackingEnabled;
}

bool PMJMesh2::InputData::isLocalBackTrackingEnabled() const
{
	return this->localBackTrackingEnabled;
}

void PMJMesh2::InputData::setMaxBackTracks(UInt maxBackTracks)
{
	this->maxBackTracks = maxBackTracks;
}

UInt PMJMesh2::InputData::getMaxBackTracks() const
{
	return this->maxBackTracks;
}

void PMJMesh2::InputData::setHeightDependentInLoad(bool heightDependentInLoad)
{
    this->heightDependentInLoad = heightDependentInLoad;
}

bool PMJMesh2::InputData::isHeightDependentInLoad() const
{
    return this->heightDependentInLoad;
}

void PMJMesh2::InputData::setRemoveLayersInLoad(UInt removeLayersInLoad)
{
    this->removeLayersInLoad = removeLayersInLoad;
}

UInt PMJMesh2::InputData::getRemoveLayersInLoad() const
{
    return this->removeLayersInLoad;
}

#if USE_REMOVED_LAYERS_PROPORTIONS
void PMJMesh2::InputData::setRemovedLayersProportions(const Data::RealVector &removedLayersProportions)
{
    this->removedLayersProportions = removedLayersProportions;
}

const Data::RealVector &PMJMesh2::InputData::getRemovedLayersProportions() const
{
    return this->removedLayersProportions;
}
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

void PMJMesh2::InputData::setLoadThreshold(Real loadThreshold)
{
    this->loadThreshold = loadThreshold;
}

Real PMJMesh2::InputData::getLoadThreshold() const
{
    return this->loadThreshold;
}

void PMJMesh2::InputData::setInterfaceBadAngleCos(Real interfaceBadAngleCos)
{
    this->interfaceBadAngleCos = interfaceBadAngleCos;
}

Real PMJMesh2::InputData::getInterfaceBadAngleCos() const
{
    return this->interfaceBadAngleCos;
}

void PMJMesh2::InputData::setInterfaceShiftDelta(Real interfaceShiftDelta)
{
    this->interfaceShiftDelta = interfaceShiftDelta;
}

Real PMJMesh2::InputData::getInterfaceShiftDelta() const
{
    return this->interfaceShiftDelta;
}

void PMJMesh2::InputData::setPartitionerType(enum PartitionerType partitionerType)
{
    this->partitionerType = partitionerType;
}

enum PartitionerType PMJMesh2::InputData::getPartitionerType() const
{
    return this->partitionerType;
}

#if USE_FILE_BASED_BSP
void PMJMesh2::InputData::setPartitionBSPFileName(const std::string &partitionBSPFileName)
{
    this->partitionBSPFileName = partitionBSPFileName;
}

std::string PMJMesh2::InputData::getPartitionBSPFileName() const
{
    return this->partitionBSPFileName;
}
#endif //#if USE_FILE_BASED_BSP

void PMJMesh2::InputData::setInterfaceMeshGenerationType(enum InterfaceMeshGenerationType interfaceMeshGenerationType)
{
    this->interfaceMeshGenerationType = interfaceMeshGenerationType;
}

enum InterfaceMeshGenerationType PMJMesh2::InputData::getInterfaceMeshGenerationType() const
{
    return this->interfaceMeshGenerationType;
}

void PMJMesh2::InputData::setInterfaceElementSizeFactor(Real interfaceElementSizeFactor)
{
    this->interfaceElementSizeFactor = interfaceElementSizeFactor;
}

Real PMJMesh2::InputData::getInterfaceElementSizeFactor() const
{
    return this->interfaceElementSizeFactor;
}

void PMJMesh2::InputData::setNumThreads(UInt numThreads)
{
    this->numThreads = numThreads;
}

UInt PMJMesh2::InputData::getNumThreads() const
{
    return this->numThreads;
}

void PMJMesh2::InputData::setNumTasksPerThread(UInt numTasksPerThread)
{
    this->numTasksPerThread = numTasksPerThread;
}

UInt PMJMesh2::InputData::getNumTasksPerThread() const
{
    return this->numTasksPerThread;
}

void PMJMesh2::InputData::setSharedParallelismEnabled(bool sharedParallelismEnabled)
{
    this->sharedParallelismEnabled = sharedParallelismEnabled;
}

bool PMJMesh2::InputData::isSharedParallelismEnabled() const
{
    return this->sharedParallelismEnabled;
}

void PMJMesh2::InputData::setCreatingInnerFrontInDecomposition(bool creatingInnerFrontInDecomposition)
{
    this->creatingInnerFrontInDecomposition = creatingInnerFrontInDecomposition;
}

bool PMJMesh2::InputData::isCreatingInnerFrontInDecomposition() const
{
    return this->creatingInnerFrontInDecomposition;
}

void PMJMesh2::InputData::setRangeProportion(Real rangeProportion)
{
	this->rangeProportion = rangeProportion;
}

Real PMJMesh2::InputData::getRangeProportion() const
{
	return this->rangeProportion;
}

void PMJMesh2::InputData::setImprovementInSlavesEnabled(bool improvementInSlavesEnabled)
{
	this->improvementInSlavesEnabled = improvementInSlavesEnabled;
}

bool PMJMesh2::InputData::isImprovementInSlavesEnabled() const
{
	return this->improvementInSlavesEnabled;
}

void PMJMesh2::InputData::setImprovementLayers(UInt improvementLayers)
{
    this->improvementLayers = improvementLayers;
}

UInt PMJMesh2::InputData::getImprovementLayers() const
{
    return this->improvementLayers;
}

void PMJMesh2::InputData::setUpdateType(enum MeshUpdateType updateType)
{
	this->updateType = updateType;
}

enum MeshUpdateType PMJMesh2::InputData::getUpdateType() const
{
	return this->updateType;
}

void PMJMesh2::InputData::setFinalMeshState(enum FinalMeshState finalMeshState)
{
    this->finalMeshState = finalMeshState;
}

enum FinalMeshState PMJMesh2::InputData::getFinalMeshState() const
{
    return this->finalMeshState;
}

void PMJMesh2::InputData::setTolerance(Real tolerance)
{
	this->tolerance = tolerance;
}

Real PMJMesh2::InputData::getTolerance() const
{
	return this->tolerance;
}

#if USE_GUI
void PMJMesh2::InputData::setStepByStep(bool stepByStep)
{
	this->stepByStep = stepByStep;
}

bool PMJMesh2::InputData::isStepByStep() const
{
	return this->stepByStep;
}
#endif //#if USE_GUI

UInt PMJMesh2::InputData::packSize(const Parallel::Communicator *comm) const
{
	UInt size = 0;

	//char - inputFileName, partitionBSPFileName
#if USE_FILE_BASED_BSP
	size += comm->packSize(static_cast<UInt>(this->inputFileName.size() + this->partitionBSPFileName.size()), Parallel::Communicator::CHAR);
#else
	size += comm->packSize(static_cast<UInt>(this->inputFileName.size()), Parallel::Communicator::CHAR);
#endif //#if USE_FILE_BASED_BSP

	// unsigned - type, inputFileName.size, partitionBSPFileName.size,
	//            dimension, numImproves, maxBackTracks,
	//            numThreads, numTasksPerThread, removeLayersInLoad,
	//            partitionerType, interfaceMeshGenerationType,
	//            improvementLayers, updateType, finalMeshState
#if USE_FILE_BASED_BSP
	size += comm->packSize(14, Parallel::Communicator::UINT);
#else
	size += comm->packSize(13, Parallel::Communicator::UINT);
#endif //#if USE_FILE_BASED_BSP

	// unsigned long - idVertex, idFrontElement, idElement,
	size += comm->packSize(3, Parallel::Communicator::ULINT);

	// bool - boundarySorted, hotellingTransformEnabled, usingSearchTrees,
	//        meshSorted, localBackTrackingEnabled,
	//        heightDependentInLoad, sharedParallelismEnabled,
	//        creatingInnerFrontInDecomposition, improvementInSlavesEnabled,
	//        stepByStep
#if USE_NUMERICAL
#if USE_GUI
	size += comm->packSize(10, Parallel::Communicator::BOOL);
#else //#if USE_GUI
	size += comm->packSize(9, Parallel::Communicator::BOOL);
#endif //#if USE_GUI
#else
#if USE_GUI
	size += comm->packSize(9, Parallel::Communicator::BOOL);
#else //#if USE_GUI
	size += comm->packSize(8, Parallel::Communicator::BOOL);
#endif //#if USE_GUI
#endif //#if USE_NUMERICAL

	// real - treeFactor, smoothingFactor, rangeProportion, tolerance,
	//        loadThreshold, interfaceBadAngleCos, interfaceShiftDelta,
	//        interfaceElementSizeFactor, removedLayersProportions
#if USE_REMOVED_LAYERS_PROPORTIONS
	size += comm->packSize(8 + this->removeLayersInLoad, Parallel::Communicator::REAL);
#else
	size += comm->packSize(8, Parallel::Communicator::REAL);
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

	return size;
}

void PMJMesh2::InputData::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
{
    comm->pack(this->type, buffer, size, position);

	comm->pack(static_cast<UInt>(this->inputFileName.size()), buffer, size, position);
	comm->pack(this->inputFileName.c_str(), static_cast<Int>(this->inputFileName.size()), buffer, size, position);

	comm->pack(this->dimension, buffer, size, position);

	comm->pack(this->idVertex, buffer, size, position);
	comm->pack(this->idFrontElement, buffer, size, position);
	comm->pack(this->idElement, buffer, size, position);

	comm->pack(this->boundarySorted, buffer, size, position);
#if USE_NUMERICAL
	comm->pack(this->hotellingTransformEnabled, buffer, size, position);
#endif //#if USE_NUMERICAL
	comm->pack(this->treeFactor, buffer, size, position);
	comm->pack(this->usingSearchTrees, buffer, size, position);
	comm->pack(this->numImproves, buffer, size, position);
	comm->pack(this->smoothingFactor, buffer, size, position);
	comm->pack(this->meshSorted, buffer, size, position);
	comm->pack(this->localBackTrackingEnabled, buffer, size, position);
	comm->pack(this->maxBackTracks, buffer, size, position);

	comm->pack(this->numThreads, buffer, size, position);
	comm->pack(this->numTasksPerThread, buffer, size, position);
	comm->pack(this->heightDependentInLoad, buffer, size, position);
	comm->pack(this->removeLayersInLoad, buffer, size, position);
#if USE_REMOVED_LAYERS_PROPORTIONS
	for (UInt i = 0; i < this->removeLayersInLoad; i++)
    {
        comm->pack(this->removedLayersProportions[i], buffer, size, position);
    }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS
    comm->pack(this->loadThreshold, buffer, size, position);
	comm->pack(this->interfaceBadAngleCos, buffer, size, position);
	comm->pack(this->interfaceShiftDelta, buffer, size, position);
	comm->pack(static_cast<UInt>(this->partitionerType), buffer, size, position);
#if USE_FILE_BASED_BSP
	comm->pack(static_cast<UInt>(this->partitionBSPFileName.size()), buffer, size, position);
	comm->pack(this->partitionBSPFileName.c_str(), static_cast<Int>(this->partitionBSPFileName.size()), buffer, size, position);
#endif //#if USE_FILE_BASED_BSP
	comm->pack(static_cast<UInt>(this->interfaceMeshGenerationType), buffer, size, position);
	comm->pack(this->interfaceElementSizeFactor, buffer, size, position);
	comm->pack(this->sharedParallelismEnabled, buffer, size, position);
	comm->pack(this->creatingInnerFrontInDecomposition, buffer, size, position);
	comm->pack(this->rangeProportion, buffer, size, position);
	comm->pack(this->improvementInSlavesEnabled, buffer, size, position);
	comm->pack(this->improvementLayers, buffer, size, position);
	comm->pack(static_cast<UInt>(this->updateType), buffer, size, position);
	comm->pack(static_cast<UInt>(this->finalMeshState), buffer, size, position);

	comm->pack(this->tolerance, buffer, size, position);

#if USE_GUI
	comm->pack(this->stepByStep, buffer, size, position);
#endif //#if USE_GUI;
}

Parallel::Package PMJMesh2::InputData::pack(const Parallel::Communicator *comm) const
{
	Int size = this->packSize(comm);
	char *buffer = NULL;
	Int position = 0;

	buffer = new char[size];

	this->pack(comm, buffer, size, position);

	Parallel::Package p(size, buffer);

	return p;
}

void PMJMesh2::InputData::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
	Int size = p.first;
	char *buffer = p.second;

	if (position == 0)
	{
		UInt ignore = 0;

		comm->unpack(ignore, buffer, size, position);
	}

	UInt uint = 0;
	comm->unpack(uint, buffer, size, position);
	char *inputFileName = new char[uint];
	comm->unpack(inputFileName, uint, buffer, size, position);
	this->inputFileName.assign(inputFileName, uint);
	delete [] inputFileName;

	comm->unpack(this->dimension, buffer, size, position);

	comm->unpack(this->idVertex, buffer, size, position);
	comm->unpack(this->idFrontElement, buffer, size, position);
	comm->unpack(this->idElement, buffer, size, position);

	comm->unpack(this->boundarySorted, buffer, size, position);
#if USE_NUMERICAL
	comm->unpack(this->hotellingTransformEnabled, buffer, size, position);
#endif //#if USE_NUMERICAL
	comm->unpack(this->treeFactor, buffer, size, position);
	comm->unpack(this->usingSearchTrees, buffer, size, position);
	comm->unpack(this->numImproves, buffer, size, position);
	comm->unpack(this->smoothingFactor, buffer, size, position);
	comm->unpack(this->meshSorted, buffer, size, position);
	comm->unpack(this->localBackTrackingEnabled, buffer, size, position);
	comm->unpack(this->maxBackTracks, buffer, size, position);

	comm->unpack(this->numThreads, buffer, size, position);
	comm->unpack(this->numTasksPerThread, buffer, size, position);
	comm->unpack(this->heightDependentInLoad, buffer, size, position);
	comm->unpack(this->removeLayersInLoad, buffer, size, position);
#if USE_REMOVED_LAYERS_PROPORTIONS
	this->removedLayersProportions.resize(this->removeLayersInLoad);
	for (UInt i = 0; i < this->removeLayersInLoad; i++)
    {
        comm->unpack(this->removedLayersProportions[i], buffer, size, position);
    }
#endif //#if USE_REMOVED_LAYERS_PROPORTIONS

    comm->unpack(this->loadThreshold, buffer, size, position);
    comm->unpack(this->interfaceBadAngleCos, buffer, size, position);
    comm->unpack(this->interfaceShiftDelta, buffer, size, position);

    comm->unpack(uint, buffer, size, position);
    this->partitionerType = static_cast<PMJMesh2::PartitionerType>(uint);

#if USE_FILE_BASED_BSP
    comm->unpack(uint, buffer, size, position);
	char *partitionBSPFileName = new char[uint];
	comm->unpack(partitionBSPFileName, uint, buffer, size, position);
	this->partitionBSPFileName.assign(partitionBSPFileName, uint);
	delete [] partitionBSPFileName;
#endif //#if USE_FILE_BASED_BSP

    comm->unpack(uint, buffer, size, position);
    this->interfaceMeshGenerationType = static_cast<PMJMesh2::InterfaceMeshGenerationType>(uint);

    comm->unpack(this->interfaceElementSizeFactor, buffer, size, position);
    comm->unpack(this->sharedParallelismEnabled, buffer, size, position);
    comm->unpack(this->creatingInnerFrontInDecomposition, buffer, size, position);
    comm->unpack(this->rangeProportion, buffer, size, position);
    comm->unpack(this->improvementInSlavesEnabled, buffer, size, position);
    comm->unpack(this->improvementLayers, buffer, size, position);

    comm->unpack(uint, buffer, size, position);
    this->updateType = static_cast<PMJMesh2::MeshUpdateType>(uint);

	comm->unpack(uint, buffer, size, position);
    this->finalMeshState = static_cast<PMJMesh2::FinalMeshState>(uint);

    comm->unpack(this->tolerance, buffer, size, position);

#if USE_GUI
	comm->unpack(this->stepByStep, buffer, size, position);
#endif //#if USE_GUI
}
