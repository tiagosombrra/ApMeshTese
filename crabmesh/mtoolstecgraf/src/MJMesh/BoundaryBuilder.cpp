#include "MJMesh/BoundaryBuilder.h"

#include "MJMesh/Boundary.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/FrontElementSurface.h"
#include "Performer/SequentialIdManager.h"
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Triangle3D.h"

MJMesh::BoundaryBuilder::BoundaryBuilder(bool sorted) :
	Performer::BoundaryBuilder()
{
	this->setSorted(sorted);

	this->boundary = new MJMesh::Boundary(this->sorted);

	this->dimension = 0;

	this->idManager = NULL;

	this->minElementSize = REAL_MAX;
}

MJMesh::BoundaryBuilder::~BoundaryBuilder()
{
	if (this->idManager)
	{
		delete this->idManager;
	}
}

void MJMesh::BoundaryBuilder::setSorted(bool sorted)
{
	this->sorted = sorted;
}

Performer::IdManager *MJMesh::BoundaryBuilder::getIdManager() const
{
	return this->idManager;
}

MJMesh::Vertex *MJMesh::BoundaryBuilder::add(ULInt id, Real x, Real y)
{
	if (this->dimension < 2)
	{
		this->dimension = 2;
	}

	if (!this->idManager)
	{
		this->idManager = new Performer::SequentialIdManager(1, 2);
	}

	if (id > this->idManager->getId(0))
	{
		this->idManager->setId(0, id);
	}

	/*Data::Point2D *p = new Data::Point2D(x, y);
	MJMesh::Vertex *v = new MJMesh::Vertex(p, id);*/
	MJMesh::Vertex *v = new MJMesh::Vertex(x, y, id);

	this->boundary->add(v);

	this->verticesMap[v->getId()] = v;

	return v;
}

MJMesh::FrontElement2D *MJMesh::BoundaryBuilder::add(ULInt id, ULInt id1, ULInt id2)
{
	if (!this->idManager)
	{
		this->idManager = new Performer::SequentialIdManager(1, 2);
	}

	if (id > this->idManager->getId(1))
	{
		this->idManager->setId(1, id);
	}

	MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(this->verticesMap[id1]);
	MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(this->verticesMap[id2]);

	//Edge2D *edge = new MJMesh::Edge2D(v1, v2, id);
	//FrontElement2D *e = new MJMesh::FrontElement2D(edge);
	MJMesh::FrontElement2D *e = new MJMesh::FrontElement2D(v1, v2, id);

	Real s = e->getVertex(0)->getPoint()->distance(e->getVertex(1)->getPoint());

	if (s < this->minElementSize)
	{
		this->minElementSize = s;
	}

	this->boundary->add(e);

	return e;
}

MJMesh::FrontElementSurface *MJMesh::BoundaryBuilder::addSurface(ULInt id, ULInt id1, ULInt id2)
{
	if (!this->idManager)
	{
		this->idManager = new Performer::SequentialIdManager(1, 2);
	}

	if (id > this->idManager->getId(1))
	{
		this->idManager->setId(1, id);
	}

	MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(this->verticesMap[id1]);
	MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(this->verticesMap[id2]);

	//Edge3D *edge = new MJMesh::Edge3D(v1, v2, id);
	//FrontElementSurface *e = new MJMesh::FrontElementSurface(edge);
	MJMesh::FrontElementSurface *e = new MJMesh::FrontElementSurface(v1, v2, id);

	Real s = e->getVertex(0)->getPoint()->distance(e->getVertex(1)->getPoint());

	if (s < this->minElementSize)
	{
		this->minElementSize = s;
	}

	this->boundary->add(e);

	return e;
}

MJMesh::Vertex *MJMesh::BoundaryBuilder::add(ULInt id, Real x, Real y, Real z)
{
	if (this->dimension < 3)
	{
		this->dimension = 3;
	}

	if (!this->idManager)
	{
		this->idManager = new Performer::SequentialIdManager(1, 2);
	}

	if (id > this->idManager->getId(0))
	{
		this->idManager->setId(0, id);
	}

	/*Data::Point3D *p = new Data::Point3D(x, y, z);
	MJMesh::Vertex *v = new MJMesh::Vertex(p, id);*/
	MJMesh::Vertex *v = new MJMesh::Vertex(x, y, z, id);

	this->boundary->add(v);

	this->verticesMap[v->getId()] = v;

	return v;
}

MJMesh::FrontElement3D *MJMesh::BoundaryBuilder::add(ULInt id, ULInt id1, ULInt id2, ULInt id3)
{
	if (!this->idManager)
	{
		this->idManager = new Performer::SequentialIdManager(1, 2);
	}

	if (id > this->idManager->getId(1))
	{
		this->idManager->setId(1, id);
	}

	MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(this->verticesMap[id1]);
	MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(this->verticesMap[id2]);
	MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(this->verticesMap[id3]);

	//Data::Triangle3D *t = new Data::Triangle3D((Point3D *)v1->getPoint(), (Point3D *)v2->getPoint(), (Point3D *)v3->getPoint(), id);
	//FrontElement3D *e = new MJMesh::FrontElement3D(t);
	MJMesh::FrontElement3D *e = new MJMesh::FrontElement3D(v1, v2, v3, id);

	for (UInt i = 0; i < e->numVertices() - 1; i++)
	{
		for (UInt j = i + 1; j < e->numVertices(); j++)
		{
			Real s = e->getVertex(i)->getPoint()->distance(e->getVertex(j)->getPoint());

			if (s < this->minElementSize)
			{
				this->minElementSize = s;
			}
		}
	}

	this->boundary->add(e);

	return e;
}

#if USE_QUADS
MJMesh::FrontElement3D *MJMesh::BoundaryBuilder::add(ULInt id, ULInt id1, ULInt id2, ULInt id3, ULInt id4)
{
	if (!this->idManager)
	{
		this->idManager = new Performer::SequentialIdManager(1, 2);
	}

	if (id > this->idManager->getId(1))
	{
		this->idManager->setId(1, id);
	}

	MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>(this->verticesMap[id1]);
	MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(this->verticesMap[id2]);
	MJMesh::Vertex *v3 = static_cast<MJMesh::Vertex *>(this->verticesMap[id3]);
	MJMesh::Vertex *v4 = static_cast<MJMesh::Vertex *>(this->verticesMap[id4]);

	//Data::Triangle3D *t = new Data::Triangle3D((Point3D *)v1->getPoint(), (Point3D *)v2->getPoint(), (Point3D *)v3->getPoint(), id);
	//FrontElement3D *e = new MJMesh::FrontElement3D(t);
	MJMesh::FrontElement3D *e = new MJMesh::FrontElement3D(v1, v2, v3, v4, id);

	for (UInt i = 0; i < e->numVertices() - 1; i++)
	{
		for (UInt j = i + 1; j < e->numVertices(); j++)
		{
			Real s = e->getVertex(i)->getPoint()->distance(e->getVertex(j)->getPoint());

			if (s < this->minElementSize)
			{
				this->minElementSize = s;
			}
		}
	}

	this->boundary->add(e);

	return e;
}
#endif //#if USE_QUADS

void MJMesh::BoundaryBuilder::incrementIds(bool verts, bool els, ULInt inc)
{
	if ((verts) || (els))
	{
		//Data::VertexVector vertices = this->boundary->getVertices();
		//Data::VertexList vertices = this->boundary->getVertices();
		//Data::FrontElementVector elements = this->boundary->getElements();
		//Data::FrontElementList elements = this->boundary->getElements();

		if (verts)
		{
			//for (Data::VertexVector::iterator iter = vertices.begin();
			//for (Data::VertexList::iterator iter = vertices.begin();
			for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
				 //iter != vertices.end(); iter++)
				 iter != this->boundary->vEnd(); iter++)
			{
				//if (verts)
				{
					(*iter)->setId((*iter)->getId() + inc);
				}

				//this->boundary->add((*iter));
			}
		}

		if (els)
		{
			//for (Data::FrontElementVector::iterator iter = elements.begin();
			//for (Data::FrontElementList::iterator iter = elements.begin();
			for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
				 //iter != elements.end(); iter++)
				 iter != this->boundary->eEnd(); iter++)
			{
				//if (els)
				{
					(*iter)->setId((*iter)->getId() + inc);
				}

				//this->boundary->add((*iter));
			}
		}

		if (verts)
		{
			this->idManager->setId(0, this->idManager->getId(0) + inc);
		}

		if (els)
		{
			this->idManager->setId(1, this->idManager->getId(1) + inc);
		}
	}
}

void MJMesh::BoundaryBuilder::clear()
{
	this->setSorted(sorted);

	if (this->boundary)
	{
		delete this->boundary;
	}

	this->boundary = new MJMesh::Boundary(this->sorted);

	this->dimension = 0;

	if (this->idManager)
	{
		delete this->idManager;
	}

	this->idManager = NULL;

	this->verticesMap.clear();

	this->minElementSize = REAL_MAX;
}

void MJMesh::BoundaryBuilder::clearElements()
{
	Data::FrontElementList fels = this->boundary->getElements();

	this->boundary->setElements(Data::FrontElementList());

	while (!fels.empty())
	{
		delete fels.front();

		fels.pop_front();
	}

	//this->boundary->setElements(fels);
}

void MJMesh::BoundaryBuilder::execute()
{
	this->boundary->setSorted(this->sorted);

	//if ((this->minElementSize > 0.0) && (this->minElementSize < 1.0))
	{
		Real tol = (this->dimension == 2) ? 1.0 : 1e-8;
		//Data::setTolerance(this->minElementSize*Data::getTolerance()*1e-8);
		//Data::setTolerance(this->minElementSize*Data::getTolerance());
		Data::setTolerance(this->minElementSize*Data::getTolerance()*tol);
	}

	if (this->sorted)
	{
		this->boundary->sort();
	}
}
