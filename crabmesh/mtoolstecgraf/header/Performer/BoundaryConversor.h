#ifndef _PERFORMER_BOUNDARY_CONVERSOR_H_
#define _PERFORMER_BOUNDARY_CONVERSOR_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class BoundaryConversor : public Performer::Builder
	{
	public:

		BoundaryConversor(Data::Boundary *original = NULL);
		virtual ~BoundaryConversor();

		virtual void setOriginal(Data::Boundary *original);
		virtual Data::Boundary *getConverted() const;

		virtual void execute();

	protected:

		virtual bool worse(Real q1, Real q2) const;
		virtual Real quality(const Data::Point *p1, const Data::Point *p2, const Data::Point *p3) const;

		virtual void addVertex(Real x, Real y, ULInt id) = 0;
		virtual void addVertex(Real x, Real y, Real z, ULInt id) = 0;
		virtual void addFrontElement(ULInt id1, ULInt id2, ULInt id) = 0;
		virtual void addFrontElement(ULInt id1, ULInt id2, ULInt id3, ULInt id) = 0;

		virtual Data::Boundary *consolidate() const = 0;

	protected:

		Data::Boundary *original;
		Data::Boundary *converted;
	};
}

#endif //#ifndef _PERFORMER_BOUNDARY_CONVERSOR_H_
