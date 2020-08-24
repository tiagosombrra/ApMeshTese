#ifndef _DATA_SPLINE_ISURFACE_H_
#define _DATA_SPLINE_ISURFACE_H_

#include "Data/Spline/Definitions.h"

#if USE_SPLINES

#include "Data/GeometricShape.h"

namespace Data
{
namespace Spline
{
	class ISurface : public Data::GeometricShape
	{
	public:

		ISurface();
		virtual ~ISurface();

		//returns the column
		virtual UInt add(UInt row, Point *p) = 0;

#if USE_EXPERIMENTAL_3
		virtual Point2D mid2D() const;
		virtual Point3D mid3D() const;
#else
		virtual Point mid() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual Box box() const;

		virtual UInt numPoints() const;
		virtual UInt numPointsU() const = 0;
		virtual UInt numPointsV() const = 0;
		virtual UInt numPoints(UInt segmentCol, UInt segmentRow) const;
		virtual UInt numPointsU(UInt segmentCol) const;
		virtual UInt numPointsV(UInt segmentRow) const;

		virtual void setPoint(UInt i, Point *p);
		virtual Point *getPoint(UInt i) const;

		virtual void setPoint(UInt i, UInt j, Point *p) = 0;
		virtual Point *getPoint(UInt i, UInt j) const = 0;

		virtual Real orientedSize() const;

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual UInt numSegmentsRow() const;
		virtual UInt numSegmentsCol() const;

		virtual Point *point(UInt segmentCol, UInt segmentRow, UInt i, UInt j) const;

		virtual bool haveAllPoints(UInt segmentCol, UInt segmentRow) const;

		virtual void move(UInt i, UInt j, Real x, Real y, Real z = 0.0);

		virtual Point3D p(UInt segmentCol, UInt segmentRow, Real u, Real v) const;
#if USE_NUMERICAL
		virtual Point3D dup(UInt segmentCol, UInt segmentRow, Real u, Real v) const;
		virtual Point3D dvp(UInt segmentCol, UInt segmentRow, Real u, Real v) const;
		virtual Point3D d2uup(UInt segmentCol, UInt segmentRow, Real u, Real v) const;
		virtual Point3D d2uvp(UInt segmentCol, UInt segmentRow, Real u, Real v) const;
		virtual Point3D d2vvp(UInt segmentCol, UInt segmentRow, Real u, Real v) const;
		virtual Vector3D normal(UInt segmentCol, UInt segmentRow, Real u, Real v) const;
#endif //#if USE_NUMERICAL

		virtual Real blendU(UInt segmentCol, UInt j, Real u) const;
		virtual Real blendV(UInt segmentRow, UInt i, Real v) const;
#if USE_NUMERICAL
		virtual Real dBlendU(UInt segmentCol, UInt j, Real u) const;
		virtual Real dBlendV(UInt segmentRow, UInt i, Real v) const;
		virtual Real d2BlendU(UInt segmentCol, UInt j, Real u) const;
		virtual Real d2BlendV(UInt segmentRow, UInt i, Real v) const;
#endif //#if USE_NUMERICAL

		virtual Point3D pt3d(const Point *p) const;

#if USE_GUI
		virtual void setR(Real r) const;
		virtual void setG(Real g) const;
		virtual void setB(Real b) const;

		virtual void highlight() const;
		virtual void unhighlight() const;

		virtual void setDiscretization(UInt discretizationU, UInt discretizationV) const;
		virtual UInt getDiscretizationU() const;
		virtual UInt getDiscretizationV() const;

		using GeometricShape::draw;
		virtual void draw() const;

		virtual void drawPoints() const;
		virtual void drawPolygon() const;
#if USE_NUMERICAL
		virtual void drawBases() const;
#endif //#if USE_NUMERICAL

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual SplineFunction *makeUSplineFunction() const = 0;
		virtual SplineFunction *makeVSplineFunction() const = 0;

	protected:

		typedef std::vector<std::vector<Point3D> > Point3DMatrix;
		typedef std::vector<std::vector<Vector3D> > Vector3DMatrix;

	protected:

		SplineFunction *uFunction;
		SplineFunction *vFunction;

#if USE_GUI
		mutable UInt discretizationU;
		mutable UInt discretizationV;
#endif //#if USE_GUI

	};
}
}

#endif //#if USE_SPLINES

#endif //#ifndef _DATA_SPLINE_ISURFACE_H_
