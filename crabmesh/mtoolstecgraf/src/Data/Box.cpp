#include "Data/Box.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/State.h"

using namespace Data;

Data::Box::Box() :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	GeometricShape()
#else
	GeometricShape(2)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 2;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

	//this->setBox(NULL, NULL);
	this->points[0] = NULL;
	this->points[1] = NULL;
}

Data::Box::Box(Point *min, Point *max) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	GeometricShape()
#else
	GeometricShape(2)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 2;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

	//this->setBox(min, max);
	this->points[0] = min;
	this->points[1] = max;
}

Data::Box::Box(const Box &box) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	GeometricShape()
#else
	GeometricShape(2)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 2;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

	//this->setBox(*box.getMin(), *box.getMax());
	//this->points[0] = (box.points[0]) ? new Point(box.points[0]) : NULL;
	//this->points[1] = (box.points[1]) ? new Point(box.points[1]) : NULL;

	UInt dim = box.dimension();
	//UInt dim = box.points[0] ? box.points[0]->dimension() : 0;

	this->points[0] = (!box.points[0]) ? NULL :
		((dim == 2) ? static_cast<Point *>(new Point2D(box.points[0])) :
#if USE_EXPERIMENTAL_3
                      static_cast<Point *>(new Point3D(box.points[0])));
#else
		((dim == 3) ? static_cast<Point *>(new Point3D(box.points[0])) : new Point(box.points[0])));
#endif //#if USE_EXPERIMENTAL_3
	this->points[1] = (!box.points[1]) ? NULL :
		((dim == 2) ? static_cast<Point *>(new Point2D(box.points[1])) :
#if USE_EXPERIMENTAL_3
                      static_cast<Point *>(new Point3D(box.points[1])));
#else
		((dim == 3) ? static_cast<Point *>(new Point3D(box.points[1])) : new Point(box.points[1])));
#endif //#if USE_EXPERIMENTAL_3
}

Data::Box::Box(const Point &min, const Point &max) :
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	GeometricShape()
#else
	GeometricShape(2)
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
{
#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 2;
	this->GeometricShape::points = this->points;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)

	//this->setBox(min, max);
	//this->points[0] = new Point(min);
	//this->points[1] = new Point(max);

	UInt dim = min.dimension();

	this->points[0] =
		((dim == 2) ? static_cast<Point *>(new Point2D(min)) :
#if USE_EXPERIMENTAL_3
                      static_cast<Point *>(new Point3D(min)));
#else
		((dim == 3) ? static_cast<Point *>(new Point3D(min)) : new Point(min)));
#endif //#if USE_EXPERIMENTAL_3
	this->points[1] =
		((dim == 2) ? static_cast<Point *>(new Point2D(max)) :
#if USE_EXPERIMENTAL_3
                      static_cast<Point *>(new Point3D(max)));
#else
		((dim == 3) ? static_cast<Point *>(new Point3D(max)) : new Point(max)));
#endif //#if USE_EXPERIMENTAL_3
}

Data::Box::~Box()
{
#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	if (this->getMin())
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	{
		delete this->getMin();
	}

#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	if (this->getMax())
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	{
		delete this->getMax();
	}

#if (!USE_EXPERIMENTAL_4)
#if USE_EXPERIMENTAL
	this->numPts = 0;
	this->GeometricShape::points = NULL;
#endif //#if USE_EXPERIMENTAL
#endif //#if (!USE_EXPERIMENTAL_4)
}

#if USE_EXPERIMENTAL_4
void Data::Box::setPoint(UInt i, Point *p)
{
    this->points[i] = p;
}

Point *Data::Box::getPoint(UInt i) const
{
    return this->points[i];
}
#endif //#if USE_EXPERIMENTAL_4

UInt Data::Box::numPoints() const
{
	return 2;
}

void Data::Box::setBox(Point *min, Point *max)
{
	this->setMin(min);
	this->setMax(max);
}

void Data::Box::setBox(const Point &min, const Point &max)
{
	//this->setBox(new Point(min), new Point(max));

	UInt dim = min.dimension();

	this->setBox(
		((dim == 2) ? static_cast<Point *>(new Point2D(min)) :
#if USE_EXPERIMENTAL_3
                      static_cast<Point *>(new Point3D(min))),
#else
		((dim == 3) ? static_cast<Point *>(new Point3D(min)) : new Point(min))),
#endif //#if USE_EXPERIMENTAL_3
		((dim == 2) ? static_cast<Point *>(new Point2D(max)) :
#if USE_EXPERIMENTAL_3
                      static_cast<Point *>(new Point3D(max))));
#else
		((dim == 3) ? static_cast<Point *>(new Point3D(max)) : new Point(max))));
#endif //#if USE_EXPERIMENTAL_3
}

void Data::Box::setMin(Point *p)
{
	//this->setPoint(0, p);
	this->points[0] = p;
}

Point *Data::Box::getMin() const
{
	//return this->getPoint(0);
	return this->points[0];
}

void Data::Box::setMax(Point *p)
{
	//this->setPoint(1, p);
	this->points[1] = p;
}

Point *Data::Box::getMax() const
{
	//return this->getPoint(1);
	return this->points[1];
}

//#if USE_EXPERIMENTAL
UInt Data::Box::dimension() const
{
	return this->points[0] ? this->points[0]->dimension() : 0;
}
//#endif //#if USE_EXPERIMENTAL

Real Data::Box::orientedSize() const
{
	Real s = 0.0;

	UInt dim = this->dimension();

	for (UInt i = 0; i < dim; i++)
	{
		Real d = this->getMax()->getCoord(i) - this->getMin()->getCoord(i);

		if (d > s)
		{
			s = d;
		}
	}

	return s;
}

bool Data::Box::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::Box::in(const Point *p) const
{
	Point *min = this->getMin();
	Point *max = this->getMax();

	UInt dim = this->dimension();

	for (UInt i = 0; i < dim; i++)
	{
		if ((p->getCoord(i) < min->getCoord(i) + Data::getTolerance()) ||
			(p->getCoord(i) > max->getCoord(i) - Data::getTolerance()))
		{
			return false;
		}
	}

	return true;
}

bool Data::Box::optIn(const Point &p) const
{
	return this->optIn(&p);
}

bool Data::Box::optIn(const Point *p) const
{
	UInt dim = (*this->points)->dimension();

	const Real *tmin = (*this->points)->getCoordsArray();
	const Real *tmax = (*(this->points + 1))->getCoordsArray();

	const Real *coords = p->getCoordsArray();

	Real tol = Data::getTolerance();

	for (UInt i = 0; i < dim; ++tmin, ++tmax, ++coords, ++i)
	{
		if (((*coords) < (*tmin) + tol) ||
			((*coords) > (*tmax) - tol))
		{
			return false;
		}
	}

	return true;
}

bool Data::Box::in(const Box &box) const
{
	return this->in(&box);
}

bool Data::Box::in(const Box *box) const
{
	Data::Point *min = this->getMin();
	Data::Point *max = this->getMax();

	UInt dimension = this->dimension();

	for (UInt i = 0; i < dimension; i++)
	{
		if ((box->getMin()->getCoord(i) < min->getCoord(i) - Data::getTolerance()) ||
			(box->getMax()->getCoord(i) > max->getCoord(i) + Data::getTolerance()))
		{
			return false;
		}
	}

	return true;
}

bool Data::Box::optIn(const Box &box) const
{
	return this->optIn(&box);
}

bool Data::Box::optIn(const Box *box) const
{
	UInt dim = (*this->points)->dimension();

	const Real *tmin = (*this->points)->getCoordsArray();
	const Real *tmax = (*(this->points + 1))->getCoordsArray();

	const Real *bmin = (*box->points)->getCoordsArray();
	const Real *bmax = (*(box->points + 1))->getCoordsArray();

	Real tol = Data::getTolerance();

	for (UInt i = 0; i < dim; ++tmin, ++tmax, ++bmin, ++bmax, ++i)
	{
		if (((*bmin) < (*tmin) - tol) ||
			((*bmax) > (*tmax) + tol))
		{
			return false;
		}
	}

	return true;
}

bool Data::Box::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::Box::on(const Point *p) const
{
	return ((!this->in(p)) && (!this->out(p)));
}

bool Data::Box::optOn(const Point &p) const
{
	return this->optOn(&p);
}

bool Data::Box::optOn(const Point *p) const
{
	return ((!this->optIn(p)) && (!this->optOut(p)));
}

bool Data::Box::on(const Box &box) const
{
	return this->on(&box);
}

bool Data::Box::on(const Box *box) const
{
	return ((this->intercept(box)) && (!this->in(box)));
}

bool Data::Box::optOn(const Box &box) const
{
	return this->optOn(&box);
}

bool Data::Box::optOn(const Box *box) const
{
	return ((this->optIntercept(box)) && (!this->optIn(box)));
}

bool Data::Box::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::Box::out(const Point *p) const
{
	Point *min = this->getMin();
	Point *max = this->getMax();

	UInt dim = this->dimension();

	for (UInt i = 0; i < dim; i++)
	{
		if ((p->getCoord(i) < min->getCoord(i) - Data::getTolerance()) ||
			(p->getCoord(i) > max->getCoord(i) + Data::getTolerance()))
		{
			return true;
		}
	}

	return false;
}

bool Data::Box::optOut(const Point &p) const
{
	return this->optOut(&p);
}

bool Data::Box::optOut(const Point *p) const
{
	UInt dim = (*this->points)->dimension();

	const Real *tmin = (*this->points)->getCoordsArray();
	const Real *tmax = (*(this->points + 1))->getCoordsArray();

	const Real *coords = p->getCoordsArray();

	Real tol = Data::getTolerance();

	for (UInt i = 0; i < dim; ++tmin, ++tmax, ++coords, ++i)
	{
		if (((*coords) < (*tmin) - tol) ||
			((*coords) > (*tmax) + tol))
		{
			return true;
		}
	}

	return false;
}

bool Data::Box::out(const Box &box) const
{
	return this->out(&box);
}

bool Data::Box::out(const Box *box) const
{
	Data::Point *min = this->getMin();
	Data::Point *max = this->getMax();

	UInt dimension = this->dimension();

	for (UInt i = 0; i < dimension; i++)
	{
		if ((box->getMax()->getCoord(i) - Data::getTolerance() < min->getCoord(i)) ||
			(box->getMin()->getCoord(i) + Data::getTolerance() > max->getCoord(i)))
		{
			return true;
		}
	}

	return false;
}

bool Data::Box::optOut(const Box &box) const
{
	return this->optOut(&box);
}

bool Data::Box::optOut(const Box *box) const
{
	UInt dim = (*this->points)->dimension();

	const Real *tmin = (*this->points)->getCoordsArray();
	const Real *tmax = (*(this->points + 1))->getCoordsArray();

	const Real *bmin = (*box->points)->getCoordsArray();
	const Real *bmax = (*(box->points + 1))->getCoordsArray();

	Real tol = Data::getTolerance();

	for (UInt i = 0; i < dim; ++tmin, ++tmax, ++bmin, ++bmax, ++i)
	{
		if (((*bmax) - tol < (*tmin)) ||
			((*bmin) + tol > (*tmax)))
		{
			return true;
		}
	}

	return false;
}

bool Data::Box::intercept(const Box &box) const
{
	return this->intercept(&box);
}

bool Data::Box::intercept(const Box *box) const
{
	UInt dim = std::max(this->dimension(), box->dimension());

	Point *tmin = this->getMin();
	Point *tmax = this->getMax();

	Point *bmin = box->getMin();
	Point *bmax = box->getMax();

	for (UInt i = 0; i < dim; i++)
	{
		if ((tmin->getCoord(i) > bmax->getCoord(i) + Data::getTolerance()) ||
			(tmax->getCoord(i) < bmin->getCoord(i) - Data::getTolerance()))
		{
			return false;
		}
	}

	return true;
}

bool Data::Box::optIntercept(const Box &box) const
{
	return this->optIntercept(&box);
}

bool Data::Box::optIntercept(const Box *box) const
{
	UInt dim = (*this->points)->dimension();

	const Real *tmin = (*this->points)->getCoordsArray();
	const Real *tmax = (*(this->points + 1))->getCoordsArray();

	const Real *bmin = (*box->points)->getCoordsArray();
	const Real *bmax = (*(box->points + 1))->getCoordsArray();

	Real tol = Data::getTolerance();

	for (UInt i = 0; i < dim; ++tmin, ++tmax, ++bmin, ++bmax, ++i)
	{
		if (((*tmin) > (*bmax) + tol) ||
			((*tmax) + tol < (*bmin)))
		{
			return false;
		}
	}

	return true;
}

bool Data::Box::outOf(const Box &box) const
{
	return this->outOf(box);
}

bool Data::Box::outOf(const Box *box) const
{
	/*//if (box->out(this))
	if (box->optOut(this))
	{
		return true;
	}

	//if (!box->intercept(this))
	if (!box->optIntercept(this))
	{
		return true;
	}

	for (UInt i = 0; i < this->dimension; i++)
	{
		if (((this->getMin()->getCoord(i) <  box->getMin()->getCoord(i)) &&
			 (this->getMax()->getCoord(i) <= box->getMin()->getCoord(i))) ||
			((this->getMax()->getCoord(i) >  box->getMax()->getCoord(i)) &&
			 (this->getMin()->getCoord(i) >= box->getMax()->getCoord(i))))
		{
			return true;
		}
	}*/

	UInt dim = (*this->points)->dimension();

	const Real *tmin = (*this->points)->getCoordsArray();
	const Real *tmax = (*(this->points + 1))->getCoordsArray();

	const Real *bmin = (*box->points)->getCoordsArray();
	const Real *bmax = (*(box->points + 1))->getCoordsArray();

	Real tol = Data::getTolerance();

	for (UInt i = 0; i < dim; ++tmin, ++tmax, ++bmin, ++bmax, ++i)
	{
		if ((((*tmax) - tol < (*bmin)) || ((*tmin) + tol > (*bmax))) || //box->out(this)
			(((*bmin) > (*tmax) + tol) || ((*bmax) + tol < (*tmin))) || //box->intercept(this)
			(((*tmin) < (*bmin)) && ((*tmax) <= (*bmin))) || //third check
			(((*tmax) > (*bmax)) && ((*tmin) >= (*bmax)))) //third check
		{
			return true;
		}
	}

	return false;
}

#if USE_ARRAY
Box &Data::Box::operator=(const Box &box)
{
	if (this != &box)
	{
#if USE_EXPERIMENTAL
		UInt dim = box.dimension();

		if ((this->points[0]) && (this->points[0]->dimension() == dim))
		{
			this->points[0]->setCoords(box.points[0]->getCoordsArray());
		}
		else
		{
			if (this->points[0])
			{
				delete this->points[0];
			}

			this->points[0] = (!box.points[0]) ? NULL :
				((dim == 2) ? static_cast<Point *>(new Point2D(box.points[0])) :
#if USE_EXPERIMENTAL_3
                              static_cast<Point *>(new Point3D(box.points[0])));
#else
				((dim == 3) ? static_cast<Point *>(new Point3D(box.points[0])) : new Point(box.points[0])));
#endif //#if USE_EXPERIMENTAL_3
		}

		if ((this->points[1]) && (this->points[1]->dimension() == dim))
		{
			this->points[1]->setCoords(box.points[1]->getCoordsArray());
		}
		else
		{
			if (this->points[1])
			{
				delete this->points[1];
			}

			this->points[1] = (!box.points[1]) ? NULL :
				((dim == 2) ? static_cast<Point *>(new Point2D(box.points[1])) :
#if USE_EXPERIMENTAL_3
                              static_cast<Point *>(new Point3D(box.points[1])));
#else
				((dim == 3) ? static_cast<Point *>(new Point3D(box.points[1])) : new Point(box.points[1])));
#endif //#if USE_EXPERIMENTAL_3
		}

#if (!USE_EXPERIMENTAL_4)
		this->numPts = 2;
		this->GeometricShape::points = this->points;
#endif //#if (!USE_EXPERIMENTAL_4)
#else
		if (this->points)
		{
			delete [] this->points;
		}

		this->numPts = box.numPts;

		this->points = new Point*[this->numPts];

		for (UInt i = 0; i < this->numPts; i++)
		{
			this->points[i] = (box.points[i]) ? new Point(box.points[i]) : NULL;
		}
#endif //#if USE_EXPERIMENTAL

#if USE_GUI
		this->setColor(box.r, box.g, box.b);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_ARRAY

#if USE_GUI
/*void Data::Box::draw() const
{
	GeometricShape::draw();
}*/

//void Data::Box::draw(bool /*fill*/) const
void Data::Box::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	/*if (this->isHighlighted())
	{
		glColor(this->getR(), this->getG(), this->getB());
		//glColor(this->getR()*0.5, this->getG()*0.5, this->getB()*0.5);
		//glColor(this->getR()*0.5, this->getG()*0.5, this->getB()*0.5, 0.5);
		//glColor(this->getR(), this->getG(), this->getB(), 0.5);
	}
	else
	{
		glColor(this->getR(), this->getG(), this->getB());
	}*/

	glColor(this->getR(), this->getG(), this->getB());

	UInt dim = this->dimension();

    glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));

	bool changed = false;
	int modes[2];

	if (this->isHighlighted())
	{
		glGetIntegerv(GL_POLYGON_MODE, modes);

		if ((modes[0] != GL_FILL) || (modes[1] != GL_FILL))
		{
			changed = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (dim == 1)
	{
		/*Point mid = this->mid();
		Point gmid = Data::getMid();
		Real invex = 1.0 - Data::getExplode();*/

		glPushMatrix();
#if USE_EXPERIMENTAL_3
			this->explodeAndShrink(this->mid2D());
#else
			this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3
			/*glTranslate(gmid.getCoord(0)*invex + mid.getCoord(0)*Data::getExplode(), gmid.getCoord(1)*invex + mid.getCoord(1)*Data::getExplode());
			glScale(Data::getShrink(), Data::getShrink());
			glTranslate(-mid.getCoord(0), -mid.getCoord(1));*/

			glBegin(GL_LINES);

				glVertex(this->getMin()->getCoord(0), 0.0);
				glVertex(this->getMax()->getCoord(0), 0.0);

			glEnd();
		glPopMatrix();
	}
	else if (dim == 2)
	{
		/*Point2D mid = this->mid();
		Point2D gmid = Data::getMid();
		Real invex = 1.0 - Data::getExplode();*/

		glPushMatrix();
#if USE_EXPERIMENTAL_3
			this->explodeAndShrink(this->mid2D());
#else
			this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3
			/*glTranslate(gmid.getX()*invex + mid.getX()*Data::getExplode(), gmid.getY()*invex + mid.getY()*Data::getExplode());
			glScale(Data::getShrink(), Data::getShrink());
			glTranslate(-mid.getX(), -mid.getY());*/

			if (this->isHighlighted())
			{
				glBegin(GL_QUADS);
			}
			else
			{
				glBegin(GL_LINE_LOOP);
			}

				glVertex(this->getMin()->getCoord(0), this->getMin()->getCoord(1));
				glVertex(this->getMax()->getCoord(0), this->getMin()->getCoord(1));
				glVertex(this->getMax()->getCoord(0), this->getMax()->getCoord(1));
				glVertex(this->getMin()->getCoord(0), this->getMax()->getCoord(1));

			glEnd();

		glPopMatrix();
	}
	else if (dim == 3)
	{
		/*GLboolean lighting = glIsEnabled(GL_LIGHTING);

		if (!this->isHighlighted())
		{
			//glEnable(GL_LIGHTING);
			glDisable(GL_LIGHTING);
		}*/

		glPushMatrix();
#if USE_EXPERIMENTAL_3
			this->explodeAndShrink(this->mid3D());
#else
			this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

			if (this->isHighlighted())
			{
				glBegin(GL_QUADS);
			}
			else
			{
				glBegin(GL_LINE_LOOP);
			}

			glNormal(0.0, 0.0, -1.0);

			glVertex(this->getMin()->getCoord(0), this->getMin()->getCoord(1), this->getMin()->getCoord(2));
			glVertex(this->getMin()->getCoord(0), this->getMax()->getCoord(1), this->getMin()->getCoord(2));
			glVertex(this->getMax()->getCoord(0), this->getMax()->getCoord(1), this->getMin()->getCoord(2));
			glVertex(this->getMax()->getCoord(0), this->getMin()->getCoord(1), this->getMin()->getCoord(2));

			glEnd();

			if (this->isHighlighted())
			{
				glBegin(GL_QUADS);
			}
			else
			{
				glBegin(GL_LINE_LOOP);
			}

			glNormal(0.0, 0.0, 1.0);

			glVertex(this->getMin()->getCoord(0), this->getMin()->getCoord(1), this->getMax()->getCoord(2));
			glVertex(this->getMax()->getCoord(0), this->getMin()->getCoord(1), this->getMax()->getCoord(2));
			glVertex(this->getMax()->getCoord(0), this->getMax()->getCoord(1), this->getMax()->getCoord(2));
			glVertex(this->getMin()->getCoord(0), this->getMax()->getCoord(1), this->getMax()->getCoord(2));

			glEnd();

			if (this->isHighlighted())
			{
				glBegin(GL_QUADS);

				glNormal(-1.0, 0.0, 0.0);

				glVertex(this->getMin()->getCoord(0), this->getMin()->getCoord(1), this->getMin()->getCoord(2));
				glVertex(this->getMin()->getCoord(0), this->getMin()->getCoord(1), this->getMax()->getCoord(2));
				glVertex(this->getMin()->getCoord(0), this->getMax()->getCoord(1), this->getMax()->getCoord(2));
				glVertex(this->getMin()->getCoord(0), this->getMax()->getCoord(1), this->getMin()->getCoord(2));

				glNormal(1.0, 0.0, 0.0);

				glVertex(this->getMax()->getCoord(0), this->getMin()->getCoord(1), this->getMin()->getCoord(2));
				glVertex(this->getMax()->getCoord(0), this->getMax()->getCoord(1), this->getMin()->getCoord(2));
				glVertex(this->getMax()->getCoord(0), this->getMax()->getCoord(1), this->getMax()->getCoord(2));
				glVertex(this->getMax()->getCoord(0), this->getMin()->getCoord(1), this->getMax()->getCoord(2));

				glNormal(0.0, -1.0, 0.0);

				glVertex(this->getMin()->getCoord(0), this->getMin()->getCoord(1), this->getMin()->getCoord(2));
				glVertex(this->getMin()->getCoord(0), this->getMin()->getCoord(1), this->getMax()->getCoord(2));
				glVertex(this->getMax()->getCoord(0), this->getMin()->getCoord(1), this->getMax()->getCoord(2));
				glVertex(this->getMax()->getCoord(0), this->getMin()->getCoord(1), this->getMin()->getCoord(2));

				glNormal(0.0, 1.0, 0.0);

				glVertex(this->getMin()->getCoord(0), this->getMax()->getCoord(1), this->getMin()->getCoord(2));
				glVertex(this->getMax()->getCoord(0), this->getMax()->getCoord(1), this->getMin()->getCoord(2));
				glVertex(this->getMax()->getCoord(0), this->getMax()->getCoord(1), this->getMax()->getCoord(2));
				glVertex(this->getMin()->getCoord(0), this->getMax()->getCoord(1), this->getMax()->getCoord(2));

				glEnd();
			}
			else
			{
				glBegin(GL_LINES);

				glNormal(-1.0, 0.0, 0.0);

				glVertex(this->getMin()->getCoord(0), this->getMin()->getCoord(1), this->getMin()->getCoord(2));
				glVertex(this->getMin()->getCoord(0), this->getMin()->getCoord(1), this->getMax()->getCoord(2));

				glNormal(1.0, 0.0, 0.0);

				glVertex(this->getMax()->getCoord(0), this->getMin()->getCoord(1), this->getMin()->getCoord(2));
				glVertex(this->getMax()->getCoord(0), this->getMin()->getCoord(1), this->getMax()->getCoord(2));

				glNormal(0.0, -1.0, 0.0);

				glVertex(this->getMax()->getCoord(0), this->getMax()->getCoord(1), this->getMin()->getCoord(2));
				glVertex(this->getMax()->getCoord(0), this->getMax()->getCoord(1), this->getMax()->getCoord(2));

				glNormal(0.0, 1.0, 0.0);

				glVertex(this->getMin()->getCoord(0), this->getMax()->getCoord(1), this->getMin()->getCoord(2));
				glVertex(this->getMin()->getCoord(0), this->getMax()->getCoord(1), this->getMax()->getCoord(2));

				glEnd();
			}
		glPopMatrix();

		/*//if ((this->isHighlighted()) && (!enabled))
		if (lighting)
		{
			//glDisable(GL_LIGHTING);
			glEnable(GL_LIGHTING);
		}*/
	}

	if (changed)
	{
		glPolygonMode(GL_FRONT, modes[0]);
		glPolygonMode(GL_BACK, modes[1]);
	}
}

std::string Data::Box::text() const
{
	std::stringstream str;

	str	<< this->getMin()->text() << " "
		<< this->getMax()->text();

	return str.str();
}
#endif //#if USE_GUI
