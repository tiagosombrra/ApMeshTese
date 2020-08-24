#include "Data/Polyhedron.h"

#include "Data/Polygon.h"

using namespace Data;

Data::Polyhedron::Polyhedron() :
	Data::GeometricShape()
{

}

#if (!USE_EXPERIMENTAL_4)
Data::Polyhedron::Polyhedron(UInt numPoints) :
	Data::GeometricShape(numPoints)
{

}

Data::Polyhedron::Polyhedron(UInt numPoints, UInt numPolygons) :
	Data::GeometricShape(numPoints)
{
	this->polygons.resize(numPolygons, NULL);
}
#endif //#if (!USE_EXPERIMENTAL_4)

Data::Polyhedron::~Polyhedron()
{
#if (!USE_EXPERIMENTAL_4)
	while (!this->polygons.empty())
	{
		Polygon *p = this->polygons.back();
		this->polygons.pop_back();

		if (!p)
		{
			continue;
		}

		UInt n = p->numPoints();

		for (UInt i = 0; i < n; i++)
		{
			p->setPoint(i, NULL);
		}

		delete p;
	}
#endif //#if (!USE_EXPERIMENTAL_4)
}

#if (!USE_EXPERIMENTAL_4)
void Data::Polyhedron::setPolygons(const PolygonVector &polygons)
{
	this->polygons = polygons;
}

const PolygonVector &Data::Polyhedron::getPolygons() const
{
	return this->polygons;
}

void Data::Polyhedron::setPolygon(UInt i, Polygon *p)
{
	if (i < this->polygons.size())
	{
		this->polygons[i] = p;
	}
}

Polygon *Data::Polyhedron::getPolygon(UInt i) const
{
	return (i < this->polygons.size()) ? this->polygons[i] : NULL;
}
#endif //#if (!USE_EXPERIMENTAL_4)

#if (!USE_EXPERIMENTAL_4)
void Data::Polyhedron::add(Polygon *p)
{
	this->polygons.push_back(p);
}

void Data::Polyhedron::remove(Polygon *p)
{
	PolygonVector::iterator it = std::find(this->polygons.begin(), this->polygons.end(), p);

	if (it != this->polygons.end())
	{
		this->polygons.erase(it);
	}
}
#endif //#if (!USE_EXPERIMENTAL_4)

#if (!USE_EXPERIMENTAL_4)
Real Data::Polyhedron::surface() const
{
	Real d = 0;

	for (UInt i = 0; i < this->polygons.size() - 1; i++)
	{
		if (this->polygons[i])
		{
			d += this->polygons[i]->surface();
		}
	}

	return d;
}
#endif //#if (!USE_EXPERIMENTAL_4)

Real Data::Polyhedron::orientedSize() const
{
	return this->orientedVolume();
}

#if (!USE_EXPERIMENTAL_4)
bool Data::Polyhedron::match(const GeometricShape &s) const
{
	return this->match(&s);
}

bool Data::Polyhedron::match(const GeometricShape *s) const
{
	Polyhedron *p = dynamic_cast<Polyhedron *>(const_cast<GeometricShape *>(s));

	return this->match(p);
}

bool Data::Polyhedron::match(const Polyhedron &p) const
{
	if (!GeometricShape::match(p))
	{
		return false;
	}

	if (this->polygons.size() != p.polygons.size())
	{
		return false;
	}

	for (UInt i = 0; i < this->polygons.size(); i++)
	{
		if ((this->polygons[i] != p.polygons[i]) &&
			(!this->polygons[i]->match(p.polygons[i])))
		{
			return false;
		}
	}

	return true;
}

bool Data::Polyhedron::match(const Polyhedron *p) const
{
	/*if (!p)
	{
		return false;
	}*/

	return p ? ((this == p) || (this->match(*p))) : false;
}

bool Data::Polyhedron::equal(const GeometricShape &s) const
{
	return this->equal(&s);
}

bool Data::Polyhedron::equal(const GeometricShape *s) const
{
	Polyhedron *p = dynamic_cast<Polyhedron *>(const_cast<GeometricShape *>(s));

	return this->equal(p);
}

bool Data::Polyhedron::equal(const Polyhedron &p) const
{
	if (!GeometricShape::equal(p))
	{
		return false;
	}

	if (this->polygons.size() != p.polygons.size())
	{
		return false;
	}

	for (UInt i = 0; i < this->polygons.size(); i++)
	{
		bool found = false;

		for (UInt j = 0; j < p.polygons.size(); j++)
		{
			if ((this->polygons[i] == p.polygons[j]) ||
				(this->polygons[i]->equal(p.polygons[j])))
			{
				found = true;

				break;
			}
		}

		if (!found)
		{
			return false;
		}
	}

	return true;
}

bool Data::Polyhedron::equal(const Polyhedron *p) const
{
	/*if (!p)
	{
		return false;
	}*/

	return p ? ((this == p) || (this->equal(*p))) : false;
}
#endif //#if (!USE_EXPERIMENTAL_4)

#if USE_ARRAY
#if (!USE_EXPERIMENTAL_4)
Polyhedron &Data::Polyhedron::operator=(const Polyhedron &p)
{
	if (this != &p)
	{
		GeometricShape::operator=(p);

		this->polygons = p.polygons;
	}

	return *this;
}
#endif //#if (!USE_EXPERIMENTAL_4)
#endif //#if USE_ARRAY

#if USE_GUI
#if (!USE_EXPERIMENTAL_4)
void Data::Polyhedron::setColor(Real r, Real g, Real b) const
{
	GeometricShape::setColor(r, g, b);

	for (UInt i = 0; i < this->polygons.size(); i++)
	{
		if (this->polygons[i])
		{
			this->polygons[i]->setColor(r, g, b);
		}
	}
}

void Data::Polyhedron::highlight() const
{
	GeometricShape::highlight();

	for (UInt i = 0; i < this->polygons.size(); i++)
	{
		if (this->polygons[i])
		{
			this->polygons[i]->highlight();
		}
	}
}

void Data::Polyhedron::unhighlight() const
{
	GeometricShape::unhighlight();

	for (UInt i = 0; i < this->polygons.size(); i++)
	{
		if (this->polygons[i])
		{
			this->polygons[i]->unhighlight();
		}
	}
}
#endif //#if (!USE_EXPERIMENTAL_4)

std::string Data::Polyhedron::text() const
{
	std::stringstream str;

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		str << " ";

		if (this->getPoint(i))
		{
			str << this->getPoint(i)->text();
		}
		else
		{
			str << "-1";
		}
	}

#if (!USE_EXPERIMENTAL_4)
	str << std::endl;

	if (!this->polygons.empty())
	{
		for (UInt i = 0; i < this->polygons.size(); i++)
		{
			str << this->polygons[i]->text() << std::endl;
		}
	}
#endif //#if (!USE_EXPERIMENTAL_4)

	return str.str();
}
#endif //#if USE_GUI
