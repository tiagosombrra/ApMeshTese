#include "Data/Vertex.h"

#include "Data/Edge.h"
#include "Data/State.h"

using namespace Data;

Data::Vertex::Vertex(Point *p, ULInt id) :
	Identifiable(id)
#if USE_GUI
	, Drawable()
#endif //#if USE_GUI
{
	this->setPoint(p);
}

Data::Vertex::Vertex::~Vertex()
{
	/*//debug
	if (this->id > 0)
	{
		std::stringstream str;
		str << "deleting at " << Data::trace();
		str << "deleting vertex at " << this << std::endl;
		str << "deleting vertex " << this->text() << std::endl;
		std::cout << str.str();
	}
	//endebug*/

	/*while (!this->adjacency.empty())
	{
		Edge *e = this->adjacency.back();
		this->adjacency.pop_back();

		if (e->getP1() == this->point)
		{
			e->setP1(NULL);
		}
		else if (e->getP2() == this->point)
		{
			e->setP2(NULL);
		}
	}*/

	if (this->point)
	{
		delete this->point;
	}
}

void Data::Vertex::setPoint(Point *p)
{
	this->point = p;
}

Point *Data::Vertex::getPoint() const
{
	return this->point;
}

/*const EdgeList &Data::Vertex::getAdjacency() const
{
	return this->adjacency;
}

void Data::Vertex::add(Edge *e)
{
	this->adjacency.push_back(e);
}

void Data::Vertex::remove(Edge *e)
{
	this->adjacency.remove(e);
}*/

#if USE_GUI
void Data::Vertex::setR(Real r) const
{
	if (this->point)
	{
		this->point->setR(r);
	}
}

Real Data::Vertex::getR() const
{
	return (this->point) ? this->point->getR() : 0.0;
}

void Data::Vertex::setG(Real g) const
{
	if (this->point)
	{
		this->point->setG(g);
	}
}

Real Data::Vertex::getG() const
{
	return (this->point) ? this->point->getG() : 0.0;
}

void Data::Vertex::setB(Real b) const
{
	if (this->point)
	{
		this->point->setB(b);
	}
}

Real Data::Vertex::getB() const
{
	return (this->point) ? this->point->getB() : 0.0;
}

void Data::Vertex::highlight() const
{
	if (this->point)
	{
		this->point->highlight();
	}

	/*for (EdgeList::const_iterator iter = this->adjacency.begin();
		 iter != this->adjacency.end(); iter++)
	{
		(*iter)->highlight();
	}*/
}

void Data::Vertex::unhighlight() const
{
	if (this->point)
	{
		this->point->unhighlight();
	}

	/*for (EdgeList::const_iterator iter = this->adjacency.begin();
		 iter != this->adjacency.end(); iter++)
	{
		(*iter)->unhighlight();
	}*/
}

bool Data::Vertex::isHighlighted() const
{
	return (this->point) ? this->point->isHighlighted() : false;
}

void Data::Vertex::draw() const
{
	if (!this->point)
	{
		return;
	}

	this->point->draw();

	this->drawId();
}

void Data::Vertex::drawId() const
{
	this->drawId(this->getR(), this->getG(), this->getB());
}

void Data::Vertex::drawId(Real r, Real g, Real b) const
{
	if (!Data::getState().isIdDrawn())
	{
		return;
	}

	if (!Data::getState().isInVisibleSpace(this->point))
	{
		return;
	}

	glPushMatrix();
		this->explodeAndShrink(this->point, true);

		/*GLboolean logic = 0, lighting;

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

		glTextId(0.0, 0.0, 0.0, this->getId());

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

std::string Data::Vertex::text() const
{
    return this->text(NULL);
}

std::string Data::Vertex::text(const Data::AbstractMesh */*mesh*/) const
{
	std::stringstream str;

	str << this->getId();

	if (this->point)
	{
#if USE_GUI
		str << this->point->text();
#else
		for (UInt i = 0; i < this->point->dimension(); i++)
		{
			str << " " << this->point->getCoord(i);
		}
#endif //#if USE_GUI
	}
	else
	{
		str << " no point";
	}

	return str.str();
}
