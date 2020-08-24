#include "Performer/MeshTransformer.h"

#include "Data/Point2D.h"
#include "Data/Vector2D.h"
#include "Data/Box.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

Performer::MeshTransformer::MeshTransformer() :
	Performer::Builder::Builder()
{
	this->mesh = NULL;
	this->boundary = NULL;
	this->transformation = UNKNOWN;
	this->mid = false;
}

Performer::MeshTransformer::~MeshTransformer()
{

}

void Performer::MeshTransformer::setBoundary(Data::Boundary *boundary)
{
	this->boundary = boundary;
}

void Performer::MeshTransformer::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

void Performer::MeshTransformer::translate(const Data::Vector3D &translation)
{
	this->translation = translation;
	this->mid = false;
	this->reference = Data::Point3D();
	this->transformation = TRANSLATION;
}

void Performer::MeshTransformer::scale(Real scale, bool mid, const Data::Point3D &reference)
{
	this->scale(Data::Vector3D(scale, scale, scale), mid, reference);
}

void Performer::MeshTransformer::scale(const Data::Vector3D &scale, bool mid, const Data::Point3D &reference)
{
	this->scaling = scale;
	this->mid = mid;
	this->reference = reference;
	this->transformation = SCALE;
}

void Performer::MeshTransformer::rotate(const Data::Vector3D &axis, Real angle, bool mid, const Data::Point3D &reference)
{
	this->rotationAxis = axis;
	this->rotationAngle = angle;
	this->mid = mid;
	this->reference = reference;
	this->transformation = ROTATION;
}

enum Performer::MeshTransformer::Transformation Performer::MeshTransformer::getTransformation() const
{
	return this->transformation;
}

void Performer::MeshTransformer::execute()
{
    if ((!this->mesh) && (!this->boundary))
    {
        return;
    }

	if (this->transformation == UNKNOWN)
	{
		return;
	}

	this->dimension = this->boundary ? this->boundary->dimension() : this->mesh->dimension();

	if (this->mid)
	{
		this->reference = this->findMid();
	}

	switch (this->transformation)
	{
	case TRANSLATION:
		this->translate();
		break;
	case SCALE:
		this->scale();
		break;
	case ROTATION:
		this->rotate();
		break;
	default:
		break;
	}
}

Data::Point3D Performer::MeshTransformer::findMid() const
{
	Data::Point3D min, max;

	for (UInt i = 0; i < this->dimension; i++)
	{
		min.setCoord(i, REAL_MAX);
		max.setCoord(i, -REAL_MAX);
	}

	if (this->boundary)
    {
        for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
             iter != end; ++iter)
        {
            Data::Point *p = (*iter)->getPoint();

            for (UInt i = 0; i < this->dimension; i++)
            {
                Real coord = p->getCoord(i);

                if (coord < min.getCoord(i))
                {
                    min.setCoord(i, coord);
                }

                if (coord > max.getCoord(i))
                {
                    max.setCoord(i, coord);
                }
            }
        }
    }
    else
    {
        for (Data::Mesh::ConstVertexIterator iter = this->mesh->vBegin(), end = this->mesh->vEnd();
             iter != end; ++iter)
        {
            Data::Point *p = (*iter)->getPoint();

            for (UInt i = 0; i < this->dimension; i++)
            {
                Real coord = p->getCoord(i);

                if (coord < min.getCoord(i))
                {
                    min.setCoord(i, coord);
                }

                if (coord > max.getCoord(i))
                {
                    max.setCoord(i, coord);
                }
            }
        }
    }

	Data::Box box(min, max);

#if USE_EXPERIMENTAL_3
	return box.mid3D();
#else
	return box.mid();
#endif //#if USE_EXPERIMENTAL_3
}

void Performer::MeshTransformer::applyTranslation(Data::Point *p) const
{
	p->sum(this->translation);
}

void Performer::MeshTransformer::applyScale(Data::Point *p) const
{
	for (UInt i = 0; i < this->dimension; i++)
	{
		p->setCoord(i, (p->getCoord(i) - this->reference.getCoord(i))*this->scaling.getCoord(i) + this->reference.getCoord(i));
	}
}

void Performer::MeshTransformer::applyRotation(Data::Point *p) const
{
	Data::Vector3D axis = this->rotationAxis;

	axis.normalize();

	Real cos = std::cos(this->rotationAngle);
	Real oneCos = 1.0 - cos;
	Real sin = std::sin(this->rotationAngle);
	Real x = axis.getCoord(0);
	Real y = axis.getCoord(1);
	Real z = axis.getCoord(2);
	Real xx = x*x;
	Real xy = x*y;
	Real yy = y*y;
	Real yz = y*z;
	Real zz = z*z;
	Real zx = z*x;

	Real mat[4][4];
	mat[0][0] = cos + xx*oneCos;
	mat[1][1] = cos + yy*oneCos;
	mat[2][2] = cos + zz*oneCos;
	mat[3][3] = 1.0;

	mat[0][1] = xy*oneCos - z*sin;
	mat[0][2] = zx*oneCos + y*sin;
	mat[0][3] = 0.0;

	mat[1][0] = xy*oneCos + z*sin;
	mat[1][2] = yz*oneCos - x*sin;
	mat[1][3] = 0.0;

	mat[2][0] = zx*oneCos - y*sin;
	mat[2][1] = yz*oneCos + x*sin;
	mat[2][3] = 0.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;

	Real *m[4];

	m[0] = mat[0];
	m[1] = mat[1];
	m[2] = mat[2];
	m[3] = mat[3];

	this->applyTransform(p, m);
}

void Performer::MeshTransformer::applyTransform(Data::Point *p, Real *matrix[4]) const
{
	p->transform(matrix);
}

void Performer::MeshTransformer::translate()
{
    if (this->boundary)
    {
        for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
             iter != end; ++iter)
        {
            this->applyTranslation((*iter)->getPoint());
        }
    }
    else
    {
        for (Data::Mesh::ConstVertexIterator iter = this->mesh->vBegin(), end = this->mesh->vEnd();
             iter != end; ++iter)
        {
            this->applyTranslation((*iter)->getPoint());
        }
    }
}

void Performer::MeshTransformer::scale()
{
	if (this->boundary)
	{
        for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
             iter != end; ++iter)
        {
            this->applyScale((*iter)->getPoint());
        }

	}
	else
    {
        for (Data::Mesh::ConstVertexIterator iter = this->mesh->vBegin(), end = this->mesh->vEnd();
             iter != end; ++iter)
        {
            this->applyScale((*iter)->getPoint());
        }
    }
}

void Performer::MeshTransformer::rotate()
{
	this->rotationAxis.normalize();

	Real cos = std::cos(this->rotationAngle);
	Real oneCos = 1.0 - cos;
	Real sin = std::sin(this->rotationAngle);
	Real x = this->rotationAxis.getCoord(0);
	Real y = this->rotationAxis.getCoord(1);
	Real z = this->rotationAxis.getCoord(2);
	Real xx = x*x;
	Real xy = x*y;
	Real yy = y*y;
	Real yz = y*z;
	Real zz = z*z;
	Real zx = z*x;

	Real mat[4][4];
	mat[0][0] = cos + xx*oneCos;
	mat[1][1] = cos + yy*oneCos;
	mat[2][2] = cos + zz*oneCos;
	mat[3][3] = 1.0;

	mat[0][1] = xy*oneCos - z*sin;
	mat[0][2] = zx*oneCos + y*sin;
	mat[0][3] = 0.0;

	mat[1][0] = xy*oneCos + z*sin;
	mat[1][2] = yz*oneCos - x*sin;
	mat[1][3] = 0.0;

	mat[2][0] = zx*oneCos - y*sin;
	mat[2][1] = yz*oneCos + x*sin;
	mat[2][3] = 0.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;

	Real *m[4];

	m[0] = mat[0];
	m[1] = mat[1];
	m[2] = mat[2];
	m[3] = mat[3];

	if (this->boundary)
    {
        for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin(), end = this->boundary->vEnd();
             iter != end; ++iter)
        {
            this->applyTransform((*iter)->getPoint(), m);
        }

    }
    else
    {
        for (Data::Mesh::ConstVertexIterator iter = this->mesh->vBegin(), end = this->mesh->vEnd();
             iter != end; ++iter)
        {
            this->applyTransform((*iter)->getPoint(), m);
        }
    }
}
