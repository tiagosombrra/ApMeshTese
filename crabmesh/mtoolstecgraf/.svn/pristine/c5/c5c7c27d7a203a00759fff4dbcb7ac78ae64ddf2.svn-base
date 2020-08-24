#include "MJMesh/QuadTreeCell.h"

#include "Data/Point2D.h"
#include "Data/Tree.h"
#include "Data/Vertex.h"

using namespace MJMesh;

MJMesh::QuadTreeCell::QuadTreeCell(MJMesh::QuadTreeCell *parent, ULInt id) :
	Data::QuadTreeCell(parent, id)
{
#if USE_ATTRIBS
	this->s = -1.0;
#endif //#if USE_ATTRIBS
}

MJMesh::QuadTreeCell::~QuadTreeCell()
{

}

#if USE_ATTRIBS
Real MJMesh::QuadTreeCell::size() const
{
	return (this->s > 0.0) ? this->s : (this->s = Data::QuadTreeCell::size(), this->s);
}
#endif //#if USE_ATTRIBS

MJMesh::QuadTreeCell *MJMesh::QuadTreeCell::find(MJMesh::FrontElement2D *e)
{
#if USE_EXPERIMENTAL_3
	return static_cast<MJMesh::QuadTreeCell *>(this->find(e->getShape()->mid2D()));
#else
	return static_cast<MJMesh::QuadTreeCell *>(this->find(e->getShape()->mid()));
#endif //#if USE_EXPERIMENTAL_3
}

Data::BoxTreeCell *MJMesh::QuadTreeCell::find(MJMesh::FrontElement *e)
{
	return static_cast<Data::BoxTreeCell *>(this->find(static_cast<MJMesh::FrontElement2D *>(e)));
}

void MJMesh::QuadTreeCell::subdivide()
{
	ULInt id = 4*this->getId();

	//bottom left
	new MJMesh::QuadTreeCell(this, ++id);
	//bottom right
	new MJMesh::QuadTreeCell(this, ++id);
	//top right
	new MJMesh::QuadTreeCell(this, ++id);
	//top left
	new MJMesh::QuadTreeCell(this, ++id);

#if (!USE_EXPERIMENTAL_TREE)
	if (this->getTree())
	{
		this->getTree()->addChildren(this);
	}
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

#if USE_ARRAY
#if USE_ATTRIBS
MJMesh::QuadTreeCell &MJMesh::QuadTreeCell::operator=(const MJMesh::QuadTreeCell &c)
{
	if (this != &c)
	{
		Data::QuadTreeCell::operator=(c);

		this->s = c.s;
	}

	return *this;
}
#endif //#if USE_ATTRIBS
#endif //#if USE_ARRAY
