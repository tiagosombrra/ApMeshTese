#ifndef _DATA_POINT_H_
#define _DATA_POINT_H_

#include "Data/Definitions.h"
#include "Data/Vector.h"

#if USE_EXACT
#include "Data/Exact.h"
#endif //#if USE_EXACT

namespace Data
{
	class Point
#if USE_GUI
		: public Data::Colorable
#endif //#if USE_GUI
	{
	public:

#if USE_EXPERIMENTAL_3
		Point();
#else
		Point(UInt dimension = 0);
		Point(const RealVector &coords);
		Point(const Point &p);
		Point(const Point *p);
		Point(const Vector &v);
		Point(const Vector *v);
		Point(const Point &p, const Vector &v);
		Point(const Point *p, const Vector *v);
#endif //#if USE_EXPERIMENTAL_3
		virtual ~Point();

#if USE_EXPERIMENTAL_3
        virtual UInt dimension() const = 0;
#else
		virtual void setDimension(UInt dimension);
		virtual UInt dimension() const;
#endif //#if USE_EXPERIMENTAL_3

		///assumes coords[] has dimension this->dim
		///copies contents from coords[] to this->coords
#if USE_EXPERIMENTAL_3
		virtual void setCoords(const Real coords[]) = 0;
		virtual void setCoords(const RealVector &coords) = 0;
		virtual RealVector getCoords() const = 0;
		virtual const Real *getCoordsArray() const = 0;
#else
		virtual void setCoords(const Real coords[]);
		virtual void setCoords(const RealVector &coords);
		virtual RealVector getCoords() const;
		virtual const Real *getCoordsArray() const;
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
		virtual void setCoord(UInt i, Real c) = 0;
		virtual Real getCoord(UInt i) const = 0;
#else
		virtual void setCoord(UInt i, Real c);
		virtual Real getCoord(UInt i) const;
#endif //#if USE_EXPERIMENTAL_3

		virtual void sum(const Vector &v);
		virtual void sum(const Vector *v);

		virtual Real distance(const Point &p) const;
		virtual Real distance(const Point *p) const;

		virtual Real squaredDistance(const Point &p) const;
		virtual Real squaredDistance(const Point *p) const;

		//matrix should be (this->getDimension() + 1) x (this->getDimension() + 1)
		virtual void transform(Real **matrix);

		virtual bool match(const Point &p) const;
		virtual bool match(const Point *p) const;

		virtual bool equal(const Point &p) const;
		virtual bool equal(const Point *p) const;

#if (!USE_EXPERIMENTAL_3)
#if USE_EXACT
		virtual EPoint epoint() const;
#endif //#if USE_EXACT
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
#if USE_ARRAY
		virtual Point &operator=(const Point &p);
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_3)

#if USE_GUI
		/*virtual void setSize(Real size) const;
		virtual Real getSize() const;*/

		virtual void highlight() const;
		virtual void unhighlight() const;

		using Colorable::draw;
		virtual void draw() const;

		using Colorable::fill;
		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;

		virtual bool isInVisibleSpace() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

#if (!USE_EXPERIMENTAL_3)
#if USE_ARRAY
		UShort dim;
#if USE_EXPERIMENTAL_2
		Real coords[3];
#else
		Real *coords;
#endif //#if USE_EXPERIMENTAL_2
#else
		RealVector coords;
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_3)

/*#if USE_GUI
		mutable Real size;
#endif //#if USE_GUI*/
	};

#if USE_EXACT
	class EPoint
#if USE_GUI
		: public Data::Colorable
#endif //#if USE_GUI
	{
	public:

		EPoint(UInt dimension = 0);
		EPoint(const ERealVector &coords);
		EPoint(const EPoint &p);
		EPoint(const EPoint *p);
		EPoint(const EVector &v);
		EPoint(const EVector *v);
		EPoint(const EPoint &p, const EVector &v);
		EPoint(const EPoint *p, const EVector *v);
		virtual ~EPoint();

		virtual void setDimension(UInt dimension);
		virtual UInt dimension() const;

		///assumes coords[] has dimension this->dim
		///copies contents from coords[] to this->coords
		virtual void setCoords(const EReal coords[]);
		virtual void setCoords(const ERealVector &coords);
		virtual ERealVector getCoords() const;
		virtual const EReal *getCoordsArray() const;

		virtual void setCoord(UInt i, EReal c);
		virtual EReal getCoord(UInt i) const;

		virtual void sum(const EVector &v);
		virtual void sum(const EVector *v);

		virtual EReal distance(const EPoint &p) const;
		virtual EReal distance(const EPoint *p) const;

		virtual EReal squaredDistance(const EPoint &p) const;
		virtual EReal squaredDistance(const EPoint *p) const;

		//matrix should be (this->getDimension() + 1) x (this->getDimension() + 1)
		virtual void transform(EReal **matrix);

		virtual bool match(const EPoint &p) const;
		virtual bool match(const EPoint *p) const;

		/*virtual bool equal(const EPoint &p) const;
		virtual bool equal(const EPoint *p) const;*/

#if (!USE_EXPERIMENTAL_3)
		virtual Point point() const;
#endif //#if (!USE_EXPERIMENTAL_3)

#if USE_ARRAY
		virtual EPoint &operator=(const EPoint &p);
#endif //#if USE_ARRAY

#if USE_GUI
		/*virtual void setSize(Real size) const;
		virtual Real getSize() const;*/

		virtual void highlight() const;
		virtual void unhighlight() const;

		using Colorable::draw;
		virtual void draw() const;

        using Colorable::fill;
#if USE_EXPERIMENTAL_3
		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const = 0;
#else
		virtual UInt fill(GLfloat *coord = NULL, GLfloat *color = NULL, GLfloat *normal = NULL) const;
#endif //#if USE_EXPERIMENTAL_3

		virtual bool isInVisibleSpace() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

#if USE_ARRAY
		UShort dim;
#if USE_EXPERIMENTAL_2
		EReal coords[3];
#else
		EReal *coords;
#endif //#if USE_EXPERIMENTAL_2
#else
		ERealVector coords;
#endif //#if USE_ARRAY

/*#if USE_GUI
		mutable Real size;
#endif //#if USE_GUI*/
	};
#endif //#if USE_EXACT
}

#endif //#ifndef _DATA_POINT_H_
