#include "Data/FrontElementBoxSearchTreeCell.h"

#include "Data/BoxTreeCell.h"
#include "Data/FrontElement.h"
#include "Data/Box.h"

#if USE_EXPERIMENTAL_5

Data::FrontElementBoxSearchTreeCell2D::FrontElementBoxSearchTreeCell2D(BoxTreeCell *cell, FrontElementBoxSearchTreeCell2D *parent, ULInt id)
	: BoxSearchTreeCell2D(cell, parent, id)
{

}

Data::FrontElementBoxSearchTreeCell2D::~FrontElementBoxSearchTreeCell2D()
{

}

bool Data::FrontElementBoxSearchTreeCell2D::inRange(const Box *box, const Identifiable *s) const
{
	//return box->intercept((static_cast<const FrontElement *>(s))->getShape()->box());
	//return (static_cast<const FrontElement *>(s))->getShape()->box().intercept(box);
	return (static_cast<const FrontElement *>(s))->getShape()->box().optIntercept(box);
}

Data::FrontElementBoxSearchTreeCell3D::FrontElementBoxSearchTreeCell3D(BoxTreeCell *cell, FrontElementBoxSearchTreeCell3D *parent, ULInt id)
	: BoxSearchTreeCell3D(cell, parent, id)
{

}

Data::FrontElementBoxSearchTreeCell3D::~FrontElementBoxSearchTreeCell3D()
{

}

bool Data::FrontElementBoxSearchTreeCell3D::inRange(const Box *box, const Identifiable *s) const
{
	//return box->intercept((static_cast<const FrontElement *>(s))->getShape()->box());
	//return (static_cast<const FrontElement *>(s))->getShape()->box().intercept(box);
	return (static_cast<const FrontElement *>(s))->getShape()->box().optIntercept(box);
}

#else

Data::FrontElementBoxSearchTreeCell::FrontElementBoxSearchTreeCell(BoxTreeCell *cell, FrontElementBoxSearchTreeCell *parent, ULInt id)
	: BoxSearchTreeCell(cell, parent, id)
{

}

Data::FrontElementBoxSearchTreeCell::~FrontElementBoxSearchTreeCell()
{

}

bool Data::FrontElementBoxSearchTreeCell::inRange(const Box *box, const Identifiable *s) const
{
	//return box->intercept((static_cast<const FrontElement *>(s))->getShape()->box());
	//return (static_cast<const FrontElement *>(s))->getShape()->box().intercept(box);
	return (static_cast<const FrontElement *>(s))->getShape()->box().optIntercept(box);
}

#endif //#if USE_EXPERIMENTAL_5
