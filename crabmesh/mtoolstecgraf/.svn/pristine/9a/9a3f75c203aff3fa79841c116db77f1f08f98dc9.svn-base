#include "Performer/BoundaryConversor.h"

#include "Data/Boundary.h"

using namespace Performer;

Performer::BoundaryConversor::BoundaryConversor(Data::Boundary *original) :
	Performer::Builder::Builder()
{
	this->setOriginal(original);
	this->converted = NULL;
}

Performer::BoundaryConversor::~BoundaryConversor()
{

}

void Performer::BoundaryConversor::setOriginal(Data::Boundary *original)
{
	this->original = original;
}

Data::Boundary *Performer::BoundaryConversor::getConverted() const
{
	return this->converted;
}

void Performer::BoundaryConversor::execute()
{
	if ((!this->original) || (this->original->size() == 0))
	{
		return;
	}

	UInt dimension = this->original->dimension();

	if (dimension == 2)
	{
		for (Data::Boundary::ConstVertexIterator iter = this->original->vBegin(), end = this->original->vEnd();
			 iter != end; iter++)
		{
			this->addVertex((*iter)->getPoint()->getCoord(0), (*iter)->getPoint()->getCoord(1), (*iter)->getId());
		}

		for (Data::Boundary::ConstFrontElementIterator iter = this->original->eBegin(), end = this->original->eEnd();
			 iter != end; iter++)
		{
			this->addFrontElement((*iter)->getVertex(0)->getId(), (*iter)->getVertex(1)->getId(), (*iter)->getId());
		}

		this->converted = this->consolidate();
	}
	else if (dimension == 3)
	{
		for (Data::Boundary::ConstVertexIterator iter = this->original->vBegin(), end = this->original->vEnd();
			 iter != end; iter++)
		{
			this->addVertex((*iter)->getPoint()->getCoord(0), (*iter)->getPoint()->getCoord(1), (*iter)->getPoint()->getCoord(2), (*iter)->getId());
		}

		ULInt id = 1;

		for (Data::Boundary::ConstFrontElementIterator iter = this->original->eBegin(), end = this->original->eEnd();
			 iter != end; iter++)
		{
			Data::FrontElement *fel = (*iter);

			if (fel->numVertices() == 3)
			{
				this->addFrontElement(fel->getVertex(0)->getId(), fel->getVertex(1)->getId(), fel->getVertex(2)->getId(), fel->getId());
			}
			else if (fel->numVertices() == 4)
			{
				Real q1 = this->quality(fel->getVertex(0)->getPoint(), fel->getVertex(1)->getPoint(), fel->getVertex(2)->getPoint());
				Real q2 = this->quality(fel->getVertex(0)->getPoint(), fel->getVertex(2)->getPoint(), fel->getVertex(3)->getPoint());

				if (q1 <= Data::getTolerance())
				{
					q1 = 0.0;
				}

				if (q2 <= Data::getTolerance())
				{
					q2 = 0.0;
				}

				Real w1 = this->worse(q1, q2) ? q1 : q2;

				Real q3 = this->quality(fel->getVertex(1)->getPoint(), fel->getVertex(2)->getPoint(), fel->getVertex(3)->getPoint());
				Real q4 = this->quality(fel->getVertex(1)->getPoint(), fel->getVertex(3)->getPoint(), fel->getVertex(0)->getPoint());

				if (q3 <= Data::getTolerance())
				{
					q3 = 0.0;
				}

				if (q4 <= Data::getTolerance())
				{
					q4 = 0.0;
				}

				Real w2 = this->worse(q3, q4) ? q3 : q4;

				if (this->worse(w1, w2))
				{
					if (q3 > 0.0)
					{
						this->addFrontElement(fel->getVertex(1)->getId(), fel->getVertex(2)->getId(), fel->getVertex(3)->getId(), id++);
					}

					if (q4 > 0.0)
					{
						this->addFrontElement(fel->getVertex(1)->getId(), fel->getVertex(3)->getId(), fel->getVertex(0)->getId(), id++);
					}
				}
				else
				{
					if (q1 > 0.0)
					{
						this->addFrontElement(fel->getVertex(0)->getId(), fel->getVertex(1)->getId(), fel->getVertex(2)->getId(), id++);
					}

					if (q2 > 0.0)
					{
						this->addFrontElement(fel->getVertex(0)->getId(), fel->getVertex(2)->getId(), fel->getVertex(3)->getId(), id++);
					}
				}
			}
		}

		this->converted = this->consolidate();
	}
}

bool Performer::BoundaryConversor::worse(Real q1, Real q2) const
{
	return (q1 <= 0.0) ? false : ((q2 <= 0.0) ? true : (q1 <= q2));
}

Real Performer::BoundaryConversor::quality(const Data::Point *p1, const Data::Point *p2, const Data::Point *p3) const
{
	Real a = p1->distance(p2);
	Real b = p2->distance(p3);
	Real c = p3->distance(p1);

	Real s = 0.5*(a + b + c);
	Real d = (s - a)*(s - b)*(s - c);

	Real rInsc = std::sqrt(d/s);
	Real rCirc = 0.25*a*b*c/sqrt(s*d);

	return 2.0*rInsc/rCirc;
}
