#include "Data/Mesh.h"

#include "Data/State.h"
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/GeometricShape.h"
#include "Data/Edge2D.h"
#include "Data/Polygon.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Element.h"
#include "Data/Boundary.h"
#include "Data/Front.h"

using namespace Data;

#if USE_EXPERIMENTAL_MESH

Data::Mesh::InnerVertexIterator::InnerVertexIterator()
{
    this->mesh = NULL;
}

Data::Mesh::InnerVertexIterator::InnerVertexIterator(Mesh *mesh)
{
    this->mesh = mesh;
    this->iter = mesh->vBegin();

    this->ensureValidIncrement();
}

Data::Mesh::InnerVertexIterator::InnerVertexIterator(Mesh *mesh, const VertexIterator &iter)
{
    this->mesh = mesh;
    this->iter = iter;

    this->ensureValidIncrement();
}

Data::Mesh::InnerVertexIterator &Data::Mesh::InnerVertexIterator::operator++()
{
    ++this->iter;

	this->ensureValidIncrement();

	return (*this);
}

Data::Mesh::InnerVertexIterator Data::Mesh::InnerVertexIterator::operator++(int)
{
    InnerVertexIterator tmp = (*this);

	++(*this);

	return tmp;
}

Data::Mesh::InnerVertexIterator &Data::Mesh::InnerVertexIterator::operator--()
{
    --this->iter;

    this->ensureValidDecrement();

	return (*this);
}

Data::Mesh::InnerVertexIterator Data::Mesh::InnerVertexIterator::operator--(int)
{
    InnerVertexIterator tmp = (*this);

	--(*this);

	return tmp;
}

Data::Mesh::InnerVertexIterator &Data::Mesh::InnerVertexIterator::operator=(const InnerVertexIterator &iter)
{
    this->mesh = iter.mesh;
    this->iter = iter.iter;

    return (*this);
}

bool Data::Mesh::InnerVertexIterator::operator==(const InnerVertexIterator &iter)
{
    return ((this->mesh == iter.mesh) && (this->iter == iter.iter));
}

bool Data::Mesh::InnerVertexIterator::operator!=(const InnerVertexIterator &iter)
{
    return ((this->mesh != iter.mesh) || (this->iter != iter.iter));
}

Vertex *Data::Mesh::InnerVertexIterator::operator*() const
{
    return (*this->iter);
}

void Data::Mesh::InnerVertexIterator::ensureValidIncrement()
{
    VertexIterator end = this->mesh->vEnd();

    while ((this->iter != end) &&
           (!this->mesh->haveInner((*this->iter))))
    {
        ++this->iter;
    }
}

void Data::Mesh::InnerVertexIterator::ensureValidDecrement()
{
    while (!this->mesh->haveInner((*this->iter)))
    {
        --this->iter;
    }
}

Data::Mesh::ConstInnerVertexIterator::ConstInnerVertexIterator()
{
    this->mesh = NULL;
}

Data::Mesh::ConstInnerVertexIterator::ConstInnerVertexIterator(const Mesh *mesh)
{
    this->mesh = mesh;
    this->iter = mesh->vBegin();

    this->ensureValidIncrement();
}

Data::Mesh::ConstInnerVertexIterator::ConstInnerVertexIterator(const Mesh *mesh, const ConstVertexIterator &iter)
{
    this->mesh = mesh;
    this->iter = iter;

    this->ensureValidIncrement();
}

Data::Mesh::ConstInnerVertexIterator &Data::Mesh::ConstInnerVertexIterator::operator++()
{
    ++this->iter;

	this->ensureValidIncrement();

	return (*this);
}

Data::Mesh::ConstInnerVertexIterator Data::Mesh::ConstInnerVertexIterator::operator++(int)
{
    ConstInnerVertexIterator tmp = (*this);

	++(*this);

	return tmp;
}

Data::Mesh::ConstInnerVertexIterator &Data::Mesh::ConstInnerVertexIterator::operator--()
{
    --this->iter;

    this->ensureValidDecrement();

	return (*this);
}

Data::Mesh::ConstInnerVertexIterator Data::Mesh::ConstInnerVertexIterator::operator--(int)
{
    ConstInnerVertexIterator tmp = (*this);

	--(*this);

	return tmp;
}

Data::Mesh::ConstInnerVertexIterator &Data::Mesh::ConstInnerVertexIterator::operator=(const ConstInnerVertexIterator &iter)
{
    this->mesh = iter.mesh;
    this->iter = iter.iter;

    return (*this);
}

bool Data::Mesh::ConstInnerVertexIterator::operator==(const ConstInnerVertexIterator &iter)
{
    return ((this->mesh == iter.mesh) && (this->iter == iter.iter));
}

bool Data::Mesh::ConstInnerVertexIterator::operator!=(const ConstInnerVertexIterator &iter)
{
    return ((this->mesh != iter.mesh) || (this->iter != iter.iter));
}

Vertex *Data::Mesh::ConstInnerVertexIterator::operator*() const
{
    return (*this->iter);
}

void Data::Mesh::ConstInnerVertexIterator::ensureValidIncrement()
{
    ConstVertexIterator end = this->mesh->vEnd();

    while ((this->iter != end) &&
           (!this->mesh->haveInner((*this->iter))))
    {
        ++this->iter;
    }
}

void Data::Mesh::ConstInnerVertexIterator::ensureValidDecrement()
{
    while (!this->mesh->haveInner((*this->iter)))
    {
        --this->iter;
    }
}

#endif //#if USE_EXPERIMENTAL_MESH

Data::Mesh::Quality::Quality()
{
    this->mesh = NULL;
    this->histogram = NULL;
    this->min = 0.0;
    this->max = 0.0;
    this->numSlots = 0;
}

Data::Mesh::Quality::~Quality()
{
    this->clear();
}

void Data::Mesh::Quality::setQuality(UInt numSlots, UInt *histogram, Real min, Real max)
{
    this->numSlots = numSlots;
    this->histogram = histogram;
    this->min = min;
    this->max = max;
}

UInt Data::Mesh::Quality::getNumSlots() const
{
    return this->numSlots;
}

const UInt *Data::Mesh::Quality::getHistogram() const
{
    return this->histogram;
}

Real Data::Mesh::Quality::getMin() const
{
    return this->min;
}

Real Data::Mesh::Quality::getMax() const
{
    return this->max;
}

#if USE_GUI
void Data::Mesh::Quality::findQuality(Data::Mesh *mesh, UInt numSlots, Real *colors)
#else
void Data::Mesh::Quality::findQuality(Data::Mesh *mesh, UInt numSlots)
#endif //#if USE_GUI
{
    this->mesh = mesh;

    if (numSlots == 0)
    {
        return;
    }

    if ((this->histogram) && (this->numSlots != numSlots))
    {
        delete [] this->histogram;
    }

    this->numSlots = numSlots;

    if (!this->histogram)
    {
        this->histogram = new UInt[this->numSlots];
    }

    for (UInt i = 0; i < this->numSlots; i++)
    {
        this->histogram[i] = 0;
    }

    this->min = 1.0;
    this->max = 0.0;

    if (!this->mesh)
    {
        return;
    }

    for (Data::Mesh::ConstElementIterator iter = this->mesh->eBegin();
         iter != this->mesh->eEnd(); iter++)
    {
        Data::Element *e = (*iter);

        Real q = e->quality();

        if (q < this->min)
        {
            this->min = q;
        }

        if (q > this->max)
        {
            this->max = q;
        }

        UInt pos = static_cast<UInt>(q*static_cast<Real>(this->numSlots));

        if (pos > this->numSlots - 1)
        {
            pos = this->numSlots - 1;
        }

#if USE_GUI
        if (colors)
        {
            UInt cpos = (this->numSlots == 10) ? pos : 10*pos/this->numSlots;

            e->setColor(colors[0*10 + cpos], colors[1*10 + cpos], colors[2*10 + cpos]);
        }
#endif //#if USE_GUI

        this->histogram[pos]++;
    }
}

void Data::Mesh::Quality::interval(UInt i, UInt total, std::stringstream &min, std::stringstream &max) const
{
    if (total == 1)
    {
        min << "0";
        max << "1";

        return;
    }

    UInt num0s = static_cast<UInt>(std::log10(total));

    if (i + 1 == total)
    {
        min << "0.";
        max << "1.";

        for (UInt j = 0; j < num0s; j++)
        {
            min << "9";
            max << "0";
        }

        return;
    }

    min << "0.";
    max << "0.";

    if (i == 0)
    {
        for (UInt j = 1; j < num0s; j++)
        {
            min << "0";
            max << "0";
        }

        min << "0";
        max << "1";

        return;
    }

    UInt num0smin = num0s - static_cast<UInt>(std::log10(i)) - 1;
    UInt num0smax = num0s - static_cast<UInt>(std::log10(i + 1)) - 1;

    for (UInt j = 0; j < num0smin; j++)
    {
        min << "0";
    }

    min << i;

    for (UInt j = 0; j < num0smax; j++)
    {
        max << "0";
    }

    max << i + 1;
}

std::string Data::Mesh::Quality::qualityString() const
{
    if (!this->histogram)
	{
		return "";
	}

	std::stringstream str;

    str.precision(6);
    str.setf(std::ios::fixed, std::ios::floatfield);

	ULInt size = this->mesh ? this->mesh->size() : 0;

	if (!this->mesh)
    {
        for (UInt i = 0; i < this->numSlots; i++)
        {
            size += this->histogram[i];
        }
    }

	str << "Number of elements: " << size;

	//to avoid division by 0
	if (size == 0)
	{
		size = 1;
	}

	UInt numSpaces = 0;

	for (UInt i = 0; i < this->numSlots; i++)
	{
		while (this->histogram[i] > std::pow(10, numSpaces))
		{
			numSpaces++;
		}
	}

	for (UInt i = 0; i < this->numSlots; i++)
	{
		str << std::endl;

		std::stringstream min, max;

		this->interval(i, this->numSlots, min, max);

		str << "[" << min.str() << ", " << max.str() << "]:\t";

		str.width(numSpaces);

		str << this->histogram[i];

		str << "\t\t";

		str << 100.0*static_cast<Real>(this->histogram[i])/static_cast<Real>(size) << " %";
	}

	str << std::endl;
	str << "Min: " << this->min << std::endl;
	str << "Max: " << this->max;

	return str.str();
}

void Data::Mesh::Quality::clear()
{
    this->mesh = NULL;

    if (this->histogram)
    {
        delete [] this->histogram;

        this->histogram = NULL;
    }

    this->min = this->max = 0.0;
    this->numSlots = 0;
}

Data::Mesh::Mesh(ULInt id) :
	Data::AbstractMesh(id)
{
#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_FRONT_ELEMENT, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI

#if USE_EXPERIMENTAL_MESH
    this->numInnerVertices = 0;
#endif //#if USE_EXPERIMENTAL_MESH
}

Data::Mesh::~Mesh()
{
	while (!this->frontElements.empty())
	{
		FrontElement *e = *this->frontElements.begin();
		this->frontElements.erase(frontElements.begin());

		if (e)
		{
			delete e;
		}
	}

	while (!this->elements.empty())
	{
		Element *e = *(this->elements.begin());
		this->elements.erase(this->elements.begin());

		if (e)
		{
			delete e;
		}
	}

	while (!this->vertices.empty())
	{
		Vertex *v = *(this->vertices.begin());
		this->vertices.erase(this->vertices.begin());

		if (v)
		{
			delete v;
		}
	}

	//this->innerVertices.clear();
}

void Data::Mesh::sortMesh()
{
    this->vertices.sort(    [&](const Data::Vertex * first, const Data::Vertex * second) {   return first->getId() < second->getId();  }    );

    this->elements.sort(    [&](const Data::Element * first, const Data::Element * second) {   return first->getId() < second->getId();  }    );
}

void Data::Mesh::setVertices(const VertexList &vertices)
{
	this->vertices = vertices;

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

const VertexList &Data::Mesh::getVertices() const
{
	return this->vertices;
}

/*VertexList Data::Mesh::getVertices(bool sorted) const
{
	if (!sorted)
	{
		//return this->vertices;
		return VertexList(this->vertices.begin(), this->vertices.end());
	}

	VertexList vertices(this->vertices.begin(), this->vertices.end());

	vertices.sort(Identifiable::smaller);

	return vertices;
}*/

/*VertexList Data::Mesh::getSortedVertices() const
{
	VertexList vertices(this->vertices.begin(), this->vertices.end());

	vertices.sort(Identifiable::smaller);

	return vertices;
}*/

void Data::Mesh::setFrontElements(const FrontElementList &frontElements)
{
	this->frontElements = frontElements;

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

const FrontElementList &Data::Mesh::getFrontElements() const
{
	return this->frontElements;
}

/*FrontElementList Data::Mesh::getFrontElements(bool sorted) const
{
	if (!sorted)
	{
		//return this->frontElements;
		return FrontElementList(this->frontElements.begin(), this->frontElements.end());
	}

	FrontElementList frontElements(this->frontElements.begin(), this->frontElements.end());

	frontElements.sort(Identifiable::smaller);

	return frontElements;
}*/

/*FrontElementList Data::Mesh::getSortedFrontElements() const
{
	FrontElementList frontElements(this->frontElements.begin(), this->frontElements.end());

	frontElements.sort(Identifiable::smaller);

	return frontElements;
}*/

void Data::Mesh::setElements(const ElementList &elements)
{
	//this->elements = elements;
	this->elements = elements;

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

const ElementList &Data::Mesh::getElements() const
{
	return this->elements;
}

/*ElementList Data::Mesh::getElements(bool sorted) const
{
	if (!sorted)
	{
		//return this->elements;
		return ElementList(this->elements.begin(), this->elements.end());
	}

	ElementList elements(this->elements.begin(), this->elements.end());

	elements.sort(Identifiable::smaller);

	return elements;
}*/

/*ElementList Data::Mesh::getSortedElements() const
{
	ElementList elements(this->elements.begin(), this->elements.end());

	elements.sort(Identifiable::smaller);

	return elements;
}*/

#if (!USE_EXPERIMENTAL_MESH)
void Data::Mesh::setInnerVertices(const VertexList &innerVertices)
{
	this->innerVertices = innerVertices;
}
#endif //#if (!USE_EXPERIMENTAL_MESH)

#if USE_EXPERIMENTAL_MESH
VertexList Data::Mesh::getInnerVertices() const
#else
const VertexList &Data::Mesh::getInnerVertices() const
#endif //#if USE_EXPERIMENTAL_MESH
{
#if USE_EXPERIMENTAL_MESH
    VertexList vertices;

    for (Mesh::ConstInnerVertexIterator iter = this->ivBegin();
         iter != this->ivEnd(); iter++)
    {
        vertices.push_back((*iter));
    }

    return vertices;

    //return Data::VertexList(this->ivBegin(), this->ivEnd());
#else
	return this->innerVertices;
#endif //#if USE_EXPERIMENTAL_MESH
}

/*VertexList Data::Mesh::getInnerVertices(bool sorted) const
{
	if (!sorted)
	{
		//return this->innerVertices;
		return VertexList(this->innerVertices.begin(), this->innerVertices.end());
	}

	VertexList innerVertices(this->innerVertices.begin(), this->innerVertices.end());

	innerVertices.sort(Identifiable::smaller);

	return innerVertices;
}*/

/*VertexList Data::Mesh::getSortedInnerVertices() const
{
	VertexList innerVertices(this->innerVertices.begin(), this->innerVertices.end());

	innerVertices.sort(Identifiable::smaller);

	return innerVertices;
}*/

UInt Data::Mesh::dimension() const
{
	return (this->vertices.empty()) ? 0 : (*this->vertices.begin())->getPoint()->dimension();
}

bool Data::Mesh::isSurface() const
{
    if (this->dimension() != 3)
    {
        return false;
    }

    if (this->size() != 0)
    {
        Data::Element *e = *this->eBegin();

        if (e->numVertices() == 3)
        {
            return true;
        }

#if USE_BRICKS
        if (e->numVertices() > 4)
        {
            return false;
        }
#endif //#if USE_BRICKS

#if (!USE_QUADS)
        return false;
#endif //#if (!USE_QUADS)

        return dynamic_cast<Data::Polygon *>(e->getShape());
    }

    if (this->frontElementsSize() != 0)
    {
        Data::FrontElement *fel = *this->feBegin();

        return (fel->numVertices() == 2);

        //return dynamic_cast<Data::Edge *>(fel->getShape());
    }

    return false;
}

void Data::Mesh::clear()
{
	this->vertices.clear();
#if USE_EXPERIMENTAL_MESH
    this->numInnerVertices = 0;
#else
	this->innerVertices.clear();
#endif //#if USE_EXPERIMENTAL_MESH
	this->frontElements.clear();
	this->elements.clear();
}

ULInt Data::Mesh::verticesSize() const
{
	return static_cast<ULInt>(this->vertices.size());
}

ULInt Data::Mesh::innerVerticesSize() const
{
#if USE_EXPERIMENTAL_MESH
    return this->numInnerVertices;
#else
	return static_cast<ULInt>(this->innerVertices.size());
#endif //#if USE_EXPERIMENTAL_MESH
}

ULInt Data::Mesh::frontElementsSize() const
{
	return static_cast<ULInt>(this->frontElements.size());
}

ULInt Data::Mesh::size() const
{
	return static_cast<ULInt>(this->elements.size());
}

void Data::Mesh::add(Vertex *v, bool inner)
{
	/*this->vertices.push_back(v);

	if (inner)
	{
		this->innerVertices.push_back(v);
	}*/

	this->vertices.push_back(v);

	if (inner)
	{
		this->addInner(v);
	}

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

#if (!USE_EXPERIMENTAL_MESH)
void Data::Mesh::addInner(Vertex *v)
{
	this->innerVertices.push_back(v);
}
#endif //#if (!USE_EXPERIMENTAL_MESH)

void Data::Mesh::add(FrontElement *e)
{
	this->frontElements.push_back(e);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

void Data::Mesh::add(Element *e)
{
	this->elements.push_back(e);

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI
}

bool Data::Mesh::remove(Vertex *v)
{
	VertexIterator it = std::find(this->vertices.begin(), this->vertices.end(), v);

	if (it != this->vertices.end())
	{
		this->vertices.erase(it);

		this->removeInner(v);

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI

		return true;
	}

	return false;
}

#if (!USE_EXPERIMENTAL_MESH)
void Data::Mesh::removeInner(Vertex *v)
{
	VertexIterator it = std::find(this->innerVertices.begin(), this->innerVertices.end(), v);

	if (it != this->innerVertices.end())
	{
		this->innerVertices.erase(it);
	}
}
#endif //#if (!USE_EXPERIMENTAL_MESH)

bool Data::Mesh::remove(FrontElement *e)
{
	FrontElementList::iterator it = std::find(this->frontElements.begin(), this->frontElements.end(), e);

	if (it != this->frontElements.end())
	{
		this->frontElements.erase(it);

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI

		return true;
	}

	return false;
}

bool Data::Mesh::remove(Element *e)
{
	ElementIterator it = std::find(this->elements.begin(), this->elements.end(), e);

	if (it != this->elements.end())
	{
		this->elements.erase(it);

#if USE_GUI
		this->outdate();
#endif //#if USE_GUI

		return true;
	}

	return false;
}

bool Data::Mesh::have(const Vertex *v) const
{
	return (std::find(this->vertices.begin(), this->vertices.end(), v) != this->vertices.end());
}

#if (!USE_EXPERIMENTAL_MESH)
bool Data::Mesh::haveInner(const Vertex *v) const
{
	return (std::find(this->innerVertices.begin(), this->innerVertices.end(), v) != this->innerVertices.end());
}
#endif //#if (!USE_EXPERIMENTAL_MESH)

bool Data::Mesh::have(const FrontElement *e) const
{
	return (std::find(this->frontElements.begin(), this->frontElements.end(), e) != this->frontElements.end());
}

bool Data::Mesh::have(const Element *e) const
{
	return (std::find(this->elements.begin(), this->elements.end(), e) != this->elements.end());
}

Data::Mesh::VertexIterator Data::Mesh::vBegin()
{
	return this->vertices.begin();
}

Data::Mesh::ConstVertexIterator Data::Mesh::vBegin() const
{
	return this->vertices.begin();
}

Data::Mesh::VertexIterator Data::Mesh::vEnd()
{
	return this->vertices.end();
}

Data::Mesh::ConstVertexIterator Data::Mesh::vEnd() const
{
	return this->vertices.end();
}

#if USE_EXPERIMENTAL_MESH
Data::Mesh::InnerVertexIterator Data::Mesh::ivBegin()
#else
Data::Mesh::VertexIterator Data::Mesh::ivBegin()
#endif //#if USE_EXPERIMENTAL_MESH
{
#if USE_EXPERIMENTAL_MESH
	return Data::Mesh::InnerVertexIterator(this, this->vertices.begin());
#else
    this->innerVertices.begin();
#endif //#if USE_EXPERIMENTAL_MESH
}

#if USE_EXPERIMENTAL_MESH
Data::Mesh::ConstInnerVertexIterator Data::Mesh::ivBegin() const
#else
Data::Mesh::ConstVertexIterator Data::Mesh::ivBegin() const
#endif //#if USE_EXPERIMENTAL_MESH
{
#if USE_EXPERIMENTAL_MESH
	return Data::Mesh::ConstInnerVertexIterator(this, this->vertices.begin());
#else
	return this->innerVertices.begin();
#endif //#if USE_EXPERIMENTAL_MESH
}

#if USE_EXPERIMENTAL_MESH
Data::Mesh::InnerVertexIterator Data::Mesh::ivEnd()
#else
Data::Mesh::VertexIterator Data::Mesh::ivEnd()
#endif //#if USE_EXPERIMENTAL_MESH
{
#if USE_EXPERIMENTAL_MESH
	return Data::Mesh::InnerVertexIterator(this, this->vertices.end());
#else
	return this->innerVertices.end();
#endif //#if USE_EXPERIMENTAL_MESH
}

#if USE_EXPERIMENTAL_MESH
Data::Mesh::ConstInnerVertexIterator Data::Mesh::ivEnd() const
#else
Data::Mesh::ConstVertexIterator Data::Mesh::ivEnd() const
#endif //#if USE_EXPERIMENTAL_MESH
{
#if USE_EXPERIMENTAL_MESH
	return Data::Mesh::ConstInnerVertexIterator(this, this->vertices.end());
#else
	return this->innerVertices.end();
#endif //#if USE_EXPERIMENTAL_MESH
}

Data::Mesh::FrontElementIterator Data::Mesh::feBegin()
{
	return this->frontElements.begin();
}

Data::Mesh::ConstFrontElementIterator Data::Mesh::feBegin() const
{
	return this->frontElements.begin();
}

Data::Mesh::FrontElementIterator Data::Mesh::feEnd()
{
	return this->frontElements.end();
}

Data::Mesh::ConstFrontElementIterator Data::Mesh::feEnd() const
{
	return this->frontElements.end();
}

Data::Mesh::ElementIterator Data::Mesh::eBegin()
{
	return this->elements.begin();
}

Data::Mesh::ConstElementIterator Data::Mesh::eBegin() const
{
	return this->elements.begin();
}

Data::Mesh::ElementIterator Data::Mesh::eEnd()
{
	return this->elements.end();
}

Data::Mesh::ConstElementIterator Data::Mesh::eEnd() const
{
	return this->elements.end();
}

Data::Mesh::VertexIterator Data::Mesh::erase(const Data::Mesh::VertexIterator &iter)
{
	this->removeInner((*iter));

#if USE_GUI
	Data::Mesh::VertexIterator iter2 = this->vertices.erase(iter);

	this->outdate();

	return iter2;
#else
	return this->vertices.erase(iter);
#endif //#if USE_GUI

	//VertexIterator iter2 = iter;
	//iter2++;

	//this->vertices.erase(iter);
//#if USE_GUI
//	this->outdate();
//#endif //#if USE_GUI
	//return iter2;
}

#if (!USE_EXPERIMENTAL_MESH)
Data::Mesh::VertexIterator Data::Mesh::eraseInner(const Data::Mesh::VertexIterator &iter)
{
	return this->innerVertices.erase(iter);

	//VertexIterator iter2 = iter;
	//iter2++;

	//this->innerVertices.erase(iter);

	//return iter2;
}

Data::Mesh::VertexIterator Data::Mesh::eraseFromInner(const Data::Mesh::VertexIterator &iter)
{
	this->vertices.remove((*iter));

#if USE_GUI
	this->outdate();
#endif //#if USE_GUI

	return this->eraseInner(iter);
}
#endif //#if (!USE_EXPERIMENTAL_MESH)

Data::Mesh::FrontElementIterator Data::Mesh::erase(const Data::Mesh::FrontElementIterator &iter)
{
#if USE_GUI
	Data::Mesh::FrontElementIterator iter2 = this->frontElements.erase(iter);

	this->outdate();

	return iter2;
#else
	return this->frontElements.erase(iter);
#endif //#endif //#if USE_GUI

	//FrontElementIterator iter2 = iter;
	//iter2++;

	//this->frontElements.erase(iter);
//#if USE_GUI
//	this->outdate();
//#endif //#if USE_GUI
	//return iter2;
}

Data::Mesh::ElementIterator Data::Mesh::erase(const Data::Mesh::ElementIterator &iter)
{
#if USE_GUI
	Data::Mesh::ElementIterator iter2 = this->elements.erase(iter);

	this->outdate();

	return iter2;
#else
	return this->elements.erase(iter);
#endif //#endif //#if USE_GUI

	//ElementIterator iter2 = iter;
	//iter2++;

	//this->elements.erase(iter);
//#if USE_GUI
//	this->outdate();
//#endif //#if USE_GUI
	//return iter2;
}

bool Data::Mesh::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::Mesh::in(const Point *p) const
{
	for (ConstElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		if ((*iter)->in(p))
		{
			return true;
		}
	}

	return false;
}

bool Data::Mesh::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::Mesh::on(const Point *p) const
{
	for (ConstElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		if ((*iter)->on(p))
		{
			return true;
		}
	}

	return false;
}

bool Data::Mesh::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::Mesh::out(const Point *p) const
{
	for (ConstElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		if ((*iter)->out(p))
		{
			return true;
		}
	}

	return false;
}

ElementList Data::Mesh::adjacency(const Vertex *v) const
{
	ElementList adj;

	for (ConstElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		if ((*iter)->have(v))
		{
			adj.push_back((*iter));
		}
	}

	return adj;
}

ElementList Data::Mesh::adjacency(const FrontElement *e) const
{
	ElementList adj;

	for (ConstElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		if ((*iter)->have(e))
		{
			adj.push_back((*iter));

			if (adj.size() == 2)
			{
				break;
			}
		}
	}

	return adj;
}

FrontElementList Data::Mesh::findAdjacentFrontElements(const Vertex *v) const
{
	FrontElementList adjs;

	for (ConstFrontElementIterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		FrontElement *fel = (*iter);

		if (fel->have(v))
		{
			adjs.push_back(fel);
		}
	}

	return adjs;
}

FrontElementList Data::Mesh::findAdjacentFrontElements(const Element *e) const
{
	FrontElementList adjs;

	for (ConstFrontElementIterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		FrontElement *fel = (*iter);

		if (e->have(fel))
		{
			adjs.push_back(fel);
		}
	}

	return adjs;
}

FrontElementList Data::Mesh::findSortedAdjacentFrontElements(const FrontElement *fe, UInt i) const
{
	return this->findSortedAdjacentFrontElements(fe, i, NULL, NULL);
}

FrontElementList Data::Mesh::findSortedAdjacentFrontElements(const FrontElement *fe, UInt i, const Boundary *boundary) const
{
	return this->findSortedAdjacentFrontElements(fe, i, boundary, NULL);
}

FrontElementList Data::Mesh::findSortedAdjacentFrontElements(const FrontElement *fe, UInt i, const Front *front) const
{
	return this->findSortedAdjacentFrontElements(fe, i, NULL, front);
}

FrontElementList Data::Mesh::findSortedAdjacentFrontElements(const FrontElement *fe, UInt i, const Boundary *boundary, const Front *front) const
{
    if (i >= fe->numVertices())
	{
		return FrontElementList();
	}

	Data::Vertex *v = fe->getVertex(i);

	return this->findSortedAdjacentFrontElements(fe, v, i, boundary, front, this->findAdjacentFrontElements(v));
}

FrontElementList Data::Mesh::findSortedAdjacentFrontElements(const FrontElement *fe, const Vertex *v, FrontElementList adjs)
{
    UInt i = 0;

    for (; i < fe->numVertices(); i++)
    {
        if (fe->getVertex(i) == v)
        {
            break;
        }
    }

    if (i == fe->numVertices())
    {
        return FrontElementList();
    }

    return findSortedAdjacentFrontElements(fe, v, i, NULL, NULL, adjs);
}

FrontElementList Data::Mesh::findSortedAdjacentFrontElements(const FrontElement *fe, const Vertex *v, UInt i,
    const Boundary *boundary, const Front *front, FrontElementList adjs)
{
	if ((boundary) || (front))
	{
		for (FrontElementList::iterator iter = adjs.begin();
			 iter != adjs.end();)
		{
			FrontElement *adj = (*iter);

			bool inboundary = boundary ? boundary->have(adj) : true;

			if (!inboundary)
			{
				iter = adjs.erase(iter);

				continue;
			}

			bool infront = front ? front->have(adj) : true;

			if (!infront)
			{
				iter = adjs.erase(iter);

				continue;
			}

			iter++;
		}
	}

	if (adjs.size() <= 1)
	{
		return adjs;
	}

	if (adjs.size() == 2)
	{
		if (adjs.back() == fe)
		{
			adjs.push_front(adjs.back());

			adjs.pop_back();
		}

		if (adjs.front() == fe)
		{
			return adjs;
		}
	}

	UInt dimension = v->getPoint()->dimension();

	struct RealBool
	{
		Real val;
		bool positive;

		bool operator<(const RealBool &rb) const
		{
			return ((this->val < rb.val) ||
					((this->val <= rb.val) && (this->positive) && (!rb.positive)));
		};
	};

	///it has to be a map, to keep the adjacency sorted
	typedef std::multimap<RealBool, FrontElement *> AngFEMap;

	AngFEMap sorted;

	if (dimension == 2)
	{
		Edge2D *edge = static_cast<Edge2D *>(fe->getShape());

#if USE_EXPERIMENTAL_3
		Vector2D vec = edge->vector2D();
#else
		Vector2D vec = edge->vector();
#endif //#if USE_EXPERIMENTAL_3

		vec.normalize();

		if (i == 1)
		{
			vec.invert();
		}

		Vector2D normal = edge->normal();

		while (!adjs.empty())
		{
			FrontElement *adj = adjs.front();

			adjs.pop_front();

			if (adj == fe)
			{
				continue;
			}

			Edge2D *ed2 = static_cast<Edge2D *>(adj->getShape());

#if USE_EXPERIMENTAL_3
			Vector2D adjv = ed2->vector2D();
#else
			Vector2D adjv = ed2->vector();
#endif //#if USE_EXPERIMENTAL_3

			adjv.normalize();

			if (adj->getVertex(1) == v)
			{
				adjv.invert();
			}

			RealBool rb;

			rb.val = vec.dot(adjv);
			rb.positive = false;

			if (rb.val < -1.0)
			{
				rb.val = -1.0;
			}
			else if (rb.val > 1.0)
			{
				rb.val = 1.0;
			}

			if ((rb.val > -1.0) && (rb.val < 1.0))
			{
				if (vec.left(adjv))
				{
					rb.val = 1.0 - rb.val;
				}
				else
				{
					rb.val = 3.0 + rb.val;
				}
			}
			else
			{
				rb.val = 1.0 - rb.val;
			}

			Vector2D adjn = ed2->normal();

			if (rb.val == 0.0)
			{
				if (adjn.dot(normal) < 0.0)
				{
					rb.val = 4.0;
					rb.positive = true;
				}
			}
			else if (rb.val == 4.0)
			{
				if (adjn.dot(normal) > 0.0)
				{
					rb.val = 0.0;
				}
				else
				{
					rb.positive = true;
				}
			}
			else if (rb.val == 2.0)
			{
				if (adjn.dot(normal) > 0.0)
				{
					rb.positive = true;
				}
			}
			else if (rb.val < 2.0)
			{
				if (adj->accordingToNormal(fe->getVertex(1 - i), false))
				{
					rb.positive = true;
				}
			}
			else
			{
				if (!adj->accordingToNormal(fe->getVertex(1 - i), false))
				{
					rb.positive = true;
				}
			}

			sorted.insert(std::make_pair(rb, adj));
		}
	}
	else
	{
		Polygon *pol = static_cast<Polygon *>(fe->getShape());

		Vector3D normal = pol->normal();

		//normal.normalize();

		Vertex *next = fe->getVertex((i + 1)%fe->numVertices());

		Vector3D vk(v->getPoint(), next->getPoint());

		vk.multiply(0.5);

		Point3D mid(v->getPoint(), &vk);

		vk.normalize();

		//Vector3D vj = vk.cross(normal);

		Point3D vend = *fe->getVertex((i + 2)%fe->numVertices())->getPoint();

		vend.project(mid, vk);

		Vector3D vec(mid, vend);

		vec.normalize();

		while (!adjs.empty())
		{
			Data::FrontElement *adj = adjs.front();

			adjs.pop_front();

			if (adj == fe)
			{
				continue;
			}

			bool found = false;
			Vertex *other = NULL;

			for (UInt j = 0; j < adj->numVertices(); j++)
			{
				if (adj->getVertex(j) == next)
				{
					found = true;
				}
				else if (adj->getVertex(j) != v)
				{
					other = adj->getVertex(j);
				}

				if ((other) && (found))
				{
					break;
				}
			}

			if (!found)
			{
				continue;
			}

			Polygon *pol2 = static_cast<Polygon *>(adj->getShape());

			Point3D otherpt = *other->getPoint();

			otherpt.project(mid, vk);

			Vector3D adjv(mid, otherpt);

			adjv.normalize();

			RealBool rb;

			rb.val = vec.dot(adjv);
			rb.positive = false;

			if (rb.val < -1.0)
			{
				rb.val = -1.0;
			}
			else if (rb.val > 1.0)
			{
				rb.val = 1.0;
			}

			if ((rb.val > -1.0) && (rb.val < 1.0))
			{
				if (fe->accordingToNormal(other, false))
				{
					rb.val = 1.0 - rb.val;
				}
				else
				{
					rb.val = 3.0 + rb.val;
				}
			}
			else
			{
				rb.val = 1.0 - rb.val;
			}

			Vector3D adjn = pol2->normal();

			if (rb.val == 0.0)
			{
				if (adjn.dot(normal) < 0.0)
				{
					rb.val = 4.0;
					rb.positive = true;
				}
			}
			else if (rb.val == 4.0)
			{
				if (adjn.dot(normal) > 0.0)
				{
					rb.val = 0.0;
				}
				else
				{
					rb.positive = true;
				}
			}
			else if (rb.val == 2.0)
			{
				if (adjn.dot(normal) > 0.0)
				{
					rb.positive = true;
				}
			}
			else if (rb.val < 2.0)
			{
				if (adj->accordingToNormal(fe->getVertex((i + 2)%fe->numVertices()), false))
				{
					rb.positive = true;
				}
			}
			else
			{
				if (!adj->accordingToNormal(fe->getVertex((i + 2)%fe->numVertices()), false))
				{
					rb.positive = true;
				}
			}

            sorted.insert(std::make_pair(rb, adj));
		}
	}

	for (AngFEMap::iterator iter = sorted.begin();
		 iter != sorted.end(); iter++)
	{
		adjs.push_back((*iter).second);
	}

	if (((!boundary) || (boundary->have(fe))) &&
		((!front) || (front->have(fe))))
	{
		adjs.push_front(const_cast<FrontElement *>(fe));
	}

	return adjs;
}

#if USE_C__11
void Data::Mesh::findLayer(const VertexList &vertices, UInt layer,
	VertexHash &layerVertices,
	bool frontElements, FrontElementHash &layerFrontElements,
	bool elements, ElementHash &layerElements) const
#else
void Data::Mesh::findLayer(const VertexList &vertices, UInt layer,
	VertexSet &layerVertices,
	bool frontElements, FrontElementSet &layerFrontElements,
	bool elements, ElementSet &layerElements) const
#endif //#if USE_C__11
{
	if (layer == 0)
	{
		if ((!frontElements) && (!elements))
		{
			layerVertices.insert(vertices.begin(), vertices.end());

			return;
		}

		for (VertexList::const_iterator iter = vertices.begin();
			 iter != vertices.end(); iter++)
		{
			Vertex *v = (*iter);

			if (!layerVertices.insert(v).second)
			{
				continue;
			}

			if (frontElements)
			{
				FrontElementList fadjs = this->findAdjacentFrontElements(v);

				layerFrontElements.insert(fadjs.begin(), fadjs.end());
			}

			if (elements)
			{
				ElementList adjs = this->adjacency(v);

				layerElements.insert(adjs.begin(), adjs.end());
			}
		}

		return;
	}

#if USE_C__11
	VertexHash current(vertices.begin(), vertices.end());
	VertexHash previous, next;
#else
	VertexSet current(vertices.begin(), vertices.end());
	VertexSet previous, next;
#endif //#if USE_C__11

	for (UInt i = 0; i < layer; i++)
	{
#if USE_C__11
		for (VertexHash::iterator iter = current.begin();
#else
		for (VertexSet::iterator iter = current.begin();
#endif //#if USE_C__11
			 iter != current.end(); iter++)
		{
			Vertex *v = (*iter);

			ElementList adjs = this->adjacency(v);

			for (ElementList::iterator iter2 = adjs.begin();
				 iter2 != adjs.end(); iter2++)
			{
				Element *e = (*iter2);

				for (UInt i = 0; i < e->numVertices(); i++)
				{
					Vertex *adj = e->getVertex(i);

					if ((adj == v) ||
						(current.find(adj) != current.end()) ||
						(previous.find(adj) != previous.end()))
					{
						continue;
					}

					next.insert(adj);
				}
			}
		}

		previous.clear();

		previous.swap(current);

		current.swap(next);
	}

	layerVertices.insert(current.begin(), current.end());

	if ((!frontElements) && (!elements))
	{
		return;
	}

#if USE_C__11
	for (VertexHash::iterator iter = current.begin();
#else
	for (VertexSet::iterator iter = current.begin();
#endif //#if USE_C__11
		 iter != current.end(); iter++)
	{
		Vertex *v = (*iter);

		if (frontElements)
		{
			FrontElementList fadjs = this->findAdjacentFrontElements(v);

			layerFrontElements.insert(fadjs.begin(), fadjs.end());
		}

		if (elements)
		{
			ElementList adjs = this->adjacency(v);

			layerElements.insert(adjs.begin(), adjs.end());
		}
	}
}

#if USE_C__11
VertexHash Data::Mesh::findLayer(const VertexList &vertices, UInt layer) const
#else
VertexSet Data::Mesh::findLayer(const VertexList &vertices, UInt layer) const
#endif //#if USE_C__11
{
#if USE_C__11
	VertexHash layerVertices;
#else
	VertexSet layerVertices;
#endif //#if USE_C__11

	this->findLayer(vertices, layer, layerVertices);

	return layerVertices;
}

#if USE_C__11
void Data::Mesh::findLayer(const VertexList &vertices, UInt layer,
	VertexHash &layerVertices) const
#else
void Data::Mesh::findLayer(const VertexList &vertices, UInt layer,
	VertexSet &layerVertices) const
#endif //#if USE_C__11
{
#if USE_C__11
	FrontElementHash layerFels;
	ElementHash layerEls;
#else
	FrontElementSet layerFels;
	ElementSet layerEls;
#endif //#if USE_C__11

	this->findLayer(vertices, layer, layerVertices, false, layerFels, false, layerEls);
}

#if USE_C__11
void Data::Mesh::findLayer(const VertexList &vertices, UInt layer,
	VertexHash &layerVertices, FrontElementHash &layerFrontElements) const
#else
void Data::Mesh::findLayer(const VertexList &vertices, UInt layer,
	VertexSet &layerVertices, FrontElementSet &layerFrontElements) const
#endif //#if USE_C__11
{
#if USE_C__11
	ElementHash layerEls;
#else
	ElementSet layerEls;
#endif //#if USE_C__11

	this->findLayer(vertices, layer, layerVertices, true, layerFrontElements, false, layerEls);
}

#if USE_C__11
void Data::Mesh::findLayer(const VertexList &vertices, UInt layer,
	VertexHash &layerVertices, ElementHash &layerElements) const
#else
void Data::Mesh::findLayer(const VertexList &vertices, UInt layer,
	VertexSet &layerVertices, ElementSet &layerElements) const
#endif //#if USE_C__11
{
#if USE_C__11
	FrontElementHash layerFels;
#else
	FrontElementSet layerFels;
#endif //#if USE_C__11

	this->findLayer(vertices, layer, layerVertices, false, layerFels, true, layerElements);
}

#if USE_C__11
void Data::Mesh::findLayer(const VertexList &vertices, UInt layer,
	VertexHash &layerVertices, FrontElementHash &layerFrontElements,
	ElementHash &layerElements) const
#else
void Data::Mesh::findLayer(const VertexList &vertices, UInt layer,
	VertexSet &layerVertices, FrontElementSet &layerFrontElements,
	ElementSet &layerElements) const
#endif //#if USE_C__11
{
	this->findLayer(vertices, layer, layerVertices, true, layerFrontElements, true, layerElements);
}

#if USE_C__11
void Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer,
	VertexHash &layerVertices,
	bool frontElements, FrontElementHash &layerFrontElements,
	bool elements, ElementHash &layerElements)
#else
void Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer,
	VertexSet &layerVertices,
	bool frontElements, FrontElementSet &layerFrontElements,
	bool elements, ElementSet &layerElements)
#endif //#if USE_C__11
{
	if (layer == 0)
	{
		for (VertexList::const_iterator iter = vertices.begin();
			 iter != vertices.end(); iter++)
		{
			Vertex *v = (*iter);

			if (!layerVertices.insert(v).second)
			{
				continue;
			}

			this->remove(v);

			if (frontElements)
			{
				FrontElementList fadjs = this->findAdjacentFrontElements(v);

				while (!fadjs.empty())
				{
					Data::FrontElement *fel = fadjs.front();

					fadjs.pop_front();

					if (layerFrontElements.insert(fel).second)
					{
						this->remove(fel);
					}
				}
			}

			if (elements)
			{
				ElementList adjs = this->adjacency(v);

				while (!adjs.empty())
				{
					Data::Element *el = adjs.front();

					adjs.pop_front();

					if (layerElements.insert(el).second)
					{
						this->remove(el);
					}
				}
			}
		}

		return;
	}

#if USE_C__11
	VertexHash current(vertices.begin(), vertices.end());
	VertexHash previous, next;
#else
    VertexSet current(vertices.begin(), vertices.end());
	VertexSet previous, next;
#endif //#if USE_C__11

	for (UInt i = 0; i < layer; i++)
	{
#if USE_C__11
		for (VertexHash::iterator iter = current.begin();
#else
		for (VertexSet::iterator iter = current.begin();
#endif //#if USE_C__11
			 iter != current.end(); iter++)
		{
			Vertex *v = (*iter);

			ElementList adjs = this->adjacency(v);

			for (ElementList::iterator iter2 = adjs.begin();
				 iter2 != adjs.end(); iter2++)
			{
				Element *e = (*iter2);

				for (UInt i = 0; i < e->numVertices(); i++)
				{
					Vertex *adj = e->getVertex(i);

					if ((adj == v) ||
						(current.find(adj) != current.end()) ||
						(previous.find(adj) != previous.end()))
					{
						continue;
					}

					next.insert(adj);
				}
			}
		}

		previous.clear();

		previous.swap(current);

		current.swap(next);
	}

#if USE_C__11
	for (VertexHash::iterator iter = current.begin();
#else
	for (VertexSet::iterator iter = current.begin();
#endif //#if USE_C__11
		 iter != current.end(); iter++)
	{
		Vertex *v = (*iter);

		this->remove(v);

		if (frontElements)
		{
			FrontElementList fadjs = this->findAdjacentFrontElements(v);

			while (!fadjs.empty())
			{
				Data::FrontElement *fel = fadjs.front();

				fadjs.pop_front();

				if (layerFrontElements.insert(fel).second)
				{
					this->remove(fel);
				}
			}
		}

		if (elements)
		{
			ElementList adjs = this->adjacency(v);

			while (!adjs.empty())
			{
				Data::Element *el = adjs.front();

				adjs.pop_front();

				if (layerElements.insert(el).second)
				{
					this->remove(el);
				}
			}
		}
	}
}

#if USE_C__11
VertexHash Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer)
#else
VertexSet Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer)
#endif //#if USE_C__11
{
#if USE_C__11
	VertexHash layerVertices;
#else
	VertexSet layerVertices;
#endif //#if USE_C__11

	this->findAndRemoveLayer(vertices, layer, layerVertices);

	return layerVertices;
}

#if USE_C__11
void Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer,
	VertexHash &layerVertices)
#else
void Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer,
	VertexSet &layerVertices)
#endif //#if USE_C__11
{
#if USE_C__11
	FrontElementHash layerFels;
	ElementHash layerEls;
#else
    FrontElementSet layerFels;
	ElementSet layerEls;
#endif //#if USE_C__11

	this->findAndRemoveLayer(vertices, layer, layerVertices, false, layerFels, false, layerEls);
}

#if USE_C__11
void Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer,
	VertexHash &layerVertices, FrontElementHash &layerFrontElements)
#else
void Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer,
	VertexSet &layerVertices, FrontElementSet &layerFrontElements)
#endif //#if USE_C__11
{
#if USE_C__11
	ElementHash layerEls;
#else
	ElementSet layerEls;
#endif //#if USE_C__11

	this->findAndRemoveLayer(vertices, layer, layerVertices, true, layerFrontElements, false, layerEls);
}

#if USE_C__11
void Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer,
	VertexHash &layerVertices, ElementHash &layerElements)
#else
void Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer,
	VertexSet &layerVertices, ElementSet &layerElements)
#endif //#if USE_C__11
{
#if USE_C__11
	FrontElementHash layerFels;
#else
	FrontElementSet layerFels;
#endif //#if USE_C__11

	this->findAndRemoveLayer(vertices, layer, layerVertices, false, layerFels, true, layerElements);
}

#if USE_C__11
void Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer,
	VertexHash &layerVertices, FrontElementHash &layerFrontElements,
	ElementHash &layerElements)
#else
void Data::Mesh::findAndRemoveLayer(const VertexList &vertices, UInt layer,
	VertexSet &layerVertices, FrontElementSet &layerFrontElements,
	ElementSet &layerElements)
#endif //#if USE_C__11
{
	this->findAndRemoveLayer(vertices, layer, layerVertices, true, layerFrontElements, true, layerElements);
}

#if USE_C__11
void Data::Mesh::findLayers(const VertexList &vertices, UInt layers,
	VertexHash &layerVertices,
	bool frontElements, FrontElementHash &layerFrontElements,
	bool elements, ElementHash &layerElements) const
#else
void Data::Mesh::findLayers(const VertexList &vertices, UInt layers,
	VertexSet &layerVertices,
	bool frontElements, FrontElementSet &layerFrontElements,
	bool elements, ElementSet &layerElements) const
#endif //#if USE_C__11
{
	if (layers == 0)
	{
		if ((!frontElements) && (!elements))
		{
			layerVertices.insert(vertices.begin(), vertices.end());

			return;
		}

		for (VertexList::const_iterator iter = vertices.begin();
			 iter != vertices.end(); iter++)
		{
			Vertex *v = (*iter);

			if (!layerVertices.insert(v).second)
			{
				continue;
			}

			if (frontElements)
			{
				FrontElementList fadjs = this->findAdjacentFrontElements(v);

				layerFrontElements.insert(fadjs.begin(), fadjs.end());
			}

			if (elements)
			{
				ElementList adjs = this->adjacency(v);

				layerElements.insert(adjs.begin(), adjs.end());
			}
		}

		return;
	}

#if USE_C__11
	VertexHash current(vertices.begin(), vertices.end());
	VertexHash previous, next;
#else
    VertexSet current(vertices.begin(), vertices.end());
	VertexSet previous, next;
#endif //#if USE_C__11

	for (UInt i = 0; i < layers; i++)
	{
#if USE_C__11
		for (VertexHash::iterator iter = current.begin();
#else
		for (VertexSet::iterator iter = current.begin();
#endif //#if USE_C__11
			 iter != current.end(); iter++)
		{
			Vertex *v = (*iter);

			if (frontElements)
			{
				FrontElementList fadjs = this->findAdjacentFrontElements(v);

				layerFrontElements.insert(fadjs.begin(), fadjs.end());
			}

			ElementList adjs = this->adjacency(v);

			for (ElementList::iterator iter2 = adjs.begin();
				 iter2 != adjs.end(); iter2++)
			{
				Element *e = (*iter2);

				if (elements)
				{
					layerElements.insert(e);
				}

				for (UInt i = 0; i < e->numVertices(); i++)
				{
					Vertex *adj = e->getVertex(i);

					if ((adj == v) ||
						(current.find(adj) != current.end()) ||
						(previous.find(adj) != previous.end()) ||
						(!next.insert(adj).second))
					{
						continue;
					}

					layerVertices.insert(adj);
				}
			}
		}

		previous.clear();

		previous.swap(current);

		current.swap(next);
	}

	if ((!frontElements) && (!elements))
	{
		return;
	}

#if USE_C__11
	for (VertexHash::iterator iter = current.begin();
#else
	for (VertexSet::iterator iter = current.begin();
#endif //#if USE_C__11
		 iter != current.end(); iter++)
	{
		Vertex *v = (*iter);

		if (frontElements)
		{
			FrontElementList fadjs = this->findAdjacentFrontElements(v);

			layerFrontElements.insert(fadjs.begin(), fadjs.end());
		}

		if (elements)
		{
			ElementList adjs = this->adjacency(v);

			layerElements.insert(adjs.begin(), adjs.end());
		}
	}
}

#if USE_C__11
VertexHash Data::Mesh::findLayers(const VertexList &vertices, UInt layers) const
#else
VertexSet Data::Mesh::findLayers(const VertexList &vertices, UInt layers) const
#endif //#if USE_C__11
{
#if USE_C__11
	VertexHash layerVertices;
#else
	VertexSet layerVertices;
#endif //#if USE_C__11

	this->findLayers(vertices, layers, layerVertices);

	return layerVertices;
}

#if USE_C__11
void Data::Mesh::findLayers(const VertexList &vertices, UInt layers,
	VertexHash &layerVertices) const
#else
void Data::Mesh::findLayers(const VertexList &vertices, UInt layers,
	VertexSet &layerVertices) const
#endif //#if USE_C__11
{
#if USE_C__11
	FrontElementHash layerFels;
	ElementHash layerEls;
#else
	FrontElementSet layerFels;
	ElementSet layerEls;
#endif //#if USE_C__11

	this->findLayers(vertices, layers, layerVertices, false, layerFels, false, layerEls);
}

#if USE_C__11
void Data::Mesh::findLayers(const VertexList &vertices, UInt layers,
	VertexHash &layerVertices, FrontElementHash &layerFrontElements) const
#else
void Data::Mesh::findLayers(const VertexList &vertices, UInt layers,
	VertexSet &layerVertices, FrontElementSet &layerFrontElements) const
#endif //#if USE_C__11
{
#if USE_C__11
	ElementHash layerEls;
#else
	ElementSet layerEls;
#endif //#if USE_C__11

	this->findLayers(vertices, layers, layerVertices, true, layerFrontElements, false, layerEls);
}

#if USE_C__11
void Data::Mesh::findLayers(const VertexList &vertices, UInt layers,
	VertexHash &layerVertices, ElementHash &layerElements) const
#else
void Data::Mesh::findLayers(const VertexList &vertices, UInt layers,
	VertexSet &layerVertices, ElementSet &layerElements) const
#endif //#if USE_C__11
{
#if USE_C__11
	FrontElementHash layerFels;
#else
	FrontElementSet layerFels;
#endif //#if USE_C__11

	this->findLayers(vertices, layers, layerVertices, false, layerFels, true, layerElements);
}

#if USE_C__11
void Data::Mesh::findLayers(const VertexList &vertices, UInt layers,
	VertexHash &layerVertices, FrontElementHash &layerFrontElements,
	ElementHash &layerElements) const
#else
void Data::Mesh::findLayers(const VertexList &vertices, UInt layers,
	VertexSet &layerVertices, FrontElementSet &layerFrontElements,
	ElementSet &layerElements) const
#endif //#if USE_C__11
{
	this->findLayers(vertices, layers, layerVertices, true, layerFrontElements, true, layerElements);
}

#if USE_C__11
void Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers,
	VertexHash &layerVertices,
	bool frontElements, FrontElementHash &layerFrontElements,
	bool elements, ElementHash &layerElements)
#else
void Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers,
	VertexSet &layerVertices,
	bool frontElements, FrontElementSet &layerFrontElements,
	bool elements, ElementSet &layerElements)
#endif //#if USE_C__11
{
	if (layers == 0)
	{
		for (VertexList::const_iterator iter = vertices.begin();
			 iter != vertices.end(); iter++)
		{
			Vertex *v = (*iter);

			if (!layerVertices.insert(v).second)
			{
				continue;
			}

			this->remove(v);

			if (frontElements)
			{
				FrontElementList fadjs = this->findAdjacentFrontElements(v);

				while (!fadjs.empty())
				{
					Data::FrontElement *fel = fadjs.front();

					fadjs.pop_front();

					if (layerFrontElements.insert(fel).second)
					{
						this->remove(fel);
					}
				}
			}

			if (elements)
			{
				ElementList adjs = this->adjacency(v);

				while (!adjs.empty())
				{
					Data::Element *el = adjs.front();

					adjs.pop_front();

					if (layerElements.insert(el).second)
					{
						this->remove(el);
					}
				}
			}
		}

		return;
	}

#if USE_C__11
	VertexHash current(vertices.begin(), vertices.end());
	VertexHash previous, next;
#else
	VertexSet current(vertices.begin(), vertices.end());
	VertexSet previous, next;
#endif //#if USE_C__11

	for (UInt i = 0; i < layers; i++)
	{
#if USE_C__11
		for (VertexHash::iterator iter = current.begin();
#else
		for (VertexSet::iterator iter = current.begin();
#endif //#if USE_C__11
			 iter != current.end(); iter++)
		{
			Vertex *v = (*iter);

			this->remove(v);

			if (frontElements)
			{
				FrontElementList fadjs = this->findAdjacentFrontElements(v);

				while (!fadjs.empty())
				{
					Data::FrontElement *fel = fadjs.front();

					fadjs.pop_front();

					if (layerFrontElements.insert(fel).second)
					{
						this->remove(fel);
					}
				}
			}

			ElementList adjs = this->adjacency(v);

			for (ElementList::iterator iter2 = adjs.begin();
				 iter2 != adjs.end(); iter2++)
			{
				Element *e = (*iter2);

				if (elements)
				{
					if (layerElements.insert(e).second)
					{
						this->remove(e);
					}
				}

				for (UInt i = 0; i < e->numVertices(); i++)
				{
					Vertex *adj = e->getVertex(i);

					if ((adj == v) ||
						(current.find(adj) != current.end()) ||
						(previous.find(adj) != previous.end()) ||
						(!next.insert(adj).second))
					{
						continue;
					}

					layerVertices.insert(adj);
				}
			}
		}

		previous.clear();

		previous.swap(current);

		current.swap(next);
	}

#if USE_C__11
	for (VertexHash::iterator iter = current.begin();
#else
	for (VertexSet::iterator iter = current.begin();
#endif //#if USE_C__11
		 iter != current.end(); iter++)
	{
		Vertex *v = (*iter);

		this->remove(v);

		if (frontElements)
		{
			FrontElementList fadjs = this->findAdjacentFrontElements(v);

			while (!fadjs.empty())
			{
				Data::FrontElement *fel = fadjs.front();

				fadjs.pop_front();

				if (layerFrontElements.insert(fel).second)
				{
					this->remove(fel);
				}
			}
		}

		if (elements)
		{
			ElementList adjs = this->adjacency(v);

			while (!adjs.empty())
			{
				Data::Element *el = adjs.front();

				adjs.pop_front();

				if (layerElements.insert(el).second)
				{
					this->remove(el);
				}
			}
		}
	}
}

#if USE_C__11
VertexHash Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers)
#else
VertexSet Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers)
#endif //#if USE_C__11
{
#if USE_C__11
	VertexHash layerVertices;
#else
	VertexSet layerVertices;
#endif //#if USE_C__11

	this->findAndRemoveLayers(vertices, layers, layerVertices);

	return layerVertices;
}

#if USE_C__11
void Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers,
	VertexHash &layerVertices)
#else
void Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers,
	VertexSet &layerVertices)
#endif //#if USE_C__11
{
#if USE_C__11
	FrontElementHash layerFels;
	ElementHash layerEls;
#else
	FrontElementSet layerFels;
	ElementSet layerEls;
#endif //#if USE_C__11

	this->findAndRemoveLayers(vertices, layers, layerVertices, false, layerFels, false, layerEls);
}

#if USE_C__11
void Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers,
	VertexHash &layerVertices, FrontElementHash &layerFrontElements)
#else
void Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers,
	VertexSet &layerVertices, FrontElementSet &layerFrontElements)
#endif //#if USE_C__11
{
#if USE_C__11
	ElementHash layerEls;
#else
	ElementSet layerEls;
#endif //#if USE_C__11

	this->findAndRemoveLayers(vertices, layers, layerVertices, true, layerFrontElements, false, layerEls);
}

#if USE_C__11
void Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers,
	VertexHash &layerVertices, ElementHash &layerElements)
#else
void Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers,
	VertexSet &layerVertices, ElementSet &layerElements)
#endif //#if USE_C__11
{
#if USE_C__11
	FrontElementHash layerFels;
#else
	FrontElementSet layerFels;
#endif //#if USE_C__11

	this->findAndRemoveLayers(vertices, layers, layerVertices, false, layerFels, true, layerElements);
}

#if USE_C__11
void Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers,
	VertexHash &layerVertices, FrontElementHash &layerFrontElements,
	ElementHash &layerElements)
#else
void Data::Mesh::findAndRemoveLayers(const VertexList &vertices, UInt layers,
	VertexSet &layerVertices, FrontElementSet &layerFrontElements,
	ElementSet &layerElements)
#endif //#if USE_C__11
{
	this->findAndRemoveLayers(vertices, layers, layerVertices, true, layerFrontElements, true, layerElements);
}

#if USE_GUI
void Data::Mesh::draw() const
{
	/*if (this->frontElements.empty())
	{
		//for (ElementList::const_iterator iter = this->elements.begin();
		for (ElementSet::const_iterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			if (!this->isHighlighted())
			{
				(*iter)->draw(false);

				continue;
			}
		}
	}
	else
	{
		//for (ElementList::const_iterator iter = this->elements.begin();
		for (ElementSet::const_iterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			(*iter)->draw();
		}

		//for (FrontElementList::const_iterator iter = this->frontElements.begin();
		for (FrontElementSet::const_iterator iter = this->frontElements.begin();
			 iter != this->frontElements.end(); iter++)
		{
			if (!this->isHighlighted())
			{
				(*iter)->draw(true);

				continue;
			}

			bool highlighted = (*iter)->getShape()->isHighlighted();
			Real r, g, b;

			if (!highlighted)
			{
				(*iter)->highlight();
			}

			r = (*iter)->getR();
			g = (*iter)->getG();
			b = (*iter)->getB();

			(*iter)->setColor(this->getR(), this->getG(), this->getB());

			(*iter)->draw(true);

			(*iter)->setColor(r, g, b);

			(*iter)->unhighlight();

			(*iter)->draw(false);

			if (highlighted)
			{
				(*iter)->highlight();
			}
		}
	}*/

	if (!this->hasLists())
	{
		this->createList(2);
	}

	if (this->hasList())
	{
		this->drawList();

		return;
	}

	this->beginList();

	if (Data::getState().isElementDrawn())
	{
		for (ConstElementIterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				// (*iter)->draw(this->isHighlighted());
				(*iter)->draw();
			}
		}
	}

	if (Data::getState().isFrontElementDrawn())
	{
		for (ConstFrontElementIterator iter = this->frontElements.begin();
			 iter != this->frontElements.end(); iter++)
		{
			//if ((*iter)->getShape()->isHighlighted())
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->draw();
			}
		}
	}

	if (Data::getState().isVertexDrawn())
	{
		for (ConstVertexIterator iter = this->vertices.begin();
			 iter != this->vertices.end(); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getPoint()))
			{
				(*iter)->draw();
			}
		}
	}

	this->endList();
}

void Data::Mesh::drawPick() const
{
	if (Data::getState().isElementDrawn())
	{
		for (ConstElementIterator iter = this->elements.begin();
			 iter != this->elements.end(); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawPick();
			}
		}
	}

	if (Data::getState().isFrontElementDrawn())
	{
		for (ConstFrontElementIterator iter = this->frontElements.begin();
			 iter != this->frontElements.end(); iter++)
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
			 iter != this->vertices.end(); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getPoint()))
			{
				(*iter)->drawPick();
			}
		}
	}
}

void Data::Mesh::drawNormals(Real factor) const
{
	if (!this->hasLists())
	{
		this->createList(2);
	}

	if (this->hasList(1))
	{
		this->drawList(1);

		return;
	}

	this->beginList(1);

	if (Data::getState().isFrontElementDrawn())
	{
		for (ConstFrontElementIterator iter = this->frontElements.begin();
			 iter != this->frontElements.end(); iter++)
		{
			//if (Data::getState().isInVisibleSpace((*iter)->getShape()))
			{
				(*iter)->drawNormal(factor);
			}
		}
	}

	this->endList();
}
#endif //#if USE_GUI
std::string Data::Mesh::text() const
{
	std::stringstream str;

	//for (VertexList::const_iterator iter = this->vertices.begin();
	for (ConstVertexIterator iter = this->vertices.begin();
		 iter != this->vertices.end(); iter++)
	{
		str << (this->haveInner((*iter)) ? "i" : "") << "v " << (*iter)->text() << std::endl;
	}

	for (ConstFrontElementIterator iter = this->frontElements.begin();
		 iter != this->frontElements.end(); iter++)
	{
		str << "fel " << (*iter)->text() << std::endl;
	}

	for (ConstElementIterator iter = this->elements.begin();
		 iter != this->elements.end(); iter++)
	{
		str << "e " << (*iter)->text() << std::endl;
	}

	return str.str();
}
//#endif //#if USE_GUI
