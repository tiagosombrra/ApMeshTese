#ifndef _FUNCMESH_FRONT_ADVANCER_H_
#define _FUNCMESH_FRONT_ADVANCER_H_

#include "FuncMesh/Definitions.h"
//#include "Performer/FrontAdvancer.h"
#include "MJMesh/FrontAdvancer.h"

namespace FuncMesh
{
	//class FrontAdvancer : public Performer::FrontAdvancer
	class FrontAdvancer : public MJMesh::FrontAdvancer
	{
	public:

		FrontAdvancer(const Data::Boundary *boundary = NULL);
		virtual ~FrontAdvancer();

		//virtual void setMakeElement(Data::ElementTuple (*fMakeElement)(const Data::FrontElement *, const Data::Point *));

		virtual void setRadius(Real (*fRadius)(const Real p1[], const Real p2[]));
		virtual void setRadius(Real (*fRadius)(const Real p1[], const Real p2[], const Real p3[]));
		//virtual void setRadius(Real (*fRadius)(Real, Real, Real, Real));
		//virtual void setRadius(Real (*fRadius)(Real, Real, Real, Real, Real, Real, Real, Real, Real));

		virtual void setIdeal(bool (*fIdeal)(const Real p1[], const Real p2[], Real ideal[]));
		virtual void setIdeal(bool (*fIdeal)(const Real p1[], const Real p2[], const Real p3[], Real ideal[]));
		//virtual void setIdeal(bool (*fIdeal)(Real, Real, Real, Real, Real *, Real *));
		//virtual void setIdeal(bool (*fIdeal)(Real, Real, Real, Real, Real, Real, Real, Real, Real, Real *, Real *, Real *));

	private:

		//virtual Data::ElementTuple makeElement(const Data::FrontElement *e, const Data::Point *p) const;

		Real radius(const Data::FrontElement *element) const;

		Data::PointList ideal(const Data::FrontElement *element, Real factor = 1.0) const;

	protected:

		//Data::ElementTuple (*fMakeElement)(const Data::FrontElement *, const Data::Point *);

		Real (*fRadius2D)(const Real p1[], const Real p2[]);
		Real (*fRadius3D)(const Real p1[], const Real p2[], const Real p3[]);
		//Real (*fRadius2D)(Real, Real, Real, Real);
		//Real (*fRadius3D)(Real, Real, Real, Real, Real, Real, Real, Real, Real);

		bool (*fIdeal2D)(const Real p1[], const Real p2[], Real ideal[]);
		bool (*fIdeal3D)(const Real p1[], const Real p2[], const Real p3[], Real ideal[]);
		//bool (*fIdeal2D)(Real, Real, Real, Real, Real *, Real *);
		//bool (*fIdeal3D)(Real, Real, Real, Real, Real, Real, Real, Real, Real, Real *, Real *, Real *);
	};
}

#endif //#ifndef _FUNCMESH_FRONT_ADVANCER_H_
