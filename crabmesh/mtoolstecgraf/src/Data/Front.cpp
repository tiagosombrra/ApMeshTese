#include "Data/Front.h"

#include "Data/Box.h"
#include "Data/FrontElement.h"
#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "Data/Boundary.h"
#include "Data/OStream.h"

#if USE_GUI
#include "Data/State.h"
#endif //#if USE_GUI

using namespace Data;

#if USE_SET
/*Data::AbstractFront::AbstractFront(const AbstractFront &front) :
	Data::AbstractMesh(front.id)
{
    this->sorted = front.sorted;
}*/

Data::AbstractFront::AbstractFront(bool sorted, ULInt id) :
	Data::AbstractMesh(id)
{
	this->sorted = sorted;
}

/*Data::AbstractFront::~AbstractFront()
{

}*/

UShort Data::AbstractFront::numStates() const
{
	return 0;
}

bool Data::AbstractFront::operator()(const FrontElement *e1, const FrontElement *e2) const
{
	//return ((this->sorted) && (e1->getShape()->smaller(e2->getShape())));

	if (!this->sorted)
    {
        return false;
    }

    /*if (e1->getShape()->smaller(e2->getShape()))
    {
        return true;
    }*/

    Real s1 = e1->getShape()->size();
    Real s2 = e2->getShape()->size();
    Real tol = Data::getTolerance();

    if (s1 < s2 - tol)
    {
        return true;
    }

    if (s2 < s1 - tol)
    {
        return false;
    }

    return e1->getId() < e2->getId();
}

#if USE_GUI
void Data::AbstractFront::draw() const
{

}

std::string Data::AbstractFront::text() const
{
	return "";
}
#endif //#if USE_GUI
#endif //#if USE_SET

Data::Front::AllFrontElementIterator::AllFrontElementIterator(Front *front)
{
	this->front = front;
	this->state = 0;
	this->iter = this->front->eBegin(this->state);

	this->ensureValidIncrement();
}

Data::Front::AllFrontElementIterator::AllFrontElementIterator(Front *front, Data::UShort state, FrontElementIterator iter)
{
	this->front = front;
	this->state = state;
	this->iter = iter;

	this->ensureValidIncrement();
}

Data::Front::AllFrontElementIterator &Data::Front::AllFrontElementIterator::operator++()
{
	++this->iter;

	this->ensureValidIncrement();

	return (*this);
}

Data::Front::AllFrontElementIterator Data::Front::AllFrontElementIterator::operator++(int)
{
	Data::Front::AllFrontElementIterator tmp = (*this);

	++(*this);

	return tmp;
}

Data::Front::AllFrontElementIterator &Data::Front::AllFrontElementIterator::operator--()
{
	this->ensureValidDecrement();

	--this->iter;

	return (*this);
}

Data::Front::AllFrontElementIterator Data::Front::AllFrontElementIterator::operator--(int)
{
	Data::Front::AllFrontElementIterator tmp = (*this);

	--(*this);

	return tmp;
}

Data::Front::AllFrontElementIterator &Data::Front::AllFrontElementIterator::operator=(const Data::Front::AllFrontElementIterator &iter)
{
	this->front = iter.front;
	this->state = iter.state;
	this->iter = iter.iter;

	return (*this);
}

bool Data::Front::AllFrontElementIterator::operator==(const Data::Front::AllFrontElementIterator &iter)
{
	return ((this->state == iter.state) && (this->iter == iter.iter));
}

bool Data::Front::AllFrontElementIterator::operator!=(const Data::Front::AllFrontElementIterator &iter)
{
	return ((this->state != iter.state) || (this->iter != iter.iter));
}

FrontElement *Data::Front::AllFrontElementIterator::operator*() const
{
	return (*this->iter);
}

UShort Data::Front::AllFrontElementIterator::getState() const
{
    return this->state;
}

void Data::Front::AllFrontElementIterator::ensureValidIncrement()
{
	UShort numStates = this->front->numStates();

	while ((this->state + 1 < numStates) &&
		   (this->iter == this->front->eEnd(this->state)))
	{
		++this->state;

		this->iter = this->front->eBegin(this->state);
	}
}

void Data::Front::AllFrontElementIterator::ensureValidDecrement()
{
	while ((this->state > 0) &&
		   (this->iter == this->front->eBegin(this->state)))
	{
		--this->state;

		this->iter = this->front->eEnd(this->state);
	}
}

Data::Front::ConstAllFrontElementIterator::ConstAllFrontElementIterator(const Front *front)
{
	this->front = front;
	this->state = 0;
	this->iter = this->front->eBegin(this->state);

	this->ensureValidIncrement();
}

Data::Front::ConstAllFrontElementIterator::ConstAllFrontElementIterator(const Front *front, UShort state, ConstFrontElementIterator iter)
{
	this->front = front;
	this->state = state;
	this->iter = iter;

	this->ensureValidIncrement();
}

Data::Front::ConstAllFrontElementIterator &Data::Front::ConstAllFrontElementIterator::operator++()
{
	++this->iter;

	this->ensureValidIncrement();

	return (*this);
}

Data::Front::ConstAllFrontElementIterator Data::Front::ConstAllFrontElementIterator::operator++(int)
{
	Data::Front::ConstAllFrontElementIterator tmp = (*this);

	++(*this);

	return tmp;
}

Data::Front::ConstAllFrontElementIterator &Data::Front::ConstAllFrontElementIterator::operator--()
{
	this->ensureValidDecrement();

	--this->iter;

	return (*this);
}

Data::Front::ConstAllFrontElementIterator Data::Front::ConstAllFrontElementIterator::operator--(int)
{
	Data::Front::ConstAllFrontElementIterator tmp = (*this);

	--(*this);

	return tmp;
}

Data::Front::ConstAllFrontElementIterator &Data::Front::ConstAllFrontElementIterator::operator=(const Data::Front::ConstAllFrontElementIterator &iter)
{
	this->front = iter.front;
	this->state = iter.state;
	this->iter = iter.iter;

	return (*this);
}

bool Data::Front::ConstAllFrontElementIterator::operator==(const Data::Front::ConstAllFrontElementIterator &iter)
{
	return ((this->state == iter.state) && (this->iter == iter.iter));
}

bool Data::Front::ConstAllFrontElementIterator::operator!=(const Data::Front::ConstAllFrontElementIterator &iter)
{
	return ((this->state != iter.state) || (this->iter != iter.iter));
}

FrontElement *Data::Front::ConstAllFrontElementIterator::operator*() const
{
	return (*this->iter);
}

void Data::Front::ConstAllFrontElementIterator::ensureValidIncrement()
{
	UShort numStates = this->front->numStates();

	while ((this->state + 1 < numStates) &&
		   (this->iter == this->front->eEnd(this->state)))
	{
		++this->state;

		this->iter = this->front->eBegin(this->state);
	}
}

void Data::Front::ConstAllFrontElementIterator::ensureValidDecrement()
{
	while ((this->state > 0) &&
		   (this->iter == this->front->eBegin(this->state)))
	{
		--this->state;

		this->iter = this->front->eEnd(this->state);
	}
}

Data::Front::Front(const Boundary *boundary, ULInt id) :
#if USE_SET
	Data::AbstractFront(boundary->isSorted(), id)
#else
	Data::AbstractMesh(id)
#endif //#if USE_SET
{
#if (!USE_SET)
	//this->setSorted(boundary->isSorted());
	this->sorted = boundary->isSorted();
#endif //#if (!USE_SET)

#if USE_SET
	this->elements = this->makeSet();
#endif //#if USE_SET

	//VertexVector vertices = boundary->getVertices();

	//FrontElementVector elements = boundary->getElements();

	this->vertices = boundary->getVertices();

/*#if USE_SET
	VertexList vertices = boundary->getVertices();

	this->vertices.insert(vertices.begin(), vertices.end());
#else
	//this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());
	this->vertices = boundary->getVertices();
#endif //#if USE_SET*/

#if USE_SET
	FrontElementList elements = boundary->getElements();

	this->elements->insert(elements.begin(), elements.end());
#else
	//this->elements.insert(this->elements.end(), elements.begin(), elements.end());
	this->elements = boundary->getElements();
#endif //#if USE_SET

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::FRONT_ELEMENT_FRONT_ELEMENT, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Front::Front(bool sorted, ULInt id) :
#if USE_SET
	Data::AbstractFront(sorted, id)
#else
	Data::AbstractMesh(id)
#endif //#if USE_SET
{
#if (!USE_SET)
	//this->setSorted(sorted);
	this->sorted = sorted;
#endif //#if (!USE_SET)

#if USE_SET
	this->elements = NULL;
#endif //#if USE_SET

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::FRONT_ELEMENT_FRONT_ELEMENT, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Front::~Front()
{
#if USE_SET
	if (this->elements)
	{
		delete this->elements;
	}
#endif //#if USE_SET

	/*while (!this->elements.empty())
	{
		FrontElement *e = this->elements.back();
		this->elements.pop_back();

		if (e)
		{
			delete e;
		}
	}

	while (!this->vertices.empty())
	{
		Vertex *v = this->vertices.back();
		this->vertices.pop_back();

		if (v)
		{
			delete v;
		}
	}*/
}

void Data::Front::set(const Data::Front *front)
{
	this->sorted = front->sorted;

	this->setVertices(front->getVertices());

#if USE_SET
	//this->setVertices(front->getVerticesSet());
	this->setElements(front->getElementsSet());
#else
	//this->setVertices(front->getVertices());
	this->setElements(front->getElements());
#endif //#if USE_SET
}

void Data::Front::setVertices(const VertexList &vertices)
{
	this->vertices = vertices;

/*#if USE_SET
	this->vertices.clear();

	this->vertices.insert(vertices.begin(), vertices.end());
#else
	this->vertices = vertices;
#endif //#if USE_SET*/

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

const VertexList &Data::Front::getVertices() const
/*#if USE_SET
VertexList Data::Front::getVertices() const
#else
const VertexList &Data::Front::getVertices() const
#endif //#if USE_SET*/
{
	return this->vertices;

/*#if USE_SET
	VertexList vertices(this->vertices.begin(), this->vertices.end());

	return vertices;
#else
	return this->vertices;
#endif //#if USE_SET*/
}

Vertex *Data::Front::getVertex(ULInt id) const
{
	for (ConstVertexIterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		if ((*iter)->getId() == id)
		{
			return (*iter);
		}
	}

	return NULL;
}

void Data::Front::setElements(const FrontElementList &elements)
{
#if USE_SET
	if (!this->elements)
	{
		this->elements = this->makeSet();
	}
	else
	{
		this->elements->clear();
	}

	for (FrontElementList::const_iterator iter = elements.begin();
		 iter != elements.end(); iter++)
	{
		this->elements->insert((*iter));
	}
#else
	this->elements = elements;
#endif //#if USE_SET

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

#if USE_SET
FrontElementList Data::Front::getElements() const
#else
const FrontElementList &Data::Front::getElements() const
#endif //#if USE_SET
{
#if USE_SET
	if (!this->elements)
	{
		return FrontElementList();
	}

	FrontElementList aux(this->elements->begin(), this->elements->end());

	return aux;
#else
	return this->elements;
#endif //#if USE_SET
}

FrontElementList Data::Front::getAllElements() const
{
	return this->getElements();
}

FrontElement *Data::Front::getElement(ULInt id) const
{
	return this->find(this->elements, id);
}

void Data::Front::setSorted(bool sorted)
{
#if USE_SET
	bool changed = (this->sorted != sorted);
#endif //#if USE_SET

	this->sorted = sorted;

#if USE_SET
	if (changed)
	{
		if (this->elements)
		{
			Data::FrontElementMultiSet *elements = this->elements;

			this->elements = this->makeSet();

			for (FrontElementIterator iter = elements->begin();
				 iter != elements->end(); iter++)
			{
				this->elements->insert((*iter));
			}

			elements->clear();

			delete elements;
		}

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI
	}
#endif //#if USE_SET
}

bool Data::Front::isSorted() const
{
	return this->sorted;
}

UInt Data::Front::dimension() const
{
	return (this->vertices.empty()) ? 0 : this->vertices.front()->getPoint()->dimension();

/*#if USE_SET
	return (this->vertices.empty()) ? 0 : (*this->vertices.begin())->getPoint()->dimension();
#else
	return (this->vertices.empty()) ? 0 : this->vertices.front()->getPoint()->dimension();
#endif //#if USE_SET*/
}

void Data::Front::clear()
{
	this->vertices.clear();

#if USE_SET
	if (this->elements)
	{
		this->elements->clear();
	}
#else
	this->elements.clear();
#endif //#if USE_SET

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void Data::Front::add(Vertex *v)
{
	this->vertices.push_back(v);

/*#if USE_SET
	this->vertices.insert(v);
#else
	this->vertices.push_back(v);
#endif //#if USE_SET*/

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void Data::Front::add(FrontElement *e)
{
#if USE_SET
	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	this->elements->insert(e);
#else
	this->elements.push_back(e);
#endif //#if USE_SET

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

bool Data::Front::remove(const Vertex *v)
{
	VertexIterator it = std::find(this->vertices.begin(), this->vertices.end(), v);

/*#if USE_SET
	VertexIterator it = this->vertices.find(const_cast<Vertex *>(v));
#else
	VertexIterator it = std::find(this->vertices.begin(), this->vertices.end(), v);
#endif //#if USE_SET*/

	if (it != this->vertices.end())
	{
		this->vertices.erase(it);

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI

		return true;
	}

	return false;
}

bool Data::Front::remove(const FrontElement *e)
{
#if USE_SET
	if (this->elements->empty())
	{
		return false;
	}

	//FrontElementIterator it = std::find(this->elements->begin(), this->elements->end(), e);
	std::pair<FrontElementIterator, FrontElementIterator>
		pair = this->elements->equal_range(const_cast<Data::FrontElement *>(e));
	ConstFrontElementIterator it = std::find(pair.first, pair.second, e);

	//if (it != this->elements->end())
	if (it != pair.second)
	{
		this->elements->erase(it);

#else
	if (this->elements.empty())
	{
		return false;
	}

	FrontElementList::iterator it = std::find(this->elements.begin(), this->elements.end(), e);

	if (it != this->elements.end())
	{
		this->elements.erase(it);

#endif //#if USE_SET

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI

		return true;
	}

	return false;
}

bool Data::Front::isEmpty() const
{
#if USE_SET
	/*if (!this->elements)
	{
		this->elements = this->makeSet();
	}*/

	return this->elements->empty();
#else
	return this->elements.empty();
#endif //#if USE_SET
}

ULInt Data::Front::size() const
{
#if USE_SET
	return this->elements ? static_cast<ULInt>(this->elements->size()) : 0;
#else
	return this->elements.size();
#endif //#if USE_SET
}

ULInt Data::Front::size(UShort /*state*/) const
{
	return this->size();
}

ULInt Data::Front::verticesSize() const
{
	return static_cast<ULInt>(this->vertices.size());
}

FrontElement *Data::Front::next() const
{
#if USE_SET
	return *this->elements->begin();
#else
	return this->elements.front();
#endif //#if USE_SET
}

bool Data::Front::reject(FrontElement *e, UInt /*code*/)
{
	this->remove(e);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI

	return true;
}

bool Data::Front::shouldRemove(const Vertex *v) const
{
	return !Front::have(this->elements, v);
}

bool Data::Front::shouldRemove(const FrontElement */*e*/) const
{
	return true;
}

Element *Data::Front::advance(FrontElement *e, Vertex *v, FrontElementList &newFront)
{
	FrontElementList oldFront;

	return this->advance(e, v, newFront, oldFront);
}

bool Data::Front::have(const Vertex *v) const
{
	return std::find(this->vertices.begin(), this->vertices.end(), v) != this->vertices.end();

/*#if USE_SET
	return this->vertices.find(const_cast<Vertex *>(v)) != this->vertices.end();
#else
	return std::find(this->vertices.begin(), this->vertices.end(), v) != this->vertices.end();
#endif //#if USE_SET*/
}

bool Data::Front::have(const FrontElement *e) const
{
	return Front::have(this->elements, e);
}

bool Data::Front::haveAny(const FrontElementList &elements) const
{
	return Front::haveAny(this->elements, elements);
}

FrontElement *Data::Front::find(const VertexVector &vertices, const Vertex *other) const
{
	VertexList others;

	if (other)
	{
		others.push_back(const_cast<Vertex *>(other));
	}

	return this->find(vertices, others);
}

FrontElement *Data::Front::find(const VertexVector &vertices, const VertexList &others) const
{
	return this->find(this->elements, vertices, others);
}

Data::Front::VertexIterator Data::Front::vBegin()
{
	return this->vertices.begin();
}

Data::Front::ConstVertexIterator Data::Front::vBegin() const
{
	return this->vertices.begin();
}

Data::Front::VertexIterator Data::Front::vEnd()
{
	return this->vertices.end();
}

Data::Front::ConstVertexIterator Data::Front::vEnd() const
{
	return this->vertices.end();
}

Data::Front::FrontElementIterator Data::Front::eBegin(UShort /*state*/)
{
#if USE_SET
	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	return this->elements->begin();
#else
	return elements.begin();
#endif //#if USE_SET
}

Data::Front::ConstFrontElementIterator Data::Front::eBegin(UShort /*state*/) const
{
#if USE_SET
	/*if (!this->elements)
	{
		this->elements = this->makeSet();
	}*/

	return this->elements->begin();
#else
	return elements.begin();
#endif //#if USE_SET
}

Data::Front::FrontElementIterator Data::Front::eEnd(UShort /*state*/)
{
#if USE_SET
	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	return this->elements->end();
#else
	return elements.end();
#endif //#if USE_SET
}

Data::Front::ConstFrontElementIterator Data::Front::eEnd(UShort /*state*/) const
{
#if USE_SET
	/*if (!this->elements)
	{
		this->elements = this->makeSet();
	}*/

	return this->elements->end();
#else
	return elements.end();
#endif //#if USE_SET
}

Data::Front::AllFrontElementIterator Data::Front::allEBegin()
{
	return AllFrontElementIterator(this);
}

Data::Front::ConstAllFrontElementIterator Data::Front::allEBegin() const
{
	return ConstAllFrontElementIterator(const_cast<Data::Front *>(this));
}

Data::Front::AllFrontElementIterator Data::Front::allEEnd()
{
	return AllFrontElementIterator(this, static_cast<UShort>(this->numStates() - 1), this->eEnd(static_cast<UShort>(this->numStates() - 1)));
}

Data::Front::ConstAllFrontElementIterator Data::Front::allEEnd() const
{
	return ConstAllFrontElementIterator(const_cast<Data::Front *>(this), static_cast<UShort>(this->numStates() - 1), this->eEnd(static_cast<UShort>(this->numStates() - 1)));
}

Data::Front::VertexIterator Data::Front::erase(Data::Front::VertexIterator &iter)
{
/*#if USE_SET
	//Data::Front::VertexIterator iter2 = iter;
	//iter2++;

	//this->vertices.erase(iter);
	this->vertices.erase(iter++);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI

	//return iter2;
	return iter;
#else*/
#if USE_GUI
	Data::Front::VertexIterator iter2 = this->vertices.erase(iter);

	this->outdate();

	return iter2;
#else
	return this->vertices.erase(iter);
#endif //#if USE_GUI
//#endif //#if USE_SET
}

Data::Front::FrontElementIterator Data::Front::erase(Data::Front::FrontElementIterator &iter, UShort /*state*/)
{
#if USE_SET
	//Data::Front::FrontElementIterator iter2 = iter;
	//iter2++;

	//this->elements->erase(iter);
	this->elements->erase(iter++);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI

	//return iter2;
	return iter;
#else
#if USE_GUI
	Data::Front::FrontElementIterator iter2 = this->elements.erase(iter);

	this->outdate();

	return iter2;
#else
	return this->elements.erase(iter);
#endif //#if USE_GUI
#endif //#if USE_SET
}

Data::Front::AllFrontElementIterator Data::Front::erase(Data::Front::AllFrontElementIterator &iter)
{
	return Data::Front::AllFrontElementIterator(this, iter.state, this->erase(iter.iter, iter.state));
}

/*#if USE_SET
bool Data::Front::operator()(const FrontElement *e1, const FrontElement *e2) const
{*/
	/*if ((this->isSorted()) && (e1->getShape()->size() < e2->getShape()->size()))
	//if ((this->isSorted()) && (e1->getShape()->size() + Data::getTolerance() < e2->getShape()->size()))
	{
		return true;
	}

	return false;*/

	/*if (e1->getTimestamp() < e2->getTimestamp())
	{
		return true;
	}

	return e1 < e2;*/

	/*std::cout << "bb" << std::endl;

	return this->better(e1, e2);

	return this->isSorted() ?
		(e1->getShape()->size() > e2->getShape()->size()) :
		//workaround
		//(e1->getId() > e2->getId());
		(e1 > e2);

	//When the front is not sorted, than the front should be like a normal
	// queue, i.e., first in, first out. Thus, to achieve this goal, the
	// ids could be used, since a front element that was created before should
	// have a smaller id. However, when a front element is created and inserted
	// in the heap, its id is 0, and is only set when it is inserted in the
	// mesh, invalidating the heap internal structure. Therefore, as a
	// workaround, the position in memory is compared instead of the id.
	// Hopefully, a front element created before will be allocated in a smaller
	// memory position.*/
/*}
#endif //#if USE_SET*/

Element *Data::Front::advance(FrontElement *e, Vertex *v, FrontElementList &newFront,
	FrontElementList &oldFront)
{
	/*if (Data::getState().isDebug())
	{
		std::cout << "debug front::advance front element = " << e->text() << ", " << e << std::endl;
		std::cout << "debug front::advance vertex = " << v->text() << ", " << v << std::endl;
	}*/

	if (!this->have(v))
	{
		/*if (Data::getState().isDebug())
        {
            std::cout << "debug front::advance adding vertex " << v->text() << " to front" << std::endl;
        }*/

		this->add(v);
	}

	VertexSet vertices;

	Element *newe = e->advance(v, this, newFront, oldFront);

	if (this->shouldRemove(e))
	{
		/*if (Data::getState().isDebug())
        {
            std::cout << "debug front::advance removing front element " << e->text() << " from front" << std::endl;
        }*/

		this->remove(e);
	}

	//VertexVector vs = e->getVertices();
	Vertex **vs = const_cast<Vertex **>(e->getVerticesArray());

	vertices.insert(vs, vs + e->numVertices());

	for (FrontElementList::iterator iter = oldFront.begin();
		 iter != oldFront.end(); iter++)
	{
		FrontElement *el = (*iter);

		/*{
			std::cout << "debug front::advance old front element = " << el->text() << ", " << el << std::endl;
		}*/

		if (!this->shouldRemove(el))
		{
			continue;
		}

		/*if (Data::getState().isDebug())
        {
            std::cout << "debug front::advance removing old front element " << el->text() << " from front" << std::endl;
        }*/

		this->remove(el);

		//vs = el->getVertices();
		vs = const_cast<Vertex **>(el->getVerticesArray());

		//vertices.insert(vs.begin(), vs.end());
		vertices.insert(vs, vs + e->numVertices());
	}

	//vs.clear();

	//cout << "debug front::advance newfront = " << newFront.size() << endl;

	for (FrontElementList::iterator iter = newFront.begin();
		 iter != newFront.end(); iter++)
	{
		/*if (Data::getState().isDebug())
        {
            std::cout << "debug front::advance adding new front element " << (*iter)->text() << " to front" << std::endl;
        }*/

		this->add((*iter));
	}

	//cout << "debug front::advance trying to remove v = " << v->text() << endl;

	if (this->shouldRemove(v))
	{
		//cout << "debug front::advance will now remove v = " << v->text() << endl;

		/*if (Data::getState().isDebug())
        {
            std::cout << "debug front::advance removing vertex " << v->text() << " from front" << std::endl;
        }*/

		this->remove(v);
	}

	//while (!vertices.empty())
	for (VertexSet::iterator iter = vertices.begin();
		 iter != vertices.end(); ++iter)
	{
		//Vertex *ve = *vertices.begin();
		Vertex *ve = (*iter);
		//vertices.erase(vertices.begin());

		//cout << "debug front::advance trying to remove ve = " << ve->text() << endl;

		if ((ve != v) && (this->shouldRemove(ve)))
		{
			//cout << "debug front::advance will now remove ve = " << ve->text() << endl;

			/*if (Data::getState().isDebug())
            {
                std::cout << "debug front::advance removing old vertex " << ve->text() << " from front" << std::endl;
            }*/

			this->remove(ve);
		}
	}

	/*if (Data::getState().isDebug())
    {
        std::cout << "debug front::advance returning element " << newe->text() << std::endl;
    }*/

	return newe;
}

bool Data::Front::intercept(const Element *e, const VertexList &list) const
{
	for (VertexList::const_iterator iter = list.begin();
		 iter != list.end(); iter++)
	{
		//1st test -> test if the element has the pointer
		//2nd test -> test if the element has the coordinates
		if ((e->have((*iter))) || (e->have(*(*iter))))
		{
			continue;
		}

		if (!e->out((*iter)))
		{
			return true;
		}
	}

	return false;
}

bool Data::Front::intercept(const Element *e) const
{
	return this->intercept(e, this->verticesInRange(e->getShape()->box()));
}

bool Data::Front::intercept(const FrontElement *e, const FrontElementList &list) const
{
	for (FrontElementList::const_iterator iter = list.begin();
		 iter != list.end(); iter++)
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

bool Data::Front::intercept(const FrontElement *e) const
{
	//return Front::intercept(this->elements, e);

	return this->intercept(e, this->frontElementsInRange(e->getShape()->box()));
}

bool Data::Front::intercept(const FrontElement *e, const Point *p, const FrontElementList &list) const
{
	for (FrontElementList::const_iterator iter = list.begin();
		 iter != list.end(); iter++)
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

bool Data::Front::intercept(const FrontElement *e, const Point *p) const
{
	//return Front::intercept(this->elements, e, p);

	Box b(*p, *p);

	//Real s = 0.5*e->getShape()->size();
	Real s = e->getShape()->size();

	for (UInt i = 0; i < b.dimension(); i++)
	{
		b.getMin()->setCoord(i, b.getMin()->getCoord(i) - s);
		b.getMax()->setCoord(i, b.getMax()->getCoord(i) + s);
	}

	return this->intercept(e, p, this->frontElementsInRange(b));
}

VertexList Data::Front::verticesInRange(const PointList &p, Real radius, bool useBox) const
{
	if (useBox)
	{
		VertexList inbox;

		return this->verticesInRange(p, radius, inbox);
	}

	VertexList vertices;

	for (ConstVertexIterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		for (PointList::const_iterator iter2 = p.begin();
			 iter2 != p.end(); iter2++)
		{
			//sstd::cout << "debug, distance = " << p[i]->distance((*iter)->getPoint()) << ", radius = " << radius << std::endl;

			if ((*iter2)->distance((*iter)->getPoint()) <= radius)
			{
				vertices.push_back((*iter));

				break;
			}
		}
	}

	return vertices;

	/*VertexList inbox;
	const VertexList *range;

	if (useBox)
	{
		UInt dimension = 0;

		for (PointList::const_iterator iter = p.begin();
			 iter != p.end(); iter++)
		{
			if ((*iter)->dimension() > dimension)
			{
				dimension = (*iter)->dimension();
			}
		}

		Point pmin(dimension);
		Point pmax(dimension);

		for (UInt j = 0; j < dimension; j++)
		{
			pmin.setCoord(j, p.front()->getCoord(j));
			pmax.setCoord(j, p.front()->getCoord(j));
		}

		for (PointList::const_iterator iter = ++p.begin();
			 iter != p.end(); iter++)
		{
			for (UInt j = 0; j < dimension; j++)
			{
				Real coord = (*iter)->getCoord(j);

				if (coord < pmin.getCoord(j))
				{
					pmin.setCoord(j, coord);
				}

				if (coord > pmax.getCoord(j))
				{
					pmax.setCoord(j, coord);
				}
			}
		}

		for (UInt j = 0; j < dimension; j++)
		{
			pmin.setCoord(j, pmin.getCoord(j) - radius);
			pmax.setCoord(j, pmax.getCoord(j) + radius);
		}

		Box box(pmin, pmax);

		inbox = this->verticesInRange(box);

		range = &inbox;

#if USE_SET
		VertexList vertices;

		for (VertexList::const_iterator iter = range->begin();
			 iter != range->end(); iter++)
		{
			for (PointList::const_iterator iter2 = p.begin();
				 iter2 != p.end(); iter2++)
			{
				//sstd::cout << "debug, distance = " << p[i]->distance((*iter)->getPoint()) << ", radius = " << radius << std::endl;

				if ((*iter2)->distance((*iter)->getPoint()) <= radius)
				{
					vertices.push_back((*iter));

					break;
				}
			}
		}

		return vertices;
#endif //#if USE_SET
	}
	else
	{
#if USE_SET
		VertexList vertices;

		for (ConstVertexIterator iter = this->vertices.begin();
			 iter != this->vertices.end(); iter++)
		{
			for (PointList::const_iterator iter2 = p.begin();
				 iter2 != p.end(); iter2++)
			{
				if ((*iter2)->distance((*iter)->getPoint()) <= radius)
				{
					vertices.push_back((*iter));

					break;
				}
			}
		}

		return vertices;
#else
		range = &(this->vertices);
#endif //#if USE_SET
	}

#if (!USE_SET)
	VertexList vertices;

	for (VertexList::const_iterator iter = range->begin();
		 iter != range->end(); iter++)
	{
		for (PointList::const_iterator iter2 = p.begin();
			 iter2 != p.end(); iter2++)
		{
			if ((*iter2)->distance((*iter)->getPoint()) <= radius)
			{
				vertices.push_back((*iter));

				break;
			}
		}
	}

	return vertices;
#endif //#if (!USE_SET)*/
}

VertexList Data::Front::verticesInRange(const PointList &p, Real radius, Data::VertexList &inbox) const
{
#if USE_EXPERIMENTAL_3
    Point2D min2d, max2d;
    Point3D min3d, max3d;

    UInt dimension = p.front()->dimension();

    Point &pmin = (dimension == 2) ? static_cast<Point &>(min2d) : static_cast<Point &>(min3d);
    Point &pmax = (dimension == 2) ? static_cast<Point &>(max2d) : static_cast<Point &>(max3d);
#else
	UInt dimension = 0;

	for (PointList::const_iterator iter = p.begin();
		 iter != p.end(); iter++)
	{
		if ((*iter)->dimension() > dimension)
		{
			dimension = (*iter)->dimension();
		}
	}

	Point pmin(dimension);
	Point pmax(dimension);
#endif //#if USE_EXPERIMENTAL_3

	for (UInt j = 0; j < dimension; j++)
	{
		pmin.setCoord(j, p.front()->getCoord(j));
		pmax.setCoord(j, p.front()->getCoord(j));
	}

	for (PointList::const_iterator iter = ++p.begin();
		 iter != p.end(); iter++)
	{
		for (UInt j = 0; j < dimension; j++)
		{
			Real coord = (*iter)->getCoord(j);

			if (coord < pmin.getCoord(j))
			{
				pmin.setCoord(j, coord);
			}

			if (coord > pmax.getCoord(j))
			{
				pmax.setCoord(j, coord);
			}
		}
	}

	for (UInt j = 0; j < dimension; j++)
	{
		pmin.setCoord(j, pmin.getCoord(j) - radius);
		pmax.setCoord(j, pmax.getCoord(j) + radius);
	}

	Box box(pmin, pmax);

	inbox = this->verticesInRange(box);

	VertexList vertices;

	for (VertexList::const_iterator iter = inbox.begin();
		 iter != inbox.end(); iter++)
	{
		for (PointList::const_iterator iter2 = p.begin();
			 iter2 != p.end(); iter2++)
		{
			if ((*iter2)->distance((*iter)->getPoint()) <= radius)
			{
				vertices.push_back((*iter));

				break;
			}
		}
	}

	return vertices;
}

VertexList Data::Front::verticesInRange(const Box &box) const
{
	VertexList vertices;

	for (ConstVertexIterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		if (!box.out((*iter)->getPoint()))
		{
			vertices.push_back((*iter));
		}
	}

	return vertices;
}

FrontElementList Data::Front::frontElementsInRange(const Box &box) const
{
	FrontElementList fels;

	for (ConstFrontElementIterator iter = this->elements->begin();
		 iter != this->elements->end(); iter++)
	{
		if ((*iter)->getShape()->box().intercept(box))
		{
			fels.push_back((*iter));
		}
	}

	return fels;
}

#if USE_GUI
void Data::Front::draw() const
{
	if (!this->hasLists())
	{
		this->createList(3);
	}

	if (this->hasList())
	{
		this->drawList();

		return;
	}

	this->beginList();

	if (Data::getState().isFrontElementDrawn())
	{
	    Real r, g, b;
        Data::getState().getTypeColor(Data::State::ColoredType::FRONT_ELEMENT_FRONT_ELEMENT, r, g, b);

		for (ConstFrontElementIterator iter = this->elements->begin();
			 (!this->elements->empty()) && (iter != this->elements->end()); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				//(*iter)->draw(this->getR(), this->getG(), this->getB());
				(*iter)->draw(r, g, b);
			}
		}
	}

	if (Data::getState().isVertexDrawn())
	{
	    Real r, g, b;
        Data::getState().getTypeColor(Data::State::ColoredType::FRONT_VERTEX, r, g, b);

		for (ConstVertexIterator iter = this->vertices.begin();
			 (!this->vertices.empty()) && (iter != this->vertices.end()); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getPoint()))
			{
				//(*iter)->draw(this->getR(), this->getG(), this->getB());
				(*iter)->draw(r, g, b);
			}
		}
	}

	this->endList();
}

void Data::Front::drawPick() const
{
	if (Data::getState().isFrontElementDrawn())
	{
		for (ConstFrontElementIterator iter = this->elements->begin();
			 (!this->elements->empty()) && (iter != this->elements->end()); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawPick();
			}
		}
	}

	if (Data::getState().isVertexDrawn())
	{
		for (ConstVertexIterator iter = this->vertices.begin();
			 (!this->vertices.empty()) && (iter != this->vertices.end()); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getPoint()))
			{
				(*iter)->drawPick();
			}
		}
	}
}

void Data::Front::drawNormals(Real factor) const
{
	if (!this->hasLists())
	{
		this->createList(3);
	}

	if (this->hasList(1))
	{
		this->drawList(1);

		return;
	}

	this->beginList(1);

	if (Data::getState().isFrontElementDrawn())
	{
	    Real r, g, b;
        Data::getState().getTypeColor(Data::State::ColoredType::FRONT_ELEMENT_FRONT_ELEMENT, r, g, b);

		for (ConstFrontElementIterator iter = this->elements->begin();
			 (!this->elements->empty()) && (iter != this->elements->end()); iter++)
		{
			/*FrontElement *fel = (*iter);

			Real r = fel->getR();
			Real g = fel->getG();
			Real b = fel->getB();

			fel->setColor(this->getR(), this->getG(), this->getB());

			fel->drawNormal(factor);

			fel->setColor(r, g, b);*/
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				//(*iter)->drawNormal(this->getR(), this->getG(), this->getB(), factor);
				(*iter)->drawNormal(r, g, b, factor);
			}
		}
	}

	this->endList();
}

void Data::Front::drawDiscretization() const
{
	if (!this->hasLists())
	{
		this->createList(3);
	}

	if (this->hasList(2))
	{
		this->drawList(2);

		return;
	}

	this->beginList(2);

	if (Data::getState().isVertexDrawn())
	{
        Real r, g, b;
        Data::getState().getTypeColor(Data::State::ColoredType::FRONT_VERTEX, r, g, b);

		if (!this->vertices.empty())
		{
			UInt dimension = (*this->vertices.begin())->getPoint()->dimension();

			for (ConstVertexIterator iter = this->vertices.begin();
				 iter != this->vertices.end(); iter++)
			{
				if (!Data::getState().isInVisibleSpace((*iter)->getPoint()))
				{
					continue;
				}

				Data::Vertex *v = (*iter);

				Data::FrontElementList bfels;

#if USE_SET
				for (Data::FrontElementSet::const_iterator iter2 = this->elements->begin();
					 iter2 != this->elements->end(); iter2++)
#else
				for (Data::FrontElementList::const_iterator iter2 = this->elements.begin();
					 iter2 != this->elements.end(); iter2++)
#endif //#if USE_SET
				{
					if ((*iter2)->have(v))
					{
						bfels.push_back((*iter2));
					}
				}

				if (bfels.empty())
				{
					continue;
				}

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
					Data::FrontElement *e = bfels.front();
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
				VECTORv(s, dimension, vec)

				s.multiply(-1.0);

				vec.sum(v->getPoint());
				s.sum(v->getPoint());

				if (dimension == 2)
				{
					Point2D p1(vec);
					Point2D p2(s);

					Data::Edge2D e(&p1, &p2);

					//e.setColor(this->getR(), this->getG(), this->getB());
					e.setColor(r, g, b);
					//e.draw(false);
					e.draw();

					e.setPoints(NULL, NULL);
				}
				else if (dimension == 3)
				{
					Point3D p1(vec);
					Point3D p2(s);

					Data::Edge e(&p1, &p2);

					//e.setColor(this->getR(), this->getG(), this->getB());
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

std::string Data::Front::text() const
{
	std::stringstream str;

	{
		for (ConstFrontElementIterator iter = this->elements->begin();
			 iter != this->elements->end(); iter++)
		{
			str << " " << (*iter)->getId();
		}
	}

	str << std::endl;

	return str.str();
}
#endif //#if USE_GUI

#if USE_SET
/*void Data::Front::setVertices(const VertexSet &vertices)
{
	this->vertices = vertices;

#if USE_GUI
	this->outdate();
#endif//#if USE_GUI
}

const VertexSet &Data::Front::getVerticesSet() const
{
	return this->vertices;
}*/

void Data::Front::setElements(const FrontElementMultiSet &elements)
{
	if (!this->elements)
	{
		this->elements = this->makeSet();
	}

	(*this->elements) = elements;

#if USE_GUI
	this->outdate();
#endif//#if USE_GUI
}

const FrontElementMultiSet &Data::Front::getElementsSet() const
{
	return *this->elements;
}

UShort Data::Front::numStates() const
{
	return 1;
}

FrontElementMultiSet *Data::Front::makeSet() const
{
	return new FrontElementMultiSet(AbstractFront(this->isSorted(), this->getId()));
}
#endif //#if USE_SET

bool Data::Front::accordingToNormal(const Data::FrontElement *fel, const VertexList &others) const
{
    for (VertexList::const_iterator iter = others.begin();
         iter != others.end(); iter++)
    {
        if (!fel->accordingToNormal(*iter))
        {
            return false;
        }
    }

    return true;
}

#if USE_SET
bool Data::Front::have(const FrontElementMultiSet *list, const Vertex *v) const
#else
bool Data::Front::have(const FrontElementList &list, const Vertex *v) const
#endif //#if USE_SET
{
	for (ConstFrontElementIterator iter = list->begin();
		 iter != list->end(); iter++)
	{
		if ((*iter)->have(v))
		{
			return true;
		}
	}

	return false;
}

#if USE_SET
bool Data::Front::have(const FrontElementMultiSet *list, const FrontElement *e) const
#else
bool Data::Front::have(const FrontElementList &list, const FrontElement *e) const
#endif //#if USE_SET
{
#if USE_SET
	//return (list->empty() ? false : (std::find(list->begin(), list->end(), e) != list->end()));
	std::pair<FrontElementIterator, FrontElementIterator>
			pair = list->equal_range(const_cast<Data::FrontElement *>(e));
	return std::find(pair.first, pair.second, e) != pair.second;
#else
	return (list.empty() ? false : (std::find(list.begin(), list.end(), e) != list.end()));
#endif //#if USE_SET
}

#if USE_SET
bool Data::Front::haveAny(const FrontElementMultiSet *list, const FrontElementList &elements) const
#else
bool Data::Front::haveAny(const FrontElementList &list, const FrontElementList &elements) const
#endif //#if USE_SET
{
	/*for (FrontElementList::const_iterator iter = elements.begin();
		 iter != elements.end(); iter++)
	{
		if (Front::have(list, (*iter)))
		{
			return true;
		}
	}

	return false;*/

	//I think the below code is better because the list is usualy much
	//  greater than the elements list
	for (ConstFrontElementIterator iter = list->begin();
		 iter != list->end(); iter++)
	{
		for (FrontElementList::const_iterator iter2 = elements.begin();
			 iter2 != elements.end(); iter2++)
		{
			if ((*iter) == (*iter2))
			{
				return true;
			}
		}
	}

	return false;
}

#if USE_SET
FrontElement *Data::Front::find(const FrontElementMultiSet *list, const VertexVector &vertices, const VertexList &others) const
#else
FrontElement *Data::Front::find(const FrontElementList &list, const VertexVector &vertices, const VertexList &others) const
#endif //#if USE_SET
{
	Data::FrontElementList geometricals;
	Data::FrontElementList matches;

#if USE_SET
	for (FrontElementMultiSet::const_iterator iter = list->begin();
		 iter != list->end(); iter++)
#else
	for (FrontElementList::const_iterator iter = list.begin();
		 iter != list.end(); iter++)
#endif //#if USE_SET
	{
		if ((*iter)->match(vertices))
		{
            matches.push_back((*iter));

            continue;
		}

		if ((*iter)->geometricallyMatch(vertices))
		{
			geometricals.push_back((*iter));
		}
	}

	if (matches.size() == 1)
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
    }

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

#if USE_SET
FrontElement *Data::Front::find(const FrontElementMultiSet *list, ULInt id) const
#else
FrontElement *Data::Front::find(const FrontElementList &list, ULInt id) const
#endif //#if USE_SET
{
	for (ConstFrontElementIterator iter = list->begin();
		 iter != list->end(); iter++)
	{
		if ((*iter)->getId() == id)
		{
			return (*iter);
		}
	}

	return NULL;
}

/*bool Data::Front::intercept(const FrontElementList &list, const FrontElement *e)
{
	for (FrontElementList::const_iterator iter = list.begin();
		 iter != list.end(); iter++)
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
}*/

/*bool Data::Front::intercept(const FrontElementList &list, const FrontElement *e, const Point *p)
{
	for (FrontElementList::const_iterator iter = list.begin();
		 iter != list.end(); iter++)
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
}*/
