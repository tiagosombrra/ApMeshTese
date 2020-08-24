#ifndef _DATA_VECTOR_H_
#define _DATA_VECTOR_H_

#include "Data/Definitions.h"
#include "Data/Colorable.h"

#if USE_EXACT
#include "Data/Exact.h"
#endif //#if USE_EXACT

#if USE_EXPERIMENTAL_3

#define VECTORd(NAME, dimension)                        \
    Data::Vector2D NAME ## 2d;                          \
    Data::Vector3D NAME ## 3d;                          \
    Data::Vector &NAME = ((dimension) == 2) ?           \
        static_cast<Data::Vector &>(NAME ## 2d) :       \
        static_cast<Data::Vector &>(NAME ## 3d);

#define VECTORc(NAME, dimension, coords)                \
    Data::Vector2D NAME ## 2d(coords);                  \
    Data::Vector3D NAME ## 3d;                          \
    Data::Vector &NAME = ((dimension) == 2) ?           \
        static_cast<Data::Vector &>(NAME ## 2d) :       \
        (NAME ## 3d = Data::Vector3D(coords), static_cast<Data::Vector &>(NAME ## 3d));

#define VECTORv(NAME, dimension, vec)                   \
    Data::Vector2D NAME ## 2d(vec);                     \
    Data::Vector3D NAME ## 3d;                          \
    Data::Vector &NAME = ((dimension) == 2) ?           \
        static_cast<Data::Vector &>(NAME ## 2d) :       \
        (NAME ## 3d = Data::Vector3D(vec), static_cast<Data::Vector &>(NAME ## 3d));

#define VECTORp VECTORv

#define VECTORpp(NAME, dimension, p1, p2)               \
    Data::Vector2D NAME ## 2d(p1, p2);                  \
    Data::Vector3D NAME ## 3d;                          \
    Data::Vector &NAME = ((dimension) == 2) ?           \
        static_cast<Data::Vector &>(NAME ## 2d) :       \
        (NAME ## 3d = Data::Vector3D(p1, p2), static_cast<Data::Vector &>(NAME ## 3d));

#else

#define VECTORd(NAME, dimension)                        \
    Data::Vector NAME(dimension);

#define VECTORc(NAME, dimension, coords)                \
    Data::Vector NAME(coords);

#define VECTORv(NAME, dimension, vec)                   \
    Data::Vector NAME(vec);

#define VECTORp VECTORv

#define VECTORpp(NAME, dimension, p1, p2)               \
    Data::Vector NAME(p1, p2);

#endif //#if USE_EXPERIMENTAL_3

namespace Data
{
	class Vector
#if USE_GUI
		: public Data::Colorable
#endif //#if USE_GUI
	{
	public:

#if USE_EXPERIMENTAL_3
        Vector();
#else
		Vector(UInt dimension = 0);
		Vector(const RealVector &coords);
		Vector(const Vector &v);
		Vector(const Vector *v);
		Vector(const Point &p);
		Vector(const Point *p);
		Vector(const Point &p1, const Point &p2);
		Vector(const Point *p1, const Point *p2);
#endif //#if USE_EXPERIMENTAL_3
		virtual ~Vector();

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
		virtual void sum(const Point &p);
		virtual void sum(const Point *p);

		virtual void multiply(Real d);

		virtual void invert();

		virtual Real dot(const Vector &v) const;
		virtual Real dot(const Vector *v) const;

		virtual Real norm() const;
		virtual Real squaredNorm() const;

		virtual Real angle(const Vector &v) const;
		virtual Real angle(const Vector *v) const;

		virtual Real cosAngle(const Vector &v) const;
		virtual Real cosAngle(const Vector *v) const;

		virtual void normalize();

		//matrix should be (dimension + 1) x (dimension + 1)
		virtual void transform(Real **matrix);

		virtual bool match(const Vector &v) const;
		virtual bool match(const Vector *v) const;

		virtual bool equal(const Vector &v) const;
		virtual bool equal(const Vector *v) const;

#if (!USE_EXPERIMENTAL_3)
#if USE_EXACT
		virtual EVector evector() const;
#endif //#if USE_EXACT
#endif //#if USE_EXPERIMENTAL_3

#if (!USE_EXPERIMENTAL_3)
#if USE_ARRAY
		virtual Vector &operator=(const Vector &v);
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_3)

#if USE_GUI
		using Colorable::draw;
		virtual void draw() const;

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
	};

#if USE_EXACT
	class EVector
#if USE_GUI
		: public Data::Colorable
#endif //#if USE_GUI
	{
	public:

#if USE_EXPERIMENTAL_3
		EVector();
#else
		EVector(UInt dimension = 0);
		EVector(const ERealVector &coords);
		EVector(const EVector &v);
		EVector(const EVector *v);
		EVector(const EPoint &p);
		EVector(const EPoint *p);
		EVector(const EPoint &p1, const EPoint &p2);
		EVector(const EPoint *p1, const EPoint *p2);
#endif //#if USE_EXPERIMENTAL_3
		virtual ~EVector();

#if USE_EXPERIMENTAL_3
        virtual UInt dimension() const = 0;
#else
		virtual void setDimension(UInt dimension);
		virtual UInt dimension() const;
#endif //#if USE_EXPERIMENTAL_3

		///assumes coords[] has dimension this->dim
		///copies contents from coords[] to this->coords
#if USE_EXPERIMENTAL_3
		virtual void setCoords(const EReal coords[]) = 0;
		virtual void setCoords(const ERealVector &coords) = 0;
		virtual ERealVector getCoords() const = 0;
		virtual const EReal *getCoordsArray() const = 0;
#else
		virtual void setCoords(const EReal coords[]);
		virtual void setCoords(const ERealVector &coords);
		virtual ERealVector getCoords() const;
		virtual const EReal *getCoordsArray() const;
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
		virtual void setCoord(UInt i, EReal c) = 0;
		virtual EReal getCoord(UInt i) const = 0;
#else
		virtual void setCoord(UInt i, EReal c);
		virtual EReal getCoord(UInt i) const;
#endif //#if USE_EXPERIMENTAL_3

		virtual void sum(const EVector &v);
		virtual void sum(const EVector *v);

		virtual void multiply(EReal d);

		virtual void invert();

		virtual EReal dot(const EVector &v) const;
		virtual EReal dot(const EVector *v) const;

		virtual EReal norm() const;
		virtual EReal squaredNorm() const;

		virtual EReal angle(const EVector &v) const;
		virtual EReal angle(const EVector *v) const;

		virtual EReal cosAngle(const EVector &v) const;
		virtual EReal cosAngle(const EVector *v) const;

		virtual void normalize();

		//matrix should be (dimension + 1) x (dimension + 1)
		virtual void transform(EReal **matrix);

		virtual bool match(const EVector &v) const;
		virtual bool match(const EVector *v) const;

		//virtual bool equal(const EVector &v) const;
		//virtual bool equal(const EVector *v) const;

#if (!USE_EXPERIMENTAL_3)
		virtual Vector vector() const;
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
#if USE_ARRAY
		virtual EVector &operator=(const EVector &v);
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_3)

#if USE_GUI
		using Colorable::draw;
		virtual void draw() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

#if (!USE_EXPERIMENTAL_3)
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
#endif //#if (!USE_EXPERIMENTAL_3)
	};
#endif //#if USE_EXACT
}

#endif //#ifndef _DATA_VECTOR_H_
