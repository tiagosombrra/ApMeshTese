#include "TecGraf/OrientedSurface.h"

#if USE_TECGRAF

#include "TecGraf/Surface.h"

using namespace TecGraf;

TecGraf::OrientedSurface::OrientedSurface()
#if USE_GUI
	: Data::ListDisplayable::ListDisplayable()
#endif //#if USE_GUI
{
	this->surface = NULL;
	this->positive = true;
}

TecGraf::OrientedSurface::~OrientedSurface()
{

}

void TecGraf::OrientedSurface::setSurface(Surface *surface)
{
	this->surface = surface;
}

Surface *TecGraf::OrientedSurface::getSurface() const
{
	return this->surface;
}

void TecGraf::OrientedSurface::setPositive(bool positive)
{
	this->positive = positive;
}

bool TecGraf::OrientedSurface::isPositive() const
{
	return this->positive;
}

bool TecGraf::OrientedSurface::isNegative() const
{
	return !this->isPositive();
}

void TecGraf::OrientedSurface::setOrientation(Int orientation)
{
	this->setPositive(orientation >= 0);
}

Int TecGraf::OrientedSurface::getOrientation() const
{
	return this->isPositive() ? 1 : -1;
}

void TecGraf::OrientedSurface::setId(ULInt id)
{
	if (this->surface)
	{
		this->surface->setId(id);
	}
}

ULInt TecGraf::OrientedSurface::getId() const
{
	return (this->surface) ? this->surface->getId() : 0;
}

#if USE_GUI
#include "Data/State.h"

void TecGraf::OrientedSurface::setR(Real r) const
{
	if (this->surface)
	{
		this->surface->setR(r);
	}
}

Real TecGraf::OrientedSurface::getR() const
{
	return this->surface ? this->surface->getR() : 0.0;
}

void TecGraf::OrientedSurface::setG(Real g) const
{
	if (this->surface)
	{
		this->surface->setG(g);
	}
}

Real TecGraf::OrientedSurface::getG() const
{
	return this->surface ? this->surface->getG() : 0.0;
}

void TecGraf::OrientedSurface::setB(Real b) const
{
	if (this->surface)
	{
		this->surface->setB(b);
	}
}

Real TecGraf::OrientedSurface::getB() const
{
	return this->surface ? this->surface->getB() : 0.0;
}

void TecGraf::OrientedSurface::highlight() const
{
	if (this->surface)
	{
		this->surface->highlight();
	}
}

void TecGraf::OrientedSurface::unhighlight() const
{
	if (this->surface)
	{
		this->surface->unhighlight();
	}
}

bool TecGraf::OrientedSurface::isHighlighted() const
{
	return this->surface ? this->surface->isHighlighted() : false;
}

void TecGraf::OrientedSurface::clearLists() const
{
	Data::ListDisplayable::clearLists();

	this->surface->clearLists();
}

void TecGraf::OrientedSurface::draw(bool lighting) const
{
	/*if (!this->hasLists())
	{
		this->createList(1);
	}

	if (this->hasList())
	{
		this->drawList();

		return;
	}

	this->beginList();*/

	if (this->surface)
	{
		if (this->isPositive())
		{
			this->surface->draw(lighting);
		}
		else
		{
			for (Data::Boundary::FrontElementIterator iter = this->surface->eBegin();
				 iter != this->surface->eEnd(); iter++)
			{
				(*iter)->invert();
			}

			this->surface->outdate();
			this->surface->clearLists();

			this->surface->draw(lighting);

			for (Data::Boundary::FrontElementIterator iter = this->surface->eBegin();
				 iter != this->surface->eEnd(); iter++)
			{
				(*iter)->invert();
			}

			this->surface->outdate();
			this->surface->clearLists();

			Data::getState().clearDisplayLists();
		}
	}

	//this->endList();
}

void TecGraf::OrientedSurface::draw() const
{
	/*if (!this->hasLists())
	{
		this->createList(1);
	}

	if (this->hasList())
	{
		this->drawList();

		return;
	}

	this->beginList();*/

	if (this->surface)
	{
		if (this->isPositive())
		{
			this->surface->draw();
		}
		else
		{
			for (Data::Boundary::FrontElementIterator iter = this->surface->eBegin();
				 iter != this->surface->eEnd(); iter++)
			{
				(*iter)->invert();
			}

			this->surface->clearLists();

			this->surface->draw();

			for (Data::Boundary::FrontElementIterator iter = this->surface->eBegin();
				 iter != this->surface->eEnd(); iter++)
			{
				(*iter)->invert();
			}

			this->surface->clearLists();
		}
	}

	//this->endList();
}

std::string TecGraf::OrientedSurface::text() const
{
	return "";
}
#endif //#if USE_GUI

#endif //#if USE_TECGRAF
