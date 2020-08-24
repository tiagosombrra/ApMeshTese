#include "Data/Drawable.h"

#if USE_GUI

#include "Data/Point.h"
#include "Data/State.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Element.h"

using namespace Data;

Data::Drawable::Drawable()
{

}

Data::Drawable::~Drawable()
{

}

void Data::Drawable::setColor(Real r, Real g, Real b) const
{
	this->setR(r);
	this->setG(g);
	this->setB(b);
}

void Data::Drawable::getColor(Real &r, Real &g, Real &b) const
{
	r = this->getR();
	g = this->getG();
	b = this->getB();
}

void Data::Drawable::draw(Real r, Real g, Real b) const
{
	Real orr = this->getR();
	Real org = this->getG();
	Real orb = this->getB();

	this->setColor(r, g, b);

	this->draw();

	this->setColor(orr, org, orb);
}

void Data::Drawable::drawPick() const
{
	Name name;

	name.ptr = const_cast<Drawable *>(this);

	for (UInt i = 0; i < this->numElems; i++)
	{
		glPushName(this->useUints ? name.uints[i]: name.bytes[i]);
	}

	this->draw();

	for (UInt i = 0; i < this->numElems; i++)
	{
		glPopName();
	}
}

DrawableList Data::Drawable::picked(GLint numHits, GLuint buffer[]) const
{
	DrawableMultiMap sorted;

	GLuint *ptr = &(buffer[0]);

	for (GLint i = 0; i < numHits; i++)
	{
		GLuint names = *ptr;

		ptr++;

		GLuint z = (*ptr);

		ptr++;
		ptr++;

		for (GLuint j = 0; static_cast<GLuint>(this->numElems) + j < names; j++)
		{
			ptr++;
		}

		Name name;

		for (GLuint j = 0; j < this->numElems; j++, ptr++)
		{
			if (this->useUints)
			{
				name.uints[j] = static_cast<GLuint>(*ptr);
			}
			else
			{
				name.bytes[j] = static_cast<GLbyte>(*ptr);
			}
		}

		Drawable *obj = name.ptr;

		if (((!Data::getState().isVertexPickable()) && (dynamic_cast<Vertex *>(obj))) ||
			((!Data::getState().isFrontElementPickable()) && (dynamic_cast<FrontElement *>(obj))) ||
			((!Data::getState().isElementPickable()) && (dynamic_cast<Element *>(obj)) && (!dynamic_cast<FrontElement *>(obj))))
		{
			continue;
		}

		bool found = false;

		std::pair<DrawableMultiMap::iterator, DrawableMultiMap::iterator> p = sorted.equal_range(z);

		for (DrawableMultiMap::iterator iter = p.first; iter != p.second; iter++)
		{
			if ((*iter).second == obj)
			{
				found = true;

				break;
			}
		}

		if (!found)
		{
			sorted.insert(std::make_pair(z, obj));
		}
	}

	DrawableList shapes;

	for (DrawableMultiMap::iterator iter = sorted.begin();
		 iter != sorted.end(); iter++)
	{
		shapes.push_back((*iter).second);
	}

	return shapes;
}

UInt Data::Drawable::fill(GLfloat */*coord*/, GLfloat */*color*/, GLfloat */*normal*/) const
{
	return 0;
}

UInt Data::Drawable::fill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList */*adjacency*/) const
{
	return this->fill(coord, color, normal);
}

UInt Data::Drawable::qfill(GLfloat */*coord*/, GLfloat */*color*/, GLfloat */*normal*/) const
{
	return 0;
}

UInt Data::Drawable::qfill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList */*adjacency*/) const
{
	return this->qfill(coord, color, normal);
}

UInt Data::Drawable::tfill(GLfloat */*coord*/, GLfloat */*color*/, GLfloat */*normal*/) const
{
	return 0;
}

UInt Data::Drawable::tfill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList */*adjacency*/) const
{
	return this->tfill(coord, color, normal);
}

void Data::Drawable::explodeAndShrink(const Point &mid, bool point, bool vector) const
{
	this->explodeAndShrink(&mid, point, vector);
}

void Data::Drawable::explodeAndShrink(const Point *mid, bool point, bool vector) const
{
	//Point gmid = Data::getState().getMid();
	Real invex = 1.0 - Data::getState().getExplode();
	Real invexExt = 1.0 - Data::getState().getExplodeExternal();

	Real x = mid->getCoord(0);
	Real y = mid->getCoord(1);
	Real z = (mid->dimension() == 2) ? 0.0 : mid->getCoord(2);

	Real xe = Data::getState().getMidExternal().getCoord(0);
	Real ye = Data::getState().getMidExternal().getCoord(1);
	Real ze = Data::getState().getMidExternal().getCoord(2);

	glTranslate(Data::getState().getMid().getCoord(0)*invex*invexExt + x*Data::getState().getExplode() + xe*Data::getState().getExplodeExternal(),
				Data::getState().getMid().getCoord(1)*invex*invexExt + y*Data::getState().getExplode() + ye*Data::getState().getExplodeExternal(),
				Data::getState().getMid().getCoord(2)*invex*invexExt + z*Data::getState().getExplode() + ze*Data::getState().getExplodeExternal());

	if (point)
	{
	    glTranslate(-xe, -ye, -ze);

		return;
	}

	glScale(Data::getState().getShrink(), Data::getState().getShrink(), Data::getState().getShrink());

	if (vector)
	{
	    glTranslate(-xe, -ye, -ze);

		return;
	}

	glTranslate(-x - xe, -y - ye, -z - ze);
}

void Data::Drawable::explodeAndShrink(GLfloat *coord, const Point &mid, bool point, bool vector) const
{
	bool third = (mid.dimension() >= 3);

	coord[0] -= static_cast<GLfloat>(Data::getState().getMidExternal().getCoord(0));
	coord[1] -= static_cast<GLfloat>(Data::getState().getMidExternal().getCoord(1));
    if (third) coord[2] -= static_cast<GLfloat>(Data::getState().getMidExternal().getCoord(2));

	if (!point)
	{
		if (!vector)
		{
			coord[0] -= static_cast<GLfloat>(mid.getCoord(0));
			coord[1] -= static_cast<GLfloat>(mid.getCoord(1));
			if (third) coord[2] -= static_cast<GLfloat>(mid.getCoord(2));
		}

		coord[0] *= static_cast<GLfloat>(Data::getState().getShrink());
		coord[1] *= static_cast<GLfloat>(Data::getState().getShrink());
		if (third) coord[2] *= static_cast<GLfloat>(Data::getState().getShrink());
	}

	//Point gmid = Data::getState().getMid();
	Real invex = 1.0 - Data::getState().getExplode();
	Real invexExt = 1.0 - Data::getState().getExplodeExternal();

	coord[0] += static_cast<GLfloat>(Data::getState().getMid().getCoord(0)*invex*invexExt + mid.getCoord(0)*Data::getState().getExplode() + Data::getState().getMidExternal().getCoord(0)*Data::getState().getExplodeExternal());
	coord[1] += static_cast<GLfloat>(Data::getState().getMid().getCoord(1)*invex*invexExt + mid.getCoord(1)*Data::getState().getExplode() + Data::getState().getMidExternal().getCoord(1)*Data::getState().getExplodeExternal());
	if (third) coord[2] += static_cast<GLfloat>(Data::getState().getMid().getCoord(2)*invex*invexExt + mid.getCoord(2)*Data::getState().getExplode() + Data::getState().getMidExternal().getCoord(2)*Data::getState().getExplodeExternal());
}

#endif //#if USE_GUI
