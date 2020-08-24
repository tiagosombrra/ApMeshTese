#include "Data/Spline/Surface.h"

#if USE_SPLINES

using namespace Data;
using namespace Data::Spline;

Data::Spline::Surface::Surface() :
	Data::Spline::ISurface::ISurface()
{

}

Data::Spline::Surface::~Surface()
{
	for (UInt i = 0; i < this->numPointsV(); i++)
	{
		for (UInt j = 0; j < this->numPointsU(); j++)
		{
			if (this->points[i][j])
			{
				delete this->points[i][j];
			}
		}
	}
}

UInt Data::Spline::Surface::add(UInt row, Point *p)
{
#if USE_GUI
	p->setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		p->highlight();
	}
	else
	{
		p->unhighlight();
	}
#endif //#if USE_GUI

	UInt col = this->addResizingPoints(row, p);

	if (!this->uFunction)
	{
		this->uFunction = this->makeUSplineFunction();
	}

	if (!this->vFunction)
	{
		this->vFunction = this->makeVSplineFunction();
	}

	return col;
}

UInt Data::Spline::Surface::addResizingPoints(UInt row, Point *p)
{
	UInt prevSize = static_cast<UInt>(this->points.size());

	UInt newSize = row + 1;

	if (prevSize < newSize)
	{
		UInt usize = this->numPointsU();

		this->points.resize(newSize);

		for (UInt i = prevSize; i < newSize; i++)
		{
			this->points[i].resize(usize, NULL);
		}
	}

	UInt size = static_cast<UInt>(this->points.size());

	if (this->points[row].empty())
	{
		UInt usize = this->numPointsU() + 1;

		for (UInt i = 0; i < size; i++)
		{
			this->points[i].resize(usize, NULL);
		}

		this->points[row][0] = p;

		return 0;
	}

	if (!this->points[row].back())
	{
		UInt usize = this->numPointsU();

		for (UInt i = 0; i < usize; i++)
		{
			if (!this->points[row][i])
			{
				this->points[row][i] = p;

				return i;
			}
		}
	}

	UInt usize = this->numPointsU() + 1;

	for (UInt i = 0; i < size; i++)
	{
		this->points[i].resize(usize, NULL);
	}

	this->points[row][usize - 1] = p;

	return usize - 1;
}

UInt Data::Spline::Surface::numPointsU() const
{
	return (this->numPointsV() == 0) ? 0 : static_cast<UInt>(this->points[0].size());
}

UInt Data::Spline::Surface::numPointsV() const
{
	return static_cast<UInt>(this->points.size());
}

void Data::Spline::Surface::setPoint(UInt i, UInt j, Point *p)
{
	this->points[i][j] = p;

#if USE_GUI
	p->setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		p->highlight();
	}
	else
	{
		p->unhighlight();
	}
#endif //#if USE_GUI
}

Point *Data::Spline::Surface::getPoint(UInt i, UInt j) const
{
	return this->points[i][j];
}

#endif //#if USE_SPLINES
