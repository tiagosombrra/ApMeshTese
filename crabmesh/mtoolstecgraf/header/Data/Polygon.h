#ifndef _DATA_POLYGON_H_
#define _DATA_POLYGON_H_

#include "Data/Definitions.h"
#include "Data/GeometricShape.h"

namespace Data
{
	class Polygon : public Data::GeometricShape
	{
	public:

		Polygon();
#if (!USE_EXPERIMENTAL_4)
		Polygon(UInt numPoints);
#endif //#if (!USE_EXPERIMENTAL_4)
		virtual ~Polygon();

		virtual UInt numSides() const;

		virtual Real perimeter() const;
		virtual Real surface() const = 0;
		virtual Real orientedSurface() const = 0;

		virtual Real orientedSize() const;

		//normal is not normalized in 2D (it would be (0, 0, 1))
		//normal is normalized in 3D
		//normal is inexistent in other dimensions (0, 0, 0)
		virtual Vector3D normal() const;

#if USE_EXACT
		//enormal is not normalized
		virtual EVector3D enormal() const;
#endif //#if USE_EXACT

		virtual bool in(const Point &p) const = 0;
		virtual bool in(const Point *p) const = 0;

		virtual bool on(const Point &p) const = 0;
		virtual bool on(const Point *p) const = 0;

		virtual bool out(const Point &p) const = 0;
		virtual bool out(const Point *p) const = 0;

		virtual bool match(const GeometricShape &s) const;
		virtual bool match(const GeometricShape *s) const;
		virtual bool match(const Polygon &p) const;
		virtual bool match(const Polygon *p) const;

		virtual bool equal(const GeometricShape &s) const;
		virtual bool equal(const GeometricShape *s) const;
		virtual bool equal(const Polygon &p) const;
		virtual bool equal(const Polygon *p) const;

#if USE_ARRAY
		using GeometricShape::operator=;
#endif //#if USE_ARRAY

#if USE_GUI
		//virtual void draw() const = 0;

		virtual std::string text() const;
#endif //#if USE_GUI
	};
}

#endif //#ifndef _DATA_POLYGON_H_
