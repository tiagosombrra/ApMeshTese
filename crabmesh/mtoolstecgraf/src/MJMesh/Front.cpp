#include "MJMesh/Front.h"

#include "Data/GeometricShape.h"
#include "Data/Box.h"
#include "Data/GeometricSearchTree.h"
#include "Data/BoxSearchTree.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/QuadTree.h"
#include "MJMesh/OctTree.h"

#if USE_GUI
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "Data/State.h"
#endif //#if USE_GUI

using namespace MJMesh;

#if USE_SET
/*bool MJMesh::Front::operator()(const Data::FrontElement *e1, const Data::FrontElement *e2) const
{
	std::cout << "debug operator in MJMesh::front, sorted = " << this->isSorted() << std::endl;

	if ((this->isSorted()) && (e1->getShape()->size() < e2->getShape()->size()))
	{
		return true;
	}

	return ((MJMesh::FrontElement *)e1)->getTimestamp() < ((MJMesh::FrontElement *)e2)->getTimestamp();
}*/

/*bool MJMesh::Front::better(const Data::FrontElement *e1, const Data::FrontElement *e2) const
{
	std::cout << "cc" << std::endl;

	if ((this->isSorted()) && (e1->getShape()->size() < e2->getShape()->size()))
	{
		return true;
	}

	return ((MJMesh::FrontElement *)e1)->getTimestamp() < ((MJMesh::FrontElement *)e2)->getTimestamp();
}*/
#endif //#if USE_SET

MJMesh::Front::Front(bool sorted, Data::GeometricSearchTree *vstree,
	Data::GeometricSearchTree *festree, ULInt id) :
	Data::Front(sorted, id)
{
	//std::cout << "new mjmesh::front at " << this << std::endl;
	//std::cout << "trace: " << Data::trace() << std::endl;

	this->step = USE_FRONT;

	this->setVSTree(vstree);
	this->setFESTree(festree);

#if USE_SET
	this->boundary = NULL;
	this->rejected = NULL;
#endif //#if USE_SET
}

MJMesh::Front::Front(const MJMesh::Boundary *boundary,
	Data::GeometricSearchTree *vstree, Data::GeometricSearchTree *festree, ULInt id) :
	Data::Front(boundary, id)
{
	//std::cout << "new mjmesh::front at " << this << std::endl;
	//std::cout << "trace: " << Data::trace() << std::endl;

	this->step = USE_FRONT;

	this->setVSTree(vstree);
	this->setFESTree(festree);

#if USE_SET
	this->boundary = this->makeSet();

	(*this->boundary).swap(*this->elements);

	delete this->elements;

	this->elements = this->makeSet();
	this->rejected = this->makeSet();
#else
	this->boundary.swap(this->elements);
#endif //#if USE_SET

	this->addSpecificData();
}

MJMesh::Front::~Front()
{
	//std::cout << "delete mjmesh::front at " << this << ", " << this->size() << ", " << this->verticesSize() << std::endl;
	//std::cout << "trace: " << Data::trace() << std::endl;

	this->deleteSearchTrees();

	this->removeSpecificData();

#if USE_SET
	if (this->boundary)
	{
		delete this->boundary;
	}

	if (this->rejected)
	{
		delete this->rejected;
	}
#endif //#if USE_SET

	/*while (!this->boundary.empty())
	{
		MJMesh::FrontElement *e = (MJMesh::FrontElement *)this->boundary.back();
		this->boundary.pop_back();

		this->rejected.remove(e);
	}

	while (!this->rejected.empty())
	{
		FrontElement *e = this->rejected.back();
		this->rejected.pop_back();

		if (e)
		{
			delete e;
		}
	}*/
}

void MJMesh::Front::setSorted(bool sorted)
{
#if USE_SET
	bool changed = (this->isSorted() != sorted);
#endif //#if USE_SET

	this->sorted = sorted;

#if USE_SET
	if (changed)
	{
		if (this->boundary)
		{
			Data::FrontElementMultiSet *boundary = this->boundary;

			this->boundary = this->makeSet();

			for (Data::Front::FrontElementIterator iter = boundary->begin();
				 iter != boundary->end(); iter++)
			{
				static_cast<MJMesh::FrontElement *>((*iter))->getFrontData(this).frontIt =
					this->boundary->insert((*iter));
			}

			boundary->clear();

			delete boundary;
		}

		if (this->elements)
		{
			Data::FrontElementMultiSet *elements = this->elements;

			this->elements = this->makeSet();

			for (Data::Front::FrontElementIterator iter = elements->begin();
				 iter != elements->end(); iter++)
			{
				static_cast<MJMesh::FrontElement *>((*iter))->getFrontData(this).frontIt =
				this->elements->insert((*iter));
			}

			elements->clear();

			delete elements;
		}

		if (this->rejected)
		{
			Data::FrontElementMultiSet *rejected = this->rejected;

			this->rejected = this->makeSet();

			for (Data::Front::FrontElementIterator iter = rejected->begin();
				 iter != rejected->end(); iter++)
			{
				static_cast<MJMesh::FrontElement *>((*iter))->getFrontData(this).frontIt =
					this->rejected->insert((*iter));
			}

			rejected->clear();

			delete rejected;
		}
	}
#endif //#if USE_SET
}

void MJMesh::Front::sortVertexs()
{
    this->vertices.sort(    [&](const Data::Vertex * first, const Data::Vertex * second) {   return first->getId() < second->getId();  }    );
}

void MJMesh::Front::set(const Data::Front *front)
{
	this->sorted = static_cast<const MJMesh::Front *>(front)->sorted;

	this->setVertices(static_cast<const MJMesh::Front *>(front)->getVertices());

#if USE_SET
	//this->setVertices(static_cast<const MJMesh::Front *>(front)->getVerticesSet());
	this->setBoundary(static_cast<const MJMesh::Front *>(front)->getBoundarySet());
	this->setElements(static_cast<const MJMesh::Front *>(front)->getElementsSet());
	this->setRejected(static_cast<const MJMesh::Front *>(front)->getRejectedSet());
#else
	//this->setVertices(static_cast<const MJMesh::Front *>(front)->getVertices());
	this->setBoundary(static_cast<const MJMesh::Front *>(front)->getBoundary());
	this->setElements(static_cast<const MJMesh::Front *>(front)->getElements());
	this->setRejected(static_cast<const MJMesh::Front *>(front)->getRejected());
#endif //#if USE_SET
}

Data::FrontElement *MJMesh::Front::getElement(ULInt id) const
{
	Data::FrontElement *e = NULL;

	e = this->find(this->boundary, id);

	if (e)
	{
		return e;
	}

	e = this->find(this->elements, id);

	if (e)
	{
		return e;
	}

	e = this->find(this->rejected, id);

	if (e)
	{
		return e;
	}

	return NULL;
}

#if USE_SET
Data::FrontElementList MJMesh::Front::getBoundary() const
#else
const Data::FrontElementList &MJMesh::Front::getBoundary() const
#endif //#if USE_SET
{
#if USE_SET
	if (!this->boundary)
	{
		return Data::FrontElementList();
	}

	Data::FrontElementList aux(this->boundary->begin(), this->boundary->end());

	return aux;
#else
	return this->boundary;
#endif //#if USE_SET
}

void MJMesh::Front::setVertices(const Data::VertexList &vertices)
{
	this->removeVerticesSpecificData(this->vertices, false);

	this->vertices = vertices;

	this->addVerticesSpecificData(this->vertices);

/*#if USE_SET
	this->vertices.clear();

	for (Data::VertexList::const_iterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		static_cast<MJMesh::Vertex *>((*iter))->addFrontData(this, this->vertices.insert((*iter)).first);
	}
#else
	this->vertices = vertices;

	this->addVerticesSpecificData();
#endif //#if USE_SET*/
}

void MJMesh::Front::setElements(const Data::FrontElementList &elements)
{
	this->removeSpecificData(this->elements, false);

#if USE_SET
	if (!this->boundary)
	{
		this->boundary = this->makeSet();
	}

	if (!this->elements)
	{
		this->elements = this->makeSet();
	}
	else
	{
		this->elements->clear();
	}

	if (!this->rejected)
	{
		this->rejected = this->makeSet();
	}

	for (Data::FrontElementList::const_iterator iter = elements.begin();
		 iter != elements.end(); iter++)
	{
		Data::Front::FrontElementIterator it = this->elements->insert((*iter));

		static_cast<MJMesh::FrontElement *>((*iter))->addFrontData(this, 1, it);
	}
#else
	this->elements = elements;

	this->addSpecificData(this->elements, 1);
#endif //#if USE_SET

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void MJMesh::Front::setRejected(const Data::FrontElementList &rejected)
{
	this->removeSpecificData(this->rejected, false);

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
	else
	{
		this->rejected->clear();
	}

	for (Data::FrontElementList::const_iterator iter = rejected.begin();
		 iter != rejected.end(); iter++)
	{
		Data::Front::FrontElementIterator it = this->rejected->insert((*iter));

		static_cast<MJMesh::FrontElement *>((*iter))->addFrontData(this, 2, it);
	}

	/*for (FrontElementList::const_iterator iter = rejected.begin();
		 iter != rejected.end(); iter++)
	{
		this->rejected->insert((*iter));
	}*/
#else
	this->rejected = rejected;

	this->addSpecificData(this->rejected, 2);
#endif //#if USE_SET

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

#if USE_SET
Data::FrontElementList MJMesh::Front::getRejected() const
#else
const Data::FrontElementList &MJMesh::Front::getRejected() const
#endif //#if USE_SET
{
#if USE_SET
	if (!this->rejected)
	{
		return Data::FrontElementList();
	}

	Data::FrontElementList aux(this->rejected->begin(), this->rejected->end());

	return aux;
#else
	return this->rejected;
#endif //#if USE_SET
}

Data::FrontElementList MJMesh::Front::getAllElements() const
{
#if USE_SET
	Data::FrontElementList fels;

	if (this->boundary)
	{
		fels.insert(fels.end(), this->boundary->begin(), this->boundary->end());
	}

	if (this->elements)
	{
		fels.insert(fels.end(), this->elements->begin(), this->elements->end());
	}

	if (this->rejected)
	{
		fels.insert(fels.end(), this->rejected->begin(), this->rejected->end());
	}
#else
	Data::FrontElementList fels = this->boundary;
	fels.insert(fels.end(), this->elements.begin(), this->elements.end());
	fels.insert(fels.end(), this->rejected.begin(), this->rejected.end());
#endif //#if USE_SET

	return fels;
}

void MJMesh::Front::setVSTree(Data::GeometricSearchTree *vstree)
{
	this->vstree = vstree;
}

void MJMesh::Front::setFESTree(Data::GeometricSearchTree *festree)
{
	this->festree = festree;
}

bool MJMesh::Front::isUsingSearchTrees() const
{
	return this->vstree;
}

bool MJMesh::Front::isUsingRejected() const
{
	return (this->step == USE_REJECTED);
}

void MJMesh::Front::clear()
{
	this->clearSearchTrees();

	this->removeSpecificData();

	Data::Front::clear();

#if USE_SET
	if (this->boundary)
	{
		this->boundary->clear();
	}

	if (this->rejected)
	{
		this->rejected->clear();
	}
#else
	this->boundary.clear();
	this->rejected.clear();
#endif //#if USE_SET

	//this->clearSearchTrees();

	this->step = USE_FRONT;
}

void MJMesh::Front::clearSearchTrees()
{
	this->removeSearchTreeSpecificData();

	if (this->vstree)
	{
		this->vstree->clear();
	}

	if (this->festree)
	{
		this->festree->clear();
	}
}

void MJMesh::Front::deleteSearchTrees(bool ignoreContents)
{
	if (!ignoreContents)
	{
		this->removeSearchTreeSpecificData();
	}

	//Real debugstart = Data::time();

	if (this->vstree)
	{
		delete this->vstree;

		this->vstree = NULL;

		//std::cout << "Time for deleting vtree = " << Data::time() - debugstart << std::endl;
		//debugstart = Data::time();
	}

	if (this->festree)
	{
		delete this->festree;

		this->festree = NULL;

		//std::cout << "Time for deleting fetree = " << Data::time() - debugstart << std::endl;
	}
}

void MJMesh::Front::add(Data::Vertex *v)
{
	/*if (v->getId() == 612)
	{
		std::cout << "MJMesh::Front::add, " << v->text() << std::endl;
	}*/

	/*if (this->have(v))
	{
		//std::cout << "vertex " << v->text() << " is already in front" << std::endl;

		return;
	}*/

	static_cast<MJMesh::Vertex *>(v)->addFrontData(this);

	if (this->vstree)
	{
		this->vstree->add(v);
	}

	this->vertices.push_back(v);

	static_cast<MJMesh::Vertex *>(v)->addFrontData(this, --this->vertices.end());

/*#if USE_SET
	static_cast<MJMesh::Vertex *>(v)->addFrontData(this, this->vertices.insert(v).first);
#else
	this->vertices.push_back(v);

	static_cast<MJMesh::Vertex *>(v)->addFrontData(this, --this->vertices.end());
#endif //#if USE_SET*/

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

/*void MJMesh::Front::add(MJMesh::Vertex *v)
{
	this->add((Data::Vertex *)v);
}*/

bool MJMesh::Front::remove(const Data::Vertex *v)
{
	/*if (v->getId() == 612)
	{
		std::cout << "MJMesh::Front::remove, " << v->text() << std::endl;
	}*/

	/*if (!this->have(v))
	{
		//std::cout << "vertex " << v->text() << " is NOT in front" << std::endl;

		return false;
	}*/

	/*if (std::find(this->vertices.begin(), this->vertices.end(), v) == this->vertices.end())
	{
		std::cout << "vertex at " << v << " is not in front at " << this << std::endl;
		std::cout << "vertex " << v->getId() << " is not in front at " << this << std::endl;
	}*/

	if (this->vstree)
	{
		this->vstree->remove(v);
	}

	MJMesh::VertexSpecificData::FrontSpecificData fsp =
		static_cast<const MJMesh::Vertex *>(v)->getAndRemoveFrontData(this);

	this->vertices.erase(fsp.frontIt);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI

	return true;
}

/*bool MJMesh::Front::remove(const MJMesh::Vertex *v)
{
	return this->remove((const Data::Vertex *)v);
}*/

void MJMesh::Front::add(Data::FrontElement *e)
{
	/*{
		std::cout << "debug adding fel " << e->text() << ", " << e << " to front" << std::endl;
	}*/

	static_cast<MJMesh::FrontElement *>(e)->addFrontData(this);

	if (this->festree)
	{
		this->festree->add(e);
	}

	if (this->step == USE_REJECTED)
	{
		this->step = USE_FRONT;
	}

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

	Data::Front::FrontElementIterator it =
		this->elements->insert(e);

	static_cast<MJMesh::FrontElement *>(e)->setFrontData(this, 1, it);
#else
	if ((this->isSorted()) && (!this->elements.empty()))
	{
		Data::Front::FrontElementIterator position = this->elements.end();

		Real size = e->getShape()->size();
		//Real size = e->getShape()->size() + Data::getTolerance();

		for (Data::Front::FrontElementIterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			GeometricShape *s = (*iter)->getShape();

			if (s->size() > size)
			{
				position = iter;

				break;
			}
		}

		this->elements.insert(position, e);

		static_cast<MJMesh::FrontElement *>(e)->setFrontData(this, 1, position);
	}
	else
	{
		this->elements.push_back(e);

		static_cast<MJMesh::FrontElement *>(e)->setFrontData(this, 1, this->elements.back());
	}
#endif //#if USE_SET
	/*if (this->step == USE_FRONT)
	{
		if ((this->isSorted()) && (!this->elements.empty()))
		{
			Front::FrontElementIterator position = this->elements.end();

			Real size = ((MJMesh::FrontElement *)e)->size();

			for (Data::Front::FrontElementIterator iter = this->elements.begin();
				 iter != this->elements.end(); iter++)
			{
				GeometricShape *s = (*iter)->getShape();

				if (s->size() > size)
				{
					position = iter;

					break;
				}
			}

			this->elements.insert(position, e);
		}
		else
		{
			this->elements.push_back(e);
		}
	}
	else
	{
		this->rejected.push_back(e);
	}*/

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

/*void MJMesh::Front::add(MJMesh::FrontElement *e)
{
	this->add((Data::FrontElement *)e);
}*/

bool MJMesh::Front::remove(const Data::FrontElement *e)
{
	return this->remove(e, false);
}

bool MJMesh::Front::remove(const Data::FrontElement *e, bool rejecting)
{
	if (!rejecting)
	{
		if (this->festree)
		{
			this->festree->remove(e);
		}
	}

	MJMesh::FrontElementSpecificData::FrontSpecificData fsp =
		rejecting ? static_cast<const MJMesh::FrontElement *>(e)->getFrontData(this) :
		static_cast<const MJMesh::FrontElement *>(e)->getAndRemoveFrontData(this);

	if (fsp.state == 0)
	{
#if USE_SET
		this->boundary->erase(fsp.frontIt);
#else
		this->boundary.erase(fsp.frontIt);
#endif //#if USE_SET

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI

		return true;
	}

	if (fsp.state == 1)
	{
#if USE_SET
		this->elements->erase(fsp.frontIt);
#else
		this->elements.erase(fsp.frontIt);
#endif //#if USE_SET

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI

		return true;
	}

	if (fsp.state == 2)
	{
#if USE_SET
		this->rejected->erase(fsp.frontIt);
#else
		this->rejected.erase(fsp.frontIt);
#endif //#if USE_SET

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI

		return true;
	}

	return false;
}

/*bool MJMesh::Front::remove(const MJMesh::FrontElement *e)
{
	return this->remove((const Data::FrontElement *)e);
}*/

void MJMesh::Front::prepareForTopologyPhase(bool usingSearchTrees)
{
#if USE_SET
	if (!this->rejected->empty())
	{
		if (this->elements->empty())
		{
			(*this->rejected).swap((*this->elements));

			for (Data::Front::FrontElementIterator iter = this->elements->begin();
				 iter != this->elements->end(); iter++)
			{
				static_cast<MJMesh::FrontElement *>((*iter))->setFrontData(this, 1, iter);
			}
		}
		else
		{
			for (Data::Front::FrontElementIterator iter = this->rejected->begin();
				 iter != this->rejected->end(); iter++)
			{
				Data::Front::FrontElementIterator it = this->elements->insert((*iter));
				static_cast<MJMesh::FrontElement *>((*iter))->setFrontData(this, 1, it);
			}

			this->rejected->clear();
		}
	}
#else
	if (!this->rejected.empty())
	{
		if (this->elements.empty())
		{
			this->rejected.swap(this->elements);

			for (Data::Front::FrontElementIterator iter = this->elements.begin();
				 iter != this->elements.end(); iter++)
			{
				static_cast<MJMesh::FrontElement *>((*iter))->setFrontData(this, 1, iter);
			}
		}
		else
		{
			for (Data::Front::FrontElementIterator iter = this->rejected.begin();
				 iter != this->rejected.end(); iter++)
			{
				this->elements.push_back((*iter));

				static_cast<MJMesh::FrontElement *>((*iter))->setFrontData(this, 1, this->elements.back());
			}

			this->rejected.clear();
		}
	}
#endif //#if USE_SET

	if (!usingSearchTrees)
	{
		this->deleteSearchTrees();
	}

	this->step = USE_FRONT;

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

bool MJMesh::Front::isEmpty() const
{
/*#if USE_SET
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
#endif //#if USE_SET*/

	if (this->step == USE_FRONT)
	{
#if USE_SET
		if ((!this->boundary->empty()) || (!this->elements->empty()))
#else
		if ((!this->boundary.empty()) || (!this->elements.empty()))
#endif //#if USE_SET
		{
			return false;
		}

		this->step = USE_REJECTED;
	}

#if USE_SET
	return this->rejected->empty();
#else
	return this->rejected.empty();
#endif //#if USE_SET
}

ULInt MJMesh::Front::size() const
{
#if USE_SET
	return static_cast<ULInt>(
		(this->boundary ? this->boundary->size() : 0) +
		(this->elements ? this->elements->size() : 0) +
		(this->rejected ? this->rejected->size() : 0));
#else
	return this->boundary.size() + this->elements.size() + this->rejected.size();
#endif //#if USE_SET
}

ULInt MJMesh::Front::size(UShort state) const
{
#if USE_SET
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
	}*/

	if (state == 0)
	{
		return static_cast<ULInt>(this->boundary->size());
	}

	if (state == 1)
	{
		return static_cast<ULInt>(this->elements->size());
	}

	return static_cast<ULInt>(this->rejected->size());
#else
	if (state == 0)
	{
		return static_cast<ULInt>(this->boundary.size());
	}

	if (state == 1)
	{
		return static_cast<ULInt>(this->elements.size());
	}

	return static_cast<ULInt>(this->rejected.size());
#endif //#if USE_SET
}

Data::FrontElement *MJMesh::Front::next() const
{
#if USE_SET
	return (this->step == USE_FRONT) ?
		   ((this->boundary->empty()) ?
		    ((this->elements->empty()) ?
			 NULL :
			 *this->elements->begin()) :
			 *this->boundary->begin()) :
		   (this->rejected->empty() ?
			NULL :
			*this->rejected->begin());

	/*if (this->step == USE_FRONT)
	{
		if (!this->boundary->empty())
		{
			return *this->boundary->begin();
		}

		if (!this->elements->empty())
		{
			return *this->elements->begin();
		}
	}

	if (!this->rejected->empty())
	{
		return *this->rejected->begin();
	}

	return NULL;*/
#else
	if (this->step == USE_FRONT)
	{
		if (!this->boundary.empty())
		{
			return this->boundary.front();
		}

		if (!this->elements.empty())
		{
			return this->elements.front();
		}
	}

	if (!this->rejected.empty())
	{
		return this->rejected.front();
	}

	return NULL;
#endif //#if USE_SET
}

bool MJMesh::Front::reject(Data::FrontElement *e, UInt /*code*/)
{
	if (this->step == USE_REJECTED)
	{
		return false;
	}

	this->remove(e, true);

#if USE_SET

	Data::Front::FrontElementIterator it =
		this->rejected->insert(e);

	static_cast<MJMesh::FrontElement *>(e)->setFrontData(this, 2, it);
#else
	this->rejected.push_back(e);

	static_cast<MJMesh::FrontElement *>(e)->setFrontData(this, 2, this->rejected.back());
#endif //#if USE_SET

	return true;
}

/*bool MJMesh::Front::reject(MJMesh::FrontElement *e, UInt code)
{
	return this->reject((Data::FrontElement *)e, code);
}*/

bool MJMesh::Front::shouldRemove(const Data::Vertex *v) const
{
	/*if (v->getId() == 612)
	{
		std::cout << "MJMesh::Front::shouldRemove, " << v->text() << std::endl;
	}*/

	return (!this->haveAny(static_cast<const MJMesh::Vertex *>(v)->getAdjacentFrontElements()));

	/*Data::FrontElementList adj = static_cast<const MJMesh::Vertex *>(v)->getAdjacentFrontElements();

	if (this->festree)
	{
		if (adj.empty())
		{
			return true;
		}

		Data::Box box = adj.front()->getShape()->box();

		UInt dim = box.dimension();

		for (Data::Front::FrontElementIterator iter = ++adj.begin();
			 iter != adj.end(); iter++)
		{
			Data::Box b = (*iter)->getShape()->box();

			for (UInt i = 0; i < dim; i++)
			{
				if (b.getMin()->getCoord(i) < box.getMin()->getCoord(i))
				{
					box.getMin()->setCoord(i, b.getMin()->getCoord(i));
				}

				if (b.getMax()->getCoord(i) > box.getMax()->getCoord(i))
				{
					box.getMax()->setCoord(i, b.getMax()->getCoord(i));
				}
			}
		}

		Data::IdentifiableList shapes = this->festree->inRange(box);

		while (!shapes.empty())
		{
			for (Data::Front::FrontElementIterator iter = adj.begin();
				 iter != adj.end(); iter++)
			{
				if (shapes.back() == (*iter))
				{
					return false;
				}
			}

			shapes.pop_back();
		}

		return true;
	}

	for (Data::Front::FrontElementIterator iter = adj.begin();
		 iter != adj.end(); iter++)
	{
		Data::FrontElement *e = (*iter);

#if USE_SET
		Data::Front::ConstFrontElementIterator it;

		if (!this->boundary->empty())
#else
		Data::FrontElementList::const_iterator it;

		if (!this->boundary.empty())
#endif //#if USE_SET
		{
#if USE_SET
			it = std::find(this->boundary->begin(), this->boundary->end(), e);
#else
			it = std::find(this->boundary.begin(), this->boundary.end(), e);
#endif //#if USE_SET

#if USE_SET
			if (it != this->boundary->end())
#else
			if (it != this->boundary.end())
#endif //#if USE_SET
			{
				return false;
			}
		}

#if USE_SET
		if (!this->elements->empty())
#else
		if (!this->elements.empty())
#endif //#if USE_SET
		{
#if USE_SET
			it = std::find(this->elements->begin(), this->elements->end(), e);
#else
			it = std::find(this->elements.begin(), this->elements.end(), e);
#endif //#if USE_SET

#if USE_SET
			if (it != this->elements->end())
#else
			if (it != this->elements.end())
#endif //#if USE_SET
			{
				return false;
			}
		}

#if USE_SET
		if (!this->rejected->empty())
#else
		if (!this->rejected.empty())
#endif //#if USE_SET
		{
#if USE_SET
			it = std::find(this->rejected->begin(), this->rejected->end(), e);
#else
			it = std::find(this->rejected.begin(), this->rejected.end(), e);
#endif //#if USE_SET

#if USE_SET
			if (it != this->rejected->end())
#else
			if (it != this->rejected.end())
#endif //#if USE_SET
			{
				return false;
			}
		}
	}

	return true;*/

	//return this->shouldRemove((const MJMesh::Vertex *)v);

	/*if (!Data::Front::shouldRemove(v))
	{
		return false;
	}

	if (Data::Front::have(this->boundary, v))
	{
		return false;
	}

	if (Data::Front::have(this->rejected, v))
	{
		return false;
	}

	return true;*/
}

/*bool MJMesh::Front::shouldRemove(const MJMesh::Vertex *v) const
{
	Data::FrontElementList adj = v->getAdjacentFrontElements();

	for (Front::FrontElementIterator iter = adj.begin();
		 iter != adj.end(); iter++)
	{
		Data::FrontElement *e = (*iter);

		Data::FrontElementList::const_iterator it = std::find(this->boundary.begin(), this->boundary.end(), e);

		if (it != this->boundary.end())
		{
			return false;
		}

		it = std::find(this->elements.begin(), this->elements.end(), e);

		if (it != this->elements.end())
		{
			return false;
		}

		it = std::find(this->rejected.begin(), this->rejected.end(), e);

		if (it != this->rejected.end())
		{
			return false;
		}
	}

	return true;
}*/

/*bool MJMesh::Front::intercept(const Data::FrontElement *e) const
{
	if (Data::Front::intercept(this->elements, e))
	{
		return true;
	}

	if (Data::Front::intercept(this->boundary, e))
	{
		return true;
	}

	if (Data::Front::intercept(this->rejected, e))
	{
		return true;
	}

	return false;
}*/

/*bool MJMesh::Front::intercept(const MJMesh::FrontElement *e) const
{
	return this->intercept((const Data::FrontElement *)e);
}*/

/*bool MJMesh::Front::intercept(const Data::FrontElement *e, const Data::Point *p) const
{
	if (Data::Front::intercept(this->elements, e, p))
	{
		return true;
	}

	if (Data::Front::intercept(this->boundary, e, p))
	{
		return true;
	}

	if (Data::Front::intercept(this->rejected, e, p))
	{
		return true;
	}

	return false;
}*/

bool MJMesh::Front::interceptElements(const Data::Point *p, const Data::ElementSet &elements) const
{
	for (Data::ElementSet::const_iterator iter = elements.begin();
		 iter != elements.end(); iter++)
	{
		if ((*iter)->in(p))
		{
			return true;
		}
	}

	return false;
}

bool MJMesh::Front::interceptElements(const Data::Point *p, const Data::FrontElementList &frontElements) const
{
	Data::ElementSet elements;

	for (Data::FrontElementList::const_iterator iter = frontElements.begin();
		 iter != frontElements.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			//Data::ElementList els = static_cast<MJMesh::Vertex *>(fel->getVertex(i))->getAdjacentElements();

			//elements.insert(els.begin(), els.end());

			elements.insert(static_cast<MJMesh::Vertex *>(fel->getVertex(i))->eBegin(), static_cast<MJMesh::Vertex *>(fel->getVertex(i))->eEnd());
		}

		/*MJMesh::VertexVector vertices = static_cast<MJMesh::FrontElement *>(fels.back())->getVertices();

		while (!vertices.empty())
		{
			Data::ElementList els = static_cast<MJMesh::Vertex *>(vertices.back())->getAdjacentElements();

			elements.insert(els.begin(), els.end());

			vertices.pop_back();
		}

		fels.pop_back();*/
	}

	return this->interceptElements(p, elements);
}

bool MJMesh::Front::interceptElements(const Data::Point *p) const
{
	Data::ElementSet elements;

	if (this->festree)
	{
		Data::Box box(*p, *p);

		for (UInt i = 0; i < p->dimension(); i++)
		{
			box.getMin()->setCoord(i, box.getMin()->getCoord(i) - Data::getTolerance());
			box.getMax()->setCoord(i, box.getMax()->getCoord(i) + Data::getTolerance());
		}

		Data::IdentifiableList fels = this->festree->inRange(box);

		while (!fels.empty())
		{
			Data::FrontElement *fel = static_cast<Data::FrontElement *>(fels.back());

			fels.pop_back();

			for (UInt i = 0; i < fel->numVertices(); i++)
			{
				//Data::ElementList els = static_cast<MJMesh::Vertex *>(fel->getVertex(i))->getAdjacentElements();

				//elements.insert(els.begin(), els.end());

				elements.insert(static_cast<MJMesh::Vertex *>(fel->getVertex(i))->eBegin(), static_cast<MJMesh::Vertex *>(fel->getVertex(i))->eEnd());
			}

			/*MJMesh::VertexVector vertices = static_cast<MJMesh::FrontElement *>(fels.back())->getVertices();

			while (!vertices.empty())
			{
				Data::ElementList els = static_cast<MJMesh::Vertex *>(vertices.back())->getAdjacentElements();

				elements.insert(els.begin(), els.end());

				vertices.pop_back();
			}

			fels.pop_back();*/
		}
	}
	else
	{
		for (Data::Front::ConstVertexIterator iter = this->vertices.begin();
			 iter != this->vertices.end(); iter++)
		{
			//Data::ElementList els = static_cast<MJMesh::Vertex *>((*iter))->getAdjacentElements();

			//elements.insert(els.begin(), els.end());

			elements.insert(static_cast<MJMesh::Vertex *>((*iter))->eBegin(), static_cast<MJMesh::Vertex *>((*iter))->eEnd());
		}
	}

	return this->interceptElements(p, elements);
}

Data::VertexList MJMesh::Front::verticesInRange(const Data::Box &box) const
{
	if (this->vstree)
	{
		Data::IdentifiableList vs = this->vstree->inRange(box);

		Data::VertexList vertices;

		while (!vs.empty())
		{
			vertices.push_front(static_cast<Data::Vertex *>(vs.back()));

			vs.pop_back();
		}

		return vertices;
	}

	return Data::Front::verticesInRange(box);
}

Data::FrontElementList MJMesh::Front::frontElementsInRange(const Data::Box &box) const
{
	if (this->festree)
	{
		Data::IdentifiableList fes = this->festree->inRange(box);

		Data::FrontElementList fels;

		while (!fes.empty())
		{
			fels.push_front(static_cast<Data::FrontElement *>(fes.back()));

			fes.pop_back();
		}

		return fels;
	}

	Data::FrontElementList fels;

	for (Data::Front::ConstFrontElementIterator iter = this->elements->begin();
		 iter != this->elements->end(); iter++)
	{
		//if ((*iter)->getShape()->box().intercept(box))
		if ((*iter)->getShape()->box().optIntercept(box))
		{
			fels.push_back((*iter));
		}
	}

	for (Data::Front::ConstFrontElementIterator iter = this->boundary->begin();
		 iter != this->boundary->end(); iter++)
	{
		//if ((*iter)->getShape()->box().intercept(box))
		if ((*iter)->getShape()->box().optIntercept(box))
		{
			fels.push_back((*iter));
		}
	}

	for (Data::Front::ConstFrontElementIterator iter = this->rejected->begin();
		 iter != this->rejected->end(); iter++)
	{
		//if ((*iter)->getShape()->box().intercept(box))
		if ((*iter)->getShape()->box().optIntercept(box))
		{
			fels.push_back((*iter));
		}
	}

	return fels;
}

bool MJMesh::Front::have(const Data::Vertex *v) const
{
	return static_cast<const MJMesh::Vertex *>(v)->isInFront(this);
}

bool MJMesh::Front::have(const Data::FrontElement *e) const
{
	return static_cast<const MJMesh::FrontElement *>(e)->isInFront(this);
}

bool MJMesh::Front::have(const Data::FrontElement *e, UShort state) const
{
	return (static_cast<const MJMesh::FrontElement *>(e)->getFrontState(this) == state);
}

UShort MJMesh::Front::getState(const Data::FrontElement *e) const
{
    return static_cast<const MJMesh::FrontElement *>(e)->getFrontState(this);
}

bool MJMesh::Front::haveAny(const Data::FrontElementList &elements) const
{
	for (Data::FrontElementList::const_iterator iter = elements.begin();
		 iter != elements.end(); iter++)
	{
		if (this->have((*iter)))
		{
			return true;
		}
	}

	return false;
}

Data::FrontElement *MJMesh::Front::find(const Data::VertexVector &vertices, const Data::VertexList &others) const
{
	if (vertices.empty())
	{
		return NULL;
	}

	//if (true)
	{
		//Data::FrontElementList fels = static_cast<MJMesh::Vertex *>(vertices[0])->getAdjacentFrontElements();
		MJMesh::Vertex *test = static_cast<MJMesh::Vertex *>(vertices[0]);

		for (UInt i = 1; i < vertices.size(); i++)
		{
			//Data::FrontElementList adjs = static_cast<MJMesh::Vertex *>(vertices[i])->getAdjacentFrontElements();

			//if (adjs.size() < fels.size())
			if (static_cast<MJMesh::Vertex *>(vertices[i])->numAdjacentFrontElements() < test->numAdjacentFrontElements())
			{
				//fels = adjs;
				//fels.swap(adjs);
				test = static_cast<MJMesh::Vertex *>(vertices[i]);
			}
		}

		//Data::FrontElementList matches;
		Data::FrontElementList geometricals;

		//while (!fels.empty())
		for (Data::FrontElementList::iterator iter = test->feBegin();
             iter != test->feEnd(); iter++)
		{
			//Data::FrontElement *e = fels.front();
			Data::FrontElement *e = (*iter);

			//fels.pop_front();

			if (!this->have(e))
            {
                continue;
            }

			if (e->match(vertices))
			{
                //matches.push_back(e);

                if (this->accordingToNormal(e, others))
                {
                    return e;
                }

                continue;
			}

			if (e->geometricallyMatch(vertices))
			{
				geometricals.push_back(e);
			}
		}

		/*if (matches.size() == 1)
        {
            return matches.front();
        }

        for (Data::FrontElementList::iterator iter = matches.begin();
             iter != matches.end(); iter++)
        {
            if (this->accordingToNormal((*iter), others))
            {
                return (*iter);
            }
        }*/

		for (Data::FrontElementList::iterator iter = geometricals.begin();
             iter != geometricals.end(); iter++)
        {
            if (this->accordingToNormal((*iter), others))
            {
                return (*iter);
            }
        }

		return NULL;
	}

	/*if (this->festree)
	{
		Data::Point min(vertices[0]->getPoint());
		Data::Point max(vertices[0]->getPoint());

		for (UInt i = 1; i < vertices.size(); i++)
		{
			Data::Point *p = vertices[i]->getPoint();

			for (UInt j = 0; j < p->dimension(); j++)
			{
				if (p->getCoord(j) < min.getCoord(j))
				{
					min.setCoord(j, p->getCoord(j));
				}

				if (p->getCoord(j) > max.getCoord(j))
				{
					max.setCoord(j, p->getCoord(j));
				}
			}
		}

		Data::Box box(min, max);

		ShapeList shapes = this->festree->inRange(box);

		while (!shapes.empty())
		{
			Data::FrontElement *e = static_cast<Data::FrontElement *>(shapes.back());

			shapes.pop_back();

			if (e->match(vertices))
			{
				return e;
			}
		}

		return NULL;
	}

	Data::FrontElement *e = Data::Front::find(this->elements, vertices);

	if (e)
	{
		return e;
	}

	e = Data::Front::find(this->boundary, vertices);

	if (e)
	{
		return e;
	}

	return Data::Front::find(this->rejected, vertices);*/
}

Data::Front::FrontElementIterator MJMesh::Front::eBegin(UShort state)
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
#endif //#if USE_SET

	if (state == 0)
	{
#if USE_SET
		return this->boundary->begin();
#else
		return this->boundary.begin();
#endif //#if USE_SET
	}

	if (state == 1)
	{
#if USE_SET
		return this->elements->begin();
#else
		return this->elements.begin();
#endif //#if USE_SET
	}

#if USE_SET
	return this->rejected->begin();
#else
	return this->rejected.begin();
#endif //#if USE_SET
}

Data::Front::ConstFrontElementIterator MJMesh::Front::eBegin(UShort state) const
{
/*#if USE_SET
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
#endif //#if USE_SET*/

	if (state == 0)
	{
#if USE_SET
		return this->boundary->begin();
#else
		return this->boundary.begin();
#endif //#if USE_SET
	}

	if (state == 1)
	{
#if USE_SET
		return this->elements->begin();
#else
		return this->elements.begin();
#endif //#if USE_SET
	}

#if USE_SET
	return this->rejected->begin();
#else
	return this->rejected.begin();
#endif //#if USE_SET
}

Data::Front::FrontElementIterator MJMesh::Front::eEnd(UShort state)
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
#endif //#if USE_SET

	if (state == 0)
	{
#if USE_SET
		return this->boundary->end();
#else
		return this->boundary.end();
#endif //#if USE_SET
	}

	if (state == 1)
	{
#if USE_SET
		return this->elements->end();
#else
		return this->elements.end();
#endif //#if USE_SET
	}

#if USE_SET
	return this->rejected->end();
#else
	return this->rejected.end();
#endif //#if USE_SET
}

Data::Front::ConstFrontElementIterator MJMesh::Front::eEnd(UShort state) const
{
/*#if USE_SET
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
#endif //#if USE_SET*/

	if (state == 0)
	{
#if USE_SET
		return this->boundary->end();
#else
		return this->boundary.end();
#endif //#if USE_SET
	}

	if (state == 1)
	{
#if USE_SET
		return this->elements->end();
#else
		return this->elements.end();
#endif //#if USE_SET
	}

#if USE_SET
	return this->rejected->end();
#else
	return this->rejected.end();
#endif //#if USE_SET
}

Data::Front::VertexIterator MJMesh::Front::erase(Data::Front::VertexIterator &iter)
{
	/*if ((*iter)->getId() == 612)
	{
		std::cout << "MJMesh::Front::erase, " << (*iter)->text() << std::endl;
	}*/

	if ((this->vstree))
	{
		this->vstree->remove((*iter));
	}

	static_cast<const MJMesh::Vertex *>((*iter))->removeFrontData(this);

	/*//Data::Front::VertexIterator iter2 = iter;

	//iter2++;

	//this->vertices.erase(iter);
	this->vertices.erase(iter++);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI

	//return iter2;
	return iter;*/
#if USE_GUI
	Data::Front::VertexIterator iter2 = this->vertices.erase(iter);

	this->outdate();

	return iter2;
#else
	return this->vertices.erase(iter);
#endif //#if USE_GUI
}

Data::Front::FrontElementIterator MJMesh::Front::erase(Data::Front::FrontElementIterator &iter, UShort state)
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
	else
	{
		//this->rejected->erase(iter);
		this->rejected->erase(iter++);
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI

	//return iter2;
	return iter++;
}

#if USE_GUI
void MJMesh::Front::draw() const
{
	if (!this->hasLists())
	{
		this->createList(6);
	}

	if (this->hasList(3))
	{
		this->drawList(3);

		return;
	}

	this->beginList(3);

	//glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth()));

    //glPointSize(static_cast<GLfloat>(Data::getState().getPointSize()));

	if ((Data::getState().isFrontElementDrawn()) || (Data::getState().isVertexDrawn()))
	{
		GLboolean lighting = glIsEnabled(GL_LIGHTING);

	    if (Data::getState().isFrontElementDrawn())
		{
			int modes[2];

			glGetIntegerv(GL_POLYGON_MODE, modes);

			Real r, g, b;
			Data::getState().getTypeColor(Data::State::ColoredType::FRONT_ELEMENT_FRONT_ELEMENT, r, g, b);

#if USE_SET
			this->drawWithArrayLinesOrPlanes(this->elements->begin(), this->elements->end(), this->dimension(), this->size(1),
#else
			this->drawWithArrayLinesOrPlanes(this->elements.begin(), this->elements.end(), this->dimension(), this->size(1),
#endif //
				Data::getState().isIdDrawn(), lighting, Data::getState().getLineWidth(),
				true, ((modes[0] == GL_FILL) || (modes[1] == GL_FILL)), Data::getState().isTriangleBorderDrawn(),
				false, NULL,
				true, r, g, b, false);

/*#if USE_SET
		this->draw(*this->elements, r, g, b, lighting);
#else
		this->draw(this->elements, r, g, b, lighting);
#endif //#if USE_SET*/

			Data::getState().getTypeColor(Data::State::ColoredType::FRONT_BOUNDARY_FRONT_ELEMENT, r, g, b);

/*#if USE_SET
		this->draw(*this->boundary, r, g, b, lighting);
#else
		this->draw(this->boundary, r, g, b, lighting);
#endif //#if USE_SET*/

#if USE_SET
			this->drawWithArrayLinesOrPlanes(this->boundary->begin(), this->boundary->end(), this->dimension(), static_cast<UInt>(this->boundary->size()),
#else
			this->drawWithArrayLinesOrPlanes(this->boundary.begin(), this->boundary.end(), this->dimension(), static_cast<UInt>(this->boundary.size()),
#endif //#if USE_SET
				Data::getState().isIdDrawn(), lighting, Data::getState().getLineWidth(),
				true, ((modes[0] == GL_FILL) || (modes[1] == GL_FILL)), Data::getState().isTriangleBorderDrawn(),
				false, NULL,
				true, r, g, b, false);

			Data::getState().getTypeColor(Data::State::ColoredType::FRONT_REJECTED_FRONT_ELEMENT, r, g, b);

/*#if USE_SET
		this->draw(*this->rejected, r, g, b, lighting);
#else
		this->draw(this->rejected, r, g, b, lighting);
#endif //#if USE_SET*/

#if USE_SET
			this->drawWithArrayLinesOrPlanes(this->rejected->begin(), this->rejected->end(), this->dimension(), static_cast<UInt>(this->rejected->size()),
#else
			this->drawWithArrayLinesOrPlanes(this->rejected.begin(), this->rejected.end(), this->dimension(), static_cast<UInt>(this->rejected.size()),
#endif //#if USE_SET
				Data::getState().isIdDrawn(), lighting, Data::getState().getLineWidth(),
				true, ((modes[0] == GL_FILL) || (modes[1] == GL_FILL)), Data::getState().isTriangleBorderDrawn(),
				false, NULL,
				true, r, g, b, false);
		}

		if (Data::getState().isVertexDrawn())
		{
			Data::getState().getTypeColor(Data::State::ColoredType::FRONT_VERTEX, r, g, b);

			this->drawWithArrayPoints(this->vertices.begin(), this->vertices.end(), this->dimension(), static_cast<UInt>(this->vertices.size()),
				Data::getState().isIdDrawn(), false, false, Data::getState().getPointSize(false), Data::getState().getPointSize(true),
				true, r, g, b, false);
		}
	}

	this->endList();
}

/*#if USE_SET
void MJMesh::Front::draw(const Data::FrontElementMultiSet &elements, Real r, Real g, Real b, GLboolean lighting) const
#else
void MJMesh::Front::draw(const Data::FrontElementList &elements, Real r, Real g, Real b, GLboolean lighting) const
#endif //#if USE_SET
{
	if (elements.empty())
	{
		return;
	}

	UInt dimension = this->dimension();

	int modes[2];

	glGetIntegerv(GL_POLYGON_MODE, modes);

	//bool wireframe = ((modes[0] != GL_FILL) || (modes[1] != GL_FILL));

	//GLboolean lighting = glIsEnabled(GL_LIGHTING);

	if (Data::getState().isFrontElementDrawn())
	{
		UInt size = dimension*static_cast<UInt>(elements.size());

		GLfloat *fels = new GLfloat[dimension*size];
		GLfloat *colors = new GLfloat[3*size];
		GLfloat *normals = (dimension == 2) ? NULL : new GLfloat[3*size];

		GLuint i = 0;

		for (Data::Front::ConstFrontElementIterator iter = elements.begin();
			 (!elements.empty()) && (iter != elements.end()) && (i < size); iter++)
		{
			UInt j = i;

			if ((*iter) && ((*iter)->getShape()))
            {
                i += (*iter)->getShape()->fill(fels + dimension*i, NULL, normals ? normals + 3*i : NULL);
            }

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

		bool lines = (dimension == 2) || ((!elements.empty()) && ((*elements.begin())->numVertices() == 2));

		if ((lighting) && (lines) && (dimension == 3)) glDisable(GL_LIGHTING);

		glDrawArrays(lines ? GL_LINES : GL_TRIANGLES, 0, i);

        if ((lighting) && (lines) && (dimension == 3)) glEnable(GL_LIGHTING);

        if ((!lines) && (dimension == 3) && (!wireframe) && (Data::getState().isTriangleBorderDrawn()))
		{
			//GLboolean lighting = glIsEnabled(GL_LIGHTING);

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

			for (Data::Front::ConstFrontElementIterator iter = elements.begin();
				 (!elements.empty()) && (iter != elements.end()) && (i < size); iter++)
			{
				(*iter)->drawId(r, g, b);
			}
		}
	}
}*/

void MJMesh::Front::drawPick() const
{
	if (Data::getState().isFrontElementDrawn())
	{
		for (Data::Front::ConstFrontElementIterator iter = this->boundary->begin();
			 (!this->boundary->empty()) && (iter != this->boundary->end()); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawPick();
			}
		}

		for (Data::Front::ConstFrontElementIterator iter = this->rejected->begin();
			 (!this->rejected->empty()) && (iter != this->rejected->end()); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawPick();
			}
		}
	}

	Data::Front::drawPick();
}

void MJMesh::Front::drawNormals(Real factor) const
{
	if (!this->hasLists())
	{
		this->createList(6);
	}

	if (this->hasList(4))
	{
		this->drawList(4);

		Data::Front::drawNormals(factor);

		return;
	}

	this->beginList(4);

	if (Data::getState().isFrontElementDrawn())
	{
	    Real r, g, b;
        Data::getState().getTypeColor(Data::State::ColoredType::FRONT_BOUNDARY_FRONT_ELEMENT, r, g, b);

		for (Data::Front::ConstFrontElementIterator iter = this->boundary->begin();
			 (!this->boundary->empty()) && (iter != this->boundary->end()); iter++)
		{
			/*Data::FrontElement *fel = (*iter);

			Real r = fel->getR();
			Real g = fel->getG();
			Real b = fel->getB();

			fel->setColor(this->getR(), this->getG(), this->getB());

			fel->drawNormal(factor);

			fel->setColor(r, g, b);*/

			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawNormal(r, g, b, factor);
			}
		}

		Data::getState().getTypeColor(Data::State::ColoredType::FRONT_REJECTED_FRONT_ELEMENT, r, g, b);

		for (Data::Front::ConstFrontElementIterator iter = this->rejected->begin();
			 (!this->rejected->empty()) && (iter != this->rejected->end()); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawNormal(r, g, b, factor);
			}
		}
	}

	this->endList();

	Data::Front::drawNormals(factor);
}

void MJMesh::Front::drawDiscretization() const
{
	if (!this->hasLists())
	{
		this->createList(6);
	}

	if (this->hasList(5))
	{
		this->drawList(5);

		return;
	}

	this->beginList(5);

	if (Data::getState().isVertexDrawn())
	{
	    Real r, g, b;
        Data::getState().getTypeColor(Data::State::ColoredType::FRONT_VERTEX, r, g, b);

		if (!this->vertices.empty())
		{
			UInt dimension = (*this->vertices.begin())->getPoint()->dimension();

			for (Data::Front::ConstVertexIterator iter = this->vertices.begin();
				 iter != this->vertices.end(); iter++)
			{
				if (!Data::getState().isInVisibleSpace((*iter)->getPoint()))
				{
					continue;
				}

				MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

				//Data::FrontElementList fels = v->getAdjacentFrontElements();
				Data::FrontElementList bfels;

				//while (!fels.empty())
				for (Data::FrontElementList::iterator iter2 = v->feBegin();
					 iter2 != v->feEnd(); iter2++)
				{
#if USE_SET
					//bool found = (std::find(this->boundary->begin(), this->boundary->end(), fels.front()) != this->boundary->end()) ||
					bool found = (std::find(this->boundary->begin(), this->boundary->end(), (*iter2)) != this->boundary->end()) ||
						//(std::find(this->elements->begin(), this->elements->end(), fels.front()) != this->elements->end()) ||
						(std::find(this->elements->begin(), this->elements->end(), (*iter2)) != this->elements->end()) ||
						//(std::find(this->rejected->begin(), this->rejected->end(), fels.front()) != this->rejected->end());
						(std::find(this->rejected->begin(), this->rejected->end(), (*iter2)) != this->rejected->end());
#else
					//bool found = (std::find(this->boundary.begin(), this->boundary.end(), fels.front()) != this->boundary.end()) ||
					bool found = (std::find(this->boundary.begin(), this->boundary.end(), (*iter2)) != this->boundary.end()) ||
						//(std::find(this->elements.begin(), this->elements.end(), fels.front()) != this->elements.end()) ||
						(std::find(this->elements.begin(), this->elements.end(), (*iter2)) != this->elements.end()) ||
						//(std::find(this->rejected.begin(), this->rejected.end(), fels.front()) != this->rejected.end());
						(std::find(this->rejected.begin(), this->rejected.end(), (*iter2)) != this->rejected.end());
#endif //#if USE_SET

					if (found)
					{
						//bfels.push_back(fels.front());
						bfels.push_back((*iter2));
					}

					//fels.pop_front();
				}

				if (bfels.empty())
				{
					continue;
				}

				//Data::Vector vec(dimension);
				VECTORd(vec, dimension);

				/*while (!bfels.empty())
				{
					MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>(bfels.front());
					bfels.pop_front();

					Data::Vector normal = (dimension == 2) ?
						static_cast<Data::Vector>(static_cast<Data::Edge2D *>(e->getShape())->normal()) :
						static_cast<Data::Vector>(static_cast<Data::Triangle3D *>(e->getShape())->normal());

					//normal.normalize();

					vec.sum(normal);
				}

				vec.normalize();

				vec.multiply(0.4);

				Data::Vector s = vec;

				s.multiply(-1.0);

				vec.sum(v->getPoint());
				s.sum(v->getPoint());*/

				Real size = 0.0, count = static_cast<Real>(bfels.size());

				while (!bfels.empty())
				{
					MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>(bfels.front());
					bfels.pop_front();

					//Data::Vector normal = (dimension == 2) ?
					//	static_cast<Data::Vector>(static_cast<Data::Edge2D *>(e->getShape())->normal()) :
					//	static_cast<Data::Vector>(static_cast<Data::Triangle3D *>(e->getShape())->normal());

					size += e->getShape()->size();

					//normal.normalize();

					//vec.sum(normal);
					if (dimension == 2)
					{
						vec.sum(static_cast<Data::Edge2D *>(e->getShape())->normal());
					}
					else
					{
						vec.sum(static_cast<Data::Triangle3D *>(e->getShape())->normal());
					}
				}

				vec.normalize();

				size /= count*2.0;

				vec.multiply(size);
				//vec.multiply(10.0);
				//vec.multiply(0.5);

				//Data::Vector s = vec;
				VECTORv(s, dimension, vec);

				s.multiply(-1.0);

				vec.sum(v->getPoint());
				s.sum(v->getPoint());

				if (dimension == 2)
				{
					Data::Point2D p1(vec);
					Data::Point2D p2(s);

					Data::Edge2D e(&p1, &p2);

					e.unhighlight();

					e.setColor(r, g, b);
					//e.draw(false);
					e.draw();

					e.setPoints(NULL, NULL);
				}
				else if (dimension == 3)
				{
					Data::Point3D p1(vec);
					Data::Point3D p2(s);

					Data::Edge e(&p1, &p2);

					e.unhighlight();

					e.setColor(r, g, b);
					//e.draw(false);
					e.draw();

					e.setPoints(NULL, NULL);
				}
			}
		}
	}

	this->endList();
}
#endif //#if USE_GUI

#if USE_SET
/*void MJMesh::Front::setVertices(const Data::VertexSet &vertices)
{
	this->removeVerticesSpecificData(this->vertices, false);

	this->vertices = vertices;

	this->addVerticesSpecificData();

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}*/

void MJMesh::Front::setBoundary(const Data::FrontElementMultiSet &boundary)
{
	this->removeSpecificData(this->boundary, false);

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

	(*this->boundary) = boundary;

	this->addSpecificData(this->boundary, 0);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

const Data::FrontElementMultiSet &MJMesh::Front::getBoundarySet() const
{
	return (*this->boundary);
}

void MJMesh::Front::setElements(const Data::FrontElementMultiSet &elements)
{
	this->removeSpecificData(this->elements, false);

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

	(*this->elements) = elements;

	this->addSpecificData(this->elements, 1);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void MJMesh::Front::setRejected(const Data::FrontElementMultiSet &rejected)
{
	this->removeSpecificData(this->rejected, false);

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

	(*this->rejected) = rejected;

	this->addSpecificData(this->rejected, 2);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

const Data::FrontElementMultiSet &MJMesh::Front::getRejectedSet() const
{
	return (*this->rejected);
}

UShort MJMesh::Front::numStates() const
{
	return 3;
}

/*Data::FrontElementMultiSet *MJMesh::Front::makeSet() const
{
	//return new Data::FrontElementMultiSet(MJMesh::Front(this->isSorted()));
	return Data::Front::makeSet();
}*/
#endif //#if USE_SET

#if USE_SET
void MJMesh::Front::addSpecificData(Data::FrontElementMultiSet *list, UShort state) const
#else
void MJMesh::Front::addSpecificData(Data::FrontElementList &list, UShort state) const
#endif //#if USE_SET
{
#if USE_SET
	if (!list)
	{
		return;
	}
#endif //#if USE_SET

	for (Data::Front::FrontElementIterator iter = list->begin();
		 //(!list->empty()) && (iter != list->end()); iter++)
		 iter != list->end(); iter++)
	{
		static_cast<MJMesh::FrontElement *>((*iter))->addFrontData(this, state, iter);
	}
}

#if USE_SET
void MJMesh::Front::removeSpecificData(Data::FrontElementMultiSet *list, bool ignoreContents) const
#else
void MJMesh::Front::removeSpecificData(Data::FrontElementList &list, bool ignoreContents) const
#endif //#if USE_SET
{
#if USE_SET
	if (!list)
	{
		return;
	}

	if (ignoreContents)
	{
		list->clear();

		return;
	}
#else
	if (ignoreContents)
	{
		list.clear();

		return;
	}
#endif //#if USE_SET

	for (Data::Front::FrontElementIterator iter = list->begin();
		 //(!list->empty()) && (iter != list->end()); iter++)
		 iter != list->end(); iter++)
	{
		/*Data::Front::FrontElementIterator iter2 = iter;
		for (++iter2; iter2 != list->end(); iter2++)
		{
			if ((*iter2) == (*iter))
			{
				std::cout << "fel at " << (*iter) << " appeared again in front at " << this << std::endl;
				std::cout << "fel " << (*iter)->getId() << " appeared again in front at " << this << std::endl;
			}
		}*/

		static_cast<MJMesh::FrontElement *>((*iter))->removeFrontData(this);
	}
}

void MJMesh::Front::addVerticesSpecificData(Data::VertexList &vertices) const
{
	for (Data::VertexList::iterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		/*if ((*iter)->getId() == 612)
		{
			std::cout << "MJMesh::Front::addVerticesSpecificData, " << (*iter)->text() << std::endl;
		}*/

		static_cast<MJMesh::Vertex *>((*iter))->addFrontData(this, iter);
	}
}

void MJMesh::Front::removeVerticesSpecificData(Data::VertexList &vertices, bool ignoreContents) const
/*#if USE_SET
void MJMesh::Front::removeVerticesSpecificData(Data::VertexSet &vertices, bool ignoreContents) const
#else
void MJMesh::Front::removeVerticesSpecificData(Data::VertexList &vertices, bool ignoreContents) const
#endif //#if USE_SET*/
{
	if (ignoreContents)
	{
		vertices.clear();

		return;
	}

	for (Data::Front::ConstVertexIterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		/*if ((*iter)->getId() == 160)
		{
			std::cout << "MJMesh::Front::removeVerticesSpecificData, at " << (*iter) << std::endl;
			//std::cout << "MJMesh::Front::removeVerticesSpecificData, " << (*iter)->text() << std::endl;
		}*/

		static_cast<MJMesh::Vertex *>((*iter))->removeFrontData(this);
	}
}

void MJMesh::Front::addSpecificData()
{
	this->addVerticesSpecificData(this->vertices);

	this->addSpecificData(this->boundary, 0);
	this->addSpecificData(this->elements, 1);
	this->addSpecificData(this->rejected, 2);
}

void MJMesh::Front::removeSpecificData(bool ignoreContents)
{
	this->removeVerticesSpecificData(this->vertices, ignoreContents);

	this->removeSpecificData(this->boundary, ignoreContents);
	this->removeSpecificData(this->elements, ignoreContents);
	this->removeSpecificData(this->rejected, ignoreContents);
}

#if USE_SET
void MJMesh::Front::removeSearchTreeSpecificData(Data::FrontElementMultiSet *list) const
#else
void MJMesh::Front::removeSearchTreeSpecificData(Data::FrontElementList &list) const
#endif //#if USE_SET
{
#if USE_SET
	if (!list)
	{
		return;
	}
#endif //#if USE_SET

    if (!this->festree)
    {
        return;
    }

	for (Data::Front::FrontElementIterator iter = list->begin();
		 //(!list->empty()) && (iter != list->end()); iter++)
		 iter != list->end(); iter++)
	{
		static_cast<MJMesh::FrontElement *>((*iter))->getFrontSTNodes(this).clear();
	}
}

void MJMesh::Front::removeSearchTreeSpecificData() const
{
	this->removeSearchTreeSpecificData(this->boundary);
	this->removeSearchTreeSpecificData(this->elements);
	this->removeSearchTreeSpecificData(this->rejected);

	if (!this->vstree)
    {
        return;
    }

	for (Data::Front::ConstVertexIterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		/*if ((*iter)->getId() == 612)
		{
			std::cout << "MJMesh::Front::removeSearchTreeSpecificData, " << (*iter)->text() << std::endl;
		}*/

		static_cast<MJMesh::Vertex *>((*iter))->getFrontSTNodes(this).clear();
	}
}
