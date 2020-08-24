#include "Data/Spline/RationalCurve.h"

#if USE_SPLINES

#include "Data/Point3D.h"

using namespace Data;
using namespace Data::Spline;

Data::Spline::RationalCurve::RationalCurve() :
	Data::Spline::ICurve::ICurve()
{

}

Data::Spline::RationalCurve::~RationalCurve()
{

}

void Data::Spline::RationalCurve::add(Point *p)
{
	this->add(p, 1.0);
}

void Data::Spline::RationalCurve::setWeight(UInt i, Real weight)
{
	this->weights[i] = weight;
}

Real Data::Spline::RationalCurve::getWeight(UInt i) const
{
	return this->weights[i];
}

void Data::Spline::RationalCurve::setWeight(UInt /*segment*/, UInt i, Real weight)
{
	this->setWeight(i, weight);
}

Real Data::Spline::RationalCurve::getWeight(UInt /*segment*/, UInt i) const
{
	return this->getWeight(i);
}

Point3D Data::Spline::RationalCurve::p(UInt segment, Real t) const
{
	Real sumBlends = 0.0;

	Vector3D sum;

	for (UInt i = 0; i < this->numPoints(segment); i++)
	{
		Vector3D pt(this->pt3d(this->point(segment, i)));

		Real blend = this->blend(segment, i, t);

		blend *= this->getWeight(segment, i);

		sumBlends += blend;

		pt.multiply(blend);

		sum.sum(pt);
	}

	if (std::fabs(sumBlends) > Data::getTolerance())
	{
		sum.multiply(1.0/sumBlends);
	}
	else if (this->numPoints() > 0)
	{
		sum = this->pt3d(this->point(segment, 0));
	}

	return Point3D(sum);
}

#if USE_NUMERICAL
Point3D Data::Spline::RationalCurve::dp(UInt segment, Real t) const
{
	Real sumBlends = 0.0;
	Real sumDBlends = 0.0;

	Vector3D sum;
	Vector3D dSum;

	for (UInt i = 0; i < this->numPoints(segment); i++)
	{
		Vector3D pt(this->pt3d(this->point(segment, i)));
		Vector3D dPt = pt;

		Real blend = this->blend(segment, i, t);
		Real dBlend = this->dBlend(segment, i, t);

		blend *= this->getWeight(segment, i);
		dBlend *= this->getWeight(segment, i);

		sumBlends += blend;
		sumDBlends += dBlend;

		pt.multiply(blend);
		dPt.multiply(dBlend);

		sum.sum(pt);
		dSum.sum(dPt);
	}

	if (std::fabs(sumBlends) > Data::getTolerance())
	{
		dSum.multiply(1.0/sumBlends);
		sum.multiply(-sumDBlends/(sumBlends*sumBlends));

		dSum.sum(sum);
	}
	else if (this->numPoints() > 0)
	{
		dSum = this->pt3d(this->point(segment, 0));
	}

	return Point3D(dSum);
}

Point3D Data::Spline::RationalCurve::d2p(UInt segment, Real t) const
{
	Real sumBlends = 0.0;
	Real sumDBlends = 0.0;
	Real sumD2Blends = 0.0;

	Vector3D sum;
	Vector3D dSum;
	Vector3D d2Sum;

	for (UInt i = 0; i < this->numPoints(segment); i++)
	{
		Vector3D pt(this->pt3d(this->point(segment, i)));
		Vector3D dPt = pt;
		Vector3D d2Pt = pt;

		Real blend = this->blend(segment, i, t);
		Real dBlend = this->dBlend(segment, i, t);
		Real d2Blend = this->d2Blend(segment, i, t);

		blend *= this->getWeight(segment, i);
		dBlend *= this->getWeight(segment, i);
		d2Blend *= this->getWeight(segment, i);

		sumBlends += blend;
		sumDBlends += dBlend;
		sumD2Blends += d2Blend;

		pt.multiply(blend);
		dPt.multiply(dBlend);
		d2Pt.multiply(d2Blend);

		sum.sum(pt);
		dSum.sum(dPt);
		d2Sum.sum(d2Pt);
	}

	if (std::fabs(sumBlends) > Data::getTolerance())
	{
		d2Sum.multiply(1.0/sumBlends);
		dSum.multiply(-2.0*sumDBlends/(sumBlends*sumBlends));
		sum.multiply(2.0*sumDBlends*sumDBlends/(sumBlends*sumBlends*sumBlends) - sumD2Blends/(sumBlends*sumBlends));

		d2Sum.sum(dSum);
		d2Sum.sum(sum);
	}
	else if (this->numPoints() > 0)
	{
		d2Sum = this->pt3d(this->point(segment, 0));
	}

	return Point3D(d2Sum);
}
#endif //#if USE_NUMERICAL

#endif //#if USE_SPLINES
