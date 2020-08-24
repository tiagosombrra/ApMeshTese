#include "Data/AbstractMesh.h"

using namespace Data;

Data::AbstractMesh::AbstractMesh(ULInt id) :
	Data::Identifiable(id)
#if USE_GUI
	, Data::ListDisplayable()
#endif //#if USE_GUI
{

}

Data::AbstractMesh::~AbstractMesh()
{

}

bool Data::AbstractMesh::have(const Vertex */*v*/) const
{
    return false;
}

bool Data::AbstractMesh::have(const FrontElement */*fel*/) const
{
    return false;
}

bool Data::AbstractMesh::have(const Element */*e*/) const
{
    return false;
}

ULInt Data::AbstractMesh::verticesSize() const
{
    return 0;
}

ULInt Data::AbstractMesh::size() const
{
    return 0;
}

UInt Data::AbstractMesh::dimension() const
{
    return 0;
}
