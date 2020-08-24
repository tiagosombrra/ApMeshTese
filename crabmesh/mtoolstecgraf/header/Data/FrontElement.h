#ifndef _DATA_FRONT_ELEMENT_H_
#define _DATA_FRONT_ELEMENT_H_

#include "Data/Definitions.h"
#include "Data/Element.h"

namespace Data
{
	class FrontElement : public Data::Element
	{
	public:

		FrontElement(GeometricShape *shape = NULL, ULInt id = 0);
		virtual ~FrontElement();

		//given a point, returns its rank for this front element
		virtual Real rank(const Point *p, const Data::PointList &ideal) const = 0;

		//crates a new element, connecting this front element with the given
		// vertex. new front elements are created and old front elements
		// are retrieved from the front. these elements are inserted in
		// the two given lists, respectively
		virtual Element *advance(Vertex *v, const Front *f,
			FrontElementList &newFront, FrontElementList &oldFront) const = 0;

		using Element::intercept;

		//tests if the given front element intercepts this front element
		//virtual bool intercept(const FrontElement &e) const = 0;
		virtual bool intercept(const FrontElement *e) const = 0;

#if USE_EXACT
		virtual bool eintercept(const FrontElement *e) const = 0;
#endif //#if USE_EXACT

		//tests if the given point intercepts this front element, for example,
		// if the point p is too close to this front element
		//   OBS.: this virtual method signature exists in class Data::Element
		//virtual bool intercept(const Point *p) const = 0;

		//inverts this front element so that its normal points to the other
		// side (half-space)
		virtual void invert() = 0;

		//tests if the given vertex is in the positive half-space of this
		// front element
		virtual bool accordingToNormal(Vertex *v, bool insideTest = true) const;
		virtual bool accordingToNormal(Point *p, bool insideTest = true) const = 0;

		//testes if this front element is equal to the front element determined
		// by the given vertices
		virtual bool match(const VertexVector &vertices) const = 0;
		virtual bool geometricallyMatch(const VertexVector &vertices) const = 0;

		using Element::have;

#if USE_ARRAY
		using Element::operator=;
#endif //#if USE_ARRAY

#if USE_GUI
		virtual void drawNormal(Real factor) const = 0;
		virtual void drawNormal(Real r, Real g, Real b, Real factor) const;
#endif //#if USE_GUI

		virtual Real quality() const;

		virtual bool isBad() const;

	private:

		//front elements do not have front elements
		virtual bool have(const FrontElement &e) const;
		virtual bool have(const FrontElement *e) const;
	};
}

#endif //#ifndef _DATA_FRONT_ELEMENT_H_
