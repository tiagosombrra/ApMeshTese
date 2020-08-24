#include "Performer/EdgeCollapseBoundaryAdapter.h"

#include "Data/Vector3D.h"
#include "Data/Point3D.h"
#include "Data/Triangle.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

using namespace Performer;

Performer::EdgeCollapseBoundaryAdapter::EdgeCollapseBoundaryAdapter(Data::Boundary *boundary, Data::Mesh *mesh) :
	Performer::BoundaryAdapter::BoundaryAdapter(boundary)
{
	this->dimension = 0;
	this->sizeProportion = 4.0;
	this->qualityThreshold = 0.2;
	this->onlyInPlane = false;
	this->allowHangingVertices = false;
	this->ok = true;
	this->adapted = false;
	this->mesh = mesh;
}

Performer::EdgeCollapseBoundaryAdapter::EdgeCollapseBoundaryAdapter(Data::Mesh *surface) :
	Performer::BoundaryAdapter::BoundaryAdapter(surface)
{
	this->dimension = 0;
	this->sizeProportion = 4.0;
	this->qualityThreshold = 0.2;
	this->onlyInPlane = false;
	this->allowHangingVertices = false;
	this->ok = true;
	this->adapted = false;
	this->mesh = NULL;
}

Performer::EdgeCollapseBoundaryAdapter::~EdgeCollapseBoundaryAdapter()
{

}

void Performer::EdgeCollapseBoundaryAdapter::setSizeProportion(Real sizeProportion)
{
	this->sizeProportion = sizeProportion;
}

void Performer::EdgeCollapseBoundaryAdapter::setQualityThreshold(Real qualityThreshold)
{
	this->qualityThreshold = qualityThreshold;
}

void Performer::EdgeCollapseBoundaryAdapter::setOnlyInPlane(bool onlyInPlane)
{
	this->onlyInPlane = onlyInPlane;
}

void Performer::EdgeCollapseBoundaryAdapter::setAllowHangingVertices(bool allowHangingVertices)
{
	this->allowHangingVertices = allowHangingVertices;
}

bool Performer::EdgeCollapseBoundaryAdapter::isAdapted() const
{
	return this->adapted;
}

bool Performer::EdgeCollapseBoundaryAdapter::isOk() const
{
	return this->ok;
}

void Performer::EdgeCollapseBoundaryAdapter::execute()
{
	if ((!this->getMesh()) || (this->getMesh()->size() == 0) || (this->sizeProportion <= 1.0))
	{
		return;
	}

	this->dimension = this->getMesh()->dimension();

	if (/*(this->dimension != 2) && */(this->dimension != 3))
	{
		return;
	}

	this->adapted = false;
	this->ok = true;

	if (!this->identifyAdjacencies())
	{
		this->adjacencies.clear();
		this->elements.clear();

		this->ok = false;

		return;
	}

	this->identifyQualities();

	this->executeAdaptation();
}

bool Performer::EdgeCollapseBoundaryAdapter::adapt(Data::/*Front*/Element *element,
	Data::/*Front*/ElementList &/*newElements*/, Data::VertexList &/*newVertices*/) const
{
	UInt coll = this->shouldCollapse(static_cast<Data::Triangle *>(element->getShape()));

	if (coll >= element->getShape()->numPoints())
	{
		//std::cout << "should not collapse" << std::endl;

		return false;
	}

	if (!this->canCollapse(element, coll))
	{
		//std::cout << "cannot not collapse" << std::endl;

		return false;
	}

	Data::Vertex *v1 = element->getVertex(coll);
	Data::Vertex *v2 = element->getVertex((coll+1)%element->getShape()->numPoints());

	Data::/*Front*/Element *adj = this->adjacencies[element][v1];

	if (!adj)
    {
        return false;
    }

	//std::cout << "adj is at " << adj << std::endl;
	//std::cout << "adj is " << adj->getId() << std::endl;

	//std::cout << "vertices are at " << v1 << " and " << v2 << std::endl;
	//std::cout << "vertices are " << v1->getId() << " and " << v2->getId() << std::endl;

	bool removed = this->removeFromElements(adj);

	if (this->collapse(element, adj, v1, v2))
	{
		//std::cout << "collapsed" << std::endl;

		this->deletes(element, adj, v1, v2);

		this->adapted = true;

		return true;
	}

	if (removed)
	{
		//std::cout << "did not collapse, adding adj back" << std::endl;

		this->addToElements(adj);
	}
	/*else
	{
		std::cout << "did not collapse" << std::endl;
	}*/

	return false;
}

Real Performer::EdgeCollapseBoundaryAdapter::hangingAngle(Data::Vertex *v, Data::/*Front*/ElementList &adjacency) const
{
	bool hasAdj = true;

	Real angle = 0.0;

	for (Data::/*Front*/ElementList::const_iterator iter = adjacency.begin();
		 iter != adjacency.end(); iter++)
	{
		Data::/*Front*/Element *adj = (*iter);

		UInt i = adj->numVertices();

		for (i = 0; i < adj->numVertices(); i++)
		{
			if (adj->getVertex(i) == v)
			{
				break;
			}
		}

		if (i == adj->numVertices())
		{
			return true;
		}

		UInt j = (i + 1)%adj->numVertices();
		UInt k = (i + adj->numVertices() - 1)%adj->numVertices();

		AdjacenciesIdentifier::AdjacencyMap::iterator iter2 = this->adjacencies.find(adj);
		AdjacenciesIdentifier::VertexAdjacencyMap::iterator iter3 = (*iter2).second.find(v);

		if (!(*iter3).second)
		{
			if (hasAdj)
			{
				hasAdj = false;
			}
			else
			{
				return -1.0;
			}
		}

		Data::Vector3D v1(v->getPoint(), adj->getVertex(j)->getPoint());
		Data::Vector3D v2(v->getPoint(), adj->getVertex(k)->getPoint());

		angle += v1.angle(v2);
	}

	return angle;
}

// hanging 0 -> inner vertex
// hanging 1 -> angle != 180
// hanging 2 -> angle == 180
UInt Performer::EdgeCollapseBoundaryAdapter::hanging(Data::Vertex *v, Data::/*Front*/ElementList &adjacency) const
{
	UInt dimension = v->getPoint()->dimension();

	if (dimension == 2)
	{
		return (adjacency.size() == 1) ? 1 : 0;
	}

	if (dimension == 3)
	{
		if (!this->inPlane(v, adjacency))
		{
			return 1;
		}

		Real angle = this->hangingAngle(v, adjacency);

		if (angle < 0.0)
		{
			return 1;
		}

		if (std::fabs(angle - 2.0*M_PI) <= Data::getTolerance())
		{
			return 0;
		}

		if (std::fabs(angle - M_PI) > Data::getTolerance())
		{
			return 1;
		}

		return 2;
	}

	return 0;
}

bool Performer::EdgeCollapseBoundaryAdapter::inPlane(Data::Vertex *v, Data::/*Front*/ElementList &adjacency) const
{
	UInt dimension = v->getPoint()->dimension();

	if (dimension == 2)
	{
		return true;
	}

	if (dimension == 3)
	{
		Data::Vector3D v = static_cast<Data::Polygon *>(adjacency.front()->getShape())->normal();

		//v.normalize();

		for (Data::/*Front*/ElementList::const_iterator iter = ++adjacency.begin();
			 iter != adjacency.end(); iter++)
		{
			Data::Vector3D n = static_cast<Data::Polygon *>((*iter)->getShape())->normal();

			//n.normalize();

			if (n.dot(v) <= 1.0 - Data::getTolerance())
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

bool Performer::EdgeCollapseBoundaryAdapter::collapse(Data::/*Front*/Element *fel,
	Data::/*Front*/Element *adj, Data::Vertex *v1, Data::Vertex *v2) const
{
	Data::ElementList adjs1 = this->adjacency(v1);
	Data::ElementList adjs2 = this->adjacency(v2);

	UInt h1 = (this->allowHangingVertices) ? this->hanging(v1, adjs1) : 0;
	UInt h2 = (this->allowHangingVertices) ? this->hanging(v2, adjs2) : 0;

	if ((h1 == 1) && (h2 == 1))
	{
		return false;
	}

	bool pl1 = this->inPlane(v1, adjs1);
	bool pl2 = this->inPlane(v2, adjs2);

	if ((this->onlyInPlane) && (!pl1) && (!pl2))
	{
		return false;
	}

	for (Data::/*Front*/ElementList::iterator iter = adjs2.begin();
		 iter != adjs2.end(); iter++)
	{
		if (((*iter) == fel) || ((*iter) == adj))
		{
			continue;
		}

		if (std::find(adjs1.begin(), adjs1.end(), (*iter)) != adjs1.end())
		{
			return false;
		}
	}

	//std::cout << "removing adjacency of v1 " << v1->getId() << std::endl;

	Data::BoolVector rem(adjs1.size() + adjs2.size());

	UInt i = 0;

	for (Data::/*Front*/ElementList::iterator iter = adjs1.begin();
		 iter != adjs1.end();)
	{
		if (((*iter) == fel) || ((*iter) == adj))
		{
			iter = adjs1.erase(iter);

			continue;
		}

		rem[i++] = this->removeFromElements((*iter));

		iter++;
	}

	//std::cout << "removing adjacency of v2 " << v2->getId() << std::endl;

	for (Data::/*Front*/ElementList::iterator iter = adjs2.begin();
		 iter != adjs2.end();)
	{
		if (((*iter) == fel) || ((*iter) == adj))
		{
			iter = adjs2.erase(iter);

			continue;
		}

		rem[i++] = this->removeFromElements((*iter));

		(*iter)->replace(v2, v1);

		iter++;
	}

	adjs1.splice(adjs1.end(), adjs2);

	if (((h1 == 0) && (h2 == 0)) ||
		((h1 == 2) && (h2 == 2)))
	{
		if (((pl1) && (pl2)) || ((!pl1) && (!pl2)))
		{
			//if both vertices lie on planes or both do not lie on planes,
			//  move to the center of the edge

			Data::Vector3D v(v1->getPoint(), v2->getPoint());

			v.multiply(0.5);

			v1->getPoint()->sum(v);
		}
		else if ((pl1) && (!pl2))
		{
			//if both vertices one vertex lie on a plane and the other do not,
			//  keep the one that does not lie on the plane

			v1->getPoint()->setCoords(v2->getPoint()->getCoordsArray());
		}
	}
	else if (((h1 == 0) && (h2 == 1)) || ((h1 == 0) && (h2 == 2)) || ((h1 == 2) && (h2 == 1)))
	{
		v1->getPoint()->setCoords(v2->getPoint()->getCoordsArray());
	}

	/*fel->setVertex(0, NULL);
	fel->setVertex(1, NULL);
	fel->setVertex(2, NULL);

	adj->setVertex(0, NULL);
	adj->setVertex(1, NULL);
	adj->setVertex(2, NULL);*/

	i = 0;

	for (Data::/*Front*/ElementList::iterator iter = adjs1.begin();
		 iter != adjs1.end(); iter++)
	{
		Data::/*Front*/Element *fe = (*iter);

		if (rem[i++])
		{
			this->addToElements(fe);
		}

		AdjacenciesIdentifier::AdjacencyMap::iterator iter2 = this->adjacencies.find(fe);

		AdjacenciesIdentifier::VertexAdjacencyMap::iterator iter3 = (*iter2).second.find(v1);

		if (iter3 != (*iter2).second.end())
		{
			if ((*iter3).second == fel)
			{
				(*iter3).second = this->adjacencies[fel][v2];
			}
			else if ((adj) && ((*iter3).second == adj))
			{
				(*iter3).second = this->adjacencies[adj][v2];
			}
		}
		else
		{
			iter3 = (*iter2).second.find(v2);

			if ((*iter3).second == fel)
			{
				(*iter2).second.insert(std::make_pair(v1, this->adjacencies[fel][v1]));
			}
			else if ((adj) && ((*iter3).second == adj))
			{
				(*iter2).second.insert(std::make_pair(v1, this->adjacencies[adj][v1]));
			}
			else
			{
				(*iter2).second.insert(std::make_pair(v1, (*iter3).second));
			}

			(*iter2).second.erase(iter3);
		}

		for (iter3 = (*iter2).second.begin(); iter3 != (*iter2).second.end(); iter3++)
		{
			if (((*iter3).first == v1) || ((*iter3).first == v2))
			{
				continue;
			}

			if ((*iter3).second == fel)
			{
				(*iter3).second = this->adjacencies[fel][(*iter3).first];
			}
			else if ((adj) && ((*iter3).second == adj))
			{
				(*iter3).second = this->adjacencies[adj][(*iter3).first];
			}
		}
	}

	this->adjacencies.erase(fel);

	if (adj)
	{
		this->adjacencies.erase(adj);
	}

	return true;
}

bool Performer::EdgeCollapseBoundaryAdapter::deletes(Data::/*Front*/Element *fel,
    Data::/*Front*/Element *adj, Data::Vertex */*v1*/, Data::Vertex *v2) const
{
	if (mesh)
	{
		this->mesh->remove(static_cast<Data::FrontElement *>(fel));

		if (adj)
		{
			this->mesh->remove(static_cast<Data::FrontElement *>(adj));
		}

		this->mesh->remove(v2);
	}

	//this->getBoundary()->remove(fel);
	this->remove(fel);

	if (adj)
	{
		//this->getBoundary()->remove(adj);
		this->remove(adj);
	}

	this->remove(v2);

	delete fel;

	if (adj)
	{
		delete adj;
	}

	delete v2;

	return true;
}

Real Performer::EdgeCollapseBoundaryAdapter::quality(const Data::Triangle *t) const
{
	Real a = t->getP1()->distance(t->getP2());
	Real b = t->getP2()->distance(t->getP3());
	Real c = t->getP3()->distance(t->getP1());

	Real s = 0.5*(a + b + c);
	Real d = (s - a)*(s - b)*(s - c);

	Real rInsc = std::sqrt(d/s);
	Real rCirc = 0.25*a*b*c/sqrt(s*d);

	return 2.0*rInsc/rCirc;
}

UInt Performer::EdgeCollapseBoundaryAdapter::shouldCollapse(const Data::Triangle *t) const
{
	UInt numPts = t->numPoints();

	Real sizes[numPts];
	UInt min = 0;

	for (UInt i = 0; i < numPts; i++)
	{
		sizes[i] = t->getPoint(i)->distance(t->getPoint((i+1)%numPts));

		if (sizes[i] < sizes[min])
		{
			min = i;
		}
	}

	Real threshold = sizes[min]*this->sizeProportion;

	for (UInt i = 0; i < numPts; i++)
	{
		if (i == min)
		{
			continue;
		}

		if (sizes[i] <= threshold)
		{
			return numPts;
		}
	}

	return min;
}

bool Performer::EdgeCollapseBoundaryAdapter::canCollapse(const Data::/*Front*/Element *fel, UInt i) const
{
	if (this->allowHangingVertices)
	{
		Data::Vertex *v1 = fel->getVertex(i);
		Data::Vertex *v2 = fel->getVertex((i+1)%fel->getShape()->numPoints());

		Data::ElementList adjs1 = this->adjacency(v1);
		Data::ElementList adjs2 = this->adjacency(v2);

		UInt h1 = this->hanging(v1, adjs1);
		UInt h2 = this->hanging(v2, adjs2);

		if ((h1 == 0) || (h2 == 0))
		{
			return true;
		}

		if ((h1 == 1) && (h2 == 1))
		{
			return false;
		}

		if (((h1 == 1) && (h2 == 2)) ||
			((h1 == 2) && (h2 == 1)))
		{
			return true;
		}

		AdjacenciesIdentifier::AdjacencyMap::iterator iter2 = this->adjacencies.find(const_cast<Data::/*Front*/Element *>(fel));
		AdjacenciesIdentifier::VertexAdjacencyMap::iterator iter3 = (*iter2).second.find(v1);

		if ((*iter3).second != NULL)
		{
			return false;
		}

		return true;
	}

	return true;
}

bool Performer::EdgeCollapseBoundaryAdapter::addToElements(Data::/*Front*/Element *fel) const
{
	//std::cout << "trying to add fel at " << fel << std::endl;
	//std::cout << "trying to add fel " << fel->getId() << std::endl;

	Real q = this->quality(static_cast<Data::Triangle *>(fel->getShape()));

	if (q > this->qualityThreshold)
	{
		//std::cout << "not added, outside range" << std::endl;

		return false;
	}

	//std::cout << "added" << std::endl;

	this->elements.insert(std::make_pair(q, fel));

	return true;
}

bool Performer::EdgeCollapseBoundaryAdapter::removeFromElements(const Data::/*Front*/Element *fel) const
{
	//std::cout << "trying to remove fel at " << fel << std::endl;
	//std::cout << "trying to remove fel " << fel->getId() << std::endl;

	if ((this->allowHangingVertices) && (!fel))
	{
		return false;
	}

	Real q = this->quality(static_cast<Data::Triangle *>(fel->getShape()));

	if (q > this->qualityThreshold)
	{
		//std::cout << "not removed, outside range" << std::endl;

		return false;
	}

	std::pair<RealEMultiMap::iterator, RealEMultiMap::iterator> range = this->elements.equal_range(q);

	for (RealEMultiMap::iterator iter = range.first;
		 iter != range.second; iter++)
	{
		if ((*iter).second == fel)
		{
			this->elements.erase(iter);

			//std::cout << "removed" << std::endl;

			return true;
		}
	}

	//std::cout << "not removed, not found" << std::endl;

	return false;
}

AdjacenciesIdentifier *Performer::EdgeCollapseBoundaryAdapter::makeAdjacenciesIdentifier() const
{
	AdjacenciesIdentifier *identifier = new AdjacenciesIdentifier();

	if (this->isBoundary())
    {
        identifier->setBoundary(this->getBoundary());

        identifier->setAllowHangingVertices(this->allowHangingVertices);
    }
    else
    {
        identifier->setSurface(this->getSurface());

        identifier->setAllowHangingVertices(true);
    }

	return identifier;
}

bool Performer::EdgeCollapseBoundaryAdapter::identifyAdjacencies()
{
	AdjacenciesIdentifier *identifier = this->makeAdjacenciesIdentifier();

	identifier->execute();

	identifier->getAdjacenciesSwapping(this->adjacencies);

	delete identifier;

	return !this->adjacencies.empty();
}

void Performer::EdgeCollapseBoundaryAdapter::identifyQualities()
{
	if (this->isBoundary())
    {
        for (Data::Boundary::ConstFrontElementIterator iter = this->getBoundary()->eBegin(), end = this->getBoundary()->eEnd();
             iter != end; iter++)
        {
            this->addToElements((*iter));
        }
    }
    else
    {
        for (Data::Mesh::ConstElementIterator iter = this->getSurface()->eBegin(), end = this->getSurface()->eEnd();
             iter != end; iter++)
        {
            this->addToElements((*iter));
        }
    }
}

void Performer::EdgeCollapseBoundaryAdapter::executeAdaptation()
{
	Data::/*Front*/ElementList newElements;
	Data::VertexList newVertices;

	while (!this->elements.empty())
	{
		RealEMultiMap::iterator iter = this->elements.begin();

		Data::/*Front*/Element *fel = (*iter).second;

		//std::cout << "trying to collapse fel at " << fel << std::endl;
		//std::cout << "trying to collapse fel " << fel->getId() << std::endl;

		this->elements.erase(iter);

		this->adapt(fel, newElements, newVertices);
	}
}
