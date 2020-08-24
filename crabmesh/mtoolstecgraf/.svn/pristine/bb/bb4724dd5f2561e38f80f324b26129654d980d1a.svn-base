#include "IO/MJMeshFileReader.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Mesh.h"

using namespace IO;

IO::MJMeshFileReader::MJMeshFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

	this->dimension = 0;
	this->treeFactor = 0.0;
	this->tolerance = 0.0;

	this->addFiletype(".2djmsh");
	this->addFiletype(".3djmsh");
}

IO::MJMeshFileReader::~MJMeshFileReader()
{

}

void IO::MJMeshFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

UInt IO::MJMeshFileReader::getDimension() const
{
	return this->dimension;
}

Real IO::MJMeshFileReader::getTreeFactor() const
{
	return this->treeFactor;
}

Real IO::MJMeshFileReader::getTolerance() const
{
	return this->tolerance;
}

bool IO::MJMeshFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return (this->boundaryBuilder != NULL);
}

Data::AbstractMesh *IO::MJMeshFileReader::makeMesh() const
{
	return NULL;
}

void IO::MJMeshFileReader::read()
{
	if (this->isFiletype(this->filetypes.front()))
	{
		this->dimension = 2;
	}
	else if (this->isFiletype(this->filetypes.back()))
	{
		this->dimension = 3;
	}
	else
	{
		if (this->mesh)
		{
			delete this->mesh;

			this->mesh = NULL;
		}

		return;
	}

	bool ok = this->readMJMesh();

    if (!ok)
	{
		this->boundaryBuilder->clear();

		if (this->mesh)
		{
			delete this->mesh;

			this->mesh = NULL;
		}

		this->dimension = 0;
		this->treeFactor = 0.0;
		this->tolerance = 0.0;
	}
}

bool IO::MJMeshFileReader::readMJMesh()
{
	bool ok = false;

    ULInt verticesCount;

	this->file >> this->treeFactor;
	this->file >> this->tolerance;

	this->file >> verticesCount;

    if (this->dimension == 2)
    {
    	ULInt id = 0;
    	ULInt idFirst = 0;
    	ULInt idLast = 0;
    	ULInt idEdges = 0;

    	for (ULInt i = 0; i < verticesCount; i++)
    	{
    		std::string str;

    		std::getline(this->file, str);

    		while (str.size() <= 1)
    		{
    			std::getline(this->file, str);
    		}

    		if (str == "-1")
    		{
    			if (idFirst != idLast) //dangling vertex
				{
					this->boundaryBuilder->add(++idEdges, idLast, idFirst);
				}

				i--;

				idFirst = idLast = 0;

				continue;
    		}

    		std::stringstream stream(str.c_str());

    		stream >> id;

    		if (idFirst == 0)
    		{
    			idFirst = id;
    		}

    		Real x, y;

    		stream >> x;
    		stream >> y;

    		this->boundaryBuilder->add(id, x, y);

    		if (idLast != 0)
    		{
				this->boundaryBuilder->add(++idEdges, idLast, id);
    		}

    		idLast = id;
    	}

    	if (idFirst != idLast)
		{
			this->boundaryBuilder->add(++idEdges, idLast, idFirst);
		}

    	ok = true;
    }
    else if (this->dimension == 3)
    {
		for (ULInt i = 0; i < verticesCount; i++)
    	{
    		std::string str;

    		std::getline(this->file, str);

    		while (str.size() <= 1)
    		{
    			std::getline(this->file, str);
    		}

    		std::stringstream stream(str.c_str());

    		ULInt id;

    		stream >> id;

    		Real x, y, z;

    		stream >> x;
    		stream >> y;
    		stream >> z;

    		this->boundaryBuilder->add(id, x, y, z);
    	}

    	ULInt elementsCount;

    	this->file >> elementsCount;

    	for (ULInt i = 0; i < elementsCount; i++)
    	{
    		std::string str;

    		std::getline(this->file, str);

    		while (str.size() <= 1)
    		{
    			std::getline(this->file, str);
    		}

    		std::stringstream stream(str.c_str());

    		ULInt id, idv1, idv2, idv3;

    		stream >> id;
    		stream >> idv1;
    		stream >> idv2;
    		stream >> idv3;

    		this->boundaryBuilder->add(id, idv1, idv2, idv3);
    	}

    	ok = true;
    }

    return ok;
}
