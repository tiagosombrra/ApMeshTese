#include "PMJMesh2/APrioriFrontPartitioner.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Box.h"
#include "MJMesh/FrontElement.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/MJMeshTask.h"

using namespace PMJMesh2;

PMJMesh2::APrioriFrontPartitioner::APrioriFrontPartitioner(UInt dimension, bool addInRejected, PMJMesh::MJMeshTask *task,
    const MJMesh::Front *front, UInt direction, Real coord) :
        PMJMesh2::FrontPartitioner::FrontPartitioner(dimension, addInRejected, task, front)
{
    this->direction = direction;
    this->coord = coord;
    this->less = false;
}

PMJMesh2::APrioriFrontPartitioner::~APrioriFrontPartitioner()
{

}

bool PMJMesh2::APrioriFrontPartitioner::optIn(const Data::Box */*box*/, const Data::Point *point) const
{
    return (this->less) ?
        (point->getCoord(this->direction) < this->coord - Data::getTolerance()) :
        (point->getCoord(this->direction) > this->coord + Data::getTolerance());
}

bool PMJMesh2::APrioriFrontPartitioner::optOut(const Data::Box */*box*/, const Data::Point *point) const
{
    return (this->less) ?
        (point->getCoord(this->direction) > this->coord + Data::getTolerance()) :
        (point->getCoord(this->direction) < this->coord - Data::getTolerance());
}

bool PMJMesh2::APrioriFrontPartitioner::completelyIn(const Data::Box *box, const MJMesh::FrontElement *e) const
{
    bool in = true;

	for (UInt i = 0; i < e->numVertices(); i++)
	{
		if (!this->optIn(box, e->getVertex(i)->getPoint()))
		{
			in = false;

			break;
		}
	}

	return in;
}

bool PMJMesh2::APrioriFrontPartitioner::out(const Data::Box *box, const MJMesh::FrontElement *e) const
{
	/*//test if the two bounding boxes intercept
	if (!e->getShape()->box().optIntercept(box))
	{
		//the two bounding boxes do not intercept each other
		return true;
	}

#if USE_EXPERIMENTAL_3
	return (this->dimension == 2) ?
        this->clip2D(box, e->getShape()).empty() :
        this->clip3D(box, e->getShape()).empty();
#else
	return this->clip(box, e->getShape()).empty();
#endif //#if USE_EXPERIMENTAL_3*/

    bool out = true;

	for (UInt i = 0; i < e->numVertices(); i++)
	{
		if (!this->optOut(box, e->getVertex(i)->getPoint()))
		{
			out = false;

			break;
		}
	}

	return out;
}

void PMJMesh2::APrioriFrontPartitioner::execute()
{
    if ((this->dimension != 2) && (this->dimension != 3))
    {
        return;
    }

    if (!this->task)
    {
        return;
    }

    /*//if ((this->task->getId() == 57) || (this->task->getId() == 58))
    {
        std::cout << "debug apriorifrontpartitioner task " << this->task->getId() << std::endl;
    }*/

    PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>(this->task->getPartition());

    const Data::Box *box = p->getBox();

    if (this->dimension == 2)
    {
        this->less = box->mid2D().getCoord(this->direction) < coord;
    }
    else
    {
        this->less = box->mid3D().getCoord(this->direction) < coord;
    }

    PMJMesh2::FrontPartitioner::execute();
}
