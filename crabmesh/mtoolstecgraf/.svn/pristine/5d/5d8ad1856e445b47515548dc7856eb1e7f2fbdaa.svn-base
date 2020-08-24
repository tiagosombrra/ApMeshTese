#ifndef _DATA_VECTOR3D_H_
#define _DATA_VECTOR3D_H_

#include "Data/Definitions.h"
#include "Data/Vector.h"

#if USE_EXACT
#include "Data/Exact.h"
#endif //#if USE_EXACT

namespace Data
{
	class Vector3D : public Data::Vector
	{
	public:

		Vector3D(Real x = 0.0, Real y = 0.0, Real z = 0.0);
		Vector3D(const Vector3D &v);
		Vector3D(const Vector &v);
		Vector3D(const Vector *v);
		Vector3D(const Point &p);
		Vector3D(const Point *p);
		Vector3D(const Point &p1, const Point &p2);
		Vector3D(const Point *p1, const Point *p2);
		virtual ~Vector3D();

		virtual void setPosition(Real x, Real y, Real z);

		virtual void setX(Real x);
		virtual Real getX() const;

		virtual void setY(Real y);
		virtual Real getY() const;

		virtual void setZ(Real z);
		virtual Real getZ() const;

		virtual UInt dimension() const;

#if USE_EXPERIMENTAL_3
        virtual void setCoords(const Real coords[]);
		virtual void setCoords(const RealVector &coords);
		virtual RealVector getCoords() const;
		virtual const Real *getCoordsArray() const;
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
        virtual void setCoord(UInt i, Real c);
		virtual Real getCoord(UInt i) const;
#endif //#if USE_EXPERIMENTAL_3

		virtual Vector3D cross(const Vector3D &v) const;
		virtual Vector3D cross(const Vector3D *v) const;

#if USE_EXACT
		virtual EVector3D evector3D() const;
#endif //#if USE_EXACT

#if USE_ARRAY
		using Vector::operator=;
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
		virtual Vector3D &operator=(const Vector3D &v);
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
#if USE_EXPERIMENTAL_3
        virtual Vector3D &operator=(const Vector &p);
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
		using Vector::draw;
		virtual void draw() const;
#endif //#if USE_GUI

#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	protected:

		Real coords[3];
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	};

#if USE_EXACT
	class EVector3D : public Data::EVector
	{
	public:

		EVector3D(EReal x = 0.0, EReal y = 0.0, EReal z = 0.0);
		EVector3D(const EVector &v);
		EVector3D(const EVector *v);
		EVector3D(const EPoint &p);
		EVector3D(const EPoint *p);
		EVector3D(const EPoint &p1, const EPoint &p2);
		EVector3D(const EPoint *p1, const EPoint *p2);
		virtual ~EVector3D();

		virtual void setPosition(EReal x, EReal y, EReal z);

		virtual void setX(EReal x);
		virtual EReal getX() const;

		virtual void setY(EReal y);
		virtual EReal getY() const;

		virtual void setZ(EReal z);
		virtual EReal getZ() const;

		virtual UInt dimension() const;

#if USE_EXPERIMENTAL_3
        virtual void setCoords(const EReal coords[]);
		virtual void setCoords(const ERealVector &coords);
		virtual ERealVector getCoords() const;
		virtual const EReal *getCoordsArray() const;
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
        virtual void setCoord(UInt i, EReal c);
		virtual EReal getCoord(UInt i) const;
#endif //#if USE_EXPERIMENTAL_3

		virtual EVector3D cross(const EVector3D &v) const;
		virtual EVector3D cross(const EVector3D *v) const;

		virtual Vector3D vector3D() const;

#if USE_ARRAY
		using EVector::operator=;
#if USE_EXPERIMENTAL_3
        virtual EVector3D &operator=(const EVector3D &v);
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
		using EVector::draw;
		virtual void draw() const;
#endif //#if USE_GUI

#if USE_EXPERIMENTAL_3
    protected:

		EReal coords[3];
#endif //#if USE_EXPERIMENTAL_3
	};
#endif //#if USE_EXACT

}

#endif //#ifndef _DATA_VECTOR3D_H_
