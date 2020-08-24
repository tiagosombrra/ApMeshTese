#include "IO/AbaqusFileReader.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/FrontElement3D.h"

using namespace IO;

IO::AbaqusFileReader::AbaqusFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

	this->boundary = true;

	this->addFiletype(".inp");
}

IO::AbaqusFileReader::~AbaqusFileReader()
{

}

void IO::AbaqusFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

bool IO::AbaqusFileReader::isBoundary() const
{
	return this->boundary;
}

bool IO::AbaqusFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return (this->boundaryBuilder != NULL);
}

Data::AbstractMesh *IO::AbaqusFileReader::makeMesh() const
{
	return NULL;
}

Data::AbstractMesh *IO::AbaqusFileReader::makeMesh(bool boundary) const
{
	return boundary ? static_cast<Data::AbstractMesh *>(new MJMesh::Boundary()) : static_cast<Data::AbstractMesh *>(new MJMesh::Mesh());
}

void IO::AbaqusFileReader::read()
{
	bool ok = this->readAbaqus();

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

bool IO::AbaqusFileReader::readAbaqus()
{
	std::string str;

    do
    {
    	std::getline(this->file, str);

    	if ((str.size() >= 5) && (str.substr(0, 5) == "*Node"))
		{
			break;
		}
    }
    while (!this->file.eof());

    if (this->file.eof())
    {
    	this->file.close();

    	return false;
    }

    //reading vertices
    ULInt lastVertId = 0;

    while (true)
	{
		if (this->file.eof())
		{
			this->file.close();

			return false;
		}

		ULInt id = 0;

		std::getline(this->file, str);

		if (str[0] == '*')
		{
			break;
		}

		std::stringstream linestr;

		linestr << str;

		linestr >> id;

		lastVertId = id;

		Real x, y, z;

		linestr >> str; //comma
		linestr >> x;
		linestr >> str; //comma
		linestr >> y;
		linestr >> str; //comma
		linestr >> z;

		this->boundaryBuilder->add(id, x, y, z);
	}

	while (true)
	{
		if ((str.size() >= 8) && (str.substr(0, 8) == "*Element"))
		{
			break;
		}

		std::getline(this->file, str);
	}

	if ((this->file.eof()) || (lastVertId == 0))
	{
		this->file.close();

		return false;
	}

	//ULInt lastFEId = 0;

	//reading faces
	while (true)
	{
		std::stringstream linestr;

		linestr << str;

		std::getline(linestr, str, ','); //ignore *Element
		std::getline(linestr, str);

		std::stringstream linestr2;

		linestr2 << str;
		linestr2 >> str;

		if (str[str.size() - 1] == ',')
		{
			str = str.substr(0, str.size() - 1);
		}

		UInt type = 0;

		if ((str == "type=S4") || (str == "type=S4R"))
		{
			type = 2;
		}
		else if ((str == "type=S3") || (str == "type=S3R"))
		{
			type = 1;
		}

		if (type == 0)
		{
			break;
		}

		while (true)
		{
			if (this->file.eof())
			{
				this->file.close();

				return false;
			}

			std::getline(this->file, str);

			if (str[0] == '*')
			{
				break;
			}

			ULInt id, id1, id2, id3, id4;

			std::stringstream linestr;

			linestr << str;
			linestr >> id;
			linestr >> str;
			linestr >> id1;
			linestr >> str;
			linestr >> id2;
			linestr >> str;
			linestr >> id3;

			if (type == 1)
			{
				this->boundaryBuilder->add(id, id1, id2, id3);
			}
			else if (type == 2)
			{
				linestr >> str;
				linestr >> id4;

#if USE_QUADS
				this->boundaryBuilder->add(id, id1, id2, id3, id4);
#endif //#if USE_QUADS
			}
		}

		if ((str.size() < 8) || (str.substr(0, 8) != "*Element"))
		{
			break;
		}
	}

	if (this->file.eof())
	{
		this->file.close();

		return false;
	}

	this->file.close();

	return true;
}
