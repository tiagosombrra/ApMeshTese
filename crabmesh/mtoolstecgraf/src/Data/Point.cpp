#include "Data/Point.h"

#include "Data/Identifiable.h"
#include "Data/State.h"
#if USE_EXPERIMENTAL_3
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#endif //#if USE_EXPERIMENTAL_3

using namespace Data;

#if USE_EXPERIMENTAL_3
Data::Point::Point()
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}
#else
Data::Point::Point(UInt dimension)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = 0;
#if USE_EXPERIMENTAL_2
    this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = NULL;
#endif //#if USE_EXPERIMENTAL_2
#endif //#if USE_ARRAY

	this->setDimension(dimension);

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Point::Point(const RealVector &coords)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = 0;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = NULL;
#endif //#if USE_EXPERIMENTAL_2
#endif //#if USE_ARRAY

	this->setCoords(coords);

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Point::Point(const Point &p)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = p.dim;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new Real[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p.coords[i];
	}
#else
	this->setCoords(p.getCoords());
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Point::Point(const Point *p)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = p->dim;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new Real[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p->coords[i];
	}
#else
	this->setCoords(p->getCoords());
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Point::Point(const Point &p, const Vector &v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	/*this->dim = std::max(p.dim, static_cast<UShort>(v.dimension()));
	this->coords = new Real[this->dim];

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p.getCoord(i) + v.getCoord(i);
	}*/

	UShort pdim = p.dim;
	UShort vdim = static_cast<UShort>(v.dimension());

#if USE_EXPERIMENTAL_2
    this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#endif //#if USE_EXPERIMENTAL_2

	if (pdim == vdim)
	{
		this->dim = pdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < this->dim; i++)
		{
			this->coords[i] = p.coords[i] + v.getCoord(i);
		}
	}
	else if (pdim < vdim)
	{
		this->dim = vdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < pdim; i++)
		{
			this->coords[i] = p.coords[i] + v.getCoord(i);
		}

		for (UInt i = pdim; i < vdim; i++)
		{
			this->coords[i] = v.getCoord(i);
		}
	}
	else
	{
		this->dim = pdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < vdim; i++)
		{
			this->coords[i] = p.coords[i] + v.getCoord(i);
		}

		for (UInt i = vdim; i < pdim; i++)
		{
			this->coords[i] = p.coords[i];
		}
	}

#else
	this->setCoords(p.getCoords());

	this->sum(v);
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Point::Point(const Point *p, const Vector *v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	/*this->dim = std::max(p->dim, static_cast<UShort>(v->dimension()));
	this->coords = new Real[this->dim];

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p->getCoord(i) + v->getCoord(i);
	}*/

	UShort pdim = p->dim;
	UShort vdim = static_cast<UShort>(v->dimension());

#if USE_EXPERIMENTAL_2
    this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#endif //#if USE_EXPERIMENTAL_2

	if (pdim == vdim)
	{
		this->dim = pdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < this->dim; i++)
		{
			this->coords[i] = p->coords[i] + v->getCoord(i);
		}
	}
	else if (pdim < vdim)
	{
		this->dim = vdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < pdim; i++)
		{
			this->coords[i] = p->coords[i] + v->getCoord(i);
		}

		for (UInt i = pdim; i < vdim; i++)
		{
			this->coords[i] = v->getCoord(i);
		}
	}
	else
	{
		this->dim = pdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < vdim; i++)
		{
			this->coords[i] = p->coords[i] + v->getCoord(i);
		}

		for (UInt i = vdim; i < pdim; i++)
		{
			this->coords[i] = p->coords[i];
		}
	}
#else
	this->setCoords(p->getCoords());

	this->sum(v);
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Point::Point(const Vector &v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = static_cast<UShort>(v.dimension());
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new Real[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = v.getCoord(i);
	}
#else
	this->setCoords(v.getCoords());
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::Point::Point(const Vector *v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = static_cast<UShort>(v->dimension());
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new Real[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = v->getCoord(i);
	}
#else
	this->setCoords(v->getCoords());
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}
#endif //#if USE_EXPERIMENTAL_3

Data::Point::~Point()
{
#if (!USE_EXPERIMENTAL_3)
#if USE_ARRAY
#if (!USE_EXPERIMENTAL_2)
	if (this->coords)
	{
		delete [] this->coords;
	}
#endif //#if (!USE_EXPERIMENTAL_2)
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_3)
}

#if (!USE_EXPERIMENTAL_3)
void Data::Point::setDimension(UInt dimension)
{
#if USE_EXPERIMENTAL_2
    this->dim = static_cast<UShort>(dimension);
#elif USE_ARRAY
	if (dimension != this->dim)
	{
		Real *coords = this->coords;

		this->coords = new Real[dimension];

		UInt min = std::min(dimension, static_cast<UInt>(this->dim));

		if (coords)
		{
			for (UInt i = 0; i < min; i++)
			{
				this->coords[i] = coords[i];
			}

			delete [] coords;
		}

		for (UInt i = min; i < dimension; i++)
		{
			this->coords[i] = 0.0;
		}

		this->dim = static_cast<UShort>(dimension);
	}
#else
	this->coords.resize(dimension, 0.0);
#endif //#if USE_ARRAY
}

UInt Data::Point::dimension() const
{
#if USE_ARRAY
	return this->dim;
#else
	return this->coords.size();
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
void Data::Point::setCoords(const Real coords[])
{
	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = coords[i];
	}
}

void Data::Point::setCoords(const RealVector &coords)
{
#if USE_EXPERIMENTAL_2
    this->dim = std::min(static_cast<UShort>(3), static_cast<UShort>(coords.size()));

    for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = coords[i];
	}

	for (UInt i = this->dim; i < 3; i++)
	{
		this->coords[i] = 0.0;
	}
#elif USE_ARRAY
	if (this->dim != coords.size())
	{
		if (this->coords)
		{
			delete [] this->coords;
		}

		this->dim = static_cast<UShort>(coords.size());

		this->coords = new Real[this->dim];
	}

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = coords[i];
	}
#else
	this->coords = coords;
#endif //#if USE_ARRAY
}

RealVector Data::Point::getCoords() const
{
#if USE_ARRAY
	return RealVector(this->coords, this->coords + this->dim);
#else
	return this->coords;
#endif //#if USE_ARRAY
}

const Real *Data::Point::getCoordsArray() const
{
#if USE_ARRAY
	return this->coords;
#else
	return &this->coords[0];
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
void Data::Point::setCoord(UInt i, Real c)
{
	//if (i < this->dimension())
	{
		this->coords[i] = c;
	}
}

Real Data::Point::getCoord(UInt i) const
{
	//return (i < this->dimension()) ? this->coords[i] : 0.0;
	return this->coords[i];
}
#endif //#if (!USE_EXPERIMENTAL_3)

void Data::Point::sum(const Vector &v)
{
	this->sum(&v);
}

void Data::Point::sum(const Vector *v)
{
#if USE_EXPERIMENTAL_3
    for (UInt i = 0; i < this->dimension(); i++)
	{
		this->setCoord(i, this->getCoord(i) + v->getCoord(i));
	}
#else
	UInt vdim = v->dimension();
	//UInt dim = std::max(vdim, this->dimension());

	//this->setDimension(dim);
	this->setDimension(std::max(vdim, this->dimension()));

	//for (UInt i = 0; i < dim; i++)
	for (UInt i = 0; i < vdim; i++)
	{
		this->coords[i] += v->getCoord(i);
		//this->setCoord(i, this->getCoord(i) + v->getCoord(i));
	}
#endif //#if USE_EXPERIMENTAL_3
}

Real Data::Point::distance(const Point &p) const
{
	return this->distance(&p);
}

Real Data::Point::distance(const Point *p) const
{
#if USE_EXPERIMENTAL_3
    if (this->dimension() == 2)
    {
        Vector2D v(this, p);

        return v.norm();
    }

    Vector3D v(this, p);

    return v.norm();
#else
	Vector v(this, p);

	return v.norm();
#endif //#if USE_EXPERIMENTAL_3
}

Real Data::Point::squaredDistance(const Point &p) const
{
	return this->squaredDistance(&p);
}

Real Data::Point::squaredDistance(const Point *p) const
{
#if USE_EXPERIMENTAL_3
    if (this->dimension() == 2)
    {
        Vector2D v(this, p);

        return v.squaredNorm();
    }

	Vector3D v(this, p);

	return v.squaredNorm();
#else
	Vector v(this, p);

	return v.squaredNorm();
#endif //#if USE_EXPERIMENTAL_3
}

void Data::Point::transform(Real **matrix)
{
	UInt dim = this->dimension();

	Real values[dim];

	for (UInt i = 0; i < dim; i++)
	{
		values[i] = 0.0;

		for (UInt j = 0; j < dim; j++)
		{
			//values[i] += matrix[i][j] * this->coords[j];
			values[i] += matrix[i][j] * this->getCoord(j);
		}

		values[i] += matrix[i][dim];
	}

	for (UInt i = 0; i < dim; i++)
	{
		//this->coords[i] = values[i];
		this->setCoord(i, values[i]);
	}
}

bool Data::Point::match(const Point &p) const
{
	return this->match(&p);
}

bool Data::Point::match(const Point *p) const
{
	UInt dim = std::max(this->dimension(), p->dimension());

	for (UInt i = 0; i < dim; i++)
	{
		//both coordinates should be exactly equal, do not use tolerance here
		if (this->getCoord(i) != p->getCoord(i))
		{
			return false;
		}
	}

	return true;
}

bool Data::Point::equal(const Point &p) const
{
	return this->equal(&p);
}

bool Data::Point::equal(const Point *p) const
{
	UInt dim = std::max(this->dimension(), p->dimension());

	for (UInt i = 0; i < dim; i++)
	{
		if (std::fabs(this->getCoord(i) - p->getCoord(i)) > Data::getTolerance())
		{
			return false;
		}
	}

	return true;
}

#if (!USE_EXPERIMENTAL_3)
#if USE_EXACT
EPoint Data::Point::epoint() const
{
	EPoint pt(this->dimension());

	for (UInt i = 0; i < this->dimension(); i++)
	{
		pt.setCoord(i, this->getCoord(i));
	}

#if USE_GUI
	//pt.setSize(this->getSize());

	pt.setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		pt.highlight();
	}
#endif //#if USE_GUI

	return pt;
}
#endif //#if USE_EXACT
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
#if USE_ARRAY
Point &Data::Point::operator=(const Point &p)
{
	if (this != &p)
	{
#if USE_EXPERIMENTAL_2
        this->dim = p.dim;

        for (UShort i = 0; i < this->dim; i++)
		{
			this->coords[i] = p.coords[i];
		}

		for (UShort i = this->dim; i < 3; i++)
		{
			this->coords[i] = p.coords[i];
		}
#elif USE_ARRAY
		if (this->dim != p.dim)
		{
			if (this->coords)
			{
				delete [] this->coords;
			}

			this->dim = p.dim;

			this->coords = new Real[this->dim];
		}

		for (UInt i = 0; i < this->dim; i++)
		{
			this->coords[i] = p.coords[i];
		}
#else
		this->coords = p.coords;
#endif //#if USE_ARRAY

#if USE_GUI
		this->setColor(p.r, p.g, p.b);

		//this->setSize(p.size);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_3)

#if USE_GUI
/*void Data::Point::setSize(Real size) const
{
	this->size = size;
}

Real Data::Point::getSize() const
{
	return this->size;
}*/

void Data::Point::highlight() const
{
	Colorable::highlight();

	//this->setSize(5.0);
}

void Data::Point::unhighlight() const
{
	Colorable::unhighlight();

	//this->setSize(2.0);
}

void Data::Point::draw() const
{

}

UInt Data::Point::fill(GLfloat *coord, GLfloat *color, GLfloat */*normal*/) const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = static_cast<GLfloat>(this->getR());
		color[1] = static_cast<GLfloat>(this->getG());
		color[2] = static_cast<GLfloat>(this->getB());
	}

	for (UInt i = 0; i < std::min(this->dimension(), static_cast<UInt>(3)); i++)
	{
		coord[i] = static_cast<GLfloat>(0.0);
	}

	this->explodeAndShrink(coord, *this, true);

	return 1;
}

bool Data::Point::isInVisibleSpace() const
{
	for (UInt i = 0; i < 3; i++)
	{
		if (((Data::getState().isPlaneEnabled(2*i    )) && (this->getCoord(i) > Data::getState().getPlaneCoord(2*i    ))) ||
			((Data::getState().isPlaneEnabled(2*i + 1)) && (this->getCoord(i) < Data::getState().getPlaneCoord(2*i + 1))))
		{
			return false;
		}
	}

	return true;
}

std::string Data::Point::text() const
{
	std::stringstream str;

	for (UInt i = 0; i < this->dimension(); i++)
	{
		//str.precision(20);
		//str << " " << this->coords[i];
		str << " " << this->getCoord(i);
	}

	return str.str();
}
#endif //#if USE_GUI

#if USE_EXACT

#include "Data/Identifiable.h"
#include "Data/State.h"

Data::EPoint::EPoint(UInt dimension)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = 0;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = NULL;
#endif //#if USE_EXPERIMENTAL_2
#endif //#if USE_ARRAY

	this->setDimension(dimension);

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::EPoint::EPoint(const ERealVector &coords)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = 0;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = NULL;
#endif //#if USE_EXPERIMENTAL_2
#endif //#if USE_ARRAY

	this->setCoords(coords);

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::EPoint::EPoint(const EPoint &p)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = p.dim;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2];
#else
	this->coords = new EReal[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p.coords[i];
	}
#else
	this->setCoords(p.getCoords());
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::EPoint::EPoint(const EPoint *p)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = p->dim;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new EReal[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p->coords[i];
	}
#else
	this->setCoords(p->getCoords());
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::EPoint::EPoint(const EPoint &p, const EVector &v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	/*this->dim = std::max(p.dim, static_cast<UShort>(v.dimension()));
	this->coords = new EReal[this->dim];

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p.getCoord(i) + v.getCoord(i);
	}*/

	UShort pdim = p.dim;
	UShort vdim = static_cast<UShort>(v.dimension());

#if USE_EXPERIMENTAL_2
    this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#endif //#if USE_EXPERIMENTAL_2

	if (pdim == vdim)
	{
		this->dim = pdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < this->dim; i++)
		{
			this->coords[i] = p.coords[i] + v.getCoord(i);
		}
	}
	else if (pdim < vdim)
	{
		this->dim = vdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < pdim; i++)
		{
			this->coords[i] = p.coords[i] + v.getCoord(i);
		}

		for (UInt i = pdim; i < vdim; i++)
		{
			this->coords[i] = v.getCoord(i);
		}
	}
	else
	{
		this->dim = pdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < vdim; i++)
		{
			this->coords[i] = p.coords[i] + v.getCoord(i);
		}

		for (UInt i = vdim; i < pdim; i++)
		{
			this->coords[i] = p.coords[i];
		}
	}
#else
	this->setCoords(p.getCoords());

	this->sum(v);
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::EPoint::EPoint(const EPoint *p, const EVector *v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	/*this->dim = std::max(p->dim, static_cast<UShort>(v->dimension()));
	this->coords = new EReal[this->dim];

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p->getCoord(i) + v->getCoord(i);
	}*/

	UShort pdim = p->dim;
	UShort vdim = static_cast<UShort>(v->dimension());

#if USE_EXPERIMENTAL_2
    this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#endif //#if USE_EXPERIMENTAL_2

	if (pdim == vdim)
	{
		this->dim = pdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < this->dim; i++)
		{
			this->coords[i] = p->coords[i] + v->getCoord(i);
		}
	}
	else if (pdim < vdim)
	{
		this->dim = vdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < pdim; i++)
		{
			this->coords[i] = p->coords[i] + v->getCoord(i);
		}

		for (UInt i = pdim; i < vdim; i++)
		{
			this->coords[i] = v->getCoord(i);
		}
	}
	else
	{
		this->dim = pdim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < vdim; i++)
		{
			this->coords[i] = p->coords[i] + v->getCoord(i);
		}

		for (UInt i = vdim; i < pdim; i++)
		{
			this->coords[i] = p->coords[i];
		}
	}
#else
	this->setCoords(p->getCoords());

	this->sum(v);
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::EPoint::EPoint(const EVector &v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = static_cast<UShort>(v.dimension());
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new EReal[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = v.getCoord(i);
	}
#else
	this->setCoords(v.getCoords());
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::EPoint::EPoint(const EVector *v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = static_cast<UShort>(v->dimension());
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new EReal[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = v->getCoord(i);
	}
#else
	this->setCoords(v->getCoords());
#endif //#if USE_ARRAY

#if USE_GUI
	Real r, g, b;
    Data::getState().getTypeColor(Data::State::ColoredType::MESH_VERTEX, r, g, b);
	this->setColor(r, g, b);
#endif //#if USE_GUI
}

Data::EPoint::~EPoint()
{
#if USE_ARRAY
#if (!USE_EXPERIMENTAL_2)
	if (this->coords)
	{
		delete [] this->coords;
	}
#endif //#if (!USE_EXPERIMENTAL_2)
#endif //#if USE_ARRAY
}

void Data::EPoint::setDimension(UInt dimension)
{
#if USE_EXPERIMENTAL_2
    this->dim = static_cast<UShort>(dimension);
#elif USE_ARRAY
	if (dimension != this->dim)
	{
		EReal *coords = this->coords;

		this->coords = new EReal[dimension];

		UInt min = std::min(dimension, static_cast<UInt>(this->dim));

		if (coords)
		{
			for (UInt i = 0; i < min; i++)
			{
				this->coords[i] = coords[i];
			}

			delete [] coords;
		}

		for (UInt i = min; i < dimension; i++)
		{
			this->coords[i] = 0.0;
		}

		this->dim = static_cast<UShort>(dimension);
	}
#else
	this->coords.resize(dimension, 0.0);
#endif //#if USE_ARRAY
}

UInt Data::EPoint::dimension() const
{
#if USE_ARRAY
	return this->dim;
#else
	return this->coords.size();
#endif //#if USE_ARRAY
}

void Data::EPoint::setCoords(const EReal coords[])
{
	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = coords[i];
	}
}

void Data::EPoint::setCoords(const ERealVector &coords)
{
#if USE_EXPERIMENTAL_2
    this->dim = std::min(static_cast<UShort>(3), static_cast<UShort>(coords.size()));

    for (UShort i = 0; i < this->dim; i++)
    {
        this->coords[i] = coords[i];
    }

    for (UShort i = this->dim; i < 3; i++)
    {
        this->coords[i] = 0.0;
    }
#elif USE_ARRAY
	if (this->dim != coords.size())
	{
		if (this->coords)
		{
			delete [] this->coords;
		}

		this->dim = static_cast<UShort>(coords.size());

		this->coords = new EReal[this->dim];
	}

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = coords[i];
	}
#else
	this->coords = coords;
#endif //#if USE_ARRAY
}

ERealVector Data::EPoint::getCoords() const
{
#if USE_ARRAY
	return ERealVector(this->coords, this->coords + this->dim);
#else
	return this->coords;
#endif //#if USE_ARRAY
}

const EReal *Data::EPoint::getCoordsArray() const
{
#if USE_ARRAY
	return this->coords;
#else
	return &this->coords[0];
#endif //#if USE_ARRAY
}

void Data::EPoint::setCoord(UInt i, EReal c)
{
	//if (i < this->dimension())
	{
		this->coords[i] = c;
	}
}

EReal Data::EPoint::getCoord(UInt i) const
{
	//return (i < this->dimension()) ? this->coords[i] : EReal(0.0);
	return this->coords[i];
}

void Data::EPoint::sum(const EVector &v)
{
	this->sum(&v);
}

void Data::EPoint::sum(const EVector *v)
{
	UInt vdim = v->dimension();
	//UInt dim = std::max(v->dimension(), this->dimension());

	//this->setDimension(dim);
	this->setDimension(std::max(vdim, this->dimension()));

	//for (UInt i = 0; i < dim; i++)
	for (UInt i = 0; i < vdim; i++)
	{
		this->coords[i] += v->getCoord(i);
		//this->setCoord(i, this->getCoord(i) + v->getCoord(i));
	}
}

EReal Data::EPoint::distance(const EPoint &p) const
{
	return this->distance(&p);
}

EReal Data::EPoint::distance(const EPoint *p) const
{
#if USE_EXPERIMENTAL_3
    if (this->dimension() == 2)
    {
        EVector2D v(this, p);

        return v.norm();
    }

    EVector3D v(this, p);

    return v.norm();
#else
	EVector v(this, p);

	return v.norm();
#endif //#if USE_EXPERIMENTAL_3
}

EReal Data::EPoint::squaredDistance(const EPoint &p) const
{
	return this->squaredDistance(&p);
}

EReal Data::EPoint::squaredDistance(const EPoint *p) const
{
#if USE_EXPERIMENTAL_3
	if (this->dimension() == 2)
    {
        EVector2D v(this, p);

        return v.squaredNorm();
    }

	EVector3D v(this, p);

	return v.squaredNorm();
#else
	EVector v(this, p);

	return v.squaredNorm();
#endif //#if USE_EXPERIMENTAL_3
}

void Data::EPoint::transform(EReal **matrix)
{
	UInt dim = this->dimension();

	EReal values[dim];

	for (UInt i = 0; i < dim; i++)
	{
		values[i] = 0.0;

		for (UInt j = 0; j < dim; j++)
		{
			//values[i] += matrix[i][j] * this->coords[j];
			values[i] += matrix[i][j] * this->getCoord(j);
		}

		values[i] += matrix[i][dim];
	}

	for (UInt i = 0; i < dim; i++)
	{
		//this->coords[i] = values[i];
		this->setCoord(i, values[i]);
	}
}

bool Data::EPoint::match(const EPoint &p) const
{
	return this->match(&p);
}

bool Data::EPoint::match(const EPoint *p) const
{
	UInt dim = std::max(this->dimension(), p->dimension());

	for (UInt i = 0; i < dim; i++)
	{
		//both coordinates should be exactly equal, do not use tolerance here
		if (this->getCoord(i) != p->getCoord(i))
		{
			return false;
		}
	}

	return true;
}

/*bool Data::EPoint::equal(const EPoint &p) const
{
	return this->equal(&p);
}

bool Data::EPoint::equal(const EPoint *p) const
{
	UInt dim = std::max(this->dimension(), p->dimension());

	for (UInt i = 0; i < dim; i++)
	{
		if (std::fabs(this->getCoord(i) - p->getCoord(i)) > Data::getTolerance())
		{
			return false;
		}
	}

	return true;
}*/

#if (!USE_EXPERIMENTAL_3)
Point Data::EPoint::point() const
{
	Point pt(this->dimension());

	for (UInt i = 0; i < this->dimension(); i++)
	{
		pt.setCoord(i, this->getCoord(i));
	}

#if USE_GUI
	//pt.setSize(this->getSize());

	pt.setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		pt.highlight();
	}
#endif //#if USE_GUI

	return pt;
}
#endif //#if (!USE_EXPERIMENTAL_3)

#if USE_ARRAY
EPoint &Data::EPoint::operator=(const EPoint &p)
{
	if (this != &p)
	{
#if USE_EXPERIMENTAL_2
        this->dim = p.dim;

        for (UShort i = 0; i < this->dim; i++)
        {
            this->coords[i] = p.coords[i];
        }

        for (UShort i = this->dim; i < 3; i++)
        {
            this->coords[i] = 0.0;
        }
#elif USE_ARRAY
		if (this->dim != p.dim)
		{
			if (this->coords)
			{
				delete [] this->coords;
			}

			this->dim = p.dim;

			this->coords = new EReal[this->dim];
		}

		for (UInt i = 0; i < this->dim; i++)
		{
			this->coords[i] = p.coords[i];
		}
#else
		this->coords = p.coords;
#endif //#if USE_ARRAY

#if USE_GUI
		this->setColor(p.r, p.g, p.b);

		//this->setSize(p.size);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_ARRAY

#if USE_GUI
/*void Data::EPoint::setSize(Real size) const
{
	this->size = size;
}

Real Data::EPoint::getSize() const
{
	return this->size;
}*/

void Data::EPoint::highlight() const
{
	Colorable::highlight();

	//this->setSize(5.0);
}

void Data::EPoint::unhighlight() const
{
	Colorable::unhighlight();

	//this->setSize(2.0);
}

void Data::EPoint::draw() const
{

}

#if (!USE_EXPERIMENTAL_3)
UInt Data::EPoint::fill(GLfloat *coord, GLfloat *color, GLfloat */*normal*/) const
{
	Point pt = this->point();

	if (!Data::getState().isInVisibleSpace(&pt))
	{
		return 0;
	}

	if (color)
	{
		color[0] = static_cast<GLfloat>(this->getR());
		color[1] = static_cast<GLfloat>(this->getG());
		color[2] = static_cast<GLfloat>(this->getB());
	}

	for (UInt i = 0; i < std::min(this->dimension(), static_cast<UInt>(3)); i++)
	{
		coord[i] = static_cast<GLfloat>(0.0);
	}

	this->explodeAndShrink(coord, pt, true);

	return 1;
}
#endif //#if (!USE_EXPERIMENTAL_3)

bool Data::EPoint::isInVisibleSpace() const
{
	for (UInt i = 0; i < 3; i++)
	{
		if (((Data::getState().isPlaneEnabled(2*i    )) && (this->getCoord(i) > Data::getState().getPlaneCoord(2*i    ))) ||
			((Data::getState().isPlaneEnabled(2*i + 1)) && (this->getCoord(i) < Data::getState().getPlaneCoord(2*i + 1))))
		{
			return false;
		}
	}

	return true;
}

std::string Data::EPoint::text() const
{
	std::stringstream str;

	for (UInt i = 0; i < this->dimension(); i++)
	{
		//str.precision(20);
		//str << " " << this->coords[i];
		str << " " << this->getCoord(i);
	}

	return str.str();
}
#endif //#if USE_GUI

#endif //#if USE_EXACT
