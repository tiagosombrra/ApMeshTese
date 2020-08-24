#include "Data/Spline/ICurve.h"

#if USE_SPLINES

#include "Data/State.h"
#include "Data/Point3D.h"
#include "Data/Point2D.h"
#include "Data/Spline/SplineFunction.h"
#include "Numerical/Function.h"
#include "Numerical/GaussLegendreIntegralCalculator.h"
#include "Numerical/BisectionEquationRootFinder.h"
#include "Numerical/FalsePositionEquationRootFinder.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::ICurve::ICurve() :
	Data::GeometricShape::GeometricShape()
{
#if USE_GUI
	Data::Colorable::setR(1.0);
	Data::Colorable::setG(1.0);
	Data::Colorable::setB(1.0);

	this->discretization = 4;
#endif //#if USE_GUI

	this->function = NULL;
}

Data::Spline::ICurve::~ICurve()
{
	if (this->function)
	{
		delete this->function;
	}
}

UInt Data::Spline::ICurve::numPoints(UInt /*segment*/) const
{
	return this->numPoints();
}

#if USE_NUMERICAL
Box Data::Spline::ICurve::box(UInt segment) const
{
	/*struct CoordFunction : public Numerical::IntervalEquationRootFunction
	{
		UInt segment;
		Real minimum;
		Real maximum;
		UInt coord;
		const Spline::ICurve *curve;

		Real min()
		{
			return this->minimum;
		};

		Real max()
		{
			return this->maximum;
		};

		using Numerical::IntervalEquationRootFunction::f;
		Real f(Real x)
		{
			Point3D p = this->curve->p(segment, x);

			return p.getCoord(coord);
		};
	};*/

	class CoordFunction : public Numerical::Function
	{
    public:

        UInt segment;
        UInt coord;
        const Spline::ICurve *curve;

        using Numerical::Function::f;
		Real f(Real x)
		{
			Point3D p = this->curve->p(this->segment, x);

			return p.getCoord(this->coord);
		};
	};

	CoordFunction x, y, z;

	x.segment = y.segment = z.segment = segment;
	//x.minimum = y.minimum = z.minimum = 0.0;
	//x.maximum = y.maximum = z.maximum = 1.0;
	x.curve = y.curve = z.curve = this;
	x.coord = 0;
	y.coord = 1;
	z.coord = 2;

	//Numerical::MinBisectionEquationRootFinder minxRoot, minyRoot, minzRoot;
	//Numerical::MaxBisectionEquationRootFinder maxxRoot, maxyRoot, maxzRoot;
	Numerical::MinFalsePositionEquationRootFinder minxRoot, minyRoot, minzRoot;
	Numerical::MaxFalsePositionEquationRootFinder maxxRoot, maxyRoot, maxzRoot;

	minxRoot.setMin(0.0);
	minyRoot.setMin(0.0);
	minzRoot.setMin(0.0);
	maxxRoot.setMin(0.0);
	maxyRoot.setMin(0.0);
	maxzRoot.setMin(0.0);
	minxRoot.setMax(1.0);
	minyRoot.setMax(1.0);
	minzRoot.setMax(1.0);
	maxxRoot.setMax(1.0);
	maxyRoot.setMax(1.0);
	maxzRoot.setMax(1.0);

	minxRoot.setFunction(&x);
	minyRoot.setFunction(&y);
	minzRoot.setFunction(&z);
	maxxRoot.setFunction(&x);
	maxyRoot.setFunction(&y);
	maxzRoot.setFunction(&z);

	//bool ok = true;

	Data::Point3D min, max;

	//Real tminx = minxRoot.execute(&x, ok);
	minxRoot.execute();
	Real tminx = minxRoot.getRoot();

	//if (ok)
	if (minxRoot.isOk())
	{
		Real minx = this->p(segment, tminx).getCoord(0);

		min.setX(minx);
	}

	//Real tminy = minyRoot.execute(&y, ok);
	minyRoot.execute();
	Real tminy = minyRoot.getRoot();

	//if (ok)
	if (minyRoot.isOk())
	{
		Real miny = this->p(segment, tminy).getCoord(1);

		min.setY(miny);
	}

	//Real tminz = minzRoot.execute(&z, ok);
	minzRoot.execute();
	Real tminz = minzRoot.getRoot();

	//if (ok)
	if (minzRoot.isOk())
	{
		Real minz = this->p(segment, tminz).getCoord(2);

		min.setZ(minz);
	}

	//Real tmaxx = maxxRoot.execute(&x, ok);
	maxxRoot.execute();
	Real tmaxx = maxxRoot.getRoot();

	//if (ok)
	if (maxxRoot.isOk())
	{
		Real maxx = this->p(segment, tmaxx).getCoord(0);

		max.setX(maxx);
	}

	//Real tmaxy = maxyRoot.execute(&y, ok);
	maxyRoot.execute();
	Real tmaxy = maxyRoot.getRoot();

	//if (ok)
	if (maxyRoot.isOk())
	{
		Real maxy = this->p(segment, tmaxy).getCoord(1);

		max.setY(maxy);
	}

	//Real tmaxz = maxzRoot.execute(&z, ok);
	maxzRoot.execute();
	Real tmaxz = maxzRoot.getRoot();

	//if (ok)
	if (maxzRoot.isOk())
	{
		Real maxz = this->p(segment, tmaxz).getCoord(2);

		max.setZ(maxz);
	}

	return Box(min, max);
}

Box Data::Spline::ICurve::box() const
{
	if (this->numSegments() == 0)
	{
		return Box(Point3D(), Point3D());
	}

	Box box = this->box(0);

	for (UInt j = 1; j < this->numSegments(); j++)
	{
		Box b = this->box(j);

		for (UInt i = 0; i < box.dimension(); i++)
		{
			if (b.getMin()->getCoord(i) < box.getMin()->getCoord(i))
			{
				box.getMin()->setCoord(i, b.getMin()->getCoord(i));
			}

			if (b.getMax()->getCoord(i) > box.getMax()->getCoord(i))
			{
				box.getMax()->setCoord(i, b.getMax()->getCoord(i));
			}
		}
	}

	return box;
}
#endif //#if USE_NUMERICAL

Real Data::Spline::ICurve::orientedSize() const
{
	Real size = 0;

	for (UInt j = 0; j < this->numSegments(); j++)
	{
		Point3D p1 = this->p(j, 0.0);

#if USE_GUI
		UInt discretization = std::max(static_cast<UInt>(1), this->discretization*(this->numPoints(j) - 1));
#else
		UInt discretization = std::max(static_cast<UInt>(1), 4*(this->numPoints(j) - 1));
#endif //#if USE_GUI

		Real incr = 1.0/static_cast<Real>(discretization);

		for (UInt i = 1; i < discretization; i++)
		{
			Point3D p2 = this->p(j, i*incr);

			size += p1.distance(p2);

			p1 = p2;
		}

		Point3D p2 = this->p(j, 1.0);

		size += p1.distance(p2);
	}

	return size;
}

bool Data::Spline::ICurve::in(const Point &/*p*/) const
{
	return false;
}

bool Data::Spline::ICurve::in(const Point */*p*/) const
{
	return false;
}

bool Data::Spline::ICurve::on(const Point &/*p*/) const
{
	return false;
}

bool Data::Spline::ICurve::on(const Point */*p*/) const
{
	return false;
}

bool Data::Spline::ICurve::out(const Point &/*p*/) const
{
	return false;
}

bool Data::Spline::ICurve::out(const Point */*p*/) const
{
	return false;
}

UInt Data::Spline::ICurve::numSegments() const
{
	return 1;
}

Point *Data::Spline::ICurve::point(UInt /*segment*/, UInt i) const
{
	return this->getPoint(i);
}

void Data::Spline::ICurve::move(UInt i, Real x, Real y, Real z)
{
	this->getPoint(i)->setCoord(0, x);
	this->getPoint(i)->setCoord(1, y);

	if (this->getPoint(i)->dimension() > 2)
	{
		this->getPoint(i)->setCoord(2, z);
	}
}

Point3D Data::Spline::ICurve::p(UInt segment, Real t) const
{
	Vector3D sum;

	for (UInt i = 0; i < this->numPoints(segment); i++)
	{
		Vector3D pt(this->pt3d(this->point(segment, i)));

		Real blend = this->blend(segment, i, t);

		pt.multiply(blend);

		sum.sum(pt);
	}

	return Point3D(sum);
}

#if USE_NUMERICAL
Point3D Data::Spline::ICurve::dp(UInt segment, Real t) const
{
	Vector3D sum;

	for (UInt i = 0; i < this->numPoints(segment); i++)
	{
		Vector3D pt(this->pt3d(this->point(segment, i)));

		Real blend = this->dBlend(segment, i, t);

		pt.multiply(blend);

		sum.sum(pt);
	}

	return Point3D(sum);
}

Point3D Data::Spline::ICurve::d2p(UInt segment, Real t) const
{
	Vector3D sum;

	for (UInt i = 0; i < this->numPoints(segment); i++)
	{
		Vector3D pt(this->pt3d(this->point(segment, i)));

		Real blend = this->d2Blend(segment, i, t);

		pt.multiply(blend);

		sum.sum(pt);
	}

	return Point3D(sum);
}
#endif //#if USE_NUMERICAL

Real Data::Spline::ICurve::blend(UInt segment, UInt i, Real t) const
{
	return (!this->function) ? 0.0 : this->function->blend(i, this->numPoints(segment), t);
}

#if USE_NUMERICAL
Real Data::Spline::ICurve::dBlend(UInt segment, UInt i, Real t) const
{
	return (!this->function) ? 0.0 : this->function->dBlend(i, this->numPoints(segment), t);
}

Real Data::Spline::ICurve::d2Blend(UInt segment, UInt i, Real t) const
{
	return (!this->function) ? 0.0 : this->function->d2Blend(i, this->numPoints(segment), t);
}
#endif //#if USE_NUMERICAL

UInt Data::Spline::ICurve::degree(UInt /*segment*/) const
{
	return (!this->function) ? 0 : this->function->degree();
}

UInt Data::Spline::ICurve::minDegree() const
{
	UInt min, max;

	this->degrees(min, max);

	return min;
}

UInt Data::Spline::ICurve::maxDegree() const
{
	UInt min, max;

	this->degrees(min, max);

	return max;
}

void Data::Spline::ICurve::degrees(UInt &min, UInt &max) const
{
	min = this->degree(0);
	max = min;

	for (UInt j = 1; j < this->numSegments(); j++)
	{
		UInt deg = this->degree(j);

		if (deg < min)
		{
			min = deg;
		}

		if (deg > max)
		{
			max = deg;
		}
	}
}

#if USE_NUMERICAL
Real Data::Spline::ICurve::length(UInt segment, Real tmin, Real tmax, UInt parts, UInt points) const
{
	if (parts < 1)
	{
		parts = 1;
	}

	//struct LengthIntegralFunction : public Numerical::IntegralFunction
	class LengthIntegralFunction : public Numerical::Function
	{
    public:

		//Real minimum;
		//Real maximum;
		UInt segment;
		const Spline::ICurve *curve;

		/*Real min()
		{
			return this->minimum;
		}

		Real max()
		{
			return this->maximum;
		}*/

		//using Numerical::IntegralFunction::f;
		using Numerical::Function::f;
		Real f(Real x)
		{
			Vector3D dp(this->curve->dp(this->segment, x));

			return dp.norm();
		}
	};

	LengthIntegralFunction function;
	Numerical::GaussLegendreIntegralCalculator integral;

	//function.minimum = 0.0;
	//function.maximum = tmin;
	function.segment = segment;
	function.curve = this;

	integral.setMin(0.0);
	integral.setMax(tmin);
	integral.setFunction(&function);
	integral.setNumPoints(points);

	Real total = 0.0;
	Real partitionLength = (tmax - tmin)/static_cast<Real>(parts);

	for (UInt k = 0; k < parts; k++)
	{
		//function.minimum = function.maximum;
		//function.maximum = tmin + static_cast<Real>(k + 1) * partitionLength;
		integral.setMin(integral.getMax());
		integral.setMax(tmin + static_cast<Real>(k + 1) * partitionLength);

		//total += integral.execute(&function, points);

		integral.execute();

		total += integral.getIntegral();
	}

	return total;
}

Real Data::Spline::ICurve::length(UInt segment, Real tmin, Real tmax) const
{
	UInt deg = this->degree(segment);

	return this->length(segment, tmin, tmax, deg*deg, deg + 1);
}

Real Data::Spline::ICurve::length(UInt segment) const
{
	return this->length(segment, 0.0, 1.0);
}

Real Data::Spline::ICurve::length() const
{
	Real l = 0.0;

	for (UInt j = 0; j < this->numSegments(); j++)
	{
		l += this->length(j);
	}

	return l;
}
#endif //#if USE_NUMERICAL

#if USE_NUMERICAL
Real Data::Spline::ICurve::parameterInHalfLength(UInt segment, Real tmin, Real tmax) const
{
	/*struct HalfLengthFunction : public Numerical::IntervalEquationRootFunction
	{
		UInt segment;
		Real minimum;
		Real maximum;
		Real length;
		const Spline::ICurve *curve;

		Real min()
		{
			return this->minimum;
		};

		Real max()
		{
			return this->maximum;
		};

		using Numerical::IntervalEquationRootFunction::f;
		Real f(Real x)
		{
			Real l = (x <= this->minimum) ? 0.0 :
				((x >= this->maximum) ? this->length : this->curve->length(this->segment, this->minimum, x));

			return (l - this->length*0.5);
		};
	};*/

	class HalfLengthFunction : public Numerical::Function
	{
    public:

        UInt segment;
		Real length;
		Real minimum;
		Real maximum;
		const Spline::ICurve *curve;

		using Numerical::Function::f;
		Real f(Real x)
		{
			Real l = (x <= this->minimum) ? 0.0 :
				((x >= this->maximum) ? this->length : this->curve->length(this->segment, this->minimum, x));

			return (l - this->length*0.5);
		};
	};

	HalfLengthFunction f;

	f.segment = segment;
	f.minimum = tmin;
	f.maximum = tmax;
	f.curve = this;
	f.length = this->length(segment, tmin, tmax);

	//Numerical::BisectionEquationRootFinder eqRoot;
	Numerical::FalsePositionEquationRootFinder eqRoot;

	eqRoot.setMin(tmin);
	eqRoot.setMax(tmax);
	eqRoot.setFunction(&f);

	//bool ok = true;

	//Real t = eqRoot.execute(&f, ok);
	eqRoot.execute();
	Real t = eqRoot.getRoot();

	//return ok ? t : -1.0;
	return eqRoot.isOk() ? t : -1.0;
}

Real Data::Spline::ICurve::parameterInLength(UInt segment, Real length) const
{
	//struct LengthFunction : public Numerical::IntervalEquationRootFunction
	class LengthFunction : public Numerical::Function
	{
    public:

		UInt segment;
		Real minimum;
		//Real maximum;
		Real length;
		const Spline::ICurve *curve;

		/*Real min()
		{
			return this->minimum;
		};

		Real max()
		{
			return this->maximum;
		};*/

		//using Numerical::IntervalEquationRootFunction::f;
		using Numerical::Function::f;
		Real f(Real x)
		{
			Real l = (x <= this->minimum) ? 0.0 : this->curve->length(this->segment, this->minimum, x);

			return (l - this->length);
		};
	};

	LengthFunction f;

	f.segment = segment;
	f.minimum = 0.0;
	//f.maximum = 1.0;
	f.curve = this;
	f.length = length;

	//Numerical::BisectionEquationRootFinder eqRoot;
	Numerical::FalsePositionEquationRootFinder eqRoot;

	eqRoot.setMin(0.0);
	eqRoot.setMax(1.0);
	eqRoot.setFunction(&f);

	//bool ok = true;

	//Real t = eqRoot.execute(&f, ok);
	eqRoot.execute();
	Real t = eqRoot.getRoot();

	//return ok ? t : -1.0;
	return eqRoot.isOk() ? t : -1.0;
}

Real Data::Spline::ICurve::parameterInParameterizedLength(UInt segment, Real parameterizedLength) const
{
	return this->parameterInLength(segment, this->length(segment, 0.0, 1.0)*parameterizedLength);
}

Point3D Data::Spline::ICurve::pointInHalfLength(UInt segment, Real tmin, Real tmax) const
{
	Real t = this->parameterInHalfLength(segment, tmin, tmax);

	if ((t < tmin) || (t > tmax))
	{
		return Point3D();
	}

	return this->p(segment, t);
}

Point3D Data::Spline::ICurve::pointInLength(UInt segment, Real length) const
{
	Real t = (length <= 0.0) ? 0.0 :
		((length >= this->length(segment, 0.0, 1.0)) ? 1.0 : this->parameterInLength(segment, length));

	if ((t < 0.0) || (t > 1.0))
	{
		return Point3D();
	}

	return this->p(segment, t);
}

Point3D Data::Spline::ICurve::pointInParameterInParameterizedLength(UInt segment, Real parameterizedLength) const
{
	Real t = (parameterizedLength <= 0.0) ? 0.0 :
		((parameterizedLength >= 1.0) ? 1.0 : this->parameterInParameterizedLength(segment, parameterizedLength));

	if ((t < 0.0) || (t > 1.0))
	{
		return Point3D();
	}

	return this->p(segment, t);
}
#endif //#if USE_NUMERICAL

#if USE_NUMERICAL
Real Data::Spline::ICurve::parameter(UInt segment, const Point3D &pt) const
{
	//struct DistanceFunction : public Numerical::IntervalEquationRootFunction
	class DistanceFunction : public Numerical::Function
	{
    public:

		UInt segment;
		//Real minimum;
		//Real maximum;
		const Point3D *pt;
		const Spline::ICurve *curve;

		/*Real min()
		{
			return this->minimum;
		};

		Real max()
		{
			return this->maximum;
		};*/

		//using Numerical::IntervalEquationRootFunction::f;
		using Numerical::Function::f;
		Real f(Real x)
		{
			Point3D p = this->curve->p(this->segment, x);

			return p.distance(this->pt);
		};
	};

	DistanceFunction f;

	f.segment = segment;
	//f.minimum = 0.0;
	//f.maximum = 1.0;
	f.curve = this;
	f.pt = &pt;

	//Numerical::ClosestBisectionEquationRootFinder eqRoot;
	Numerical::ClosestFalsePositionEquationRootFinder eqRoot;

	eqRoot.setMin(0.0);
	eqRoot.setMax(1.0);
	eqRoot.setFunction(&f);

	//bool ok = true;

	//Real t = eqRoot.execute(&f, ok);
	eqRoot.execute();
	Real t = eqRoot.getRoot();

	//return ok ? t : -1.0;
	return eqRoot.isOk() ? t : -1.0;
}
#endif //#if USE_NUMERICAL

#if USE_NUMERICAL
Real Data::Spline::ICurve::curvature(UInt segment, Real t) const
{
	Vector3D dv(this->dp(segment, t));

	Real dvNorm = dv.squaredNorm();

	if (dvNorm <= Data::getTolerance())
	{
		return 0.0;
	}

	Real den = std::pow(dvNorm, 3.0/2.0);

	Vector3D d2v(this->d2p(segment, t));

	Vector3D cross = dv.cross(d2v);
	Real num = cross.norm();

	return num/den;
}
#endif //#if USE_NUMERICAL

#if USE_GUI
void Data::Spline::ICurve::setR(Real r) const
{
	Data::Colorable::setR(r);

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		this->getPoint(i)->setR(r);
	}
}

void Data::Spline::ICurve::setG(Real g) const
{
	Data::Colorable::setG(g);

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		this->getPoint(i)->setG(g);
	}
}

void Data::Spline::ICurve::setB(Real b) const
{
	Data::Colorable::setB(b);

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		this->getPoint(i)->setB(b);
	}
}

void Data::Spline::ICurve::highlight() const
{
	Data::Colorable::highlight();

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		this->getPoint(i)->highlight();
	}
}

void Data::Spline::ICurve::unhighlight() const
{
	Data::Colorable::unhighlight();

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		this->getPoint(i)->unhighlight();
	}
}

void Data::Spline::ICurve::setDiscretization(UInt discretization) const
{
	this->discretization = discretization;

	if (this->discretization < 1)
	{
		this->discretization = 1;
	}
}

UInt Data::Spline::ICurve::getDiscretization() const
{
	return this->discretization;
}

void Data::Spline::ICurve::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		if (this->dimension() == 2)
        {
            this->explodeAndShrink(this->mid2D());
        }
        else
        {
            this->explodeAndShrink(this->mid3D());
        }
#else
		this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

		for (UInt j = 0; j < this->numSegments(); j++)
		{
			UInt discretization = std::max(static_cast<UInt>(1), this->discretization*(this->numPoints(j) - 1));

			Real incr = 1.0/static_cast<Real>(discretization);

			glBegin(GL_LINE_STRIP);

				for (UInt i = 0; i < discretization; i++)
				{
					Point3D p = this->p(j, i*incr);

					glVertex(p.getX(), p.getY(), p.getZ());
				}

				Point3D p = this->p(j, 1.0);

				glVertex(p.getX(), p.getY(), p.getZ());

			glEnd();
		}

	glPopMatrix();

/*#if USE_NUMERICAL
	//debug
    for (UInt j = 0; j < this->numSegments(); j++)
	{
		for (UInt i = 0; i < 20; i++)
		{
			Real t = static_cast<Real>(std::rand())/static_cast<Real>(RAND_MAX);

			Data::Point3D p = this->p(j, t);

			Real nt = this->parameter(j, p);

			if ((std::fabs(nt) > 0.0001) &&
				(std::fabs(t - nt) > 0.0001))
			{
				this->parameter(j, p);
			}

			//std::cout << "t = " << t << ", nt = " << nt << std::endl;
		}
	}
	//endebug
#endif //#if USE_NUMERICAL*/
}

#if USE_NUMERICAL
void Data::Spline::ICurve::drawSpacedPoints() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glColor(this->getR(), this->getG(), this->getB());

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		if (this->dimension() == 2)
        {
            this->explodeAndShrink(this->mid2D());
        }
        else
        {
            this->explodeAndShrink(this->mid3D());
        }
#else
		this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

		for (UInt j = 0; j < this->numSegments(); j++)
		{
			UInt discretization = std::max(static_cast<UInt>(1), this->discretization*(this->numPoints(j) - 1));

			Real incr = 1.0/static_cast<Real>(discretization);

			Real length = this->length(j, 0.0, 1.0);

			glBegin(GL_POINTS);

				for (UInt i = 0; i < discretization; i++)
				{
					Real t = this->parameterInLength(j, i*incr*length);

					Point3D p = this->p(j, t);

					glVertex(p.getX(), p.getY(), p.getZ());
				}

				Real t = this->parameterInLength(j, length);

				Point3D p = this->p(j, t);

				glVertex(p.getX(), p.getY(), p.getZ());

			glEnd();
		}

	glPopMatrix();
}
#endif //#if USE_NUMERICAL

void Data::Spline::ICurve::drawPoints() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glColor(this->getR(), this->getG(), this->getB());

	for (UInt i = 0; i < this->numPoints(); i++)
	{
#if USE_EXPERIMENTAL_3
		if (this->dimension() == 2)
        {
            this->explodeAndShrink(this->mid2D());
        }
        else
        {
            this->explodeAndShrink(this->mid3D());
        }
#else
		this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

		this->getPoint(i)->draw();
	}
}

void Data::Spline::ICurve::drawPolygon() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		if (this->dimension() == 2)
        {
            this->explodeAndShrink(this->mid2D());
        }
        else
        {
            this->explodeAndShrink(this->mid3D());
        }
#else
		this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

		for (UInt j = 0; j < this->numSegments(); j++)
		{
			glBegin(GL_LINE_STRIP);

				for (UInt i = 0; i < this->numPoints(j); i++)
				{
					Point3D p = this->pt3d(this->point(j, i));

					glVertex(p.getX(), p.getY(), p.getZ());
				}

			glEnd();
		}

	glPopMatrix();
}

#if USE_NUMERICAL
void Data::Spline::ICurve::drawBases() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth(this->isHighlighted())));
	glPointSize(static_cast<GLfloat>(Data::getState().getPointSize(this->isHighlighted())));

	glColor(this->getR(), this->getG(), this->getB());

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		if (this->dimension() == 2)
        {
            this->explodeAndShrink(this->mid2D());
        }
        else
        {
            this->explodeAndShrink(this->mid3D());
        }
#else
		this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

		for (UInt j = 0; j < this->numSegments(); j++)
		{
			UInt discretization = this->discretization*(this->numPoints(j) - 1);

			Real incr = 1.0/static_cast<Real>(discretization);

			for (UInt i = 0; i < discretization; i++)
			{
				Point3D p = this->p(j, i*incr);
				Vector3D dp(this->dp(j, i*incr));
				dp.normalize();
				Vector3D d2p(this->d2p(j, i*incr));
				d2p.normalize();

				Vector3D dx = d2p.cross(dp);
				dx.normalize();

				Vector3D dy = dp.cross(dx);

				glPushMatrix();
					glTranslate(p.getX(), p.getY(), p.getZ());

					glBegin(GL_POINTS);
						glVertex(0.0, 0.0, 0.0);
					glEnd();

					glBegin(GL_LINES);
						glVertex(0.0, 0.0, 0.0);
						glVertex(dp.getX(), dp.getY(), dp.getZ());

						glVertex(0.0, 0.0, 0.0);
						glVertex(dx.getX(), dx.getY(), dx.getZ());

						glVertex(0.0, 0.0, 0.0);
						glVertex(dy.getX(), dy.getY(), dy.getZ());
					glEnd();
				glPopMatrix();
			}

			Point3D p = this->p(j, 1.0);
			Vector3D dp(this->dp(j, 1.0));
			dp.normalize();
			Vector3D d2p(this->d2p(j, 1.0));
			d2p.normalize();

			Vector3D dx = d2p.cross(dp);
			dx.normalize();

			Vector3D dy = dp.cross(dx);

			glPushMatrix();
				glTranslate(p.getX(), p.getY(), p.getZ());

				glBegin(GL_POINTS);
					glVertex(0.0, 0.0, 0.0);
				glEnd();

				glBegin(GL_LINES);
					glVertex(0.0, 0.0, 0.0);
					glVertex(dp.getX(), dp.getY(), dp.getZ());

					glVertex(0.0, 0.0, 0.0);
					glVertex(dx.getX(), dx.getY(), dx.getZ());

					glVertex(0.0, 0.0, 0.0);
					glVertex(dy.getX(), dy.getY(), dy.getZ());
				glEnd();
			glPopMatrix();
		}

	glPopMatrix();
}
#endif //#if USE_NUMERICAL

std::string Data::Spline::ICurve::text() const
{
	std::stringstream s;

	s << "segments: " << this->numSegments() << ", points: " << this->numPoints() << ": ";

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		s << this->getPoint(i)->text();

		if (i < this->numPoints() - 1)
		{
			s << ", ";
		}
	}

	UInt min, max;
	this->degrees(min, max);

	s << ", minDegree = " << min << ", maxDegree = " << max;
#if USE_NUMERICAL
	s << ", length = " << this->length();
#endif //#if USE_NUMERICAL

	return s.str();
}
#endif //#if USE_GUI

Point3D Data::Spline::ICurve::pt3d(const Point *p) const
{
	if (p->dimension() == 1)
	{
		return Point3D(p->getCoord(0));
	}

	if (p->dimension() == 2)
	{
		return Point3D(p->getCoord(0), p->getCoord(1));
	}

	if (p->dimension() == 3)
	{
		return Point3D(p);
	}

	return Point3D();
}

/*SplineFunction *Data::Spline::ICurve::makeSplineFunction() const
{
	return NULL;
}*/

#endif //#if USE_SPLINES
