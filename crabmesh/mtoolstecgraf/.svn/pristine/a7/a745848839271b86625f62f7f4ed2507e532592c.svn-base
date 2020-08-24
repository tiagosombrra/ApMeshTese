#include "IO/MeshFileReader.h"

IO::MeshFileReader::MeshFileReader() :
	IO::FileReader::FileReader()
{
	this->mesh = NULL;
}

IO::MeshFileReader::~MeshFileReader()
{

}

Data::AbstractMesh *IO::MeshFileReader::getMesh() const
{
	return this->mesh;
}

bool IO::MeshFileReader::makeStructures()
{
	this->mesh = this->makeMesh();

	return true;
}
