#include "Tools/MainDrive.h"

#include "Data/State.h"

#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/BoundaryBuilder.h"

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
#include "IO/XMLFileReader.h"
#include "IO/AbaqusFileReader.h"
#include "IO/AbaqusFileWriter.h"
#include "IO/GenFileReader.h"
#include "IO/GenFileWriter.h"

using namespace Tools;

const UInt Tools::MainDrive::QUALITY_RANGE = 10;

Tools::MainDrive::MainDrive() :
	Performer::MainDrive::MainDrive()
{
	this->tool = NULL;

	this->dimension = 0;

	this->boundary = NULL;
	this->mesh = NULL;
#if USE_TECGRAF
	this->model = NULL;
#endif //#if USE_TECGRAF

	this->boundaryBuilder = NULL;
}

Tools::MainDrive::~MainDrive()
{
	this->clear();
}

void Tools::MainDrive::setTool(Tool *tool)
{
	this->tool = tool;
}

Tool *Tools::MainDrive::getTool() const
{
	return this->tool;
}

void Tools::MainDrive::addTool(Tool *tool)
{
	this->tools.push_back(tool);
}

Tool *Tools::MainDrive::getLastTool() const
{
	return this->tools.back();
}

UInt Tools::MainDrive::getDimension() const
{
	return this->dimension;
}

void Tools::MainDrive::setBoundary(MJMesh::Boundary *boundary)
{
	this->boundary = boundary;

#if USE_GUI
	if (this->boundaryBuilder)
	{
		this->boundaryBuilder->setBoundary(boundary);
	}
#endif //#if USE_GUI
}

MJMesh::Boundary *Tools::MainDrive::getBoundary() const
{
//#if USE_GUI
	return this->boundary ? this->boundary :
		(this->boundaryBuilder ? static_cast<MJMesh::Boundary *>(this->boundaryBuilder->getBoundary()) : NULL);
/*#else
	return this->boundary;
#endif //#if USE_GUI*/
}

void Tools::MainDrive::setMesh(MJMesh::Mesh *mesh)
{
	this->mesh = mesh;
}

MJMesh::Mesh *Tools::MainDrive::getMesh() const
{
	return this->mesh;
}

#if USE_TECGRAF
void Tools::MainDrive::setModel(TecGraf::Model *model)
{
	this->model = model;
}

TecGraf::Model *Tools::MainDrive::getModel() const
{
	return this->model;
}
#endif //#if USE_TECGRAF

bool Tools::MainDrive::loadFile()
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

        if (this->boundaryBuilder->getBoundary())
        {
            delete this->boundaryBuilder;

            this->boundaryBuilder = NULL;
        }

        return this->loadOutputFile(this->inputFileName);
    }

	return false;
}

bool Tools::MainDrive::saveFile() const
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

bool Tools::MainDrive::loadInputFile(const std::string &inputFileName)
{
	std::string extension = inputFileName.substr(inputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

	this->boundaryBuilder = static_cast<MJMesh::BoundaryBuilder *>(this->makeBoundaryBuilder());

	if (extension == ".2djmsh")
	{
		IO::MJMeshFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->dimension = reader.getDimension();
		Data::setTolerance(reader.getTolerance());

		return (this->dimension == 2);
	}

	if (extension == ".3djmsh")
	{
		IO::MJMeshFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->dimension = reader.getDimension();
		Data::setTolerance(reader.getTolerance());

		return (this->dimension == 3);
	}

	if (extension == ".dmsh")
	{
		this->dimension = 3;

		IO::JmeshFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		Data::setTolerance(reader.getTolerance());

		return (reader.getTreeFactor() > 0.0);
	}

	if (extension == ".jmsh")
	{
		this->dimension = 3;

		IO::JmeshFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		Data::setTolerance(reader.getTolerance());

		return (reader.getTreeFactor() > 0.0);
	}

	if (extension == ".wmsh")
	{
		this->dimension = 3;

		IO::JmeshFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		Data::setTolerance(reader.getTolerance());

		return (reader.getTreeFactor() > 0.0);
	}

	if (extension == ".stl")
	{
		this->dimension = 3;

		IO::STLFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		return true;
	}

	if (extension == ".vtk")
	{
		this->dimension = 3;

		IO::VTKFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		return true;
	}

	if (extension == ".cdb")
	{
		this->dimension = 3;

		IO::CDBFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		if (!reader.isBoundary())
		{
			this->mesh = static_cast<MJMesh::Mesh *>(reader.getMesh());
		}

		return (true);
	}

	if (extension == ".off")
	{
		this->dimension = 3;

		IO::OFFFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		return (true);
	}

	if (extension == ".inp")
	{
		IO::AbaqusFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		this->dimension = 3;

		return true;
	}

#if USE_TINYXML
	if (extension == ".xml")
	{
		this->dimension = 3;

		IO::XMLFileReader reader;

		reader.setFilename(inputFileName);
#if USE_TECGRAF
		reader.setForceModel(true);
#endif //#if USE_TECGRAF
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		if (!reader.isBoundary())
		{
			if (reader.isMesh())
			{
				delete reader.getMesh();
			}
#if USE_TECGRAF
			else
			{
				this->model = reader.getModel();
			}
#endif //#if USE_TECGRAF
		}

		return (true);
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
            this->dimension = reader.getDimension();

            return true;
        }

        delete reader.getMesh();

        return false;
    }

	return false;
}

bool Tools::MainDrive::saveInputFile(const std::string &outputFileName) const
{
	std::string extension = outputFileName.substr(outputFileName.find_last_of('.'));

	if (extension.size() == 0)
	{
		return false;
	}

	if ((!this->getBoundary()) || (this->getBoundary()->size() == 0))
	{
		return false;
	}

	if ((extension == ".2djmsh") || (extension == ".3djmsh"))
	{
		IO::MJMeshFileWriter writer;

		writer.setFilename(outputFileName);
		writer.setMesh(this->getBoundary());
		writer.setDimension(this->dimension);
		writer.setTreeFactor(0.4);
		writer.setTolerance(Data::getTolerance());

		writer.execute();

		return true;
	}

	if (extension == ".dmsh")
	{
		IO::JmeshFileWriter writer;

		writer.setFilename(outputFileName);
		writer.setMesh(this->getBoundary());
		writer.setTreeFactor(0.4);
		writer.setTolerance(Data::getTolerance());

		writer.execute();

		return true;
	}

	if (extension == ".jmsh")
	{
		IO::JmeshFileWriter writer;

		writer.setFilename(outputFileName);
		writer.setMesh(this->getBoundary());
		writer.setTreeFactor(0.4);
		writer.setTolerance(Data::getTolerance());

		writer.execute();

		return true;
	}

	if (extension == ".wmsh")
	{
		IO::JmeshFileWriter writer;

		writer.setFilename(outputFileName);
		writer.setMesh(this->getBoundary());
		writer.setTreeFactor(0.4);
		writer.setTolerance(Data::getTolerance());

		writer.execute();

		return true;
	}

	if (extension == ".inp")
	{
		IO::AbaqusFileWriter writer;

		writer.setFilename(outputFileName);
		writer.setMesh(this->getBoundary());

		writer.execute();

		return true;
	}

	if (extension == ".gen")
    {
        if (!this->getBoundary())
        {
            return false;
        }

        IO::GenFileWriter writer;

        writer.setFilename(outputFileName);
        writer.setType(IO::GenFileWriter::BOUNDARY);
        writer.setMesh(this->getBoundary());

        writer.execute();

        return true;
    }

	return false;
}

bool Tools::MainDrive::loadOutputFile(const std::string &inputFileName)
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
		IO::TextMeshFileReader reader;

		reader.setFilename(inputFileName);

		reader.execute();

		this->mesh = static_cast<MJMesh::Mesh *>(reader.getMesh());
		this->dimension = reader.getDimension();

		ok = (this->dimension != 0);
	}
	else if ((extension == ".dat") || (extension == ".pos"))
	{
		this->boundaryBuilder = static_cast<MJMesh::BoundaryBuilder *>(this->makeBoundaryBuilder());

		IO::NeutralFileReader reader;

		reader.setFilename(inputFileName);
		reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

		reader.execute();

		if (!reader.isBoundary())
		{
			this->mesh = static_cast<MJMesh::Mesh *>(reader.getMesh());
		}

		this->dimension = reader.getDimension();

		ok = (this->dimension != 0);
	}
    else if (extension == ".gen")
    {
        this->boundaryBuilder = static_cast<MJMesh::BoundaryBuilder *>(this->makeBoundaryBuilder());

        IO::GenFileReader reader;

        reader.setFilename(inputFileName);
        reader.setBoundaryBuilder(static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder));

        reader.execute();

        if (!reader.isBoundary())
        {
            this->dimension = reader.getDimension();
            this->mesh = static_cast<MJMesh::Mesh *>(reader.getMesh());

            delete this->boundaryBuilder;

            this->boundaryBuilder = NULL;

            return true;
        }

        static_cast<MJMesh::BoundaryBuilder *>(this->boundaryBuilder)->clear();

        return false;
    }

	if (ok)
	{
		this->findQuality();
	}

	return ok;
}

bool Tools::MainDrive::saveOutputFile(const std::string &outputFileName) const
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
		IO::TextMeshFileWriter writer;

		writer.setFilename(outputFileName);
		writer.setMesh(this->getMesh());
		writer.setDimension(this->dimension);

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

		writer.setDimension(this->dimension);

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

void Tools::MainDrive::execute()
{
	if ((!this->tool) && (this->tools.empty()))
	{
		return;
	}

	if (this->tool)
	{
		this->tool->execute();
	}

	for (ToolList::iterator iter = this->tools.begin();
		 iter != this->tools.end(); iter++)
	{
		(*iter)->execute();
	}
}

void Tools::MainDrive::clear()
{
	if (this->tool)
	{
		delete this->tool;
	}

	while (!this->tools.empty())
	{
		delete this->tools.front();

		this->tools.pop_front();
	}

	this->quality.clear();

	if (this->mesh)
	{
		if (this->boundary)
		{
			Data::VertexList vertices;
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
}

Performer::BoundaryBuilder *Tools::MainDrive::makeBoundaryBuilder() const
{
	return new MJMesh::BoundaryBuilder(true);
}

void Tools::MainDrive::findQuality()
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
