#include "IO/OFFFileReader.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/FrontElement3D.h"

using namespace IO;

IO::OFFFileReader::OFFFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

	this->addFiletype(".off");
}

IO::OFFFileReader::~OFFFileReader()
{

}

void IO::OFFFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

bool IO::OFFFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return (this->boundaryBuilder != NULL);
}

Data::AbstractMesh *IO::OFFFileReader::makeMesh() const
{
	return NULL;
}

Data::AbstractMesh *IO::OFFFileReader::makeMesh(bool boundary) const
{
	return boundary ? static_cast<Data::AbstractMesh *>(new MJMesh::Boundary()) : static_cast<Data::AbstractMesh *>(new MJMesh::Mesh());
}

void IO::OFFFileReader::read()
{
	bool ok = this->readOFF();

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

bool IO::OFFFileReader::readOFF()
{
	std::string str;

	std::getline(this->file, str); //ignore

	ULInt numVertices, numFaces, numEdges;

	this->file >> numVertices;
	this->file >> numFaces;
	this->file >> numEdges;

	ULInt id = 0;

	for (ULInt i = 0; i < numVertices; i++)
	{
		Real x, y, z;

		this->file >> x;
		this->file >> y;
		this->file >> z;

		this->boundaryBuilder->add(++id, x, y, z);
	}

	id = 0;

#if USE_QUADS
	ULInt vs[4] = {0, 0, 0, 0};
#else
	ULInt vs[3] = {0, 0, 0};
#endif //#if USE_QUADS

	for (UInt i = 0; i < numFaces; i++)
	{
		this->file >> numVertices;

		if (numVertices == 3)
		{
			this->file >> vs[0];
			this->file >> vs[1];
			this->file >> vs[2];

			this->boundaryBuilder->add(++id, vs[0] + 1, vs[1] + 1, vs[2] + 1);
		}
#if USE_QUADS
		else if (numVertices == 4)
		{
			this->file >> vs[0];
			this->file >> vs[1];
			this->file >> vs[2];
			this->file >> vs[3];

			this->boundaryBuilder->add(++id, vs[0] + 1, vs[1] + 1, vs[2] + 1, vs[3] + 1);
		}
#endif //#if USE_QUADS
		else
		{
			this->file.close();

			return false;
		}
	}

	this->file.close();

	return true;
}
