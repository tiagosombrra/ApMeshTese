#include "IO/MeshFileWriter.h"

IO::MeshFileWriter::MeshFileWriter() :
	IO::FileWriter::FileWriter()
{
	this->mesh = NULL;
}

IO::MeshFileWriter::~MeshFileWriter()
{

}

void IO::MeshFileWriter::setMesh(const Data::AbstractMesh *mesh)
{
	this->mesh = mesh;
}

bool IO::MeshFileWriter::areStructuresCorrect() const
{
	return (this->mesh != NULL);
}
