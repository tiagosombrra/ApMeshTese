#include "Data/AlternatingDigitalTreeCell.h"

#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace Data;

/*Data::AlternatingDigitalTreeCell::AlternatingDigitalTreeCell(AlternatingDigitalTree *tree) :
	Data::GeometricSearchTreeNode::GeometricSearchTreeNode(tree)
{
	this->active = true;
}*/

Data::AlternatingDigitalTreeCell::AlternatingDigitalTreeCell(AlternatingDigitalTreeCell *parent) :
	Data::GeometricSearchTreeNode::GeometricSearchTreeNode()
{
#if (!USE_EXPERIMENTAL_5)
	//this->add(parent);
	this->setAdjacent(0, parent);
#endif //#if (!USE_EXPERIMENTAL_5)
	this->l = parent ? (parent->level() + 1) : 0;
	this->setTree(parent ? parent->getTree() : NULL);

	this->active = true;

#if USE_EXPERIMENTAL_3
    this->mid = NULL;
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_5
    this->adjacents[0] = parent;

    this->adjacents[1] = this->adjacents[2] = NULL;

    /*if (parent)
    {
        parent->add(this);
    }*/
#endif //#if USE_EXPERIMENTAL_5

#if USE_C__11
	this->shapes.reserve(1);
#endif //#if USE_C__11
}

Data::AlternatingDigitalTreeCell::~AlternatingDigitalTreeCell()
{
#if USE_EXPERIMENTAL_3
    if (this->mid)
    {
        delete this->mid;
    }
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_5
	if (!this->isLeaf())
	{
		this->setTree(NULL);
	}

	for (UInt i = 1; i < 3; i++)
	{
		if (this->adjacents[i])
		{
			delete this->adjacents[i];
		}
	}
#endif //#if USE_EXPERIMENTAL_5
}

#if USE_EXPERIMENTAL_5
UInt Data::AlternatingDigitalTreeCell::numAdjacents() const
{
    UInt count = 1;

    if (this->adjacents[1])
    {
        count++;
    }

    if (this->adjacents[2])
    {
        count++;
    }

    return count;
}

void Data::AlternatingDigitalTreeCell::setAdjacent(UInt i, GraphNode *n)
{
    this->adjacents[i] = n;
}

GraphNode *Data::AlternatingDigitalTreeCell::getAdjacent(UInt i) const
{
    return this->adjacents[i];
}

GraphNodeVector Data::AlternatingDigitalTreeCell::getAdjacents() const
{
    return GraphNodeVector(this->adjacents, this->adjacents + 3);
}

void Data::AlternatingDigitalTreeCell::add(GraphNode *n)
{
    if (!this->adjacents[1])
    {
        this->adjacents[1] = n;

        return;
    }

    if (!this->adjacents[2])
    {
        this->adjacents[2] = n;
    }
}

void Data::AlternatingDigitalTreeCell::remove(GraphNode *n)
{
    for (UInt i = 1; i < 3; i++)
    {
        if (this->adjacents[i] == n)
        {
            this->adjacents[i] = NULL;

            break;
        }
    }
}
#endif //#if USE_EXPERIMENTAL_5

/*const Box &Data::AlternatingDigitalTreeCell::getBox() const
{
	return this->box;
}

bool Data::AlternatingDigitalTreeCell::isActive() const
{
	return this->active;
}*/

AlternatingDigitalTreeCell *Data::AlternatingDigitalTreeCell::leftChild() const
{
	return this->isLeaf() ? NULL : static_cast<AlternatingDigitalTreeCell *>(this->adjacents[1]);
}

AlternatingDigitalTreeCell *Data::AlternatingDigitalTreeCell::rightChild() const
{
	return this->isLeaf() ? NULL : static_cast<AlternatingDigitalTreeCell *>(this->adjacents[2]);
}

#if USE_ARRAY
AlternatingDigitalTreeCell &Data::AlternatingDigitalTreeCell::operator=(const AlternatingDigitalTreeCell &c)
{
	if (this != &c)
	{
		GeometricSearchTreeNode::operator=(c);

#if USE_EXPERIMENTAL_5
        for (UInt i = 0; i < 3; i++)
        {
            this->adjacents[i] = c.adjacents[i];
        }
#endif //#if USE_EXPERIMENTAL_5

		this->box = c.box;
		this->global = c.global;
		this->active = c.active;

#if USE_EXPERIMENTAL_3
        if (c.mid)
        {
            if (this->mid)
            {
                if (this->mid->dimension() == c.mid->dimension())
                {
                    for (UInt i = 0; i < this->mid->dimension(); i++)
                    {
                        this->mid->setCoord(i, c.mid->getCoord(i));
                    }
                }
                else
                {
                    delete this->mid;

                    this->mid = NULL;
                }
            }

            if (!this->mid)
            {
                this->mid = (c.mid->dimension() == 2) ?
                    static_cast<Data::Point *>(new Point2D(c.mid)) :
                    static_cast<Data::Point *>(new Point3D(c.mid));
            }
        }
        else
        {
            if (this->mid)
            {
                delete this->mid;

                this->mid = NULL;
            }
        }
#endif //#if USE_EXPERIMENTAL_3
	}

	return *this;
}
#endif //#if USE_ARRAY

void Data::AlternatingDigitalTreeCell::setShape(const Identifiable *shape)
{
	this->shapes.clear();

	this->shapes.insert(const_cast<Identifiable *>(shape));
}

Identifiable *Data::AlternatingDigitalTreeCell::getShape() const
{
	return (this->shapes.empty()) ? NULL : (*this->shapes.begin());
}

Identifiable *Data::AlternatingDigitalTreeCell::getAndRemoveShape()
{
	if (this->shapes.empty())
	{
		return NULL;
	}

	Identifiable *shape = (*this->shapes.begin());

	this->shapes.clear();

	return shape;
}

void Data::AlternatingDigitalTreeCell::subdivide()
{
#if (!USE_EXPERIMENTAL_5)
	Data::GraphNode *c = NULL;

	this->add(c);
	this->add(c);
#endif //#if (!USE_EXPERIMENTAL_5)
}

void Data::AlternatingDigitalTreeCell::makeLeftChild()
{
	//this->adjacents[1] = new AlternatingDigitalTreeCell(this);
	//this->adjacents[1] = this->makeChild();
	this->setAdjacent(1, this->makeChild());
}

void Data::AlternatingDigitalTreeCell::makeRightChild()
{
	//this->adjacents[2] = new AlternatingDigitalTreeCell(this);
	//this->adjacents[2] = this->makeChild();
	this->setAdjacent(2, this->makeChild());
}

bool Data::AlternatingDigitalTreeCell::inGlobal(const Data::Point *pt) const
{
	UInt dim = pt->dimension();

	for (UInt i = 0; i < dim; i++)
	{
		Real coord = pt->getCoord(i);

		if (coord < this->global.getMin()->getCoord(i))
		{
			return false;
		}

		if (coord > this->global.getMax()->getCoord(i))
		{
			return false;
		}
	}

	return true;
}

bool Data::AlternatingDigitalTreeCell::in(const Data::Box *box) const
{
	UInt dim = box->dimension();

	for (UInt i = 0; i < dim; i++)
	{
		//if (box->getMin()->getCoord(i) < this->box.getMin()->getCoord(i))
		if (box->getMax()->getCoord(i) < this->box.getMin()->getCoord(i))
		{
			return false;
		}

		//if (box->getMax()->getCoord(i) > this->box.getMax()->getCoord(i))
		if (box->getMin()->getCoord(i) > this->box.getMax()->getCoord(i))
		{
			return false;
		}
	}

	return true;
}

bool Data::AlternatingDigitalTreeCell::inGlobal(const Data::Box *box) const
{
	UInt dim = box->dimension();

	for (UInt i = 0; i < dim; i++)
	{
		if (box->getMin()->getCoord(i) < this->global.getMin()->getCoord(i))
		{
			return false;
		}

		if (box->getMax()->getCoord(i) > this->global.getMax()->getCoord(i))
		{
			return false;
		}
	}

	return true;
}

bool Data::AlternatingDigitalTreeCell::add(const Identifiable *shape, const Point *pt, const Box *box)
{
	UInt key = this->level() % pt->dimension();

    if ((this->leftChild()) && (this->rightChild()))
	{
#if USE_EXPERIMENTAL_3
        bool ret = (pt->getCoord(key) < this->mid->getCoord(key)) ?
#else
        bool ret = (pt->getCoord(key) < this->mid.getCoord(key)) ?
#endif //#if USE_EXPERIMENTAL_3
			this->leftChild()->add(shape, pt, box) :
			this->rightChild()->add(shape, pt, box);

        // update the global limits on the way up

        this->adjustGlobal();
        this->adjustGlobalWithBoxOrMid();

        return ret;
    }

    // otherwise put the data here

    if (!this->leftChild())
	{
        // first deal with the case of no left child.  Here there
        // are two cases.  Either the new node goes to the left
        // child or else it replaces the current node that goes to
        // the left.

        if (this->isLeaf())
		{
			this->subdivide();
		}

		this->makeLeftChild();

#if USE_EXPERIMENTAL_3
        if (pt->getCoord(key) < this->mid->getCoord(key))
#else
        if (pt->getCoord(key) < this->mid.getCoord(key))
#endif //#if USE_EXPERIMENTAL_3
		{
#if USE_EXPERIMENTAL_3
			this->leftChild()->mid = (pt->dimension() == 2) ?
                static_cast<Point *>(new Point2D(pt)) :
                static_cast<Point *>(new Point3D(pt));
			this->leftChild()->box = (box) ? *box : Data::Box(*this->leftChild()->mid, *this->leftChild()->mid);
#else
			this->leftChild()->mid = *pt;
			this->leftChild()->box = (box) ? *box : Data::Box(this->leftChild()->mid, this->leftChild()->mid);
#endif //#if USE_EXPERIMENTAL_3
			this->leftChild()->global = this->leftChild()->box;
			this->leftChild()->set(shape, pt, box);
        }
        else
		{
            this->leftChild()->active = this->active;
            this->leftChild()->mid = this->mid;
            this->leftChild()->box = this->box;
            this->leftChild()->global = this->leftChild()->box;
            this->leftChild()->set(this);

            this->active = true;
#if USE_EXPERIMENTAL_3
            this->mid = (pt->dimension() == 2) ?
                static_cast<Point *>(new Point2D(pt)) :
                static_cast<Point *>(new Point3D(pt));
            this->box = (box) ? *box : Data::Box(*this->mid, *this->mid);
#else
            this->mid = *pt;
            this->box = (box) ? *box : Data::Box(this->mid, this->mid);
#endif //#if USE_EXPERIMENTAL_3
            this->set(shape, pt, box);
        }
    }
    else
	{
        // Else the left child exists, if the new data is bigger
        // than the current node it goes to the right

        this->makeRightChild();

#if USE_EXPERIMENTAL_3
        if (pt->getCoord(key) >= this->mid->getCoord(key))
#else
        if (pt->getCoord(key) >= this->mid.getCoord(key))
#endif //#if USE_EXPERIMENTAL_3
		{
#if USE_EXPERIMENTAL_3
			this->rightChild()->mid = (pt->dimension() == 2) ?
                static_cast<Point *>(new Point2D(pt)) :
                static_cast<Point *>(new Point3D(pt));
			this->rightChild()->box = (box) ? *box : Data::Box(*this->rightChild()->mid, *this->rightChild()->mid);
#else
			this->rightChild()->mid = *pt;
			this->rightChild()->box = (box) ? *box : Data::Box(this->rightChild()->mid, this->rightChild()->mid);
#endif //#if USE_EXPERIMENTAL_3
			this->rightChild()->global = this->rightChild()->box;
			this->rightChild()->set(shape, pt, box);
        }
        else
		{
            // the data at the current tree node must go to the
            // right child

            this->rightChild()->active = this->active;
            this->rightChild()->mid = this->mid;
            this->rightChild()->box = this->box;
            this->rightChild()->global = this->rightChild()->box;
            this->rightChild()->set(this);

            // check to see if the new data goes to the node
            // or if we need move the left child to the node
            // and put the new data on the left.

            //int next_key = (this->level()+1) % pt->dimension();
            //if (tree->leftChild()->mid.getCoord(next_key) < pt->getCoord(next_key)) {

#if USE_EXPERIMENTAL_3
            if (this->leftChild()->mid->getCoord(key) < pt->getCoord(key))
#else
            if (this->leftChild()->mid.getCoord(key) < pt->getCoord(key))
#endif //#if USE_EXPERIMENTAL_3
			{
                this->active = true;
#if USE_EXPERIMENTAL_3
				this->mid = (pt->dimension() == 2) ?
                    static_cast<Point *>(new Point2D(pt)) :
                    static_cast<Point *>(new Point3D(pt));
				this->box = (box) ? *box : Data::Box(*this->mid, *this->mid);
#else
				this->mid = *pt;
				this->box = (box) ? *box : Data::Box(this->mid, this->mid);
#endif //#if USE_EXPERIMENTAL_3
				this->set(shape, pt, box);
            }
            else
			{
				//Data::Box tmpglobal = this->global;

                this->active = this->leftChild()->active;
                this->mid = this->leftChild()->mid;
                this->box = this->leftChild()->box;
                this->set(this->leftChild());

                this->leftChild()->active = true;
#if USE_EXPERIMENTAL_3
                this->leftChild()->mid = (pt->dimension() == 2) ?
                    static_cast<Point *>(new Point2D(pt)) :
                    static_cast<Point *>(new Point3D(pt));
                this->leftChild()->box = (box) ? *box : Data::Box(*this->mid, *this->mid);
#else
                this->leftChild()->mid = *pt;
                this->leftChild()->box = (box) ? *box : Data::Box(this->mid, this->mid);
#endif //#if USE_EXPERIMENTAL_3
                this->leftChild()->global = this->leftChild()->box;
				this->leftChild()->set(shape, pt, box);

				/*UInt dim = this->leftChild()->global.dimension();

                for (UInt i = 0; i < dim; i++)
                {
                    this->leftChild()->global.getMin()->setCoord(i, std::min(
                        this->leftChild()->box.getMin()->getCoord(i), tmpglobal.getMin()->getCoord(i)));
                    this->leftChild()->global.getMax()->setCoord(i, std::max(
                        this->leftChild()->box.getMax()->getCoord(i), tmpglobal.getMax()->getCoord(i)));
                }*/
            }
        }
    }

    this->adjustGlobal();
    this->adjustGlobalWithBoxOrMid();

    return true;
}

bool Data::AlternatingDigitalTreeCell::remove(const Identifiable *shape, const Point *pt, const Box *box)
{
	return box ? ((this->removeCheap(shape, pt)) ? true : this->removeFull(shape, box)) : this->removeFull(shape, pt);
}

bool Data::AlternatingDigitalTreeCell::removeCheap(const Identifiable *shape, const Point *pt)
{
	if (this->getShape() == shape)
	{
        this->active = false;

        this->adjustGlobal();

        return true;
    }

    // check the left subtree

    bool found = false;
    UInt key = this->level() % pt->dimension();

    if ((this->leftChild()) &&
#if USE_EXPERIMENTAL_3
		((pt->getCoord(key) <= this->mid->getCoord(key)) || (this->mid->getCoord(key) == this->leftChild()->mid->getCoord(key))))
#else
		((pt->getCoord(key) <= this->mid.getCoord(key)) || (this->mid.getCoord(key) == this->leftChild()->mid.getCoord(key))))
#endif //#if USE_EXPERIMENTAL_3
	{
		found = this->leftChild()->removeCheap(shape, pt);
    }

    // check the right subtree

    if ((!found) &&
		(this->rightChild()) &&
#if USE_EXPERIMENTAL_3
		((pt->getCoord(key) >= this->mid->getCoord(key)) || (this->mid->getCoord(key) == this->rightChild()->mid->getCoord(key))))
#else
		((pt->getCoord(key) >= this->mid.getCoord(key)) || (this->mid.getCoord(key) == this->rightChild()->mid.getCoord(key))))
#endif //#if USE_EXPERIMENTAL_3
	{
        found = this->rightChild()->removeCheap(shape, pt);
    }

    // update the global bounds

    if (found)
	{
        this->adjustGlobal();
        this->adjustGlobalWithBoxOrMid();
    }

    return found;
}

bool Data::AlternatingDigitalTreeCell::removeFull(const Identifiable *shape, const Box *box)
{
	if (this->getShape() == shape)
	{
        this->active = false;

        this->adjustGlobal();

        return true;
    }

    // check the left subtree

    bool found = false;

    if ((this->leftChild()) && (this->leftChild()->inGlobal(box)))
	{
		found = this->leftChild()->removeFull(shape, box);
	}

    // check the right subtree

    if ((!found) && (this->rightChild()) && (this->rightChild()->inGlobal(box)))
	{
		found = this->rightChild()->removeFull(shape, box);
    }

    // update the global bounds

    if (found)
	{
        this->adjustGlobal();
        this->adjustGlobalWithBoxOrMid();
    }

    return found;
}

bool Data::AlternatingDigitalTreeCell::removeFull(const Identifiable *shape, const Point *pt)
{
	if (this->getShape() == shape)
	{
        this->active = false;

        this->adjustGlobal();

        return true;
    }

    // check the left subtree

    bool found = false ;

    if ((this->leftChild()) && (this->leftChild()->inGlobal(pt)))
	{
		found = this->leftChild()->removeFull(shape, pt);
    }

    // check the right subtree

    if ((!found) && (this->rightChild()) && (this->rightChild()->inGlobal(pt)))
	{
        found = this->rightChild()->removeFull(shape, pt);
    }

    // update the global bounds

    if (found)
	{
        this->adjustGlobal();
        this->adjustGlobalWithBoxOrMid();
    }

    return found;
}

void Data::AlternatingDigitalTreeCell::build(UInt numShapes, Identifiable **shapes, Point **pts, Box **boxes)
{
    UInt key = this->level() % pts[0]->dimension();

    Int divider = this->split(key, numShapes, shapes, pts, boxes);

#if USE_EXPERIMENTAL_3
    this->mid = (pts[divider]->dimension() == 2) ?
        static_cast<Data::Point *>(new Point2D(pts[divider])) :
        static_cast<Data::Point *>(new Point3D(pts[divider]));
    this->box = (boxes) ? *boxes[divider] : Data::Box(*this->mid, *this->mid);
#else
    this->mid = *pts[divider];
    this->box = (boxes) ? *boxes[divider] : Data::Box(this->mid, this->mid);
#endif //#if USE_EXPERIMENTAL_3
    this->global = this->box;
    this->set(shapes[divider], pts[divider], boxes ? boxes[divider] : NULL);

	if ((divider != 0) || ((divider + 1) != static_cast<Int>(numShapes)))
	{
        this->subdivide();
	}

    if (divider != 0)
	{
        // Add left subtree and recursively fill it.

        this->makeLeftChild();

        this->leftChild()->build(divider, shapes, pts, boxes);
    }

    if ((divider + 1) != static_cast<Int>(numShapes))
	{
        // Add right subtree and recursively fill it.

        this->makeRightChild();

        this->rightChild()->build(numShapes - divider - 1, &shapes[divider+1], &pts[divider+1], boxes ? &boxes[divider+1] : NULL);
    }

    this->adjustGlobal();
    this->adjustGlobalWithBoxOrMid();
}

GeometricSearchTreeNode::ShapeSet Data::AlternatingDigitalTreeCell::allShapes() const
{
	GeometricSearchTreeNode::ShapeSet shapes;

	this->allShapes(shapes);

	return shapes;
}

void Data::AlternatingDigitalTreeCell::allShapes(GeometricSearchTreeNode::ShapeSet &shapes) const
{
	if ((this->active) && (!this->shapes.empty()))
	{
		shapes.insert(*this->shapes.begin());
	}

	if (this->leftChild())
	{
		this->leftChild()->allShapes(shapes);
	}

	if (this->rightChild())
	{
		this->rightChild()->allShapes(shapes);
	}
}

void Data::AlternatingDigitalTreeCell::allShapes(IdentifiableList &shapes) const
{
	if ((this->active) && (!this->shapes.empty()))
	{
		shapes.push_back(*this->shapes.begin());
	}

	if (this->leftChild())
	{
		this->leftChild()->allShapes(shapes);
	}

	if (this->rightChild())
	{
		this->rightChild()->allShapes(shapes);
	}
}

GeometricSearchTreeNode::ShapeSet Data::AlternatingDigitalTreeCell::inRange(const Box *box) const
{
	GeometricSearchTreeNode::ShapeSet shapes;

	this->inRange(box, shapes);

	return shapes;
}

void Data::AlternatingDigitalTreeCell::inRangeBox(const Box *box, GeometricSearchTreeNode::ShapeSet &shapes) const
{
	UInt key = this->level() % box->dimension();

    // Check to see if the current node overlaps the query box.

    if (this->active)
	{
		if (this->in(box))
		{
			shapes.insert(this->getShape());
		}
    }

    // check the left subtree

    if ((this->leftChild()) &&
#if USE_EXPERIMENTAL_3
		((box->getMin()->getCoord(key) <= this->global.getMax()->getCoord(key)) || (this->mid->getCoord(key) == this->leftChild()->mid->getCoord(key))))
#else
		((box->getMin()->getCoord(key) <= this->global.getMax()->getCoord(key)) || (this->mid.getCoord(key) == this->leftChild()->mid.getCoord(key))))
#endif //#if USE_EXPERIMENTAL_3
	{
        this->leftChild()->inRangeBox(box, shapes);
    }

    // check the right subtree

    if ((this->rightChild()) && (box->getMax()->getCoord(key) >= this->global.getMin()->getCoord(key)))
	{
		this->rightChild()->inRangeBox(box, shapes);
    }
}

void Data::AlternatingDigitalTreeCell::inRangeMid(const Box *box, GeometricSearchTreeNode::ShapeSet &shapes) const
{
	UInt key = this->level() % box->dimension();

    // Check to see if the current node overlaps the query box.

    if (this->active)
	{
		bool out = false;

		UInt dim = box->dimension();

		for (UInt i = 0; i < dim; i++)
		{
#if USE_EXPERIMENTAL_3
			Real coord = this->mid->getCoord(i);
#else
			Real coord = this->mid.getCoord(i);
#endif //#if USE_EXPERIMENTAL_3

			if (coord < box->getMin()->getCoord(i))
			{
				out = true;

				break;
			}

			if (coord > box->getMax()->getCoord(i))
			{
				out = true;

				break;
			}
		}

        if (!out)
		{
			shapes.insert(this->getShape());
        }
    }

    // check the left subtree

    if ((this->leftChild()) && (box->getMin()->getCoord(key) <= this->global.getMax()->getCoord(key)))
	{
		this->leftChild()->inRangeMid(box, shapes);
    }

    // check the right subtree

    if ((this->rightChild()) && (box->getMax()->getCoord(key) >= this->global.getMin()->getCoord(key)))
	{
		this->rightChild()->inRangeMid(box, shapes);
    }
}

void Data::AlternatingDigitalTreeCell::adjustGlobal()
{
	if (this->leftChild())
    {
        if (this->rightChild())
        {
            UInt dim = this->global.dimension();

			for (UInt i = 0; i < dim; i++)
			{
				this->global.getMin()->setCoord(i, std::min(
                    this->leftChild()->global.getMin()->getCoord(i),
                    this->rightChild()->global.getMin()->getCoord(i)));

				this->global.getMax()->setCoord(i, std::max(
                    this->leftChild()->global.getMax()->getCoord(i),
                    this->rightChild()->global.getMax()->getCoord(i)));
			}

			/*for (UInt i = 0; i < dim; i++)
			{
				this->global.getMin()->setCoord(i, std::min(this->global.getMin()->getCoord(i),
                    std::min(this->leftChild()->global.getMin()->getCoord(i), this->rightChild()->global.getMin()->getCoord(i))));

				this->global.getMax()->setCoord(i, std::max(this->global.getMax()->getCoord(i),
                    std::max( this->leftChild()->global.getMax()->getCoord(i), this->rightChild()->global.getMax()->getCoord(i))));
			}*/
        }
        else
        {
            UInt dim = this->global.dimension();

            for (UInt i = 0; i < dim; i++)
			{
				this->global.getMin()->setCoord(i, this->leftChild()->global.getMin()->getCoord(i));
				this->global.getMax()->setCoord(i, this->leftChild()->global.getMax()->getCoord(i));
			}

            /*UInt dim = this->global.dimension();

			for (UInt i = 0; i < dim; i++)
			{
				this->global.getMin()->setCoord(i, std::min(
                    this->leftChild()->global.getMin()->getCoord(i),
                    this->global.getMin()->getCoord(i)));

				this->global.getMax()->setCoord(i, std::max(
                    this->leftChild()->global.getMax()->getCoord(i),
                    this->global.getMax()->getCoord(i)));
			}*/
        }
    }
    else if (this->rightChild())
    {
        UInt dim = this->global.dimension();

        for (UInt i = 0; i < dim; i++)
        {
            this->global.getMin()->setCoord(i, this->rightChild()->global.getMin()->getCoord(i));
            this->global.getMax()->setCoord(i, this->rightChild()->global.getMax()->getCoord(i));
        }

        /*UInt dim = this->global.dimension();

        for (UInt i = 0; i < dim; i++)
        {
            this->global.getMin()->setCoord(i, std::min(
                this->rightChild()->global.getMin()->getCoord(i),
                this->global.getMin()->getCoord(i)));

            this->global.getMax()->setCoord(i, std::max(
                this->rightChild()->global.getMax()->getCoord(i),
                this->global.getMax()->getCoord(i)));
        }*/
    }

	/*if (this->leftChild())
	{
		this->box.setBox(*this->leftChild()->box.getMin(), *this->leftChild()->box.getMax());

        if (this->rightChild())
		{
			UInt dim = this->box.dimension();

			for (UInt i = 0; i < dim; i++)
			{
				Real coord = this->rightChild()->box.getMin()->getCoord(i);

				if (coord < this->box.getMin()->getCoord(i))
				{
					this->box.getMin()->setCoord(i, coord);
				}

				coord = this->rightChild()->box.getMax()->getCoord(i);

				if (coord > this->box.getMax()->getCoord(i))
				{
					this->box.getMax()->setCoord(i, coord);
				}
			}
        }
    }
    else if (this->rightChild())
	{
        this->box.setBox(*this->rightChild()->box.getMin(), *this->rightChild()->box.getMax());
    }*/
}

void Data::AlternatingDigitalTreeCell::adjustGlobalWithBoxOrMid()
{

}

UInt Data::AlternatingDigitalTreeCell::median(UInt comp, UInt numPts, Point **pts) const
{
	// algorithm is from Numerical Recipes

	if (numPts == 1)
	{
		return 0;
	}

	UInt *indices = new UInt[numPts];

	UInt numLeft = numPts;
	UInt numBelow = 0;

	for (UInt i = 0; i < numPts; i++)
	{
		indices[i] = i ;
	}

	while (numLeft > 2)
	{
		Real compValue = (pts[indices[0]]->getCoord(comp) +
			pts[indices[numLeft/2]]->getCoord(comp) +
			pts[indices[numLeft-1]]->getCoord(comp)) / 3.0;

		UInt numLess = 0, numEq = 0;

		for (UInt i = 0; i < numLeft; i++)
		{
			/*if (pts[indices[i]]->getCoord(comp) <= compValue)
			{
				if (pts[indices[i]]->getCoord(comp) < compValue)
				{
					numLess++;
				}
				else
				{
					numEq++;
				}
			}*/

			Real value = pts[indices[i]]->getCoord(comp);

            if (value < compValue)
            {
                numLess++;
            }
            else if (value <= compValue)
            {
                numEq++;
            }
		}

		if ((numLess + numEq == 0) || (numLess == numLeft) || (numEq == numLeft))
		{
			UInt retVal = indices[0];

			delete [] indices;

			return retVal;
		}

		if (numLess + numBelow > numPts / 2)
		{
			// Select numbers less than compValue for further searching.
			UInt nextSlot = 0;

			for (UInt i = 0; i < numLeft; i++)
			{
				if (pts[indices[i]]->getCoord(comp) < compValue)
				{
					//indices[nextSlot] = indices[i];
					//nextSlot++;
					indices[nextSlot++] = indices[i];
				}
			}

			numLeft = numLess;
			//assert (nextSlot == numLeft) ;

		}
		else if (numLess + numEq + numBelow > numPts / 2)
		{
			// Select numbers less than or equal to compValue for further searching.
			UInt nextSlot = 0;

			for (UInt i = 0; i < numLeft; i++)
			{
				if (pts[indices[i]]->getCoord(comp) == compValue)
				{
					//indices[nextSlot] = indices[i];
					//nextSlot++;
					indices[nextSlot++] = indices[i];
				}
			}

			numLeft = numEq;
			numBelow += numLess;
			//assert (nextSlot == numEq) ;
		}
		else
		{
			// Select numbers greater than compValue for further searching.
			UInt nextSlot = 0;

			for (UInt i = 0; i < numLeft; i++)
			{
				if (pts[indices[i]]->getCoord(comp) > compValue)
				{
					//indices[nextSlot] = indices[i];
					//nextSlot++;
					indices[nextSlot++] = indices[i];
				}
			}

			numLeft = numLeft - numLess - numEq ;
			//assert(nextSlot == numLeft) ;
			numBelow += numLess + numEq;
		}
	}

	UInt retVal = 0;

	if (numLeft == 1)
	{
		retVal = indices[0];
	}
	else
	{
		if (pts[indices[0]]->getCoord(comp) > pts[indices[1]]->getCoord(comp))
		{
			UInt swap = indices[0];
			indices[0] = indices[1];
			indices[1] = swap;
		}

		retVal = (numBelow + numLeft > numPts/2 + 1) ? indices[0] : indices[1];
	}

	delete [] indices;

	return retVal;
}

void Data::AlternatingDigitalTreeCell::swap(Int i, Int j,
	Identifiable **shapes, Point **pts, Box **boxes) const
{
    Identifiable *id = shapes[i];
    shapes[i] = shapes[j];
    shapes[j] = id;

    Point *pt = pts[i];
    pts[i] = pts[j];
    pts[j] = pt;

    if (boxes)
	{
		Box *box = boxes[i];
		boxes[i] = boxes[j];
		boxes[j] = box;
	}
}

Int Data::AlternatingDigitalTreeCell::split(UInt comp, UInt numShapes,
	Identifiable **shapes, Point **pts, Box **boxes) const
{
	if (numShapes == 1)
	{
		return 0;
	}

    Int pivot = static_cast<Int>(this->median(comp, numShapes, pts));

    this->swap(pivot, 0, shapes, pts, boxes);

    Real compValue = pts[0]->getCoord(comp);
    Int leftEnd = 0;
    Int rightEnd = numShapes;

    while (leftEnd < rightEnd)
	{
        leftEnd++;
        rightEnd--;

        while ((pts[leftEnd]->getCoord(comp) < compValue) && (leftEnd < rightEnd))
		{
			leftEnd++;
		}

        while ((pts[rightEnd]->getCoord(comp) > compValue) && (leftEnd <= rightEnd))
		{
			rightEnd--;
		}

        if (leftEnd < rightEnd)
		{
			this->swap(leftEnd, rightEnd, shapes, pts, boxes);
		}
    }

    this->swap(rightEnd, 0, shapes, pts, boxes) ;

    Int divider = rightEnd;
    Int balance = numShapes - 2*divider - 1;

    if (balance >= 2)
	{
        leftEnd  = divider;
        rightEnd = numShapes;

        while ((leftEnd < rightEnd) && (balance >= 2))
		{
            leftEnd++;
            rightEnd--;

            while ((pts[leftEnd]->getCoord(comp) <= compValue) && (leftEnd < rightEnd))
			{
                if (pts[leftEnd]->getCoord(comp) == compValue)
                {
                    divider++;
                    balance -= 2;
                }

                leftEnd++;
            }

            while ((pts[rightEnd]->getCoord(comp) != compValue) && (leftEnd < rightEnd))
			{
                rightEnd--;
            }

            if ((leftEnd < rightEnd) && (balance >= 2))
			{
                this->swap(leftEnd, rightEnd, shapes, pts, boxes);

                divider++;
                balance -= 2;
            }
        }
    }
    else if (balance <= -2)
	{
        // Fix up equal keys to the left of the divider.

        leftEnd  = -1;
        rightEnd = divider;

        while ((leftEnd < rightEnd) && (balance <= -2))
		{
            leftEnd++;
            rightEnd--;

            while ((pts[leftEnd]->getCoord(comp) != compValue) && (leftEnd < rightEnd))
			{
                leftEnd++ ;
            }

            while ((pts[rightEnd]->getCoord(comp) >= compValue) && (leftEnd < rightEnd))
			{
                if (pts[rightEnd]->getCoord(comp) == compValue)
                {
                    divider--;
                    balance += 2;
                }

                rightEnd--;
            }

            if ((leftEnd < rightEnd) && (balance <= -2))
			{
				this->swap(leftEnd, rightEnd, shapes, pts, boxes);

				divider--;
				balance += 2;
            }
        }
    }

    return divider;
}

#if USE_GUI
void Data::AlternatingDigitalTreeCell::draw() const
{
	if (this->isLeaf())
	{
		this->box.draw();

		return;
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		if (this->child(i))
		{
			this->child(i)->draw();
		}
	}
}
#endif //#if USE_GUI
