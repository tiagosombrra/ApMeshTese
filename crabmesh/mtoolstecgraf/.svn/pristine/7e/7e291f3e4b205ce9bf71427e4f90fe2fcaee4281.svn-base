#include "TecGraf/Surface.h"

#if USE_TECGRAF

#include "MJMesh/Vertex.h"

using namespace TecGraf;

TecGraf::Surface::Surface(ULInt id) :
	MJMesh::Boundary::Boundary(id)
{

}

TecGraf::Surface::Surface(Surface *surface) :
	MJMesh::Boundary::Boundary(surface)
{
	this->name = surface->name;
	this->type = surface->type;
}

TecGraf::Surface::~Surface()
{
	for (Data::Boundary::VertexIterator iter = this->vBegin();
		 iter != this->vEnd();)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

		if (v->getData().boundaries.size() > 1)
		{
			iter = this->erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void TecGraf::Surface::setName(const std::string &name)
{
	this->name = name;
}

const std::string &TecGraf::Surface::getName() const
{
	return this->name;
}

void TecGraf::Surface::setType(const std::string &type)
{
	this->type = type;
}

const std::string &TecGraf::Surface::getType() const
{
	return this->type;
}

#if USE_GUI
std::string TecGraf::Surface::text() const
{
	std::stringstream str;

	str << this->getId() << " " << this->getName();

	return str.str();
}
#endif //#if USE_GUI

#endif //#if USE_TECGRAF
