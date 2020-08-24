#include "Data/VertexBoxSearchTreeCell.h"

#include "Data/Box.h"

#if USE_EXPERIMENTAL_5

Data::VertexBoxSearchTreeCell2D::VertexBoxSearchTreeCell2D(BoxTreeCell *cell, VertexBoxSearchTreeCell2D *parent, ULInt id)
	: BoxSearchTreeCell2D(cell, parent, id)
{

}

Data::VertexBoxSearchTreeCell2D::~VertexBoxSearchTreeCell2D()
{

}

bool Data::VertexBoxSearchTreeCell2D::inRange(const Box *box, const Identifiable *s) const
{
	return !box->out((static_cast<const Vertex *>(s))->getPoint());
}

Data::VertexBoxSearchTreeCell3D::VertexBoxSearchTreeCell3D(BoxTreeCell *cell, VertexBoxSearchTreeCell3D *parent, ULInt id)
	: BoxSearchTreeCell3D(cell, parent, id)
{

}

Data::VertexBoxSearchTreeCell3D::~VertexBoxSearchTreeCell3D()
{

}

bool Data::VertexBoxSearchTreeCell3D::inRange(const Box *box, const Identifiable *s) const
{
	return !box->out((static_cast<const Vertex *>(s))->getPoint());
}

#else

Data::VertexBoxSearchTreeCell::VertexBoxSearchTreeCell(BoxTreeCell *cell, VertexBoxSearchTreeCell *parent, ULInt id)
	: BoxSearchTreeCell(cell, parent, id)
{

}

Data::VertexBoxSearchTreeCell::~VertexBoxSearchTreeCell()
{

}

bool Data::VertexBoxSearchTreeCell::inRange(const Box *box, const Identifiable *s) const
{
	return !box->out((static_cast<const Vertex *>(s))->getPoint());
}

#endif //#if USE_EXPERIMENTAL_5
