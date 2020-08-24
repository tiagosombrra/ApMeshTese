#include "PMJMesh/FinalFrontUpdater.h"

#if (!USE_NEW_FRONT_UPDATER)

#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh/WorkerMainDrive.h"
#include "PMJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/Vertex.h"
#include "Data/Boundary.h"

PMJMesh::FinalFrontUpdater::FinalFrontUpdater(UInt dimension, UInt smoothingLayer,
	bool parallel, bool shared, Data::Boundary *boundary, PMJMesh::Front *front,
	MJMesh::Mesh *mesh, const PMJMesh::TaskManager *taskManager,
	Parallel::Communicator *comm) :
	PMJMesh::EmptyFrontUpdater::EmptyFrontUpdater(dimension, smoothingLayer,
		parallel, shared, boundary, front, mesh, taskManager, comm)
{

}

PMJMesh::FinalFrontUpdater::~FinalFrontUpdater()
{

}

void PMJMesh::FinalFrontUpdater::splitMeshShared()
{
	Data::FrontElementList fels = this->front->getAllElements();

	Data::VertexList vertices = this->front->getVertices();
	Data::VertexSet allVLayers[this->smoothingLayer + 2];
	Data::FrontElementSet allFELayers[this->smoothingLayer + 1];
	Data::ElementSet allELayers[this->smoothingLayer + 1];

	allVLayers[0].insert(vertices.begin(), vertices.end());

	for (UInt i = 0; i < this->smoothingLayer + 1; i++)
	{
		for (Parallel::TaskVector::reverse_iterator iter = this->tasks.rbegin();
			 iter != this->tasks.rend(); iter++)
		{
			PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>((*iter));

			MJMesh::Mesh *mesh = t->getMesh();

			mesh->findLayer(vertices, 1, allVLayers[i + 1]);

			Data::VertexSet vs;
			Data::FrontElementSet fels;
			Data::ElementSet els;

			mesh->findAndRemoveLayers(vertices, 0, vs, fels, els);

			allFELayers[i].insert(fels.begin(), fels.end());
			allELayers[i].insert(els.begin(), els.end());
		}

		vertices.clear();

		vertices.insert(vertices.end(), allVLayers[i + 1].begin(), allVLayers[i + 1].end());
	}

	Data::FrontElementSet lastLayerFels;

	for (Parallel::TaskVector::reverse_iterator iter = this->tasks.rbegin();
		 iter != this->tasks.rend(); iter++)
	{
		PMJMesh::MJMeshTask *t = static_cast<PMJMesh::MJMeshTask *>((*iter));

		MJMesh::Mesh *mesh = t->getMesh();

		Data::VertexSet layer;

		mesh->findLayer(vertices, 0, layer, lastLayerFels);
	}

	Data::VertexSet boundaryVertices;
	Data::FrontElementSet boundaryFels;

	for (Data::FrontElementSet::iterator iter = lastLayerFels.begin();
		 iter != lastLayerFels.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		Data::ElementList adjs = this->mesh->adjacency(fel);

		this->adjustElements(adjs, allELayers[this->smoothingLayer]);

		if (!adjs.empty())
		{
			boundaryFels.insert(fel);

			for (UInt i = 0; i < fel->numVertices(); i++)
			{
				boundaryVertices.insert(fel->getVertex(i));
			}
		}
	}

	for (UInt i = 0; i < this->smoothingLayer + 1; i++)
	{
		for (Data::VertexSet::iterator iter = allVLayers[i].begin();
			 iter != allVLayers[i].end(); iter++)
		{
			this->mesh->add((*iter));
		}

		for (Data::FrontElementSet::iterator iter = allFELayers[i].begin();
			 iter != allFELayers[i].end(); iter++)
		{
			Data::FrontElement *fel = (*iter);

			this->mesh->add(fel);

			for (UInt i = 0; i < fel->numVertices(); i++)
			{
				static_cast<MJMesh::Vertex *>(fel->getVertex(i))->add(fel);
			}
		}

		for (Data::ElementSet::iterator iter = allELayers[i].begin();
			 iter != allELayers[i].end(); iter++)
		{
			Data::Element *e = (*iter);

			this->mesh->add(e);

			for (UInt i = 0; i < e->numVertices(); i++)
			{
				static_cast<MJMesh::Vertex *>(e->getVertex(i))->add(e);
			}
		}
	}

	this->buildBoundaryMaps();

	for (Data::VertexSet::iterator iter = boundaryVertices.begin();
		 iter != boundaryVertices.end(); iter++)
	{
		Data::Vertex *v = (*iter);

		if (!this->boundary->have(v))
		{
			this->boundary->add(v);

			this->mesh->add(v, false);
		}
	}

	for (Data::FrontElementSet::iterator iter = boundaryFels.begin();
		 iter != boundaryFels.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		if (!this->boundary->have(fel))
		{
			this->boundary->add(fel);

			this->mesh->add(fel);
		}
	}

	this->clearBoundaryMaps();
}

void PMJMesh::FinalFrontUpdater::finalizeMeshShared()
{
	this->restoreBoundary();

	for (UInt i = 0; i < this->tasks.size(); i++)
	{
		this->task = static_cast<MJMeshTask *>(this->tasks[i]);

		PMJMesh::FullFrontUpdater::updateMeshShared();

		delete this->task;

		this->task = NULL;
		this->tasks[i] = NULL;
	}

	this->tasks.clear();
}
#endif //#if (!USE_NEW_FRONT_UPDATER)
