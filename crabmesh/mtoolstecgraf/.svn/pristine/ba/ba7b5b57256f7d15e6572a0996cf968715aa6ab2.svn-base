#ifndef _DATA_POLYHEDRON_H_
#define _DATA_POLYHEDRON_H_

#include "Data/Definitions.h"
#include "Data/GeometricShape.h"

namespace Data
{
	class Polyhedron : public Data::GeometricShape
	{
	public:

		Polyhedron();
#if (!USE_EXPERIMENTAL_4)
		Polyhedron(UInt numPoints);
		Polyhedron(UInt numPoints, UInt numPolygons);
#endif //#if (!USE_EXPERIMENTAL_4)
		virtual ~Polyhedron();

		virtual UInt numSides() const = 0;

#if (!USE_EXPERIMENTAL_4)
		virtual void setPolygons(const PolygonVector &polygons);
		virtual const PolygonVector &getPolygons() const;

		virtual void setPolygon(UInt i, Polygon *p);
		virtual Polygon *getPolygon(UInt i) const;
#endif //#if (!USE_EXPERIMENTAL_4)

#if (!USE_EXPERIMENTAL_4)
		using GeometricShape::add;
		using GeometricShape::remove;

		virtual void add(Polygon *p);
		virtual void remove(Polygon *p);
#endif //#if (!USE_EXPERIMENTAL_4)

#if USE_EXPERIMENTAL_4
		virtual Real surface() const = 0;
#else
		virtual Real surface() const;
#endif //#if USE_EXPERIMENTAL_4
		virtual Real volume() const = 0;
		virtual Real orientedVolume() const = 0;

		virtual Real orientedSize() const;

		virtual bool in(const Point &p) const = 0;
		virtual bool in(const Point *p) const = 0;

		virtual bool on(const Point &p) const = 0;
		virtual bool on(const Point *p) const = 0;

		virtual bool out(const Point &p) const = 0;
		virtual bool out(const Point *p) const = 0;

#if (!USE_EXPERIMENTAL_4)
		virtual bool match(const GeometricShape &s) const;
		virtual bool match(const GeometricShape *s) const;
		virtual bool match(const Polyhedron &p) const;
		virtual bool match(const Polyhedron *p) const;

		virtual bool equal(const GeometricShape &s) const;
		virtual bool equal(const GeometricShape *s) const;
		virtual bool equal(const Polyhedron &p) const;
		virtual bool equal(const Polyhedron *p) const;
#endif //#if (!USE_EXPERIMENTAL_4)

#if USE_ARRAY
#if (!USE_EXPERIMENTAL_4)
		using GeometricShape::operator=;
		virtual Polyhedron &operator=(const Polyhedron &p);
#endif //#if (!USE_EXPERIMENTAL_4)
#endif //#if USE_ARRAY

#if USE_GUI
		//virtual void draw() const = 0;
#if (!USE_EXPERIMENTAL_4)
		virtual void setColor(Real r, Real g, Real b) const;
		virtual void highlight() const;
		virtual void unhighlight() const;
#endif //#if (!USE_EXPERIMENTAL_4)

		virtual std::string text() const;
#endif //#if USE_GUI

#if (!USE_EXPERIMENTAL_4)
	protected:

		PolygonVector polygons;
#endif //#if (!USE_EXPERIMENTAL_4)
	};
}

#endif //#ifndef _DATA_POLYHEDRON_H_
