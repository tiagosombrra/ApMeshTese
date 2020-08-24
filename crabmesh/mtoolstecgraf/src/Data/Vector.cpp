#include "Data/Vector.h"

#include "Data/Point.h"
#include "Data/Identifiable.h"

using namespace Data;

#if USE_EXPERIMENTAL_3
Data::Vector::Vector()
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{

}
#else
Data::Vector::Vector(UInt dimension)
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
}

Data::Vector::Vector(const RealVector &coords)
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
}

Data::Vector::Vector(const Vector &v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = v.dim;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new Real[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = v.coords[i];
	}
#else
	this->setCoords(v.getCoords());
#endif //#if USE_ARRAY
}

Data::Vector::Vector(const Vector *v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = v->dim;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new Real[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = v->coords[i];
	}
#else
	this->setCoords(v->getCoords());
#endif //#if USE_ARRAY
}

Data::Vector::Vector(const Point &p)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = static_cast<UShort>(p.dimension());
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new Real[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p.getCoord(i);
	}
#else
	this->setCoords(p.getCoords());
#endif //#if USE_ARRAY
}

Data::Vector::Vector(const Point *p)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = static_cast<UShort>(p->dimension());
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new Real[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p->getCoord(i);
	}
#else
	this->setCoords(p->getCoords());
#endif //#if USE_ARRAY
}

Data::Vector::Vector(const Point &p1, const Point &p2)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	/*this->dim = static_cast<UShort>(std::max(p1.dimension(), p2.dimension()));

	this->coords = new Real[this->dim];

	for (UInt i = 0; i < dim; i++)
	{
		this->coords[i] = p2.getCoord(i) - p1.getCoord(i);
	}*/

	UShort p1dim = static_cast<UShort>(p1.dimension());
	UShort p2dim = static_cast<UShort>(p2.dimension());

#if USE_EXPERIMENTAL_2
    this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#endif //#if USE_EXPERIMENTAL_2

	if (p1dim == p2dim)
	{
		this->dim = p1dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < dim; i++)
		{
			this->coords[i] = p2.getCoord(i) - p1.getCoord(i);
		}
	}
	else if (p1dim < p2dim)
	{
		this->dim = p2dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < p1dim; i++)
		{
			this->coords[i] = p2.getCoord(i) - p1.getCoord(i);
		}

		for (UInt i = p1dim; i < p2dim; i++)
		{
			this->coords[i] = p2.getCoord(i);
		}
	}
	else
	{
		this->dim = p1dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < p2dim; i++)
		{
			this->coords[i] = p2.getCoord(i) - p1.getCoord(i);
		}

		for (UInt i = p2dim; i < p1dim; i++)
		{
			this->coords[i] = -p1.getCoord(i);
		}
	}
#else
	this->setDimension(std::max(p1.dimension(), p2.dimension()));

	UInt dim = this->dimension();

	for (UInt i = 0; i < dim; i++)
	{
		this->setCoord(i, p2.getCoord(i) - p1.getCoord(i));
	}
#endif //#if USE_ARRAY
}

Data::Vector::Vector(const Point *p1, const Point *p2)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	/*this->dim = static_cast<UShort>(std::max(p1->dimension(), p2->dimension()));

	this->coords = new Real[this->dim];

	for (UInt i = 0; i < dim; i++)
	{
		this->coords[i] = p2->getCoord(i) - p1->getCoord(i);
	}*/

	UShort p1dim = static_cast<UShort>(p1->dimension());
	UShort p2dim = static_cast<UShort>(p2->dimension());

#if USE_EXPERIMENTAL_2
    this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#endif //#if USE_EXPERIMENTAL_2

	if (p1dim == p2dim)
	{
		this->dim = p1dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < dim; i++)
		{
			this->coords[i] = p2->getCoord(i) - p1->getCoord(i);
		}
	}
	else if (p1dim < p2dim)
	{
		this->dim = p2dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < p1dim; i++)
		{
			this->coords[i] = p2->getCoord(i) - p1->getCoord(i);
		}

		for (UInt i = p1dim; i < p2dim; i++)
		{
			this->coords[i] = p2->getCoord(i);
		}
	}
	else
	{
		this->dim = p1dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new Real[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < p2dim; i++)
		{
			this->coords[i] = p2->getCoord(i) - p1->getCoord(i);
		}

		for (UInt i = p2dim; i < p1dim; i++)
		{
			this->coords[i] = -p1->getCoord(i);
		}
	}
#else
	this->setDimension(std::max(p1->dimension(), p2->dimension()));

	UInt dim = this->dimension();

	for (UInt i = 0; i < dim; i++)
	{
		this->setCoord(i, p2->getCoord(i) - p1->getCoord(i));
	}
#endif //#if USE_ARRAY
}
#endif //#if USE_EXPERIMENTAL_3

Data::Vector::~Vector()
{
#if USE_ARRAY
#if (!USE_EXPERIMENTAL_2)
	if (this->coords)
	{
		delete [] this->coords;
	}
#endif //#if USE_EXPERIMENTAL_2
#endif //#if USE_ARRAY
}

#if (!USE_EXPERIMENTAL_3)
void Data::Vector::setDimension(UInt dimension)
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

UInt Data::Vector::dimension() const
{
#if USE_ARRAY
	return this->dim;
#else
	return this->coords.size();
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
void Data::Vector::setCoords(const Real coords[])
{
	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = coords[i];
	}
}

void Data::Vector::setCoords(const RealVector &coords)
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

RealVector Data::Vector::getCoords() const
{
#if USE_ARRAY
	return RealVector(this->coords, this->coords + this->dim);
#else
	return this->coords;
#endif //#if USE_ARRAY
}

const Real *Data::Vector::getCoordsArray() const
{
#if USE_ARRAY
	return this->coords;
#else
	return &this->coords[0];
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
void Data::Vector::setCoord(UInt i, Real c)
{
	//if (i < this->dimension())
	{
		this->coords[i] = c;
	}
}

Real Data::Vector::getCoord(UInt i) const
{
	//return (i < this->dimension()) ? this->coords[i] : 0.0;
	return this->coords[i];
}
#endif //#if (!USE_EXPERIMENTAL_3)

void Data::Vector::sum(const Vector &v)
{
	this->sum(&v);
}

void Data::Vector::sum(const Vector *v)
{
#if USE_EXPERIMENTAL_3
    for (UInt i = 0; i < this->dimension(); i++)
	{
		this->setCoord(i, this->getCoord(i) + v->getCoord(i));
	}
#else
	UInt vdim = v->dimension();
	//UInt dim = std::max(v->dimension(), this->dimension());

	//this->setDimension(dim);
	this->setDimension(std::max(vdim, this->dimension()));

	//for (UInt i = 0; i < dim; i++)
	for (UInt i = 0; i < vdim; i++)
	{
		this->coords[i] += v->coords[i];
		//this->coords[i] += v->getCoord(i);
	}
#endif //#if USE_EXPERIMENTAL_3
}

void Data::Vector::sum(const Point &p)
{
	this->sum(&p);
}

void Data::Vector::sum(const Point *p)
{
#if USE_EXPERIMENTAL_3
    for (UInt i = 0; i < this->dimension(); i++)
	{
		this->setCoord(i, this->getCoord(i) + p->getCoord(i));
	}
#else
	UInt pdim = p->dimension();
	//UInt dim = std::max(p->dimension(), this->dimension());

	//this->setDimension(dim);
	this->setDimension(std::max(pdim, this->dimension()));

	//for (UInt i = 0; i < dim; i++)
	for (UInt i = 0; i < pdim; i++)
	{
		this->coords[i] += p->coords[i];
		//this->coords[i] += p->getCoord(i);
	}
#endif //#if USE_EXPERIMENTAL_3
}

void Data::Vector::multiply(Real d)
{
	UInt dim = this->dimension();

	for (UInt i = 0; i < dim; i++)
	{
#if USE_EXPERIMENTAL_3
		this->setCoord(i, this->getCoord(i)*d);
#else
		this->coords[i] *= d;
#endif //#if USE_EXPERIMENTAL_3
	}
}

void Data::Vector::invert()
{
	this->multiply(-1.0);
}

Real Data::Vector::dot(const Vector &v) const
{
	return this->dot(&v);
}

Real Data::Vector::dot(const Vector *v) const
{
	Real dot = 0.0;

	UInt dim = std::min(this->dimension(), v->dimension());

	for (UInt i = 0; i < dim; i++)
	{
#if USE_EXPERIMENTAL_3
		dot += this->getCoord(i)*v->getCoord(i);
#else
		dot += this->coords[i]*v->coords[i];
#endif //#if USE_EXPERIMENTAL_3
	}

	return dot;
}

Real Data::Vector::norm() const
{
	return std::sqrt(this->squaredNorm());
}

Real Data::Vector::squaredNorm() const
{
	return this->dot(this);
}

Real Data::Vector::cosAngle(const Vector &v) const
{
	return this->cosAngle(&v);
}

Real Data::Vector::cosAngle(const Vector *v) const
{
	return this->dot(v)/(this->norm()*v->norm());
}

Real Data::Vector::angle(const Vector &v) const
{
	return this->angle(&v);
}

Real Data::Vector::angle(const Vector *v) const
{
	Real cos = this->cosAngle(v);

	//some errors might occur due to precision
	/*if (cos < -1.0)
	{
		cos = -1.0;
	}
	else if (cos > 1.0)
	{
		cos = 1.0;
	}

	return std::acos(cos);*/
	//return std::acos((cos < -1.0) ? -1.0 :((cos > 1.0) ? 1.0 : cos));
	return (cos <= -1.0) ? M_PI : ((cos >= 1.0) ? 0.0 : std::acos(cos));
}

void Data::Vector::normalize()
{
	Real n = this->norm();

	if (n > Data::getTolerance()) this->multiply(1.0/n);
}

void Data::Vector::transform(Real **matrix)
{
	UInt dim = this->dimension();

	Real values[dim];

	for (UInt i = 0; i < dim; i++)
	{
		values[i] = 0.0;

		for (UInt j = 0; j < dim; j++)
		{
#if USE_EXPERIMENTAL_3
			values[i] += matrix[i][j] * this->getCoord(j);
#else
			values[i] += matrix[i][j] * this->coords[j];
#endif //#if USE_EXPERIMENTAL_3
		}

		values[i] += matrix[i][dim];
	}
}

bool Data::Vector::match(const Vector &v) const
{
	return this->match(&v);
}

bool Data::Vector::match(const Vector *v) const
{
	UInt dim = std::max(this->dimension(), v->dimension());

	for (UInt i = 0; i < dim; i++)
	{
		//both coordinates should be exactly equal, do not use tolerance here
		if (this->getCoord(i) != v->getCoord(i))
		{
			return false;
		}
	}

	return true;
}

bool Data::Vector::equal(const Vector &v) const
{
	return this->equal(&v);
}

bool Data::Vector::equal(const Vector *v) const
{
	UInt dim = std::max(this->dimension(), v->dimension());

	for (UInt i = 0; i < dim; i++)
	{
		if (std::fabs(this->getCoord(i) - v->getCoord(i)) < Data::getTolerance())
		{
			return false;
		}
	}

	return true;
}

#if (!USE_EXPERIMENTAL_3)
#if USE_EXACT
EVector Data::Vector::evector() const
{
	EVector v(this->dimension());

	for (UInt i = 0; i < this->dimension(); i++)
	{
		v.setCoord(i, this->getCoord(i));
	}

#if USE_GUI
	v.setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		v.highlight();
	}
#endif //#if USE_GUI

	return v;
}
#endif //#if USE_EXACT
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
#if USE_ARRAY
Vector &Data::Vector::operator=(const Vector &v)
{
	if (this != &v)
	{
#if USE_EXPERIMENTAL_2
        this->dim = v.dim;

        for (UShort i = 0; i < this->dim; i++)
        {
            this->coords[i] = v.coords[i];
        }

        for (UShort i = this->dim; i < 3; i++)
        {
            this->coords[i] = 0.0;
        }
#else
		if (this->dim != v.dim)
		{
			if (this->coords)
			{
				delete [] this->coords;
			}

			this->dim = v.dim;

			this->coords = new Real[this->dim];
		}

		for (UInt i = 0; i < this->dim; i++)
		{
			this->coords[i] = v.coords[i];
		}
#endif //#if USE_EXPERIMENTAL_2

#if USE_GUI
		this->setColor(v.r, v.g, v.b);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_3)

#if USE_GUI
void Data::Vector::draw() const
{

}

std::string Data::Vector::text() const
{
	std::stringstream str;

	for (UInt i = 0; i < this->dimension(); i++)
	{
#if USE_EXPERIMENTAL_3
		str << " " << this->getCoord(i);
#else
		str << " " << this->coords[i];
#endif //#if USE_EXPERIMENTAL_3
	}

	str << std::endl;

	return str.str();
}
#endif //#if USE_GUI

#if USE_EXACT

#if USE_EXPERIMENTAL_3
Data::EVector::EVector()
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{

}
#else
Data::EVector::EVector(UInt dimension)
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
}

Data::EVector::EVector(const ERealVector &coords)
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
}

Data::EVector::EVector(const EVector &v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = v.dim;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new EReal[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = v.coords[i];
	}
#else
	this->setCoords(v.getCoords());
#endif //#if USE_ARRAY
}

Data::EVector::EVector(const EVector *v)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = v->dim;
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new EReal[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = v->coords[i];
	}
#else
	this->setCoords(v->getCoords());
#endif //#if USE_ARRAY
}

Data::EVector::EVector(const EPoint &p)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = static_cast<UShort>(p.dimension());
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new EReal[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p.getCoord(i);
	}
#else
	this->setCoords(p.getCoords());
#endif //#if USE_ARRAY
}

Data::EVector::EVector(const EPoint *p)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	this->dim = static_cast<UShort>(p->dimension());
#if USE_EXPERIMENTAL_2
	this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#else
	this->coords = new EReal[this->dim];
#endif //#if USE_EXPERIMENTAL_2

	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = p->getCoord(i);
	}
#else
	this->setCoords(p->getCoords());
#endif //#if USE_ARRAY
}

Data::EVector::EVector(const EPoint &p1, const EPoint &p2)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	/*this->dim = static_cast<UShort>(std::max(p1.dimension(), p2.dimension()));

	this->coords = new EReal[this->dim];

	for (UInt i = 0; i < dim; i++)
	{
		this->coords[i] = p2.getCoord(i) - p1.getCoord(i);
	}*/

	UShort p1dim = static_cast<UShort>(p1.dimension());
	UShort p2dim = static_cast<UShort>(p2.dimension());

#if USE_EXPERIMENTAL_2
    this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#endif //#if USE_EXPERIMENTAL_2

	if (p1dim == p2dim)
	{
		this->dim = p1dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < dim; i++)
		{
			this->coords[i] = p2.getCoord(i) - p1.getCoord(i);
		}
	}
	else if (p1dim < p2dim)
	{
		this->dim = p2dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < p1dim; i++)
		{
			this->coords[i] = p2.getCoord(i) - p1.getCoord(i);
		}

		for (UInt i = p1dim; i < p2dim; i++)
		{
			this->coords[i] = p2.getCoord(i);
		}
	}
	else
	{
		this->dim = p1dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < p2dim; i++)
		{
			this->coords[i] = p2.getCoord(i) - p1.getCoord(i);
		}

		for (UInt i = p2dim; i < p1dim; i++)
		{
			this->coords[i] = -p1.getCoord(i);
		}
	}
#else
	this->setDimension(std::max(p1.dimension(), p2.dimension()));

	UInt dim = this->dimension();

	for (UInt i = 0; i < dim; i++)
	{
		this->setCoord(i, p2.getCoord(i) - p1.getCoord(i));
	}
#endif //#if USE_ARRAY
}

Data::EVector::EVector(const EPoint *p1, const EPoint *p2)
#if USE_GUI
	: Colorable()
#endif //#if USE_GUI
{
#if USE_ARRAY
	/*this->dim = static_cast<UShort>(std::max(p1->dimension(), p2->dimension()));

	this->coords = new EReal[this->dim];

	for (UInt i = 0; i < dim; i++)
	{
		this->coords[i] = p2->getCoord(i) - p1->getCoord(i);
	}*/

	UShort p1dim = static_cast<UShort>(p1->dimension());
	UShort p2dim = static_cast<UShort>(p2->dimension());

#if USE_EXPERIMENTAL_2
    this->coords[0] = this->coords[1] = this->coords[2] = 0.0;
#endif //#if USE_EXPERIMENTAL_2

	if (p1dim == p2dim)
	{
		this->dim = p1dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < dim; i++)
		{
			this->coords[i] = p2->getCoord(i) - p1->getCoord(i);
		}
	}
	else if (p1dim < p2dim)
	{
		this->dim = p2dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < p1dim; i++)
		{
			this->coords[i] = p2->getCoord(i) - p1->getCoord(i);
		}

		for (UInt i = p1dim; i < p2dim; i++)
		{
			this->coords[i] = p2->getCoord(i);
		}
	}
	else
	{
		this->dim = p1dim;
#if (!USE_EXPERIMENTAL_2)
		this->coords = new EReal[this->dim];
#endif //#if (!USE_EXPERIMENTAL_2)

		for (UInt i = 0; i < p2dim; i++)
		{
			this->coords[i] = p2->getCoord(i) - p1->getCoord(i);
		}

		for (UInt i = p2dim; i < p1dim; i++)
		{
			this->coords[i] = -p1->getCoord(i);
		}
	}
#else
	this->setDimension(std::max(p1->dimension(), p2->dimension()));

	UInt dim = this->dimension();

	for (UInt i = 0; i < dim; i++)
	{
		this->setCoord(i, p2->getCoord(i) - p1->getCoord(i));
	}
#endif //#if USE_ARRAY
}
#endif //#if USE_EXPERIMENTAL_3

Data::EVector::~EVector()
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
void Data::EVector::setDimension(UInt dimension)
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

UInt Data::EVector::dimension() const
{
#if USE_ARRAY
	return this->dim;
#else
	return this->coords.size();
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
void Data::EVector::setCoords(const EReal coords[])
{
	for (UInt i = 0; i < this->dim; i++)
	{
		this->coords[i] = coords[i];
	}
}

void Data::EVector::setCoords(const ERealVector &coords)
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

ERealVector Data::EVector::getCoords() const
{
#if USE_ARRAY
	return ERealVector(this->coords, this->coords + this->dim);
#else
	return this->coords;
#endif //#if USE_ARRAY
}

const EReal *Data::EVector::getCoordsArray() const
{
#if USE_ARRAY
	return this->coords;
#else
	return &this->coords[0];
#endif //#if USE_ARRAY
}
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
void Data::EVector::setCoord(UInt i, EReal c)
{
	//if (i < this->dimension())
	{
		this->coords[i] = c;
	}
}

EReal Data::EVector::getCoord(UInt i) const
{
	//return (i < this->dimension()) ? coords[i] : EReal(0.0);
	return coords[i];
}
#endif //#if (!USE_EXPERIMENTAL_3)

void Data::EVector::sum(const EVector &v)
{
	this->sum(&v);
}

void Data::EVector::sum(const EVector *v)
{
#if USE_EXPERIMENTAL_3
    for (UInt i = 0; i < this->dimension(); i++)
	{
		this->setCoord(i, this->getCoord(i) + v->getCoord(i));
	}
#else
	//UInt dim = std::max(v->dimension(), this->dimension());
	UInt vdim = v->dimension();

	//this->setDimension(dim);
	this->setDimension(std::max(vdim, this->dimension()));

	//for (UInt i = 0; i < dim; i++)
	for (UInt i = 0; i < vdim; i++)
	{
		//this->coords[i] += v->getCoord(i);
		this->coords[i] += v->coords[i];
	}
#endif //#if USE_EXPERIMENTAL_3
}

void Data::EVector::multiply(EReal d)
{
	UInt dim = this->dimension();

	for (UInt i = 0; i < dim; i++)
	{
#if USE_EXPERIMENTAL_3
		this->setCoord(i, this->getCoord(i)*d);
#else
		this->coords[i] *= d;
#endif //#if USE_EXPERIMENTAL_3
	}
}

void Data::EVector::invert()
{
	this->multiply(-1.0);
}

EReal Data::EVector::dot(const EVector &v) const
{
	return this->dot(&v);
}

EReal Data::EVector::dot(const EVector *v) const
{
	EReal dot = 0.0;

	UInt dim = std::min(this->dimension(), v->dimension());

	for (UInt i = 0; i < dim; i++)
	{
#if USE_EXPERIMENTAL_3
		dot += this->getCoord(i)*v->getCoord(i);
#else
		dot += this->coords[i]*v->coords[i];
#endif //#if USE_EXPERIMENTAL_3
	}

	return dot;
}

EReal Data::EVector::norm() const
{
	return std::sqrt(this->squaredNorm());
}

EReal Data::EVector::squaredNorm() const
{
	return this->dot(this);
}

EReal Data::EVector::cosAngle(const EVector &v) const
{
	return this->cosAngle(&v);
}

EReal Data::EVector::cosAngle(const EVector *v) const
{
	return this->dot(v)/(this->norm()*v->norm());
}

EReal Data::EVector::angle(const EVector &v) const
{
	return this->angle(&v);
}

EReal Data::EVector::angle(const EVector *v) const
{
	EReal cos = this->cosAngle(v);

	//some errors might occur due to precision
	/*if (cos < -1.0)
	{
		cos = -1.0;
	}
	else if (cos > 1.0)
	{
		cos = 1.0;
	}

	return std::acos(cos);*/
	//return std::acos((cos < -1.0) ? -1.0 :((cos > 1.0) ? 1.0 : cos));
	return (cos <= -1.0) ? M_PI : ((cos >= 1.0) ? 0.0 : std::acos(cos));
}

void Data::EVector::normalize()
{
	EReal n = this->norm();

	if (n != 0.0) this->multiply(1.0/n);
}

void Data::EVector::transform(EReal **matrix)
{
	UInt dim = this->dimension();

	EReal values[dim];

	for (UInt i = 0; i < dim; i++)
	{
		values[i] = 0.0;

		for (UInt j = 0; j < dim; j++)
		{
#if USE_EXPERIMENTAL_3
			values[i] += matrix[i][j] * this->getCoord(j);
#else
			values[i] += matrix[i][j] * this->coords[j];
#endif //#if USE_EXPERIMENTAL_3
		}

		values[i] += matrix[i][dim];
	}
}

bool Data::EVector::match(const EVector &v) const
{
	return this->match(&v);
}

bool Data::EVector::match(const EVector *v) const
{
	UInt dim = std::max(this->dimension(), v->dimension());

	for (UInt i = 0; i < dim; i++)
	{
		//both coordinates should be exactly equal, do not use tolerance here
		if (this->getCoord(i) != v->getCoord(i))
		{
			return false;
		}
	}

	return true;
}

/*bool Data::EVector::equal(const EVector &v) const
{
	return this->equal(&v);
}

bool Data::EVector::equal(const EVector *v) const
{
	UInt dim = std::max(this->dimension(), v->dimension());

	for (UInt i = 0; i < dim; i++)
	{
		if (std::fabs(this->getCoord(i) - v->getCoord(i)) < Data::getTolerance())
		{
			return false;
		}
	}

	return true;
}*/

#if (!USE_EXPERIMENTAL_3)
Vector Data::EVector::vector() const
{
	Vector v(this->dimension());

	for (UInt i = 0; i < this->dimension(); i++)
	{
		v.setCoord(i, this->getCoord(i));
	}

#if USE_GUI
	v.setColor(this->getR(), this->getG(), this->getB());

	if (this->isHighlighted())
	{
		v.highlight();
	}
#endif //#if USE_GUI

	return v;
}
#endif //#if (!USE_EXPERIMENTAL_3)

#if (!USE_EXPERIMENTAL_3)
#if USE_ARRAY
EVector &Data::EVector::operator=(const EVector &v)
{
	if (this != &v)
	{
#if USE_EXPERIMENTAL_2
        this->dim = v.dim;

        for (UShort i = 0; i < this->dim; i++)
        {
            this->coords[i] = v.coords[i];
        }

        for (UShort i = this->dim; i < 3; i++)
        {
            this->coords[i] = 0.0;
        }
#else
		if (this->dim != v.dim)
		{
			if (this->coords)
			{
				delete [] this->coords;
			}

			this->dim = v.dim;

			this->coords = new EReal[this->dim];
		}

		for (UInt i = 0; i < this->dim; i++)
		{
			this->coords[i] = v.coords[i];
		}
#endif //#if USE_EXPERIMENTAL_2

#if USE_GUI
		this->setColor(v.r, v.g, v.b);
#endif //#if USE_GUI
	}

	return *this;
}
#endif //#if USE_ARRAY
#endif //#if (!USE_EXPERIMENTAL_3)

#if USE_GUI
void Data::EVector::draw() const
{

}

std::string Data::EVector::text() const
{
	std::stringstream str;

	for (UInt i = 0; i < this->dimension(); i++)
	{
#if USE_EXPERIMENTAL_3
		str << " " << this->getCoord(i);
#else
		str << " " << this->coords[i];
#endif //#if USE_EXPERIMENTAL_3
	}

	str << std::endl;

	return str.str();
}
#endif //#if USE_GUI

#endif //#if USE_EXACT
