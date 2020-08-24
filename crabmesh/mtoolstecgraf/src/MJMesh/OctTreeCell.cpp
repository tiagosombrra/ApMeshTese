#include "MJMesh/OctTreeCell.h"

#include "Data/Point3D.h"
#include "Data/Tree.h"
#include "Data/Vertex.h"

using namespace MJMesh;

MJMesh::OctTreeCell::OctTreeCell(MJMesh::OctTreeCell *parent, ULInt id) :
	Data::OctTreeCell(parent, id)
{
#if USE_ATTRIBS
	this->s = -1.0;
#endif //#if USE_ATTRIBS
}

MJMesh::OctTreeCell::~OctTreeCell()
{

}

#if USE_ATTRIBS
Real MJMesh::OctTreeCell::size() const
{
	return (this->s > 0.0) ? this->s : (this->s = Data::OctTreeCell::size(), this->s);
}
#endif //#if USE_ATTRIBS

MJMesh::OctTreeCell *MJMesh::OctTreeCell::find(MJMesh::FrontElement3D *e)
{
#if USE_EXPERIMENTAL_3
	return static_cast<MJMesh::OctTreeCell *>(this->find(e->getShape()->mid3D()));
#else
	return static_cast<MJMesh::OctTreeCell *>(this->find(e->getShape()->mid()));
#endif //#if USE_EXPERIMENTAL_3
}

Data::BoxTreeCell *MJMesh::OctTreeCell::find(MJMesh::FrontElement *e)
{
	return static_cast<Data::BoxTreeCell *>(this->find(static_cast<MJMesh::FrontElement3D *>(e)));
}

void MJMesh::OctTreeCell::subdivide()
{
	ULInt id = 8*this->getId();

	//bottom left far
	new MJMesh::OctTreeCell(this, ++id);
	//bottom right far
	new MJMesh::OctTreeCell(this, ++id);
	//top right far
	new MJMesh::OctTreeCell(this, ++id);
	//top left far
	new MJMesh::OctTreeCell(this, ++id);
	//bottom left near
	new MJMesh::OctTreeCell(this, ++id);
	//bottom right near
	new MJMesh::OctTreeCell(this, ++id);
	//top right near
	new MJMesh::OctTreeCell(this, ++id);
	//top left near
	new MJMesh::OctTreeCell(this, ++id);

#if (!USE_EXPERIMENTAL_TREE)
	if (this->getTree())
	{
		this->getTree()->addChildren(this);
	}
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

#if USE_ARRAY
#if USE_ATTRIBS
MJMesh::OctTreeCell &MJMesh::OctTreeCell::operator=(const MJMesh::OctTreeCell &c)
{
	if (this != &c)
	{
		Data::OctTreeCell::operator=(c);

		this->s = c.s;
	}

	return *this;
}
#endif //#if USE_ATTRIBS
#endif //#if USE_ARRAY
