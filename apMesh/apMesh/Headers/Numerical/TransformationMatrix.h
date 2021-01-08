#ifndef _NUMERICAL_TRANSFORMATION_MATRIX_H_
#define _NUMERICAL_TRANSFORMATION_MATRIX_H_

#include "Data/Definitions.h"
#include "Numerical/Definitions.h"

#if USE_NUMERICAL

#include "Data/Point.h"
#include "Numerical/IMatrix.h"
#include "Numerical/DenseMatrix.h"
#include "Numerical/SymmetricMatrix.h"

namespace Numerical
{
    class TransformationMatrix : virtual public Numerical::IMatrix
    {
    public:

        TransformationMatrix(UInt dimension);
        TransformationMatrix(const TransformationMatrix &m);
        virtual ~TransformationMatrix();

        virtual UInt numRows() const;
		virtual UInt numCols() const;

        virtual Data::Point2D apply(const Data::Point2D &p) const;
        virtual Data::Point2D apply(const Data::Point2D *p) const;
        virtual void applyTo(Data::Point2D &p) const;
        virtual void applyTo(Data::Point2D *p) const;

        virtual Data::Point3D apply(const Data::Point3D &p) const;
        virtual Data::Point3D apply(const Data::Point3D *p) const;
        virtual void applyTo(Data::Point3D &p) const;
        virtual void applyTo(Data::Point3D *p) const;

        virtual Numerical::IColumnMatrix *apply(const Numerical::IColumnMatrix &m) const;
        virtual Numerical::IColumnMatrix *apply(const Numerical::IColumnMatrix *m) const;
        virtual void applyTo(Numerical::IColumnMatrix &m) const;
        virtual void applyTo(Numerical::IColumnMatrix *m) const;

    protected:

        UInt dimension;

    };

    class GeneralTransformationMatrix :
        public Numerical::TransformationMatrix,
        public Numerical::TMatrix<Real, 4, 4>
    {
    public:

        GeneralTransformationMatrix(UInt dimension);
        GeneralTransformationMatrix(const GeneralTransformationMatrix &m);
        virtual ~GeneralTransformationMatrix();

        virtual void setElement(UInt i, UInt j, Real element);
        virtual Real getElement(UInt i, UInt j) const;

        using Numerical::TransformationMatrix::apply;
        using Numerical::TransformationMatrix::applyTo;

        virtual void applyTo(Data::Point2D *p) const;
        virtual void applyTo(Data::Point3D *p) const;
        virtual void applyTo(Numerical::IColumnMatrix *m) const;

        virtual std::pair<Data::Point2D, Real> apply(const Data::Point2D &p, Real w) const;
        virtual std::pair<Data::Point2D, Real> apply(const Data::Point2D *p, Real w) const;
        virtual void applyTo(Data::Point2D &p, Real &w) const;
        virtual void applyTo(Data::Point2D *p, Real &w) const;

        virtual std::pair<Data::Point3D, Real> apply(const Data::Point3D &p, Real w) const;
        virtual std::pair<Data::Point3D, Real>  apply(const Data::Point3D *p, Real w) const;
        virtual void applyTo(Data::Point3D &p, Real &w) const;
        virtual void applyTo(Data::Point3D *p, Real &w) const;

        virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);
        template <UInt n> void applyTo(Data::Point *p, Real &w) const;

    };

    class TranslationMatrix :
        public Numerical::TransformationMatrix,
        public Numerical::TMatrix<Real, 3, 1>
    {
    public:

        TranslationMatrix(const TranslationMatrix &t);
        TranslationMatrix(Real tx, Real ty);
        TranslationMatrix(Real tx, Real ty, Real tz);
        template<UInt n> TranslationMatrix(const Real t[n]);
        virtual ~TranslationMatrix();

        virtual void setElement(UInt i, UInt j, Real element);

        virtual void setTranslation(Real tx, Real ty);
        virtual void setTranslation(Real tx, Real ty, Real tz);
        virtual void getTranslation(Real &tx, Real &ty) const;
        virtual void getTranslation(Real &tx, Real &ty, Real &tz) const;

        template<UInt n> void setTranslation(const Real t[n]);
        template<UInt n> void getTranslation(Real t[n]) const;

        virtual void setTranslation(UInt i, Real t);
        virtual Real getTranslation(UInt i) const;

        virtual void setTx(Real tx);
        virtual Real getTx() const;

        virtual void setTy(Real ty);
        virtual Real getTy() const;

        virtual void setTz(Real tz);
        virtual Real getTz() const;

        using Numerical::TransformationMatrix::applyTo;

        virtual void applyTo(Data::Point2D *p) const;
        virtual void applyTo(Data::Point3D *p) const;
        virtual void applyTo(Numerical::IColumnMatrix *m) const;

        virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);

    };

    class ScaleMatrix :
        public Numerical::TransformationMatrix,
        public Numerical::TMatrix<Real, 3, 1>
    {
    public:

        ScaleMatrix(const ScaleMatrix &s);
        ScaleMatrix(Real sx, Real sy);
        ScaleMatrix(Real sx, Real sy, Real sz);
        template<UInt n> ScaleMatrix(const Real s[n]);
        virtual ~ScaleMatrix();

        virtual void setElement(UInt i, UInt j, Real element);

        virtual void setScale(Real sx, Real sy);
        virtual void setScale(Real sx, Real sy, Real sz);
        virtual void getScale(Real &sx, Real &sy) const;
        virtual void getScale(Real &sx, Real &sy, Real &sz) const;

        template<UInt n> void setScale(const Real s[n]);
        template<UInt n> void getScale(Real s[n]) const;

        virtual void setScale(UInt i, Real s);
        virtual Real getScale(UInt i) const;

        virtual void setSx(Real sx);
        virtual Real getSx() const;

        virtual void setSy(Real sy);
        virtual Real getSy() const;

        virtual void setSz(Real sz);
        virtual Real getSz() const;

        virtual bool isSymmetric() const;

        using Numerical::TransformationMatrix::applyTo;

        virtual void applyTo(Data::Point2D *p) const;
        virtual void applyTo(Data::Point3D *p) const;
        virtual void applyTo(Numerical::IColumnMatrix *m) const;

        virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);

    };

    class RotationMatrix :
        public Numerical::TransformationMatrix,
        public Numerical::TMatrix<Real, 3, 3>
    {
    public:

        RotationMatrix(const RotationMatrix &r);
        RotationMatrix(Real x, Real y, Real angle);
        RotationMatrix(Real x, Real y, Real z, Real angle);
        template<UInt n> RotationMatrix(const Real r[n], Real angle);
        virtual ~RotationMatrix();

        virtual void setElement(UInt i, UInt j, Real element);

        virtual void setRotation(Real x, Real y, Real angle);
        virtual void setRotation(Real x, Real y, Real z, Real angle);
        virtual void getRotation(Real &x, Real &y, Real &angle) const;
        virtual void getRotation(Real &x, Real &y, Real &z, Real &angle) const;

        template<UInt n> void setRotation(const Real r[n], Real angle);
        template<UInt n> void getRotation(Real r[n], Real &angle) const;

        using Numerical::TransformationMatrix::applyTo;

        virtual void applyTo(Data::Point2D *p) const;
        virtual void applyTo(Data::Point3D *p) const;
        virtual void applyTo(Numerical::IColumnMatrix *m) const;

        virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);

    };

    class ShearMatrix :
        public Numerical::TransformationMatrix,
        public Numerical::TMatrix<Real, 3, 2>
    {
    public:

        ShearMatrix(const ShearMatrix &s);
        ShearMatrix(Real sxy, Real syx);
        ShearMatrix(Real sxy, Real sxz, Real syx, Real syz, Real szx, Real szy);
        virtual ~ShearMatrix();

        virtual void setElement(UInt i, UInt j, Real element);

        virtual void setShear(Real sxy, Real syx);
        virtual void setShear(Real sxy, Real sxz, Real syx, Real syz, Real szx, Real szy);
        virtual void getShear(Real &sxy, Real &syx) const;
        virtual void getShear(Real &sxy, Real &sxz, Real &syx, Real &syz, Real &szx, Real &szy) const;

        virtual void setShear(UInt i, UInt j, Real s);
        virtual Real getShear(UInt i, UInt j) const;

        virtual void setSxy(Real sxy);
        virtual Real getSxy() const;

        virtual void setSxz(Real sxz);
        virtual Real getSxz() const;

        virtual void setSyx(Real syx);
        virtual Real getSyx() const;

        virtual void setSyz(Real syz);
        virtual Real getSyz() const;

        virtual void setSzx(Real szx);
        virtual Real getSzx() const;

        virtual void setSzy(Real szy);
        virtual Real getSzy() const;

        using Numerical::TransformationMatrix::applyTo;

        virtual void applyTo(Data::Point2D *p) const;
        virtual void applyTo(Data::Point3D *p) const;
        virtual void applyTo(Numerical::IColumnMatrix *m) const;

        virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);

    };

    class ReflectionMatrix :
        public Numerical::TransformationMatrix,
        public Numerical::TSymmetricMatrix<3>
    {
    public:

        ReflectionMatrix(const ReflectionMatrix &r);
        ReflectionMatrix(Real rx, Real ry);
        ReflectionMatrix(Real rx, Real ry, Real rz);
        template<UInt n> ReflectionMatrix(const Real r[n]);
        virtual ~ReflectionMatrix();

        using Numerical::TSymmetricMatrix<3>::setElement;
        using Numerical::TSymmetricMatrix<3>::getElement;

        using Numerical::TSymmetricMatrix<3>::numRows;
        virtual UInt numRows() const;
        using Numerical::TSymmetricMatrix<3>::numCols;
        virtual UInt numCols() const;

        virtual void setReflection(Real rx, Real ry);
        virtual void setReflection(Real rx, Real ry, Real rz);
        virtual void getReflection(Real &rx, Real &ry) const;
        virtual void getReflection(Real &rx, Real &ry, Real &rz) const;

        template<UInt n> void setReflection(const Real r[n]);
        template<UInt n> void getReflection(Real r[n]) const;

        virtual bool isSymmetric() const;

        using Numerical::TransformationMatrix::applyTo;

        virtual void applyTo(Data::Point2D *p) const;
        virtual void applyTo(Data::Point3D *p) const;
        virtual void applyTo(Numerical::IColumnMatrix *m) const;

        virtual IMatrix *clone() const;

    protected:

        using Numerical::TSymmetricMatrix<3>::element;

    };

    class BasisChangeMatrix :
        public Numerical::TransformationMatrix,
        public Numerical::TMatrix<Real, 3, 4>
    {
    public:

        BasisChangeMatrix(const BasisChangeMatrix &bc);
        BasisChangeMatrix(Real x[2], Real y[2], Real p[2]);
        BasisChangeMatrix(Real x[3], Real y[3], Real z[3], Real p[3]);
        virtual ~BasisChangeMatrix();

        //virtual void setElement(UInt i, UInt j, Real element);

        virtual void setBasisChange(Real x[2], Real y[2], Real p[2]);
        virtual void setBasisChange(Real x[3], Real y[3], Real z[3], Real p[3]);
        virtual void getBasisChange(Real x[2], Real y[2], Real p[2]) const;
        virtual void getBasisChange(Real x[3], Real y[3], Real z[3], Real p[3]) const;

        virtual void setX(Real x[3]);
        virtual void getX(Real x[3]) const;

        virtual void setY(Real y[3]);
        virtual void getY(Real y[3]) const;

        virtual void setZ(Real z[3]);
        virtual void getZ(Real z[3]) const;

        virtual void setP(Real p[3]);
        virtual void getP(Real p[3]) const;

        virtual Data::Point2D applyBack(const Data::Point2D &p) const;
        virtual Data::Point2D applyBack(const Data::Point2D *p) const;
        virtual void applyBackTo(Data::Point2D &p) const;
        virtual void applyBackTo(Data::Point2D *p) const;

        virtual Data::Point3D applyBack(const Data::Point3D &p) const;
        virtual Data::Point3D applyBack(const Data::Point3D *p) const;
        virtual void applyBackTo(Data::Point3D &p) const;
        virtual void applyBackTo(Data::Point3D *p) const;

        virtual Numerical::IColumnMatrix *applyBack(const Numerical::IColumnMatrix &m) const;
        virtual Numerical::IColumnMatrix *applyBack(const Numerical::IColumnMatrix *m) const;
        virtual void applyBackTo(Numerical::IColumnMatrix &m) const;
        virtual void applyBackTo(Numerical::IColumnMatrix *m) const;

        virtual IMatrix *clone() const;

    protected:

        virtual Real &element(UInt i, UInt j);

    };
}

template <Data::UInt n>
void Numerical::GeneralTransformationMatrix::applyTo(Data::Point *p, Real &w) const
{
    Real v[n+1];

    for (UInt i = 0; i < n; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < n; j++)
        {
            v[i] += this->getElement(i, j)*p->getCoord(j);
        }

        v[i] += this->getElement(i, n)*w;
    }

    v[n] = 0.0;

    for (UInt j = 0; j < n; j++)
    {
        v[n] += this->getElement(n, j)*p->getCoord(j);
    }

    v[n] += this->getElement(n, n)*w;

    for (UInt i = 0; i < n; i++)
    {
        p->setCoord(i, v[i]);
    }

    w = v[n];
}

template <Data::UInt n>
Numerical::TranslationMatrix::TranslationMatrix(const Data::Real t[n]) :
    Numerical::TransformationMatrix::TransformationMatrix(std::min(n, 3u)),
    Numerical::TMatrix<Real, 3, 1>::TMatrix()
{
    this->setTranslation<n>(t);
}

template <Data::UInt n>
void Numerical::TranslationMatrix::setTranslation(const Data::Real t[n])
{
    UInt m = std::min(n, this->dimension);

    for (UInt i = 0; i < m; i++)
    {
        this->setTranslation(i, t[i]);
    }

    for (UInt i = m; i < this->dimension; i++)
    {
        this->setTranslation(i, 0.0);
    }
}

template <Data::UInt n>
void Numerical::TranslationMatrix::getTranslation(Data::Real t[n]) const
{
    UInt m = std::min(n, this->dimension);

    for (UInt i = 0; i < m; i++)
    {
        t[i] = this->getTranslation(i);
    }
}

template <Data::UInt n>
Numerical::ScaleMatrix::ScaleMatrix(const Data::Real s[n]) :
    Numerical::TransformationMatrix::TransformationMatrix(std::min(n, 3u)),
    Numerical::TMatrix<Real, 3, 1>::TMatrix()
{
    this->setScale<n>(s);
}

template <Data::UInt n>
void Numerical::ScaleMatrix::setScale(const Data::Real s[n])
{
    UInt m = std::min(n, this->dimension);

    for (UInt i = 0; i < m; i++)
    {
        this->setScale(i, s[i]);
    }

    for (UInt i = m; i < this->dimension; i++)
    {
        this->setScale(i, 1.0);
    }
}

template <Data::UInt n>
void Numerical::ScaleMatrix::getScale(Data::Real s[n]) const
{
    UInt m = std::min(n, this->dimension);

    for (UInt i = 0; i < m; i++)
    {
        s[i] = this->getScale(i);
    }
}

template <Data::UInt n>
Numerical::RotationMatrix::RotationMatrix(const Data::Real r[n], Real angle) :
    Numerical::TransformationMatrix::TransformationMatrix(std::min(n, 3u)),
    Numerical::TMatrix<Real, 3, 3>::TMatrix()
{
    this->setRotation<n>(r, angle);
}

template <Data::UInt n>
void Numerical::RotationMatrix::setRotation(const Data::Real r[n], Real angle)
{
    UInt m = std::min(n, this->dimension);

    if (m == 2)
    {
        this->setRotation(r[0], r[1], 0.0, angle);
    }
    else
    {
        this->setRotation(r[0], r[1], r[2], angle);
    }
}

template <Data::UInt n>
void Numerical::RotationMatrix::getRotation(Data::Real r[n], Real &angle) const
{
    UInt m = std::min(n, this->dimension);

    if (m == 2)
    {
        Real z = 0.0;

        this->getRotation(r[0], r[1], z, angle);
    }
    else
    {
        this->getRotation(r[0], r[1], r[2], angle);
    }
}

template <Data::UInt n>
Numerical::ReflectionMatrix::ReflectionMatrix(const Data::Real r[n]) :
    Numerical::TransformationMatrix::TransformationMatrix(std::min(n, 3u)),
    Numerical::TSymmetricMatrix<3>::TSymmetricMatrix()
{
    this->setReflection<n>(r);
}

template <Data::UInt n>
void Numerical::ReflectionMatrix::setReflection(const Data::Real r[n])
{
    UInt m = std::min(n, this->dimension);

    if (m == 2)
    {
        this->setReflection(r[0], r[1]);
    }
    else
    {
        this->setReflection(r[0], r[1], r[2]);
    }
}

template <Data::UInt n>
void Numerical::ReflectionMatrix::getReflection(Data::Real r[n]) const
{
    UInt m = std::min(n, this->dimension);

    if (m == 2)
    {
        this->getReflection(r[0], r[1]);
    }
    else
    {
        this->getReflection(r[0], r[1], r[2]);
    }
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_TRANSFORMATION_MATRIX_H_
