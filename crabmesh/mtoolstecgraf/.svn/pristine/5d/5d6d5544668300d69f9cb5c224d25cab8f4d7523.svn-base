#include "PMJMesh/WorkerLaplacianSmoother.h"

#include "MJMesh/Mesh.h"
#include "MJMesh/Triangle3D.h"
#include "MJMesh/Edge2D.h"
#include "Data/Element.h"
#include "Data/FrontElement.h"
#include "Data/GeometricShape.h"
//debug
#include "Data/Vertex.h"
//endebug

#if (USE_NEW_FRONT_UPDATER /*&& false*/)
#include "MJMesh/Boundary.h"
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)

#if USE_DELETE_FELS
PMJMesh::WorkerLaplacianSmoother::WorkerLaplacianSmoother(UInt dimension,
	Data::Mesh *mesh, bool testAdjacency, Real factor) :
#else
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
PMJMesh::WorkerLaplacianSmoother::WorkerLaplacianSmoother(UInt dimension,
	const MJMesh::Boundary *boundary, bool removeForSmoothing,
	UInt unsmoothingLayer, Data::Mesh *mesh,
	bool testAdjacency, Real factor) :
#else
PMJMesh::WorkerLaplacianSmoother::WorkerLaplacianSmoother(UInt dimension,
	const Data::VertexList &front, const Data::FrontElementList &boundary,
	UInt unsmoothingLayer, Data::Mesh *mesh,
	bool testAdjacency, Real factor) :
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
#endif //#if USE_DELETE_FELS
	MJMesh::LaplacianSmoother(dimension, mesh, testAdjacency, factor)
{
#if (!USE_DELETE_FELS)
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
	this->setBoundary(boundary);
	this->setRemoveForSmoothing(removeForSmoothing);
#else
	this->setFront(front);
	this->setBoundary(boundary);
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)

	this->setUnsmoothingLayer(unsmoothingLayer);
#endif //#if (!USE_DELETE_FELS)
}

PMJMesh::WorkerLaplacianSmoother::~WorkerLaplacianSmoother()
{

}

#if (!USE_DELETE_FELS)
void PMJMesh::WorkerLaplacianSmoother::setUnsmoothingLayer(UInt unsmoothingLayer)
{
	this->unsmoothingLayer = unsmoothingLayer;
}

#if (USE_NEW_FRONT_UPDATER /*&& false*/)
void PMJMesh::WorkerLaplacianSmoother::setBoundary(const MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

void PMJMesh::WorkerLaplacianSmoother::setRemoveForSmoothing(bool removeForSmoothing)
{
	this->removeForSmoothing = removeForSmoothing;
}
#else
void PMJMesh::WorkerLaplacianSmoother::setFront(const Data::VertexList &front)
{
	this->front = front;
}

void PMJMesh::WorkerLaplacianSmoother::setBoundary(const Data::FrontElementList &boundary)
{
	this->boundary = boundary;
}
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
#endif //#if (!USE_DELETE_FELS)

void PMJMesh::WorkerLaplacianSmoother::execute()
{
#if (!USE_DELETE_FELS)
	if (this->status.getType() == this->notInit.getType())
	{
		if (this->removed.empty())
		{
			this->findLayers();
		}

		this->removeLayers();

		/*this->oldInnerVertices = this->mesh->getInnerVertices();

		this->mesh->setInnerVertices(this->findNewInnerVertices());*/

		//removes the front elements that might be a race condition, because
		//  they also may belong to other tasks
		/*for (Data::FrontElementList::const_iterator iter = this->boundary.begin();
			 iter != this->boundary.end(); iter++)
		{
			Data::FrontElement *e = (*iter);

			((MJMesh::Mesh *)this->mesh)->remove(e, false);
		}*/
	}
#endif //#if (!USE_DELETE_FELS)

	MJMesh::LaplacianSmoother::execute();

#if (!USE_DELETE_FELS)
	if (this->status.getType() == this->ended.getType())
	{
		this->addLayers();

		/*this->mesh->setInnerVertices(this->oldInnerVertices);*/

		/*//adds the front elements that were previously removed
		for (Data::FrontElementList::const_iterator iter = this->boundary.begin();
			 iter != this->boundary.end(); iter++)
		{
			Data::FrontElement *e = (*iter);

			this->mesh->add(e);
		}*/
	}
#endif //#if (!USE_DELETE_FELS)
}

#if (!USE_DELETE_FELS)
#if USE_ATTRIBS
void PMJMesh::WorkerLaplacianSmoother::clearAttribs() const
{
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
	for (Data::Mesh::ConstFrontElementIterator iter = this->mesh->feBegin();
		 iter != this->mesh->feEnd(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		if ((this->removeForSmoothing) && (this->boundary->have(fel)))
		{
			continue;
		}

		Data::GeometricShape *s = fel->getShape();

		//if (s->dimension() == 2)
		if (this->dimension == 2)
		{
#if USE_POOL
			static_cast<MJMesh::Edge2D *>(s)->recalcAttribs();
#else
			static_cast<MJMesh::Edge2D *>(s)->clearAttribs();
#endif //#if USE_POOL
		}
		else
		{
#if USE_POOL
			static_cast<MJMesh::Triangle3D *>(s)->recalcAttribs();
#else
			static_cast<MJMesh::Triangle3D *>(s)->clearAttribs();
#endif //#if USE_POOL
		}
	}
#else
	//Data::FrontElementList fels = this->mesh->getFrontElements();

	Data::FrontElementSet boundary(this->boundary.begin(), this->boundary.end());

	//while (!fels.empty())
	for (Data::Mesh::ConstFrontElementIterator iter = this->mesh->feBegin();
		 iter != this->mesh->feEnd(); iter++)
	{
		//Data::FrontElement *fel = fels.front();
		Data::FrontElement *fel = (*iter);

		//fels.pop_front();

		//Data::FrontElementList::const_iterator it = std::find(this->boundary.begin(), this->boundary.end(), fel);
		Data::FrontElementSet::const_iterator it = boundary.find(fel);

		//if (it != this->boundary.end())
		if (it != boundary.end())
		{
			continue;
		}

		Data::GeometricShape *s = fel->getShape();

		//if (s->dimension() == 2)
		if (this->dimension == 2)
		{
#if USE_POOL
			static_cast<MJMesh::Edge2D *>(s)->recalcAttribs();
#else
			static_cast<MJMesh::Edge2D *>(s)->clearAttribs();
#endif //#if USE_POOL
		}
		else
		{
#if USE_POOL
			static_cast<MJMesh::Triangle3D *>(s)->recalcAttribs();
#else
			static_cast<MJMesh::Triangle3D *>(s)->clearAttribs();
#endif //#if USE_POOL
		}
	}
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)
}
#endif //#if USE_ATTRIBS

/*Data::VertexList PMJMesh::WorkerLaplacianSmoother::findNewInnerVertices() const
{
	Data::VertexSet removed = this->mesh->findLayers(this->front, this->unsmoothingLayer);

	Data::VertexList newInnerVertices;

	for (Data::VertexList::const_iterator iter = this->oldInnerVertices.begin();
		 iter != this->oldInnerVertices.end(); iter++)
	{
		Data::Vertex *v = (*iter);

		if (removed.find(v) == removed.end())
		{
			newInnerVertices.push_back(v);
		}
	}

	return newInnerVertices;
}*/

void PMJMesh::WorkerLaplacianSmoother::findLayers()
{
#if (USE_NEW_FRONT_UPDATER /*&& false*/)
	this->removed.clear();
	this->mesh->findLayers(this->boundary->getVertices(), this->unsmoothingLayer, this->removed);
#else
	this->removed = this->mesh->findLayers(this->front, this->unsmoothingLayer);
#endif //#if (USE_NEW_FRONT_UPDATER /*&& false*/)

#if USE_C__11
	for (Data::VertexHash::iterator iter = this->removed.begin();
#else
	for (Data::VertexSet::iterator iter = this->removed.begin();
#endif //#if USE_C__11
		 iter != this->removed.end();)
	{
	    //std::cout << "testing v at " << (*iter) << std::endl;
	    //std::cout << "testing v " << (*iter)->getId() << std::endl;

		if (this->mesh->haveInner((*iter)))
		{
			iter++;
		}
		else
		{
#if USE_C__11
			iter = this->removed.erase(iter);
#else
			this->removed.erase(iter++);
#endif //#if USE_C__11
		}
	}
}

void PMJMesh::WorkerLaplacianSmoother::removeLayers() const
{
#if USE_C__11
	for (Data::VertexHash::const_iterator iter = this->removed.begin();
#else
	for (Data::VertexSet::iterator iter = this->removed.begin();
#endif //#if USE_C__11
		 iter != this->removed.end(); iter++)
	{
		this->mesh->removeInner((*iter));
	}
}

void PMJMesh::WorkerLaplacianSmoother::addLayers() const
{
#if USE_C__11
	for (Data::VertexHash::const_iterator iter = this->removed.begin();
#else
	for (Data::VertexSet::iterator iter = this->removed.begin();
#endif //#if USE_C__11
		 iter != this->removed.end(); iter++)
	{
		this->mesh->addInner((*iter));
	}
}
#endif //#if (!USE_DELETE_FELS)
