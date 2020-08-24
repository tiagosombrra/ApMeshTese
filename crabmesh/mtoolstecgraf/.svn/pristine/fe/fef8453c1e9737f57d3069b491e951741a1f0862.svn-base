#include "IO/JmeshFileReader.h"

#include "MJMesh/BoundaryBuilder.h"
#include "MJMesh/Mesh.h"

using namespace IO;

IO::JmeshFileReader::JmeshFileReader() :
	IO::MeshFileReader::MeshFileReader()
{
	this->boundaryBuilder = NULL;

	this->internal = false;
	this->fromone = false;
	this->treeFactor = 0.0;
	this->tolerance = 0.0;

	this->addFiletype(".dmsh");
	this->addFiletype(".jmsh");
	this->addFiletype(".wmsh");
}

IO::JmeshFileReader::~JmeshFileReader()
{

}

void IO::JmeshFileReader::setBoundaryBuilder(MJMesh::BoundaryBuilder *boundaryBuilder)
{
	this->boundaryBuilder = boundaryBuilder;
}

bool IO::JmeshFileReader::isInternal() const
{
	return this->internal;
}

bool IO::JmeshFileReader::isFromone() const
{
	return this->fromone;
}

Real IO::JmeshFileReader::getTreeFactor() const
{
	return this->treeFactor;
}

Real IO::JmeshFileReader::getTolerance() const
{
	return this->tolerance;
}

bool IO::JmeshFileReader::makeStructures()
{
	bool ret = MeshFileReader::makeStructures();

	if (!ret)
	{
		return false;
	}

	return (this->boundaryBuilder != NULL);
}

Data::AbstractMesh *IO::JmeshFileReader::makeMesh() const
{
	return NULL;
}

void IO::JmeshFileReader::read()
{
	bool ok = false;

	if (this->isFiletype(this->filetypes.front()))
	{
		ok = this->readDMsh();
	}
	else if (this->isFiletype(*(++this->filetypes.begin())))
	{
		ok = this->readJMsh();
	}
	else if (this->isFiletype(this->filetypes.back()))
	{
		ok = this->readWMsh();
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

	if (!ok)
	{
		this->boundaryBuilder->clear();

		if (this->mesh)
		{
			delete this->mesh;

			this->mesh = NULL;
		}

		this->internal = false;
		this->fromone = false;
		this->treeFactor = 0.0;
		this->tolerance = 0.0;
	}
}

bool IO::JmeshFileReader::readDMsh()
{
	ULInt verticesCount, elementsCount;

	this->file >> this->treeFactor;
	this->file >> this->tolerance;

	this->file >> this->internal;
	this->file >> this->fromone;

	this->file >> verticesCount;

	ULInt id = 1;

	for (UInt i = 0; i < verticesCount; i++)
	{
		std::string str;

		std::getline(this->file, str);

		while (str.size() <= 1)
		{
			std::getline(this->file, str);
		}

		std::stringstream stream(str.c_str());

		Real x, y, z;

		stream >> x;
		stream >> y;
		stream >> z;

		this->boundaryBuilder->add(id++, x, y, z);
	}

	this->file >> elementsCount;

	id = 1;

	for (UInt i = 0; i < elementsCount; i++)
	{
		std::string str;

		std::getline(this->file, str);

		while (str.size() <= 1)
		{
			std::getline(this->file, str);
		}

		std::stringstream stream(str.c_str());

		ULInt idv1, idv2, idv3;

		stream >> idv1;
		stream >> idv2;
		stream >> idv3;

		if (!fromone)
		{
			idv1++;
			idv2++;
			idv3++;
		}

		this->boundaryBuilder->add(id++, idv1, idv2, idv3);
	}

	return true;
}

bool IO::JmeshFileReader::readJMsh()
{
	ULInt verticesCount, elementsCount;

	this->file >> this->treeFactor;
	this->file >> this->tolerance;

	this->file >> this->internal;
	this->file >> this->fromone;

	this->file >> verticesCount;

	for (UInt i = 0; i < verticesCount; i++)
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

		if (i == 0)
		{
			if ((fromone) && (id == 0))
			{
				fromone = false;
			}
			else if ((!fromone) && (id != 0))
			{
				fromone = true;
			}
		}

		if (!fromone)
		{
			id++;
		}

		this->boundaryBuilder->add(id, x, y, z);
	}

	this->file >> elementsCount;

	bool inconsistent = false;

	std::streampos pos = std::ios::end;

	for (UInt j = 0; j < 2; j++)
	{
		inconsistent = false;

		ULInt id = 1;

		if (j == 0)
		{
			pos = this->file.tellg();
		}
		else
		{
			this->file.seekg(pos);
		}

		for (UInt i = 0; i < elementsCount; i++)
		{
			std::string str;

			std::getline(this->file, str);

			while (str.size() <= 1)
			{
				std::getline(this->file, str);
			}

			std::stringstream stream(str.c_str());

			ULInt idv1, idv2, idv3;

			stream >> idv1;
			stream >> idv2;
			stream >> idv3;

			if ((fromone) &&
				((idv1 == 0) || (idv2 == 0) || (idv3 == 0)))
			{
				inconsistent = true;

				fromone = false;

				this->boundaryBuilder->clearElements();

				//move back in the file

				break;
			}

			if (!fromone)
			{
				idv1++;
				idv2++;
				idv3++;
			}

			this->boundaryBuilder->add(id++, idv1, idv2, idv3);
		}

		if (!inconsistent)
		{
			break;
		}
	}

	return true;
}

bool IO::JmeshFileReader::readWMsh()
{
	ULInt verticesCount, elementsCount;

	this->treeFactor = 0.4;
	this->tolerance = 0.001;

	this->file >> verticesCount;
	this->file >> elementsCount;

	bool found0V = false;
	bool found0E = false;

	for (UInt i = 0; i < verticesCount; i++)
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

		if (stream.eof())
		{
			verticesCount++;

			continue;
		}

		Real x, y, z;

		stream >> x;

		if (stream.eof())
		{
			verticesCount++;

			continue;
		}

		stream >> y;

		if (stream.eof())
		{
			verticesCount++;

			continue;
		}

		stream >> z;

		if (id == 0)
		{
			found0V = true;
		}

		this->boundaryBuilder->add(id, x, y, z);
	}

	for (UInt i = 0; i < elementsCount; i++)
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

		if (id == 0)
		{
			found0E = true;
		}

		//in wmsh format, triangles are in cw instead of ccw
		this->boundaryBuilder->add(id, idv1, idv3, idv2);
	}

	this->boundaryBuilder->incrementIds(found0V, found0E);

	return true;
}
