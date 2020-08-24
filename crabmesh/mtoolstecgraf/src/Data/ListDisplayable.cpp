#include "Data/ListDisplayable.h"

#if USE_GUI

#include "Data/State.h"

using namespace Data;

Data::ListDisplayable::ListDisplayable(Real r, Real g, Real b) :
	Data::Colorable(r, g, b)
{
	this->list = 0;
	this->frozen = false;
	this->drawing = false;

	this->drawingMeasure = 0;
	this->drawingTotal = 0;
}

Data::ListDisplayable::~ListDisplayable()
{
	this->clearLists();
}

void Data::ListDisplayable::clearList(UInt list) const
{
	if (this->frozen)
	{
		this->addPendingClear(list);

		return;
	}

	glDeleteLists(this->list + list, 1);

	this->lists[list] = false;
}

void Data::ListDisplayable::clearLists() const
{
	if (this->list == 0)
	{
		return;
	}

	if (this->frozen)
	{
		this->addPendingClear();

		return;
	}

	//std::cout << "clearing display list for " << this << ", list = " << this->list << ", size = " << static_cast<UInt>(this->lists.size()) /*<< ", lists[0] = " << this->lists[0] << ", lists[1] = " << this->lists[1] */<< std::endl;

	//glDeleteLists(this->list, static_cast<UInt>(this->lists.size()));
	Data::getState().clearDisplayList(this->list, static_cast<UInt>(this->lists.size()));

	//std::cout << "end clearing display list for " << this << std::endl;

	this->lists.clear();
	this->outdated.clear();

	this->list = 0;
}

void Data::ListDisplayable::createList(UInt numLists) const
{
	//std::cout << "creating display list for " << this << ", size = " << this->lists.size() << std::endl;

	this->lists.resize(numLists, false);
	this->outdated.resize(numLists, true);

	this->list = glGenLists(numLists);

	//std::cout << "end creating display list for " << this << ", list = " << this->list << ", size = " << this->lists.size() << ", lists[0] = " << this->lists[0] << ", lists[1] = " << this->lists[1] << std::endl;
}

void Data::ListDisplayable::beginList(UInt list) const
{
	//std::cout << "filling display list for " << this << ", list = " << this->list << ", size = " << this->lists.size() << ", lists[0] = " << this->lists[0] << ", lists[1] = " << this->lists[1] << std::endl;

	this->drawing = true;

	glNewList(this->list + list, GL_COMPILE_AND_EXECUTE);

	this->lists[list] = true;
	this->outdated[list] = false;

	this->drawingMeasure = 0;
	this->drawingTotal = 0;
}

void Data::ListDisplayable::endList() const
{
	glEndList();

	this->drawing = false;

	this->drawingMeasure = 0;
	this->drawingTotal = 0;

	//std::cout << "end filling display list for " << this << ", list = " << this->list << ", size = " << this->lists.size() << ", lists[0] = " << this->lists[0] << ", lists[1] = " << this->lists[1] << std::endl;
}

void Data::ListDisplayable::drawList(UInt list) const
{
	//std::cout << "drawing display list for " << this << ", list = " << this->list << ", size = " << this->lists.size() << ", lists[0] = " << this->lists[0] << ", lists[1] = " << this->lists[1] << std::endl;

	glCallList(this->list + list);

	//std::cout << "end drawing display list for " << this << ", list = " << this->list << ", size = " << this->lists.size() << ", lists[0] = " << this->lists[0] << ", lists[1] = " << this->lists[1] << std::endl;
}

bool Data::ListDisplayable::hasList(UInt list) const
{
	return ((this->list != 0) && (this->lists[list]));
}

bool Data::ListDisplayable::hasLists() const
{
	return (this->list != 0);
}

void Data::ListDisplayable::setFrozen(bool frozen) const
{
	this->frozen = frozen;

	if (this->frozen)
	{
		//while (this->drawing);
	}
	else
	{
		this->resolvePendingClear();
	}
}

bool Data::ListDisplayable::isFrozen() const
{
	return this->frozen;
}

void Data::ListDisplayable::freeze() const
{
	this->setFrozen(true);
}

void Data::ListDisplayable::unfreeze() const
{
	this->setFrozen(false);
}

bool Data::ListDisplayable::isOutdated() const
{
	for (UInt i = 0; i < this->outdated.size(); i++)
	{
		if ((this->lists[i]) && (outdated[i]))
		{
			return true;
		}
	}

	return false;
}

bool Data::ListDisplayable::isDrawing() const
{
	return this->drawing;
}

void Data::ListDisplayable::setR(Real r) const
{
	Data::Colorable::setR(r);

	this->outdate();
}

void Data::ListDisplayable::setG(Real g) const
{
	Data::Colorable::setG(g);

	this->outdate();
}

void Data::ListDisplayable::setB(Real b) const
{
	Data::Colorable::setB(b);

	this->outdate();
}

void Data::ListDisplayable::highlight() const
{
	Data::Colorable::highlight();

	this->outdate();
}

void Data::ListDisplayable::unhighlight() const
{
	Data::Colorable::unhighlight();

	this->outdate();
}

void Data::ListDisplayable::outdate() const
{
	for (UInt i = 0; i < this->outdated.size(); i++)
	{
		this->outdated[i] = true;
	}
}

void Data::ListDisplayable::update() const
{
	for (UInt i = 0; i < this->outdated.size(); i++)
	{
		this->outdated[i] = false;
	}
}

void Data::ListDisplayable::addPendingClear() const
{
	if (this->pending.empty())
	{
		this->pending.resize(this->lists.size(), true);
	}
}

void Data::ListDisplayable::addPendingClear(UInt list) const
{
	if (this->pending.empty())
	{
		this->pending.resize(this->lists.size(), false);
	}

	this->pending[list] = true;
}

void Data::ListDisplayable::resolvePendingClear() const
{
	if (this->pending.empty())
	{
		return;
	}

	bool all = true;

	for (UInt i = 0; i < this->pending.size(); i++)
	{
		if (!this->pending[i])
		{
			all = false;

			break;
		}
	}

	if (all)
	{
		this->clearLists();
	}
	else
	{
		for (UInt i = 0; i < this->pending.size(); i++)
		{
			if (this->pending[i])
			{
				this->clearList(i);
			}
		}
	}

	this->pending.clear();
}

ULInt Data::ListDisplayable::getDrawingMeasure() const
{
    return this->drawingMeasure;
}

ULInt Data::ListDisplayable::getDrawingTotal() const
{
    return this->drawingTotal;
}

#endif //#if USE_GUI
