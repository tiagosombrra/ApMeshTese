#include "Numerical/TransformationMatrix.h"

#if USE_NUMERICAL

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#include "Numerical/ColumnMatrix.h"

using namespace Data;
using namespace Numerical;

Numerical::TransformationMatrix::TransformationMatrix(UInt dimension) :
    Numerical::IMatrix::IMatrix()
{
    this->dimension = std::min(dimension, 3u);
}

Numerical::TransformationMatrix::TransformationMatrix(const TransformationMatrix &m) :
    Numerical::IMatrix::IMatrix(m)
{
    this->dimension = m.dimension;
}

Numerical::TransformationMatrix::~TransformationMatrix()
{

}

UInt Numerical::TransformationMatrix::numRows() const
{
    return this->dimension + 1;
}

UInt Numerical::TransformationMatrix::numCols() const
{
    return this->numRows();
}

Data::Point2D Numerical::TransformationMatrix::apply(const Data::Point2D &p) const
{
    Data::Point2D p2 = p;

    this->applyTo(p2);

    return p2;
}

Data::Point2D Numerical::TransformationMatrix::apply(const Data::Point2D *p) const
{
    return this->apply(*p);
}

void Numerical::TransformationMatrix::applyTo(Data::Point2D &p) const
{
    this->applyTo(&p);
}

void Numerical::TransformationMatrix::applyTo(Data::Point2D *p) const
{
    Real v[2];

    for (UInt i = 0; i < 2; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < 2; j++)
        {
            v[i] += this->getElement(i, j)*p->getCoord(j);
        }

        v[i] += this->getElement(i, this->dimension);
    }

    for (UInt i = 0; i < 2; i++)
    {
        p->setCoord(i, v[i]);
    }
}

Data::Point3D Numerical::TransformationMatrix::apply(const Data::Point3D &p) const
{
    Data::Point3D p2 = p;

    this->applyTo(p2);

    return p2;
}

Data::Point3D Numerical::TransformationMatrix::apply(const Data::Point3D *p) const
{
    return this->apply(*p);
}

void Numerical::TransformationMatrix::applyTo(Data::Point3D &p) const
{
    this->applyTo(&p);
}

void Numerical::TransformationMatrix::applyTo(Data::Point3D *p) const
{
    Real v[3];

    for (UInt i = 0; i < 3; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < 3; j++)
        {
            v[i] += this->getElement(i, j)*p->getCoord(j);
        }

        v[i] += this->getElement(i, 3);
    }

    for (UInt i = 0; i < 3; i++)
    {
        p->setCoord(i, v[i]);
    }
}

Numerical::IColumnMatrix *Numerical::TransformationMatrix::apply(const Numerical::IColumnMatrix &m) const
{
    Numerical::IColumnMatrix *m2 = static_cast<Numerical::IColumnMatrix *>(m.clone());

    this->applyTo(m2);

    return m2;
}

Numerical::IColumnMatrix *Numerical::TransformationMatrix::apply(const Numerical::IColumnMatrix *m) const
{
    return this->apply(*m);
}

void Numerical::TransformationMatrix::applyTo(Numerical::IColumnMatrix &m) const
{
    this->applyTo(&m);
}

void Numerical::TransformationMatrix::applyTo(Numerical::IColumnMatrix *m) const
{
    UInt n = std::min(m->numRows(), this->dimension + 1);

    Real v[this->dimension + 1];

    for (UInt i = 0; i < n; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < n; j++)
        {
            v[i] += this->getElement(i, j)*m->getElement(j);
        }

        if (m->numRows() == this->dimension)
        {
            v[i] += this->getElement(i, this->dimension);
        }
    }

    for (UInt i = 0; i < n; i++)
    {
        m->setElement(i, v[i]);
    }
}

Numerical::GeneralTransformationMatrix::GeneralTransformationMatrix(UInt dimension) :
    Numerical::TransformationMatrix::TransformationMatrix(dimension),
    Numerical::TMatrix<Real, 4, 4>::TMatrix()
{
    for (UInt i = 0; i < 4; i++)
    {
        for (UInt j = 0; j < 4; j++)
        {
            this->element(i, j) = 0.0;
        }

        this->element(i, i) = 1.0;
    }
}

Numerical::GeneralTransformationMatrix::GeneralTransformationMatrix(const GeneralTransformationMatrix &m) :
    Numerical::TransformationMatrix::TransformationMatrix(m),
    Numerical::TMatrix<Real, 4, 4>::TMatrix()
{
    /*for (UInt i = 0; i < 4; i++)
    {
        for (UInt j = 0; j < 4; j++)
        {
            this->element(i, j) = m.element(i, j);
        }
    }*/

    this->copyFrom(m);
}

Numerical::GeneralTransformationMatrix::~GeneralTransformationMatrix()
{

}

void Numerical::GeneralTransformationMatrix::setElement(UInt i, UInt j, Real element)
{
    this->element(i, ((this->dimension != 3) && (j == this->dimension)) ? 3 : j) = element;
}

Real Numerical::GeneralTransformationMatrix::getElement(UInt i, UInt j) const
{
    return const_cast<GeneralTransformationMatrix *>(this)->element(i, ((this->dimension != 3) && (j == this->dimension)) ? 3 : j);
}

void Numerical::GeneralTransformationMatrix::applyTo(Data::Point2D *p) const
{
    /*Real v[2];

    for (UInt i = 0; i < 2; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < 2; j++)
        {
            v[i] += this->m[i][j]*p->getCoord(j);
        }

        v[i] += this->m[i][2];
    }

    for (UInt i = 0; i < 2; i++)
    {
        p->setCoord(i, v[i]);
    }*/

    Real w = 1.0;

    this->applyTo(p, w);
}

void Numerical::GeneralTransformationMatrix::applyTo(Data::Point3D *p) const
{
    /*Real v[3];

    for (UInt i = 0; i < 3; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < 3; j++)
        {
            v[i] += this->m[i][j]*p->getCoord(j);
        }

        v[i] += this->m[i][3];
    }

    for (UInt i = 0; i < 3; i++)
    {
        p->setCoord(i, v[i]);
    }*/

    Real w = 1.0;

    this->applyTo(p, w);
}

void Numerical::GeneralTransformationMatrix::applyTo(Numerical::IColumnMatrix *m) const
{
    Real v[4];

    UInt n = std::min(m->numRows(), 4u);

    for (UInt i = 0; i < 4; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < n; j++)
        {
            v[i] += this->getElement(i, j)*m->getElement(j);
        }

        if (m->numRows() == this->dimension)
        {
            v[i] += this->getElement(i, this->dimension);
        }
    }

    for (UInt i = 0; i < n; i++)
    {
        m->setElement(i, v[i]);
    }
}

std::pair<Data::Point2D, Real> Numerical::GeneralTransformationMatrix::apply(const Data::Point2D &p, Real w) const
{
    Point2D p2 = p;
    Real w2 = w;

    this->applyTo(p2, w2);

    return std::make_pair(p2, w2);
}

std::pair<Data::Point2D, Real> Numerical::GeneralTransformationMatrix::apply(const Data::Point2D *p, Real w) const
{
    return this->apply(*p, w);
}

void Numerical::GeneralTransformationMatrix::applyTo(Data::Point2D &p, Real &w) const
{
    this->applyTo(p, w);
}

void Numerical::GeneralTransformationMatrix::applyTo(Data::Point2D *p, Real &w) const
{
    this->applyTo<2>(p, w);
}

std::pair<Data::Point3D, Real> Numerical::GeneralTransformationMatrix::apply(const Data::Point3D &p, Real w) const
{
    Point3D p2 = p;
    Real w2 = w;

    this->applyTo(p2, w2);

    return std::make_pair(p2, w2);
}

std::pair<Data::Point3D, Real> Numerical::GeneralTransformationMatrix::apply(const Data::Point3D *p, Real w) const
{
    return this->apply(*p, w);
}

void Numerical::GeneralTransformationMatrix::applyTo(Data::Point3D &p, Real &w) const
{
    this->applyTo(p, w);
}

void Numerical::GeneralTransformationMatrix::applyTo(Data::Point3D *p, Real &w) const
{
    this->applyTo<3>(p, w);
}

IMatrix *Numerical::GeneralTransformationMatrix::clone() const
{
    return new GeneralTransformationMatrix(*this);
}

/*Real Numerical::GeneralTransformationMatrix::element(UInt i, UInt j) const
{
    return this->Numerical::TMatrix<Real, 4, 4>::element(i, j);
}*/

Real &Numerical::GeneralTransformationMatrix::element(UInt i, UInt j)
{
    return this->Numerical::TMatrix<Real, 4, 4>::element(i, j);
}

Numerical::TranslationMatrix::TranslationMatrix(const TranslationMatrix &t) :
    Numerical::TransformationMatrix::TransformationMatrix(t),
    Numerical::TMatrix<Real, 3, 1>::TMatrix()
{
    //this->setTranslation<3u>(t.t);
    this->copyFrom(t);
}

Numerical::TranslationMatrix::TranslationMatrix(Real tx, Real ty) :
    Numerical::TransformationMatrix::TransformationMatrix(2),
    Numerical::TMatrix<Real, 3, 1>::TMatrix()
{
    this->setTranslation(tx, ty);
}

Numerical::TranslationMatrix::TranslationMatrix(Real tx, Real ty, Real tz) :
    Numerical::TransformationMatrix::TransformationMatrix(3),
    Numerical::TMatrix<Real, 3, 1>::TMatrix()
{
    this->setTranslation(tx, ty, tz);
}

Numerical::TranslationMatrix::~TranslationMatrix()
{

}

void Numerical::TranslationMatrix::setElement(UInt i, UInt j, Real element)
{
    if ((j == this->dimension) && (i < this->dimension))
    {
        this->element(i, j) = element;
    }
}

void Numerical::TranslationMatrix::setTranslation(Real tx, Real ty)
{
    this->setTx(tx);
    this->setTy(ty);
    this->setTz(0.0);
}

void Numerical::TranslationMatrix::setTranslation(Real tx, Real ty, Real tz)
{
    this->setTx(tx);
    this->setTy(ty);
    this->setTz(tz);
}

void Numerical::TranslationMatrix::getTranslation(Real &tx, Real &ty) const
{
    tx = this->getTx();
    ty = this->getTy();
}

void Numerical::TranslationMatrix::getTranslation(Real &tx, Real &ty, Real &tz) const
{
    tx = this->getTx();
    ty = this->getTy();
    tz = this->getTz();
}

void Numerical::TranslationMatrix::setTranslation(UInt i, Real t)
{
    this->setElement(i, 3, t);
}

Real Numerical::TranslationMatrix::getTranslation(UInt i) const
{
    return this->getElement(i, 3);
}

void Numerical::TranslationMatrix::setTx(Real tx)
{
    this->setTranslation(0u, tx);
}

Real Numerical::TranslationMatrix::getTx() const
{
    return this->getTranslation(0u);
}

void Numerical::TranslationMatrix::setTy(Real ty)
{
    this->setTranslation(1u, ty);
}

Real Numerical::TranslationMatrix::getTy() const
{
    return this->getTranslation(1u);
}

void Numerical::TranslationMatrix::setTz(Real tz)
{
    this->setTranslation(2u, tz);
}

Real Numerical::TranslationMatrix::getTz() const
{
    return this->getTranslation(2u);
}

void Numerical::TranslationMatrix::applyTo(Data::Point2D *p) const
{
    Data::Vector2D v(this->getTx(), this->getTy());

    p->sum(v);
}

void Numerical::TranslationMatrix::applyTo(Data::Point3D *p) const
{
    Data::Vector3D v(this->getTx(), this->getTy(), this->getTz());

    p->sum(v);
}

void Numerical::TranslationMatrix::applyTo(Numerical::IColumnMatrix *m) const
{
    UInt n = std::min(m->numRows(), this->dimension);

    for (UInt i = 0; i < n; i++)
    {
        m->setElement(i, m->getElement(i) + this->getTranslation(i));
    }
}

IMatrix *Numerical::TranslationMatrix::clone() const
{
    return new TranslationMatrix(*this);
}

Real &Numerical::TranslationMatrix::element(UInt i, UInt j)
{
    static Real zero = 0.0;
    static Real one = 1.0;

    return (i == j) ? one :
        ((j == this->dimension) ? this->Numerical::TMatrix<Real, 3, 1>::element(i, 0) : zero);
}

Numerical::ScaleMatrix::ScaleMatrix(const ScaleMatrix &s) :
    Numerical::TransformationMatrix::TransformationMatrix(s),
    Numerical::TMatrix<Real, 3, 1>::TMatrix()
{
    this->copyFrom(s);
}

Numerical::ScaleMatrix::ScaleMatrix(Real sx, Real sy) :
    Numerical::TransformationMatrix::TransformationMatrix(2),
    Numerical::TMatrix<Real, 3, 1>::TMatrix()
{
    this->setScale(sx, sy);
}

Numerical::ScaleMatrix::ScaleMatrix(Real sx, Real sy, Real sz) :
    Numerical::TransformationMatrix::TransformationMatrix(3),
    Numerical::TMatrix<Real, 3, 1>::TMatrix()
{
    this->setScale(sx, sy, sz);
}

Numerical::ScaleMatrix::~ScaleMatrix()
{

}

void Numerical::ScaleMatrix::setElement(UInt i, UInt j, Real element)
{
    if ((i == j) && (i < this->dimension))
    {
        this->element(i, j) = element;
    }
}

void Numerical::ScaleMatrix::setScale(Real sx, Real sy)
{
    this->setSx(sx);
    this->setSy(sy);
    this->setSz(1.0);
}

void Numerical::ScaleMatrix::setScale(Real sx, Real sy, Real sz)
{
    this->setSx(sx);
    this->setSy(sy);
    this->setSz(sz);
}

void Numerical::ScaleMatrix::getScale(Real &sx, Real &sy) const
{
    sx = this->getSx();
    sy = this->getSy();
}

void Numerical::ScaleMatrix::getScale(Real &sx, Real &sy, Real &sz) const
{
    sx = this->getSx();
    sy = this->getSy();
    sz = this->getSz();
}

void Numerical::ScaleMatrix::setScale(UInt i, Real s)
{
    this->setElement(i, i, s);
}

Real Numerical::ScaleMatrix::getScale(UInt i) const
{
    return this->getElement(i, i);
}

void Numerical::ScaleMatrix::setSx(Real sx)
{
    this->setScale(0u, sx);
}

Real Numerical::ScaleMatrix::getSx() const
{
    return this->getScale(0u);
}

void Numerical::ScaleMatrix::setSy(Real sy)
{
    this->setScale(1u, sy);
}

Real Numerical::ScaleMatrix::getSy() const
{
    return this->getScale(1u);
}

void Numerical::ScaleMatrix::setSz(Real sz)
{
    this->setScale(2u, sz);
}

Real Numerical::ScaleMatrix::getSz() const
{
    return this->getScale(2u);
}

bool Numerical::ScaleMatrix::isSymmetric() const
{
    return true;
}

void Numerical::ScaleMatrix::applyTo(Data::Point2D *p) const
{
    for (UInt i = 0; i < 2; i++)
    {
        p->setCoord(i, p->getCoord(i)*this->getScale(i));
    }
}

void Numerical::ScaleMatrix::applyTo(Data::Point3D *p) const
{
    for (UInt i = 0; i < 3; i++)
    {
        p->setCoord(i, p->getCoord(i)*this->getScale(i));
    }
}

void Numerical::ScaleMatrix::applyTo(Numerical::IColumnMatrix *m) const
{
    UInt n = std::min(m->numRows(), this->dimension);

    for (UInt i = 0; i < n; i++)
    {
        m->setElement(i, m->getElement(i)*this->getScale(i));
    }
}

IMatrix *Numerical::ScaleMatrix::clone() const
{
    return new ScaleMatrix(*this);
}

Real &Numerical::ScaleMatrix::element(UInt i, UInt j)
{
    static Real zero = 0.0;
    static Real one = 1.0;

    return (i != j) ? zero :
        ((j != this->dimension) ? this->Numerical::TMatrix<Real, 3, 1>::element(i, 0) : one);
}

Numerical::RotationMatrix::RotationMatrix(const RotationMatrix &r) :
    Numerical::TransformationMatrix::TransformationMatrix(r),
    Numerical::TMatrix<Real, 3, 3>::TMatrix()
{
    this->copyFrom(r);
}

Numerical::RotationMatrix::RotationMatrix(Real x, Real y, Real angle) :
    Numerical::TransformationMatrix::TransformationMatrix(2),
    Numerical::TMatrix<Real, 3, 3>::TMatrix()
{
    this->setRotation(x, y, angle);
}

Numerical::RotationMatrix::RotationMatrix(Real x, Real y, Real z, Real angle) :
    Numerical::TransformationMatrix::TransformationMatrix(3),
    Numerical::TMatrix<Real, 3, 3>::TMatrix()
{
    this->setRotation(x, y, z, angle);
}

Numerical::RotationMatrix::~RotationMatrix()
{

}

void Numerical::RotationMatrix::setElement(UInt i, UInt j, Real element)
{
    if ((i < this->dimension) && (j < this->dimension))
    {
        this->element(i, j) = element;
    }
}

void Numerical::RotationMatrix::setRotation(Real x, Real y, Real angle)
{
    this->setRotation(x, y, 0.0, angle);
}

void Numerical::RotationMatrix::setRotation(Real x, Real y, Real z, Real angle)
{
    Real c = std::cos(angle);
    Real s = std::sin(angle);
    Real C = 1.0 - c;

    this->setElement(0, 0, x*x*C + c);
    this->setElement(0, 1, x*y*C - z*s);
    this->setElement(0, 2, (this->dimension == 2) ? 0.0 : x*z*C + y*s);
    this->setElement(1, 0, y*x*C + z*s);
    this->setElement(1, 1, y*y*C + c);
    this->setElement(1, 2, (this->dimension == 2) ? 0.0 : y*z*C - x*s);
    this->setElement(2, 0, (this->dimension == 2) ? 0.0 : z*x*C - y*s);
    this->setElement(2, 1, (this->dimension == 2) ? 0.0 : z*y*C + x*s);
    this->setElement(2, 2, (this->dimension == 2) ? 1.0 : z*z*C + c);
}

void Numerical::RotationMatrix::getRotation(Real &x, Real &y, Real &angle) const
{
    Real z = 0.0;

    this->getRotation(x, y, z, angle);
}

void Numerical::RotationMatrix::getRotation(Real &x, Real &y, Real &z, Real &angle) const
{
    x = this->getElement(2, 1) - this->getElement(1, 2);
    y = this->getElement(0, 2) - this->getElement(2, 0);
    z = (this->dimension == 2) ? 0.0 : this->getElement(1, 0) - this->getElement(0, 1);

    Real r = std::sqrt(x*x + y*y + z*z);

    x /= r;
    y /= r;
    z /= r;

    Real t = this->getElement(0, 0) + this->getElement(1, 1) + ((this->dimension == 2) ? 0.0 : this->getElement(2, 2));

    angle = std::atan2(r, t - 1.0);
}

void Numerical::RotationMatrix::applyTo(Data::Point2D *p) const
{
    Real v[2];

    for (UInt i = 0; i < 2; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < 2; j++)
        {
            v[i] += this->getElement(i, j)*p->getCoord(j);
        }
    }

    for (UInt i = 0; i < 2; i++)
    {
        p->setCoord(i, v[i]);
    }
}

void Numerical::RotationMatrix::applyTo(Data::Point3D *p) const
{
    Real v[3];

    for (UInt i = 0; i < 3; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < 3; j++)
        {
            v[i] += this->getElement(i, j)*p->getCoord(j);
        }
    }

    for (UInt i = 0; i < 3; i++)
    {
        p->setCoord(i, v[i]);
    }
}

void Numerical::RotationMatrix::applyTo(Numerical::IColumnMatrix *m) const
{
    UInt n = std::min(m->numRows(), this->dimension);

    Real v[n];

    for (UInt i = 0; i < n; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < n; j++)
        {
            v[i] += this->getElement(i, j)*m->getElement(j);
        }
    }

    for (UInt i = 0; i < n; i++)
    {
        m->setElement(i, v[i]);
    }
}

IMatrix *Numerical::RotationMatrix::clone() const
{
    return new RotationMatrix(*this);
}

Real &Numerical::RotationMatrix::element(UInt i, UInt j)
{
    return this->Numerical::TMatrix<Real, 3, 3>::element(i, j);
}

Numerical::ShearMatrix::ShearMatrix(const ShearMatrix &s) :
    Numerical::TransformationMatrix::TransformationMatrix(s),
    Numerical::TMatrix<Real, 3, 2>::TMatrix()
{
    this->copyFrom(s);
}

Numerical::ShearMatrix::ShearMatrix(Real sxy, Real syx) :
    Numerical::TransformationMatrix::TransformationMatrix(2),
    Numerical::TMatrix<Real, 3, 2>::TMatrix()
{
    this->setShear(sxy, syx);
}

Numerical::ShearMatrix::ShearMatrix(Real sxy, Real sxz, Real syx, Real syz, Real szx, Real szy) :
    Numerical::TransformationMatrix::TransformationMatrix(3),
    Numerical::TMatrix<Real, 3, 2>::TMatrix()
{
    this->setShear(sxy, sxz, syx, syz, szx, szy);
}

Numerical::ShearMatrix::~ShearMatrix()
{

}

void Numerical::ShearMatrix::setElement(UInt i, UInt j, Real element)
{
    if ((i != j) && (i < this->dimension))
    {
        this->element(i, j) = element;
    }
}

void Numerical::ShearMatrix::setShear(Real sxy, Real syx)
{
    this->setSxy(sxy);
    this->setSxz(0.0);
    this->setSyx(syx);
    this->setSyz(0.0);
    this->setSzx(0.0);
    this->setSzy(0.0);
}

void Numerical::ShearMatrix::setShear(Real sxy, Real sxz, Real syx, Real syz, Real szx, Real szy)
{
    this->setSxy(sxy);
    this->setSxz(sxz);
    this->setSyx(syx);
    this->setSyz(syz);
    this->setSzx(szx);
    this->setSzy(szy);
}

void Numerical::ShearMatrix::getShear(Real &sxy, Real &syx) const
{
    sxy = this->getSxy();
    syx = this->getSyx();
}

void Numerical::ShearMatrix::getShear(Real &sxy, Real &sxz, Real &syx, Real &syz, Real &szx, Real &szy) const
{
    sxy = this->getSxy();
    sxz = this->getSxz();
    syx = this->getSyx();
    syz = this->getSyz();
    szx = this->getSzx();
    szy = this->getSzy();
}

void Numerical::ShearMatrix::setShear(UInt i, UInt j, Real s)
{
    this->setElement(i, j, s);
}

Real Numerical::ShearMatrix::getShear(UInt i, UInt j) const
{
    return this->getElement(i, j);
}

void Numerical::ShearMatrix::setSxy(Real sxy)
{
    this->setShear(0u, 1u, sxy);
}

Real Numerical::ShearMatrix::getSxy() const
{
    return this->getShear(0u, 1u);
}

void Numerical::ShearMatrix::setSxz(Real sxz)
{
    this->setShear(0u, 2u, sxz);
}

Real Numerical::ShearMatrix::getSxz() const
{
    return this->getShear(0u, 2u);
}

void Numerical::ShearMatrix::setSyx(Real syx)
{
    this->setShear(1u, 0u, syx);
}

Real Numerical::ShearMatrix::getSyx() const
{
    return this->getShear(1u, 0u);
}

void Numerical::ShearMatrix::setSyz(Real syz)
{
    this->setShear(1u, 2u, syz);
}

Real Numerical::ShearMatrix::getSyz() const
{
    return this->getShear(1u, 2u);
}

void Numerical::ShearMatrix::setSzx(Real szx)
{
    this->setShear(2u, 0u, szx);
}

Real Numerical::ShearMatrix::getSzx() const
{
    return this->getShear(2u, 0u);
}

void Numerical::ShearMatrix::setSzy(Real szy)
{
    this->setShear(2u, 1u, szy);
}

Real Numerical::ShearMatrix::getSzy() const
{
    return this->getShear(2u, 1u);
}

void Numerical::ShearMatrix::applyTo(Data::Point2D *p) const
{
    Real v[2];

    for (UInt i = 0; i < 2; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < 2; j++)
        {
            v[i] += this->getElement(i, j)*p->getCoord(j);
        }
    }

    for (UInt i = 0; i < 2; i++)
    {
        p->setCoord(i, v[i]);
    }
}

void Numerical::ShearMatrix::applyTo(Data::Point3D *p) const
{
    Real v[3];

    for (UInt i = 0; i < 3; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < 3; j++)
        {
            v[i] += this->getElement(i, j)*p->getCoord(j);
        }
    }

    for (UInt i = 0; i < 3; i++)
    {
        p->setCoord(i, v[i]);
    }
}

void Numerical::ShearMatrix::applyTo(Numerical::IColumnMatrix *m) const
{
    UInt n = std::min(m->numRows(), this->dimension);

    Real v[n];

    for (UInt i = 0; i < n; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < n; j++)
        {
            v[i] += this->getElement(i, j)*m->getElement(j);
        }
    }

    for (UInt i = 0; i < n; i++)
    {
        m->setElement(i, v[i]);
    }
}

IMatrix *Numerical::ShearMatrix::clone() const
{
    return new ShearMatrix(*this);
}

Real &Numerical::ShearMatrix::element(UInt i, UInt j)
{
    static Real one = 1.0;

    return (i == j) ? one :
        ((j < i) ? this->Numerical::TMatrix<Real, 3, 2>::element(i, j) : this->Numerical::TMatrix<Real, 3, 2>::element(i, j - 1));
}

Numerical::ReflectionMatrix::ReflectionMatrix(const ReflectionMatrix &r) :
    Numerical::TransformationMatrix::TransformationMatrix(r),
    Numerical::TSymmetricMatrix<3>::TSymmetricMatrix<3>()
{
    this->copyFrom(r);
}

Numerical::ReflectionMatrix::ReflectionMatrix(Real rx, Real ry) :
    Numerical::TransformationMatrix::TransformationMatrix(2),
    Numerical::TSymmetricMatrix<3>::TSymmetricMatrix<3>()
{
    this->setReflection(rx, ry);
}

Numerical::ReflectionMatrix::ReflectionMatrix(Real rx, Real ry, Real rz) :
    Numerical::TransformationMatrix::TransformationMatrix(3),
    Numerical::TSymmetricMatrix<3>::TSymmetricMatrix()
{
    this->setReflection(rx, ry, rz);
}

Numerical::ReflectionMatrix::~ReflectionMatrix()
{

}

UInt Numerical::ReflectionMatrix::numRows() const
{
    return this->Numerical::TSymmetricMatrix<3>::numRows();
}

UInt Numerical::ReflectionMatrix::numCols() const
{
    return this->Numerical::TSymmetricMatrix<3>::numCols();
}

void Numerical::ReflectionMatrix::setReflection(Real rx, Real ry)
{
    this->setReflection(rx, ry, 0.0);
}

void Numerical::ReflectionMatrix::setReflection(Real rx, Real ry, Real rz)
{
    this->setElement(0, 0, 1.0 - 2.0*rx*rx);
    this->setElement(0, 1, 0.0 - 2.0*rx*ry);
    this->setElement(0, 2, 0.0 - 2.0*rx*rz);
    this->setElement(1, 1, 1.0 - 2.0*ry*ry);
    this->setElement(1, 2, 0.0 - 2.0*ry*rz);
    this->setElement(2, 2, 1.0 - 2.0*rz*rz);
}

void Numerical::ReflectionMatrix::getReflection(Real &rx, Real &ry) const
{
    Real rz = 0.0;

    this->getReflection(rx, ry, rz);
}

void Numerical::ReflectionMatrix::getReflection(Real &rx, Real &ry, Real &rz) const
{
    rx = std::sqrt((1.0 - this->getElement(0, 0))*0.5);
    ry = std::sqrt((1.0 - this->getElement(1, 1))*0.5);
    rz = std::sqrt((1.0 - this->getElement(2, 2))*0.5);
}

bool Numerical::ReflectionMatrix::isSymmetric() const
{
    return true;
}

void Numerical::ReflectionMatrix::applyTo(Data::Point2D *p) const
{
    Real v[2];

    for (UInt i = 0; i < 2; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < 2; j++)
        {
            v[i] += this->getElement(i, j)*p->getCoord(j);
        }
    }

    for (UInt i = 0; i < 2; i++)
    {
        p->setCoord(i, v[i]);
    }
}

void Numerical::ReflectionMatrix::applyTo(Data::Point3D *p) const
{
    Real v[3];

    for (UInt i = 0; i < 3; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < 3; j++)
        {
            v[i] += this->getElement(i, j)*p->getCoord(j);
        }
    }

    for (UInt i = 0; i < 3; i++)
    {
        p->setCoord(i, v[i]);
    }
}

void Numerical::ReflectionMatrix::applyTo(Numerical::IColumnMatrix *m) const
{
    UInt n = std::min(m->numRows(), this->dimension);

    Real v[n];

    for (UInt i = 0; i < n; i++)
    {
        v[i] = 0.0;

        for (UInt j = 0; j < n; j++)
        {
            v[i] += this->getElement(i, j)*m->getElement(j);
        }
    }

    for (UInt i = 0; i < n; i++)
    {
        m->setElement(i, v[i]);
    }
}

IMatrix *Numerical::ReflectionMatrix::clone() const
{
    return new ReflectionMatrix(*this);
}

Numerical::BasisChangeMatrix::BasisChangeMatrix(const BasisChangeMatrix &bc) :
    Numerical::TransformationMatrix::TransformationMatrix(bc),
    Numerical::TMatrix<Real, 3, 4>::TMatrix()
{
    this->copyFrom(bc);
}

Numerical::BasisChangeMatrix::BasisChangeMatrix(Real x[2], Real y[2], Real p[2]) :
    Numerical::TransformationMatrix::TransformationMatrix(2),
    Numerical::TMatrix<Real, 3, 4>::TMatrix()
{
    this->setBasisChange(x, y, p);
}

Numerical::BasisChangeMatrix::BasisChangeMatrix(Real x[3], Real y[3], Real z[3], Real p[3]) :
    Numerical::TransformationMatrix::TransformationMatrix(3),
    Numerical::TMatrix<Real, 3, 4>::TMatrix()
{
    this->setBasisChange(x, y, z, p);
}

Numerical::BasisChangeMatrix::~BasisChangeMatrix()
{

}

/*void Numerical::BasisChangeMatrix::setElement(UInt i, UInt j, Real element)
{
    if ((i == j) && (i < this->dimension))
    {
        this->element(i, j) = element;
    }
}*/

void Numerical::BasisChangeMatrix::setBasisChange(Real x[2], Real y[2], Real p[2])
{
    this->setElement(0, 0, x[0]);
    this->setElement(1, 0, x[1]);
    this->setElement(2, 0, 0.0);

    this->setElement(0, 1, y[0]);
    this->setElement(1, 1, y[1]);
    this->setElement(2, 1, 0.0);

    this->setElement(0, 2, (this->dimension == 2) ? p[0] : 0.0);
    this->setElement(1, 2, (this->dimension == 2) ? p[1] : 0.0);
    this->setElement(2, 2, (this->dimension == 2) ? 1.0 : 0.0);

    this->setElement(0, 3, (this->dimension == 3) ? p[0] : 0.0);
    this->setElement(1, 3, (this->dimension == 3) ? p[1] : 0.0);
    this->setElement(2, 3, (this->dimension == 3) ? 0.0 : 0.0);
}

void Numerical::BasisChangeMatrix::setBasisChange(Real x[3], Real y[3], Real z[3], Real p[3])
{
    this->setX(x);
    this->setY(y);
    this->setZ(z);
    this->setP(p);
}

void Numerical::BasisChangeMatrix::getBasisChange(Real x[2], Real y[2], Real p[2]) const
{
    x[0] = this->getElement(0, 0);
    x[1] = this->getElement(1, 0);

    y[0] = this->getElement(0, 1);
    y[1] = this->getElement(1, 1);

    p[0] = this->getElement(0, this->dimension);
    p[1] = this->getElement(1, this->dimension);
}

void Numerical::BasisChangeMatrix::getBasisChange(Real x[3], Real y[3], Real z[3], Real p[3]) const
{
    this->getX(x);
    this->getY(y);
    this->getZ(z);
    this->getP(p);
}

void Numerical::BasisChangeMatrix::setX(Real x[3])
{
    this->setElement(0, 0, x[0]);
    this->setElement(1, 0, x[1]);
    this->setElement(2, 0, x[2]);
}

void Numerical::BasisChangeMatrix::getX(Real x[3]) const
{
    x[0] = this->getElement(0, 0);
    x[1] = this->getElement(1, 0);
    x[2] = this->getElement(2, 0);
}

void Numerical::BasisChangeMatrix::setY(Real y[3])
{
    this->setElement(0, 1, y[0]);
    this->setElement(1, 1, y[1]);
    this->setElement(2, 1, y[2]);
}

void Numerical::BasisChangeMatrix::getY(Real y[3]) const
{
    y[0] = this->getElement(0, 1);
    y[1] = this->getElement(1, 1);
    y[2] = this->getElement(2, 1);
}

void Numerical::BasisChangeMatrix::setZ(Real z[3])
{
    this->setElement(0, 2, z[0]);
    this->setElement(1, 2, z[1]);
    this->setElement(2, 2, z[2]);
}

void Numerical::BasisChangeMatrix::getZ(Real z[3]) const
{
    z[0] = this->getElement(0, 2);
    z[1] = this->getElement(1, 2);
    z[2] = this->getElement(2, 2);
}

void Numerical::BasisChangeMatrix::setP(Real p[3])
{
    this->setElement(0, 3, p[0]);
    this->setElement(1, 3, p[1]);
    this->setElement(2, 3, p[2]);
}

void Numerical::BasisChangeMatrix::getP(Real p[3]) const
{
    p[0] = this->getElement(0, 3);
    p[1] = this->getElement(1, 3);
    p[2] = this->getElement(1, 3);
}

Data::Point2D Numerical::BasisChangeMatrix::applyBack(const Data::Point2D &p) const
{
    Data::Point2D p2 = p;

    this->applyBack(p2);

    return p2;
}

Data::Point2D Numerical::BasisChangeMatrix::applyBack(const Data::Point2D *p) const
{
    return this->applyBack(*p);
}

void Numerical::BasisChangeMatrix::applyBackTo(Data::Point2D &p) const
{
    this->applyBackTo(&p);
}

void Numerical::BasisChangeMatrix::applyBackTo(Data::Point2D *p) const
{
    Real x[2] = {this->getElement(0, 0), this->getElement(0, 1)};
    Real y[2] = {this->getElement(1, 0), this->getElement(1, 1)};

    Real pt[2]= {-this->getElement(0, 0)*this->getElement(0, this->dimension) - this->getElement(1, 0)*this->getElement(1, this->dimension),
                 -this->getElement(0, 1)*this->getElement(0, this->dimension) - this->getElement(1, 1)*this->getElement(1, this->dimension)};

    BasisChangeMatrix back(x, y, pt);

    back.applyTo(p);
}

Data::Point3D Numerical::BasisChangeMatrix::applyBack(const Data::Point3D &p) const
{
    Data::Point3D p2 = p;

    this->applyBack(p2);

    return p2;
}

Data::Point3D Numerical::BasisChangeMatrix::applyBack(const Data::Point3D *p) const
{
    return this->applyBack(*p);
}

void Numerical::BasisChangeMatrix::applyBackTo(Data::Point3D &p) const
{
    this->applyBackTo(&p);
}

void Numerical::BasisChangeMatrix::applyBackTo(Data::Point3D *p) const
{
    Real x[3] = {this->getElement(0, 0), this->getElement(0, 1), this->getElement(0, 2)};
    Real y[3] = {this->getElement(1, 0), this->getElement(1, 1), this->getElement(1, 2)};
    Real z[3] = {this->getElement(2, 0), this->getElement(2, 1), this->getElement(2, 2)};

    Real pt[3]= {-this->getElement(0, 0)*this->getElement(0, this->dimension) - this->getElement(1, 0)*this->getElement(1, this->dimension) - this->getElement(2, 0)*this->getElement(2, this->dimension),
                 -this->getElement(0, 1)*this->getElement(0, this->dimension) - this->getElement(1, 1)*this->getElement(1, this->dimension) - this->getElement(2, 1)*this->getElement(2, this->dimension),
                 -this->getElement(0, 2)*this->getElement(0, this->dimension) - this->getElement(1, 2)*this->getElement(1, this->dimension) - this->getElement(2, 2)*this->getElement(2, this->dimension)};

    BasisChangeMatrix back(x, y, z, pt);

    back.applyTo(p);
}

Numerical::IColumnMatrix *Numerical::BasisChangeMatrix::applyBack(const Numerical::IColumnMatrix &m) const
{
    Numerical::IColumnMatrix *m2 = static_cast<Numerical::IColumnMatrix *>(m.clone());

    this->applyBack(m2);

    return m2;
}

Numerical::IColumnMatrix *Numerical::BasisChangeMatrix::applyBack(const Numerical::IColumnMatrix *m) const
{
    return this->applyBack(*m);
}

void Numerical::BasisChangeMatrix::applyBackTo(Numerical::IColumnMatrix &m) const
{
    this->applyBackTo(&m);
}

void Numerical::BasisChangeMatrix::applyBackTo(Numerical::IColumnMatrix *m) const
{
    if (this->dimension == 2)
    {
        Real x[2] = {this->getElement(0, 0), this->getElement(0, 1)};
        Real y[2] = {this->getElement(1, 0), this->getElement(1, 1)};

        Real pt[2]= {-this->getElement(0, 0)*this->getElement(0, this->dimension) - this->getElement(1, 0)*this->getElement(1, this->dimension),
                     -this->getElement(0, 1)*this->getElement(0, this->dimension) - this->getElement(1, 1)*this->getElement(1, this->dimension)};

        BasisChangeMatrix back(x, y, pt);

        back.applyTo(m);
    }
    else
    {
        Real x[3] = {this->getElement(0, 0), this->getElement(0, 1), this->getElement(0, 2)};
        Real y[3] = {this->getElement(1, 0), this->getElement(1, 1), this->getElement(1, 2)};
        Real z[3] = {this->getElement(2, 0), this->getElement(2, 1), this->getElement(2, 2)};

        Real pt[3]= {-this->getElement(0, 0)*this->getElement(0, this->dimension) - this->getElement(1, 0)*this->getElement(1, this->dimension) - this->getElement(2, 0)*this->getElement(2, this->dimension),
                     -this->getElement(0, 1)*this->getElement(0, this->dimension) - this->getElement(1, 1)*this->getElement(1, this->dimension) - this->getElement(2, 1)*this->getElement(2, this->dimension),
                     -this->getElement(0, 2)*this->getElement(0, this->dimension) - this->getElement(1, 2)*this->getElement(1, this->dimension) - this->getElement(2, 2)*this->getElement(2, this->dimension)};

        BasisChangeMatrix back(x, y, z, pt);

        back.applyTo(m);
    }
}

IMatrix *Numerical::BasisChangeMatrix::clone() const
{
    return new BasisChangeMatrix(*this);
}

Real &Numerical::BasisChangeMatrix::element(UInt i, UInt j)
{
    static Real zero = 0.0;
    static Real one = 1.0;

    return (i < 3) ? this->Numerical::TMatrix<Real, 3, 4>::element(i, j) :
        ((j < 3) ? zero : one);
}

#endif //#if USE_NUMERICAL
