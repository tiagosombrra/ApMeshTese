#ifndef _PMJMESH_DEFINITIONS_H_
#define _PMJMESH_DEFINITIONS_H_

#include "MJMesh/Definitions.h"
#include "Parallel/Definitions.h"

#define USE_UNIQUE_BOUNDARY FALSE
#define USE_UNIQUE_SEARCH_TREE FALSE

#define USE_FEL_MAP_IN_TREE_CONSTRUCTION TRUE
#define USE_NEW_FRONT_UPDATER TRUE
#define USE_WORKER_CELL_CLASSIFIER_2 ((USE_NEW_FRONT_UPDATER) && (TRUE))

#define USE_THREAD_COLOR ((USE_GUI) && (TRUE))

#if USE_GUI
#ifdef USE_WORKER_FILE_LOADER
#undef USE_WORKER_FILE_LOADER
#endif //#ifdef USE_WORKER_FILE_LOADER

#define USE_WORKER_FILE_LOADER FALSE
#endif //#if USE_GUI

#define DEBUG_TIMING_WORKER_CELL_CLASSIFIER ((DEBUG_TIMING_PMTOOLS) && (!USE_WORKER_CELL_CLASSIFIER_2) && (FALSE))

namespace PMJMesh
{
	using Data::Short;
	using Data::UShort;
	using Data::Int;
	using Data::LInt;
	using Data::UInt;
	using Data::ULInt;
	using Data::Real;
	using Data::LReal;

	//Data
	class QuadTree;
	class QuadTreeCell;
	class OctTree;
	class OctTreeCell;

	// PMJMesh
	class TaskManager;
	class GlobalTaskManager;
	class AdjacencyGlobalTaskManager;
	class LocalTaskManager;
	class CovarianceLocalTaskManager;
	class APrioriTaskManager;
	class SecondaryPartitionBuilder;

	// PMJMesh::Performer
	class AngleTreeCellClassifier;

#if USE_NEW_FRONT_UPDATER
	class AbstractMeshUpdater;

	class ActualFrontUpdater;

	class MeshUpdater;
	class PartialMeshUpdater;
	//class NoBoundaryPartialMeshUpdater;
	class EmptyMeshUpdater;
	//class NoBoundaryEmptyMeshUpdater;
	class FinalMeshUpdater;
#else
	//this is a superclass for all the actual implemented front updaters. this
	// front updater also updates the mesh.
	class FrontUpdater;
	//this class updates the front and all the mesh, so that the mesh
	// improvement can be applied in all the mesh.
	class FullFrontUpdater;
	//this class updates the front, but only updates the mesh partially, so that
	// the mesh improvement can be applied in only the updated part of the mesh.
	class PartialFrontUpdater;
	//this class updates the front, but not the mesh, so that the mesh
	// improvement has to retrieve the partial mesh before its application.
	class EmptyFrontUpdater;
	//this class updates the front like the EmptyFrontUpdater, but the final
	// improvement is done only on layers of mesh the final front, instead of
	// the several fronts, like in PartialFrontUpdater or EmptyFrontUpdater
	class FinalFrontUpdater;
#endif //#if USE_NEW_FRONT_UPDATER

	class CrossingReplacer;

	class TreeUpdater;

	class MasterCellClassifier;
	class WorkerMainDrive;
	class WorkerCellClassifier;
#if (USE_NEW_FRONT_UPDATER)
	class WorkerCellClassifier2;
#endif //#if (USE_NEW_FRONT_UPDATER)
	class WorkerLaplacianSmoother;
	class WorkerMeshImprover;

	// PMJMesh::MJMesh::Data
	class InputData;
	class Front;
	class PartitionQuadTree;
	class PartitionQuadTreeCell;
	class PartitionOctTree;
	class PartitionOctTreeCell;
	class PartitionBSPTree;
	class PartitionBSPTreeCell;
	class VertexSearchTree;
	class FrontElementSearchTree;

	// PMJMesh::MJMesh::Performer
	class BoundaryBuilder;
	class HotellingTransformer;
	class TreeGenerator;
	class MinTreeRefiner;
	class WorkerMinTreeRefiner;
	class WorkerMinTreeDisrefiner;
	class NeighborTreeRefiner;
	class PositionTreeRefiner;
	class VertexSearchTreeGenerator;
	class FrontElementSearchTreeGenerator;
	class BoxTreeShifter;
	class BSPTreeShifter;
	class GeometryFrontAdvancer;
	class TopologyFrontAdvancer;
	class BackTrackingFrontAdvancer;
	class MeshBackTracker;
	class LoadCalculator;
	class PosterioriLoadCalculator;

	// PMJMesh::Parallel::Data
	class Communicator;
	class MJMeshTask;
	class BoundaryTransferable;
	class TreeTransferable;
	class MeshTransferable;
	class Partition;

	// PMJMesh::Parallel::Performer
	//class PartitionTreeGenerator;
	//class PartitionNeighborTreeRefiner;
	class TreePartitioner;
	class MaxLoadTreePartitioner;
	class AverageLoadTreePartitioner;
	class BoundaryLoadTreePartitioner;
	class MinNumberTreePartitioner;
	class BoundaryNumberTreePartitioner;
	class BSPTreePartitioner;
	class MinNumberBSPTreePartitioner;
	class BoundaryMinNumberBSPTreePartitioner;
	class SmartMinNumberBSPTreePartitioner;
	class BoundarySmartMinNumberBSPTreePartitioner;

	class FrontPartitioner;

	class MainDrive;

	enum //transferable objects
	{
		TRANSFERABLE_MESSAGE = 0,
		TRANSFERABLE_INPUT_DATA = 1,
		TRANSFERABLE_BOUNDARY,
		TRANSFERABLE_TREE,
		TRANSFERABLE_MESH,
		TRANSFERABLE_PARTITION,
		TRANSFERABLE_MJMESH_TASK
	};

	enum TaskType
	{
		TASK_GEOMETRY_FRONT_ADVANCER = 1,
		TASK_MJMESH_MAIN_DRIVE
	};

	enum //messages
	{

	};

	enum PartitionerType
	{
		BOX_LOAD = 1,
		BOX_NUMBER,
		BSP_NUMBER,
		SMART_BSP_NUMBER
	};

	enum MeshUpdateType
	{
		FULL = 1,
		PARTIAL,
		PARTIAL_NO_BOUNDARY,
		EMPTY,
		EMPTY_NO_BOUNDARY,
		FINAL
	};

	enum ShiftType
	{
		NONE = 0, //no shift
		LATERAL,
		DIAGONAL,
		LATERAL_DIAGONAL,
		DIAGONAL_VERTEX, //3D only
		LATERAL_DIAGONAL_VERTEX, //3D only
		DIAGONAL_DIAGONAL_VERTEX, //3D only
		LATERAL_DIAGONAL_DIAGONAL_VERTEX //3D only
	};

	enum AvailableAlgorithms
	{
		GLOBAL_SHIFT,
		GLOBAL_SHIFT_WITH_ADJACENCY,
		LOCAL_SHIFT,
		LOCAL_SHIFT_WITH_COVARIANCE,
		A_PRIORI_INTERFACE_MESH,
	};

	//typedef std::vector<Data::PositionMap> PositionMapVector;

#if USE_C__11
	typedef std::unordered_multimap<UInt, Data::Vertex *> VertexMultiHashMap;
	typedef std::unordered_map<ULInt, Data::FrontElementList> FEListHashMap;
	typedef std::unordered_map<Data::BoxTreeCell *, Data::FrontElementList> CellFEListHashMap;
#endif //#if USE_C__11

	typedef std::multimap<UInt, Data::Vertex *> VertexMultiMap;
	typedef std::map<ULInt, Data::FrontElementList> FEListMap;
	typedef std::map<Data::BoxTreeCell *, Data::FrontElementList> CellFEListMap;

#if (!USE_GUI)
	std::string help(const std::string &programName);

	int execute(int argc, char *argv[]);
#endif //#if (!USE_GUI)

	void setPosition(UInt dimension, Data::BoxTreeCell *cell, enum Data::Position pos);
	void setPosition(UInt dimension, const Data::BoxTree *tree, ULInt id, enum Data::Position pos);
	enum Data::Position getPosition(UInt dimension, const Data::BoxTreeCell *cell);
	enum Data::Position getPosition(UInt dimension, const Data::BoxTree *tree, ULInt id);

#if USE_POOL
	void initThreadPools(const Parallel::ThreadManager *tm);
#if USE_OPENMP
	void mergeThreadPools();
#endif //#if USE_OPENMP
	void finalizeThreadPools(const Parallel::ThreadManager *tm);
#if USE_OPENMP
	void resizeThreadPools();
#endif //#if USE_OPENMP
#endif //#if USE_POOL
}

#endif //#ifndef _PMJMESH_DEFINITIONS_H_
