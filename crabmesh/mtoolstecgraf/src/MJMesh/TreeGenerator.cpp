#include "MJMesh/TreeGenerator.h"

#include "MJMesh/FrontElement.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/QuadTree.h"
#include "MJMesh/OctTree.h"

#include "Data/GeometricShape.h"
#include "Data/BoxTreeCell.h"
#include "Data/Tree.h"
#include "Data/Box.h"

using namespace MJMesh;

const Performer::Status MJMesh::TreeGenerator::notInit =
	Performer::Status(MJMesh::TreeGenerator::NOT_INIT, "not initiated");
const Performer::Status MJMesh::TreeGenerator::init =
	Performer::Status(MJMesh::TreeGenerator::INIT, "initiated");
	const Performer::Status MJMesh::TreeGenerator::ended =
	Performer::Status(MJMesh::TreeGenerator::ENDED, "ended");
const Performer::Status MJMesh::TreeGenerator::error =
	Performer::Status(MJMesh::TreeGenerator::ERROR, "error");

MJMesh::TreeGenerator::TreeGenerator(UInt dimension,
	const MJMesh::Boundary *boundary, Real factor) :
	Performer::TreeGenerator()
{
	this->setBoundary(boundary);

	this->setDimension(dimension);

	this->setFactor(factor);

	this->root = NULL;

	this->minLevel = 0;
	this->maxLevel = 0;

	this->status = MJMesh::TreeGenerator::notInit;
}

MJMesh::TreeGenerator::~TreeGenerator()
{

}

void MJMesh::TreeGenerator::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void MJMesh::TreeGenerator::setFactor(Real factor)
{
	this->factor = factor;
}

void MJMesh::TreeGenerator::setBoundary(const MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

UInt MJMesh::TreeGenerator::getMinLevel() const
{
    return this->minLevel;
}

UInt MJMesh::TreeGenerator::getMaxLevel() const
{
    return this->maxLevel;
}

bool MJMesh::TreeGenerator::shouldSubdivide(Data::BoxTreeCell *c, Real elSize) const
{
	/*return ((this->dimension == 2) ?
			(c->size() >= elSize*this->factor) :                //length of quadtree cell edge
			(std::pow(c->size(), 2.0) >= elSize*this->factor)); //area of octree cell face, because c->size is length of octree cell edge*/
	return ((this->dimension == 2) ?
			(c->size()*this->factor >= elSize) :                //length of quadtree cell edge
			(std::pow(c->size(), 2.0)*this->factor >= elSize)); //area of octree cell face, because c->size is length of octree cell edge
}

bool MJMesh::TreeGenerator::shouldSubdivide(Data::BoxTreeCell *c, MJMesh::FrontElement *e) const
{
	return this->shouldSubdivide(c, e->getShape()->size());
}

/*void MJMesh::TreeGenerator::subdivide(Data::BoxTreeCell *c, Data::Point p, Real elSize)
{
	if (c->out(p))
	{
		return;
	}

	if (!(this->shouldSubdivide(c, elSize)))
	{
		return;
	}

	if (!c->isSubdivided())
	{
		this->subdivide(c);
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->subdivide((Data::BoxTreeCell *)c->child(i), p, elSize);
	}
}*/

bool MJMesh::TreeGenerator::subdivide(Data::BoxTreeCell *c, MJMesh::FrontElement *e)
{
	//this->subdivide(c, e->getShape()->mid(), e->getShape()->size());

	//if (c->out(e->getShape()->mid()))
#if USE_EXPERIMENTAL_3
    Data::Point2D mid2D;
    Data::Point3D mid3D;

    Data::Point &mid = (this->dimension == 2) ?
        (mid2D = e->getShape()->mid2D(), static_cast<Data::Point &>(mid2D)) :
        (mid3D = e->getShape()->mid3D(), static_cast<Data::Point &>(mid3D));

	if (c->optOut(mid))
#else
	if (c->optOut(e->getShape()->mid()))
#endif //#if USE_EXPERIMENTAL_3
	{
		return false;
	}

#if USE_JMSH_DEBUG_MESSAGES
	//std::cout << "markos debug, generating tree, level = " << c->level() + 1 << std::endl;
	//std::cout << "markos debug, generating tree, carea = " << std::pow(c->size(), 2.0) << std::endl;
#endif //#if USE_JMSH_DEBUG_MESSAGES

	if (!(this->shouldSubdivide(c, e)))
	{
	    UInt level = c->level();

        if ((this->minLevel == 0) || (level < this->minLevel))
        {
            this->minLevel = level;
        }

        if ((this->maxLevel == 0) || (level > this->maxLevel))
        {
            this->maxLevel = level;
        }

		return true;
	}

	if (!c->isSubdivided())
	{
		this->subdivide(c);
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->subdivide(static_cast<Data::BoxTreeCell *>(c->child(i)), e);
	}

	return true;
}

void MJMesh::TreeGenerator::execute()
{
	switch (this->status.getType())
	{
	case MJMesh::TreeGenerator::ERROR:
	case MJMesh::TreeGenerator::ENDED:
		return;

	case MJMesh::TreeGenerator::NOT_INIT:
		{
			if ((!this->boundary) || (this->factor < Data::getTolerance()))
			{
				return;
			}

			if ((this->dimension != 2) && (this->dimension != 3))
			{
				return;
			}

			Data::Box *box = this->boundary->box();
			Real span = 0.0;

			for (UInt i = 0; i < dimension; i++)
			{
				Real s = box->getMax()->getCoord(i) - box->getMin()->getCoord(i);

				if (s > span)
				{
					span = s;
				}
			}

#if USE_EXPERIMENTAL_3
			Data::Point2D mid2D;
            Data::Point3D mid3D;

            Data::Point &mid = (this->dimension == 2) ?
                (mid2D = box->mid2D(), static_cast<Data::Point &>(mid2D)) :
                (mid3D = box->mid3D(), static_cast<Data::Point &>(mid3D));
#else
			Data::Point mid = box->mid();
#endif //#if USE_EXPERIMENTAL_3

			span *= 0.5;

			for (UInt i = 0; i < dimension; i++)
			{
				box->getMin()->setCoord(i, mid.getCoord(i) - span);
				box->getMax()->setCoord(i, mid.getCoord(i) + span);
			}

			if (this->dimension == 2)
			{
				Data::Point2D min(box->getMin());
				Data::Point2D max(box->getMax());

				this->graph = this->makeTree(min, max);
			}
			else if (this->dimension == 3)
			{
				Data::Point3D min(box->getMin());
				Data::Point3D max(box->getMax());

				this->graph = this->makeTree(min, max);
			}

			delete box;

			//this->elements = this->boundary->getElements(); ///TODO optimize it (pointer to list)

			this->root = static_cast<Data::BoxTreeCell *>(this->getTree()->getRoot());

			this->root->setId(0);

			this->iter = this->boundary->eBegin();

			this->minLevel = this->maxLevel = 0;

			this->status = MJMesh::TreeGenerator::init;
		}
	case MJMesh::TreeGenerator::INIT:

		//while (!this->elements.empty())
		while (iter != this->boundary->eEnd())
		{
			//MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>(this->elements.back());
			MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>((*iter));
			//this->elements.pop_back();

#if USE_JMSH_DEBUG_MESSAGES
			//markos debug
			/*if (this->dimension == 3)
			{
				std::cout << "markos debug, generating tree, face = " << e->getVertex(0)->getId() - 1 << ", " << e->getVertex(1)->getId() - 1 << ", " << e->getVertex(2)->getId() - 1 << std::endl;
				std::cout << "markos debug, generating tree, face->v1 = " << e->getVertex(0)->getPoint()->getCoord(0) << ", " << e->getVertex(0)->getPoint()->getCoord(1) << ", " << e->getVertex(0)->getPoint()->getCoord(2) << std::endl;
				std::cout << "markos debug, generating tree, face->v2 = " << e->getVertex(1)->getPoint()->getCoord(0) << ", " << e->getVertex(1)->getPoint()->getCoord(1) << ", " << e->getVertex(1)->getPoint()->getCoord(2) << std::endl;
				std::cout << "markos debug, generating tree, face->v3 = " << e->getVertex(2)->getPoint()->getCoord(0) << ", " << e->getVertex(2)->getPoint()->getCoord(1) << ", " << e->getVertex(2)->getPoint()->getCoord(2) << std::endl;
				std::cout << "markos debug, generating tree, mid = " << e->getShape()->mid().getCoord(0) << ", " << e->getShape()->mid().getCoord(1) << ", " << e->getShape()->mid().getCoord(2) << std::endl;
				std::cout << "markos debug, generating tree, area = " << e->getShape()->size() << std::endl;
			}*/
			//end markos debug
#endif //#if USE_JMSH_DEBUG_MESSAGES

#if USE_GUI
			e->unhighlight();
#endif //#if USE_GUI

#if USE_GUI
			static_cast<Data::BoxTree *>(this->getTree())->freeze();
#endif //#if USE_GUI

			this->subdivide(this->root, e);

#if USE_GUI
			static_cast<Data::BoxTree *>(this->getTree())->unfreeze();
#endif //#if USE_GUI

            iter++;

#if USE_GUI
			//if (!this->elements.empty())
			if (iter != this->boundary->eEnd())
			{
				(*iter)->highlight();
			}

			if (Data::isStepByStep()) break;
#endif //#if USE_GUI
		}

		//if (this->elements.empty())
		if (iter == this->boundary->eEnd())
		{
			this->root = NULL;

#if USE_GUI
            if (this->getTree())
            {
                for (Data::Tree::LeafIterator iter = this->getTree()->lBegin();
                     iter != this->getTree()->lEnd(); iter++)
                {
                    static_cast<Data::BoxTreeCell *>((*iter))->box();
                }
            }
#endif //#if USE_GUI

			this->status = MJMesh::TreeGenerator::ended;
		}

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI
	}
}

Data::Tree *MJMesh::TreeGenerator::makeTree(const Data::Point2D &min, const Data::Point2D &max) const
{
	return new MJMesh::QuadTree(min, max);
}

Data::Tree *MJMesh::TreeGenerator::makeTree(const Data::Point3D &min, const Data::Point3D &max) const
{
	return new MJMesh::OctTree(min, max);
}

void MJMesh::TreeGenerator::subdivide(Data::BoxTreeCell *c) const
{
	c->subdivide();
}
