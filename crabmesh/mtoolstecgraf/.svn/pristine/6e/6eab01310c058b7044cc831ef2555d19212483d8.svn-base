#include "MJMesh/TopologyFrontAdvancer.h"

#include "MJMesh/FrontElement.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"

using namespace MJMesh;

MJMesh::TopologyFrontAdvancer::TopologyFrontAdvancer(UInt dimension,
	const MJMesh::Boundary *boundary, const Data::BoxTree *tree, MJMesh::Front *front,
	MJMesh::Mesh *mesh, bool usingSearchTrees) :
	MJMesh::FrontAdvancer(dimension, boundary, tree, usingSearchTrees)
{
	this->setFront(front);

	this->setMesh(mesh);
}

MJMesh::TopologyFrontAdvancer::~TopologyFrontAdvancer()
{

}

void MJMesh::TopologyFrontAdvancer::setFront(MJMesh::Front *front)
{
	this->front = front;
}

void MJMesh::TopologyFrontAdvancer::setMesh(MJMesh::Mesh *mesh)
{
	this->mesh = mesh;
}

void MJMesh::TopologyFrontAdvancer::execute()
{
	//Real debugstart = Data::time();

	if ((this->status.getType() == FrontAdvancer::notInit.getType()) && (this->front))
	{
		static_cast<MJMesh::Front *>(this->front)->prepareForTopologyPhase(this->usingSearchTrees);
	}

	//this->timeInit += Data::time() - debugstart;

	MJMesh::FrontAdvancer::execute();

	//debugstart = Data::time();

	if ((this->status.getType() == FrontAdvancer::ended.getType()) && (this->front) && (this->usingSearchTrees))
	{
		//to clear the search trees
		static_cast<MJMesh::Front *>(this->front)->prepareForTopologyPhase(false);
	}

	//this->timeEnd += Data::time() - debugstart;
}

bool MJMesh::TopologyFrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point, Real /*radius*/) const
{
	return element->accordingToNormal(point, false);
}

Data::PointList MJMesh::TopologyFrontAdvancer::ideal(const Data::FrontElement *element, Real factor) const
{
	return (this->usingSearchTrees) ? MJMesh::FrontAdvancer::ideal(element, factor) : Data::PointList();
}

Real MJMesh::TopologyFrontAdvancer::radius(const Data::FrontElement *element) const
{
	return (this->usingSearchTrees) ? MJMesh::FrontAdvancer::radius(element) : -1.0;
}

Data::VertexList MJMesh::TopologyFrontAdvancer::candidates(const Data::FrontElement *element) const
{
	//if ((!this->usingSearchTrees) || (static_cast<MJMesh::Front *>(this->front)->isUsingRejected()))
	//if (!this->usingSearchTrees)
	if ((!this->usingSearchTrees) ||
        ((this->dimension == 2) && (static_cast<MJMesh::Front *>(this->front)->isUsingRejected())))
	{
		return MJMesh::FrontAdvancer::candidates(element);
	}

	Data::PointList ideal = this->ideal(element);

	Real radius = 3.0*this->radius(element);

	Data::VertexList vertices = this->front->verticesInRange(ideal, radius + Data::getTolerance());

	while (!ideal.empty())
	{
		delete ideal.back();

		ideal.pop_back();
	}

	return vertices;
}

Data::Vertex *MJMesh::TopologyFrontAdvancer::best(const Data::FrontElement *element) const
{
	//return MJMesh::FrontAdvancer::best(element, false);
	return this->bestWithoutInsertion(element);
}
