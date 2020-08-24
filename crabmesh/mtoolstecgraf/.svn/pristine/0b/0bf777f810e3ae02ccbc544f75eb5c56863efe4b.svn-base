#include "MJMesh/FrontElement.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/Element.h"
#include "MJMesh/Boundary.h"
#include "Data/GeometricShape.h"
#include "Data/Point.h"

using namespace MJMesh;

MJMesh::FrontElement::FrontElement(Data::GeometricShape *shape, ULInt id) :
	Data::FrontElement(shape, id)
{
    /*if (this->getId() == 186413)
    {
        std::cout << "creating fel " << id << " at " << this << ", backtrace: " << std::endl;
        std::cout << Data::trace() << std::endl;
    }*/
}

MJMesh::FrontElement::~FrontElement()
{
    /*if (this->getId() == 100)
    {
        std::cout << "deleting fel " << this->getId() << std::endl;
    }*/

	/*while (!this->vertices.empty())
	{
		MJMesh::Vertex *v = (MJMesh::Vertex *)this->vertices.back();
		this->vertices.pop_back();

		v->remove(this);
	}*/

	/*if (this->getId() == 186413)
    {
        //std::cout << "deleting fel " << this->text() << " at " << this << ", backtrace: " << std::endl;
        std::cout << "deleting fel " << this->getId() << " at " << this << ", backtrace: " << std::endl;
        std::cout << Data::trace() << std::endl;
    }*/

	for (UInt i = 0; i < this->numVertices(); i++)
	{
		if (this->vertices[i])
		{
			static_cast<MJMesh::Vertex *>(this->vertices[i])->remove(this);
		}
	}

#if USE_ARRAY
#if USE_EXPERIMENTAL
	if (this->vertices)
#endif //#if USE_EXPERIMENTAL
	{
		this->numVerts = 0;

		delete [] this->vertices;

		this->vertices = NULL;
	}
#endif //#if USE_ARRAY

	/*//debug
	if (!this->data.meshes.empty())
	{
		std::cout << "FrontElement " << this->getId() << " still in mesh" << std::endl;
		std::cout << Data::trace() << std::endl;
	}

	if (!this->data.fronts.empty())
	{
		std::cout << "FrontElement " << this->getId() << " still in front" << std::endl;
		std::cout << Data::trace() << std::endl;
	}

	if (!this->data.boundaries.empty())
	{
		std::cout << "FrontElement " << this->getId() << " still in boundary" << std::endl;
		std::cout << Data::trace() << std::endl;
	}
	//endebug*/
}

MJMesh::Vertex *MJMesh::FrontElement::getV1() const
{
	return static_cast<MJMesh::Vertex *>(this->getVertex(0));
}

MJMesh::Vertex *MJMesh::FrontElement::getV2() const
{
	return static_cast<MJMesh::Vertex *>(this->getVertex(1));
}

const Data::ElementList &MJMesh::FrontElement::getAdjacency() const
{
	return this->adjacency;
}

void MJMesh::FrontElement::add(MJMesh::Element *e) const
{
	//debug
	//std::cout << "adding element " << e->text() << " as an adjacent of fel " << this->text() << std::endl;
	//endebug

	/*if ((this->id == 161) || (this->id == 162))
	{
		std::cout << "debug frontelement::add " << this->text() << std::endl;
	}*/

	this->adjacency.push_back(e);

	/*//debug
	if (this->adjacency.size() > 2)
	{
	    std::cout << " temporary inconsistency at fel " << this->text() << " by adding e " << e->text() << std::endl;
	}
    //endebug*/
}

void MJMesh::FrontElement::remove(MJMesh::Element *e) const
{
	/*if (this->id == 650)
	{
		std::cout << "debug frontelement::remove " << this->text() << std::endl;
	}

	UInt debug = this->adjacency.size();*/

	this->adjacency.remove(e);

	/*//debug
	if ((this->adjacency.size() != debug) && (debug > 2))
    {
	    std::cout << " temporary inconsistency at fel " << this->text() << " by removing e " << e->text() << std::endl;
	}
    //endebug*/
}

bool MJMesh::FrontElement::haveAdjacent(const MJMesh::Element *e) const
{
	return (std::find(this->adjacency.begin(), this->adjacency.end(), e) != this->adjacency.end());
}

void MJMesh::FrontElement::clearAdjacency() const
{
	this->adjacency.clear();
}

Data::ElementList::iterator MJMesh::FrontElement::eBegin()
{
    return this->adjacency.begin();
}

Data::ElementList::iterator MJMesh::FrontElement::eEnd()
{
    return this->adjacency.end();
}

Data::ElementList::const_iterator MJMesh::FrontElement::eBegin() const
{
    return this->adjacency.begin();
}

Data::ElementList::const_iterator MJMesh::FrontElement::eEnd() const
{
    return this->adjacency.end();
}

Data::ElementList::iterator MJMesh::FrontElement::erase(Data::ElementList::iterator &iter)
{
    /*if (this->adjacency.size() > 2)
    {
	    std::cout << " temporary inconsistency at fel " << this->text() << " by erasing e " << (*iter)->text() << std::endl;
	}*/

    return this->adjacency.erase(iter);
}

UInt MJMesh::FrontElement::numAdjacents() const
{
	return static_cast<UInt>(this->adjacency.size());
}

Real MJMesh::FrontElement::angle(const Data::Point &p) const
{
	return this->angle(&p);
}

Real MJMesh::FrontElement::rank(const Data::Point *p, const Data::PointList &/*ideal*/) const
{
	return this->rank(p);
}

Real MJMesh::FrontElement::rank(const Data::Point *p) const
{
	return this->angle(p);
}

/*void MJMesh::FrontElement::remove(MJMesh::Vertex *v)
{
	VertexVector::iterator it = std::find(this->vertices.begin(), this->vertices.end(), v);

	if (it != this->vertices.end())
	{
		this->vertices.erase(it);
	}
}*/

void MJMesh::FrontElement::invert()
{
	/*if ((this->id == 163) || (this->id == 164))
	{
		std::cout << "debug frontelement::inverting " << this->text() << std::endl;
	}*/

	//swap 1st vertex/point with 2nd vertex/point
	Data::Vertex *v = this->getVertex(0);
	Data::Point *p = this->getShape()->getPoint(0);

	this->vertices[0] = this->getVertex(1);
	this->getShape()->setPoint(0, this->getShape()->getPoint(1));

	this->vertices[1] = v;
	this->getShape()->setPoint(1, p);
}

MJMesh::FrontElementSpecificData &MJMesh::FrontElement::getData() const
{
	return this->data;
}

void MJMesh::FrontElement::addBoundaryData(const MJMesh::Boundary *boundary, const Data::Boundary::FrontElementIterator &boundaryIt) const
{
	this->data.boundaries[boundary] = boundaryIt;
}

void MJMesh::FrontElement::removeBoundaryData(const MJMesh::Boundary *boundary) const
{
	this->data.boundaries.erase(boundary);
}

bool MJMesh::FrontElement::isInBoundary(const MJMesh::Boundary *boundary) const
{
	return (this->data.boundaries.find(boundary) != this->data.boundaries.end());
}

const Data::Boundary::FrontElementIterator &MJMesh::FrontElement::getBoundaryData(const MJMesh::Boundary *boundary) const
{
	return (*this->data.boundaries.find(boundary)).second;
}

Data::Boundary::FrontElementIterator MJMesh::FrontElement::getAndRemoveBoundaryData(const MJMesh::Boundary *boundary) const
{
	FrontElementSpecificData::BoundaryDataMap::iterator iter = this->data.boundaries.find(boundary);

	Data::Boundary::FrontElementIterator it = (*iter).second;

	this->data.boundaries.erase(iter);

	return it;
}

void MJMesh::FrontElement::swapBoundaryData(const MJMesh::Boundary *boundary, const MJMesh::Boundary *other) const
{
	FrontElementSpecificData::BoundaryDataMap::iterator bit = this->data.boundaries.find(boundary);
	FrontElementSpecificData::BoundaryDataMap::iterator oit = this->data.boundaries.find(other);

	Data::Boundary::FrontElementIterator tmp = (*bit).second;
	(*bit).second = (*oit).second;
	(*oit).second = tmp;
}

void MJMesh::FrontElement::replaceBoundaryData(const MJMesh::Boundary *boundary, const MJMesh::Boundary *other) const
{
	FrontElementSpecificData::BoundaryDataMap::iterator bit = this->data.boundaries.find(boundary);

	this->data.boundaries[other] = (*bit).second;

	this->data.boundaries.erase(bit);
}

void MJMesh::FrontElement::addFrontData(const Front *front) const
{
	this->data.fronts.insert(std::make_pair(front, FrontElementSpecificData::FrontSpecificData()));
}

void MJMesh::FrontElement::addFrontData(const Front *front, UShort state, const Data::Front::FrontElementIterator &frontIt) const
{
	FrontElementSpecificData::FrontSpecificData *fsp = &this->data.fronts[front];
	fsp->state = state;
	fsp->frontIt = frontIt;
}

void MJMesh::FrontElement::addFrontData(const Front *front,
	const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const
{
	FrontElementSpecificData::FrontSpecificData *fsp = &this->data.fronts[front];

	SpecificData::SearchTreeNodeSpecificData stndata;
	stndata.stnode = stnode;
	stndata.stnodeIt = stnodeIt;

	fsp->searchNodes.push_back(stndata);
}

/*void MJMesh::FrontElement::addFrontData(const Front *front, UShort state, const Data::Front::FrontElementIterator &frontIt,
	const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const
{
	SpecificData::FrontSpecificData fsp;

	fsp.state = state;
	fsp.frontIt = frontIt;
	fsp.stnode = stnode;
	fsp.stnodeIt = stnodeIt;

	this->data.fronts[front] = fsp;
}*/

void MJMesh::FrontElement::removeFrontData(const Front *front) const
{
	this->data.fronts.erase(front);
}

MJMesh::FrontElementSpecificData::FrontSpecificData MJMesh::FrontElement::getAndRemoveFrontData(const Front *front) const
{
	FrontElementSpecificData::FrontDataMap::iterator iter = this->data.fronts.find(front);

	FrontElementSpecificData::FrontSpecificData fsp = (*iter).second;

	this->data.fronts.erase(iter);

	return fsp;
}

void MJMesh::FrontElement::setFrontData(const Front *front, UShort state, const Data::Front::FrontElementIterator &frontIt) const
{
	FrontElementSpecificData::FrontDataMap::iterator iter = this->data.fronts.find(front);

	(*iter).second.state = state;
	(*iter).second.frontIt = frontIt;
}

void MJMesh::FrontElement::setFrontData(const Front *front, const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const
{
	FrontElementSpecificData::FrontDataMap::iterator iter = this->data.fronts.find(front);

	for (SpecificData::SearchTreeDataList::iterator iter2 = (*iter).second.searchNodes.begin();
		 iter2 != (*iter).second.searchNodes.end(); iter2++)
	{
		if ((*iter2).stnode == stnode)
		{
			(*iter2).stnodeIt = stnodeIt;

			break;
		}
	}
}

/*void MJMesh::FrontElement::setFrontData(const Front *front, UShort state, const Data::Front::FrontElementIterator &frontIt,
	const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const
{
	SpecificData::FrontDataMap::iterator iter = this->data.fronts.find(front);

	(*iter).second.state = state;
	(*iter).second.frontIt = frontIt;
	(*iter).second.stnode = stnode;
	(*iter).second.stnodeIt = stnodeIt;
}*/

bool MJMesh::FrontElement::isInFront(const Front *front) const
{
	return (this->data.fronts.find(front) != this->data.fronts.end());
}

bool MJMesh::FrontElement::isInAnyFront() const
{
	return (!this->data.fronts.empty());
}

bool MJMesh::FrontElement::isInAnyFrontOtherThan(const Front *front) const
{
	UInt numFronts = static_cast<UInt>(this->data.fronts.size());

	return ((numFronts == 1) && ((*this->data.fronts.begin()).first != front)) ? true : (numFronts > 1);
}

MJMesh::FrontElementSpecificData::FrontSpecificData &MJMesh::FrontElement::getFrontData(const Front *front) const
{
	return (*this->data.fronts.find(front)).second;
}

UShort MJMesh::FrontElement::getFrontState(const Front *front) const
{
	return this->getFrontData(front).state;
}

const Data::Front::FrontElementIterator &MJMesh::FrontElement::getFrontIterator(const Front *front) const
{
	return this->getFrontData(front).frontIt;
}

MJMesh::FrontElementSpecificData::SearchTreeDataList &MJMesh::FrontElement::getFrontSTNodes(const Front *front) const
{
	return this->getFrontData(front).searchNodes;
}

/*const Data::GeometricSearchTreeNode *MJMesh::FrontElement::getFrontSTNode(const Front *front) const
{
	return this->getFrontData(front).stnode;
}

const Data::GeometricSearchTreeNode::ShapeSet::iterator &MJMesh::FrontElement::getSTNodeIterator(const Front *front) const
{
	return this->getFrontData(front).stnodeIt;
}*/

void MJMesh::FrontElement::addMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::FrontElementIterator &meshIt) const
{
	this->data.meshes[mesh] = meshIt;
}

void MJMesh::FrontElement::removeMeshData(const MJMesh::Mesh *mesh) const
{
	this->data.meshes.erase(mesh);
}

bool MJMesh::FrontElement::isInMesh(const MJMesh::Mesh *mesh) const
{
	return (this->data.meshes.find(mesh) != this->data.meshes.end());
}

void MJMesh::FrontElement::setMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::FrontElementIterator &meshIt) const
{
	(*this->data.meshes.find(mesh)).second = meshIt;
}

const Data::Mesh::FrontElementIterator &MJMesh::FrontElement::getMeshIterator(const MJMesh::Mesh *mesh) const
{
	return (*this->data.meshes.find(mesh)).second;
}

Data::Mesh::FrontElementIterator MJMesh::FrontElement::getAndRemoveMeshIterator(const MJMesh::Mesh *mesh) const
{
	FrontElementSpecificData::MeshDataMap::iterator iter = this->data.meshes.find(mesh);

	Data::Mesh::FrontElementIterator it = (*iter).second;

	this->data.meshes.erase(iter);

	return it;
}

void MJMesh::FrontElement::swapMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const
{
	FrontElementSpecificData::MeshDataMap::iterator mit = this->data.meshes.find(mesh);
	FrontElementSpecificData::MeshDataMap::iterator oit = this->data.meshes.find(other);

	Data::Mesh::FrontElementIterator tmp = (*mit).second;
	(*mit).second = (*oit).second;
	(*oit).second = tmp;
}

void MJMesh::FrontElement::replaceMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const
{
	FrontElementSpecificData::MeshDataMap::iterator mit = this->data.meshes.find(mesh);

	this->data.meshes[other] = (*mit).second;

	this->data.meshes.erase(mit);
}

#if USE_ARRAY
MJMesh::FrontElement &MJMesh::FrontElement::operator=(const MJMesh::FrontElement &e)
{
	if (this != &e)
	{
		Data::FrontElement::operator=(e);

		this->adjacency = static_cast<const MJMesh::FrontElement &>(e).adjacency;
	}

	return *this;
}
#endif //#if USE_ARRAY

std::string MJMesh::FrontElement::text(const Data::AbstractMesh *mesh) const
{
	std::stringstream str;

	str << Data::FrontElement::text(mesh)
		<< "; adj = " << this->adjacency.size() << ":";

	for (Data::ElementList::const_iterator iter = this->adjacency.begin();
		 iter != this->adjacency.end(); iter++)
	{
		if ((!mesh) || (mesh->have((*iter))))
        {
            str << " " << (*iter)->getId();
        }
	}

	return str.str();
}
