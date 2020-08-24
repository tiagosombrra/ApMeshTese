#include "Data/Element.h"

#include "Data/State.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace Data;

Data::Element::Element(GeometricShape *shape, ULInt id) :
	Identifiable(id)
#if USE_GUI
	,  Drawable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->vertices = NULL;
	this->numVerts = 0;
#endif //#if USE_ARRAY

	this->setShape(shape);

	//this->setId(id);
}

Data::Element::~Element()
{
	//this->vertices.clear();

	delete this->shape;

#if USE_ARRAY
	if (this->vertices)
	{
		delete [] this->vertices;
	}
#endif //#if USE_ARRAY
}

void Data::Element::setShape(GeometricShape *shape)
{
	this->shape = shape;
}

GeometricShape *Data::Element::getShape() const
{
	return this->shape;
}

UInt Data::Element::numVertices() const
{
#if USE_ARRAY
	return this->numVerts;
#else
	return this->vertices.size();
#endif //#if USE_ARRAY
}

VertexVector Data::Element::getVertices() const
{
#if USE_ARRAY
	return this->vertices ? VertexVector(this->vertices, this->vertices + this->numVerts) : VertexVector();
#else
	return this->vertices;
#endif //#if USE_ARRAY
}

Vertex *const *Data::Element::getVerticesArray() const
{
#if USE_ARRAY
	return this->vertices;
#else
	return &this->vertices[0];
#endif //#if USE_ARRAY
}

void Data::Element::setVertex(UInt i, Vertex *v)
{
	//if (i < this->vertices.size())
	{
		this->vertices[i] = v;

		if (this->shape)
		{
			this->shape->setPoint(i, (v) ? v->getPoint() : NULL);
		}
	}
}

UInt Data::Element::index(const Vertex *v) const
{
    for (UInt i = 0; i < this->numVertices(); i++)
	{
		if (this->getVertex(i) == v)
		{
			return i;
		}
	}

	return this->numVertices();
}

bool Data::Element::replace(Vertex *oldVertex, Vertex *newVertex)
{
	UInt i = this->index(oldVertex);

	if (i == this->numVertices())
    {
        return false;
    }

    this->setVertex(i, newVertex);

    return true;
}

Data::Vertex *Data::Element::getVertex(UInt i) const
{
	//return (i < this->numVertices()) ? this->vertices[i] : NULL;
	return this->vertices[i];
}

void Data::Element::clear()
{
#if USE_ARRAY
	delete [] this->vertices;

	this->vertices = NULL;

	this->numVerts = 0;
#else
	this->vertices.clear();
#endif //#if USE_ARRAY

	for (UInt i = 0; i < shape->numPoints(); i++)
	{
		this->shape->setPoint(i, NULL);
	}
}

bool Data::Element::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::Element::in(const Point *p) const
{
	return this->shape ? this->shape->in(p) : false;
}

bool Data::Element::in(const Vertex &v) const
{
	return this->in(&v);
}

bool Data::Element::in(const Vertex *v) const
{
	return this->in(v->getPoint());
}

bool Data::Element::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::Element::on(const Point *p) const
{
	return this->shape ? this->shape->on(p) : false;
}

bool Data::Element::on(const Vertex &v) const
{
	return this->on(&v);
}

bool Data::Element::on(const Vertex *v) const
{
	return this->on(v->getPoint());
}

bool Data::Element::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::Element::out(const Point *p) const
{
	return this->shape ? this->shape->out(p) : false;
}

bool Data::Element::out(const Vertex &v) const
{
	return this->out(&v);
}

bool Data::Element::out(const Vertex *v) const
{
	return this->out(v->getPoint());
}

bool Data::Element::haveMatching(const Point &p) const
{
    return this->shape ? this->shape->haveMatching(p) : false;
}

bool Data::Element::have(const Point &p) const
{
	return this->shape ? this->shape->have(p) : false;
}

bool Data::Element::have(const Point *p) const
{
	return this->shape ? this->shape->have(p) : false;
}

bool Data::Element::have(const Vertex &v) const
{
	return this->have(*v.getPoint());
}

bool Data::Element::have(const Vertex *v) const
{
#if USE_ARRAY
	for (UInt i = 0; i < this->numVertices(); i++)
	{
		if (this->vertices[i] == v)
		{
			return true;
		}
	}

	return false;
#else
	return this->vertices.empty() ? false : std::find(this->vertices.begin(), this->vertices.end(), v) != this->vertices.end();
#endif //#if USE_ARRAY
}

void Data::Element::resize(UInt numVertices)
{
#if USE_ARRAY
	if (numVertices != this->numVerts)
	{
		Vertex **vertices = this->vertices;

		this->vertices = new Vertex*[numVertices];

		UInt min = std::min(numVertices, this->numVerts);

		if (vertices)
		{
			for (UInt i = 0; i < min; i++)
			{
				this->vertices[i] = vertices[i];
			}

			delete [] vertices;
		}

		for (UInt i = min; i < numVertices; i++)
		{
			this->vertices[i] = NULL;
		}

		this->numVerts = numVertices;
	}
#else
	this->vertices.resize(numVertices, NULL);
#endif //#if USE_ARRAY
}

#if USE_ARRAY
Element &Data::Element::operator=(const Element &e)
{
	if (this != &e)
	{
		this->id = e.id;

		if (this->numVerts != e.numVerts)
		{
			if (this->vertices)
			{
				delete [] this->vertices;
			}

			this->numVerts = e.numVerts;

			this->vertices = new Vertex*[this->numVerts];
		}

		/*if (this->vertices)
		{
			delete [] this->vertices;
		}

		this->numVerts = e.numVerts;

		this->vertices = new Vertex*[this->numVerts];*/

		for (UInt i = 0; i < this->numVerts; i++)
		{
			this->vertices[i] = e.vertices[i];
		}

#if USE_GUI
		this->setColor(e.getR(), e.getG(), e.getB());
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_ARRAY

#if USE_GUI
void Data::Element::setR(Real r) const
{
	if (this->shape)
	{
		this->shape->setR(r);
	}
}

Real Data::Element::getR() const
{
	return (this->shape) ? this->shape->getR() : 0.0;
}

void Data::Element::setG(Real g) const
{
	if (this->shape)
	{
		this->shape->setG(g);
	}
}

Real Data::Element::getG() const
{
	return (this->shape) ? this->shape->getG() : 0.0;
}

void Data::Element::setB(Real b) const
{
	if (this->shape)
	{
		this->shape->setB(b);
	}
}

Real Data::Element::getB() const
{
	return (this->shape) ? this->shape->getB() : 0.0;
}

void Data::Element::highlight() const
{
	if (this->shape)
	{
		this->shape->highlight();
	}
}

void Data::Element::unhighlight() const
{
	if (this->shape)
	{
		this->shape->unhighlight();
	}
}

bool Data::Element::isHighlighted() const
{
	return (this->shape) ? this->shape->isHighlighted() : false;
}

/*void Data::Element::draw(bool fill) const
{
	if (this->shape)
	{
		this->shape->draw(fill);
	}
}*/

void Data::Element::draw() const
{
	//this->draw(true);
	this->shape->draw();

	this->drawId();
}

void Data::Element::drawId() const
{
	this->drawId(this->getR(), this->getG(), this->getB());
}

void Data::Element::drawId(Real r, Real g, Real b) const
{
	if (!Data::getState().isIdDrawn())
	{
		return;
	}

	if (!Data::getState().isInVisibleSpace(this->shape))
	{
		return;
	}

	glPushMatrix();
		/*GLboolean logic = 0, lighting = 0;

		glGetBooleanv(GL_COLOR_LOGIC_OP, &logic);
		glGetBooleanv(GL_LIGHTING, &lighting);

		if (lighting)
		{
			glDisable(GL_LIGHTING);
		}

		if (!logic)
		{
			glEnable(GL_COLOR_LOGIC_OP);
		}

		GLint opcode = GL_COPY;

		glGetIntegerv(GL_LOGIC_OP_MODE, &opcode);

		glLogicOp(GL_XOR);*/

		glColor(r, g, b);

#if USE_EXPERIMENTAL_3
        if (this->shape->dimension() == 2)
        {
            Point2D mid = this->shape->mid2D();

            this->explodeAndShrink(mid);

            glTextId(mid.getCoord(0), mid.getCoord(1), 0.0, this->getId());
        }
        else
        {
            Point3D mid = this->shape->mid3D();

            this->explodeAndShrink(mid);

            glTextId(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2), this->getId());
        }
#else
		Point mid = this->shape->mid();

		this->explodeAndShrink(mid);

		glTextId(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2), this->getId());
#endif //#if USE_EXPERIMENTAL_3

		/*glLogicOp(opcode);

		if (!logic)
		{
			glDisable(GL_COLOR_LOGIC_OP);
		}

		if (lighting)
		{
			glEnable(GL_LIGHTING);
		}*/
	glPopMatrix();
}
#endif //#if USE_GUI

std::string Data::Element::text() const
{
    return this->text(NULL);
}

std::string Data::Element::text(const Data::AbstractMesh */*mesh*/) const
{
	if (!this->shape)
	{
		return "-1";
	}

	std::stringstream str;
	//str.precision(30);

	str << this->getId();

	for (UInt i = 0; i < this->numVertices(); i++)
	{
		str << " ";

		if (this->vertices[i])
		{
			str << this->vertices[i]->getId();
		}
		else
		{
			str << "-1";
		}
	}

	if (this->shape)
	{
		UInt npts = this->shape->numPoints();

		if (npts > 0)
		{
			bool allExist = true;

			for (UInt i = 0; i < npts; i++)
			{
				if (!this->shape->getPoint(i))
				{
					allExist = false;

					break;
				}
			}

			if (allExist)
			{
				str << " size = " << this->shape->size();
			}
		}
	}

	return str.str();
}
