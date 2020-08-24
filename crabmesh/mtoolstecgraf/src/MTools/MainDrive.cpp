#include "MTools/MainDrive.h"

#if USE_MTOOLS

#include <exception>
#include <cstring>

#include "Data/State.h"
#include "Data/Point3D.h"
#include "Data/OctTree.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/HotellingTransformer.h"

#include "Performer/TreeGenerator.h"

#include "IO/MJMeshFileReader.h"
#include "IO/MJMeshFileWriter.h"
#include "IO/JmeshFileReader.h"
#include "IO/JmeshFileWriter.h"
#include "IO/TextMeshFileReader.h"
#include "IO/TextMeshFileWriter.h"
#include "IO/NeutralFileReader.h"
#include "IO/NeutralFileWriter.h"
#include "IO/STLFileReader.h"
#include "IO/VTKFileReader.h"
#include "IO/CDBFileReader.h"
#include "IO/OFFFileReader.h"
#include "IO/AbaqusFileReader.h"
#include "IO/GenFileReader.h"
#include "IO/GenFileWriter.h"
#if USE_TINYXML
#include "IO/XMLFileReader.h"
#endif //#if USE_TINYXML

#include "MTools/MshRegion3D.h"
#include "MTools/TreeGenerator.h"
//debug
#include "MTools/AdvFront3D.h"
//endebug

#if USE_GUI
#include "MTools/AdvFront3D.h"
#endif //#if USE_GUI

using namespace MTools;

const UInt MTools::MainDrive::QUALITY_RANGE = 10;

MTools::MainDrive::MainDrive() :
	Performer::MainDrive::MainDrive()
{
	this->Performer::MainDrive::status = this->notInit;

#if USE_NUMERICAL
	this->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL
	this->setUsingUniformRefinement(false);
	this->treeFactor = 0.85;

	this->boundary = NULL;
	this->tree = NULL;
	this->mesh = NULL;

#if USE_GUI
	this->front = NULL;
#endif //#if USE_GUI

#if USE_TIME_MEASUREMENTS
	this->runtime = 0.0;
#endif //#if USE_TIME_MEASUREMENTS

	//this->idManager = NULL;
	this->boundaryBuilder = NULL;
#if USE_NUMERICAL
	this->hotellingTransformer = NULL;
#endif //#if USE_NUMERICAL
	this->treeGenerator = NULL;
	this->mshRegion = NULL;
}

MTools::MainDrive::~MainDrive()
{
	this->clear();
}

#if USE_NUMERICAL
void MTools::MainDrive::setHotellingTransformEnabled(bool hotellingTransformEnabled)
{
    this->hotellingTransformEnabled = hotellingTransformEnabled;
}

bool MTools::MainDrive::isHotellingTransformEnabled() const
{
    return this->hotellingTransformEnabled;
}
#endif //#if USE_NUMERICAL

void MTools::MainDrive::setUsingUniformRefinement(bool usingUniformRefinement)
{
	this->usingUniformRefinement = usingUniformRefinement;
}

bool MTools::MainDrive::isUsingUniformRefinement() const
{
	return this->usingUniformRefinement;
}

#if USE_GUI
void MTools::MainDrive::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;

	if (this->boundaryBuilder)
	{
		this->boundaryBuilder->setBoundary(boundary);
	}
}

void MTools::MainDrive::setTree(Data::BoxTree *tree)
{
	this->tree = tree;

	if (this->treeGenerator)
	{
		this->treeGenerator->setTree(tree);
	}
}

void MTools::MainDrive::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}
#endif //#if USE_GUI

const Data::Boundary *MTools::MainDrive::getBoundary() const
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

const Data::BoxTree *MTools::MainDrive::getTree() const
{
	return this->tree;
}

const Data::Mesh *MTools::MainDrive::getMesh() const
{
	return this->mesh;
}

#if USE_GUI
const Data::Front *MTools::MainDrive::getFront() const
{
	return this->front;
}
#endif //#if USE_GUI

std::string MTools::MainDrive::qualityString() const
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

void MTools::MainDrive::findQuality()
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

void MTools::MainDrive::removeHangingVertices()
{
#if USE_EXPERIMENTAL_MESH
	for (Data::Mesh::InnerVertexIterator iter = this->mesh->ivBegin();
#else
	for (Data::Mesh::VertexIterator iter = this->mesh->ivBegin();
#endif //#if USE_EXPERIMENTAL_MESH
		 iter != this->mesh->ivEnd();)
	{
		Data::Vertex *v = (*iter);

		//bool empty = this->mesh->adjacency(v).empty();
		bool empty = (static_cast<MJMesh::Vertex *>(v)->numAdjacentElements() == 0);

		if (empty)
		{
			iter = this->mesh->eraseFromInner(iter);

			delete v;
		}
		else
		{
			++iter;
		}
	}
}

void MTools::MainDrive::clear()
{
	this->Performer::MainDrive::status = this->notInit;

#if USE_NUMERICAL
	this->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL
	this->setUsingUniformRefinement(false);
	this->treeFactor = 0.85;

#if USE_GUI
	if (this->front)
	{
		delete this->front;

		this->front = NULL;
	}
#endif //#if USE_GUI

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

	this->quality.clear();

#if USE_TIME_MEASUREMENTS
	this->runtime = 0.0;
#endif //#if USE_TIME_MEASUREMENTS

	/*if (this->idManager)
	{
		delete this->idManager;

		this->idManger = NULL;
	}*/

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

	if (this->mshRegion)
	{
		delete this->mshRegion;

		this->mshRegion = NULL;
	}
}

bool MTools::MainDrive::loadFile()
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

	if ((extension == ".3djmsh") ||
		(extension == ".dmsh") || (extension == ".jmsh") || (extension == ".wmsh") ||
		(extension == ".stl") || (extension == ".vtk") || (extension == ".cdb") ||
		(extension == ".off") || (extension == ".inp") || (extension == ".gen")
#if USE_TINYXML
		|| (extension == ".xml")
#endif //#if USE_TINYXML
		)
	{
		return this->loadInputFile(this->inputFileName);
	}

	return false;
}

bool MTools::MainDrive::saveFile() const
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

	if ((extension == ".dat") || (extension == ".txt") ||
        (extension == ".gen"))
	{
		return this->saveOutputFile(this->outputFileName);
	}
	else if ((extension == ".bbgen") || (extension == ".bvgen") || (extension == ".bsgen") ||
			 (extension == ".mbgen") || (extension == ".mvgen") || (extension == ".mbgen"))
    {
        return IO::GenFileWriter::write(this->outputFileName, this->getBoundary(), this->getMesh());
    }

	return false;
}

void MTools::MainDrive::execute()
{
	if ((this->Performer::MainDrive::status.getType() == Performer::MainDrive::success.getType()) ||
		(this->Performer::MainDrive::status.getType() >= Performer::MainDrive::error.getType()))
	{
		return;
	}

	this->Performer::MainDrive::status = this->init;

	if (((this->getBoundary()) && (this->getBoundary()->size() == 0)) ||
		((!this->getBoundary()) && ((!this->boundaryBuilder) ||
			(!this->boundaryBuilder->getBoundary()) ||
			(this->boundaryBuilder->getBoundary()->size() == 0))))
	{
		this->Performer::MainDrive::status = this->notInit;

		return;
	}

#if DEBUG_TIMING_PMTOOLS
	Real timeInit, timeBoundaryBuilder, timeHotellingTransform, timeHotellingBackTransform, timeOverhead, timeGeneration;
	timeInit = timeBoundaryBuilder = timeHotellingTransform = timeHotellingBackTransform = timeOverhead = timeGeneration = 0.0;
	Real debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

#if USE_TIME_MEASUREMENTS
	Real start = Data::time();
#endif //#if USE_TIME_MEASUREMENTS

	this->mshRegion = this->makeMshRegion();

#if DEBUG_TIMING_PMTOOLS
	timeInit = Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

	if (!this->executeBoundaryBuilder(true))
	{
		this->Performer::MainDrive::status = this->error;

		return;
	}

#if DEBUG_TIMING_PMTOOLS
	timeBoundaryBuilder = Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS


#if USE_NUMERICAL
#if ((USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))
	Real hotstart = Data::time();
#endif //#if ((USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))

	if (!this->executeHotellingTransform())
	{
		this->Performer::MainDrive::status = this->error;

#if ((USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))
		Real hotend = Data::time();
		Real hottime = hotend - hotstart;
		std::cout << "Runtime (Hotelling) = " << hottime << " seconds" << std::endl;
#endif //#if ((USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))

		return;
	}

#if ((USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))
    {
        Real hotend = Data::time();
        Real hottime = hotend - hotstart;
        std::cout << "Runtime (Hotelling) = " << hottime << " seconds" << std::endl;
    }
#endif //#if ((USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))

#if DEBUG_TIMING_PMTOOLS
	timeHotellingTransform = Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS
#endif //#if USE_NUMERICAL

	this->mshRegion->SetUseUniformRefinement(this->usingUniformRefinement);

	Msh3D::MshRegion3D::MeshParams params;

	this->mshRegion->GetMeshParams(params);

	//params.OctreeRefineFactor = this->treeFactor;
	params.MaxGeneratedElements = INT_MAX;
	//params.MaxVolumeRestarts = 0 ;

	this->mshRegion->SetMeshParams(params);

	//bool step = Data::isStepByStep();
	//Data::setStepByStep(true);

	try
	{
		this->mshRegion->execute();
	}
	catch (std::exception &e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());

#if USE_TIME_MEASUREMENTS
		Real end = Data::time();
		this->runtime += end - start;
		std::cout << "Runtime = " << this->runtime << " seconds" << std::endl;
#endif //#if USE_TIME_MEASUREMENTS

#if USE_GUI
		this->executeMeshBuilderInError();
#endif //#if USE_GUI

		delete this->mshRegion;

		this->mshRegion = NULL;

		this->Performer::MainDrive::status = this->error;

		return;
	}

#if DEBUG_TIMING_PMTOOLS
	timeGeneration = Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMTOOLS

	//Data::setStepByStep(step);

#if USE_GUI
	this->executeTreeGenerator(true);
#endif //#if USE_GUI

	this->executeMeshBuilder();

#if DEBUG_TIMING_PMTOOLS
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMTOOLS

#if USE_NUMERICAL
#if ((USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))
	hotstart = Data::time();
#endif //#if ((USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))

	this->executeHotellingBackTransform();

#if ((USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))
    {
        Real hotend = Data::time();
        Real hottime = hotend - hotstart;
        std::cout << "Runtime (Back Hotelling) = " << hottime << " seconds" << std::endl;
    }
#endif //#if ((USE_TIME_MEASUREMENTS) && (!DEBUG_TIMING_PMTOOLS))

#if DEBUG_TIMING_PMTOOLS
	timeHotellingBackTransform = Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMTOOLS
#endif //#if USE_NUMERICAL

	delete this->mshRegion;

	this->mshRegion = NULL;

#if USE_TIME_MEASUREMENTS
	Real end = Data::time();
	this->runtime += end - start;
	std::cout << "Runtime = " << this->runtime << " seconds" << std::endl;
#endif //#if USE_TIME_MEASUREMENTS

	this->executeFinalize();

#if DEBUG_TIMING_PMTOOLS
	timeOverhead = debugruntimeGeneration -
		Msh3D::AdvFront3D::timeTree - MTools::MshRegion3D::timeBuildAdvFront - Msh3D::AdvFront3D::timeAdvFront -
		Msh3D::AdvFront3D::timeFaceSwap - Msh3D::AdvFront3D::timeEdgeSwap - Msh3D::AdvFront3D::timeSmoothing;

    Real debugruntime = debugruntimeGeneration + timeInit + timeBoundaryBuilder + timeHotellingTransform + timeHotellingBackTransform;

	std::cout << "Timings:" << std::endl;

	std::cout << "initializing: " << timeInit << " (" << 100.0*timeInit/debugruntime << " %)" << std::endl;

	std::cout << "building boundary: " << timeBoundaryBuilder << " (" << 100.0*timeBoundaryBuilder/debugruntime << " %)" << std::endl;

#if USE_NUMERICAL
	std::cout << "hotelling transform: " << timeHotellingTransform << " (" << 100.0*timeHotellingTransform/debugruntime << " %)" << std::endl;
#endif //#if USE_NUMERICAL

	std::cout << "building adv front: " << MTools::MshRegion3D::timeBuildAdvFront << " (" << 100.0*MTools::MshRegion3D::timeBuildAdvFront/debugruntime << " %)" << std::endl;

	std::cout << "generating octree: " << Msh3D::AdvFront3D::timeTree << " (" << 100.0*Msh3D::AdvFront3D::timeTree/debugruntime << " %)" << std::endl;

	std::cout << "generating mesh: " << Msh3D::AdvFront3D::timeAdvFront << " (" << 100.0*Msh3D::AdvFront3D::timeAdvFront/debugruntime << " %)" << std::endl;

	std::cout << "improving mesh: " << Msh3D::AdvFront3D::timeFaceSwap + Msh3D::AdvFront3D::timeEdgeSwap + Msh3D::AdvFront3D::timeSmoothing << " (" << 100.0*(Msh3D::AdvFront3D::timeFaceSwap + Msh3D::AdvFront3D::timeEdgeSwap + Msh3D::AdvFront3D::timeSmoothing)/debugruntime << " %)" << std::endl;

#if USE_NUMERICAL
	std::cout << "hotelling back transform: " << timeHotellingBackTransform << " (" << 100.0*timeHotellingBackTransform/debugruntime << " %)" << std::endl;
#endif //#if USE_NUMERICAL

	std::cout << "overhead: " << timeOverhead << " (" << 100.0*timeOverhead/debugruntime << " %)" << std::endl;
#endif //#if DEBUG_TIMING_PMTOOLS

	this->Performer::MainDrive::status = this->success;
}

bool MTools::MainDrive::loadInputFile(const std::string &inputFileName)
{
	this->clear();

	std::string extension = inputFileName.substr(inputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

	this->boundaryBuilder = this->makeBoundaryBuilder();

	if (extension == ".3djmsh")
	{
		IO::MJMeshFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		UInt dimension = reader.getDimension();
		this->treeFactor = reader.getTreeFactor();
		Data::setTolerance(reader.getTolerance());

		return (dimension != 0);
	}

	if (extension == ".dmsh")
	{
		IO::JmeshFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->treeFactor = reader.getTreeFactor();
		Data::setTolerance(reader.getTolerance());

		return (this->treeFactor > 0.0);
	}

	if (extension == ".jmsh")
	{
		IO::JmeshFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->treeFactor = reader.getTreeFactor();
		Data::setTolerance(reader.getTolerance());

		return (this->treeFactor > 0.0);
	}

	if (extension == ".wmsh")
	{
		IO::JmeshFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->treeFactor = reader.getTreeFactor();
		Data::setTolerance(reader.getTolerance());

		return (this->treeFactor > 0.0);
	}

	if (extension == ".stl")
	{
		IO::STLFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->treeFactor = 0.4;

		return true;
	}

	if (extension == ".vtk")
	{
		IO::VTKFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->treeFactor = 0.4;

		return true;
	}

	if (extension == ".cdb")
	{
		IO::CDBFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->treeFactor = 0.4;

		if (!reader.isBoundary())
		{
			this->mesh = static_cast<MJMesh::Mesh *>(reader.getMesh());
		}

		return true;
	}

	if (extension == ".off")
	{
		IO::OFFFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->treeFactor = 0.4;

		return true;
	}

	if (extension == ".inp")
	{
		IO::AbaqusFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->treeFactor = 0.4;

		return true;
	}

#if USE_TINYXML
	if (extension == ".xml")
	{
		IO::XMLFileReader reader;

		reader.setFilename(inputFileName);
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

    if (extension == ".gen")
    {
        IO::GenFileReader reader;

        reader.setFilename(inputFileName);
        reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

        reader.execute();

        if (reader.isBoundary())
        {
            UInt dimension = reader.getDimension();

            if ((dimension == 3) && (!reader.isSurface()))
            {
                this->treeFactor = 0.4;

                return true;
            }

            static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->clear();

            return false;
        }

        delete reader.getMesh();

        return false;
    }

	return false;
}

bool MTools::MainDrive::saveInputFile(const std::string &/*outputFileName*/) const
{
	return false;
}

bool MTools::MainDrive::loadOutputFile(const std::string &/*inputFileName*/)
{
	return false;
}

bool MTools::MainDrive::saveOutputFile(const std::string &outputFileName) const
{
	std::string extension = outputFileName.substr(outputFileName.find_last_of('.'));

	if (extension.size() == 0)
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
		IO::TextMeshFileWriter writer;

		writer.setFilename(outputFileName);
		writer.setMesh(this->getMesh());
		writer.setDimension(3);

		writer.execute();

		return true;
	}

	if (extension == ".dat")
	{
		IO::NeutralFileWriter writer;

		writer.setFilename(outputFileName);

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

		writer.setDimension(3);

		writer.execute();

		return true;
	}

	if (extension == ".gen")
    {
        if (!this->getMesh())
        {
            return false;
        }

        IO::GenFileWriter writer;

        writer.setFilename(outputFileName);
        writer.setType(IO::GenFileWriter::VOLUME);
        writer.setMesh(this->getMesh());

        writer.execute();

        return true;
    }

	return false;
}

bool MTools::MainDrive::fillMToolsBoundary()
{
    try
	{
		for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
			 iter != this->boundary->vEnd(); iter++)
		{
			Data::Vertex *v = (*iter);

			int id = static_cast<int>(v->getId());
			FTools::Vec3D vec(v->getPoint()->getCoord(0), v->getPoint()->getCoord(1), v->getPoint()->getCoord(2));

			this->mshRegion->AddNode(id, vec);
		}

		for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
			 iter != this->boundary->eEnd(); iter++)
		{
			Data::FrontElement *fel = (*iter);

			FTools::List<int> nodes;

			if (fel->numVertices() == 3)
			{
				nodes.Append(static_cast<int>(fel->getVertex(0)->getId()));
				nodes.Append(static_cast<int>(fel->getVertex(2)->getId()));
				nodes.Append(static_cast<int>(fel->getVertex(1)->getId()));
			}

			this->mshRegion->AddFace(nodes);
		}
	}
	catch (std::exception &e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());

		return false;
	}

	return true;
}

bool MTools::MainDrive::executeBoundaryBuilder(bool del)
{
	static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->setSorted(false);

	this->boundaryBuilder->execute();

	this->boundary = static_cast<MJMesh::Boundary *>(this->boundaryBuilder->getBoundary());

	//this->idManager = this->makeIdManager(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->getIdManager());

	if (del)
	{
		delete this->boundaryBuilder;

		this->boundaryBuilder = NULL;
	}

#if USE_NUMERICAL
    return true;
#else
    return this->fillMToolsBoundary();
#endif //#if USE_NUMERICAL
}

#if USE_NUMERICAL
bool MTools::MainDrive::executeHotellingTransform()
{
    if (this->hotellingTransformEnabled)
    {
        this->hotellingTransformer = this->makeHotellingTransformer();

        this->hotellingTransformer->execute();
    }

    return this->fillMToolsBoundary();
}
#endif //#if USE_NUMERICAL

bool MTools::MainDrive::executeTreeGenerator(bool del)
{
	this->treeGenerator = this->makeTreeGenerator();

#if USE_GUI
	bool step = Data::isStepByStep();

	Data::setStepByStep(false);
#endif //#if USE_GUI

	this->treeGenerator->execute();

#if USE_GUI
	Data::setStepByStep(step);
#endif //#if USE_GUI

	this->tree = static_cast<Data::OctTree *>(this->treeGenerator->getTree());

	if (del)
	{
		delete this->treeGenerator;

		this->treeGenerator = NULL;
	}

	return true;
}

bool MTools::MainDrive::executeMeshBuilder()
{
	this->mesh = new MJMesh::Mesh();

	//converting vertices
	Data::VertexMap vertices;

	//Data::VertexList bverts = this->boundary->getVertices();

	//for (Data::VertexList::iterator iter = bverts.begin();
	for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
		 //iter != bverts.end(); iter++)
		 iter != this->boundary->vEnd(); iter++)
	{
		Data::Vertex *v = (*iter);

		this->mesh->add(v, false);

		vertices[v->getId()] = v;
	}

	Msh3D::MshRegion3D::MshRegNodeIterator niter = this->mshRegion->GetInternalNodes();

	for (niter.First(); niter.More(); niter.Next())
	{
		ULInt id = static_cast<ULInt>(niter.Id());

		Data::Point3D *p = new Data::Point3D(niter.NCoord(0), niter.NCoord(1), niter.NCoord(2));

		MJMesh::Vertex *v = new MJMesh::Vertex(p, id);

		this->mesh->add(v);

		vertices[v->getId()] = v;
	}

	//converting elements
	Msh3D::MshRegion3D::MshRegElemIterator eiter = this->mshRegion->GetElements();

	for	(eiter.First(); eiter.More(); eiter.Next())
	{
		ULInt id = static_cast<ULInt>(eiter.Id()) + 1;

		MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(vertices[eiter.Node(0)]);
		MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(vertices[eiter.Node(1)]);
		MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(vertices[eiter.Node(2)]);
		MJMesh::Vertex *v4 = static_cast<MJMesh::Vertex *>(vertices[eiter.Node(3)]);

		MJMesh::Element3D *el = new MJMesh::Element3D(v1, v2, v3, v4, id);

		this->mesh->add(el);

		Data::FrontElementList fels = this->mesh->findAdjacentFrontElements(el);

		while (!fels.empty())
		{
			MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>(fels.front());

			fels.pop_front();

			fel->add(el);
		}
	}

	//converting front elements
	//Data::FrontElementList fels = this->boundary->getElements();

	//for (Data::FrontElementList::iterator iter = fels.begin();
	for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
		 //iter != fels.end(); iter++)
		 iter != this->boundary->eEnd(); iter++)
	{
		this->mesh->add((*iter));
	}

	return true;
}

#if USE_NUMERICAL
bool MTools::MainDrive::executeHotellingBackTransform()
{
    if (!this->hotellingTransformEnabled)
    {
        return true;
    }

    this->hotellingTransformer->setMesh(this->mesh);

    this->hotellingTransformer->executeBack();

    delete this->hotellingTransformer;

    this->hotellingTransformer = NULL;

    return true;
}
#endif //#if USE_NUMERICAL

#if USE_GUI
void MTools::MainDrive::executeMeshBuilderInError()
{
	const MTools::AdvFront3D *advFront = static_cast<const MTools::AdvFront3D *>(this->mshRegion->GetAdvFront());

	MTools::AdvFront3D::TetHash *tets = advFront->GetTetTable();

	if ((!tets) || (tets->Len() == 0))
	{
		return;
	}

	this->mesh = new MJMesh::Mesh();

	///Converting vertices
	Data::VertexMap vertices;
	ULInt maxId = 0;

	for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
		 iter != this->boundary->vEnd(); iter++)
	{
		Data::Vertex *v = (*iter);

		this->mesh->add(v, false);

		vertices[v->getId()] = v;

		if (v->getId() > maxId)
		{
			maxId = v->getId();
		}
	}

	MTools::AdvFront3D::IntNodeHash *nodes = advFront->GetIntNodeTable();

	MTools::AdvFront3D::IntNodeHashIter niter(nodes);

    for (niter.First(); niter.More(); ++niter)
	{
        if (static_cast<ULInt>(niter.Entry().id) > maxId)
		{
			Data::Point3D *p = new Data::Point3D(niter.Entry().coord[0], niter.Entry().coord[1], niter.Entry().coord[2]);

			MJMesh::Vertex *v = new MJMesh::Vertex(p, static_cast<ULInt>(niter.Entry().id));

			this->mesh->add(v);

			vertices[v->getId()] = v;
		}
    }

    ///Converting front elements
    for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
		 iter != this->boundary->eEnd(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		this->mesh->add(fel);
	}

	MTools::AdvFront3D::FFaceHeap *FaceHeap = advFront->GetFaceHeap();
	ULInt id = 0;

	if ((FaceHeap) && (FaceHeap->Len() > 0))
	{
		this->front = new MJMesh::Front();
	}

	while ((FaceHeap) && (FaceHeap->Len() > 0))
	{
		MTools::AdvFront3D::FFace *face = *FaceHeap->GetMin();

		Data::Vertex *vs[3];
		Data::FrontElement *fel = NULL;

		bool fromInput = true;

		for (int i = 0; i < 3; i++)
		{
			vs[i] = vertices[face->node_id[i]];

			if ((fromInput) && (vs[i]->getId() > maxId))
			{
				fromInput = false;
			}
		}

		if (fromInput)
		{
			for (UInt k = 0; k < 3; k++)
			{
				//Data::FrontElementList adjFels = this->mesh->findAdjacentFrontElements(vs[k]);

				//for (Data::FrontElementList::iterator iter = adjFels.begin();
				for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(vs[k])->feBegin();
					 //iter != adjFels.end(); iter++)
					 iter != static_cast<MJMesh::Vertex *>(vs[k])->feEnd(); iter++)
				{
					Data::FrontElement *adj = (*iter);

					bool found[3] = {false, false, false};

					for (UInt i = 0; i < 3; i++)
					{
						for (UInt j = 0; j < 3; j++)
						{
							if (found[j])
							{
								continue;
							}

							if (adj->getVertex(i) == vs[j])
							{
								found[j] = true;

								break;
							}
						}
					}

					if ((found[0]) && (found[1]) && (found[2]))
					{
						fel = adj;

						break;
					}
				}

				if (fel)
				{
					break;
				}
			}
		}

		if (!fel)
		{
			fel = new MJMesh::FrontElement3D(
				static_cast<MJMesh::Vertex *>(vs[0]),
				static_cast<MJMesh::Vertex *>(vs[1]),
				static_cast<MJMesh::Vertex *>(vs[2]),
				++id);

			this->mesh->add(fel);
		}

		this->front->add(fel);
	}

	///Converting elements
	MTools::AdvFront3D::TetHashIter eiter(tets);

	id = 0;

    for (eiter.First(); eiter.More(); ++eiter)
	{
        MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(vertices[eiter.Entry().node_id[0]]);
        MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(vertices[eiter.Entry().node_id[1]]);
        MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(vertices[eiter.Entry().node_id[2]]);
        MJMesh::Vertex *v4 = static_cast<MJMesh::Vertex *>(vertices[eiter.Entry().node_id[3]]);

        MJMesh::Element3D *e = new MJMesh::Element3D(v1, v2, v3, v4, ++id);

        this->mesh->add(e);
    }
}
#endif //#if USE_GUI

bool MTools::MainDrive::executeFinalize()
{
	this->removeHangingVertices();

	this->findQuality();

	return true;
}

/*Performer::IdManager *MTools::MainDrive::makeIdManager(const Performer::IdManager *idManager) const
{
	Performer::IdManager *idm = new Performer::SequentialIdManager(1, 3);

	if (idManager)
	{
		idm->setId(0, idManager->current(0) + 1);
		idm->setId(1, idManager->current(1) + 1);
	}

	return idm;
}*/

Performer::BoundaryBuilder *MTools::MainDrive::makeBoundaryBuilder() const
{
	return new MJMesh::BoundaryBuilder(false);
}

#if USE_NUMERICAL
MJMesh::HotellingTransformer *MTools::MainDrive::makeHotellingTransformer() const
{
    MJMesh::HotellingTransformer *hotellingTransformer = new MJMesh::HotellingTransformer();

    hotellingTransformer->setBoundary(this->boundary);
    hotellingTransformer->setMode(Performer::CovarianceMatrixFinder::Mode::NORMAL);
    hotellingTransformer->setFrontElementCentroids(false);
    hotellingTransformer->setWeighted(false);

    return hotellingTransformer;
}
#endif //#if USE_NUMERICAL

Performer::TreeGenerator *MTools::MainDrive::makeTreeGenerator() const
{
	return new MTools::TreeGenerator(this->mshRegion->GetOctTree());
}

MTools::MshRegion3D *MTools::MainDrive::makeMshRegion() const
{
	return new MTools::MshRegion3D();
}

#endif //#if USE_MTOOLS
