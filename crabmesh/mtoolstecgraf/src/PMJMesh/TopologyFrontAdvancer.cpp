#include "PMJMesh/TopologyFrontAdvancer.h"

#include "Data/Point.h"
#include "MJMesh/FrontElement.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/GeometryFrontAdvancer.h"

using namespace PMJMesh;

PMJMesh::TopologyFrontAdvancer::TopologyFrontAdvancer(UInt dimension,
	const MJMesh::Boundary *boundary, const Data::BoxTree *tree, PMJMesh::Front *front,
	MJMesh::Mesh *mesh, const Data::Box *box, Parallel::Communicator *comm,
	Real rangeProportion, bool usingSearchTrees, bool testingBox) :
	MJMesh::TopologyFrontAdvancer(dimension, boundary, tree, front, mesh, usingSearchTrees)
{
	this->gfa = new PMJMesh::GeometryFrontAdvancer(dimension, boundary, tree, comm, rangeProportion, usingSearchTrees, testingBox);

	this->gfa->setBox(box);
#if USE_DELETE_FELS
	this->gfa->setFront(front);
#endif //#if USE_DELETE_FELS
}

PMJMesh::TopologyFrontAdvancer::~TopologyFrontAdvancer()
{
	if (this->gfa)
	{
		this->gfa->setBox(NULL);
#if USE_DELETE_FELS
		this->gfa->setFront(NULL);
#endif //#if USE_DELETE_FELS

		delete this->gfa;
	}
}

void PMJMesh::TopologyFrontAdvancer::setTestingBox(bool testingBox)
{
    if (this->gfa)
    {
        this->gfa->setTestingBox(testingBox);
    }
}

bool PMJMesh::TopologyFrontAdvancer::isTestingBox() const
{
    return (this->gfa) ? this->gfa->isTestingBox() : false;
}

void PMJMesh::TopologyFrontAdvancer::setRangeProportion(Real rangeProportion)
{
	if (this->gfa)
	{
		this->gfa->setRangeProportion(rangeProportion);
	}
}

Real PMJMesh::TopologyFrontAdvancer::getRangeProportion() const
{
	return (this->gfa) ? this->gfa->getRangeProportion() : 0.0;
}

#if USE_DELETE_FELS
void PMJMesh::TopologyFrontAdvancer::setOldFront(const PMJMesh::Front *oldFront)
{
    if (this->gfa)
    {
        return this->gfa->setOldFront(oldFront);
    }
}
#endif //#if USE_DELETE_FELS

Data::FrontElement *PMJMesh::TopologyFrontAdvancer::next() const
{
/*#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

	if (!this->isTestingBox())
	{
/*#if DEBUG_TIMING_PMJMESH
		Data::FrontElement *fel = MJMesh::TopologyFrontAdvancer::next();

		this->timeNextTopology += Data::time() - debugstart;

		return fel;
#else*/
	    return MJMesh::TopologyFrontAdvancer::next();
//#endif //#if DEBUG_TIMING_PMJMESH
	}

	Data::FrontElement *fel = NULL;

	while (!fel)
	{
		/*if (this->isEmpty())
		{
			break;
		}*/

		fel = MJMesh::TopologyFrontAdvancer::next();

		if (!fel)
		{
			break;
		}

		Data::PointList ideal = gfa->ideal(fel);

		if (ideal.empty())
		{
/*#if DEBUG_TIMING_PMJMESH
			this->timeNextTopology += Data::time() - debugstart;

			debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

			this->reject(fel, 3);

/*#if DEBUG_TIMING_PMJMESH
			this->timeNextTopologyReject += Data::time() - debugstart;

			debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

			fel = NULL;

			continue;
		}

		Real radius = gfa->radius(fel);

		bool valid = true;

		for (Data::PointList::const_iterator iter = ideal.begin();
			 iter != ideal.end(); iter++)
		{
			if ((valid) && (gfa->isRestricted(fel, (*iter), radius)))
			{
				valid = false;
			}

			delete (*iter);
		}

		if (!valid)
		{
/*#if DEBUG_TIMING_PMJMESH
			this->timeNextTopology += Data::time() - debugstart;

			debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

			this->reject(fel, 3);

/*#if DEBUG_TIMING_PMJMESH
			this->timeNextTopologyReject += Data::time() - debugstart;

			debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH*/

			fel = NULL;
		}
	}

/*#if DEBUG_TIMING_PMJMESH
	this->timeNextTopology += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH*/

	return fel;
}

#if USE_DELETE_FELS
bool PMJMesh::TopologyFrontAdvancer::shouldDelete(const Data::FrontElement *e) const
{
    return this->gfa->shouldDelete(e);
}
#endif //#if USE_DELETE_FELS

bool PMJMesh::TopologyFrontAdvancer::better(const Data::FrontElement *element, Data::Vertex *candidate, Data::Vertex *old) const
{
	return this->gfa->better(element, candidate, old);
}

Data::Vertex *PMJMesh::TopologyFrontAdvancer::bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, Real radius) const
{
	this->gfa->restricted(candidates);

	return MJMesh::TopologyFrontAdvancer::bestValid(element, candidates, ideal, radius);
}

Data::Vertex *PMJMesh::TopologyFrontAdvancer::bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::VertexList &vertices, Real radius) const
{
	this->gfa->restricted(candidates);

	return MJMesh::TopologyFrontAdvancer::bestValid(element, candidates, ideal, vertices, radius);
}

Data::Vertex *PMJMesh::TopologyFrontAdvancer::bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::FrontElementList &frontElements, Real radius) const
{
	this->gfa->restricted(candidates);

	return MJMesh::TopologyFrontAdvancer::bestValid(element, candidates, ideal, frontElements, radius);
}

Data::Vertex *PMJMesh::TopologyFrontAdvancer::bestValid(const Data::FrontElement *element, Data::VertexList &candidates, const Data::PointList &ideal, const Data::VertexList &vertices, const Data::FrontElementList &frontElements, Real radius) const
{
	this->gfa->restricted(candidates);

	return MJMesh::TopologyFrontAdvancer::bestValid(element, candidates, ideal, vertices, frontElements, radius);
}

bool PMJMesh::TopologyFrontAdvancer::reject(Data::FrontElement *e, UInt code) const
{
	return ((code == 0) && (static_cast<MJMesh::FrontElement *>(e)->getFrontState(static_cast<MJMesh::Front *>(this->front)) == 2)) ?
		this->front->reject(e, 2) : this->front->reject(e, code);
}
