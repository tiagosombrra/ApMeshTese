#include "MJMesh/Vertex.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Edge.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"

#include "Data/OStream.h"

using namespace MJMesh;

MJMesh::Vertex::Vertex(Data::Point *p, ULInt id) :
	Data::Vertex(p, id)
{

}

MJMesh::Vertex::Vertex(Real x, Real y, ULInt id) :
	Data::Vertex(new Data::Point2D(x, y), id)
{

}

MJMesh::Vertex::Vertex(Real x, Real y, Real z, ULInt id) :
	Data::Vertex(new Data::Point3D(x, y, z), id)
{

}

MJMesh::Vertex::~Vertex()
{
	/*//debug
    if (this->getId() == 160)
    {
        Data::cout() << "MJMesh::Vertex deleting v " << this->getId() << " at " << this << std::endl << "trace: " << Data::trace() << Data::endl;
    }
    //endebug*/

	/*if (this->getId() != 0)
	{
		Data::cout() << "deleting vertex " << this << ": " << this->getId() << std::endl << Data::trace() << Data::endl;
	}*/

	/*while (!this->adjacentElements.empty())
	{
		MJMesh::Element *e = (MJMesh::Element *)this->adjacentElements.back();

		this->adjacentElements.pop_back();

		e->remove(this);
	}

	while (!this->adjacentFrontElements.empty())
	{
		MJMesh::FrontElement *e = (MJMesh::FrontElement *)this->adjacentFrontElements.back();

		this->adjacentFrontElements.pop_back();

		e->remove(this);
	}*/

	this->adjacentElements.clear();
	this->adjacentFrontElements.clear();

	/*if ((this->getPoint()) && (this->getPoint()->dimension() == 3))
	{
		while (!this->adjacency.empty())
		{
			Data::Edge *e = this->adjacency.back();
			this->adjacency.pop_back();

			delete e;
		}
	}*/

	/*//debug
	if (!this->data.meshes.empty())
	{
		std::cout << "Vertex " << this->getId() << " still in mesh" << std::endl;
		std::cout << Data::trace() << std::endl;
	}

	if (!this->data.fronts.empty())
	{
		std::cout << "Vertex " << this->getId() << " still in front" << std::endl;
		std::cout << Data::trace() << std::endl;
	}

	if (!this->data.boundaries.empty())
	{
		std::cout << "Vertex " << this->getId() << " still in boundary" << std::endl;
		std::cout << Data::trace() << std::endl;
	}
	//endebug*/
}

void MJMesh::Vertex::add(Data::Element *e)
{
    //debug
//    if (this->getId() == 484)
//    {
//        Data::cout() << "adding element at " << e << " as adj of vertex at " << this << Data::endl;
//        Data::cout() << "adding element " << e->text() << " as adj of vertex " << this->text() << Data::endl;
//        Data::cout() << "trace " << Data::trace() << Data::endl;
//    }
    //debug

	this->adjacentElements.push_back(e);
}

void MJMesh::Vertex::remove(Data::Element *e)
{
    //debug
//    if (this->getId() == 484)
//    {
//        Data::cout() << "removing element at " << e << " from adjs of vertex at " << this << Data::endl;
//        Data::cout() << "removing element " << e->text() << " from adjs of vertex " << this->text() << Data::endl;
//        Data::cout() << "trace " << Data::trace() << Data::endl;
//    }
    //debug

	this->adjacentElements.remove(e);
}

bool MJMesh::Vertex::have(Data::Element *e)
{
	return this->adjacentElements.empty() ? false :
			(std::find(this->adjacentElements.begin(), this->adjacentElements.end(), e) != this->adjacentElements.end());
}

void MJMesh::Vertex::clearAdjacentElements()
{
    //debug
//    if (this->getId() == 484)
//    {
//        Data::cout() << "clearing all adj elements of vertex at " << this << Data::endl;
//        Data::cout() << "clearing all adj elements adj of vertex " << this->text() << Data::endl;
//    }
    //debug

    this->adjacentElements.clear();
}

UInt MJMesh::Vertex::numAdjacentElements() const
{
    return static_cast<UInt>(this->adjacentElements.size());
}

Data::ElementList::iterator MJMesh::Vertex::eBegin()
{
    return this->adjacentElements.begin();
}

Data::ElementList::iterator MJMesh::Vertex::eEnd()
{
    return this->adjacentElements.end();
}

Data::ElementList::const_iterator MJMesh::Vertex::eBegin() const
{
    return this->adjacentElements.begin();
}

Data::ElementList::const_iterator MJMesh::Vertex::eEnd() const
{
    return this->adjacentElements.end();
}

Data::ElementList::iterator MJMesh::Vertex::erase(Data::ElementList::iterator &iter)
{
    //debug
//    if (this->getId() == 484)
//    {
//        Data::cout() << "erasing element at " << (*iter) << " from adjs of vertex at " << this << Data::endl;
//        Data::cout() << "erasing element " << (*iter)->text() << " from adjs of vertex " << this->text() << Data::endl;
//        Data::cout() << "trace " << Data::trace() << Data::endl;
//    }
    //debug

    return this->adjacentElements.erase(iter);
}

void MJMesh::Vertex::add(Data::FrontElement *e)
{
	this->adjacentFrontElements.push_back(e);
}

void MJMesh::Vertex::remove(Data::FrontElement *e)
{
	this->adjacentFrontElements.remove(e);
}

bool MJMesh::Vertex::have(Data::FrontElement *e)
{
	return this->adjacentFrontElements.empty() ? false :
			(std::find(this->adjacentFrontElements.begin(), this->adjacentFrontElements.end(), e) != this->adjacentFrontElements.end());
}

void MJMesh::Vertex::clearAdjacentFrontElements()
{
    this->adjacentFrontElements.clear();
}

UInt MJMesh::Vertex::numAdjacentFrontElements() const
{
    return static_cast<UInt>(this->adjacentFrontElements.size());
}

Data::FrontElementList::iterator MJMesh::Vertex::feBegin()
{
    return this->adjacentFrontElements.begin();
}

Data::FrontElementList::iterator MJMesh::Vertex::feEnd()
{
    return this->adjacentFrontElements.end();
}

Data::FrontElementList::const_iterator MJMesh::Vertex::feBegin() const
{
    return this->adjacentFrontElements.begin();
}

Data::FrontElementList::const_iterator MJMesh::Vertex::feEnd() const
{
    return this->adjacentFrontElements.end();
}

Data::FrontElementList::iterator MJMesh::Vertex::erase(Data::FrontElementList::iterator &iter)
{
    return this->adjacentFrontElements.erase(iter);
}

void MJMesh::Vertex::setAdjacentElements(const Data::ElementList &adjacentElements)
{
    //debug
//    if (this->getId() == 484)
//    {
//        Data::cout() << "setting adj elems all adj elements of vertex at " << this << Data::endl;
//        Data::cout() << "setting adj elems all adj elements adj of vertex " << this->text() << Data::endl;
//    }
    //debug

	this->adjacentElements = adjacentElements;
}

const Data::ElementList &MJMesh::Vertex::getAdjacentElements() const
{
	return this->adjacentElements;
}

void MJMesh::Vertex::setAdjacentFrontElements(const Data::FrontElementList &adjacentFrontElements)
{
	this->adjacentFrontElements = adjacentFrontElements;
}

const Data::FrontElementList &MJMesh::Vertex::getAdjacentFrontElements() const
{
	return this->adjacentFrontElements;
}

/*void MJMesh::Vertex::add(Data::Edge *e)
{
	Data::Vertex::add(e);
}

void MJMesh::Vertex::remove(Data::Edge *e)
{
	Data::Vertex::remove(e);
}*/

MJMesh::VertexSpecificData &MJMesh::Vertex::getData() const
{
	return this->data;
}

void MJMesh::Vertex::addBoundaryData(const MJMesh::Boundary *boundary, const Data::Mesh::VertexIterator &boundaryIt) const
{
	this->data.boundaries[boundary] = boundaryIt;
}

void MJMesh::Vertex::removeBoundaryData(const MJMesh::Boundary *boundary) const
{
	this->data.boundaries.erase(boundary);
}

bool MJMesh::Vertex::isInBoundary(const MJMesh::Boundary *boundary) const
{
	return (this->data.boundaries.find(boundary) != this->data.boundaries.end());
}

const Data::Boundary::VertexIterator &MJMesh::Vertex::getBoundaryData(const MJMesh::Boundary *boundary) const
{
	return (*this->data.boundaries.find(boundary)).second;
}

Data::Boundary::VertexIterator MJMesh::Vertex::getAndRemoveBoundaryData(const MJMesh::Boundary *boundary) const
{
	VertexSpecificData::BoundaryDataMap::iterator iter = this->data.boundaries.find(boundary);

	Data::Boundary::VertexIterator it = (*iter).second;

	this->data.boundaries.erase(iter);

	return it;
}

void MJMesh::Vertex::swapBoundaryData(const MJMesh::Boundary *boundary, const MJMesh::Boundary *other) const
{
	VertexSpecificData::BoundaryDataMap::iterator bit = this->data.boundaries.find(boundary);
	VertexSpecificData::BoundaryDataMap::iterator oit = this->data.boundaries.find(other);

	Data::Boundary::VertexIterator tmp = (*bit).second;
	(*bit).second = (*oit).second;
	(*oit).second = tmp;
}

void MJMesh::Vertex::replaceBoundaryData(const MJMesh::Boundary *boundary, const MJMesh::Boundary *other) const
{
	VertexSpecificData::BoundaryDataMap::iterator bit = this->data.boundaries.find(boundary);

	this->data.boundaries[other] = (*bit).second;

	this->data.boundaries.erase(bit);
}

void MJMesh::Vertex::addFrontData(const Front *front) const
{
	this->data.fronts.insert(std::make_pair(front, VertexSpecificData::FrontSpecificData()));
}

void MJMesh::Vertex::addFrontData(const Front *front, const Data::Front::VertexIterator &frontIt) const
{
	VertexSpecificData::FrontSpecificData *fsp = &this->data.fronts[front];
	fsp->frontIt = frontIt;
}

void MJMesh::Vertex::addFrontData(const Front *front,
	const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const
{
	VertexSpecificData::FrontSpecificData *fsp = &this->data.fronts[front];

	SpecificData::SearchTreeNodeSpecificData stndata;
	stndata.stnode = stnode;
	stndata.stnodeIt = stnodeIt;

	fsp->searchNodes.push_back(stndata);
}

/*void MJMesh::Vertex::addFrontData(const Front *front, const Data::Front::VertexIterator &frontIt,
	const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const
{
	SpecificData::FrontSpecificData fsp;

	fsp.frontIt = frontIt;
	fsp.stnode = stnode;
	fsp.stnodeIt = stnodeIt;

	this->data.fronts[front] = fsp;
}*/

void MJMesh::Vertex::removeFrontData(const Front *front) const
{
	this->data.fronts.erase(front);
}

MJMesh::VertexSpecificData::FrontSpecificData MJMesh::Vertex::getAndRemoveFrontData(const Front *front) const
{
	VertexSpecificData::FrontDataMap::iterator iter = this->data.fronts.find(front);

	VertexSpecificData::FrontSpecificData fsp = (*iter).second;

	this->data.fronts.erase(iter);

	return fsp;
}

void MJMesh::Vertex::setFrontData(const Front *front, const Data::Front::VertexIterator &frontIt) const
{
	VertexSpecificData::FrontDataMap::iterator iter = this->data.fronts.find(front);

	(*iter).second.frontIt = frontIt;
}

void MJMesh::Vertex::setFrontData(const Front *front, const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const
{
	VertexSpecificData::FrontDataMap::iterator iter = this->data.fronts.find(front);

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

/*void MJMesh::Vertex::setFrontData(const Front *front, const Data::Front::VertexIterator &frontIt,
	const Data::GeometricSearchTreeNode *stnode, const Data::GeometricSearchTreeNode::ShapeSet::iterator &stnodeIt) const
{
	SpecificData::FrontDataMap::iterator iter = this->data.fronts.find(front);

	(*iter).second.frontIt = frontIt;
	(*iter).second.stnode = stnode;
	(*iter).second.stnodeIt = stnodeIt;
}*/

bool MJMesh::Vertex::isInFront(const Front *front) const
{
	return (this->data.fronts.find(front) != this->data.fronts.end());
}

bool MJMesh::Vertex::isInAnyFront() const
{
	return (!this->data.fronts.empty());
}

bool MJMesh::Vertex::isInAnyFrontOtherThan(const Front *front) const
{
	UInt numFronts = static_cast<UInt>(this->data.fronts.size());

	return ((numFronts == 1) && ((*this->data.fronts.begin()).first != front)) ? true : (numFronts > 1);
}

MJMesh::VertexSpecificData::FrontSpecificData &MJMesh::Vertex::getFrontData(const Front *front) const
{
	return (*this->data.fronts.find(front)).second;
}

const Data::Front::VertexIterator &MJMesh::Vertex::getFrontIterator(const Front *front) const
{
	return this->getFrontData(front).frontIt;
}

MJMesh::VertexSpecificData::SearchTreeDataList &MJMesh::Vertex::getFrontSTNodes(const Front *front) const
{
	return this->getFrontData(front).searchNodes;
}

/*const Data::GeometricSearchTreeNode *MJMesh::Vertex::getFrontSTNode(const Front *front) const
{
	return this->getFrontData(front).stnode;
}

const Data::GeometricSearchTreeNode::ShapeSet::iterator &MJMesh::Vertex::getSTNodeIterator(const Front *front) const
{
	return this->getFrontData(front).stnodeIt;
}*/

void MJMesh::Vertex::addMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt) const
{
	VertexSpecificData::MeshSpecificData *msp = &this->data.meshes[mesh];

	msp->meshIt = meshIt;
}

#if USE_EXPERIMENTAL_MESH
void MJMesh::Vertex::addMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt, bool inner) const
#else
void MJMesh::Vertex::addMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt, bool inner, const Data::Mesh::VertexIterator &innerMeshIt) const
#endif //#if USE_EXPERIMENTAL_MESH
{
	VertexSpecificData::MeshSpecificData *msp = &this->data.meshes[mesh];

	msp->meshIt = meshIt;
	msp->inner = inner;
#if (!USE_EXPERIMENTAL_MESH)
	msp->innerMeshIt = innerMeshIt;
#endif //#if (!USE_EXPERIMENTAL_MESH)
}

void MJMesh::Vertex::addMeshData(const MJMesh::Mesh *mesh, const VertexSpecificData::MeshSpecificData &data) const
{
	this->data.meshes[mesh] = data;
}

void MJMesh::Vertex::removeMeshData(const MJMesh::Mesh *mesh) const
{
	this->data.meshes.erase(mesh);
}

bool MJMesh::Vertex::isInMesh(const MJMesh::Mesh *mesh) const
{
	return (this->data.meshes.find(mesh) != this->data.meshes.end());
}

#if USE_EXPERIMENTAL_MESH
void MJMesh::Vertex::setMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt, bool inner) const
#else
void MJMesh::Vertex::setMeshData(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt, bool inner, const Data::Mesh::VertexIterator &innerMeshIt) const
#endif //#if USE_EXPERIMENTAL_MESH
{
	VertexSpecificData::MeshDataMap::iterator it = this->data.meshes.find(mesh);

	(*it).second.meshIt = meshIt;
	(*it).second.inner = inner;
#if (!USE_EXPERIMENTAL_MESH)
	(*it).second.innerMeshIt = innerMeshIt;
#endif //#if (!USE_EXPERIMENTAL_MESH)
}

void MJMesh::Vertex::setMeshIterator(const MJMesh::Mesh *mesh, const Data::Mesh::VertexIterator &meshIt) const
{
	(*this->data.meshes.find(mesh)).second.meshIt = meshIt;
}

#if USE_EXPERIMENTAL_MESH
void MJMesh::Vertex::setMeshInner(const MJMesh::Mesh *mesh, bool inner) const
#else
void MJMesh::Vertex::setMeshInner(const MJMesh::Mesh *mesh, bool inner, const Data::Mesh::VertexIterator &innerMeshIt) const
#endif //#if USE_EXPERIMENTAL_MESH
{
	VertexSpecificData::MeshDataMap::iterator it = this->data.meshes.find(mesh);

	(*it).second.inner = inner;
#if (!USE_EXPERIMENTAL_MESH)
	(*it).second.innerMeshIt = innerMeshIt;
#endif //#if (!USE_EXPERIMENTAL_MESH)
}

const Data::Mesh::VertexIterator &MJMesh::Vertex::getMeshIterator(const MJMesh::Mesh *mesh) const
{
	return (*this->data.meshes.find(mesh)).second.meshIt;
}

bool MJMesh::Vertex::isMeshInner(const MJMesh::Mesh *mesh) const
{
	return (*this->data.meshes.find(mesh)).second.inner;
}

#if (!USE_EXPERIMENTAL_MESH)
const Data::Mesh::VertexIterator &MJMesh::Vertex::getMeshInnerIterator(const MJMesh::Mesh *mesh) const
{
	return (*this->data.meshes.find(mesh)).second.innerMeshIt;
}
#endif //#if (!USE_EXPERIMENTAL_MESH)

MJMesh::VertexSpecificData::MeshSpecificData MJMesh::Vertex::getAndRemoveMeshData(const MJMesh::Mesh *mesh) const
{
	VertexSpecificData::MeshDataMap::iterator it = this->data.meshes.find(mesh);

	VertexSpecificData::MeshSpecificData msp = (*it).second;

	this->data.meshes.erase(it);

	return msp;
}

void MJMesh::Vertex::swapMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const
{
	VertexSpecificData::MeshDataMap::iterator mit = this->data.meshes.find(mesh);
	VertexSpecificData::MeshDataMap::iterator oit = this->data.meshes.find(other);

	VertexSpecificData::MeshSpecificData tmp = (*mit).second;
	(*mit).second = (*oit).second;
	(*oit).second = tmp;
}

void MJMesh::Vertex::replaceMeshData(const MJMesh::Mesh *mesh, const MJMesh::Mesh *other) const
{
	VertexSpecificData::MeshDataMap::iterator mit = this->data.meshes.find(mesh);

	this->data.meshes[other] = (*mit).second;

	this->data.meshes.erase(mit);
}

#if USE_GUI
void MJMesh::Vertex::highlight() const
{
	if (this->point)
	{
		this->point->highlight();
	}

	for (Data::FrontElementList::const_iterator iter = this->adjacentFrontElements.begin();
		 iter != this->adjacentFrontElements.end(); iter++)
	{
		(*iter)->highlight();
	}

	if (this->point->dimension() == 3)
	{
		for (Data::ElementList::const_iterator iter = this->adjacentElements.begin();
			 iter != this->adjacentElements.end(); iter++)
		{
			(*iter)->highlight();
		}
	}
}

void MJMesh::Vertex::unhighlight() const
{
	if (this->point)
	{
		this->point->unhighlight();
	}

	for (Data::FrontElementList::const_iterator iter = this->adjacentFrontElements.begin();
		 iter != this->adjacentFrontElements.end(); iter++)
	{
		(*iter)->unhighlight();
	}

	if (this->point->dimension() == 3)
	{
		for (Data::ElementList::const_iterator iter = this->adjacentElements.begin();
			 iter != this->adjacentElements.end(); iter++)
		{
			(*iter)->unhighlight();
		}
	}
}
#endif //#if USE_GUI

std::string MJMesh::Vertex::text(const Data::AbstractMesh *mesh) const
{
	std::stringstream str;

	str << Data::Vertex::text(mesh)
		//<< ", adj = " << this->adjacency.size()
		<< "; fe = " << this->adjacentFrontElements.size() << ":";

	for (Data::FrontElementList::const_iterator iter = this->adjacentFrontElements.begin();
		 iter != this->adjacentFrontElements.end(); iter++)
	{
	    if ((!mesh) || (mesh->have((*iter))))
        {
            str << " " << (*iter)->getId();
        }
	}

	str << "; e = " << this->adjacentElements.size() << ":";

	for (Data::ElementList::const_iterator iter = this->adjacentElements.begin();
		 iter != this->adjacentElements.end(); iter++)
	{
		if ((!mesh) || (mesh->have((*iter))))
        {
            str << " " << (*iter)->getId();
        }
	}

	return str.str();
}
