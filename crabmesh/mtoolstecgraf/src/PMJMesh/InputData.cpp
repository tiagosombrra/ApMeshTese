#include "PMJMesh/InputData.h"

#include "PMJMesh/Communicator.h"

using namespace PMJMesh;

PMJMesh::InputData::InputData() :
	Parallel::Transferable(PMJMesh::TRANSFERABLE_INPUT_DATA)
{
	this->setIdVertex(0);
	this->setIdFrontElement(0);
	this->setIdElement(0);

	this->setDimension(0);
	this->setAlgorithm(GLOBAL_SHIFT);

#if USE_NUMERICAL
	this->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL
	this->setBoundarySorted(false);

	this->setTreeFactor(0.0);
	this->setUsingSearchTrees(false);
	this->setRangeProportion(1.0);

	this->setNumImproves(0);
	this->setSmoothingFactor(0.0);
#if (!USE_DELETE_FELS)
	this->setUnsmoothingLayer(0);
#endif //#if (!USE_DELETE_FELS)
	this->setSmoothingLayer(0);
	this->setMeshSorted(false);
	this->setLocalBackTrackingEnabled(false);
	this->setMaxBackTracks(0);

	this->setParallel(true);
	this->setMinRefinementPostponed(false);
	this->setUsePartitioningThreshold(true);
	this->setPartitioningThreshold(0.0);
	this->setLoadBalancer(Parallel::UNDEFINED);

	this->setSkippingInnerInDecomposition(false);
	this->setCreateInnerFrontInDecomposition(false);

	this->setTolerance(0.0);

#if USE_GUI
	this->setStepByStep(false);
#endif //#if USE_GUI
}

PMJMesh::InputData::~InputData()
{

}

void PMJMesh::InputData::setIdVertex(ULInt idVertex)
{
	this->idVertex = idVertex;
}

ULInt PMJMesh::InputData::getIdVertex() const
{
	return this->idVertex;
}

void PMJMesh::InputData::setIdFrontElement(ULInt idFrontElement)
{
	this->idFrontElement = idFrontElement;
}

ULInt PMJMesh::InputData::getIdFrontElement() const
{
	return this->idFrontElement;
}

void PMJMesh::InputData::setIdElement(ULInt idElement)
{
	this->idElement = idElement;
}

ULInt PMJMesh::InputData::getIdElement() const
{
	return this->idElement;
}

void PMJMesh::InputData::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

UInt PMJMesh::InputData::getDimension() const
{
	return this->dimension;
}

void PMJMesh::InputData::setAlgorithm(enum AvailableAlgorithms algorithm)
{
	this->algorithm = algorithm;
}

enum AvailableAlgorithms PMJMesh::InputData::getAlgorithm() const
{
	return this->algorithm;
}

#if USE_NUMERICAL
void PMJMesh::InputData::setHotellingTransformEnabled(bool hotellingTransformEnabled)
{
	this->hotellingTransformEnabled = hotellingTransformEnabled;
}

bool PMJMesh::InputData::isHotellingTransformEnabled() const
{
	return this->hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void PMJMesh::InputData::setBoundarySorted(bool boundarySorted)
{
	this->boundarySorted = boundarySorted;
}

bool PMJMesh::InputData::isBoundarySorted() const
{
	return this->boundarySorted;
}

void PMJMesh::InputData::setTreeFactor(Real treeFactor)
{
	this->treeFactor = treeFactor;
}

Real PMJMesh::InputData::getTreeFactor() const
{
	return this->treeFactor;
}

void PMJMesh::InputData::setUsingSearchTrees(bool usingSearchTrees)
{
	this->usingSearchTrees = usingSearchTrees;
}

bool PMJMesh::InputData::isUsingSearchTrees() const
{
	return this->usingSearchTrees;
}

void PMJMesh::InputData::setRangeProportion(Real rangeProportion)
{
	this->rangeProportion = rangeProportion;
}

Real PMJMesh::InputData::getRangeProportion() const
{
	return this->rangeProportion;
}

void PMJMesh::InputData::setSmoothInSlavesEnabled(bool smoothInSlavesEnabled)
{
	this->smoothInSlavesEnabled = smoothInSlavesEnabled;
}

bool PMJMesh::InputData::isSmoothInSlavesEnabled() const
{
	return this->smoothInSlavesEnabled;
}

void PMJMesh::InputData::setNumImproves(UInt numImproves)
{
	this->numImproves = numImproves;
}

UInt PMJMesh::InputData::getNumImproves() const
{
	return this->numImproves;
}

void PMJMesh::InputData::setSmoothingFactor(Real smoothingFactor)
{
	this->smoothingFactor = smoothingFactor;
}

Real PMJMesh::InputData::getSmoothingFactor() const
{
	return this->smoothingFactor;
}

#if (!USE_DELETE_FELS)
void PMJMesh::InputData::setUnsmoothingLayer(UInt unsmoothingLayer)
{
	this->unsmoothingLayer = unsmoothingLayer;
}

UInt PMJMesh::InputData::getUnsmoothingLayer() const
{
	return this->unsmoothingLayer;
}
#endif //#if (!USE_DELETE_FELS)

void PMJMesh::InputData::setSmoothingLayer(UInt smoothingLayer)
{
	this->smoothingLayer = smoothingLayer;
}

UInt PMJMesh::InputData::getSmoothingLayer() const
{
	return this->smoothingLayer;
}

void PMJMesh::InputData::setMeshSorted(bool meshSorted)
{
	this->meshSorted = meshSorted;
}

bool PMJMesh::InputData::isMeshSorted() const
{
	return this->meshSorted;
}

void PMJMesh::InputData::setLocalBackTrackingEnabled(bool localBackTrackingEnabled)
{
	this->localBackTrackingEnabled = localBackTrackingEnabled;
}

bool PMJMesh::InputData::isLocalBackTrackingEnabled() const
{
	return this->localBackTrackingEnabled;
}

void PMJMesh::InputData::setMaxBackTracks(UInt maxBackTracks)
{
	this->maxBackTracks = maxBackTracks;
}

UInt PMJMesh::InputData::getMaxBackTracks() const
{
	return this->maxBackTracks;
}

void PMJMesh::InputData::setUpdateType(enum MeshUpdateType updateType)
{
	this->updateType = updateType;
}

enum MeshUpdateType PMJMesh::InputData::getUpdateType() const
{
	return this->updateType;
}

void PMJMesh::InputData::setParallel(bool parallel)
{
	this->parallel = parallel;
}

bool PMJMesh::InputData::isParallel() const
{
	return this->parallel;
}

void PMJMesh::InputData::setMinRefinementPostponed(bool minRefinementPostponed)
{
	this->minRefinementPostponed = minRefinementPostponed;
}

bool PMJMesh::InputData::isMinRefinementPostponed() const
{
	return this->minRefinementPostponed;
}

void PMJMesh::InputData::setUsePartitioningThreshold(bool usePartitioningThreshold)
{
	this->usePartitioningThreshold = usePartitioningThreshold;
}

bool PMJMesh::InputData::isUsePartitioningThreshold() const
{
	return this->usePartitioningThreshold;
}

void PMJMesh::InputData::setPartitioningThreshold(Real partitioningThreshold)
{
	this->partitioningThreshold = partitioningThreshold;
}

Real PMJMesh::InputData::getPartitioningThreshold() const
{
	return this->partitioningThreshold;
}

void PMJMesh::InputData::setLoadBalancer(enum Parallel::AvailableLoadBalancers loadBalancer)
{
	this->loadBalancer = loadBalancer;
}

enum Parallel::AvailableLoadBalancers PMJMesh::InputData::getLoadBalancer() const
{
	return this->loadBalancer;
}

void PMJMesh::InputData::setSkippingInnerInDecomposition(bool skippingInnerInDecomposition)
{
	this->skippingInnerInDecomposition = skippingInnerInDecomposition;
}

bool PMJMesh::InputData::isSkippingInnerInDecomposition() const
{
	return this->skippingInnerInDecomposition;
}

void PMJMesh::InputData::setCreateInnerFrontInDecomposition(bool createInnerFrontInDecomposition)
{
	this->createInnerFrontInDecomposition = createInnerFrontInDecomposition;
}

bool PMJMesh::InputData::isCreateInnerFrontInDecomposition() const
{
	return this->createInnerFrontInDecomposition;
}

void PMJMesh::InputData::setTolerance(Real tolerance)
{
	this->tolerance = tolerance;
}

Real PMJMesh::InputData::getTolerance() const
{
	return this->tolerance;
}

#if USE_GUI
void PMJMesh::InputData::setStepByStep(bool stepByStep)
{
	this->stepByStep = stepByStep;
}

bool PMJMesh::InputData::isStepByStep() const
{
	return this->stepByStep;
}
#endif //#if USE_GUI

UInt PMJMesh::InputData::packSize(const Parallel::Communicator *comm) const
{
	UInt size = 0;

	// unsigned - type, dimension, numImproves, unsmoothingLayer, smoothingLayer,
	//            maxBackTracks, updateType, loadBalancer, algorithm
#if USE_DELETE_FELS
	size += comm->packSize(8, Parallel::Communicator::UINT);
#else
	size += comm->packSize(9, Parallel::Communicator::UINT);
#endif //#if USE_DELETE_FELS

	// long - idVertex, idFrontElement, idElement,
	size += comm->packSize(3, Parallel::Communicator::ULINT);

	// bool - boundarySorted, hotellingTransformEnabled, usingSearchTrees, meshSorted,
	//		  localBackTrackingEnabled, parallel, usePartitioningThreshold, stepByStep,
	//        smoothInSlavesEnabled, minRefinementPostponed, skippingInnerInDecomposition,
	//        createInnerFrontInDecomposition
#if USE_NUMERICAL
#if USE_GUI
	size += comm->packSize(12, Parallel::Communicator::BOOL);
#else //#if USE_GUI
	size += comm->packSize(11, Parallel::Communicator::BOOL);
#endif //#if USE_GUI
#else
#if USE_GUI
	size += comm->packSize(11, Parallel::Communicator::BOOL);
#else //#if USE_GUI
	size += comm->packSize(10, Parallel::Communicator::BOOL);
#endif //#if USE_GUI
#endif //#if USE_NUMERICAL

	// real - treeFactor, rangeProportion, smoothingFactor, partitioningThreshold, tolerance
	size += comm->packSize(5, Parallel::Communicator::REAL);

	return size;
}

void PMJMesh::InputData::pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const
{
	comm->pack(this->type, buffer, size, position);

	comm->pack(this->idVertex, buffer, size, position);
	comm->pack(this->idFrontElement, buffer, size, position);
	comm->pack(this->idElement, buffer, size, position);

	comm->pack(this->dimension, buffer, size, position);

	comm->pack(this->algorithm, buffer, size, position);

	comm->pack(this->boundarySorted, buffer, size, position);
#if USE_NUMERICAL
	comm->pack(this->hotellingTransformEnabled, buffer, size, position);
#endif //#if USE_NUMERICAL
	comm->pack(this->treeFactor, buffer, size, position);
	comm->pack(this->usingSearchTrees, buffer, size, position);
	comm->pack(this->rangeProportion, buffer, size, position);

	comm->pack(this->smoothInSlavesEnabled, buffer, size, position);
	comm->pack(this->numImproves, buffer, size, position);
	comm->pack(this->smoothingFactor, buffer, size, position);
#if (!USE_DELETE_FELS)
	comm->pack(this->unsmoothingLayer, buffer, size, position);
#endif //#if (!USE_DELETE_FELS)
	comm->pack(this->smoothingLayer, buffer, size, position);
	comm->pack(this->meshSorted, buffer, size, position);
	comm->pack(this->localBackTrackingEnabled, buffer, size, position);
	comm->pack(this->maxBackTracks, buffer, size, position);

	comm->pack(this->updateType, buffer, size, position);

	comm->pack(this->parallel, buffer, size, position);
	comm->pack(this->minRefinementPostponed, buffer, size, position);
	comm->pack(this->usePartitioningThreshold, buffer, size, position);
	comm->pack(this->partitioningThreshold, buffer, size, position);
	comm->pack(this->loadBalancer, buffer, size, position);

	comm->pack(this->skippingInnerInDecomposition, buffer, size, position);
	comm->pack(this->createInnerFrontInDecomposition, buffer, size, position);

	comm->pack(this->tolerance, buffer, size, position);

#if USE_GUI
	comm->pack(this->stepByStep, buffer, size, position);
#endif //#if USE_GUI
}

Parallel::Package PMJMesh::InputData::pack(const Parallel::Communicator *comm) const
{
	Int size = this->packSize(comm);
	char *buffer = NULL;
	Int position = 0;

	buffer = new char[size];

	this->pack(comm, buffer, size, position);

	Parallel::Package p(size, buffer);

	return p;
}

void PMJMesh::InputData::unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position)
{
	Int size = p.first;
	char *buffer = p.second;

	if (position == 0)
	{
		UInt ignore = 0;

		comm->unpack(ignore, buffer, size, position);
	}

	comm->unpack(this->idVertex, buffer, size, position);
	comm->unpack(this->idFrontElement, buffer, size, position);
	comm->unpack(this->idElement, buffer, size, position);

	comm->unpack(this->dimension, buffer, size, position);

	UInt algorithm = 0;

	comm->unpack(algorithm, buffer, size, position);

	this->algorithm = static_cast<PMJMesh::AvailableAlgorithms>(algorithm);

	comm->unpack(this->boundarySorted, buffer, size, position);
#if USE_NUMERICAL
	comm->unpack(this->hotellingTransformEnabled, buffer, size, position);
#endif //#if USE_NUMERICAL
	comm->unpack(this->treeFactor, buffer, size, position);
	comm->unpack(this->usingSearchTrees, buffer, size, position);
	comm->unpack(this->rangeProportion, buffer, size, position);

	comm->unpack(this->smoothInSlavesEnabled, buffer, size, position);
	comm->unpack(this->numImproves, buffer, size, position);
	comm->unpack(this->smoothingFactor, buffer, size, position);
#if (!USE_DELETE_FELS)
	comm->unpack(this->unsmoothingLayer, buffer, size, position);
#endif //#if (!USE_DELETE_FELS)
	comm->unpack(this->smoothingLayer, buffer, size, position);
	comm->unpack(this->meshSorted, buffer, size, position);
	comm->unpack(this->localBackTrackingEnabled, buffer, size, position);
	comm->unpack(this->maxBackTracks, buffer, size, position);

	UInt updateType = 0;

	comm->unpack(updateType, buffer, size, position);

	this->updateType = static_cast<PMJMesh::MeshUpdateType>(updateType);

	comm->unpack(this->parallel, buffer, size, position);
	comm->unpack(this->minRefinementPostponed, buffer, size, position);
	comm->unpack(this->usePartitioningThreshold, buffer, size, position);
	comm->unpack(this->partitioningThreshold, buffer, size, position);

	UInt loadBalancer = 0;

	comm->unpack(loadBalancer, buffer, size, position);

	this->loadBalancer = static_cast<Parallel::AvailableLoadBalancers>(loadBalancer);

	comm->unpack(this->skippingInnerInDecomposition, buffer, size, position);
	comm->unpack(this->createInnerFrontInDecomposition, buffer, size, position);

	comm->unpack(this->tolerance, buffer, size, position);

#if USE_GUI
	comm->unpack(this->stepByStep, buffer, size, position);
#endif //#if USE_GUI
}
