#ifndef _DATA_BOX_H_
#define _DATA_BOX_H_

#include "Data/Definitions.h"
#include "Data/GeometricShape.h"

#if USE_TREE_CELL_POOL
#include "Data/Poolable.h"
#endif //#if USE_TREE_CELL_POOL

namespace Data
{
	//for consistency, max->getCoord(i) >= min->getCoord(i), for all i.

	class Box : public Data::GeometricShape
#if USE_TREE_CELL_POOL
		, public Data::Poolable<Data::Box>
#endif //#if USE_TREE_CELL_POOL
	{
#if USE_TREE_CELL_POOL
		USING_POOLABLE(Data::Box)
#endif //#if USE_TREE_CELL_POOL

	public:

		Box();
		Box(const Box &box);
		Box(Point *min, Point *max);
		Box(const Point &min, const Point &max);
		virtual ~Box();

#if USE_EXPERIMENTAL_4
        virtual void setPoint(UInt i, Point *p);
		virtual Point *getPoint(UInt i) const;
#endif //#if USE_EXPERIMENTAL_4

		virtual UInt numPoints() const;

		virtual void setBox(Point *min, Point *max);
		virtual void setBox(const Point &min, const Point &max);

		virtual void setMin(Point *p);
		virtual Point *getMin() const;

		virtual void setMax(Point *p);
		virtual Point *getMax() const;

//#if USE_EXPERIMENTAL
		virtual UInt dimension() const;
//#endif //#if USE_EXPERIMENTAL

		virtual Real orientedSize() const;

		///opt*() methods assume that this box and the input parameter
		///  (either point or box) have the same dimension

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		virtual bool optIn(const Point &p) const;
		virtual bool optIn(const Point *p) const;

		virtual bool in(const Box &box) const;
		virtual bool in(const Box *box) const;

		virtual bool optIn(const Box &box) const;
		virtual bool optIn(const Box *box) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		virtual bool optOn(const Point &p) const;
		virtual bool optOn(const Point *p) const;

		virtual bool on(const Box &box) const;
		virtual bool on(const Box *box) const;

		virtual bool optOn(const Box &box) const;
		virtual bool optOn(const Box *box) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual bool optOut(const Point &p) const;
		virtual bool optOut(const Point *p) const;

		virtual bool out(const Box &box) const;
		virtual bool out(const Box *box) const;

		virtual bool optOut(const Box &box) const;
		virtual bool optOut(const Box *box) const;

		virtual bool intercept(const Box &box) const;
		virtual bool intercept(const Box *box) const;

		virtual bool optIntercept(const Box &box) const;
		virtual bool optIntercept(const Box *box) const;

		virtual bool outOf(const Box &box) const;
		virtual bool outOf(const Box *box) const;

#if USE_ARRAY
		using GeometricShape::operator=;
		virtual Box &operator=(const Box &box);
#endif //#if USE_ARRAY

#if USE_GUI
		using GeometricShape::draw;
		virtual void draw() const;
		//virtual void draw(bool fill) const;

		virtual std::string text() const;
#endif //#if USE_GUI

#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	protected:

		Point *points[2];
#endif //#if ((USE_EXPERIMENTAL) || (USE_EXPERIMENTAL_4))
	};
}

#endif //#ifndef _DATA_BOX_H_
