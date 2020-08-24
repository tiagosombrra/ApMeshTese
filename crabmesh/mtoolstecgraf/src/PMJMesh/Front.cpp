#include "PMJMesh/Front.h"

#include "Data/GeometricShape.h"
#include "Data/Box.h"
#include "Data/GeometricSearchTree.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Vertex.h"

#if USE_GUI
#include "Data/State.h"
#endif //#if USE_GUI

using namespace PMJMesh;

PMJMesh::Front::Front(bool sorted, Data::GeometricSearchTree *vstree,
	Data::GeometricSearchTree *festree, ULInt id) :
	MJMesh::Front(sorted, vstree, festree, id)
{
	//std::cout << "new pmjmesh::front at " << this << std::endl;
	//std::cout << "trace: " << Data::trace() << std::endl;

	//this->crossing = this->makeSet();
	this->crossing = NULL;
}

PMJMesh::Front::Front(const MJMesh::Boundary *boundary, Data::GeometricSearchTree *vstree,
	Data::GeometricSearchTree *festree, ULInt id) :
	MJMesh::Front(boundary, vstree, festree, id)
{
	//std::cout << "new pmjmesh::front at " << this << std::endl;
	//std::cout << "trace: " << Data::trace() << std::endl;

	this->crossing = this->makeSet();
}

PMJMesh::Front::~Front()
{
	//std::cout << "delete pmjmesh::front at " << this << ", " << this->size() << ", " << this->verticesSize() << std::endl;
	//std::cout << "trace: " << Data::trace() << std::endl;

	this->deleteSearchTrees();

	this->removeSpecificData();
	//this clears the lists, so that the MJMesh::Front::~Front won't need to
	//  try to remove the specific data anymore
	this->removeSpecificData(true);

	/*this->deleteSearchTrees(true);

	this->removeSpecificData(true);*/

	if (this->crossing)
	{
		delete this->crossing;
	}

#if USE_UNIQUE_SEARCH_TREE
	this->deleteSearchTrees();

	/*if (this->vstree)
	{
		delete this->vstree;

		this->vstree = NULL;
	}

	if (this->festree)
	{
		delete this->festree;

		this->festree = NULL;
	}*/
#endif //#if USE_UNIQUE_SEARCH_TREE
}

#if USE_SET
void PMJMesh::Front::initialize()
{
	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}
}
#endif //#if USE_SET

void PMJMesh::Front::set(const Data::Front *front)
{
	this->sorted = static_cast<const PMJMesh::Front *>(front)->sorted;

	this->setVertices(static_cast<const PMJMesh::Front *>(front)->getVertices());

#if USE_SET
	//this->setVertices(static_cast<const PMJMesh::Front *>(front)->getVerticesSet());
	this->setBoundary(static_cast<const PMJMesh::Front *>(front)->getBoundarySet());
	this->setElements(static_cast<const PMJMesh::Front *>(front)->getElementsSet());
	this->setRejected(static_cast<const PMJMesh::Front *>(front)->getRejectedSet());
	this->setCrossing(static_cast<const PMJMesh::Front *>(front)->getCrossingSet());
#else
	//this->setVertices(static_cast<const PMJMesh::Front *>(front)->getVertices());
	this->setBoundary(static_cast<const PMJMesh::Front *>(front)->getBoundary());
	this->setElements(static_cast<const PMJMesh::Front *>(front)->getElements());
	this->setRejected(static_cast<const PMJMesh::Front *>(front)->getRejected());
	this->setCrossing(static_cast<const PMJMesh::Front *>(front)->getCrossing());
#endif //#if USE_SET
}

Data::GeometricSearchTree *PMJMesh::Front::getVSTree() const
{
	return this->vstree;
}

Data::GeometricSearchTree *PMJMesh::Front::getFESTree() const
{
	return this->festree;
}

void PMJMesh::Front::setBoundary(const Data::FrontElementList &boundary)
{
	this->removeSpecificData(this->boundary, false);

#if USE_SET
	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}
	else
	{
		this->boundary->clear();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}

	for (Data::FrontElementList::const_iterator iter = boundary.begin();
		 iter != boundary.end(); iter++)
	{
		Data::Front::FrontElementIterator it = this->boundary->insert((*iter));

		static_cast<MJMesh::FrontElement *>((*iter))->addFrontData(this, 0, it);
	}
#else
	this->boundary = boundary;

	this->addSpecificData(this->boundary, 0);
#endif //#if USE_SET

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void PMJMesh::Front::setCrossing(const Data::FrontElementList &crossing)
{
	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	this->removeSpecificData(this->crossing, false);

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}

	for (Data::FrontElementList::const_iterator iter = crossing.begin();
		 iter != crossing.end(); iter++)
	{
		Data::Front::FrontElementIterator it = this->crossing->insert((*iter));

		static_cast<MJMesh::FrontElement *>((*iter))->addFrontData(this, 3, it);
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

Data::FrontElementList PMJMesh::Front::getCrossing() const
{
	return Data::FrontElementList(this->crossing->begin(), this->crossing->end());
}

void PMJMesh::Front::addElement(Data::FrontElement *e)
{
#if USE_SET
	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}

	Data::Front::FrontElementIterator it =
		this->elements->insert(e);

	static_cast<MJMesh::FrontElement *>(e)->addFrontData(this, 1, it);
#else
	this->elements.push_back(e);

	static_cast<MJMesh::FrontElement *>(e)->addFrontData(this, 1, --this->elements.end());
#endif //#if USE_SET

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void PMJMesh::Front::addBoundary(Data::FrontElement *e)
{
#if USE_SET
	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}

	Data::Front::FrontElementIterator it =
		this->boundary->insert(e);

	static_cast<MJMesh::FrontElement *>(e)->addFrontData(this, 0, it);
#else
	this->boundary.push_back(e);

	static_cast<MJMesh::FrontElement *>(e)->addFrontData(this, 0, --this->elements.end());
#endif //#if USE_SET

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void PMJMesh::Front::addRejected(Data::FrontElement *e)
{
#if USE_SET
	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}

	Data::Front::FrontElementIterator it =
		this->rejected->insert(e);

	static_cast<MJMesh::FrontElement *>(e)->addFrontData(this, 2, it);
#else
	this->rejected.push_back(e);

	static_cast<MJMesh::FrontElement *>(e)->addFrontData(this, 2, --this->elements.end());
#endif //#if USE_SET

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void PMJMesh::Front::addCrossing(Data::FrontElement *e)
{
	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}

	static_cast<MJMesh::FrontElement *>(e)->addFrontData(this, 3, this->crossing->insert(e));

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void PMJMesh::Front::clear()
{
	MJMesh::Front::clear();

	if (this->crossing)
	{
		this->crossing->clear();
	}
}

/*#if USE_UNIQUE_SEARCH_TREE
void PMJMesh::Front::clearSearchTrees()
{
	if (this->vstree)
	{
		this->vstree->clear();
	}

	if (this->festree)
	{
		this->festree->clear();
	}
}
#endif //#if USE_UNIQUE_SEARCH_TREE*/

/*UInt PMJMesh::Front::size() const
{
	return static_cast<UInt>(MJMesh::Front::size() + this->crossing->size());
}*/

ULInt PMJMesh::Front::size(UShort state) const
{
	if (state < 3)
	{
		return MJMesh::Front::size(state);
	}

	return static_cast<ULInt>(this->crossing ? this->crossing->size() : 0);
}

bool PMJMesh::Front::remove(const Data::FrontElement *e, bool rejecting)
{
	/*{
		std::cout << "debug removing fel " << e->text() << ", " << e << " from front" << std::endl;
	}*/

	if ((!rejecting) && (this->festree))
	{
		this->festree->remove(e);
	}

	MJMesh::FrontElementSpecificData::FrontSpecificData fsp =
		rejecting ? static_cast<const MJMesh::FrontElement *>(e)->getFrontData(this) :
		static_cast<const MJMesh::FrontElement *>(e)->getAndRemoveFrontData(this);

#if USE_SET
	Data::FrontElementMultiSet *list = NULL;
#else
	Data::FrontElementList *list = NULL;
#endif //#if USE_SET

	if (fsp.state == 0)
	{
		list = this->boundary;
	}
	else if (fsp.state == 1)
	{
		list = this->elements;
	}
	else if (fsp.state == 2)
	{
		list = this->rejected;
	}
	else if (fsp.state == 3)
	{
		list = this->crossing;
	}

	if (list)
	{
		list->erase(fsp.frontIt);

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI

		return true;
	}

	return false;
}

#if USE_UNIQUE_SEARCH_TREE
void PMJMesh::Front::prepareForTopologyPhase(bool usingSearchTrees)
{
	/*if (this->vstree)
	{
		delete this->vstree;

		this->vstree = NULL;
	}

	if (this->festree)
	{
		delete this->festree;

		this->festree = NULL;
	}*/

	this->deleteSearchTrees();

	MJMesh::Front::prepareForTopologyPhase(usingSearchTrees);
}
#endif //#if USE_UNIQUE_SEARCH_TREE

bool PMJMesh::Front::reject(Data::FrontElement *e, UInt code)
{
	UShort state = static_cast<MJMesh::FrontElement *>(e)->getFrontState(this);

	if ((code > 0) && (state != 3))
	{
		this->remove(e, true);

		static_cast<MJMesh::FrontElement *>(e)->setFrontData(this, 3, this->crossing->insert(e));

		return true;
	}

	if (state == 3)
	{
		return true;
	}

	return MJMesh::Front::reject(e, code);
}

Data::FrontElement *PMJMesh::Front::getElement(ULInt id) const
{
	Data::FrontElement *e = MJMesh::Front::getElement(id);

	if (e)
	{
		return e;
	}

	e = this->find(this->crossing, id);

	if (e)
	{
		return e;
	}

	return NULL;
}

bool PMJMesh::Front::intercept(const Data::FrontElement *e) const
{
	if (this->festree)
	{
		return MJMesh::Front::intercept(e);
	}

	if (MJMesh::Front::intercept(e))
	{
		return true;
	}

	for (Data::Front::ConstFrontElementIterator iter = this->crossing->begin();
		 iter != this->crossing->end(); iter++)
	{
		if ((*iter) == e)
		{
			continue;
		}

		if ((*iter)->intercept(e))
		{
			return true;
		}
	}

	return false;
}

bool PMJMesh::Front::intercept(const Data::FrontElement *e, const Data::Point *p) const
{
	if (this->festree)
	{
		return MJMesh::Front::intercept(e, p);
	}

	if (MJMesh::Front::intercept(e, p))
	{
		return true;
	}

	for (Data::Front::ConstFrontElementIterator iter = this->crossing->begin();
		 iter != this->crossing->end(); iter++)
	{
		if (((*iter) == e) ||
			((*iter)->getShape()->equal(e->getShape())))
		{
			continue;
		}

		if ((*iter)->intercept(p))
		{
			return true;
		}
	}

	return false;
}

/*Data::FrontElement *PMJMesh::Front::find(const Data::VertexVector &vertices, const Data::VertexList &others) const
{
	if (this->festree)
	{
		return MJMesh::Front::find(vertices, others);
	}

	Data::FrontElement *e = MJMesh::Front::find(vertices, others);

	if (e)
	{
		return e;
	}

    Data::FrontElementList geometricals;

	for (Data::FrontElementMultiSet::const_iterator iter = this->crossing->begin();
		 iter != this->crossing->end(); iter++)
	{
        if ((*iter)->match(vertices))
        {
            return (*iter);
        }

		if ((*iter)->geometricallyMatch(vertices))
		{
			geometricals.push_back((*iter));

			*//*bool accordingToNormal = true;

			for (Data::VertexList::const_iterator iter2 = others.begin();
				 iter2 != others.end(); iter2++)
			{
				if (!(*iter)->accordingToNormal((*iter2)))
				{
					accordingToNormal = false;

					break;
				}
			}

			if (accordingToNormal)
			{
				return (*iter);
			}*//*
		}
	}

	for (Data::FrontElementList::iterator iter = geometricals.begin();
         iter != geometricals.end(); iter++)
    {
        bool accordingToNormal = true;

        for (Data::VertexList::const_iterator iter2 = others.begin();
             iter2 != others.end(); iter2++)
        {
            if (!(*iter)->accordingToNormal(*iter2))
            {
                accordingToNormal = false;

                break;
            }
        }

        if (accordingToNormal)
        {
            return (*iter);
        }
    }

	return NULL;
}*/

void PMJMesh::Front::moveBoundaryToElements()
{
	if (this->boundary->empty())
	{
		return;
	}

	if (this->elements->empty())
	{
		(*this->boundary).swap((*this->elements));

		for (Data::Front::FrontElementIterator iter = this->elements->begin();
			 iter != this->elements->end(); iter++)
		{
			static_cast<MJMesh::FrontElement *>((*iter))->setFrontData(this, 1, iter);
		}
	}
	else
	{
		for (Data::Front::FrontElementIterator iter = this->boundary->begin();
			 iter != this->boundary->end(); iter++)
		{
			Data::Front::FrontElementIterator it = this->elements->insert((*iter));
			static_cast<MJMesh::FrontElement *>((*iter))->setFrontData(this, 1, it);
		}

		this->boundary->clear();
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void PMJMesh::Front::moveRejectedToElements()
{
	this->prepareForTopologyPhase(true);
}

void PMJMesh::Front::moveRejectedToCrossing()
{
	if (!this->rejected)
	{
		this->initialize();

		return;
	}

	if (this->rejected->empty())
	{
		return;
	}

	if (this->crossing->empty())
	{
		(*this->rejected).swap((*this->crossing));

		for (Data::Front::FrontElementIterator iter = this->crossing->begin();
			 iter != this->crossing->end(); iter++)
		{
			static_cast<MJMesh::FrontElement *>((*iter))->setFrontData(this, 3, iter);
		}
	}
	else
	{
		for (Data::Front::FrontElementIterator iter = this->rejected->begin();
			 iter != this->rejected->end(); iter++)
		{
			Data::Front::FrontElementIterator it = this->crossing->insert((*iter));
			static_cast<MJMesh::FrontElement *>((*iter))->setFrontData(this, 3, it);
		}

		this->rejected->clear();
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void PMJMesh::Front::moveCrossingToRejected()
{
	if (this->crossing->empty())
	{
		return;
	}

	if (this->rejected->empty())
	{
		(*this->crossing).swap((*this->rejected));

		for (Data::Front::FrontElementIterator iter = this->rejected->begin();
			 iter != this->rejected->end(); iter++)
		{
			static_cast<MJMesh::FrontElement *>((*iter))->setFrontData(this, 2, iter);
		}
	}
	else
	{
		for (Data::Front::FrontElementIterator iter = this->crossing->begin();
			 iter != this->crossing->end(); iter++)
		{
			Data::Front::FrontElementIterator it = this->rejected->insert((*iter));
			static_cast<MJMesh::FrontElement *>((*iter))->setFrontData(this, 2, it);
		}

		this->crossing->clear();
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

PMJMesh::Front *PMJMesh::Front::setIntersection(PMJMesh::Front *front)
{
	if (!front)
	{
		return NULL;
	}

	PMJMesh::Front *intersection = new PMJMesh::Front(this->isSorted());

#if USE_SET
	//force creation of sets
	//intersection->setCrossing(Data::FrontElementList());
	intersection->initialize();
#endif //#if USE_SET

    ULInt sf = front->size(0) + front->size(1) + front->size(2) + front->size(3);
    ULInt st = this->size(0) + this->size(1) + this->size(2) + this->size(3);

    if ((sf == 0) || (st == 0))
    {
        return intersection;
    }

    PMJMesh::Front *other, *iterated = (sf < st) ? (other = this, front) : (other = front, this);

	for (UShort state = 0; state < 4; state++)
	{
		/*if (front->size(state) == 0)
		{
			continue;
		}*/

		for (Data::Front::FrontElementIterator iter = iterated->eBegin(state);
			 iter != iterated->eEnd(state);)
		{
			if (!other->have((*iter)))
			{
				iter++;

				continue;
			}

			Data::FrontElement *e = (*iter);

			other->remove(e);

			iter = iterated->erase(iter, state);

			intersection->add(e);

			//std::cout << "fel " << e->text() << " added to common" << std::endl;
			//std::cout << "fel " << e->text() << " removed from old" << std::endl;
			//std::cout << "fel " << e->text() << " removed from new" << std::endl;

			for (UInt i = 0; i < e->numVertices(); i++)
			{
				Data::Vertex *v = e->getVertex(i);

				if (!intersection->have(v))
				{
					intersection->add(v);

					//std::cout << "v " << v->text() << " at " << v << " added to common" << std::endl;
				}

				if (other->shouldRemove(v))
				{
					//std::cout << "v " << v->text() << " at " << v << " removed from old" << std::endl;

					other->remove(v);
				}

				if (iterated->shouldRemove(v))
				{
					//std::cout << "v " << v->text() << " at " << v << " removed from new" << std::endl;

					iterated->remove(v);
				}
			}
		}
	}

	return intersection;
}

void PMJMesh::Front::setMerge(PMJMesh::Front *front)
{
	if (!front)
	{
		return;
	}

	for (UShort state = 0; state < 4; state++)
	{
		for (Data::Front::FrontElementIterator iter = front->eBegin(state);
			 iter != front->eEnd(state);)
		{
			if (this->have((*iter)))
			{
				iter++;

				continue;
			}

			Data::FrontElement *e = (*iter);

			iter = front->erase(iter, state);

			this->add(e);

			for (UInt i = 0; i < e->numVertices(); i++)
			{
				Data::Vertex *v = e->getVertex(i);

				if (!this->have(v))
				{
					this->add(v);
				}

				if (front->shouldRemove(v))
				{
					front->remove(v);
				}
			}
		}
	}
}

Data::Front::FrontElementIterator PMJMesh::Front::eBegin(UShort state)
{
	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}

	if (state == 0)
	{
		return this->boundary->begin();
	}

	if (state == 1)
	{
		return this->elements->begin();
	}

	if (state == 2)
	{
		return this->rejected->begin();
	}

	return this->crossing->begin();
}

Data::Front::ConstFrontElementIterator PMJMesh::Front::eBegin(UShort state) const
{
	/*if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}*/

	if (state == 0)
	{
		return this->boundary->begin();
	}

	if (state == 1)
	{
		return this->elements->begin();
	}

	if (state == 2)
	{
		return this->rejected->begin();
	}

	return this->crossing->begin();
}

Data::Front::FrontElementIterator PMJMesh::Front::eEnd(UShort state)
{
	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}

	if (state == 0)
	{
		return this->boundary->end();
	}

	if (state == 1)
	{
		return this->elements->end();
	}

	if (state == 2)
	{
		return this->rejected->end();
	}

	return this->crossing->end();
}

Data::Front::ConstFrontElementIterator PMJMesh::Front::eEnd(UShort state) const
{
	/*if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}*/

	if (state == 0)
	{
		return this->boundary->end();
	}

	if (state == 1)
	{
		return this->elements->end();
	}

	if (state == 2)
	{
		return this->rejected->end();
	}

	return this->crossing->end();
}

Data::Front::FrontElementIterator PMJMesh::Front::erase(Data::Front::FrontElementIterator &iter, UShort state)
{
	if (this->festree)
	{
		this->festree->remove((*iter));
	}

	static_cast<const MJMesh::FrontElement *>((*iter))->removeFrontData(this);

	//Data::Front::FrontElementIterator iter2 = iter;

	//iter2++;

	if (state == 0)
	{
		//this->boundary->erase(iter);
		this->boundary->erase(iter++);
	}
	else if (state == 1)
	{
		//this->elements->erase(iter);
		this->elements->erase(iter++);
	}
	else if (state == 2)
	{
		//this->rejected->erase(iter);
		this->rejected->erase(iter++);
	}
	else
	{
		//this->crossing->erase(iter);
		this->crossing->erase(iter++);
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI

	//return iter2;
	return iter;
}

Data::Front::VertexIterator PMJMesh::Front::replace(Data::Front::VertexIterator &iter, Data::Vertex *v)
{
	this->add(v);

	return this->erase(iter);
}

Data::Front::FrontElementIterator PMJMesh::Front::replace(Data::Front::FrontElementIterator &iter, UShort state, Data::FrontElement *e)
{
	if (state == 0)
	{
		this->addBoundary(e);
	}
	else if (state == 1)
	{
		this->addElement(e);
	}
	else if (state == 2)
	{
		this->addRejected(e);
	}
	else if (state == 3)
	{
		this->addCrossing(e);
	}

	return this->erase(iter, state);
}

void PMJMesh::Front::setCrossing(const Data::FrontElementMultiSet &crossing)
{
	this->removeSpecificData(this->crossing, false);

	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	if (!this->crossing)
	{
		this->crossing = this->makeSet();
	}

	(*this->crossing) = crossing;

	this->addSpecificData(this->crossing, 3);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

const Data::FrontElementMultiSet &PMJMesh::Front::getCrossingSet() const
{
	return (*this->crossing);
}

/*UShort PMJMesh::Front::numStates() const
{
	return 4;
}*/

void PMJMesh::Front::addSpecificData()
{
	MJMesh::Front::addSpecificData();

	this->addSpecificData(this->crossing, 3);
}

void PMJMesh::Front::removeSpecificData(bool ignoreContents)
{
	MJMesh::Front::removeSpecificData(ignoreContents);

	this->removeSpecificData(this->crossing, ignoreContents);
}

void PMJMesh::Front::removeSearchTreeSpecificData() const
{
	this->removeSearchTreeSpecificData(this->crossing);

	MJMesh::Front::removeSearchTreeSpecificData();
}

#if USE_GUI
void PMJMesh::Front::draw() const
{
	if (!this->hasLists())
	{
		this->createList(8);
	}

	if (this->hasList(6))
	{
		this->drawList(6);

		MJMesh::Front::draw();

		return;
	}

	this->beginList(6);

	//MJMesh::Front::draw();

	/*//debug
	if (this->vstree)
	{
		this->vstree->draw();
	}
	//endebug*/

	if (Data::getState().isFrontElementDrawn())
	{
	    Real r, g, b;
        Data::getState().getTypeColor(Data::State::ColoredType::FRONT_REJECTED_FRONT_ELEMENT, r, g, b);

        int modes[2];

		glGetIntegerv(GL_POLYGON_MODE, modes);

		GLboolean lighting = glIsEnabled(GL_LIGHTING);

		this->drawWithArrayLinesOrPlanes(this->crossing->begin(), this->crossing->end(), this->dimension(), this->size(3),
			Data::getState().isIdDrawn(), lighting, Data::getState().getLineWidth(),
			true, ((modes[0] == GL_FILL) || (modes[1] == GL_FILL)), Data::getState().isTriangleBorderDrawn(),
			false, NULL,
			true, r, g, b, false);

		/*if (!this->crossing->empty())
		{
		    GLboolean lighting = glIsEnabled(GL_LIGHTING);

			UInt dimension = this->dimension();

			int modes[2];

			glGetIntegerv(GL_POLYGON_MODE, modes);

			bool wireframe = ((modes[0] != GL_FILL) || (modes[1] != GL_FILL));

			UInt size = dimension*static_cast<UInt>(this->crossing->size());

			GLfloat *fels = new GLfloat[dimension*size];
			GLfloat *colors = new GLfloat[3*size];
			GLfloat *normals = (dimension == 2) ? NULL : new GLfloat[3*size];

			GLuint i = 0;

			for (Data::FrontElementSet::const_iterator iter = this->crossing->begin();
				 (!this->crossing->empty()) && (iter != this->crossing->end()) && (i < size); iter++)
			{
				UInt j = i;

				i += (*iter)->getShape()->fill(fels + dimension*i, NULL, normals ? normals + 3*i : NULL);

				for (; j < i; j++)
				{
					colors[3*j + 0] = static_cast<GLfloat>(r);
					colors[3*j + 1] = static_cast<GLfloat>(g);
					colors[3*j + 2] = static_cast<GLfloat>(b);
				}
			}

			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			if (normals) glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(dimension, GL_FLOAT, 0, fels);
			glColorPointer(3, GL_FLOAT, 0, colors);
			if (normals) glNormalPointer(GL_FLOAT, 0, normals);

			bool lines = (dimension == 2) || ((!this->crossing->empty()) && ((*this->crossing->begin())->numVertices() == 2));

			glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth()));

            if ((lighting) && (lines) && (dimension == 3)) glDisable(GL_LIGHTING);

            glDrawArrays(lines ? GL_LINES : GL_TRIANGLES, 0, i);

            if ((lighting) && (lines) && (dimension == 3)) glEnable(GL_LIGHTING);

            if ((!lines) && (dimension == 3) && (!wireframe) && (Data::getState().isTriangleBorderDrawn()))
			{
				GLboolean lighting = glIsEnabled(GL_LIGHTING);

				if (lighting) glDisable(GL_LIGHTING);

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				glDrawArrays(GL_TRIANGLES, 0, i);

				if (lighting) glEnable(GL_LIGHTING);

				glPolygonMode(GL_FRONT, modes[0]);
				glPolygonMode(GL_BACK,  modes[1]);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			if (normals) glDisableClientState(GL_NORMAL_ARRAY);

			glDisable(GL_COLOR_MATERIAL);

			delete [] fels;
			delete [] colors;
			if (normals) delete [] normals;

			if (Data::getState().isIdDrawn())
			{
				i = 0;

				for (Data::Front::ConstFrontElementIterator iter = this->crossing->begin();
					 (!this->crossing->empty()) && (iter != this->crossing->end()) && (i < size); iter++)
				{
					(*iter)->drawId(r, g, b);
				}
			}
		}*/
	}

	this->endList();

	MJMesh::Front::draw();
}

void PMJMesh::Front::drawPick() const
{
	MJMesh::Front::drawPick();

	if (Data::getState().isFrontElementDrawn())
	{
		for (Data::Front::FrontElementIterator iter = this->crossing->begin();
			 (!this->crossing->empty()) && (iter != this->crossing->end()); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawPick();
			}
		}
	}
}

void PMJMesh::Front::drawNormals(Real factor) const
{
	if (!this->hasLists())
	{
		this->createList(8);
	}

	if (this->hasList(7))
	{
		this->drawList(7);

		MJMesh::Front::drawNormals(factor);

		return;
	}

	this->beginList(7);

	if (Data::getState().isFrontElementDrawn())
	{
	    Real r, g, b;
        Data::getState().getTypeColor(Data::State::ColoredType::FRONT_REJECTED_FRONT_ELEMENT, r, g, b);

		for (Data::FrontElementSet::iterator iter = this->crossing->begin();
			 (!this->crossing->empty()) && (iter != this->crossing->end()); iter++)
		{
			/*Data::FrontElement *fel = (*iter);

			Real r = fel->getR();
			Real g = fel->getG();
			Real b = fel->getB();

			fel->setColor(0.0, 1.0, 0.0);

			fel->drawNormal(factor);

			fel->setColor(r, g, b);

			fel->draw();*/

			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawNormal(r, g, b, factor);
			}
		}
	}

	this->endList();

	MJMesh::Front::drawNormals(factor);
}
#endif //#if USE_GUI
