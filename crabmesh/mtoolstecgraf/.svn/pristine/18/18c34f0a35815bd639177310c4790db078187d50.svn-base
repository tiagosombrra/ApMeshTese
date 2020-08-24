#include "IO/TextMeshFileWriter.h"

#include "Data/Mesh.h"

IO::TextMeshFileWriter::TextMeshFileWriter() :
	IO::MeshFileWriter::MeshFileWriter()
{
	this->dimension = 0;

	this->addFiletype(".txt");
}

IO::TextMeshFileWriter::~TextMeshFileWriter()
{

}

void IO::TextMeshFileWriter::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void IO::TextMeshFileWriter::write() const
{
	if ((this->dimension == 2) || (this->dimension == 3))
	{
		this->writeText();
	}
}

void IO::TextMeshFileWriter::writeText() const
{
	file << this->dimension << std::endl;

	//Data::VertexList vertices = static_cast<const Data::Mesh *>(this->mesh)->getVertices(true);
	//Data::VertexList vertices = static_cast<const Data::Mesh *>(this->mesh)->getSortedVertices();
	Data::VertexList vertices = static_cast<const Data::Mesh *>(this->mesh)->getVertices();

	vertices.sort(Data::Identifiable::less);

	file << vertices.size() << std::endl;

	for (Data::VertexList::iterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		Data::Point *p = (*iter)->getPoint();

		file << (*iter)->getId();

		for (UInt i = 0; i < this->dimension; i++)
		{
			file << " " << p->getCoord(i);
		}

		file << std::endl;
	}

	//Data::ElementList elements = static_cast<const Data::Mesh *>(this->mesh)->getElements(true);
	//Data::ElementList elements = static_cast<const Data::Mesh *>(this->mesh)->getSortedElements();
	Data::ElementList elements = static_cast<const Data::Mesh *>(this->mesh)->getElements();

	elements.sort(Data::Identifiable::less);

	file << elements.size() << std::endl;

	for (Data::ElementList::iterator iter = elements.begin();
		 iter != elements.end(); iter++)
	{
		Data::Element *e = (*iter);

		file << e->getId();

		for (UInt i = 0; i < e->numVertices(); i++)
		{
			file << " " << e->getVertex(i)->getId();
		}

		file << std::endl;
	}
}
