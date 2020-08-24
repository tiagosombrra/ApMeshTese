#include "PMJMesh2/AuxiliaryFrontGenerator.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Mesh.h"
#include "PMJMesh/Front.h"
#include "Performer/IdManager.h"

using namespace PMJMesh2;

PMJMesh2::AuxiliaryFrontGenerator::AuxiliaryFrontGenerator() :
    Performer::Builder::Builder()
{
    this->dimension = 0;
    this->rangeProportion = 0.0;
    this->createCrackInInnerFront = false;
    this->displaceVerticesInInnerFront = false;
    this->front = NULL;
    this->box = NULL;
    this->tree = NULL;
    this->idManager = NULL;

#if USE_DELETE_FELS
    this->oldFront = NULL;
#endif //#if USE_DELETE_FELS
}

PMJMesh2::AuxiliaryFrontGenerator::~AuxiliaryFrontGenerator()
{
    if (this->createdInnerFront())
    {
        this->deleteInnerFront();
    }
}

void PMJMesh2::AuxiliaryFrontGenerator::setDimension(UInt dimension)
{
    this->dimension = dimension;
}

void PMJMesh2::AuxiliaryFrontGenerator::setRangeProportion(Real rangeProportion)
{
    this->rangeProportion = rangeProportion;
}

void PMJMesh2::AuxiliaryFrontGenerator::setCreateCrackInInnerFront(bool createCrackInInnerFront)
{
    this->createCrackInInnerFront = createCrackInInnerFront;
}

void PMJMesh2::AuxiliaryFrontGenerator::setDisplaceVerticesInInnerFront(bool displaceVerticesInInnerFront)
{
    this->displaceVerticesInInnerFront = displaceVerticesInInnerFront;
}

void PMJMesh2::AuxiliaryFrontGenerator::setFront(PMJMesh::Front *front)
{
    this->front = front;
}

#if USE_DELETE_FELS
void PMJMesh2::AuxiliaryFrontGenerator::setOldFront(PMJMesh::Front *oldFront)
{
    this->oldFront = oldFront;
}
#endif //#if USE_DELETE_FELS

void PMJMesh2::AuxiliaryFrontGenerator::setBox(const Data::Box *box)
{
    this->box = box;
}

void PMJMesh2::AuxiliaryFrontGenerator::setTree(const Data::BoxTree *tree)
{
    this->tree = tree;
}

void PMJMesh2::AuxiliaryFrontGenerator::setIdManager(Performer::IdManager *idManager)
{
    this->idManager = idManager;
}

bool PMJMesh2::AuxiliaryFrontGenerator::createdInnerFront() const
{
    return !this->createdInnerFels.empty();
}

void PMJMesh2::AuxiliaryFrontGenerator::execute()
{
    if (this->createdInnerFront())
    {
        this->deleteInnerFront();

        return;
    }

    if (((this->dimension != 2) && (this->dimension != 3)) ||
        (this->rangeProportion <= 0.0) ||
        (!this->front) ||
        (!this->box) ||
        (!this->tree) ||
        (!this->idManager))
    {
        return;
    }

    this->createInnerFront();
}

void PMJMesh2::AuxiliaryFrontGenerator::createInnerFront()
{
    if (this->front->size() != 0)
    {
        return;
    }

#if USE_EXPERIMENTAL_3
    Data::Point2D mid2d;
    Data::Point3D mid3d;

    Data::Point &mid = (this->dimension == 2) ?
        (mid2d = this->box->mid2D(), static_cast<Data::Point &>(mid2d)) :
        (mid3d = this->box->mid3D(), static_cast<Data::Point &>(mid3d));
#else
    Data::Point mid = this->box->mid();
#endif //#if USE_EXPERIMENTAL_3

	Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->tree->find(mid));

	//UInt level = cell->level();

	Real size = cell->size();

	Data::VertexList newVertices;
	Data::FrontElementList newFels;

	bool ok = true;

	if (this->dimension == 2)
	{
		if (this->createCrackInInnerFront)
		{
			Data::Point2D *p1 = new Data::Point2D(mid.getCoord(0) - size, mid.getCoord(1));
			Data::Point2D *p2 = new Data::Point2D(mid.getCoord(0), mid.getCoord(1));
			Data::Point2D *p3 = new Data::Point2D(mid.getCoord(0) + size, mid.getCoord(1));
			Data::Point2D *p4 = new Data::Point2D(mid.getCoord(0), mid.getCoord(1));

			if (this->displaceVerticesInInnerFront)
			{
				Real displace = 0.125*size;

				p2->setCoord(1, p2->getCoord(1) + displace);
				p4->setCoord(1, p4->getCoord(1) - displace);
			}

			MJMesh::Vertex *v1 = this->makeVertex(p1);
			MJMesh::Vertex *v2 = this->makeVertex(p2);
			MJMesh::Vertex *v3 = this->makeVertex(p3);
			MJMesh::Vertex *v4 = this->makeVertex(p4);

			MJMesh::FrontElement *f1 = this->makeFrontElement(v1, v2);
			MJMesh::FrontElement *f2 = this->makeFrontElement(v2, v3);
			MJMesh::FrontElement *f3 = this->makeFrontElement(v3, v4);
			MJMesh::FrontElement *f4 = this->makeFrontElement(v4, v1);

			newVertices.push_back(v1);
			newVertices.push_back(v2);
			newVertices.push_back(v3);
			newVertices.push_back(v4);

			newFels.push_back(f1);
			newFels.push_back(f2);
			newFels.push_back(f3);
			newFels.push_back(f4);

			ok = this->checkInnerFront(newVertices, newFels, size);

			if (ok)
			{
				this->createdInnerVertices[v1->getId()].original = v1;
				this->createdInnerVertices[v1->getId()].temporaries = NULL;

				this->createdInnerVertices[v2->getId()].original = v2;
				this->createdInnerVertices[v2->getId()].temporaries = new Data::VertexList();
				this->createdInnerVertices[v2->getId()].temporaries->push_back(v4);

				this->createdInnerVertices[v3->getId()].original = v3;
				this->createdInnerVertices[v3->getId()].temporaries = NULL;

				this->createdInnerFels[f1->getId()].original = f1;
				this->createdInnerFels[f1->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f1->getId()].temporaries->push_back(f4);

				this->createdInnerFels[f2->getId()].original = f2;
				this->createdInnerFels[f2->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f2->getId()].temporaries->push_back(f3);
			}
		}
		else
		{
			Real disp = size*0.5;

			Data::Point2D *p1 = new Data::Point2D(mid.getCoord(0) - disp, mid.getCoord(1));
			Data::Point2D *p2 = new Data::Point2D(mid.getCoord(0) + disp, mid.getCoord(1));

			MJMesh::Vertex *v1 = this->makeVertex(p1);
			MJMesh::Vertex *v2 = this->makeVertex(p2);

			MJMesh::FrontElement *f1 = this->makeFrontElement(v1, v2);
			MJMesh::FrontElement *f2 = this->makeFrontElement(v2, v1);

			newVertices.push_back(v1);
			newVertices.push_back(v2);

			newFels.push_back(f1);
			newFels.push_back(f2);

			ok = this->checkInnerFront(newVertices, newFels, size);

			if (ok)
			{
				this->createdInnerVertices[v1->getId()].original = v1;
				this->createdInnerVertices[v1->getId()].temporaries = NULL;

				this->createdInnerVertices[v2->getId()].original = v2;
				this->createdInnerVertices[v2->getId()].temporaries = NULL;

				this->createdInnerFels[f1->getId()].original = f1;
				this->createdInnerFels[f1->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f1->getId()].temporaries->push_back(f2);
			}
		}
	}
	else if (this->dimension == 3)
	{
		if (this->createCrackInInnerFront)
		{
			Data::Point3D *p1 = new Data::Point3D(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2));
			Data::Point3D *p2 = new Data::Point3D(mid.getCoord(0) - size, mid.getCoord(1), mid.getCoord(2));
			Data::Point3D *p3 = new Data::Point3D(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2) + size);
			Data::Point3D *p4 = new Data::Point3D(mid.getCoord(0) + size, mid.getCoord(1), mid.getCoord(2));
			Data::Point3D *p5 = new Data::Point3D(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2) - size);
			Data::Point3D *p6 = new Data::Point3D(mid.getCoord(0), mid.getCoord(1), mid.getCoord(2));

			if (this->displaceVerticesInInnerFront)
			{
				Real displace = 0.125*size;

				p1->setCoord(1, p1->getCoord(1) + displace);
				p6->setCoord(1, p6->getCoord(1) - displace);
			}

			MJMesh::Vertex *v1 = this->makeVertex(p1);
			MJMesh::Vertex *v2 = this->makeVertex(p2);
			MJMesh::Vertex *v3 = this->makeVertex(p3);
			MJMesh::Vertex *v4 = this->makeVertex(p4);
			MJMesh::Vertex *v5 = this->makeVertex(p5);
			MJMesh::Vertex *v6 = this->makeVertex(p6);

			MJMesh::FrontElement *f1 = this->makeFrontElement(v1, v2, v3);
			MJMesh::FrontElement *f2 = this->makeFrontElement(v1, v3, v4);
			MJMesh::FrontElement *f3 = this->makeFrontElement(v1, v4, v5);
			MJMesh::FrontElement *f4 = this->makeFrontElement(v1, v5, v2);
			MJMesh::FrontElement *f5 = this->makeFrontElement(v6, v3, v2);
			MJMesh::FrontElement *f6 = this->makeFrontElement(v6, v4, v3);
			MJMesh::FrontElement *f7 = this->makeFrontElement(v6, v5, v4);
			MJMesh::FrontElement *f8 = this->makeFrontElement(v6, v2, v5);

			newVertices.push_back(v1);
			newVertices.push_back(v2);
			newVertices.push_back(v3);
			newVertices.push_back(v4);
			newVertices.push_back(v5);
			newVertices.push_back(v6);

			newFels.push_back(f1);
			newFels.push_back(f2);
			newFels.push_back(f3);
			newFels.push_back(f4);
			newFels.push_back(f5);
			newFels.push_back(f6);
			newFels.push_back(f7);
			newFels.push_back(f8);

			ok = this->checkInnerFront(newVertices, newFels, size);

			if (ok)
			{
				this->createdInnerVertices[v1->getId()].original = v1;
				this->createdInnerVertices[v1->getId()].temporaries = new Data::VertexList();
				this->createdInnerVertices[v1->getId()].temporaries->push_back(v6);

				this->createdInnerVertices[v2->getId()].original = v2;
				this->createdInnerVertices[v2->getId()].temporaries = NULL;

				this->createdInnerVertices[v3->getId()].original = v3;
				this->createdInnerVertices[v3->getId()].temporaries = NULL;

				this->createdInnerVertices[v4->getId()].original = v4;
				this->createdInnerVertices[v4->getId()].temporaries = NULL;

				this->createdInnerVertices[v5->getId()].original = v5;
				this->createdInnerVertices[v5->getId()].temporaries = NULL;

				this->createdInnerFels[f1->getId()].original = f1;
				this->createdInnerFels[f1->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f1->getId()].temporaries->push_back(f5);

				this->createdInnerFels[f2->getId()].original = f2;
				this->createdInnerFels[f2->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f2->getId()].temporaries->push_back(f6);

				this->createdInnerFels[f3->getId()].original = f3;
				this->createdInnerFels[f3->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f3->getId()].temporaries->push_back(f7);

				this->createdInnerFels[f4->getId()].original = f4;
				this->createdInnerFels[f4->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f4->getId()].temporaries->push_back(f8);
			}
		}
		else
		{
			Real disp = size*0.5;

			Data::Point3D *p1 = new Data::Point3D(mid.getCoord(0) - disp*0.5, mid.getCoord(1), mid.getCoord(2) + disp*0.5);
			Data::Point3D *p2 = new Data::Point3D(mid.getCoord(0) + disp,     mid.getCoord(1), mid.getCoord(2) + disp*0.5);
			Data::Point3D *p3 = new Data::Point3D(mid.getCoord(0) - disp*0.5, mid.getCoord(1), mid.getCoord(2) - disp);

			MJMesh::Vertex *v1 = this->makeVertex(p1);
			MJMesh::Vertex *v2 = this->makeVertex(p2);
			MJMesh::Vertex *v3 = this->makeVertex(p3);

			MJMesh::FrontElement *f1 = this->makeFrontElement(v1, v2, v3);
			MJMesh::FrontElement *f2 = this->makeFrontElement(v1, v3, v2);

			newVertices.push_back(v1);
			newVertices.push_back(v2);
			newVertices.push_back(v3);

			newFels.push_back(f1);
			newFels.push_back(f2);

			ok = this->checkInnerFront(newVertices, newFels, size);

			if (ok)
			{
				this->createdInnerVertices[v1->getId()].original = v1;
				this->createdInnerVertices[v1->getId()].temporaries = NULL;

				this->createdInnerVertices[v2->getId()].original = v2;
				this->createdInnerVertices[v2->getId()].temporaries = NULL;

				this->createdInnerVertices[v3->getId()].original = v3;
				this->createdInnerVertices[v3->getId()].temporaries = NULL;

				this->createdInnerFels[f1->getId()].original = f1;
				this->createdInnerFels[f1->getId()].temporaries = new Data::FrontElementList();
				this->createdInnerFels[f1->getId()].temporaries->push_back(f2);
			}
		}
	}

	if (ok)
    {
        for (Data::FrontElementList::iterator iter = newFels.begin();
             iter != newFels.end(); iter++)
        {
            this->front->addBoundary((*iter));

#if USE_DELETE_FELS
            this->oldFront->addBoundary((*iter));
#endif //#if USE_DELETE_FELS
        }

        for (Data::VertexList::iterator iter = newVertices.begin();
             iter != newVertices.end(); iter++)
        {
            this->front->add((*iter));
        }
    }
    else
    {
        for (Data::FrontElementList::iterator iter = newFels.begin();
             iter != newFels.end(); iter++)
        {
            delete (*iter);
        }

        for (Data::VertexList::iterator iter = newVertices.begin();
             iter != newVertices.end(); iter++)
        {
            delete (*iter);
        }
    }
}

void PMJMesh2::AuxiliaryFrontGenerator::deleteInnerFront()
{
#if USE_C__11
	for (PMJMesh::CrossingReplacer::FrontElementReplacementHashMap::iterator iter = this->createdInnerFels.begin();
#else
	for (PMJMesh::CrossingReplacer::FrontElementReplacementMap::iterator iter = this->createdInnerFels.begin();
#endif //#if USE_C__11
		 iter != this->createdInnerFels.end(); iter++)
	{
		MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter).second.original);

		while (((*iter).second.temporaries) && (!(*iter).second.temporaries->empty()))
		{
			MJMesh::FrontElement *tmp = static_cast<MJMesh::FrontElement *>((*iter).second.temporaries->front());

			(*iter).second.temporaries->pop_front();

			//Data::ElementList adjs = tmp->getAdjacency();

			//while (!adjs.empty())
			for (Data::ElementList::iterator iter = tmp->eBegin();
                 iter != tmp->eEnd(); iter = tmp->erase(iter))
			{
				//MJMesh::Element *adj = static_cast<MJMesh::Element *>(adjs.front());
				MJMesh::Element *adj = static_cast<MJMesh::Element *>((*iter));

				//adjs.pop_front();

				//tmp->remove(adj);

				fel->add(adj);
			}

			while (!tmp->getData().boundaries.empty())
			{
				MJMesh::Boundary *boundary = static_cast<MJMesh::Boundary *>(const_cast<MJMesh::Boundary *>((*tmp->getData().boundaries.begin()).first));

				boundary->remove(tmp);
			}

			while (!tmp->getData().fronts.empty())
			{
				PMJMesh::Front *front = static_cast<PMJMesh::Front *>(const_cast<MJMesh::Front *>((*tmp->getData().fronts.begin()).first));

				front->remove(tmp);
			}

			while (!tmp->getData().meshes.empty())
			{
				MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>((*tmp->getData().meshes.begin()).first);

				mesh->remove(tmp, false);
			}

			delete tmp;
		}

		if ((*iter).second.temporaries)
		{
			delete (*iter).second.temporaries;
		}

		while (!fel->getData().boundaries.empty())
        {
            MJMesh::Boundary *boundary = static_cast<MJMesh::Boundary *>(const_cast<MJMesh::Boundary *>((*fel->getData().boundaries.begin()).first));

            boundary->remove(fel);
        }

		if ((this->front) && (this->front->have(fel)))
		{
			this->front->remove(fel);
		}

#if USE_DELETE_FELS
        if ((this->oldFront) && (this->oldFront->have(fel)))
        {
            this->oldFront->remove(fel);
        }

		//if (this->mesh)
		{
			while (!fel->getData().meshes.empty())
			{
				MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>((*fel->getData().meshes.begin()).first);

				mesh->remove(fel, false);
			}

			delete fel;
		}
#endif //#if USE_DELETE_FELS
	}

	this->createdInnerFels.clear();

#if USE_C__11
	for (PMJMesh::CrossingReplacer::VertexReplacementHashMap::iterator iter = this->createdInnerVertices.begin();
#else
	for (PMJMesh::CrossingReplacer::VertexReplacementMap::iterator iter = this->createdInnerVertices.begin();
#endif //#if USE_C__11
		 iter != this->createdInnerVertices.end(); iter++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).second.original);

		Real coord = 0.0;
		UInt displace = 0;

		while (((*iter).second.temporaries) && (!(*iter).second.temporaries->empty()))
		{
			MJMesh::Vertex *tmp = static_cast<MJMesh::Vertex *>((*iter).second.temporaries->front());

			(*iter).second.temporaries->pop_front();

			//Data::FrontElementList fadjs = tmp->getAdjacentFrontElements();

			//while (!fadjs.empty())
			for (Data::FrontElementList::iterator iter = tmp->feBegin();
                 iter != tmp->feEnd(); iter = tmp->erase(iter))
			{
				//Data::FrontElement *adj = fadjs.front();
				Data::FrontElement *adj = (*iter);

				//fadjs.pop_front();

				adj->replace(tmp, v);

				//tmp->remove(adj);

				v->add(adj);
			}

			//Data::ElementList adjs = tmp->getAdjacentElements();

			//while (!adjs.empty())
			for (Data::ElementList::iterator iter = tmp->eBegin();
                 iter != tmp->eEnd(); iter = tmp->erase(iter))
			{
				//Data::Element *adj = adjs.front();
				Data::Element *adj = (*iter);

				//adjs.pop_front();

				adj->replace(tmp, v);

				//tmp->remove(adj);

				v->add(adj);
			}

			while (!tmp->getData().boundaries.empty())
			{
				MJMesh::Boundary *boundary = static_cast<MJMesh::Boundary *>(const_cast<MJMesh::Boundary *>((*tmp->getData().boundaries.begin()).first));

				boundary->remove(tmp);
			}

			while (!tmp->getData().fronts.empty())
			{
				PMJMesh::Front *front = static_cast<PMJMesh::Front *>(const_cast<MJMesh::Front *>((*tmp->getData().fronts.begin()).first));

				front->remove(tmp);
			}

			while (!tmp->getData().meshes.empty())
			{
				MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>((*tmp->getData().meshes.begin()).first);

				mesh->remove(tmp);
			}

			if (this->displaceVerticesInInnerFront)
			{
				coord += tmp->getPoint()->getCoord(1);

				displace++;
			}

			delete tmp;
		}

		if ((*iter).second.temporaries)
		{
			delete (*iter).second.temporaries;
		}

		while (!v->getData().boundaries.empty())
        {
            MJMesh::Boundary *boundary = static_cast<MJMesh::Boundary *>(const_cast<MJMesh::Boundary *>((*v->getData().boundaries.begin()).first));

            boundary->remove(v);
        }

		if ((this->front) && (this->front->have(v)))
		{
			this->front->remove(v);
		}

		for (MJMesh::VertexSpecificData::MeshDataMap::const_iterator iter = v->getData().meshes.begin();
             iter != v->getData().meshes.end(); iter++)
        {
            MJMesh::Mesh *mesh = static_cast<MJMesh::Mesh *>(const_cast<MJMesh::Mesh *>((*iter).first));

            if (!mesh->haveInner(v))
            {
                mesh->addInner(v);
            }
        }

		if ((this->displaceVerticesInInnerFront) && (displace > 0))
		{
			v->getPoint()->setCoord(1, (v->getPoint()->getCoord(1) + coord)/static_cast<Real>(displace + 1));
		}
	}

	this->createdInnerVertices.clear();
}

bool PMJMesh2::AuxiliaryFrontGenerator::checkInnerFront(const Data::VertexList &vertices,
    const Data::FrontElementList &fels, Real size) const
{
    for (Data::VertexList::const_iterator iter = vertices.begin();
		 iter != vertices.end(); iter++)
	{
		Data::Point *p = (*iter)->getPoint();

		if (this->box->optOut(p))
		{
			return false;
		}
	}

	Real dist = size*this->rangeProportion;

	for (Data::FrontElementList::const_iterator iter = fels.begin();
		 iter != fels.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

#if USE_EXPERIMENTAL_3
        struct TestBox
        {
            static bool test(const Data::Point &mid, Data::Vector &n, const Data::Box *box, const Real &dist, const Real &size)
            {
                n.multiply(size);

                n.sum(mid);

                UInt dimension = n.dimension();

                Data::Point2D min2d, max2d;
                Data::Point3D min3d, max3d;

                Data::Point &min = (dimension == 2) ? static_cast<Data::Point &>(min2d) : static_cast<Data::Point &>(min3d);
                Data::Point &max = (dimension == 2) ? static_cast<Data::Point &>(max2d) : static_cast<Data::Point &>(max3d);

                for (UInt i = 0; i < dimension; i++)
                {
                    min.setCoord(i, n.getCoord(i) - dist);
                    max.setCoord(i, n.getCoord(i) + dist);
                }

                /*if ((this->box->optOut(min)) || (this->box->optOut(max)))
                {
                    return false;
                }

                return true;*/

                return ((!box->optOut(min)) && (!box->optOut(max)));
            }
        };

        if (this->dimension == 2)
        {
            Data::Vector2D n = static_cast<Data::Edge2D *>(fel->getShape())->normal();

            if (!TestBox::test(fel->getShape()->mid2D(), n, this->box, dist, size))
            {
                return false;
            }
        }
        else
        {
            Data::Vector3D n = static_cast<Data::Triangle3D *>(fel->getShape())->normal();

            if (!TestBox::test(fel->getShape()->mid3D(), n, this->box, dist, size))
            {
                return false;
            }
        }
#else
		Data::Point mid = fel->getShape()->mid();

		Data::Vector v(mid);
		Data::Vector n(this->dimension);

		if (this->dimension == 2)
		{
			n = static_cast<Data::Edge2D *>(fel->getShape())->normal();
		}
		else
		{
			n = static_cast<Data::Triangle3D *>(fel->getShape())->normal();
		}

		n.multiply(size);

		n.sum(mid);

		Data::Point min(n), max(n);

		for (UInt i = 0; i < this->dimension; i++)
		{
			min.setCoord(i, min.getCoord(i) - dist);
			max.setCoord(i, max.getCoord(i) + dist);
		}

		if ((this->box->optOut(min)) || (this->box->optOut(max)))
		{
			return false;
		}
#endif //#if USE_EXPERIMENTAL_3
	}

    return true;
}

MJMesh::Vertex *PMJMesh2::AuxiliaryFrontGenerator::makeVertex(Data::Point *p) const
{
    return new MJMesh::Vertex(p, this->idManager->next(0));
}

MJMesh::FrontElement *PMJMesh2::AuxiliaryFrontGenerator::makeFrontElement(
    MJMesh::Vertex *v1, MJMesh::Vertex *v2) const
{
    return new MJMesh::FrontElement2D(v1, v2, this->idManager->next(1));
}

MJMesh::FrontElement *PMJMesh2::AuxiliaryFrontGenerator::makeFrontElement(
    MJMesh::Vertex *v1, MJMesh::Vertex *v2, MJMesh::Vertex *v3) const
{
    return new MJMesh::FrontElement3D(v1, v2, v3, this->idManager->next(1));
}
