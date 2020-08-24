#ifndef _TECGRAF_ORIENTED_SURFACE_H_
#define _TECGRAF_ORIENTED_SURFACE_H_

#include "TecGraf/Definitions.h"

#if USE_TECGRAF

#if USE_GUI
#include "Data/ListDisplayable.h"
#endif //#if USE_GUI

namespace TecGraf
{
	class OrientedSurface
#if USE_GUI
		: public Data::ListDisplayable
#endif //#if USE_GUI
	{
	public:

		OrientedSurface();
		virtual ~OrientedSurface();

		virtual void setSurface(Surface *surface);
		virtual Surface *getSurface() const;

		virtual void setPositive(bool positive);
		virtual bool isPositive() const;
		virtual bool isNegative() const;

		virtual void setOrientation(Int orientation);
		virtual Int getOrientation() const;

		virtual void setId(ULInt id);
		virtual ULInt getId() const;

#if USE_GUI
		virtual void setR(Real r) const;
		virtual Real getR() const;

		virtual void setG(Real g) const;
		virtual Real getG() const;

		virtual void setB(Real b) const;
		virtual Real getB() const;

		virtual void highlight() const;
		virtual void unhighlight() const;
		virtual bool isHighlighted() const;

		virtual void clearLists() const;

		using Data::Drawable::draw;
		virtual void draw(bool lighting) const;
		virtual void draw() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		Surface *surface;
		bool positive;

	};
}

#endif //#if USE_TECGRAF

#endif //#ifndef _TECGRAF_ORIENTED_SURFACE_H_
