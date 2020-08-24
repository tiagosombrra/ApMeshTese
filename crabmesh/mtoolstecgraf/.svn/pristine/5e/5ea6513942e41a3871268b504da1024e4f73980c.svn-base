#include "MJMesh/FrontAdvancer.h"

#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Triangle3D.h"
#include "Data/BoxSearchTree.h"
#include "Data/BoxTree.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Edge3D.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/Front.h"
#include "MJMesh/Mesh.h"
#include "MJMesh/VertexSearchTreeGenerator.h"
#include "MJMesh/FrontElementSearchTreeGenerator.h"

using namespace MJMesh;

MJMesh::FrontAdvancer::FrontAdvancer(UInt dimension, const Data::Boundary *boundary,
	const Data::BoxTree *tree, bool usingSearchTrees) :
	Performer::FrontAdvancer(boundary)
{
	this->setTree(tree);

	this->setUsingSearchTrees(usingSearchTrees);

	this->dimension = dimension;

	this->vstree = NULL;
	this->festree = NULL;
}

MJMesh::FrontAdvancer::~FrontAdvancer()
{
	//this->vstree = NULL;
	//this->festree = NULL;
}

void MJMesh::FrontAdvancer::setFront(MJMesh::Front *front)
{
	this->front = front;
}

Data::Front *MJMesh::FrontAdvancer::getFront() const
{
	return this->front;
}

void MJMesh::FrontAdvancer::setMesh(MJMesh::Mesh *mesh)
{
	this->mesh = mesh;
}

void MJMesh::FrontAdvancer::setTree(const Data::BoxTree *tree)
{
	this->tree = tree;
}

void MJMesh::FrontAdvancer::setUsingSearchTrees(bool usingSearchTrees)
{
	this->usingSearchTrees = usingSearchTrees;
}

void MJMesh::FrontAdvancer::setVSTree(Data::GeometricSearchTree *vstree)
{
	this->vstree = vstree;
}

void MJMesh::FrontAdvancer::setFESTree(Data::GeometricSearchTree *festree)
{
	this->festree = festree;
}

Data::Mesh *MJMesh::FrontAdvancer::makeMesh() const
{
	return new MJMesh::Mesh();
}

Data::Front *MJMesh::FrontAdvancer::makeFront() const
{
	MJMesh::Front *front = new MJMesh::Front(static_cast<const MJMesh::Boundary *>(this->boundary));

	if (this->usingSearchTrees)
	{
		//Real debugstart = Data::time();

		this->vstree = this->makeVertexSearchTree(front);

		this->festree = this->makeFrontElementSearchTree(front);

		//this->timeInitMakingTrees += Data::time() - debugstart;

		//std::cout << "this->timeInitMakingTrees = " << this->timeInitMakingTrees << std::endl;

		front->setVSTree(this->vstree);
		front->setFESTree(this->festree);
	}

	return front;
}

Data::Vertex *MJMesh::FrontAdvancer::makeVertex(Data::Point *p) const
{
	return new MJMesh::Vertex(p);
}

MJMesh::VertexSearchTreeGenerator *MJMesh::FrontAdvancer::makeVertexSearchTreeGenerator(const MJMesh::Front *front) const
{
	if (front)
	{
		return new MJMesh::VertexSearchTreeGenerator(front, this->tree);
	}

	return new MJMesh::VertexSearchTreeGenerator(static_cast<MJMesh::Front *>(this->front), this->tree);
}

Data::GeometricSearchTree *MJMesh::FrontAdvancer::makeVertexSearchTree(const MJMesh::Front *front) const
{
	MJMesh::VertexSearchTreeGenerator *vstg = this->makeVertexSearchTreeGenerator(front);

	vstg->execute();

	Data::GeometricSearchTree *tree = static_cast<Data::GeometricSearchTree *>(vstg->getTree());

	delete vstg;

	return tree;
}

MJMesh::FrontElementSearchTreeGenerator *MJMesh::FrontAdvancer::makeFrontElementSearchTreeGenerator(const MJMesh::Front *front) const
{
	if (front)
	{
		return new MJMesh::FrontElementSearchTreeGenerator(front, this->tree);
	}

	return new MJMesh::FrontElementSearchTreeGenerator(static_cast<MJMesh::Front *>(this->front), this->tree);
}

Data::GeometricSearchTree *MJMesh::FrontAdvancer::makeFrontElementSearchTree(const MJMesh::Front *front) const
{
	MJMesh::FrontElementSearchTreeGenerator *festg = this->makeFrontElementSearchTreeGenerator(front);

	festg->execute();

	Data::GeometricSearchTree *tree = static_cast<Data::GeometricSearchTree *>(festg->getTree());

	delete festg;

	return tree;
}

#if USE_ADT
void MJMesh::FrontAdvancer::advance(Data::FrontElement *e, Data::Vertex *v)
{
    Performer::FrontAdvancer::advance(e, v);

    if (((this->vstree) || (this->festree)) &&
        ((this->mesh) && ((this->mesh->size()%500) == 0)))
    {
        if (this->vstree)
        {
            this->vstree->rebuild();
        }

        if (this->festree)
        {
            this->festree->rebuild();
        }
    }
}
#endif //#if USE_ADT

Data::ElementTuple MJMesh::FrontAdvancer::makeElement(const Data::FrontElement *e, const Data::Point *p) const
{
	Data::FrontElementList *elements = new Data::FrontElementList();

	MJMesh::Vertex *v = NULL;

	if (this->dimension == 2)
	{
		v = static_cast<MJMesh::Vertex *>(this->makeVertex(const_cast<Data::Point *>(p)));

		const MJMesh::FrontElement2D *el = static_cast<const MJMesh::FrontElement2D *>(e);

		MJMesh::FrontElement2D *el1 = new MJMesh::FrontElement2D(el->getV1(), v);
		MJMesh::FrontElement2D *el2 = new MJMesh::FrontElement2D(v, el->getV2());

		elements->push_back(el1);
		elements->push_back(el2);

		MJMesh::Element2D *element = new MJMesh::Element2D(static_cast<MJMesh::Vertex *>(el->getV1()),
			static_cast<MJMesh::Vertex *>(el->getV2()), v);

#if USE_C__11
		return std::make_tuple(static_cast<Data::Vertex *>(v), elements, static_cast<Data::Element *>(element));
#else
		Data::ElementTuple t;

		t.v = v;
		t.fes = elements;
		t.e = element;

		return t;
#endif //#if USE_C__11
	}
	else if (this->dimension == 3)
	{
		v = static_cast<MJMesh::Vertex *>(this->makeVertex(const_cast<Data::Point *>(p)));

		const MJMesh::FrontElement3D *el = static_cast<const MJMesh::FrontElement3D *>(e);

		MJMesh::FrontElement3D *el1 = new MJMesh::FrontElement3D(el->getV1(), el->getV2(), v);
		MJMesh::FrontElement3D *el2 = new MJMesh::FrontElement3D(el->getV2(), el->getV3(), v);
		MJMesh::FrontElement3D *el3 = new MJMesh::FrontElement3D(el->getV3(), el->getV1(), v);

		elements->push_back(el1);
		elements->push_back(el2);
		elements->push_back(el3);

		MJMesh::Element3D *element = new MJMesh::Element3D(static_cast<MJMesh::Vertex *>(el->getV1()),
			static_cast<MJMesh::Vertex *>(el->getV2()), static_cast<MJMesh::Vertex *>(el->getV3()), v);

#if USE_C__11
		return std::make_tuple(static_cast<Data::Vertex *>(v), elements, static_cast<Data::Element *>(element));
#else
		Data::ElementTuple t;

		t.v = v;
		t.fes = elements;
		t.e = element;

		return t;
#endif //#if USE_C__11
	}

#if USE_C__11
	return std::make_tuple(static_cast<Data::Vertex *>(v), elements, static_cast<Data::Element *>(NULL));
#else
	Data::ElementTuple t;

	t.v = v;
	t.fes = elements;
	t.e = NULL;

	return t;
#endif //#if USE_C__11
}

/*Data::FrontElementList MJMesh::FrontAdvancer::makeFrontElements(
	const Data::FrontElement *element, Data::Point *p, Data::Vertex *&v) const
{
	Data::FrontElementList elements;

	v = this->makeVertex(p);

	if (this->dimension == 2)
	{
		MJMesh::FrontElement2D *el = (MJMesh::FrontElement2D *)element;

		MJMesh::FrontElement2D *el1 = new MJMesh::FrontElement2D(el->getV1(), (MJMesh::Vertex *)v);
		MJMesh::FrontElement2D *el2 = new MJMesh::FrontElement2D((MJMesh::Vertex *)v, el->getV2());

		((MJMesh::Vertex *)el->getV1())->remove((MJMesh::Edge2D *)el1->getShape());
		((MJMesh::Vertex *)v)->remove((MJMesh::Edge2D *)el1->getShape());

		((MJMesh::Vertex *)v)->remove((MJMesh::Edge2D *)el2->getShape());
		((MJMesh::Vertex *)el->getV2())->remove((MJMesh::Edge2D *)el2->getShape());

		elements.push_back(el1);
		elements.push_back(el2);
	}
	else if (this->dimension == 3)
	{
		MJMesh::FrontElement3D *el = (MJMesh::FrontElement3D *)element;

		MJMesh::FrontElement3D *el1 = new MJMesh::FrontElement3D(el->getV1(), el->getV2(), (MJMesh::Vertex *)v);
		MJMesh::FrontElement3D *el2 = new MJMesh::FrontElement3D(el->getV2(), el->getV3(), (MJMesh::Vertex *)v);
		MJMesh::FrontElement3D *el3 = new MJMesh::FrontElement3D(el->getV3(), el->getV1(), (MJMesh::Vertex *)v);

		elements.push_back(el1);
		elements.push_back(el2);
		elements.push_back(el3);
	}

	return elements;
}

Data::Element *MJMesh::FrontAdvancer::makeElement(const Data::FrontElement *element,
	const Data::FrontElementList &elements, const Vertex *v)
{
	if (this->dimension == 2)
	{
		MJMesh::Element2D *el = new MJMesh::Element2D(element->getVertex(0), element->getVertex(1), v);

		return el;
	}
	else if (this->dimension == 3)
	{
		MJMesh::Element3D *el = new MJMesh::Element3D(element->getVertex(0), element->getVertex(1), element->getVertex(2), v);

		return el;
	}

	return NULL;
}*/

bool MJMesh::FrontAdvancer::better(Real rank1, Real rank2) const
{
	return (rank1 > rank2);
}

bool MJMesh::FrontAdvancer::equal(Real rank1, Real rank2) const
{
	return (std::fabs(rank1 - rank2) < Data::getTolerance());
}

bool MJMesh::FrontAdvancer::accordingToNormal(const Data::FrontElement *element, const Data::FrontElement *adj) const
{
    for (UInt i = 0; i < element->numVertices(); i++)
    {
        if ((adj->have(element->getVertex(i))) ||
            //(adj->haveMatching(*element->getVertex(i)->getPoint())))
            (adj->have(*element->getVertex(i)->getPoint())))
        {
            continue;
        }

        if (!adj->accordingToNormal(element->getVertex(i)))
        {
            return false;
        }
    }

    return true;
}

bool MJMesh::FrontAdvancer::better(const Data::FrontElement *element, Data::Vertex *candidate,
	Data::Vertex */*old*/) const
{
	/*//if candidate and old do not have the same coordinates, than they are in
	// the same circuncircle, which means that the old vertex is kept
	//THIS DOES NOT HAPPEN, BECAUSE AN EQUAL TEST IS PERFORMED PRIOR TO ENTERING THIS METHOD
	if (!candidate->getPoint()->equal(*(old->getPoint())))
	{
		return false;
	}*/

	/*for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(candidate)->feBegin();
         iter != static_cast<MJMesh::Vertex *>(candidate)->feEnd(); iter++)
	{
		Data::FrontElement *e = (*iter);

		if (this->boundary->have(e))
		{
			if (!this->accordingToNormal(element, e))
            {
                return false;
            }
		}
	}

	return true;*/

	Data::FrontElementList els;

	for (Data::FrontElementList::iterator iter = static_cast<MJMesh::Vertex *>(candidate)->feBegin();
         iter != static_cast<MJMesh::Vertex *>(candidate)->feEnd(); iter++)
    {
        if (this->boundary->have((*iter), false))
        {
            els.push_back((*iter));
        }
    }

    if (els.empty())
    {
        return false;
    }

    if (els.size() == 1)
    {
        Data::FrontElement *adj = els.front();

        if (!this->accordingToNormal(element, adj))
        {
            return false;
        }

        return true;
    }

    if (els.size() == 2)
    {
        Data::FrontElement *adj1 = els.front();
        Data::FrontElement *adj2 = els.back();

        Data::Vertex *v2 = NULL;

        for (UInt i = 0; i < adj2->numVertices(); i++)
        {
            if (adj1->have(adj2->getVertex(i)))
            {
                continue;
            }

            v2 = adj2->getVertex(i);

            break;
        }

        if (!v2)
        {
            return false;
        }

        bool doAnd = adj1->accordingToNormal(v2, false);

        if (doAnd)
        {
            if (!this->accordingToNormal(element, adj1))
            {
                return false;
            }

            if (!this->accordingToNormal(element, adj2))
            {
                return false;
            }

            //equivalent to
            // this->accordingToNormal(element, adj1) && this->accordingToNormal(element, adj2)

            return true;
        }
        else
        {
            if (!this->accordingToNormal(element, adj1))
            {
                if (!this->accordingToNormal(element, adj2))
                {
                    return false;
                }
            }

            //equivalent to
            // this->accordingToNormal(element, adj1) || this->accordingToNormal(element, adj2)

            return true;
        }

        return true;
    }

    typedef std::list<Data::FrontElementList > FrontElementListList;

    FrontElementListList adjs;

    adjs.push_back(Data::FrontElementList());

    adjs.front().push_back(els.front());

    els.pop_front();

    UInt dimension = candidate->getPoint()->dimension();

    while (!els.empty())
    {
        for (Data::FrontElementList::iterator iter = els.begin();
             iter != els.end(); iter++)
        {
            Data::FrontElement *fel = (*iter);

            bool found = false;
            bool inserted = false;

            for (FrontElementListList::iterator it = adjs.begin();
                 it != adjs.end(); it++)
            {
                for (Data::FrontElementList::iterator iter2 = (*it).begin();
                     iter2 != (*it).end(); iter2++)
                {
                    Data::FrontElement *ofel = (*iter2);

                    UInt count = 0;
                    Data::Vertex *v2 = NULL;

                    for (UInt i = 0; i < ofel->numVertices(); i++)
                    {
                        if (fel->have(ofel->getVertex(i)))
                        {
                            count++;
                        }
                        else
                        {
                            v2 = ofel->getVertex(i);
                        }

                        if ((count == dimension - 1) && (v2))
                        {
                            break;
                        }
                    }

                    if ((count != dimension - 1) || (!v2))
                    {
                        continue;
                    }

                    found = true;

                    bool doAnd = fel->accordingToNormal(v2, false);

                    if (doAnd)
                    {
                        (*it).push_back(fel);

                        inserted = true;

                        break;
                    }
                }

                if (found)
                {
                    if (!inserted)
                    {
                        adjs.push_back(Data::FrontElementList());

                        adjs.back().push_back(fel);
                    }

                    break;
                }
            }

            if (found)
            {
                els.erase(iter);

                break;
            }
        }
    }

    while (!adjs.empty())
    {
        bool according = true;

        while (!adjs.front().empty())
        {
            Data::FrontElement *adj = adjs.front().front();

            adjs.front().pop_front();

            if (!this->accordingToNormal(element, adj))
            {
                according = false;

                break;
            }
        }

        if (according)
        {
            return true;
        }

        adjs.pop_front();
    }

	return false;
}

bool MJMesh::FrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point, Real radius) const
{
	if (!element->accordingToNormal(point, false))
	{
		return false;
	}

	if (radius > 0.0)
	{
		return (static_cast<const MJMesh::FrontElement *>(element)->distance(point) > radius*0.1);
	}

	return true;
}

/*bool MJMesh::FrontAdvancer::isValid(const Data::FrontElement *element, Data::Point *point) const
{
	if (!Performer::FrontAdvancer::isValid(element, point))
	{
		return false;
	}

	if (this->front->intercept(element, point))
	{
		return false;
	}

	return true;
}*/

bool MJMesh::FrontAdvancer::isValid(const Data::Point *point, const Data::FrontElement *e, const Data::FrontElementList &list) const
{
	//return !(this->front->intercept(e, point, list) || this->mesh->in(point));
	return !(this->front->intercept(e, point, list) || static_cast<MJMesh::Front *>(this->front)->interceptElements(point, list));
	//return !(static_cast<MJMesh::Front *>(this->front)->interceptElements(point, list));
}

bool MJMesh::FrontAdvancer::isValid(const Data::Point *point, const Data::FrontElement *e) const
{
	//cout << "debug isvalid, checking validity of point " << point->text() << " with respect to front and mesh" << endl;

	//return !(this->front->intercept(e, point) || this->mesh->in(point));
	return !(this->front->intercept(e, point) || static_cast<MJMesh::Front *>(this->front)->interceptElements(point));
	//return !(static_cast<MJMesh::Front *>(this->front)->interceptElements(point));
}

const Data::BoxTreeCell *MJMesh::FrontAdvancer::cell(const Data::Point &p) const
{
	return this->tree->find(p);
}

Data::PointList MJMesh::FrontAdvancer::ideal(const Data::FrontElement *element, Real factor) const
{
#if USE_EXPERIMENTAL_3
	Data::Point2D mid2D;
	Data::Point3D mid3D;

	Data::Point &mid = (this->dimension == 2) ?
        (mid2D = element->getShape()->mid2D(), static_cast<Data::Point &>(mid2D)) :
        (mid3D = element->getShape()->mid3D(), static_cast<Data::Point &>(mid3D));
#else
	Data::Point mid = element->getShape()->mid();
#endif //#if USE_EXPERIMENTAL_3

#if (!USE_EXPERIMENTAL_3)
	Data::Vector v(mid);
#endif //#if (!USE_EXPERIMENTAL_3)

	//Data::BoxTreeCell *c = this->tree->find(mid);
	const Data::BoxTreeCell *c = this->cell(mid);

	if (!c)
	{
		this->status = Performer::FrontAdvancer::error;

		return Data::PointList();
	}

#if USE_EXPERIMENTAL_3
    if (this->dimension == 2)
    {
        Data::Vector2D n = static_cast<Data::Edge2D *>(element->getShape())->normal();

        n.multiply(c->size()*factor);

        n.sum(mid);

        return Data::PointList(1, new Data::Point2D(n));
    }

    Data::Vector3D n = static_cast<Data::Triangle3D *>(element->getShape())->normal();

	n.multiply(c->size()*factor);

	n.sum(mid);

	return Data::PointList(1, new Data::Point3D(n));
#else
	Data::Vector n;

	if (this->dimension == 2)
	{
		n = static_cast<Data::Edge2D *>(element->getShape())->normal();
	}
	else if (this->dimension == 3)
	{
		n = static_cast<Data::Triangle3D *>(element->getShape())->normal();
	}

	//n.normalize();

	n.multiply(c->size()*factor);

	n.sum(v);

	Data::PointList points;

	if (this->dimension == 2)
	{
		points.push_back(new Data::Point2D(n));
	}
	else if (this->dimension == 3)
	{
		points.push_back(new Data::Point3D(n));
	}

	return points;
#endif //#if USE_EXPERIMENTAL_3
}

Real MJMesh::FrontAdvancer::radius(const Data::FrontElement *element) const
{
	//return this->tree->find(element->getShape()->mid())->size();
#if USE_EXPERIMENTAL_3
    Data::Point2D mid2D;
	Data::Point3D mid3D;

	return this->cell((this->dimension == 2) ?
        (mid2D = element->getShape()->mid2D(), static_cast<Data::Point &>(mid2D)) :
        (mid3D = element->getShape()->mid3D(), static_cast<Data::Point &>(mid3D)))->size();
#else
	return this->cell(element->getShape()->mid())->size();
#endif //#if USE_EXPERIMENTAL_3
}
