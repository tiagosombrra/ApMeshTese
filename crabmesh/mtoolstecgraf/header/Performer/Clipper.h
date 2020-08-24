#ifndef _PERFORMER_CLIPPER_H_
#define _PERFORMER_CLIPPER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"
#include "Data/Box.h"

namespace Performer
{
	class Clipper : public Performer::Builder
	{
	public:

		enum Type
		{
			UNKNOWN = 0,
			EDGE,
			POLYGON,
			POLYHEDRON //not implemented
		};

	public:

		Clipper(UInt dimension, const Data::Box &box,
			const Data::GeometricShape *s = NULL, enum Type type = UNKNOWN);
		Clipper(UInt dimension,
			const Data::Point &min, const Data::Point &max,
			const Data::GeometricShape *s = NULL, enum Type type = UNKNOWN);
		virtual ~Clipper();

		virtual void setDimension(UInt dimension);

		virtual void setBox(const Data::Box &box);
		virtual void setBox(const Data::Point &min, const Data::Point &max);

		virtual void setShape(const Data::GeometricShape *s, enum Type type);
		virtual void setShape(const Data::Edge *e);
		virtual void setShape(const Data::Polygon *p);
		virtual void setShape(const Data::Polyhedron *p);

#if USE_EXPERIMENTAL_3
		virtual const Data::Point2DObjList &getPoints2D() const;
		virtual const Data::Point3DObjList &getPoints3D() const;
#else
		virtual const Data::PointObjList &getPoints() const;
#endif //#if USE_EXPERIMENTAL_3

		virtual void execute();

	protected:

		virtual Real interpolate(Real val1, Real val2, Real val) const;
		virtual Data::Point *interpolate(UInt coord,
			const Data::Point &p1, const Data::Point &p2, Real val) const;

		virtual enum Data::Position clipMin(UInt coord, const Data::Point &p, Real val) const;
		virtual enum Data::Position clipMax(UInt coord, const Data::Point &p, Real val) const;
		virtual enum Data::Position clip(UInt coord, const Data::Point &p, Real val, bool min) const;

		virtual Data::Point *clip(UInt coord, const Data::Point&p1, const Data::Point &p2,
			Real val, enum Data::Position pos1, enum Data::Position pos2) const;

		virtual void clip(UInt coord, Real val, bool min);

		virtual void clip();

	protected:

		typedef std::vector<enum Data::Position> PositionVector;

		UInt dimension;

		enum Type type;
		const Data::GeometricShape *shape;

		Data::Box box;

#if USE_EXPERIMENTAL_3
		Data::Point2DObjList points2D;
		Data::Point3DObjList points3D;
#else
		Data::PointObjList points;
#endif //#if USE_EXPERIMENTAL_3
	};
}

#endif //#ifndef _PERFORMER_CLIPPER_H_
