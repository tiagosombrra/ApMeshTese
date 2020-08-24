#include "Performer/EdgeSwapBoundaryAdapter.h"

#include "Data/Point3D.h"
#include "Data/Vector3D.h"
#include "Data/Edge2D.h"
#include "Data/Edge3D.h"
#include "Data/Triangle3D.h"
#include "Data/FrontElement.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

using namespace Performer;

Performer::EdgeSwapBoundaryAdapter::EdgeSwapBoundaryAdapter(Data::Boundary *boundary) :
	Performer::BoundaryAdapter::BoundaryAdapter(boundary)
{
	this->dimension = 0;
	this->sizeProportion = 4.0;
	this->qualityThreshold = 0.2;
	this->ok = true;
	this->adapted = false;
	this->normalTestEnabled = true;
	this->changeSurfaceBoundaryEnabled = false;
	this->maxEdgesOnBoundary = 0;
}

Performer::EdgeSwapBoundaryAdapter::EdgeSwapBoundaryAdapter(Data::Mesh *surface) :
	Performer::BoundaryAdapter::BoundaryAdapter(surface)
{
	this->dimension = 0;
	this->sizeProportion = 4.0;
	this->qualityThreshold = 0.2;
	this->ok = true;
	this->adapted = false;
	this->normalTestEnabled = true;
	this->changeSurfaceBoundaryEnabled = false;
	this->maxEdgesOnBoundary = 0;
}

Performer::EdgeSwapBoundaryAdapter::~EdgeSwapBoundaryAdapter()
{

}

void Performer::EdgeSwapBoundaryAdapter::setSizeProportion(Real sizeProportion)
{
	this->sizeProportion = sizeProportion;
}

void Performer::EdgeSwapBoundaryAdapter::setQualityThreshold(Real qualityThreshold)
{
	this->qualityThreshold = qualityThreshold;
}

void Performer::EdgeSwapBoundaryAdapter::setNormalTestEnabled(bool normalTestEnabled)
{
    this->normalTestEnabled = normalTestEnabled;
}

void Performer::EdgeSwapBoundaryAdapter::setChangeSurfaceBoundaryEnabled(bool changeSurfaceBoundaryEnabled)
{
    this->changeSurfaceBoundaryEnabled = changeSurfaceBoundaryEnabled;
}

void Performer::EdgeSwapBoundaryAdapter::setMaxEdgesOnBoundary(UInt maxEdgesOnBoundary)
{
    this->maxEdgesOnBoundary = maxEdgesOnBoundary;
}

bool Performer::EdgeSwapBoundaryAdapter::isOk() const
{
	return this->ok;
}

bool Performer::EdgeSwapBoundaryAdapter::isAdapted() const
{
	return this->adapted;
}

void Performer::EdgeSwapBoundaryAdapter::execute()
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

	if (!this->changeSurfaceBoundaryEnabled)
	{
        this->maxEdgesOnBoundary = 0;
	}

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

bool Performer::EdgeSwapBoundaryAdapter::adapt(Data::/*Front*/Element *element,
	Data::/*Front*/ElementList &/*newElements*/, Data::VertexList &/*newVertices*/) const
{
    /*if (element->getId() == 723419)
    {
        std::cout << "debug" << std::endl;
    }*/

	UInt priority[3];

	if (!this->shouldSwap(static_cast<Data::Triangle *>(element->getShape()), priority))
	{
		return false;
	}

	for (UInt i = 0; i < 3; i++)
	{
		UInt max = 0;

		for (UInt j = 1; j < 3; j++)
		{
			if (priority[j] > priority[max])
			{
				max = j;
			}
		}

		if (priority[max] == 0)
		{
			break;
		}

		priority[max] = 0;

		if (!this->canSwap(element, max))
		{
			continue;
		}

		Data::Vertex *v1 = element->getVertex((max+2)%3);
		Data::Vertex *v2 = element->getVertex(max);
		Data::Vertex *v4 = element->getVertex((max+1)%3);
		Data::Vertex *v3 = NULL;

		Data::/*Front*/Element *adj = this->adjacencies[element][v2];

		for (UInt j = 0; j < 3; j++)
		{
			Data::Vertex *v = adj->getVertex(j);

			if ((v != v2) && (v != v4))
			{
				v3 = v;

				break;
			}
		}

		this->removeFromElements(adj);

		if (this->swap(element, adj, v1, v2, v3, v4))
		{
			this->adapted = true;
		}

		break;
	}

	return true;
}

bool Performer::EdgeSwapBoundaryAdapter::swap(Data::/*Front*/Element *fel, Data::/*Front*/Element *adj,
	Data::Vertex *v1, Data::Vertex *v2, Data::Vertex *v3, Data::Vertex *v4) const
{
	/*fel->setColor(1.0, 0.0, 0.0);
	adj->setColor(0.0, 1.0, 0.0);

	return false;*/

	fel->setVertex(0, v1);
	fel->setVertex(1, v2);
	fel->setVertex(2, v3);

	adj->setVertex(0, v3);
	adj->setVertex(1, v4);
	adj->setVertex(2, v1);

	Data::/*Front*/Element *fadj = this->adjacencies[fel][v4];
	Data::/*Front*/Element *aadj = this->adjacencies[adj][v2];

	this->adjacencies[fel].erase(v4);
	this->adjacencies[adj].erase(v2);

	this->adjacencies[fel][v2] = aadj;
	this->adjacencies[fel][v3] = adj;

	this->adjacencies[aadj][v3] = fel;

	this->adjacencies[adj][v4] = fadj;
	this->adjacencies[adj][v1] = fel;

	this->adjacencies[fadj][v1] = adj;

	return true;
}

Real Performer::EdgeSwapBoundaryAdapter::quality(const Data::Triangle *t) const
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

bool Performer::EdgeSwapBoundaryAdapter::shouldSwap(const Data::Triangle *t, UInt priority[]) const
{
	UInt numPts = t->numPoints();

	Real sizes[numPts];
	Real minsize = REAL_MAX;

	for (UInt i = 0; i < numPts; i++)
	{
		sizes[i] = t->getPoint(i)->distance(t->getPoint((i+1)%numPts));

		if (sizes[i] < minsize)
		{
			minsize = sizes[i];
		}
	}

	bool swap[numPts], anySwap = false;

	Real threshold = minsize*this->sizeProportion;

	for (UInt i = 0; i < numPts; i++)
	{
		swap[i] = (sizes[i] > threshold);

		anySwap = (anySwap || swap[i]);

		priority[i] = 0;
	}

	if (!anySwap)
	{
		return false;
	}

	for (UInt i = 0; i < numPts; i++)
	{
		if (!swap[i])
		{
			continue;
		}

		for (UInt j = 0; j < numPts; j++)
		{
			if (sizes[i] >= sizes[j])
			{
				priority[i]++;
			}
		}
	}

	return true;
}

bool Performer::EdgeSwapBoundaryAdapter::canSwap(const Data::/*Front*/Element *fel, UInt i) const
{
    /*if (fel->getId() == 723419)
    {
        std::cout << "debug" << std::endl;
    }*/

	Data::Vertex *v = fel->getVertex(i);
	Data::Vertex *n = fel->getVertex((i+1)%fel->numVertices());

	/*if (((v->getId() == 360768) || (n->getId() == 361003)) ||
        ((v->getId() == 361003) || (n->getId() == 360768)))
    {
        std::cout << "debug" << std::endl;
    }*/

	Data::/*Front*/Element *adj = this->adjacencies[const_cast<Data::/*Front*/Element *>(fel)][v];

	if (!adj)
    {
        return false;
    }

    if ((this->dimension == 3) && (this->isSurface()))
    {
        UInt numEdgesOnBoundary = 0;

        for (UInt j = 0; j < fel->numVertices(); j++)
        {
            Data::Vertex *v2 = fel->getVertex(j);

            if (v2 == v)
            {
                continue;
            }

            if (!this->adjacencies[const_cast<Data::/*Front*/Element *>(fel)][v2])
            {
                //return false;
                numEdgesOnBoundary++;
            }
        }

        for (UInt j = 0; j < adj->numVertices(); j++)
        {
            Data::Vertex *v2 = adj->getVertex(j);

            if (v2 == n)
            {
                continue;
            }

            if (!this->adjacencies[const_cast<Data::/*Front*/Element *>(adj)][v2])
            {
                //return false;
                numEdgesOnBoundary++;
            }
        }

        if (numEdgesOnBoundary > this->maxEdgesOnBoundary)
        {
            return false;
        }
    }

	UInt j = adj->index(n);

	Data::Triangle3D t1(fel->getVertex((i+2)%fel->numVertices())->getPoint(), v->getPoint(), adj->getVertex((j+2)%adj->numVertices())->getPoint());
	Data::Triangle3D t2(adj->getVertex((j+2)%adj->numVertices())->getPoint(), n->getPoint(), fel->getVertex((i+2)%fel->numVertices())->getPoint());

	if ((this->dimension == 3) && (this->normalTestEnabled))
	{
		Data::EVector3D nor = static_cast<Data::Triangle3D *>(fel->getShape())->enormal();
#if USE_EXPERIMENTAL_3
		Data::EVector3D vec(static_cast<Data::Point3D *>(fel->getVertex((i+2)%fel->numVertices())->getPoint())->epoint3D(),
            static_cast<Data::Point3D *>(adj->getVertex((j+2)%adj->numVertices())->getPoint())->epoint3D());
#else
		Data::EVector3D vec(fel->getVertex((i+2)%fel->numVertices())->getPoint()->epoint(), adj->getVertex((j+2)%adj->numVertices())->getPoint()->epoint());
#endif //#if USE_EXPERIMENTAL_3

		Data::EReal nnormsq = nor.squaredNorm();
		Data::EReal vnormsq = vec.squaredNorm();

		Data::EReal thr = nnormsq*vnormsq;
		thr *= Data::getTolerance();
		thr *= Data::getTolerance();

		Data::EReal dotsq = nor.dot(vec);
		dotsq *= dotsq;

		if (dotsq > thr)
		{
			return false;
		}

		Data::EVector3D nt = t1.enormal();

		if (nt.dot(nor) <= 0.0)
		{
			return false;
		}

		nt = t2.enormal();

		if (nt.dot(nor) <= 0.0)
		{
			return false;
		}
	}

	Real angVFel, angVAdj, angNFel, angNAdj;

	if (this->dimension == 2)
	{
        Data::Edge2D e;

        Data::Vertex *felv3 = fel->getVertex((i+2)%fel->numVertices());
        Data::Vertex *adjv3 = adj->getVertex((j+2)%fel->numVertices());

        e.setPoints(n->getPoint(), felv3->getPoint());
        angVFel = e.angle(v->getPoint());

        e.setPoints(adjv3->getPoint(), n->getPoint());
        angVAdj = e.angle(v->getPoint());

        e.setPoints(felv3->getPoint(), v->getPoint());
        angNFel = e.angle(n->getPoint());

        e.setPoints(v->getPoint(), adjv3->getPoint());
        angNAdj = e.angle(n->getPoint());
	}
	else
	{
        Data::Edge3D e;

        Data::Vertex *felv3 = fel->getVertex((i+2)%fel->numVertices());
        Data::Vertex *adjv3 = adj->getVertex((j+2)%fel->numVertices());

        e.setPoints(n->getPoint(), felv3->getPoint());
        angVFel = e.angle(v->getPoint());

        e.setPoints(adjv3->getPoint(), n->getPoint());
        angVAdj = e.angle(v->getPoint());

        e.setPoints(felv3->getPoint(), v->getPoint());
        angNFel = e.angle(n->getPoint());

        e.setPoints(v->getPoint(), adjv3->getPoint());
        angNAdj = e.angle(n->getPoint());
	}

	if (((angVFel + angVAdj) >= M_PI - Data::getTolerance()) ||
        ((angNFel + angNAdj) >= M_PI - Data::getTolerance()))
    {
        return false;
    }

	Real q1 = this->quality(static_cast<Data::Triangle *>(fel->getShape()));
	Real q2 = this->quality(static_cast<Data::Triangle *>(adj->getShape()));

	Real min = std::min(q1, q2);

	return ((this->quality(&t1) > min) && (this->quality(&t2) > min));
}

bool Performer::EdgeSwapBoundaryAdapter::addToElements(const Data::/*Front*/Element *fel) const
{
	/*if (fel->getId() == 723419)
    {
        std::cout << "debug" << std::endl;
    }*/

	Real q = this->quality(static_cast<Data::Triangle *>(fel->getShape()));

	if (q > this->qualityThreshold)
	{
		return false;
	}

	this->elements.insert(std::make_pair(q, const_cast<Data::/*Front*/Element *>(fel)));

	return true;
}

bool Performer::EdgeSwapBoundaryAdapter::removeFromElements(const Data::/*Front*/Element *fel) const
{
	Real q = this->quality(static_cast<Data::Triangle *>(fel->getShape()));

	if (q > this->qualityThreshold)
	{
		return false;
	}

	std::pair<RealEMultiMap::iterator, RealEMultiMap::iterator> range = this->elements.equal_range(q);

	for (RealEMultiMap::iterator iter2 = range.first;
		 iter2 != range.second; iter2++)
	{
		if ((*iter2).second == fel)
		{
			this->elements.erase(iter2);

			return true;
		}
	}

	return false;
}

AdjacenciesIdentifier *Performer::EdgeSwapBoundaryAdapter::makeAdjacenciesIdentifier() const
{
	AdjacenciesIdentifier *identifier = new AdjacenciesIdentifier();

	if (this->isBoundary())
    {
        identifier->setBoundary(this->getBoundary());
    }
    else
    {
        identifier->setSurface(this->getSurface());

        identifier->setAllowHangingVertices(true);
    }

	return identifier;
}

bool Performer::EdgeSwapBoundaryAdapter::identifyAdjacencies()
{
    if (!this->adjacencies.empty())
    {
        return true;
    }

	AdjacenciesIdentifier *identifier = this->makeAdjacenciesIdentifier();

	identifier->execute();

	identifier->getAdjacenciesSwapping(this->adjacencies);

	delete identifier;

	return !this->adjacencies.empty();
}

void Performer::EdgeSwapBoundaryAdapter::identifyQualities()
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

void Performer::EdgeSwapBoundaryAdapter::executeAdaptation()
{
	Data::/*Front*/ElementList newElements;
	Data::VertexList newVertices;

	while (!this->elements.empty())
	{
		RealEMultiMap::iterator iter = this->elements.begin();

		Data::/*Front*/Element *fel = (*iter).second;

		this->elements.erase(iter);

		this->adapt(fel, newElements, newVertices);
	}
}
