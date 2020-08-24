#ifndef _DATA_VECTOR2D_H_
#define _DATA_VECTOR2D_H_

#include "Data/Definitions.h"
#include "Data/Vector.h"

#if USE_EXACT
#include "Data/Exact.h"
#endif //#if USE_EXACT

namespace Data
{
	class Vector2D : public Data::Vector
	{
	public:

		Vector2D(Real x = 0.0, Real y = 0.0);
		Vector2D(const Vector2D &v);
		Vector2D(const Vector &v);
		Vector2D(const Vector *v);
		Vector2D(const Point &p);
		Vector2D(const Point *p);
		Vector2D(const Point &p1, const Point &p2);
		Vector2D(const Point *p1, const Point *p2);
		virtual ~Vector2D();

		virtual void setPosition(Real x, Real y);

		virtual void setX(Real x);
		virtual Real getX() const;

		virtual void setY(Real y);
		virtual Real getY() const;

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

		virtual Real cross(const Vector2D &v) const;
		virtual Real cross(const Vector2D *v) const;

		virtual Real scaledCross(const Vector2D &v) const;
		virtual Real scaledCross(const Vector2D *v) const;

		virtual Real orientedAngle() const;
		virtual Real orientedAngle(const Vector2D &v) const;
		virtual Real orientedAngle(const Vector2D *v) const;

		virtual bool left(const Vector2D &v) const;
		virtual bool left(const Vector2D *v) const;

		virtual bool left(const Point2D &p) const;
		virtual bool left(const Point2D *p) const;

		virtual bool right(const Vector2D &v) const;
		virtual bool right(const Vector2D *v) const;

		virtual bool right(const Point2D &p) const;
		virtual bool right(const Point2D *p) const;

#if USE_EXACT
		virtual EVector2D evector2D() const;
#endif //#if USE_EXACT

#if USE_ARRAY
		using Vector::operator=;
#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
		virtual Vector2D &operator=(const Vector2D &v);
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
#if USE_EXPERIMENTAL_3
        virtual Vector2D &operator=(const Vector &p);
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
		using Vector::draw;
		virtual void draw() const;
#endif //#if USE_GUI

#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	protected:

		Real coords[2];
#endif //#if ((USE_EXPERIMENTAL_3) || ((USE_EXPERIMENTAL) && (!USE_EXPERIMENTAL_2)))
	};

#if USE_EXACT
	class EVector2D : public Data::EVector
	{
	public:

		EVector2D(EReal x = 0.0, EReal y = 0.0);
		EVector2D(const EVector &v);
		EVector2D(const EVector *v);
		EVector2D(const EPoint &p);
		EVector2D(const EPoint *p);
		EVector2D(const EPoint &p1, const EPoint &p2);
		EVector2D(const EPoint *p1, const EPoint *p2);
		virtual ~EVector2D();

		virtual void setPosition(EReal x, EReal y);

		virtual void setX(EReal x);
		virtual EReal getX() const;

		virtual void setY(EReal y);
		virtual EReal getY() const;

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

		virtual EReal cross(const EVector2D &v) const;
		virtual EReal cross(const EVector2D *v) const;

		virtual EReal scaledCross(const EVector2D &v) const;
		virtual EReal scaledCross(const EVector2D *v) const;

		virtual EReal orientedAngle() const;
		virtual EReal orientedAngle(const EVector2D &v) const;
		virtual EReal orientedAngle(const EVector2D *v) const;

		virtual bool left(const EVector2D &v) const;
		virtual bool left(const EVector2D *v) const;

		virtual bool left(const EPoint2D &p) const;
		virtual bool left(const EPoint2D *p) const;

		virtual bool right(const EVector2D &v) const;
		virtual bool right(const EVector2D *v) const;

		virtual bool right(const EPoint2D &p) const;
		virtual bool right(const EPoint2D *p) const;

		virtual Vector2D vector2D() const;

#if USE_ARRAY
		using EVector::operator=;
#if USE_EXPERIMENTAL_3
        virtual EVector2D &operator=(const EVector2D &v);
#endif //#if USE_EXPERIMENTAL_3
#endif //#if USE_ARRAY

#if USE_GUI
		using EVector::draw;
		virtual void draw() const;
#endif //#if USE_GUI

#if USE_EXPERIMENTAL_3
    protected:

		EReal coords[2];
#endif //#if USE_EXPERIMENTAL_3
	};
#endif //#if USE_EXACT
}

#endif //#ifndef _DATA_VECTOR2D_H_
