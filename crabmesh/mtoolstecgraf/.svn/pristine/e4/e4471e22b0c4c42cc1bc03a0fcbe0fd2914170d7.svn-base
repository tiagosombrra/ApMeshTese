#include "IO/VTKFileReader.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/FrontElement3D.h"

using namespace IO;

IO::VTKFileReader::VTKFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

	this->addFiletype(".vtk");
}

IO::VTKFileReader::~VTKFileReader()
{

}

void IO::VTKFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

bool IO::VTKFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return (this->boundaryBuilder != NULL);
}

Data::AbstractMesh *IO::VTKFileReader::makeMesh() const
{
	return NULL;
}

Data::AbstractMesh *IO::VTKFileReader::makeMesh(bool boundary) const
{
	return boundary ? static_cast<Data::AbstractMesh *>(new MJMesh::Boundary()) : static_cast<Data::AbstractMesh *>(new MJMesh::Mesh());
}

void IO::VTKFileReader::read()
{
	bool ok = this->readVtk();

    if (!ok)
	{
		this->boundaryBuilder->clear();

		if (this->mesh)
		{
			delete this->mesh;

			this->mesh = NULL;
		}
	}
}

bool IO::VTKFileReader::readVtk()
{
	std::string str;

    do
    {
    	this->file >> str;
    }
    while ((!this->file.eof()) && (str != "POINTS"));

    if (this->file.eof())
    {
    	this->file.close();

    	return false;
    }

    ULInt numVerts;

    this->file >> numVerts;

    //type -> float or double, ignored
    this->file >> str;

    for (ULInt id = 1; id <= numVerts; id++)
    {
    	Real x, y, z;

    	this->file >> x;
    	this->file >> y;
    	this->file >> z;

    	this->boundaryBuilder->add(id, x, y, z);
    }

	do
    {
    	this->file >> str;
    }
    while ((!this->file.eof()) && (str != "POLYGONS"));

    if (this->file.eof())
    {
    	this->file.close();

    	return false;
    }

    ULInt numPolygons;

	this->file >> numPolygons;

	//another number, ignored
	this->file >> str;

	for (ULInt id = 1; id <= numPolygons; id++)
    {
    	UInt numVerts;

    	this->file >> numVerts;

    	ULInt ids[numVerts];

    	for (UInt i = 0; i < numVerts; i++)
		{
			this->file >> ids[i];
		}

    	if (numVerts == 3)
		{
			this->boundaryBuilder->add(id, ids[0] + 1, ids[1] + 1, ids[2] + 1);
		}
    }

	return true;
}
