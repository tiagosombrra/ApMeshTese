#include "MJMesh/MainDrive.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/HotellingTransformer.h"
#include "MJMesh/TreeGenerator.h"
#include "MJMesh/MinTreeRefiner.h"
#include "MJMesh/NeighborTreeRefiner.h"
#include "MJMesh/GeometryFrontAdvancer.h"
#include "MJMesh/TopologyFrontAdvancer.h"
#include "MJMesh/BackTrackingFrontAdvancer.h"
#include "MJMesh/MeshImprover.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/QuadTree.h"
#include "MJMesh/OctTree.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"

#include "Performer/SequentialIdManager.h"

#include "Data/State.h"
#include "Data/Point.h"
#include "Data/Point2D.h"
#include "Data/GeometricShape.h"
#include "Data/Triangle2D.h"
#include "Data/Triangle3D.h"
#include "Data/Vertex.h"
#include "Data/Element.h"
#include "Data/FrontElement.h"

#include "IO/MJMeshFileReader.h"
#include "IO/MJMeshFileWriter.h"
#include "IO/JmeshFileReader.h"
#include "IO/JmeshFileWriter.h"
#include "IO/TextMeshFileReader.h"
#include "IO/TextMeshFileWriter.h"
#include "IO/NeutralFileReader.h"
#include "IO/NeutralFileWriter.h"
#include "IO/GmshMshFileReader.h"
#include "IO/STLFileReader.h"
#include "IO/VTKFileReader.h"
#include "IO/CDBFileReader.h"
#include "IO/OFFFileReader.h"
#include "IO/XMLFileReader.h"
#include "IO/AbaqusFileReader.h"
#include "IO/AbaqusFileWriter.h"
#include "IO/GenFileReader.h"
#include "IO/GenFileWriter.h"

//debug
#include "Data/BoxSearchTree.h"
//endebug

using namespace MJMesh;

const Performer::Status MJMesh::MainDrive::boundaryBuilt =
	Performer::Status(MJMesh::MainDrive::BOUNDARY_BUILT, "boundary built");
#if USE_NUMERICAL
const Performer::Status MJMesh::MainDrive::hotellingTransformApplied =
	Performer::Status(MJMesh::MainDrive::HOTELLING_TRANSFORM_APPLIED, "hotelling transform applied");
#endif //#if USE_NUMERICAL
const Performer::Status MJMesh::MainDrive::treeGenerated =
	Performer::Status(MJMesh::MainDrive::TREE_GENERATED, "tree generated");
const Performer::Status MJMesh::MainDrive::treeMinRefined =
	Performer::Status(MJMesh::MainDrive::TREE_MIN_REFINED, "tree refined - minimum size");
const Performer::Status MJMesh::MainDrive::treeNeighborRefined =
	Performer::Status(MJMesh::MainDrive::TREE_NEIGHBOR_REFINED, "tree refined - neighbors");
const Performer::Status MJMesh::MainDrive::frontGeometryAdvanced =
	Performer::Status(MJMesh::MainDrive::FRONT_GEOMETRY_ADVANCED, "front advanced - geometry");
const Performer::Status MJMesh::MainDrive::frontTopologyAdvanced =
	Performer::Status(MJMesh::MainDrive::FRONT_TOPOLOGY_ADVANCED, "front advanced - topology");
const Performer::Status MJMesh::MainDrive::frontBackTrackingAdvanced =
	Performer::Status(MJMesh::MainDrive::FRONT_BACK_TRACKING_ADVANCED, "front advanced - backtracking");
const Performer::Status MJMesh::MainDrive::meshImproved =
	Performer::Status(MJMesh::MainDrive::MESH_IMPROVED, "mesh improved");
#if USE_NUMERICAL
const Performer::Status MJMesh::MainDrive::hotellingBackTransformApplied =
	Performer::Status(MJMesh::MainDrive::HOTELLING_BACK_TRANSFORM_APPLIED, "hotelling back transform applied");
#endif //#if USE_NUMERICAL

const Performer::Status MJMesh::MainDrive::errorDimension =
	Performer::Status(MJMesh::MainDrive::ERROR, "error - dimension is neither 2 nor 3");
const Performer::Status MJMesh::MainDrive::errorPointOutOfTree =
	Performer::Status(MJMesh::MainDrive::ERROR_POINT_OUT_OF_TREE, "error - point out of bounding box");
const Performer::Status MJMesh::MainDrive::errorMeshNotGeneratedTopology =
	Performer::Status(MJMesh::MainDrive::ERROR_MESH_NOT_GENERATED_TOPOLOGY, "error - mesh not generated on topology phase");
const Performer::Status MJMesh::MainDrive::errorMeshNotGeneratedBackTracking =
	Performer::Status(MJMesh::MainDrive::ERROR_MESH_NOT_GENERATED_BACK_TRACKING, "error - mesh not generated on back tracking phase");

const UInt MJMesh::MainDrive::QUALITY_RANGE = 10;

MJMesh::MainDrive::MainDrive() :
	Performer::MainDrive()
{
	this->idManager = NULL;

	this->dimension = 0;

	this->setBoundarySorted(true);
	//this->setBoundarySorted(false);
#if USE_NUMERICAL
	this->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL
	this->setUsingSearchTrees(true);
	this->setUsingUniformRefinement(true);
	this->setTreeFactor(1.0);
	this->setNumImproves(5);
	//this->setSmoothingFactor(0.5);
	this->setSmoothingFactor(1.0);
	this->setMeshSorted(true);
	this->setLocalBackTrackingEnabled(true);
	this->setMaxBackTracks(10);

	this->boundary = NULL;
	this->tree = NULL;
	this->vertexSearchTree = NULL;
	this->frontElementSearchTree = NULL;
	this->front = NULL;
	this->mesh = NULL;

	this->boundaryBuilder = NULL;
#if USE_NUMERICAL
	this->hotellingTransformer = NULL;
#endif //#if USE_NUMERICAL
	this->treeGenerator = NULL;
	this->minTreeRefiner = NULL;
	this->neighborRefiner = NULL;
	this->geometryAdvancer = NULL;
	this->topologyAdvancer = NULL;
	this->backTracking = NULL;
	this->improver = NULL;

	this->minTreeLevel = 0;

	this->status = MJMesh::MainDrive::notInit;
}

MJMesh::MainDrive::~MainDrive()
{
	this->clear();
}

UInt MJMesh::MainDrive::getDimension() const
{
	return this->dimension;
}

void MJMesh::MainDrive::setBoundarySorted(bool boundarySorted)
{
	this->boundarySorted = boundarySorted;
}

bool MJMesh::MainDrive::isBoundarySorted() const
{
	return this->boundarySorted;
}

#if USE_NUMERICAL
void MJMesh::MainDrive::setHotellingTransformEnabled(bool hotellingTransformEnabled)
{
    this->hotellingTransformEnabled = hotellingTransformEnabled;
}

bool MJMesh::MainDrive::isHotellingTransformEnabled() const
{
    return this->hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void MJMesh::MainDrive::setTreeFactor(Real treeFactor)
{
	this->treeFactor = treeFactor;
}

void MJMesh::MainDrive::setUsingSearchTrees(bool usingSearchTrees)
{
	this->usingSearchTrees = usingSearchTrees;
}

bool MJMesh::MainDrive::isUsingSearchTrees() const
{
	return this->usingSearchTrees;
}

void MJMesh::MainDrive::setUsingUniformRefinement(bool usingUniformRefinement)
{
	this->usingUniformRefinement = usingUniformRefinement;
}

bool MJMesh::MainDrive::isUsingUniformRefinement() const
{
	return this->usingUniformRefinement;
}

Real MJMesh::MainDrive::getTreeFactor() const
{
	return this->treeFactor;
}

void MJMesh::MainDrive::setNumImproves(UInt numImproves)
{
	this->numImproves = numImproves;
}

UInt MJMesh::MainDrive::getNumImproves() const
{
	return this->numImproves;
}

void MJMesh::MainDrive::setSmoothingFactor(Real smoothingFactor)
{
	this->smoothingFactor = smoothingFactor;
}

Real MJMesh::MainDrive::getSmoothingFactor() const
{
	return this->smoothingFactor;
}

void MJMesh::MainDrive::setMeshSorted(bool meshSorted)
{
	this->meshSorted = meshSorted;
}

bool MJMesh::MainDrive::isMeshSorted() const
{
	return this->meshSorted;
}

void MJMesh::MainDrive::setLocalBackTrackingEnabled(bool localBackTrackingEnabled)
{
	this->localBackTrackingEnabled = localBackTrackingEnabled;
}

bool MJMesh::MainDrive::isLocalBackTrackingEnabled() const
{
	return this->localBackTrackingEnabled;
}

void MJMesh::MainDrive::setMaxBackTracks(UInt maxBackTracks)
{
	this->maxBackTracks = maxBackTracks;
}

UInt MJMesh::MainDrive::getMaxBackTracks() const
{
	return this->maxBackTracks;
}

bool MJMesh::MainDrive::loadFile()
{
	if (this->inputFileName.empty())
	{
		return false;
	}

	size_t pos = this->inputFileName.find_last_of('.');

	if (pos == std::string::npos)
	{
		return false;
	}

	std::string extension = this->inputFileName.substr(pos);

	if (extension.size() == 0)
	{
		return false;
	}

	if ((extension == ".2djmsh") || (extension == ".3djmsh") ||
		(extension == ".dmsh") || (extension == ".jmsh") || (extension == ".wmsh") ||
        (extension == ".msh") ||
		(extension == ".stl") || (extension == ".vtk") || (extension == ".cdb") ||
		(extension == ".off") || (extension == ".inp")
#if USE_TINYXML
		|| (extension == ".xml")
#endif //#if USE_TINYXML
		)
	{
		return this->loadInputFile(this->inputFileName);
	}
	else if ((extension == ".txt") || (extension == ".dat") || (extension == ".pos"))
	{
		return this->loadOutputFile(this->inputFileName);
	}
	else if (extension == ".gen")
    {
        if (this->loadInputFile(this->inputFileName))
        {
            return true;
        }

        return this->loadOutputFile(this->inputFileName);
    }

	return false;
}

bool MJMesh::MainDrive::saveFile() const
{
	if (this->outputFileName.empty())
	{
		return false;
	}

	std::string extension = this->outputFileName.substr(this->outputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

	if ((extension == ".dat") || (extension == ".txt"))
	{
		return this->saveOutputFile(this->outputFileName);
	}
	else if ((extension == ".2djmsh") || (extension == ".3djmsh") ||
			 (extension == ".dmsh") || (extension == ".jmsh") || (extension == ".wmsh") ||
			 (extension == ".inp"))
	{
		return this->saveInputFile(this->outputFileName);
	}
	else if (extension == ".gen")
    {
        if (this->saveOutputFile(this->outputFileName))
        {
            return true;
        }

        return this->saveInputFile(this->outputFileName);
    }
    else if ((extension == ".bbgen") || (extension == ".bvgen") || (extension == ".bsgen") ||
			 (extension == ".mbgen") || (extension == ".mvgen") || (extension == ".mbgen"))
    {
        return IO::GenFileWriter::write(this->outputFileName, this->getBoundary(), this->getMesh());
    }

	return false;
}

bool MJMesh::MainDrive::loadInputFile(const std::string &inputFileName)
{
	std::string extension = inputFileName.substr(inputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

	this->boundaryBuilder = static_cast<MJMesh::BoundaryBuilder *>(this->makeBoundaryBuilder());

	if (extension == ".2djmsh")
	{
		this->dimension = 2;
		return this->loadJMeshFile(inputFileName);
	}
	else if (extension == ".3djmsh")
	{
		this->dimension = 3;
		return this->loadJMeshFile(inputFileName);
	}
	else if (extension == ".dmsh")
	{
		this->dimension = 3;
		return this->loadDMshFile(inputFileName);
	}
	else if (extension == ".jmsh")
	{
		this->dimension = 3;
		return this->loadJMshFile(inputFileName);
	}
	else if (extension == ".wmsh")
	{
		this->dimension = 3;
		return this->loadWMshFile(inputFileName);
	}
	else if (extension == ".msh")
	{
		this->dimension = 3;
		return this->loadGmshMshFile(inputFileName);
	}
	else if (extension == ".stl")
	{
		this->dimension = 3;
		return this->loadSTLFile(inputFileName);
	}
	else if (extension == ".vtk")
	{
		this->dimension = 3;
		return this->loadVTKFile(inputFileName);
	}
	else if (extension == ".cdb")
	{
		this->dimension = 3;
		return this->loadCDBFile(inputFileName);
	}
	else if (extension == ".off")
	{
		this->dimension = 3;
		return this->loadOFFFile(inputFileName);
	}
	else if (extension == ".inp")
	{
		this->dimension = 3;
		return this->loadAbaqusFile(inputFileName);
	}
#if USE_TINYXML
	else if (extension == ".xml")
	{
		this->dimension = 3;
		return this->loadXMLFile(inputFileName);
	}
#endif //#if USE_TINYXML
	else if (extension == ".gen")
	{
		return this->loadInputGenFile(inputFileName);
	}

	return false;
}

bool MJMesh::MainDrive::saveInputFile(const std::string &outputFileName) const
{
	std::string extension = outputFileName.substr(outputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

#if USE_GUI
	if ((!this->getBoundary()) || (this->getBoundary()->size() == 0))
#else
	if ((!this->boundary) || (this->boundary->size() == 0))
#endif //#if USE_GUI
	{
		return false;
	}

	if ((extension == ".2djmsh") || (extension == ".3djmsh"))
	{
		return this->saveJMeshFile(outputFileName);
	}
	else if (extension == ".dmsh")
	{
		return this->saveDMshFile(outputFileName);
	}
	else if (extension == ".jmsh")
	{
		return this->saveJMshFile(outputFileName);
	}
	else if (extension == ".wmsh")
	{
		return this->saveWMshFile(outputFileName);
	}
	else if (extension == ".inp")
	{
		return this->saveAbaqusFile(outputFileName);
	}
	else if (extension == ".gen")
	{
		return this->saveInputGenFile(outputFileName);
	}

	return false;
}

bool MJMesh::MainDrive::loadOutputFile(const std::string &inputFileName)
{
	std::string extension = inputFileName.substr(inputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

	if (this->boundary)
	{
		delete this->boundary;

		this->boundary = NULL;
	}

	//this->mesh = new MJMesh::Mesh();

	bool ok = false;

	if (extension == ".txt")
	{
		ok = this->loadTxtFile(inputFileName);
	}
	else if ((extension == ".dat") || (extension == ".pos"))
	{
		ok = this->loadNeutralFile(inputFileName);
	}
	else if (extension == ".gen")
	{
		ok = this->loadOutputGenFile(inputFileName);
	}

	if (ok)
	{
		this->findQuality();
	}

	return ok;
}

bool MJMesh::MainDrive::saveOutputFile(const std::string &outputFileName) const
{
	std::string extension = outputFileName.substr(outputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

	if ((this->dimension != 2) && (this->dimension != 3))
	{
		return false;
	}

#if USE_GUI
	if ((!this->getMesh()) || (this->getMesh()->size() == 0))
#else
	if ((!this->mesh) || (this->mesh->size() == 0))
#endif //#if USE_GUI
	{
		return false;
	}

	if (extension == ".txt")
	{
		return this->saveTxtFile(outputFileName);
	}
	else if (extension == ".dat")
	{
		return this->saveNeutralFile(outputFileName);
	}
	else if (extension == ".gen")
	{
		return this->saveOutputGenFile(outputFileName);
	}

	return false;
}

void MJMesh::MainDrive::add(ULInt id, Real x, Real y)
{
	if (!this->boundaryBuilder)
	{
		this->boundaryBuilder = this->makeBoundaryBuilder();
	}

	static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->add(id, x, y);
	this->dimension = 2;
}

void MJMesh::MainDrive::add(ULInt id, Real x, Real y, Real z)
{
	if (!this->boundaryBuilder)
	{
		this->boundaryBuilder = this->makeBoundaryBuilder();
	}

	static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->add(id, x, y, z);
	this->dimension = 3;
}

void MJMesh::MainDrive::add(ULInt id, ULInt id1, ULInt id2)
{
	if (!this->boundaryBuilder)
	{
		return;
	}

	static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->add(id, id1, id2);
}

void MJMesh::MainDrive::add(ULInt id, ULInt id1, ULInt id2, ULInt id3)
{
	if (!this->boundaryBuilder)
	{
		return;
	}

	static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->add(id, id1, id2, id3);
}

#if USE_QUADS
void MJMesh::MainDrive::add(ULInt id, ULInt id1, ULInt id2, ULInt id3, ULInt id4)
{
	if (!this->boundaryBuilder)
	{
		return;
	}

	static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->add(id, id1, id2, id3, id4);
}
#endif //#if USE_QUADS

//Iterating through all vertices
Data::Mesh::VertexIterator MJMesh::MainDrive::vBegin()
{
	return this->mesh ? this->mesh->vBegin() : Data::Mesh::VertexIterator();
}

Data::Mesh::ConstVertexIterator MJMesh::MainDrive::vBegin() const
{
	return this->mesh ? this->mesh->vBegin() : Data::Mesh::ConstVertexIterator();
}

Data::Mesh::VertexIterator MJMesh::MainDrive::vEnd()
{
	return this->mesh ? this->mesh->vEnd() : Data::Mesh::VertexIterator();
}

Data::Mesh::ConstVertexIterator MJMesh::MainDrive::vEnd() const
{
	return this->mesh ? this->mesh->vEnd() : Data::Mesh::ConstVertexIterator();
}

//Iterating through internal vertices
#if USE_EXPERIMENTAL_MESH
Data::Mesh::InnerVertexIterator MJMesh::MainDrive::ivBegin()
#else
Data::Mesh::VertexIterator MJMesh::MainDrive::ivBegin()
#endif //#if USE_EXPERIMENTAL_MESH
{
#if USE_EXPERIMENTAL_MESH
	return this->mesh ? this->mesh->ivBegin() : Data::Mesh::InnerVertexIterator();
#else
	return this->mesh ? this->mesh->ivBegin() : Data::Mesh::VertexIterator();
#endif //#if USE_EXPERIMENTAL_MESH
}

#if USE_EXPERIMENTAL_MESH
Data::Mesh::ConstInnerVertexIterator MJMesh::MainDrive::ivBegin() const
#else
Data::Mesh::ConstVertexIterator MJMesh::MainDrive::ivBegin() const
#endif //#if USE_EXPERIMENTAL_MESH
{
#if USE_EXPERIMENTAL_MESH
	return this->mesh ? const_cast<const MJMesh::Mesh *>(this->mesh)->ivBegin() : Data::Mesh::ConstInnerVertexIterator();
#else
	return this->mesh ? this->mesh->ivBegin() : Data::Mesh::ConstVertexIterator();
#endif //#if USE_EXPERIMENTAL_MESH
}

#if USE_EXPERIMENTAL_MESH
Data::Mesh::InnerVertexIterator MJMesh::MainDrive::ivEnd()
#else
Data::Mesh::VertexIterator MJMesh::MainDrive::ivEnd()
#endif //#if USE_EXPERIMENTAL_MESH
{
#if USE_EXPERIMENTAL_MESH
	return this->mesh ? this->mesh->ivEnd() : Data::Mesh::InnerVertexIterator();
#else
	return this->mesh ? this->mesh->ivEnd() : Data::Mesh::VertexIterator();
#endif //#if USE_EXPERIMENTAL_MESH
}

#if USE_EXPERIMENTAL_MESH
Data::Mesh::ConstInnerVertexIterator MJMesh::MainDrive::ivEnd() const
#else
Data::Mesh::ConstVertexIterator MJMesh::MainDrive::ivEnd() const
#endif //#if USE_EXPERIMENTAL_MESH
{
#if USE_EXPERIMENTAL_MESH
	return this->mesh ? const_cast<const MJMesh::Mesh *>(this->mesh)->ivEnd() : Data::Mesh::ConstInnerVertexIterator();
#else
	return this->mesh ? this->mesh->ivEnd() : Data::Mesh::ConstVertexIterator();
#endif //#if USE_EXPERIMENTAL_MESH
}

//Iterating through elements
Data::Mesh::ElementIterator MJMesh::MainDrive::eBegin()
{
	return this->mesh ? this->mesh->eBegin() : Data::Mesh::ElementIterator();
}

Data::Mesh::ConstElementIterator MJMesh::MainDrive::eBegin() const
{
	return this->mesh ? this->mesh->eBegin() : Data::Mesh::ConstElementIterator();
}

Data::Mesh::ElementIterator MJMesh::MainDrive::eEnd()
{
	return this->mesh ? this->mesh->eEnd() : Data::Mesh::ElementIterator();
}

Data::Mesh::ConstElementIterator MJMesh::MainDrive::eEnd() const
{
	return this->mesh ? this->mesh->eEnd() : Data::Mesh::ConstElementIterator();
}

void MJMesh::MainDrive::execute()
{
	if ((this->status.getType() == MJMesh::MainDrive::success.getType()) ||
		(this->status.getType() >= MJMesh::MainDrive::error.getType()))
	{
		return;
	}

#if USE_TIME_MEASUREMENTS
	Real start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

#if DEBUG_TIMING_PMJMESH
	Real timeInit, timeBoundaryBuilder, timeHotellingTransform, timeTree, timeAdvFront, timeImprovement, timeHotellingBackTransform, timeOverhead, debugruntime;
	timeInit = timeBoundaryBuilder = timeHotellingTransform = timeTree = timeAdvFront = timeImprovement = timeHotellingBackTransform = timeOverhead = debugruntime = 0.0;

	Real debugstart = Data::time();
#else
	//Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	switch (this->status.getType())
	{
	case MJMesh::MainDrive::NOT_INIT:
		if (!this->executeInit())
		{
			break;
		}

#if DEBUG_TIMING_PMJMESH
		timeInit = Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

		//std::cout << "Serial, MainDrive, time for executeInit() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

	case MJMesh::MainDrive::INIT:
		//begin adjusting boundary

		if (!this->executeBoundaryBuilder(true))
		{
			break;
		}

#if DEBUG_TIMING_PMJMESH
		timeBoundaryBuilder = Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

		//std::cout << "Serial, MainDrive, time for executeBoundaryBuilder() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

		//end adjusting boundary
	case MJMesh::MainDrive::BOUNDARY_BUILT:
#if USE_NUMERICAL
        //begin hotelling transform
        if (!this->executeHotellingTransform())
		{
			break;
		}

#if DEBUG_TIMING_PMJMESH
		timeHotellingTransform = Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

		//std::cout << "Serial, MainDrive, time for executeHotellingTransform() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

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

		//std::cout << "Serial, MainDrive, time for executeTreeGenerator() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::TREE_GENERATED:
		if (!this->executeTreeMinRefiner(true))
		{
			break;
		}

		//std::cout << "Serial, MainDrive, time for executeTreeMinRefiner() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::TREE_MIN_REFINED:
		if (!this->executeTreeNeighborRefiner(true))
		{
			break;
		}

#if DEBUG_TIMING_PMJMESH
		timeTree = Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

		//std::cout << "Serial, MainDrive, time for executeTreeNeighborRefiner() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

/*#if ((USE_TIME_MEASUREMENTS) && (USE_TIMING))
		{
			Real end = Data::time();
			std::cout << "Time for generating tree = " << end - start << " seconds" << std::endl;
		}
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))*/
		//end adjusting tree

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::TREE_NEIGHBOR_REFINED:
		//begin mesh generation

		if (!this->executeFrontGeometryAdvancer())
		{
			break;
		}

		//std::cout << "Serial, MainDrive, time for executeFrontGeometryAdvancer() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::FRONT_GEOMETRY_ADVANCED:
		if (!this->executeFrontTopologyAdvancer())
		{
			break;
		}

		//std::cout << "Serial, MainDrive, time for executeFrontTopologyAdvancer() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::FRONT_TOPOLOGY_ADVANCED:
		if (!this->executeFrontBackTrackingAdvancer())
		{
			break;
		}

#if DEBUG_TIMING_PMJMESH
		timeAdvFront = Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

		//end mesh generation

		//std::cout << "Serial, MainDrive, time for executeFrontBackTrackingAdvancer() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MJMesh::MainDrive::FRONT_BACK_TRACKING_ADVANCED:
		//begin improvement

		if (!this->executeMeshImprover())
		{
			break;
		}

#if DEBUG_TIMING_PMJMESH
		timeImprovement = Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

		//end improvement

		//std::cout << "Serial, MainDrive, time for executeMeshImprover() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

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

#if DEBUG_TIMING_PMJMESH
		timeHotellingBackTransform = Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

		//std::cout << "Serial, MainDrive, time for executeHotellingBackTransform() = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();

#if USE_GUI
		if ((this->isHotellingTransformEnabled()) && (Data::isStepByStep())) break;
#endif //#if USE_GUI

        //end hotelling back transform
	case MJMesh::MainDrive::HOTELLING_BACK_TRANSFORM_APPLIED:
#endif //#if USE_NUMERICAL
		{
#if USE_TIME_MEASUREMENTS
			Real end = Data::time();
			std::cout << "Runtime = " << end - start << " seconds" << std::endl;
#endif //#if USE_TIME_MEASUREMENTS

#if DEBUG_TIMING_PMJMESH
			debugruntime = end - start;
#endif //#if DEBUG_TIMING_PMJMESH
		}

		if (!this->executeFinalize())
		{
			break;
		}

		//std::cout << "Serial, MainDrive, time for executeFinalize() = " << Data::time() - debugstart << std::endl;
	}

#if DEBUG_TIMING_PMJMESH
	timeOverhead = debugruntime - timeInit - timeBoundaryBuilder - timeHotellingTransform - timeTree - timeAdvFront - timeImprovement - timeHotellingBackTransform;

	std::cout << "Timings:" << std::endl;

	std::cout << "initializing: " << timeInit << " (" << 100.0*timeInit/debugruntime << " %)" << std::endl;

	std::cout << "building boundary: " << timeBoundaryBuilder << " (" << 100.0*timeBoundaryBuilder/debugruntime << " %)" << std::endl;

#if USE_NUMERICAL
	std::cout << "hotelling transform: " << timeHotellingTransform << " (" << 100.0*timeHotellingTransform/debugruntime << " %)" << std::endl;
#endif //#if USE_NUMERICAL

	std::cout << "generating octree: " << timeTree << " (" << 100.0*timeTree/debugruntime << " %)" << std::endl;

	std::cout << "generating mesh: " << timeAdvFront << " (" << 100.0*timeAdvFront/debugruntime << " %)" << std::endl;

	std::cout << "improving mesh: " << timeImprovement << " (" << 100.0*timeImprovement/debugruntime << " %)" << std::endl;

#if USE_NUMERICAL
	std::cout << "hotelling back transform: " << timeHotellingBackTransform << " (" << 100.0*timeHotellingBackTransform/debugruntime << " %)" << std::endl;
#endif //#if USE_NUMERICAL

	std::cout << "overhead: " << timeOverhead << " (" << 100.0*timeOverhead/debugruntime << " %)" << std::endl;

	/*std::cout << std::endl << "FrontAdvancer timings:" << std::endl;
	std::cout << "initializing: " << Performer::FrontAdvancer::timeInit << std::endl;
	std::cout << "initializing (search trees): " << Performer::FrontAdvancer::timeInitMakingTrees << std::endl;
	std::cout << "executing: " << Performer::FrontAdvancer::timeExecute << std::endl;
	std::cout << "executing (is empty): " << Performer::FrontAdvancer::timeIsEmpty << std::endl;
	std::cout << "executing (next): " << Performer::FrontAdvancer::timeNext << std::endl;
	std::cout << "executing (next geometry): " << Performer::FrontAdvancer::timeNextGeometry << std::endl;
	std::cout << "executing (next geometry reject): " << Performer::FrontAdvancer::timeNextGeometryReject << std::endl;
	std::cout << "executing (next geometry out): " << Performer::FrontAdvancer::timeNextGeometryOut << std::endl;
	std::cout << "executing (next topology): " << Performer::FrontAdvancer::timeNextTopology << std::endl;
	std::cout << "executing (next topology reject): " << Performer::FrontAdvancer::timeNextTopologyReject << std::endl;
	std::cout << "executing (best): " << Performer::FrontAdvancer::timeBest << std::endl;
	std::cout << "executing (best geometry ideal): " << Performer::FrontAdvancer::timeIdeal << std::endl;
	std::cout << "executing (best geometry radius): " << Performer::FrontAdvancer::timeRadius << std::endl;
	std::cout << "executing (best geometry restricted): " << Performer::FrontAdvancer::timeRestricted << std::endl;
	std::cout << "executing (best geometry candidates): " << Performer::FrontAdvancer::timeCandidates << std::endl;
	std::cout << "executing (best geometry fels in range): " << Performer::FrontAdvancer::timeFrontElementsInRange << std::endl;
	std::cout << "executing (best geometry candidates best valid): " << Performer::FrontAdvancer::timeCandidatesBestValid << std::endl;
	std::cout << "executing (best geometry ideals best valid): " << Performer::FrontAdvancer::timeIdealBestValid << std::endl;
	std::cout << "executing (best topology candidates): " << Performer::FrontAdvancer::timeTopologyCandidates << std::endl;
	std::cout << "executing (best topology candidates best valid): " << Performer::FrontAdvancer::timeTopologyBestValid << std::endl;
	std::cout << "executing (reject): " << Performer::FrontAdvancer::timeReject << std::endl;
	std::cout << "executing (status check): " << Performer::FrontAdvancer::timeStatusCheck << std::endl;
	std::cout << "executing (error check): " << Performer::FrontAdvancer::timeErrorCheck << std::endl;
	std::cout << "executing (advance): " << Performer::FrontAdvancer::timeAdvance << std::endl;
	std::cout << "ending: " << Performer::FrontAdvancer::timeEnd << std::endl;
	std::cout << "num advs geometry: " << Performer::FrontAdvancer::numBestWithInsertion << std::endl;
	std::cout << "num advs topology: " << Performer::FrontAdvancer::numBestWithoutInsertion << std::endl;

	std::cout << std::endl << "BoxSearchTree timings:" << std::endl;
	std::cout << "initializing: " << Data::BoxSearchTree::timeInit << std::endl;
	std::cout << "executing: " << Data::BoxSearchTree::timeExecute << std::endl;
	std::cout << "ending: " << Data::BoxSearchTree::timeEnd << std::endl;*/
#endif //#if DEBUG_TIMING_PMJMESH

#if USE_TIME_MEASUREMENTS
	Real end = Data::time();
	this->runtime += end - start;
#endif //#if USE_TIME_MEASUREMENTS
}

void MJMesh::MainDrive::clear()
{
	this->status = MJMesh::MainDrive::notInit;

	if (this->idManager)
	{
		delete this->idManager;

		this->idManager = NULL;
	}

	this->dimension = 0;

	this->setBoundarySorted(true);
#if USE_NUMERICAL
	this->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL
	this->setTreeFactor(1.0);
	this->setNumImproves(5);
	//this->setSmoothingFactor(0.5);
	this->setSmoothingFactor(1.0);
	this->setMeshSorted(true);
	this->setLocalBackTrackingEnabled(true);
	this->setMaxBackTracks(10);

	this->quality.clear();

	if (this->front)
	{
		delete this->front;

		this->front = NULL;
	}

	if (this->tree)
	{
		delete this->tree;

		this->tree = NULL;
	}

	if (this->mesh)
	{
		if (this->boundary)
		{
			//Data::VertexVector vertices;
			Data::VertexList vertices;
			//Data::FrontElementVector elements;
			Data::FrontElementList elements;

			this->boundary->setVertices(vertices);
			this->boundary->setElements(elements);
		}

		delete this->mesh;

		this->mesh = NULL;
	}

	if (this->boundary)
	{
#if USE_GUI
		if ((this->boundaryBuilder) && (this->boundaryBuilder->getBoundary() == this->boundary))
		{
			this->boundaryBuilder->setBoundary(NULL);
		}
#endif //#if USE_GUI

		delete this->boundary;

		this->boundary = NULL;
	}

	if (this->boundaryBuilder)
	{
		if (this->boundaryBuilder->getBoundary())
		{
			delete this->boundaryBuilder->getBoundary();

#if USE_GUI
			this->boundaryBuilder->setBoundary(NULL);
#endif //#if USE_GUI
		}

		delete this->boundaryBuilder;

		this->boundaryBuilder = NULL;
	}

#if USE_NUMERICAL
	if (this->hotellingTransformer)
    {
        delete this->hotellingTransformer;

        this->hotellingTransformer = NULL;
    }
#endif //#if USE_NUMERICAL

	if (this->treeGenerator)
	{
		delete this->treeGenerator;

		this->treeGenerator = NULL;
	}

	if (this->minTreeRefiner)
	{
		delete this->minTreeRefiner;

		this->minTreeRefiner = NULL;
	}

	if (this->neighborRefiner)
	{
		delete this->neighborRefiner;

		this->neighborRefiner = NULL;
	}

	if (this->geometryAdvancer)
	{
		delete this->geometryAdvancer;

		this->geometryAdvancer = NULL;
	}

	if (this->topologyAdvancer)
	{
		delete this->topologyAdvancer;

		this->topologyAdvancer = NULL;
	}

	if (this->backTracking)
	{
		delete this->backTracking;

		this->backTracking = NULL;
	}

	if (this->improver)
	{
		delete this->improver;

		this->improver = NULL;
	}
}

void MJMesh::MainDrive::setDimension(UInt dimension)
{
	this->dimension = dimension;

	if (this->treeGenerator)
	{
		dynamic_cast<MJMesh::TreeGenerator *>(this->treeGenerator)->setDimension(dimension);
	}

	if (this->improver)
	{
		static_cast<MJMesh::MeshImprover *>(this->improver)->setDimension(dimension);
	}
}

#if USE_GUI
void MJMesh::MainDrive::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;

	if (this->boundaryBuilder)
	{
		this->boundaryBuilder->setBoundary(boundary);
	}

	if (this->treeGenerator)
	{
		dynamic_cast<MJMesh::TreeGenerator *>(this->treeGenerator)->setBoundary(boundary);
	}

	if (this->minTreeRefiner)
	{
		this->minTreeRefiner->setBoundary(boundary);
	}

	if (this->geometryAdvancer)
	{
		this->geometryAdvancer->setBoundary(boundary);
	}

	if (this->topologyAdvancer)
	{
		this->topologyAdvancer->setBoundary(boundary);
	}

	if (this->backTracking)
	{
		this->backTracking->setBoundary(boundary);
	}

	if (this->improver)
	{
		this->improver->setBoundary(boundary);
	}
}

void MJMesh::MainDrive::setTree(Data::BoxTree *tree)
{
	this->tree = tree;

	if (this->treeGenerator)
	{
		this->treeGenerator->setTree(tree);
	}

	if (this->minTreeRefiner)
	{
		this->minTreeRefiner->setTree(tree);
	}

	if (this->neighborRefiner)
	{
		this->neighborRefiner->setTree(tree);
	}

	if (this->geometryAdvancer)
	{
		this->geometryAdvancer->setTree(tree);
	}

	if (this->topologyAdvancer)
	{
		this->topologyAdvancer->setTree(tree);
	}

	if (this->backTracking)
	{
		this->backTracking->setTree(tree);
	}
}

void MJMesh::MainDrive::setFront(MJMesh::Front *front)
{
	this->front = front;

	if (this->geometryAdvancer)
	{
		this->geometryAdvancer->setFront(front);
	}

	if (this->topologyAdvancer)
	{
		this->topologyAdvancer->setFront(front);
	}

	if (this->backTracking)
	{
		this->backTracking->setFront(front);
	}

	if (this->improver)
	{
		this->improver->setFront(front);
	}
}

void MJMesh::MainDrive::setMesh(MJMesh::Mesh *mesh)
{
	this->mesh = mesh;

	if (this->geometryAdvancer)
	{
		this->geometryAdvancer->setMesh(mesh);
	}

	if (this->topologyAdvancer)
	{
		this->topologyAdvancer->setMesh(mesh);
	}

	if (this->backTracking)
	{
		this->backTracking->setMesh(mesh);
	}

	if (this->improver)
	{
		this->improver->setMesh(mesh);
	}
}
#endif //#if USE_GUI

MJMesh::BoundaryBuilder *MJMesh::MainDrive::getBoundaryBuilder(bool make)
{
	if (make)
	{
		if (this->boundaryBuilder)
		{
			delete this->boundaryBuilder;
		}

		this->boundaryBuilder = this->makeBoundaryBuilder();
	}

	return static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder);
}

const Data::Boundary *MJMesh::MainDrive::getBoundary() const
{
#if USE_GUI
	if (this->boundary)
	{
		return this->boundary;
	}

	if (this->boundaryBuilder)
	{
		return this->boundaryBuilder->getBoundary();
	}

	return NULL;
#else
	return this->boundary;
#endif //#if USE_GUI
}

const Data::BoxTree *MJMesh::MainDrive::getTree() const
{
#if USE_GUI
	if (this->tree)
	{
		return this->tree;
	}

	if (this->treeGenerator)
	{
		return static_cast<Data::BoxTree *>(this->treeGenerator->getTree());
	}

	return NULL;
#else
	return this->tree;
#endif //#if USE_GUI
}

const MJMesh::Front *MJMesh::MainDrive::getFront() const
{
#if USE_GUI
	if (this->front)
	{
		return this->front;
	}

	if (this->geometryAdvancer)
	{
		return static_cast<MJMesh::Front *>(this->geometryAdvancer->getFront());
	}

	return NULL;
#else
	return this->front;
#endif //#if USE_GUI
}

const MJMesh::Mesh *MJMesh::MainDrive::getMesh() const
{
#if USE_GUI
	if (this->mesh)
	{
		return this->mesh;
	}

	if (this->geometryAdvancer)
	{
		return static_cast<MJMesh::Mesh *>(this->geometryAdvancer->getMesh());
	}

	return NULL;
#else
	return this->mesh;
#endif //#if USE_GUI
}

#if USE_TIME_MEASUREMENTS
Real MJMesh::MainDrive::getRuntime() const
{
	return this->runtime;
}
#endif //#if USE_TIME_MEASUREMENTS

std::string MJMesh::MainDrive::qualityString() const
{
	if (!this->mesh)
	{
		return "";
	}

	std::stringstream str;

	str << this->quality.qualityString();

#if USE_TIME_MEASUREMENTS
	str << std::endl;

	ULInt size = this->mesh ? this->mesh->size() : 0;

	if (size == 0)
    {
        size = 1;
    }

	str << "Runtime: " << this->runtime << " seconds" << std::endl;
	str << "Elements per second: " << static_cast<Real>(size)/static_cast<Real>(this->runtime);
#endif //#if USE_TIME_MEASUREMENTS

	return str.str();
}

void MJMesh::MainDrive::findQuality()
{
#if USE_GUI
    Real scale[3][10];

    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_0, scale[0][0], scale[1][0], scale[2][0]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_1, scale[0][1], scale[1][1], scale[2][1]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_2, scale[0][2], scale[1][2], scale[2][2]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_3, scale[0][3], scale[1][3], scale[2][3]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_4, scale[0][4], scale[1][4], scale[2][4]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_5, scale[0][5], scale[1][5], scale[2][5]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_6, scale[0][6], scale[1][6], scale[2][6]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_7, scale[0][7], scale[1][7], scale[2][7]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_8, scale[0][8], scale[1][8], scale[2][8]);
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_ELEMENT_QUALITY_9, scale[0][9], scale[1][9], scale[2][9]);
#endif //#if USE_GUI

	//if (this->mesh)
	{
#if USE_GUI
		this->quality.findQuality(this->mesh, this->QUALITY_RANGE, &scale[0][0]);
#else
		this->quality.findQuality(this->mesh, this->QUALITY_RANGE);
#endif //#if USE_GUI
	}
}

Data::UI2Tuple MJMesh::MainDrive::measure() const
{
	static ULInt current = 0;
	static ULInt total = 0;
	static ULInt prevFrontSize = 0;

	LInt difference = (this->front) ? static_cast<LInt>(this->front->size()) - static_cast<LInt>(prevFrontSize) : 0;

	switch (this->status.getType())
	{
	case MJMesh::MainDrive::SUCCESS:
		current = 1;
		total = 1;
		break;

	case MJMesh::MainDrive::NOT_INIT:
		current = 0;
		total = 1;
		break;

	case MJMesh::MainDrive::INIT:
		current = 0;
		total = 1;
		break;

	case MJMesh::MainDrive::BOUNDARY_BUILT:
		current = 1;
		total = 10;
		break;

#if USE_NUMERICAL
	case MJMesh::MainDrive::HOTELLING_TRANSFORM_APPLIED:
		current = 1;
		total = 10;
		break;
#endif //#if USE_NUMERICAL

	case MJMesh::MainDrive::TREE_GENERATED:
	case MJMesh::MainDrive::TREE_MIN_REFINED:
		current = 2;
		total = 10;
		break;

	case MJMesh::MainDrive::TREE_NEIGHBOR_REFINED:
	case MJMesh::MainDrive::FRONT_GEOMETRY_ADVANCED:
		if (prevFrontSize == 0)
		{
			current = 0;
			total = this->boundary->size();
		}

		if (this->dimension == 2)
		{
			switch (difference)
			{
			case 1:
				current += 1;
				total += 2;
				break;
			case -1:
				current += 2;
				total += 1;
				break;
			case -3:
				current += 3;
				break;
			}
		}
		else
		{
			switch (difference)
			{
			case 2:
				current += 1;
				total += 3;
				break;
			case 0:
				current += 2;
				total += 2;
				break;
			case -2:
				current += 3;
				total += 1;
				break;
			case -4:
				current += 4;
				break;
			}
		}

		prevFrontSize = (this->front) ? this->front->size() : this->boundary->size();
		break;

	case MJMesh::MainDrive::FRONT_TOPOLOGY_ADVANCED:
	case MJMesh::MainDrive::FRONT_BACK_TRACKING_ADVANCED:
	case MJMesh::MainDrive::MESH_IMPROVED:
		current = 1;
		total = 2;
		break;

#if USE_NUMERICAL
	case MJMesh::MainDrive::HOTELLING_BACK_TRANSFORM_APPLIED:
		current = 9;
		total = 10;
		break;
#endif //#if USE_NUMERICAL

	case MJMesh::MainDrive::ERROR:
		current = 1;
		total = 2;
	}

#if USE_C__11
	return std::make_tuple(current, total);
#else
	Data::UI2Tuple t;

	t.values[0] = current;
	t.values[1] = total;

	return t;
#endif //#if USE_C__11
}

bool MJMesh::MainDrive::loadJMeshFile(const std::string &fileName)
{
	IO::MJMeshFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	this->dimension = reader.getDimension();
	this->treeFactor = reader.getTreeFactor();
	Data::setTolerance(reader.getTolerance());

	return (this->dimension != 0);
}

bool MJMesh::MainDrive::loadDMshFile(const std::string &fileName)
{
	IO::JmeshFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	this->treeFactor = reader.getTreeFactor();
	Data::setTolerance(reader.getTolerance());

	return (this->treeFactor > 0.0);
}

bool MJMesh::MainDrive::loadJMshFile(const std::string &fileName)
{
	IO::JmeshFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	this->treeFactor = reader.getTreeFactor();
	Data::setTolerance(reader.getTolerance());

	return (this->treeFactor > 0.0);
}

bool MJMesh::MainDrive::loadWMshFile(const std::string &fileName)
{
	IO::JmeshFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	this->treeFactor = reader.getTreeFactor();
	Data::setTolerance(reader.getTolerance());

	return (this->treeFactor > 0.0);
}

bool MJMesh::MainDrive::loadGmshMshFile(const std::string &fileName)
{
	IO::GmshMshFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	this->dimension = reader.getDimension();

	if (this->dimension == 2)
    {
        this->treeFactor = 0.85;
    }
    else if (this->dimension == 3)
    {
        this->treeFactor = 0.4;
    }

	Data::setTolerance(0.0001);

	return ((this->dimension == 2) || (this->dimension == 3));
}

bool MJMesh::MainDrive::loadSTLFile(const std::string &fileName)
{
	IO::STLFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	this->treeFactor = 0.4;

	return (true);
}

bool MJMesh::MainDrive::loadVTKFile(const std::string &fileName)
{
	IO::VTKFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	this->treeFactor = 0.4;

	return (true);
}

bool MJMesh::MainDrive::loadCDBFile(const std::string &fileName)
{
	IO::CDBFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	this->treeFactor = 0.4;

	if (!reader.isBoundary())
	{
		this->mesh = static_cast<MJMesh::Mesh *>(reader.getMesh());
	}

	return (true);
}

bool MJMesh::MainDrive::loadOFFFile(const std::string &fileName)
{
	IO::OFFFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	this->treeFactor = 0.4;

	return (true);
}

bool MJMesh::MainDrive::loadAbaqusFile(const std::string &fileName)
{
	IO::AbaqusFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	this->treeFactor = 0.4;

	return (true);
}

#if USE_TINYXML
bool MJMesh::MainDrive::loadXMLFile(const std::string &fileName)
{
	IO::XMLFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	if ((!reader.isBoundary()) && (reader.isMesh()))
	{
		this->mesh = static_cast<MJMesh::Mesh *>(reader.getMesh());
	}

	this->treeFactor = 0.4;

#if USE_TECGRAF
	if ((!reader.isBoundary()) && (!reader.isMesh()))
	{
		delete reader.getModel();
	}
#endif //#if USE_TECGRAF

	return ((reader.isBoundary()) || (reader.isMesh()));
}
#endif //#if USE_TINYXML

bool MJMesh::MainDrive::loadInputGenFile(const std::string &fileName)
{
    IO::GenFileReader reader;

    reader.setFilename(fileName);
    reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

    reader.execute();

    if (reader.isBoundary())
    {
        this->dimension = reader.getDimension();
        this->treeFactor = 0.4;

        return true;
    }

    delete reader.getMesh();

    return false;
}

bool MJMesh::MainDrive::loadOutputGenFile(const std::string &fileName)
{
    IO::GenFileReader reader;

    reader.setFilename(fileName);
    reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

    reader.execute();

    if (!reader.isBoundary())
    {
        this->dimension = reader.getDimension();
        this->mesh = static_cast<MJMesh::Mesh *>(reader.getMesh());

        return true;
    }

    static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->clear();

    return false;
}

bool MJMesh::MainDrive::loadTxtFile(const std::string &fileName)
{
	IO::TextMeshFileReader reader;

	reader.setFilename(fileName);

	reader.execute();

	this->mesh = static_cast<MJMesh::Mesh *>(reader.getMesh());
	this->dimension = reader.getDimension();

	return (this->dimension != 0);
}

bool MJMesh::MainDrive::loadNeutralFile(const std::string &fileName)
{
	this->boundaryBuilder = static_cast<MJMesh::BoundaryBuilder *>(this->makeBoundaryBuilder());

	IO::NeutralFileReader reader;

	reader.setFilename(fileName);
	reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

	reader.execute();

	if (!reader.isBoundary())
	{
		this->mesh = static_cast<MJMesh::Mesh *>(reader.getMesh());
	}

	this->dimension = reader.getDimension();

	return (this->dimension != 0);
}

bool MJMesh::MainDrive::saveJMeshFile(const std::string &fileName) const
{
	IO::MJMeshFileWriter writer;

	writer.setFilename(fileName);
	writer.setMesh(this->getBoundary());
	writer.setDimension(this->dimension);
	writer.setTreeFactor(this->treeFactor);
	writer.setTolerance(Data::getTolerance());

	writer.execute();

	return true;
}

bool MJMesh::MainDrive::saveDMshFile(const std::string &fileName) const
{
	IO::JmeshFileWriter writer;

	writer.setFilename(fileName);
	writer.setMesh(this->getBoundary());
	writer.setTreeFactor(this->treeFactor);
	writer.setTolerance(Data::getTolerance());

	writer.execute();

	return true;
}

bool MJMesh::MainDrive::saveJMshFile(const std::string &fileName) const
{
	IO::JmeshFileWriter writer;

	writer.setFilename(fileName);
	writer.setMesh(this->getBoundary());
	writer.setTreeFactor(this->treeFactor);
	writer.setTolerance(Data::getTolerance());

	writer.execute();

	return true;
}

bool MJMesh::MainDrive::saveWMshFile(const std::string &fileName) const
{
	IO::JmeshFileWriter writer;

	writer.setFilename(fileName);
	writer.setMesh(this->getBoundary());
	writer.setTreeFactor(this->treeFactor);
	writer.setTolerance(Data::getTolerance());

	writer.execute();

	return true;
}

bool MJMesh::MainDrive::saveAbaqusFile(const std::string &fileName) const
{
	IO::AbaqusFileWriter writer;

	writer.setFilename(fileName);
	writer.setMesh(this->getBoundary());

	writer.execute();

	return true;
}

bool MJMesh::MainDrive::saveTxtFile(const std::string &fileName) const
{
	IO::TextMeshFileWriter writer;

	writer.setFilename(fileName);
	writer.setMesh(this->getMesh());
	writer.setDimension(this->dimension);

	writer.execute();

	return true;
}

bool MJMesh::MainDrive::saveNeutralFile(const std::string &fileName) const
{
	IO::NeutralFileWriter writer;

	writer.setFilename(fileName);

	if ((!this->getMesh()) || (this->getMesh()->size() == 0))
	{
		writer.setMesh(this->getBoundary());
		writer.setBoundary(true);
	}
	else
	{
		writer.setMesh(this->getMesh());
		writer.setBoundary(false);
	}

	writer.setDimension(this->dimension);

	writer.execute();

	return true;
}

bool MJMesh::MainDrive::saveInputGenFile(const std::string &fileName) const
{
    if (!this->getBoundary())
    {
        return false;
    }

    IO::GenFileWriter writer;

    writer.setFilename(fileName);
    writer.setType(IO::GenFileWriter::BOUNDARY);
    writer.setMesh(this->getBoundary());

    writer.execute();

    return true;
}

bool MJMesh::MainDrive::saveOutputGenFile(const std::string &fileName) const
{
    if (!this->getMesh())
    {
        return false;
    }

    IO::GenFileWriter writer;

    writer.setFilename(fileName);
    writer.setType(IO::GenFileWriter::VOLUME);
    writer.setMesh(this->getMesh());

    writer.execute();

    return true;
}

bool MJMesh::MainDrive::executeInit()
{
	if (!this->boundaryBuilder)
	{
		this->status = MJMesh::MainDrive::notInit;

		return false;
	}

	if ((this->dimension != 2) && (this->dimension != 3))
	{
		this->status = (this->dimension != 0) ?
			MJMesh::MainDrive::errorDimension :
			MJMesh::MainDrive::notInit;

		return false;
	}

#if USE_TIME_MEASUREMENTS
	this->runtime = 0.0;
#endif //#if USE_TIME_MEASUREMENTS

	this->status = MJMesh::MainDrive::init;

	return true;
}

bool MJMesh::MainDrive::executeBoundaryBuilder(bool del)
{
	//this->boundaryBuilder->setSorted(this->boundarySorted);
	static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->setSorted(true);

	this->boundaryBuilder->execute();

	this->idManager = this->makeIdManager(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->getIdManager());

	this->boundary = static_cast<MJMesh::Boundary *>(this->boundaryBuilder->getBoundary());

	this->boundary->setSorted(this->boundarySorted);

	this->status = MJMesh::MainDrive::boundaryBuilt;

	if (del)
	{
		delete this->boundaryBuilder;

		this->boundaryBuilder = NULL;
	}

	return true;
}

#if USE_NUMERICAL
bool MJMesh::MainDrive::executeHotellingTransform()
{
    if (!this->hotellingTransformEnabled)
    {
        this->status = MJMesh::MainDrive::hotellingTransformApplied;

        return true;
    }

    this->hotellingTransformer = this->makeHotellingTransformer();

    this->hotellingTransformer->execute();

    this->status = MJMesh::MainDrive::hotellingTransformApplied;

    return true;
}
#endif //#if USE_NUMERICAL

bool MJMesh::MainDrive::executeTreeGenerator(bool del)
{
	if (!this->treeGenerator)
	{
		this->treeGenerator = this->makeTreeGenerator();

		if (!this->treeGenerator)
		{
			return false;
		}
	}

//debug
#if USE_GUI
	bool stepByStep = Data::isStepByStep();
	Data::setStepByStep(false);
#endif //#if USE_GUI
//endebug

	this->treeGenerator->execute();

//debug
#if USE_GUI
	Data::setStepByStep(stepByStep);
#endif //#if USE_GUI
//endebug

	if (!this->tree)
	{
		this->tree = static_cast<Data::BoxTree *>(treeGenerator->getTree());
	}

	if (this->treeGenerator->getStatus().getType() == dynamic_cast<MJMesh::TreeGenerator *>(this->treeGenerator)->ended.getType())
	{
	    this->minTreeLevel = dynamic_cast<MJMesh::TreeGenerator *>(this->treeGenerator)->getMinLevel();

		this->status = MJMesh::MainDrive::treeGenerated;

		if (del)
		{
			delete this->treeGenerator;

			this->treeGenerator = NULL;
		}
	}

	return true;
}

bool MJMesh::MainDrive::executeTreeMinRefiner(bool del)
{
	if (!this->usingUniformRefinement)
	{
		this->status = MJMesh::MainDrive::treeMinRefined;

		return true;
	}

	if (!this->minTreeRefiner)
	{
		this->minTreeRefiner = this->makeTreeMinRefiner();

		if (!this->minTreeRefiner)
		{
			return false;
		}
	}

//debug
#if USE_GUI
	bool stepByStep = Data::isStepByStep();
	Data::setStepByStep(false);
#endif //#if USE_GUI
//endebug

	this->minTreeRefiner->execute();

//debug
#if USE_GUI
	Data::setStepByStep(stepByStep);
#endif //#if USE_GUI
//endebug

	if (this->minTreeRefiner->getStatus().getType() == this->minTreeRefiner->ended.getType())
	{
		this->status = MJMesh::MainDrive::treeMinRefined;

		if (del)
		{
			delete this->minTreeRefiner;

			this->minTreeRefiner = NULL;
		}
	}

	return true;
}

bool MJMesh::MainDrive::executeTreeNeighborRefiner(bool del)
{
	if (!this->neighborRefiner)
	{
		this->neighborRefiner = this->makeTreeNeighborRefiner();

		if (!this->neighborRefiner)
		{
			return false;
		}
	}

//debug
#if USE_GUI
	bool stepByStep = Data::isStepByStep();
	Data::setStepByStep(false);
#endif //#if USE_GUI
//endebug

	this->neighborRefiner->execute();

//debug
#if USE_GUI
	Data::setStepByStep(stepByStep);
#endif //#if USE_GUI
//endebug

	if (this->neighborRefiner->getStatus().getType() == this->neighborRefiner->ended.getType())
	{
		this->status = MJMesh::MainDrive::treeNeighborRefined;

		if (del)
		{
			delete this->neighborRefiner;

			this->neighborRefiner = NULL;
		}
	}

	return true;
}

bool MJMesh::MainDrive::executeFrontGeometryAdvancer()
{
	if (!this->geometryAdvancer)
	{
		this->geometryAdvancer = this->makeFrontGeometryAdvancer();

		this->geometryAdvancer->setIdManager(this->idManager);
	}

	this->geometryAdvancer->execute();

	if (!this->front)
	{
		this->front = static_cast<MJMesh::Front *>(this->geometryAdvancer->getFront());
	}

	if (!this->mesh)
	{
		this->mesh = static_cast<MJMesh::Mesh *>(this->geometryAdvancer->getMesh());
	}

	if ((this->geometryAdvancer->getStatus().getType() == this->geometryAdvancer->rejected.getType()) ||
		(this->geometryAdvancer->getStatus().getType() == this->geometryAdvancer->ended.getType()))
	{
		this->status = MJMesh::MainDrive::frontGeometryAdvanced;

		delete this->geometryAdvancer;

		this->geometryAdvancer = NULL;
	}
	else if (this->geometryAdvancer->getStatus().getType() == this->geometryAdvancer->error.getType())
	{
		this->status = MJMesh::MainDrive::errorPointOutOfTree;

		delete this->geometryAdvancer;

		this->geometryAdvancer = NULL;

		return false;
	}

	return true;
}

bool MJMesh::MainDrive::executeFrontTopologyAdvancer()
{
	if (!this->topologyAdvancer)
	{
		this->topologyAdvancer = this->makeFrontTopologyAdvancer();

		this->topologyAdvancer->setIdManager(this->idManager);
	}

	this->topologyAdvancer->execute();

	//debug
	if (!this->front)
	{
		this->front = static_cast<MJMesh::Front *>(this->topologyAdvancer->getFront());
	}

	if (!this->mesh)
	{
		this->mesh = static_cast<MJMesh::Mesh *>(this->topologyAdvancer->getMesh());
	}
	//endebug

	if (this->topologyAdvancer->getStatus().getType() == this->topologyAdvancer->ended.getType())
	{
		this->status = MJMesh::MainDrive::frontTopologyAdvanced;

		delete this->topologyAdvancer;

		this->topologyAdvancer = NULL;
	}
	else if (this->topologyAdvancer->getStatus().getType() == this->topologyAdvancer->rejected.getType())
	{
		if (this->dimension == 2)
		{
			this->status = MJMesh::MainDrive::errorMeshNotGeneratedTopology;

			delete this->topologyAdvancer;

			this->topologyAdvancer = NULL;

			return false;
		}
		else if (this->dimension == 3)
		{
			this->status = MJMesh::MainDrive::frontTopologyAdvanced;

			delete this->topologyAdvancer;

			this->topologyAdvancer = NULL;
		}
	}

	return true;
}

bool MJMesh::MainDrive::executeFrontBackTrackingAdvancer()
{
	if (this->dimension == 3)
	{
		if (!this->backTracking)
		{
			this->backTracking = this->makeFrontBackTrackingAdvancer();

			this->backTracking->setIdManager(this->idManager);

			this->front = NULL;
		}

		this->backTracking->execute();

		this->front = static_cast<MJMesh::Front *>(this->backTracking->getFront());

		if (this->backTracking->getStatus().getType() == this->backTracking->ended.getType())
		{
			if (this->front)
			{
				delete this->front;

				this->front = NULL;
			}

			this->status = MJMesh::MainDrive::frontBackTrackingAdvanced;

			delete this->backTracking;

			this->backTracking = NULL;
		}
		else if (this->backTracking->getStatus().getType() == this->backTracking->rejected.getType())
		{
#if USE_GUI
			this->front = static_cast<MJMesh::Front *>(this->backTracking->getFront());
#endif //#if USE_GUI

			this->status = MJMesh::MainDrive::errorMeshNotGeneratedBackTracking;

			delete this->backTracking;

			this->backTracking = NULL;

			return false;
		}
	}
	else
	{
		if (this->front)
		{
			delete this->front;

			this->front = NULL;
		}

		this->status = MJMesh::MainDrive::frontBackTrackingAdvanced;
	}

	return true;
}

bool MJMesh::MainDrive::executeMeshImprover()
{
	if (!this->improver)
	{
		this->improver = this->makeMeshImprover();

		this->improver->setIdManager(this->idManager);
	}

	this->improver->execute();

	if (this->improver->getStatus().getType() == this->improver->ended.getType())
	{
		this->status = MJMesh::MainDrive::meshImproved;

		delete this->improver;

		this->improver = NULL;
	}

	return true;
}

#if USE_NUMERICAL
bool MJMesh::MainDrive::executeHotellingBackTransform()
{
    if (!this->hotellingTransformEnabled)
    {
        this->status = MJMesh::MainDrive::hotellingBackTransformApplied;

        return true;
    }

    this->hotellingTransformer->setMesh(this->mesh);

    this->hotellingTransformer->executeBack();

    delete this->hotellingTransformer;

    this->hotellingTransformer = NULL;

    this->status = MJMesh::MainDrive::hotellingBackTransformApplied;

    return true;
}
#endif //#if USE_NUMERICAL

bool MJMesh::MainDrive::executeFinalize()
{
	this->findQuality();

	this->status = MJMesh::MainDrive::success;

	return true;
}

Performer::IdManager *MJMesh::MainDrive::makeIdManager(const Performer::IdManager *idManager) const
{
	Performer::IdManager *idm = new Performer::SequentialIdManager(1, 3);

	if (idManager)
	{
		idm->setId(0, idManager->current(0) + 1);
		idm->setId(1, idManager->current(1) + 1);
	}

	return idm;
}

Performer::BoundaryBuilder *MJMesh::MainDrive::makeBoundaryBuilder() const
{
	return new MJMesh::BoundaryBuilder(this->boundarySorted);
}

#if USE_NUMERICAL
MJMesh::HotellingTransformer *MJMesh::MainDrive::makeHotellingTransformer() const
{
    MJMesh::HotellingTransformer *hotellingTransformer = new MJMesh::HotellingTransformer();

    hotellingTransformer->setBoundary(this->boundary);
    hotellingTransformer->setMode(Performer::CovarianceMatrixFinder::Mode::NORMAL);
    hotellingTransformer->setFrontElementCentroids(false);
    hotellingTransformer->setWeighted(false);

    return hotellingTransformer;
}
#endif //#if USE_NUMERICAL

Performer::TreeGenerator *MJMesh::MainDrive::makeTreeGenerator() const
{
	return new MJMesh::TreeGenerator(this->dimension, this->boundary, this->treeFactor);
}

MJMesh::MinTreeRefiner *MJMesh::MainDrive::makeTreeMinRefiner() const
{
	return (this->minTreeLevel != 0) ? new MJMesh::MinTreeRefiner(tree, this->minTreeLevel) :
        new MJMesh::MinTreeRefiner(this->tree, this->boundary);
}

MJMesh::NeighborTreeRefiner *MJMesh::MainDrive::makeTreeNeighborRefiner() const
{
	return new MJMesh::NeighborTreeRefiner(this->tree);
}

MJMesh::GeometryFrontAdvancer *MJMesh::MainDrive::makeFrontGeometryAdvancer() const
{
	return new MJMesh::GeometryFrontAdvancer(this->dimension, this->boundary, this->tree, this->usingSearchTrees);
}

MJMesh::TopologyFrontAdvancer *MJMesh::MainDrive::makeFrontTopologyAdvancer() const
{
	return new MJMesh::TopologyFrontAdvancer(this->dimension, this->boundary, this->tree, this->front, this->mesh, this->usingSearchTrees);
	//return new MJMesh::TopologyFrontAdvancer(this->dimension, this->boundary, this->tree, this->front, this->mesh, false);
}

MJMesh::BackTrackingFrontAdvancer *MJMesh::MainDrive::makeFrontBackTrackingAdvancer() const
{
	//return new MJMesh::BackTrackingFrontAdvancer(this->dimension, this->boundary, this->tree, this->mesh, this->front);
	return new MJMesh::BackTrackingFrontAdvancer(this->dimension, this->boundary, this->tree, this->mesh, this->front, true, true, 100*this->dimension, MJMesh::BackTrackingFrontAdvancer::FRONT_ADVANCING);
}

MJMesh::MeshImprover *MJMesh::MainDrive::makeMeshImprover() const
{
	return new MJMesh::MeshImprover(this->dimension, this->mesh, this->front,
		this->tree, this->boundary, this->numImproves, this->smoothingFactor,
		this->localBackTrackingEnabled, this->meshSorted, this->maxBackTracks);
}
