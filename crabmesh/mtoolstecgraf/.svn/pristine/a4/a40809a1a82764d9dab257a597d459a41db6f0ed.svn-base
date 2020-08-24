#include "Data/Boundary.h"

#include "Data/Point.h"
#include "Data/Edge2D.h"
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Triangle3D.h"
#include "Data/Box.h"
#include "Data/Mesh.h"

#if USE_GUI
#include "Data/State.h"
#endif //#if USE_GUI

using namespace Data;

Data::Boundary::Boundary(bool sorted, ULInt id) :
	Data::AbstractMesh(id)
{
	this->setSorted(sorted);

#if USE_GUI
	Real r, g, b;
	Data::getState().getTypeColor(Data::State::ColoredType::BOUNDARY_FRONT_ELEMENT, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Boundary::~Boundary()
{
	while (!this->elements.empty())
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
	}

	//std::cout << "debug deleting boundary" << std::endl;

	//this->verticesMap.clear();
	//this->elementsMap.clear();
}

void Data::Boundary::setVertices(const VertexList &vertices)
{
	this->vertices = vertices;

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

const VertexList &Data::Boundary::getVertices() const
{
	return this->vertices;
}

void Data::Boundary::setElements(const FrontElementList &elements)
{
	this->elements = elements;

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

const FrontElementList &Data::Boundary::getElements() const
{
	return this->elements;
}

void Data::Boundary::setSorted(bool sorted)
{
	this->sorted = sorted;
}

bool Data::Boundary::isSorted() const
{
	return this->sorted;
}

void Data::Boundary::sortBoundary()
{
    this->vertices.sort(    [&](const Data::Vertex * first, const Data::Vertex * second) {   return first->getId() < second->getId();  }    );

    this->elements.sort(    [&](const Data::Element * first, const Data::Element * second) {   return first->getId() < second->getId();  }    );
}

UInt Data::Boundary::dimension() const
{
	//return (this->vertices.empty()) ? 0 : this->vertices.front()->getPoint()->dimension();
	for (Data::Boundary::ConstVertexIterator iter = this->vertices.begin();
         iter != this->vertices.end(); iter++)
    {
        if ((*iter)->getPoint())
        {
            return (*iter)->getPoint()->dimension();
        }
    }

    return 0;
}

void Data::Boundary::add(Vertex *v)
{
	this->vertices.push_back(v);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void Data::Boundary::add(FrontElement *e)
{
	this->elements.push_back(e);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void Data::Boundary::remove(Vertex *v)
{
	this->vertices.remove(v);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void Data::Boundary::remove(FrontElement *e)
{
	this->elements.remove(e);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

Data::Vertex *Data::Boundary::getVertex(ULInt id) const
{
	for (Data::Boundary::ConstVertexIterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		if ((*iter)->getId() == id)
		{
			return (*iter);
		}
	}

	return NULL;

	/*if (it == this->verticesMap.end())
	{
		return NULL;
	}

	return (*it).second;*/
}

Data::FrontElement *Data::Boundary::getElement(ULInt id) const
{
	for (Data::Boundary::ConstFrontElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		if ((*iter)->getId() == id)
		{
			return (*iter);
		}
	}

	return NULL;
}

FrontElementList Data::Boundary::adjacency(const Vertex *v) const
{
	FrontElementList adjacency;

	for (FrontElementList::const_iterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		FrontElement *fel = (*iter);

		if (fel->have(v))
		{
			adjacency.push_back(fel);
		}
	}

	return adjacency;
}

bool Data::Boundary::have(const Vertex *v) const
{
    return this->have(v, false);
}

bool Data::Boundary::have(const Vertex *v, bool matchingCoords) const
{
	{
		//return this->vertices.empty() ? false : std::find(this->vertices.begin(), this->vertices.end(), v) != this->vertices.end();

		if (this->vertices.empty())
		{
			return false;
		}

		for (Data::VertexList::const_iterator iter = this->vertices.begin();
			 iter != this->vertices.end(); iter++)
		{
			if ((*iter) == v)
			{
				return true;
			}

			if ((matchingCoords) && ((*iter)->getPoint()->match(*v->getPoint())))
			{
				return true;
			}
		}

		return false;
	}
}

bool Data::Boundary::have(const FrontElement *e) const
{
    return this->have(e, false);
}

bool Data::Boundary::have(const FrontElement *e, bool matchingCoords) const
{
	{
		//return this->elements.empty() ? false : std::find(this->elements.begin(), this->elements.end(), e) != this->elements.end();

		if (this->elements.empty())
		{
			return false;
		}

		for (Data::FrontElementList::const_iterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			if ((*iter) == e)
			{
				return true;
			}

			if (matchingCoords)
			{
				bool match = true;

				for (UInt i = 0; i < e->numVertices(); i++)
				{
					if (!((*iter)->getVertex(i)->getPoint())->match(*e->getVertex(i)->getPoint()))
					{
						match = false;

						break;
					}
				}

				if (match)
				{
					return true;
				}
			}
		}

		return false;
	}
}

ULInt Data::Boundary::verticesSize() const
{
	return static_cast<ULInt>(this->vertices.size());
}

ULInt Data::Boundary::size() const
{
	return static_cast<ULInt>(this->elements.size());
}

Data::Boundary::VertexIterator Data::Boundary::vBegin()
{
	return this->vertices.begin();
}

Data::Boundary::ConstVertexIterator Data::Boundary::vBegin() const
{
	return this->vertices.begin();
}

Data::Boundary::VertexIterator Data::Boundary::vEnd()
{
	return this->vertices.end();
}

Data::Boundary::ConstVertexIterator Data::Boundary::vEnd() const
{
	return this->vertices.end();
}

Data::Boundary::FrontElementIterator Data::Boundary::eBegin()
{
	return this->elements.begin();
}

Data::Boundary::ConstFrontElementIterator Data::Boundary::eBegin() const
{
	return this->elements.begin();
}

Data::Boundary::FrontElementIterator Data::Boundary::eEnd()
{
	return this->elements.end();
}

Data::Boundary::ConstFrontElementIterator Data::Boundary::eEnd() const
{
	return this->elements.end();
}

Data::Boundary::VertexIterator Data::Boundary::erase(const Data::Boundary::VertexIterator &iter)
{
	return this->vertices.erase(iter);
}

Data::Boundary::FrontElementIterator Data::Boundary::erase(const Data::Boundary::FrontElementIterator &iter)
{
	return this->elements.erase(iter);
}

void Data::Boundary::clear()
{
	this->vertices.clear();
	this->elements.clear();

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

bool Data::Boundary::have(const Element *e, const Mesh *mesh, FrontElementList *fels) const
{
	FrontElementList frontElements = mesh->findAdjacentFrontElements(e);

	if (fels)
	{
		//fels->insert(fels->end(), frontElements.begin(), frontElements.end());
		fels->swap(frontElements);

		return this->have(e, *fels);
	}

	return this->have(e, frontElements);
}

bool Data::Boundary::have(const Element */*e*/, Data::FrontElementList &fels) const
{
	return this->haveAny(fels);
}

bool Data::Boundary::haveAny(const VertexList &vertices, bool matchingCoords) const
{
	/*for (UInt i = 0; i < this->vertices.size(); i++)
	{
		for (VertexList::const_iterator iter = vertices.begin();
			 iter != vertices.end(); iter++)
		{
			if (this->vertices[i] == (*iter))
			{
				return true;
			}
		}
	}*/

	for (VertexList::const_iterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		if (this->have((*iter), matchingCoords))
		{
			return true;
		}
	}

	return false;
}

bool Data::Boundary::haveAny(const FrontElementList &elements, bool matchingCoords) const
{
	/*for (UInt i = 0; i < this->elements.size(); i++)
	{
		for (FrontElementList::const_iterator iter = elements.begin();
			 iter != elements.end(); iter++)
		{
			if (this->elements[i] == (*iter))
			{
				return true;
			}
		}
	}*/

	for (FrontElementList::const_iterator iter = elements.begin();
		 iter != elements.end(); iter++)
	{
		if (this->have((*iter), matchingCoords))
		{
			return true;
		}
	}

	return false;
}

Box *Data::Boundary::box() const
{
	UInt dimension = 0;

	//for (VertexVector::iterator iter = this->vertices.begin();
	for (VertexList::const_iterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		if ((*iter)->getPoint()->dimension() > dimension)
		{
			dimension = (*iter)->getPoint()->dimension();
		}
	}

#if USE_EXPERIMENTAL_3
    Point *min, *max;

    min = (dimension == 2) ?
        (max = new Point2D(), static_cast<Point *>(new Point2D())) :
        (max = new Point3D(), static_cast<Point *>(new Point3D()));
#else
	Point *min = new Point(dimension);
	Point *max = new Point(dimension);
#endif //#if USE_EXPERIMENTAL_3

	for (UInt i = 0; i < dimension; i++)
	{
		min->setCoord(i, REAL_MAX);
		max->setCoord(i, -REAL_MAX);
	}

	//for (Data::VertexVector::iterator iter = this->vertices.begin();
	for (Data::VertexList::const_iterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		for (UInt j = 0; j < dimension; j++)
		{
			Real coord = (*iter)->getPoint()->getCoord(j);

			if (coord < min->getCoord(j))
			{
				min->setCoord(j, coord);
			}

			if (coord > max->getCoord(j))
			{
				max->setCoord(j, coord);
			}
		}
	}

	Box *b = new Box(min, max);

	return b;
}

#if USE_GUI
void Data::Boundary::draw() const
{
	if (!this->hasLists())
	{
		this->createList(3);
	}

	if (this->hasList(0))
	{
		this->drawList(0);

		return;
	}

	this->beginList(0);

	if (Data::getState().isFrontElementDrawn())
	{
	    //Real r, g, b;
	    //Data::getState().getTypeColor(Data::State::ColoredType::BOUNDARY_FRONT_ELEMENT, r, g, b);

		//for (FrontElementVector::const_iterator iter = this->elements.begin();
		for (FrontElementList::const_iterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			/*Real r = (*iter)->getR();
			Real g = (*iter)->getG();
			Real b = (*iter)->getB();

			(*iter)->setColor(this->getR(), this->getG(), this->getB());

			(*iter)->draw();

			(*iter)->setColor(r, g, b);*/

			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->draw(this->getR(), this->getR(), this->getR());
			}
		}
	}

	if (Data::getState().isVertexDrawn())
	{
	    //Real r, g, b;
	    //Data::getState().getTypeColor(Data::State::ColoredType::BOUNDARY_VERTEX, r, g, b);

		//for (VertexVector::const_iterator iter = this->vertices.begin();
		for (VertexList::const_iterator iter = this->vertices.begin();
			 iter != this->vertices.end(); iter++)
		{
			/*Real r = (*iter)->getR();
			Real g = (*iter)->getG();
			Real b = (*iter)->getB();

			(*iter)->setColor(this->getR(), this->getG(), this->getB());

			(*iter)->draw();

			(*iter)->setColor(r, g, b);*/

			//if (Data::getState().isInVisibleSpace((*iter)->getPoint()))
			{
				(*iter)->draw(this->getR(), this->getR(), this->getR());
			}
		}
	}

	this->endList();
}

void Data::Boundary::drawPick() const
{
	if (Data::getState().isFrontElementDrawn())
	{
		//for (FrontElementVector::const_iterator iter = this->elements.begin();
		for (FrontElementList::const_iterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawPick();
			}
		}
	}

	if (Data::getState().isVertexDrawn())
	{
		//for (VertexVector::const_iterator iter = this->vertices.begin();
		for (VertexList::const_iterator iter = this->vertices.begin();
			 iter != this->vertices.end(); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getPoint()))
			{
				(*iter)->drawPick();
			}
		}
	}
}

void Data::Boundary::drawNormals(Real factor) const
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
		//for (FrontElementVector::const_iterator iter = this->elements.begin();
		for (FrontElementList::const_iterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			/*Real r = (*iter)->getR();
			Real g = (*iter)->getG();
			Real b = (*iter)->getB();

			(*iter)->setColor(this->getR(), this->getG(), this->getB());

			(*iter)->drawNormal(factor);

			(*iter)->setColor(r, g, b);*/

			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawNormal(this->getR(), this->getG(), this->getB(), factor);
			}
		}
	}

	this->endList();
}

void Data::Boundary::drawDiscretization() const
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

	if (!this->vertices.empty())
	{
		//UInt dimension = this->vertices[0]->getPoint()->dimension();
		UInt dimension = this->vertices.front()->getPoint()->dimension();

		//for (Data::VertexVector::iterator iter = this->vertices.begin();
		for (Data::VertexList::const_iterator iter = this->vertices.begin();
			 iter != this->vertices.end(); iter++)
		{
			if (!Data::getState().isInVisibleSpace((*iter)->getPoint()))
			{
				continue;
			}

			Data::Vertex *v = (*iter);

			Data::FrontElementList bfels;

			for (Data::FrontElementList::const_iterator iter2 = this->elements.begin();
				 iter2 != this->elements.end(); iter2++)
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

			Real size = 0.0, count = static_cast<Real>(bfels.size());

			while (!bfels.empty())
			{
				Data::FrontElement *e = bfels.front();
				bfels.pop_front();

				//Data::Vector normal = (dimension == 2) ?
				//	static_cast<Data::Vector &>(static_cast<Data::Edge2D *>(e->getShape())->normal()) :
				//	static_cast<Data::Vector &>(static_cast<Data::Triangle3D *>(e->getShape())->normal());

				size += e->getShape()->size();

				//normal.normalize();

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

				e.setColor(this->getR(), this->getG(), this->getB());
				//e.draw(false);
				e.draw();

				e.setPoints(NULL, NULL);
			}
			else if (dimension == 3)
			{
				Point3D p1(vec);
				Point3D p2(s);

				Data::Edge e(&p1, &p2);

				e.setColor(this->getR(), this->getG(), this->getB());
				//e.draw(false);
				e.draw();

				e.setPoints(NULL, NULL);
			}
		}
	}

	this->endList();
}

std::string Data::Boundary::text() const
{
	std::stringstream s;

	//for (VertexVector::const_iterator iter = this->vertices.begin();
	for (VertexList::const_iterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		s << (*iter)->text() << std::endl;
	}

	//for (FrontElementVector::const_iterator iter = this->elements.begin();
	for (FrontElementList::const_iterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		s << (*iter)->text() << std::endl;
	}

	return s.str();
}
#endif //#if USE_GUI
