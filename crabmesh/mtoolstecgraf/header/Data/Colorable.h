#ifndef _DATA_COLORABLE_H_
#define _DATA_COLORABLE_H_

#include "Data/Definitions.h"

#if USE_GUI

#include "Data/Drawable.h"

namespace Data
{
	class Colorable : public Data::Drawable
	{
	public:

		Colorable(Real r = 1.0, Real g = 1.0, Real b = 1.0);
		virtual ~Colorable();

		virtual void setR(Real r) const;
		virtual Real getR() const;

		virtual void setG(Real g) const;
		virtual Real getG() const;

		virtual void setB(Real b) const;
		virtual Real getB() const;

		virtual void highlight() const;
		virtual void unhighlight() const;
		virtual bool isHighlighted() const;

	protected:

		mutable Real r;
		mutable Real g;
		mutable Real b;

		mutable bool highlighted;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _DATA_COLORABLE_H_
