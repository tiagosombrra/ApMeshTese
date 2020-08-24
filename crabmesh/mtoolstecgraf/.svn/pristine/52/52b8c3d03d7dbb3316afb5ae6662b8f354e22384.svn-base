#include "Tools/VisibilityIdentifier.h"

#include "Data/Point2D.h"
#include "Data/Edge2D.h"
#include "Performer/SequentialIdManager.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/Boundary.h"

using namespace Tools;

Tools::VisibilityIdentifier::VisibilityIdentifier(const Tool *tool) :
    Tools::ExternalTool::ExternalTool(tool)
{
    this->boundary = NULL;
    this->fel1 = NULL;
    this->fel2 = NULL;
    this->operation = OPERATION_UNKNOWN;
    this->angleType = ANGLE_UNKNOWN;
    this->vertex = 2;
    this->v = NULL;
    this->v1 = this->v2 = NULL;
#if USE_GUI
    this->mesh = NULL;
    this->idManager = NULL;
#endif //#if USE_GUI
}

Tools::VisibilityIdentifier::~VisibilityIdentifier()
{
#if USE_GUI
    if (this->idManager)
    {
        delete this->idManager;
    }
#endif //#if USE_GUI
}

#if USE_GUI
void Tools::VisibilityIdentifier::setMesh(MJMesh::Mesh *mesh)
{
    this->mesh = mesh;
}
#endif //#if USE_GUI

void Tools::VisibilityIdentifier::setBoundary(const MJMesh::Boundary *boundary)
{
    this->boundary = boundary;
}

void Tools::VisibilityIdentifier::setVertex(const MJMesh::Vertex *vertex, const MJMesh::Mesh *mesh)
{
    Data::FrontElementList adjs = vertex->getAdjacentFrontElements();

    if (adjs.size() == 2)
    {
        this->fel1 = static_cast<MJMesh::FrontElement2D *>(adjs.front());
        this->fel2 = static_cast<MJMesh::FrontElement2D *>(adjs.back());

        return;
    }

    if (!mesh)
    {
        return;
    }

    Data::FrontElement *fel = adjs.front();

    UInt i = fel->index(vertex);

    adjs = mesh->findSortedAdjacentFrontElements(fel, i, this->boundary);

    if (((adjs.size()%2) != 0) || (adjs.size() < 2))
    {
        return;
    }

    if (adjs.size() == 2)
    {
        this->fel1 = static_cast<MJMesh::FrontElement2D *>(adjs.front());
        this->fel2 = static_cast<MJMesh::FrontElement2D *>(adjs.back());

        return;
    }

    this->fel1 = static_cast<MJMesh::FrontElement2D *>(adjs.front());

    if (vertex == this->fel1->getVertex(0))
    {
        adjs.pop_front();

        this->fel2 = static_cast<MJMesh::FrontElement2D *>(adjs.front());
    }
    else
    {
        this->fel2 = static_cast<MJMesh::FrontElement2D *>(adjs.back());
    }
}

void Tools::VisibilityIdentifier::setVertex(const MJMesh::Vertex *vertex, const MJMesh::FrontElement2D *fel)
{
    Data::FrontElementList adjs = vertex->getAdjacentFrontElements();

    if (adjs.size() == 2)
    {
        this->fel1 = static_cast<MJMesh::FrontElement2D *>(adjs.front());
        this->fel2 = static_cast<MJMesh::FrontElement2D *>(adjs.back());

        return;
    }

    MJMesh::Mesh *mesh = new MJMesh::Mesh();

    UInt i = fel->index(vertex);

    adjs = mesh->findSortedAdjacentFrontElements(fel, i, this->boundary);

    if (((adjs.size()%2) != 0) || (adjs.size() < 2))
    {
        delete mesh;

        return;
    }

    if (adjs.size() == 2)
    {
        this->fel1 = static_cast<MJMesh::FrontElement2D *>(adjs.front());
        this->fel2 = static_cast<MJMesh::FrontElement2D *>(adjs.back());

        delete mesh;

        return;
    }

    this->fel1 = static_cast<MJMesh::FrontElement2D *>(adjs.front());

    if (vertex == this->fel1->getVertex(0))
    {
        adjs.pop_front();

        this->fel2 = static_cast<MJMesh::FrontElement2D *>(adjs.front());
    }
    else
    {
        this->fel2 = static_cast<MJMesh::FrontElement2D *>(adjs.back());
    }

    delete mesh;
}

void Tools::VisibilityIdentifier::setFrontElements(const MJMesh::FrontElement2D *fel1, const MJMesh::FrontElement2D *fel2)
{
    this->fel1 = fel1;
    this->fel2 = fel2;
}

void Tools::VisibilityIdentifier::setOperation(enum Operation operation)
{
    this->operation = operation;
}

Tools::VisibilityIdentifier::Visibility Tools::VisibilityIdentifier::getVisibility() const
{
    return this->vis1;
}

void Tools::VisibilityIdentifier::execute()
{
    if ((!this->boundary) || (!this->fel1) || (!this->fel2))
    {
        return;
    }

#if USE_GUI
    if (this->mesh)
    {
        for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
             iter != this->mesh->feEnd();)
        {
            Data::FrontElement *fel = (*iter);

            if (this->boundary->have(fel))
            {
                iter++;

                continue;
            }

            iter = this->mesh->erase(iter);

            delete fel;
        }

        for (Data::Mesh::VertexIterator iter = this->mesh->vBegin();
             iter != this->mesh->vEnd();)
        {
            Data::Vertex *v = (*iter);

            if (this->boundary->have(v))
            {
                iter++;

                continue;
            }

            iter = this->mesh->erase(iter);

            delete v;
        }

        this->idManager = new Performer::SequentialIdManager(1, 2);

        for (Data::Mesh::VertexIterator iter = this->mesh->vBegin();
             iter != this->mesh->vEnd(); iter++)
        {
            if ((*iter)->getId() >= this->idManager->getId(0))
            {
                this->idManager->setId(0, (*iter)->getId() + 1);
            }
        }

        for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
             iter != this->mesh->feEnd(); iter++)
        {
            if ((*iter)->getId() >= this->idManager->getId(1))
            {
                this->idManager->setId(1, (*iter)->getId() + 1);
            }
        }
    }
#endif //#if USE_GUI

    this->findVertex();

    if (this->v == NULL)
    {
        return;
    }

    this->findAngleType();

    if (this->angleType == ANGLE_UNKNOWN)
    {
        return;
    }

    this->findOperation();

    if (this->operation == OPERATION_UNKNOWN)
    {
        return;
    }

    this->findVisibility(this->fel1, this->vis1);
    this->findVisibility(this->fel2, this->vis2);

    this->operateVisibilities();

    //debug
#if USE_GUI
    {
        for (Data::Boundary::ConstFrontElementIterator iter = this->boundary->eBegin();
             iter != this->boundary->eEnd(); iter++)
        {
            (*iter)->setColor(0.0, 0.0, 0.0);
        }

        for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
             iter != this->boundary->vEnd(); iter++)
        {
            (*iter)->setColor(0.0, 0.0, 0.0);
        }
    }
#endif //#if USE_GUI
    //endebug

    this->findIntersections();

    this->narrowVisibility();

    this->determineVisibility();
}

void Tools::VisibilityIdentifier::findVertex()
{
    if ((this->vertex > 1) || (this->fel2))
    {
        for (UInt i = 0; i < this->fel1->numVertices(); i++)
        {
            if (this->fel2->have(this->fel1->getVertex(i)))
            {
                this->vertex = i;
            }
        }
    }

    this->v = static_cast<MJMesh::Vertex *>(this->fel1->getVertex(this->vertex));
    this->v1 = static_cast<MJMesh::Vertex *>(this->fel1->getVertex(1 - this->vertex));

    if (this->fel2)
    {
        this->v2 = static_cast<MJMesh::Vertex *>(this->fel2->getVertex(this->vertex));
    }
}

void Tools::VisibilityIdentifier::findAngleType()
{
    if (!this->fel2)
    {
        return;
    }

    if (this->fel1->accordingToNormal(this->v2->getPoint(), false))
    {
        this->angleType = CONVEX;
    }
    else if (this->fel1->accordingToNormal(this->v2->getPoint(), true))
    {
        this->angleType = COLINEAR;
    }
    else
    {
        this->angleType = CONCAVE;
    }
}

void Tools::VisibilityIdentifier::findOperation()
{
    if (this->operation != OPERATION_UNKNOWN)
    {
        return;
    }

    if (this->angleType != CONCAVE)
    {
        this->operation = INTERSECTION;

        return;
    }

    if (this->angleType != ANGLE_UNKNOWN)
    {
        this->operation = UNION;
    }
}

void Tools::VisibilityIdentifier::findVisibility(const MJMesh::FrontElement2D *fel, Visibility &vis)
{
    Data::FrontElementHash fels;

    Data::EVector2D edgeVec(static_cast<Data::Point2D *>(fel->getShape()->getPoint(0))->epoint2D(), static_cast<Data::Point2D *>(fel->getShape()->getPoint(1))->epoint2D());

    for (Data::Boundary::ConstVertexIterator iter = this->boundary->vBegin();
         iter != this->boundary->vEnd(); iter++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

        Data::EVector2D vec(static_cast<Data::Point2D *>(fel->getShape()->getPoint(0))->epoint2D(), static_cast<Data::Point2D *>(v->getPoint())->epoint2D());

        if (edgeVec.cross(vec) >= 0.0)
        {
            vis.vertices.push_back(v);

            for (Data::FrontElementList::iterator iter2 = v->feBegin();
                 iter2 != v->feEnd(); iter2++)
            {
                fels.insert((*iter2));
            }
        }
    }

    for (Data::FrontElementHash::iterator iter = fels.begin();
         iter != fels.end(); iter++)
    {
        EdgeVisibility v;

        v.fel = static_cast<MJMesh::FrontElement2D *>((*iter));

        vis.edges.push_back(v);
    }
}

void Tools::VisibilityIdentifier::operateVisibilities()
{
    if (this->operation == OPERATION_UNKNOWN)
    {
        return;
    }

    Data::VertexHash vhash1, vhash2, vhash;

    vhash1.reserve(this->vis1.vertices.size());
    vhash2.reserve(this->vis2.vertices.size());
    vhash.reserve(this->vis1.vertices.size() + this->vis2.vertices.size());

    vhash1.insert(this->vis1.vertices.begin(), this->vis1.vertices.end());
    vhash2.insert(this->vis2.vertices.begin(), this->vis2.vertices.end());

    Data::FrontElementHash fehash1, fehash2, fehash;

    fehash1.reserve(this->vis1.edges.size());
    fehash2.reserve(this->vis2.edges.size());
    fehash.reserve(this->vis1.edges.size() + this->vis2.edges.size());

    for (EdgeVisibilityList::iterator iter = this->vis1.edges.begin();
         iter != this->vis1.edges.end(); iter++)
    {
        fehash1.insert((*iter).fel);
    }

    for (EdgeVisibilityList::iterator iter = this->vis2.edges.begin();
         iter != this->vis2.edges.end(); iter++)
    {
        fehash2.insert((*iter).fel);
    }

    if (this->operation == UNION)
    {
        vhash = vhash1;
        vhash.insert(vhash2.begin(), vhash2.end());

        fehash = fehash1;
        fehash.insert(fehash2.begin(), fehash2.end());
    }
    else
    {
        for (Data::VertexHash::iterator iter = vhash1.begin();
             iter != vhash1.end(); iter++)
        {
            if (vhash2.find((*iter)) != vhash2.end())
            {
                vhash.insert((*iter));
            }
        }

        for (Data::FrontElementHash::iterator iter = fehash1.begin();
             iter != fehash1.end(); iter++)
        {
            if (fehash2.find((*iter)) != fehash2.end())
            {
                fehash.insert((*iter));
            }
        }
    }

    Visibility vis;

    for (Data::VertexHash::iterator iter = vhash.begin();
         iter != vhash.end(); iter++)
    {
        vis.vertices.push_back((*iter));
    }

    for (Data::FrontElementHash::iterator iter = fehash.begin();
         iter != fehash.end(); iter++)
    {
        EdgeVisibility v;

        v.fel = static_cast<MJMesh::FrontElement2D *>((*iter));

        vis.edges.push_back(v);
    }

    this->vis1.edges.swap(vis.edges);
    this->vis1.vertices.swap(vis.vertices);

    this->vis2.edges.clear();
    this->vis2.vertices.clear();
}

void Tools::VisibilityIdentifier::findIntersections()
{
    const Data::FrontElement *fel = (this->vertex == 0) ? this->fel1 : this->fel2;

    /*std::cout << "this->vertex = " << this->vertex << std::endl;
    std::cout << "this->v = " << this->v->text() << std::endl;
    std::cout << "this->v1 = " << this->v1->text() << std::endl;
    std::cout << "this->v2 = " << this->v2->text() << std::endl;
    std::cout << "this->fel1 = " << this->fel1->text() << std::endl;
    std::cout << "this->fel2 = " << this->fel2->text() << std::endl;
    std::cout << "fel = " << fel->text() << std::endl;
    std::cout << "this->operation = " << this->operation << std::endl;
    std::cout << "this->angleType = " << this->angleType << std::endl;*/

    bool have1 = false, have2 = false;

    Data::EVector2D edgeVec(static_cast<Data::Point2D *>(fel->getShape()->getPoint(0))->epoint2D(), static_cast<Data::Point2D *>(fel->getShape()->getPoint(1))->epoint2D());

    Data::EReal dot = edgeVec.dot(edgeVec);

    Data::EFReal firstAngle(dot*dot, edgeVec.squaredNorm()*edgeVec.squaredNorm());

    if (edgeVec.cross(edgeVec) >= 0.0)
    {
        if (dot >= 0.0)
        {
            firstAngle = 1.0 - firstAngle;
        }
        else
        {
            firstAngle = 1.0 + firstAngle;
        }
    }
    else
    {
        if (dot < 0.0)
        {
            firstAngle = 3.0 - firstAngle;
        }
        else
        {
            firstAngle = 3.0 + firstAngle;
        }
    }

    Data::EVector2D otherEdgeVec(static_cast<Data::Point2D *>(this->v->getPoint())->epoint2D(),
        fel->have(this->v1) ? static_cast<Data::Point2D *>(this->v2->getPoint())->epoint2D() : static_cast<Data::Point2D *>(this->v1->getPoint())->epoint2D());

    dot = edgeVec.dot(otherEdgeVec);

    Data::EFReal lastAngle(dot*dot, edgeVec.squaredNorm()*otherEdgeVec.squaredNorm());

    if (edgeVec.cross(otherEdgeVec) >= 0.0)
    {
        if (dot >= 0.0)
        {
            lastAngle = 1.0 - lastAngle;
        }
        else
        {
            lastAngle = 1.0 + lastAngle;
        }
    }
    else
    {
        if (dot < 0.0)
        {
            lastAngle = 3.0 - lastAngle;
        }
        else
        {
            lastAngle = 3.0 + lastAngle;
        }
    }

    //std::cout << "firstangle = " << static_cast<Real>(firstAngle) << std::endl;
    //std::cout << "lastangle  = " << static_cast<Real>(lastAngle) << std::endl;

    for (Data::VertexList::iterator iter = this->vis1.vertices.begin();
         iter != this->vis1.vertices.end(); iter++)
    {
        if ((*iter) == this->v)
        {
            continue;
        }

        if ((*iter) == this->v1)
        {
            have1 = true;
        }

        if ((*iter) == this->v2)
        {
            have2 = true;
        }

        Vtx vtx;

        vtx.artificial = false;
        vtx.v = (*iter);
        vtx.vec = Data::EVector2D(static_cast<Data::Point2D *>(this->v->getPoint())->epoint2D(), static_cast<Data::Point2D *>(vtx.v->getPoint())->epoint2D());

        dot = edgeVec.dot(vtx.vec);

        vtx.angle = Data::EFReal(dot*dot, edgeVec.squaredNorm()*vtx.vec.squaredNorm());
        vtx.dist = vtx.vec.squaredNorm();

        if (edgeVec.cross(vtx.vec) >= 0.0)
        {
            if (dot >= 0.0)
            {
                vtx.angle = 1.0 - vtx.angle;
            }
            else
            {
                vtx.angle = 1.0 + vtx.angle;
            }
        }
        else
        {
            if (dot < 0.0)
            {
                vtx.angle = 3.0 - vtx.angle;
            }
            else
            {
                vtx.angle = 3.0 + vtx.angle;
            }
        }

        //std::cout << "v " << vtx.v->getId() << ", angle " << static_cast<Real>(vtx.angle) << std::endl;

        if ((vtx.angle >= firstAngle) && (vtx.angle <= lastAngle))
        {
            this->vertices.insert(vtx);
        }
    }

    if (!have1)
    {
        Vtx vtx;

        vtx.v = this->v1;

        vtx.artificial = true;
        //vtx.vec = Data::EVector2D(static_cast<Data::Point2D *>(this->v1->getPoint())->epoint2D(), static_cast<Data::Point2D *>(this->v->getPoint())->epoint2D());
        //vtx.vec = Data::EVector2D(static_cast<Data::Point2D *>(this->v->getPoint())->epoint2D(), static_cast<Data::Point2D *>(this->v1->getPoint())->epoint2D());
        vtx.vec = (this->angleType == CONVEX) ?
            Data::EVector2D(static_cast<Data::Point2D *>(this->v1->getPoint())->epoint2D(), static_cast<Data::Point2D *>(this->v->getPoint())->epoint2D()) :
            Data::EVector2D(static_cast<Data::Point2D *>(this->v->getPoint())->epoint2D(), static_cast<Data::Point2D *>(this->v1->getPoint())->epoint2D());

        dot = edgeVec.dot(vtx.vec);

        vtx.angle = Data::EFReal(dot*dot, edgeVec.squaredNorm()*vtx.vec.squaredNorm());

        if (edgeVec.cross(vtx.vec) >= 0.0)
        {
            if (dot >= 0.0)
            {
                vtx.angle = 1.0 - vtx.angle;
            }
            else
            {
                vtx.angle = 1.0 + vtx.angle;
            }
        }
        else
        {
            if (dot < 0.0)
            {
                vtx.angle = 3.0 - vtx.angle;
            }
            else
            {
                vtx.angle = 3.0 + vtx.angle;
            }
        }

        firstAngle = vtx.angle;

        //std::cout << "new firstangle = " << static_cast<Real>(firstAngle) << std::endl;

        vtx.dist = vtx.vec.squaredNorm();

        this->vertices.insert(vtx);
    }

    if (!have2)
    {
        Vtx vtx;

        vtx.v = this->v2;

        vtx.artificial = true;
        //vtx.vec = Data::EVector2D(static_cast<Data::Point2D *>(this->v2->getPoint())->epoint2D(), static_cast<Data::Point2D *>(this->v->getPoint())->epoint2D());
        //vtx.vec = Data::EVector2D(static_cast<Data::Point2D *>(this->v->getPoint())->epoint2D(), static_cast<Data::Point2D *>(this->v2->getPoint())->epoint2D());
        vtx.vec = (this->angleType == CONVEX) ?
            Data::EVector2D(static_cast<Data::Point2D *>(this->v2->getPoint())->epoint2D(), static_cast<Data::Point2D *>(this->v->getPoint())->epoint2D()) :
            Data::EVector2D(static_cast<Data::Point2D *>(this->v->getPoint())->epoint2D(), static_cast<Data::Point2D *>(this->v2->getPoint())->epoint2D());

        dot = edgeVec.dot(vtx.vec);

        vtx.angle = Data::EFReal(dot*dot, edgeVec.squaredNorm()*vtx.vec.squaredNorm());

        if (edgeVec.cross(vtx.vec) >= 0.0)
        {
            if (dot >= 0.0)
            {
                vtx.angle = 1.0 - vtx.angle;
            }
            else
            {
                vtx.angle = 1.0 + vtx.angle;
            }
        }
        else
        {
            if (dot < 0.0)
            {
                vtx.angle = 3.0 - vtx.angle;
            }
            else
            {
                vtx.angle = 3.0 + vtx.angle;
            }
        }

        lastAngle = vtx.angle;

        //std::cout << "new lastangle  = " << static_cast<Real>(lastAngle) << std::endl;

        vtx.dist = vtx.vec.squaredNorm();

        this->vertices.insert(vtx);
    }

    for (VtxSet::iterator iter = this->vertices.begin();
         iter != this->vertices.end(); iter++)
    {
        bool test = true;

        if (iter != this->vertices.begin())
        {
            VtxSet::iterator prev = iter;

            prev--;

            if ((*iter).angle <= (*prev).angle)
            {
               test = false;
            }
        }

        if (test)
        {
            if (((*iter).angle < firstAngle) || ((*iter).angle > lastAngle))
            {
                test = false;
            }
        }

        if (!test)
        {
            continue;
        }

        enum Data::Position pos = Data::IN;

        if (((*iter).angle == firstAngle) || ((*iter).angle == lastAngle))
        {
            pos = Data::ON;
        }

        /*std::cout << "vertex = " << (*iter).v->getId() << ", angle = " << (*iter).angle << ", firstangle = " << (*this->vertices.begin()).angle << ", lastangle = " << (*--this->vertices.end()).angle
            << ", angle = " << static_cast<Real>((*iter).angle) << ", firstangle = " << static_cast<Real>((*this->vertices.begin()).angle) << ", lastangle = " << static_cast<Real>((*--this->vertices.end()).angle)
            << ", pos = " << ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : ((pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;*/

        this->findIntersections(&(*iter), (*iter).vec, this->vis1.edges, (*iter).intersections, pos);
    }

    for (FEEdgeIntersectionSetMap::iterator iter = this->edgeVisibility.begin();
         iter != this->edgeVisibility.end(); iter++)
    {
        if ((*(*iter).second.begin()).s != 0.0)
        //if ((*(*iter).second.begin()).s > Data::getTolerance())
        {
            EdgeIntersection ei;
            ei.s = 0.0;
            ei.vtx = NULL;
            ei.prevPos = ei.pos = Data::OUT;
            (*iter).second.insert(ei);
        }

        if ((*--(*iter).second.end()).s != 1.0)
        //if ((*--(*iter).second.end()).s < 1.0 - Data::getTolerance())
        {
            EdgeIntersection ei;
            ei.s = 1.0;
            ei.vtx = NULL;
            ei.prevPos = ei.pos = Data::OUT;
            (*iter).second.insert(ei);
        }
    }

    /*//debug
    Real incr = 1.0/static_cast<Real>(this->vertices.size());
    Real col = 0.0;

    for (VtxSet::iterator iter = this->vertices.begin();
         iter != this->vertices.end(); iter++, col += incr)
    {
        //(*iter).v->setR(1.0);
        //(*iter).v->setG(col);

        //(*iter).v->setColor(0.0, 0.0, 0.0);

        MJMesh::Vertex *oldV = this->v;

        IntersectionSet::iterator first = (*iter).intersections.end();

        for (IntersectionSet::iterator iter2 = (*iter).intersections.begin();
             iter2 != (*iter).intersections.end(); iter2++)
        {
            if ((first != (*iter).intersections.end()) &&
                (*iter2).t != (*first).t)
            {
                break;
            }

            if ((*iter2).t > 0.0)
            {
                first = iter2;
            }

            std::cout << "vtx " << (*iter).v->getId() << ", t " << (*iter2).t << ", fel " << ((*iter2).fel ? (*iter2).fel->text() : "") << std::endl;

            Real x = this->v->getPoint()->getCoord(0) + (*iter2).t*(*iter).vec.getCoord(0);
            Real y = this->v->getPoint()->getCoord(1) + (*iter2).t*(*iter).vec.getCoord(1);

            MJMesh::Vertex *newV = new MJMesh::Vertex(x, y, this->idManager->next(0));

            std::cout << "newV2 " << newV->text() << std::endl;

            this->mesh->add(newV);

            newV->setColor(0.0, col, 1.0);
            //newV->setColor(0.0, 0.0, 0.0);

            MJMesh::FrontElement2D *newFel = new MJMesh::FrontElement2D(oldV, newV, this->idManager->next(1));

            std::cout << "newFel2 " << newFel->text() << std::endl;

            newFel->setColor(1.0, col, 0.0);

            this->mesh->add(newFel);

            oldV = newV;
        }
    }
    //endebug*/
}

void Tools::VisibilityIdentifier::narrowVisibility()
{
    for (FEEdgeIntersectionSetMap::iterator iter = this->edgeVisibility.begin();
         iter != this->edgeVisibility.end(); iter++)
    {
        Data::FrontElement *fel = (*iter).first;

        for (EdgeIntersectionSet::iterator iter2 = (*iter).second.begin();
             iter2 != (*iter).second.end(); iter2++)
        {
            if ((*iter2).pos == Data::OUT)
            {
                continue;
            }

            if ((*iter2).pos == Data::ON)
            {
                continue;
            }

            Data::EFReal t = -1.0;
            Data::EFReal first = -1.0;

            for (IntersectionSet::iterator iter3 = (*iter2).vtx->intersections.begin();
                 iter3 != (*iter2).vtx->intersections.end(); iter3++)
            {
                if ((first < 0.0) && ((*iter3).t > 0.0))
                {
                    first = (*iter3).t;
                }

                if ((*iter3).fel == fel)
                {
                    t = (*iter3).t;

                    break;
                }
            }

            if ((t < 0.0) ||
                ((first > 0.0) && (t > first)))
            {
                (*iter2).prevPos = (*iter2).pos;
                (*iter2).pos = Data::OUT;
            }
        }
    }

    for (FEEdgeIntersectionSetMap::iterator iter = this->edgeVisibility.begin();
         iter != this->edgeVisibility.end(); iter++)
    {
        EdgeIntersectionSet::iterator first, second, third;
        first = second = third = (*iter).second.end();

        for (EdgeIntersectionSet::iterator iter2 = (*iter).second.begin();
             iter2 != (*iter).second.end();)
        {
            if (first == (*iter).second.end())
            {
                first = iter2;

                iter2++;

                continue;
            }

            if (second == (*iter).second.end())
            {
                second = iter2;

                iter2++;

                continue;
            }

            if (third == (*iter).second.end())
            {
                third = iter2;

                iter2++;
            }

            if (((*first).pos == (*second).pos) &&
                ((*second).pos == (*third).pos) &&
                ((*first).prevPos == (*second).prevPos) &&
                ((*second).prevPos == (*third).prevPos))
            {
                (*iter).second.erase(second);

                second = third;

                third = (*iter).second.end();

                continue;
            }

            first = second;

            second = third;

            third = (*iter).second.end();
        }
    }

    for (FEEdgeIntersectionSetMap::iterator iter = this->edgeVisibility.begin();
         iter != this->edgeVisibility.end();)
    {
        bool allOut = true;
        bool allOn = true;
        //bool allIn = true;

        for (EdgeIntersectionSet::iterator iter2 = (*iter).second.begin();
             iter2 != (*iter).second.end(); iter2++)
        {
            if ((*iter2).pos != Data::OUT)
            {
                allOut = false;
            }

            if ((*iter2).pos != Data::ON)
            {
                allOn = false;
            }

            /*if ((*iter2).pos != Data::IN)
            {
                allIn = false;
            }*/
        }

        if (allOut)
        {
            //the whole edge is hidden
            iter = this->edgeVisibility.erase(iter);

            continue;
        }

        if (allOn)
        {
            EdgeIntersectionSet::iterator iter2 = (*iter).second.begin();

            Data::EFReal tmin = -1.0;

            for (IntersectionSet::iterator iter3 = (*iter2).vtx->intersections.begin();
                 iter3 != (*iter2).vtx->intersections.end(); iter3++)
            {
                if ((*iter3).fel == (*iter).first)
                {
                    tmin = (*iter3).t;

                    break;
                }
            }

            if (tmin > (*(*iter2).vtx->intersections.begin()).t)
            {
                //the edge is parallel to a vector that bounds the visibility region
                //  and it is not the first edge
                iter = this->edgeVisibility.erase(iter);

                continue;
            }
        }

        iter++;
    }

    /*//debug
    for (FEEdgeIntersectionSetMap::iterator iter = this->edgeVisibility.begin();
         iter != this->edgeVisibility.end(); iter++)
    {
        MJMesh::Vertex *oldV = NULL;

        Data::Point2D p((*iter).first->getVertex(0)->getPoint());
        Data::Vector2D v((*iter).first->getVertex(0)->getPoint(), (*iter).first->getVertex(1)->getPoint());

        for (EdgeIntersectionSet::iterator iter2 = (*iter).second.begin();
             iter2 != (*iter).second.end(); iter2++)
        {
            //std::cout << "fel " << (*iter).first->getId() << ", s " << (*iter2).s << ", pos " << (*iter2).pos << std::endl;

            Real x = p.getCoord(0) + (*iter2).s*v.getCoord(0);
            Real y = p.getCoord(1) + (*iter2).s*v.getCoord(1);

            MJMesh::Vertex *newV = new MJMesh::Vertex(x, y, this->idManager->next(0));

            //std::cout << "newV " << newV->text() << std::endl;

            this->mesh->add(newV);

            Real r, g, b;

            if (((*iter2).prevPos == Data::OUT) && ((*iter2).pos == Data::IN))
            {
                r = 0.0;
                g = 0.0;
                b = 0.0;
            }
            else if (((*iter2).prevPos == Data::OUT) && ((*iter2).pos == Data::ON))
            {
                r = 0.0;
                g = 0.0;
                b = 0.0;
            }
            else if (((*iter2).prevPos == Data::OUT) && ((*iter2).pos == Data::OUT))
            {
                r = 1.0;
                g = 0.0;
                b = 0.0;
            }
            else if (((*iter2).prevPos == Data::ON) && ((*iter2).pos == Data::IN))
            {
                r = 0.0;
                g = 0.0;
                b = 0.0;
            }
            else if (((*iter2).prevPos == Data::ON) && ((*iter2).pos == Data::ON))
            {
                r = 0.0;
                g = 0.0;
                b = 1.0;
            }
            else if (((*iter2).prevPos == Data::ON) && ((*iter2).pos == Data::OUT))
            {
                r = 1.0;
                g = 0.0;
                b = 1.0;
            }
            else if (((*iter2).prevPos == Data::IN) && ((*iter2).pos == Data::IN))
            {
                r = 0.0;
                g = 1.0;
                b = 0.0;
            }
            else if (((*iter2).prevPos == Data::IN) && ((*iter2).pos == Data::ON))
            {
                r = 0.0;
                g = 0.0;
                b = 0.0;
            }
            else if (((*iter2).prevPos == Data::IN) && ((*iter2).pos == Data::OUT))
            {
                r = 1.0;
                g = 1.0;
                b = 0.0;
            }

            newV->setColor(r, g, b);

            if (oldV)
            {
                MJMesh::FrontElement2D *newFel = new MJMesh::FrontElement2D(oldV, newV, this->idManager->next(1));

                //std::cout << "newFel " << newFel->text() << std::endl;

                this->mesh->add(newFel);

                Real r = 0.0, g = 0.0, b = 0.0;

                newFel->setColor(r, g, b);
            }

            oldV = newV;
        }
    }
    //endebug*/
}

bool Tools::VisibilityIdentifier::areCollinear(const Data::Point2D *p1, const Data::Point2D *p2, const Data::Point2D *p3) const
{
    Data::EVector2D v1(p1->epoint2D(), p2->epoint2D());
    Data::EVector2D v2(p1->epoint2D(), p3->epoint2D());

    Data::EFReal dot = v1.dot(v2);

    dot /= v1.dot(v1);
    dot /= v2.dot(v2);

    return (dot == 1.0);
}

bool Tools::VisibilityIdentifier::isVisible(const Data::Point2D *p1,
    const Data::Point2D *p2, const Data::Point2D *p3, Data::EFReal t2, Data::EFReal t3, const Data::FrontElement *fel)
{
    Vtx vtx;

    vtx.v = NULL;
    vtx.angle = 0.0;
    vtx.dist = 0.0;
    vtx.artificial = true;

    Data::EVector2D v(p2->epoint2D(), p3->epoint2D());
    Data::EFReal mid = 0.5*(t2 + t3);
    v.multiply(mid);

    Data::EPoint2D p4(p2->epoint2D());
    p4.sum(v);

    vtx.vec = Data::EVector2D(p1->epoint2D(), Data::EPoint2D(p4));

    this->findIntersections(&vtx, vtx.vec, this->vis1.edges, vtx.intersections, Data::UNKNOWN, false);

    return ((vtx.intersections.empty()) || ((*vtx.intersections.begin()).fel == fel));
}

void Tools::VisibilityIdentifier::determineVisibility()
{
    for (FEEdgeIntersectionSetMap::iterator iter = this->edgeVisibility.begin();
         iter != this->edgeVisibility.end(); iter++)
    {
        Data::FrontElement *fel = (*iter).first;
        EdgeIntersectionSet::iterator first, second;
        first = second = (*iter).second.end();

        //std::cout << "analyzing fel " << fel->text() << std::endl;

        for (EdgeIntersectionSet::iterator iter2 = (*iter).second.begin();
             iter2 != (*iter).second.end();)
        {
            if (first == (*iter).second.end())
            {
                first = iter2;

                iter2++;

                continue;
            }

            if (second == (*iter).second.end())
            {
                second = iter2;

                iter2++;
            }

            if (this->segments[fel].vertices.empty())
            {
                VertexSegment vs;

                vs.pos = (*first).pos;
                vs.t = (*first).s;

                /*std::cout << "found first vertex " << std::endl;
                std::cout << "t = " << vs.t << std::endl;
                std::cout << "t = " << static_cast<Real>(vs.t) << std::endl;
                std::cout << "pos = " << ((vs.pos == Data::IN) ? "IN" : ((vs.pos == Data::ON) ? "ON" : ((vs.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;*/

                this->segments[fel].vertices.push_back(vs);
            }

            VertexSegment vs;

            vs.pos = (*second).pos;
            vs.t = (*second).s;

            /*std::cout << "found vertex " << std::endl;
            std::cout << "t = " << vs.t << std::endl;
            std::cout << "t = " << static_cast<Real>(vs.t) << std::endl;
            std::cout << "pos = " << ((vs.pos == Data::IN) ? "IN" : ((vs.pos == Data::ON) ? "ON" : ((vs.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;*/

            /*VertexSegment last = this->segments[fel].vertices.back();

            if ((last.pos == vs.pos) && (last.t == vs.t))
            {
                continue;
            }*/

            this->segments[fel].vertices.push_back(vs);

            EdgeSegment es;

            es.t1 = (*first).s;
            es.t2 = (*second).s;

            es.pos = Data::UNKNOWN;

            /*std::cout << "found segment " << std::endl;
            std::cout << "t1 = " << es.t1 << std::endl;
            std::cout << "t2 = " << es.t2 << std::endl;
            std::cout << "t1 = " << static_cast<Real>(es.t1) << std::endl;
            std::cout << "t2 = " << static_cast<Real>(es.t2) << std::endl;
            std::cout << "pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;*/

            if (((*first).pos == Data::IN) && ((*second).pos == Data::IN))
            {
                es.pos = Data::IN;

                //std::cout << "changed (1) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
            }
            else if (((*first).pos == Data::IN) && ((*second).pos == Data::ON))
            {
                es.pos = Data::IN;

                //std::cout << "changed (2) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
            }
            else if (((*first).pos == Data::IN) && ((*second).pos == Data::OUT))
            {
                //IN or OUT
                bool visible = this->isVisible(static_cast<Data::Point2D *>(this->v->getPoint()),
                    static_cast<Data::Point2D *>(fel->getVertex(0)->getPoint()), static_cast<Data::Point2D *>(fel->getVertex(1)->getPoint()), es.t1, es.t2, fel);

                if (visible)
                {
                    es.pos = Data::IN;

                    //std::cout << "changed (3 A) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                }
                else
                {
                    es.pos = Data::OUT;

                    //std::cout << "changed (3 B) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                }
            }
            else if (((*first).pos == Data::ON) && ((*second).pos == Data::IN))
            {
                es.pos = Data::IN;

                //std::cout << "changed (4) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
            }
            else if (((*first).pos == Data::ON) && ((*second).pos == Data::ON))
            {
                //IN, ON or OUT
                if ((fel->getVertex(0) == this->v) || (fel->getVertex(1) == this->v))
                {
                    es.pos = Data::ON;

                    //std::cout << "changed (5) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                }
                else
                {
                    //IN or OUT
                    if (this->angleType == CONVEX)
                    {
                        es.pos = Data::IN;

                        //std::cout << "changed (6) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                    else
                    {
                        es.pos = Data::OUT;

                        //std::cout << "changed (7) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                }
            }
            else if (((*first).pos == Data::ON) && ((*second).pos == Data::OUT))
            {
                if ((*second).prevPos == Data::IN)
                {
                    //IN or OUT
                    bool visible = this->isVisible(static_cast<Data::Point2D *>(this->v->getPoint()),
                        static_cast<Data::Point2D *>(fel->getVertex(0)->getPoint()), static_cast<Data::Point2D *>(fel->getVertex(1)->getPoint()), es.t1, es.t2, fel);

                    if (visible)
                    {
                        es.pos = Data::IN;

                        //std::cout << "changed (8) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                    else
                    {
                        es.pos = Data::OUT;

                        //std::cout << "changed (9) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                }
                else if ((*second).prevPos == Data::ON)
                {
                    //IN, ON or OUT
                    bool collinear = this->areCollinear(static_cast<Data::Point2D *>(this->v->getPoint()),
                        static_cast<Data::Point2D *>(fel->getVertex(0)->getPoint()), static_cast<Data::Point2D *>(fel->getVertex(1)->getPoint()));

                    if (collinear)
                    {
                        //ON that changes to OUT
                        es.pos = Data::OUT;

                        //std::cout << "changed (10) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                    else
                    {
                        //IN or OUT
                        if (this->angleType == CONVEX)
                        {
                            es.pos = Data::IN;

                            //std::cout << "changed (11) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                        }
                        else
                        {
                            es.pos = Data::OUT;

                            //std::cout << "changed (12) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                        }
                    }
                }
                else if ((*second).prevPos == Data::OUT)
                {
                    es.pos = Data::OUT;

                    //std::cout << "changed (13) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                }
            }
            else if (((*first).pos == Data::OUT) && ((*second).pos == Data::IN))
            {
                //IN or OUT
                bool visible = this->isVisible(static_cast<Data::Point2D *>(this->v->getPoint()),
                    static_cast<Data::Point2D *>(fel->getVertex(0)->getPoint()), static_cast<Data::Point2D *>(fel->getVertex(1)->getPoint()), es.t1, es.t2, fel);

                if (visible)
                {
                    es.pos = Data::IN;

                    //std::cout << "changed (14 A) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                }
                else
                {
                    es.pos = Data::OUT;

                    //std::cout << "changed (14 B) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                }
            }
            else if (((*first).pos == Data::OUT) && ((*second).pos == Data::ON))
            {
                if ((*first).prevPos == Data::IN)
                {
                    //IN or OUT
                    bool visible = this->isVisible(static_cast<Data::Point2D *>(this->v->getPoint()),
                        static_cast<Data::Point2D *>(fel->getVertex(0)->getPoint()), static_cast<Data::Point2D *>(fel->getVertex(1)->getPoint()), es.t1, es.t2, fel);

                    if (visible)
                    {
                        es.pos = Data::IN;

                        //std::cout << "changed (15) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                    else
                    {
                        es.pos = Data::OUT;

                        //std::cout << "changed (16) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                }
                else if ((*first).prevPos == Data::ON)
                {
                    //IN, ON or OUT
                    bool collinear = this->areCollinear(static_cast<Data::Point2D *>(this->v->getPoint()),
                        static_cast<Data::Point2D *>(fel->getVertex(0)->getPoint()), static_cast<Data::Point2D *>(fel->getVertex(1)->getPoint()));

                    if (collinear)
                    {
                        //ON that changes to OUT
                        es.pos = Data::OUT;

                        //std::cout << "changed (17) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                    else
                    {
                        //IN or OUT
                        if (this->angleType == CONVEX)
                        {
                            es.pos = Data::IN;

                            //std::cout << "changed (18) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                        }
                        else
                        {
                            es.pos = Data::OUT;

                            //std::cout << "changed (19) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                        }
                    }
                }
                else if ((*first).prevPos == Data::OUT)
                {
                    es.pos = Data::OUT;

                    //std::cout << "changed (20) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                }
            }
            else if (((*first).pos == Data::OUT) && ((*second).pos == Data::OUT))
            {
                if (((*first).prevPos == Data::OUT) || ((*second).prevPos == Data::OUT))
                {
                    es.pos = Data::OUT;

                    //std::cout << "changed (21) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                }
                else if (((*first).prevPos == Data::ON) && ((*second).prevPos == Data::ON))
                {
                    //IN, ON or OUT
                    bool collinear = this->areCollinear(static_cast<Data::Point2D *>(this->v->getPoint()),
                        static_cast<Data::Point2D *>(fel->getVertex(0)->getPoint()), static_cast<Data::Point2D *>(fel->getVertex(1)->getPoint()));

                    if (collinear)
                    {
                        //ON that changes to OUT
                        es.pos = Data::OUT;

                        //std::cout << "changed (22) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                    else
                    {
                        //IN or OUT
                        if (this->angleType == CONVEX)
                        {
                            es.pos = Data::IN;

                            //std::cout << "changed (23) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                        }
                        else
                        {
                            es.pos = Data::OUT;

                            //std::cout << "changed (24) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                        }
                    }
                }
                else
                {
                    //IN or OUT
                    bool visible = this->isVisible(static_cast<Data::Point2D *>(this->v->getPoint()),
                        static_cast<Data::Point2D *>(fel->getVertex(0)->getPoint()), static_cast<Data::Point2D *>(fel->getVertex(1)->getPoint()), es.t1, es.t2, fel);

                    if (visible)
                    {
                        es.pos = Data::IN;

                        //std::cout << "changed (25) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                    else
                    {
                        es.pos = Data::OUT;

                        //std::cout << "changed (26) pos = " << ((es.pos == Data::IN) ? "IN" : ((es.pos == Data::ON) ? "ON" : ((es.pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }
                }
            }

            this->segments[fel].segments.push_back(es);

            first = second;

            second = (*iter).second.end();
        }
    }

    for (SegmentedEdgeMap::iterator iter = this->segments.begin();
         iter != this->segments.end(); iter++)
    {
        if ((*iter).second.segments.size() < 2)
        {
            continue;
        }

        //VertexSegmentList::iterator viter1 = (*iter).second.vertices.begin();
        VertexSegmentList::iterator viter2 = ++((*iter).second.vertices.begin());
        //VertexSegmentList::iterator viter3 = viter2;
        //viter3++;

        EdgeSegmentList::iterator siter1 = (*iter).second.segments.begin();
        EdgeSegmentList::iterator siter2 = siter1;
        siter2++;

        while (siter2 != (*iter).second.segments.end())
        {
            if (((*siter1).pos == (*siter2).pos) &&
                ((*siter1).pos == (*viter2).pos))
            {
                /*std::cout << "erasing vertex of fel " << (*iter).first->text() << std::endl;
                std::cout << "t = " << (*viter2).t << std::endl;
                std::cout << "t = " << static_cast<Real>((*viter2).t) << std::endl;
                std::cout << "pos = " << (((*viter2).pos == Data::IN) ? "IN" : (((*viter2).pos == Data::ON) ? "ON" : (((*viter2).pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;

                std::cout << "merging segment of fel " << (*iter).first->text() << std::endl;
                std::cout << "t1 = " << (*siter1).t1 << std::endl;
                std::cout << "t2 = " << (*siter1).t2 << std::endl;
                std::cout << "t1 = " << static_cast<Real>((*siter1).t1) << std::endl;
                std::cout << "t1 = " << static_cast<Real>((*siter1).t2) << std::endl;
                std::cout << "pos = " << (((*siter1).pos == Data::IN) ? "IN" : (((*siter1).pos == Data::ON) ? "ON" : (((*siter1).pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                std::cout << "with segment" << std::endl;
                std::cout << "t1 = " << (*siter2).t1 << std::endl;
                std::cout << "t2 = " << (*siter2).t2 << std::endl;
                std::cout << "t1 = " << static_cast<Real>((*siter2).t1) << std::endl;
                std::cout << "t1 = " << static_cast<Real>((*siter2).t2) << std::endl;
                std::cout << "pos = " << (((*siter2).pos == Data::IN) ? "IN" : (((*siter2).pos == Data::ON) ? "ON" : (((*siter2).pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;*/

                (*siter1).t2 = (*siter2).t2;

                siter2 = (*iter).second.segments.erase(siter2);

                viter2 = (*iter).second.vertices.erase(viter2);

                continue;
            }

            siter1++;
            siter2++;
            viter2++;
        }
    }

    for (SegmentedEdgeMap::iterator iter = this->segments.begin();
         iter != this->segments.end();)
    {
        if ((*iter).second.segments.size() > 1)
        {
            iter++;

            continue;
        }

        if ((*iter).second.segments.size() == 0)
        {
            iter = this->segments.erase(iter);

            continue;
        }

        if ((*iter).second.segments.size() == 1)
        {
            if (((*iter).second.segments.front().pos == Data::OUT) &&
                ((*iter).second.vertices.front().pos == Data::OUT) &&
                ((*iter).second.vertices.back().pos == Data::OUT))
            {
                iter = this->segments.erase(iter);

                continue;
            }
        }

        iter++;
    }

    Visibility vis;
    Data::VertexHash vertices;

    for (SegmentedEdgeMap::iterator iter = this->segments.begin();
         iter != this->segments.end(); iter++)
    {
        Data::FrontElement *fel = (*iter).first;

        /*//debug
        {
            std::cout << "vertices of fel " << fel->text() << std::endl;

            for (VertexSegmentList::iterator iter2 = (*iter).second.vertices.begin();
                 iter2 != (*iter).second.vertices.end(); iter2++)
            {
                std::cout << "t = " << (*iter2).t << std::endl;
                std::cout << "t = " << static_cast<Real>((*iter2).t) << std::endl;
                std::cout << "pos = " << (((*iter2).pos == Data::IN) ? "IN" : (((*iter2).pos == Data::ON) ? "ON" : (((*iter2).pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
            }

            std::cout << "segments of fel " << fel->text() << std::endl;

            for (EdgeSegmentList::iterator iter2 = (*iter).second.segments.begin();
                 iter2 != (*iter).second.segments.end(); iter2++)
            {
                std::cout << "t1 = " << (*iter2).t1 << std::endl;
                std::cout << "t2 = " << (*iter2).t2 << std::endl;
                std::cout << "t1 = " << static_cast<Real>((*iter2).t1) << std::endl;
                std::cout << "t2 = " << static_cast<Real>((*iter2).t2) << std::endl;
                std::cout << "pos = " << (((*iter2).pos == Data::IN) ? "IN" : (((*iter2).pos == Data::ON) ? "ON" : (((*iter2).pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
            }
        }
        //endebug*/

        if (!(*iter).second.vertices.empty())
        {
            if (((*iter).second.vertices.front().pos != Data::OUT) &&
                ((*iter).second.vertices.front().t == 0.0))
            {
                if (fel->getVertex(0) != this->v)
                {
                    vertices.insert(fel->getVertex(0));
                }
            }

            if (((*iter).second.vertices.back().pos != Data::OUT) &&
                ((*iter).second.vertices.back().t == 1.0))
            {
                if (fel->getVertex(1) != this->v)
                {
                    vertices.insert(fel->getVertex(1));
                }
            }
        }

        EdgeVisibility edgVis;

        edgVis.fel = static_cast<MJMesh::FrontElement2D *>(fel);

        for (EdgeSegmentList::iterator iter2 = (*iter).second.segments.begin();
             iter2 != (*iter).second.segments.end(); iter2++)
        {
            if ((*iter2).pos == Data::OUT)
            {
                continue;
            }

            edgVis.visibility.push_back((*iter2).t1);
            edgVis.visibility.push_back((*iter2).t2);
        }

        if (!edgVis.visibility.empty())
        {
            vis.edges.push_back(edgVis);
        }
    }

    vis.vertices.insert(vis.vertices.end(), vertices.begin(), vertices.end());

    this->vis1 = vis;

    //debug
#if USE_GUI
    if (this->mesh)
    {
        for (Data::VertexList::iterator iter = this->vis1.vertices.begin();
             iter != this->vis1.vertices.end(); iter++)
        {
            (*iter)->setColor(0.0, 1.0, 0.0);
        }

        for (EdgeVisibilityList::iterator iter = this->vis1.edges.begin();
             iter != this->vis1.edges.end(); iter++)
        {
            //(*iter).fel->setColor(0.0, 0.0, 1.0);

            Data::Point2D *p = static_cast<Data::Point2D *>((*iter).fel->getVertex(0)->getPoint());
            Data::Vector2D v(p, static_cast<Data::Point2D *>((*iter).fel->getVertex(1)->getPoint()));

            for (Data::EFRealVector::iterator iter2 = (*iter).visibility.begin();
                 iter2 != (*iter).visibility.end(); iter2++)
            {
                if ((*iter2) == 0.0)
                {
                    Data::EFRealVector::iterator iter3 = iter2;
                    iter3++;

                    if ((*iter3) == 1.0)
                    {
                        (*iter).fel->setColor(0.0, 1.0, 0.0);

                        break;
                    }
                }

                Real x1 = p->getCoord(0) + (*iter2)*v.getCoord(0);
                Real y1 = p->getCoord(1) + (*iter2)*v.getCoord(1);

                MJMesh::Vertex *v1 = new MJMesh::Vertex(x1, y1, this->idManager->next(0));

                Data::EFRealVector::iterator iter3 = iter2;
                iter3++;

                Real x2 = p->getCoord(0) + (*iter3)*v.getCoord(0);
                Real y2 = p->getCoord(1) + (*iter3)*v.getCoord(1);

                MJMesh::Vertex *v2 = new MJMesh::Vertex(x2, y2, this->idManager->next(0));

                MJMesh::FrontElement2D *newFel = new MJMesh::FrontElement2D(v1, v2, this->idManager->next(1));

                if ((*iter2) == 0.0)
                {
                    v1->setColor(0.0, 1.0, 0.0);
                }
                else
                {
                    v1->setColor(0.0, 0.0, 1.0);
                }

                if ((*iter3) == 1.0)
                {
                    v2->setColor(0.0, 1.0, 0.0);
                }
                else
                {
                    v2->setColor(0.0, 0.0, 1.0);
                }

                newFel->setColor(0.0, 0.0, 1.0);

                this->mesh->add(v1);
                this->mesh->add(v2);
                this->mesh->add(newFel);

                iter2++;
            }
        }
    }
#endif //#if USE_GUI
    //endebug
}

void Tools::VisibilityIdentifier::findIntersections(const Vtx *vtx, const Data::EVector2D &vec,
    const EdgeVisibilityList &edges, IntersectionSet &intersections, enum Data::Position pos, bool addToVisibility)
{
    for (EdgeVisibilityList::const_iterator iter = edges.begin();
         iter != edges.end(); iter++)
    {
        IntersectionSet ints = this->findIntersections(vtx, vec, (*iter).fel, pos, addToVisibility);

        for (IntersectionSet::iterator iter2 = ints.begin();
             iter2 != ints.end(); iter2++)
        {
            Intersection i = (*iter2);

            i.fel = (*iter).fel;

            intersections.insert(i);
        }
    }
}

Tools::VisibilityIdentifier::IntersectionSet Tools::VisibilityIdentifier::findIntersections(
    const Vtx *vtx, const Data::EVector2D &vec, MJMesh::FrontElement2D *fel, enum Data::Position pos, bool addToVisibility)
{
    //std::cout << "will test intersection with fel " << fel->text() << std::endl;

    //codigo de Geometric Tools for Computer Graphics
    Data::Point2D *p = static_cast<Data::Point2D *>(this->v->getPoint());

    Data::Edge2D *edge = static_cast<Data::Edge2D *>(fel->getShape());
    Data::EVector2D vec1 = vec;
    Data::EVector2D vec2(static_cast<Data::Point2D *>(edge->getP1())->epoint2D(), static_cast<Data::Point2D *>(edge->getP2())->epoint2D());
    Data::EVector2D delta(p->epoint2D(), static_cast<Data::Point2D *>(edge->getP1())->epoint2D());

    Data::EReal cross = vec1.cross(vec2);

    IntersectionSet intersections;

    if (cross == 0.0)
    {
        //retas paralelas
        Data::EReal cross1 = delta.cross(vec2);

        if (cross1 == 0.0)
        {
            Data::EFReal t0(vec1.dot(delta), vec1.dot(vec1));
            Data::EFReal t1 = t0 + Data::EFReal(vec1.dot(vec2), vec1.dot(vec1));

            Data::EFReal tmin = (t0 <= t1) ? t0 : t1;
            Data::EFReal tmax = (t0 <= t1) ? t1 : t0;

            if (tmax >= 0.0)
            {
                Data::EFReal smin(0.0), smax(1.0);

                if (tmin < 0.0)
                {
                    if (tmax == tmin)
                    {
                        smin = 1.0;
                    }
                    else
                    {
                        smin = -tmin;
                        smin /= tmax - tmin;
                    }

                    tmin = 0.0;
                }

                //if (tmin > 0.0)
                {
                    Intersection i;
                    i.t = tmin;
                    intersections.insert(i);

                    if (addToVisibility)
                    {
                        EdgeIntersection ei;
                        ei.s = smin;
                        ei.vtx = vtx;
                        ei.prevPos = ei.pos = pos;
                        this->addToVisibility(fel, ei);
                    }

                    /*if (vtx->v)
                    {
                        std::cout << "debug 2, vtx->v " << vtx->v->getId() << " tmin " << tmin << ", fel " << fel->getId() << std::endl;
                        std::cout << "debug 2, vtx->v " << vtx->v->getId() << " tmin " << static_cast<Real>(tmin) << ", fel " << fel->getId() << std::endl;
                        std::cout << "smin = " << smin << ", pos = " << ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : ((pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                        std::cout << "smin = " << static_cast<Real>(smin) << ", pos = " << ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : ((pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }*/
                }

                if (tmin != tmax)
                {
                    Intersection i;
                    i.t = tmax;
                    intersections.insert(i);

                    if (addToVisibility)
                    {
                        EdgeIntersection ei;
                        ei.s = smax;
                        ei.vtx = vtx;
                        ei.prevPos = ei.pos = pos;
                        this->addToVisibility(fel, ei);
                    }

                    /*if (vtx->v)
                    {
                        std::cout << "debug 2, vtx->v " << vtx->v->getId() << " tmax " << tmax << ", fel " << fel->getId() << std::endl;
                        std::cout << "debug 2, vtx->v " << vtx->v->getId() << " tmax " << static_cast<Real>(tmax) << ", fel " << fel->getId() << std::endl;
                        std::cout << "smax = " << smax << ", pos = " << ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : ((pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                        std::cout << "smax = " << static_cast<Real>(smax) << ", pos = " << ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : ((pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                    }*/
                }
            }
        }
    }
    else
    {
        Data::EReal cross1 = delta.cross(vec2);
        Data::EReal cross2 = delta.cross(vec1);

        Data::EFReal t(cross1, cross);
        Data::EFReal s(cross2, cross);

        if ((t > 0.0) && (s >= 0.0) && (s <= 1.0))
        {
            Intersection i;
            i.t = t;
            intersections.insert(i);

            if (addToVisibility)
            {
                EdgeIntersection ei;
                ei.s = s;
                ei.vtx = vtx;
                ei.prevPos = ei.pos = pos;
                this->addToVisibility(fel, ei);
            }

            /*if (vtx->v)
            {
                std::cout << "debug 1, vtx->v " << vtx->v->getId() << " t " << t << ", fel " << fel->getId() << std::endl;
                std::cout << "debug 1, vtx->v " << vtx->v->getId() << " t " << static_cast<Real>(t) << ", fel " << fel->getId() << std::endl;
                std::cout << "s = " << s << ", pos = " << ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : ((pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
                std::cout << "s = " << static_cast<Real>(s) << ", pos = " << ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : ((pos == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
            }*/
        }
    }

    return intersections;
}

void Tools::VisibilityIdentifier::addToVisibility(MJMesh::FrontElement2D *fel, EdgeIntersection &ei)
{
    std::pair<EdgeIntersectionSet::iterator, bool> p = this->edgeVisibility[fel].insert(ei);

    if (!p.second)
    {
        if ((((*p.first).pos == Data::IN) && (ei.pos == Data::ON))  ||
            (((*p.first).pos == Data::IN) && (ei.pos == Data::OUT)) ||
            (((*p.first).pos == Data::ON) && (ei.pos == Data::OUT)))
        {
            (*p.first).pos = ei.pos;
            (*p.first).prevPos = ei.prevPos;
            (*p.first).vtx = ei.vtx;
        }
    }
}
