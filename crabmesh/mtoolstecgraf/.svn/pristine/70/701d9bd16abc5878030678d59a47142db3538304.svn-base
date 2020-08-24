#include "Tools/VisibilityBasedVertexFinder.h"

#include "Data/Point2D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/Boundary.h"
#include "Tools/FrontElementVisibilityIdentifier.h"

using namespace Tools;

Tools::VisibilityBasedVertexFinder::VisibilityBasedVertexFinder(const Tool *tool) :
    Tools::ExternalTool::ExternalTool(tool)
{
    this->searchVertex = true;

    this->vFound = false;
    this->vertex = 0;

    this->feFound = false;
    this->fel = 0;
    this->param = -1.0;
}

Tools::VisibilityBasedVertexFinder::~VisibilityBasedVertexFinder()
{

}

void Tools::VisibilityBasedVertexFinder::addInput(MJMesh::Boundary *boundary, MJMesh::FrontElement2D *fel)
{
    this->addInput(boundary, fel, fel->getShape()->mid2D());
}

void Tools::VisibilityBasedVertexFinder::addInput(MJMesh::Boundary *boundary, MJMesh::FrontElement2D *fel, const Data::Point2D &aux)
{
    this->boundaries.push_back(boundary);
    this->frontElements.push_back(fel);
    this->points.push_back(aux);
}

#if USE_GUI
void Tools::VisibilityBasedVertexFinder::addInput(MJMesh::Boundary *boundary, MJMesh::FrontElement2D *fel, MJMesh::Mesh *mesh)
{
    this->addInput(boundary, fel);
    this->meshes.push_back(mesh);
}

void Tools::VisibilityBasedVertexFinder::addInput(MJMesh::Boundary *boundary, MJMesh::FrontElement2D *fel, const Data::Point2D &aux, MJMesh::Mesh *mesh)
{
    this->addInput(boundary, fel, aux);
    this->meshes.push_back(mesh);
}
#endif //#if USE_GUI

void Tools::VisibilityBasedVertexFinder::setSearchVertex(bool searchVertex)
{
    this->searchVertex = searchVertex;
}

bool Tools::VisibilityBasedVertexFinder::found() const
{
    return (this->foundVertex() || this->foundFrontElement());
}

bool Tools::VisibilityBasedVertexFinder::foundVertex() const
{
    return this->vFound;
}

ULInt Tools::VisibilityBasedVertexFinder::getFoundVertex() const
{
    return this->vertex;
}

bool Tools::VisibilityBasedVertexFinder::foundFrontElement() const
{
    return this->feFound;
}

ULInt Tools::VisibilityBasedVertexFinder::getFoundFrontElement() const
{
    return this->fel;
}

Real Tools::VisibilityBasedVertexFinder::getFoundFrontElementParameter() const
{
    return this->param;
}

void Tools::VisibilityBasedVertexFinder::execute()
{
    if ((this->boundaries.empty()) || (this->frontElements.empty()) || (this->points.empty()) ||
        (this->boundaries.size() != this->frontElements.size()) || (this->boundaries.size() != this->points.size()))
    {
        return;
    }

#if USE_GUI
    {
        Data::BoundaryList::iterator biter = this->boundaries.begin();

        for (Data::MeshList::iterator iter = this->meshes.begin();
             iter != this->meshes.end(); iter++, biter++)
        {
            for (Data::Mesh::VertexIterator iter2 = (*iter)->vBegin();
                 iter2 != (*iter)->vEnd();)
            {
                Data::Vertex *v = (*iter2);

                if ((*biter)->have(v))
                {
                    v->setColor(0.0, 0.0, 0.0);

                    iter2++;

                    continue;
                }

                iter2 = (*iter)->erase(iter2);

                delete v;
            }

            for (Data::Mesh::FrontElementIterator iter2 = (*iter)->feBegin();
                 iter2 != (*iter)->feEnd(); iter2++)
            {
                (*iter2)->setColor(0.0, 0.0, 0.0);
            }
        }
    }
#endif //#if USE_GUI

    {
        Data::FrontElementList::iterator iter2 = this->frontElements.begin();

        for (Data::BoundaryList::iterator iter = this->boundaries.begin();
             iter != this->boundaries.end(); iter++, iter2++)
        {
            this->visibilities.push_back(this->findVisibility((*iter), (*iter2)));

            if ((this->visibilities.back().vertices.empty()) && (this->visibilities.back().edges.empty()))
            {
                return;
            }
        }
    }

    /*if (this->visibilities.size() == 1)
    {
        this->intersections.push_back(this->visibilities.front());

        if (!this->intersections.front().vertices.empty())
        {
            this->findBestVertex();
        }

        if (!this->foundVertex())
        {
            this->findBestEdge();
        }
    }
    else
    {
        VisibilityList::iterator minVerts, minEdges;
        minVerts = minEdges = this->visibilities.begin();

        for (VisibilityList::iterator iter = ++this->visibilities.begin();
             iter != this->visibilities.end(); iter++)
        {
            if ((*iter).vertices.size() < (*minVerts).vertices.size())
            {
                minVerts = iter;
            }

            if ((*iter).edges.size() < (*minEdges).edges.size())
            {
                minEdges = iter;
            }
        }

        this->intersections.resize(this->visibilities.size());

        bool foundAnyVertex = false;

        for (Data::VertexList::iterator iter = (*minVerts).vertices.begin();
             iter != (*minVerts).vertices.end(); iter++)
        {
            Data::Vertex *v = (*iter);

            Data::VertexList foundVertices;

            for (VisibilityList::iterator iter2 = this->visibilities.begin();
                 iter2 != this->visibilities.end(); iter2++)
            {
                if (iter2 == minVerts)
                {
                    foundVertices.push_back(v);

                    continue;
                }

                bool found = false;

                for (Data::VertexList::iterator iter3 = (*iter2).vertices.begin();
                     iter3 != (*iter2).vertices.end(); iter3++)
                {
                    if ((*iter3)->getId() == v->getId())
                    {
                        found = true;

                        foundVertices.push_back((*iter3));

                        break;
                    }
                }

                if (!found)
                {
                    foundVertices.clear();

                    break;
                }
            }

            if (!foundVertices.empty())
            {
                VisibilityList::iterator iter3 = this->intersections.begin();

                for (Data::VertexList::iterator iter2 = foundVertices.begin();
                     iter2 != foundVertices.end(); iter2++, iter3++)
                {
                    (*iter3).vertices.push_back((*iter2));
                }

                foundAnyVertex = true;
            }
        }

        if (foundAnyVertex)
        {
            this->findBestVertex();
        }

        if (!this->foundVertex())
        {
            for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter = (*minEdges).edges.begin();
                 iter != (*minEdges).edges.end(); iter++)
            {
                Data::FrontElement *fel = (*iter).fel;

                Tools::VisibilityIdentifier::EdgeVisibilityList foundEdges;

                for (VisibilityList::iterator iter2 = this->visibilities.begin();
                     iter2 != this->visibilities.end(); iter2++)
                {
                    if (iter2 == minEdges)
                    {
                        foundEdges.push_back((*iter));

                        continue;
                    }

                    bool found = false;

                    for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter3 = (*iter2).edges.begin();
                         iter3 != (*iter2).edges.end(); iter3++)
                    {
                        if ((*iter3).fel->getId() == fel->getId())
                        {
                            found = true;

                            foundEdges.push_back((*iter3));

                            break;
                        }
                    }

                    if (!found)
                    {
                        foundEdges.clear();

                        break;
                    }
                }

                if (!foundEdges.empty())
                {
                    FrontElementVisibilityIdentifier fevi;

                    Data::EFRealVector intersection = foundEdges.front().visibility;

                    for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter2 = ++foundEdges.begin();
                         iter2 != foundEdges.end(); iter2++)
                    {
                        intersection = fevi.mergeVisibilities(intersection, (*iter2).visibility);
                    }

                    VisibilityList::iterator iter3 = this->intersections.begin();

                    for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter2 = foundEdges.begin();
                         iter2 != foundEdges.end(); iter2++, iter3++)
                    {
                        Tools::VisibilityIdentifier::EdgeVisibility vis;
                        vis.fel = (*iter2).fel;
                        vis.visibility = intersection;

                        (*iter3).edges.push_back(vis);
                    }
                }
            }

            this->findBestEdge();
        }
    }*/

    if (this->visibilities.size() == 1)
    {
        this->intersections.push_back(this->visibilities.front());
    }
    else
    {
        VisibilityList::iterator minVerts, minEdges;
        minVerts = minEdges = this->visibilities.begin();

        for (VisibilityList::iterator iter = ++this->visibilities.begin();
             iter != this->visibilities.end(); iter++)
        {
            if ((*iter).vertices.size() < (*minVerts).vertices.size())
            {
                minVerts = iter;
            }

            if ((*iter).edges.size() < (*minEdges).edges.size())
            {
                minEdges = iter;
            }
        }

        this->intersections.resize(this->visibilities.size());

        for (Data::VertexList::iterator iter = (*minVerts).vertices.begin();
             iter != (*minVerts).vertices.end(); iter++)
        {
            Data::Vertex *v = (*iter);

            Data::VertexList foundVertices;

            for (VisibilityList::iterator iter2 = this->visibilities.begin();
                 iter2 != this->visibilities.end(); iter2++)
            {
                if (iter2 == minVerts)
                {
                    foundVertices.push_back(v);

                    continue;
                }

                bool found = false;

                for (Data::VertexList::iterator iter3 = (*iter2).vertices.begin();
                     iter3 != (*iter2).vertices.end(); iter3++)
                {
                    if ((*iter3)->getId() == v->getId())
                    {
                        found = true;

                        foundVertices.push_back((*iter3));

                        break;
                    }
                }

                if (!found)
                {
                    foundVertices.clear();

                    break;
                }
            }

            if (!foundVertices.empty())
            {
                VisibilityList::iterator iter3 = this->intersections.begin();

                for (Data::VertexList::iterator iter2 = foundVertices.begin();
                     iter2 != foundVertices.end(); iter2++, iter3++)
                {
                    (*iter3).vertices.push_back((*iter2));
                }
            }
        }

        for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter = (*minEdges).edges.begin();
             iter != (*minEdges).edges.end(); iter++)
        {
            Data::FrontElement *fel = (*iter).fel;

            Tools::VisibilityIdentifier::EdgeVisibilityList foundEdges;

            for (VisibilityList::iterator iter2 = this->visibilities.begin();
                 iter2 != this->visibilities.end(); iter2++)
            {
                if (iter2 == minEdges)
                {
                    foundEdges.push_back((*iter));

                    continue;
                }

                bool found = false;

                for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter3 = (*iter2).edges.begin();
                     iter3 != (*iter2).edges.end(); iter3++)
                {
                    if ((*iter3).fel->getId() == fel->getId())
                    {
                        found = true;

                        foundEdges.push_back((*iter3));

                        break;
                    }
                }

                if (!found)
                {
                    foundEdges.clear();

                    break;
                }
            }

            if (!foundEdges.empty())
            {
                FrontElementVisibilityIdentifier fevi;

                Data::EFRealVector intersection = foundEdges.front().visibility;

                for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter2 = ++foundEdges.begin();
                     iter2 != foundEdges.end(); iter2++)
                {
                    intersection = fevi.mergeVisibilities(intersection, (*iter2).visibility);
                }

                VisibilityList::iterator iter3 = this->intersections.begin();

                for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter2 = foundEdges.begin();
                     iter2 != foundEdges.end(); iter2++, iter3++)
                {
                    Tools::VisibilityIdentifier::EdgeVisibility vis;
                    vis.fel = (*iter2).fel;
                    vis.visibility = intersection;

                    (*iter3).edges.push_back(vis);
                }
            }
        }
    }

    this->findNextBestVertexOrEdge();
}

void Tools::VisibilityBasedVertexFinder::findNextBestVertexOrEdge()
{
    if (this->vFound)
    {
        for (VisibilityList::iterator iter = this->intersections.begin();
             iter != this->intersections.end(); iter++)
        {
            for (Data::VertexList::iterator iter2 = (*iter).vertices.begin();
                 iter2 != (*iter).vertices.end(); iter2++)
            {
                if ((*iter2)->getId() != this->vertex)
                {
                    continue;
                }

                (*iter).vertices.erase(iter2);

                break;
            }
        }
    }

    if (this->feFound)
    {
        for (VisibilityList::iterator iter = this->intersections.begin();
             iter != this->intersections.end(); iter++)
        {
            for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter2 = (*iter).edges.begin();
                 iter2 != (*iter).edges.end(); iter2++)
            {
                if ((*iter2).fel->getId() != this->fel)
                {
                    continue;
                }

                for (Data::EFRealVector::iterator iter3 = (*iter2).visibility.begin();
                     iter3 != (*iter2).visibility.end(); iter3++)
                {
                    Data::EFReal t1 = (*iter3);
                    iter3++;
                    Data::EFReal t2 = (*iter3);

                    if ((this->param >= t1) && (this->param <= t2))
                    {
                        iter3--;

                        (*iter2).visibility.erase(iter3, iter3 + 2);

                        break;
                    }
                }

                if ((*iter2).visibility.empty())
                {
                    (*iter).edges.erase(iter2);
                }

                break;
            }
        }
    }

    this->vFound = false;
    this->vertex = 0;

    this->feFound = false;
    this->fel = 0;
    this->param = -1.0;

    if (!this->intersections.front().vertices.empty())
    {
        this->findBestVertex();
    }

    if (!this->foundVertex())
    {
        this->findBestEdge();
    }
}

FrontElementVisibilityIdentifier *Tools::VisibilityBasedVertexFinder::makeFEVisibilityIdentifier() const
{
    return new Tools::FrontElementVisibilityIdentifier();
}

Tools::VisibilityIdentifier::Visibility Tools::VisibilityBasedVertexFinder::findVisibility(Data::Boundary *boundary, Data::FrontElement *fel) const
{
    if (!boundary->have(fel))
    {
        return Tools::VisibilityIdentifier::Visibility();
    }

    FrontElementVisibilityIdentifier *identifier = this->makeFEVisibilityIdentifier();

    identifier->setBoundary(static_cast<MJMesh::Boundary *>(boundary));
    identifier->setFrontElement(static_cast<MJMesh::FrontElement2D *>(fel));

    identifier->execute();

    Tools::VisibilityIdentifier::Visibility vis = identifier->getVisibility();

    delete identifier;

    return vis;
}

void Tools::VisibilityBasedVertexFinder::findBestVertex()
{
    this->vFound = false;
    this->vertex = 0;

    if ((!this->searchVertex) || (this->intersections.front().vertices.empty()))
    {
        return;
    }

    if (this->intersections.front().vertices.size() == 1)
    {
        this->vFound = true;
        this->vertex = this->intersections.front().vertices.front()->getId();

#if USE_GUI
        for (VisibilityList::iterator iter = this->intersections.begin();
             iter != this->intersections.end(); iter++)
        {
            (*iter).vertices.front()->setColor(0.0, 1.0, 0.0);
        }
#endif //#if USE_GUI

        return;
    }

    VLList vertices;

    for (VisibilityList::iterator iter = this->intersections.begin();
         iter != this->intersections.end(); iter++)
    {
        vertices.push_back(Data::VertexList());

        for (Data::VertexList::iterator iter3 = (*iter).vertices.begin();
             iter3 != (*iter).vertices.end(); iter3++)
        {
            vertices.back().push_back((*iter3));
        }
    }

    this->vFound = true;
    this->vertex = this->best(vertices);
}

void Tools::VisibilityBasedVertexFinder::findBestEdge()
{
    this->feFound = false;
    this->fel = 0;
    this->param = -1.0;

    if (this->intersections.front().edges.empty())
    {
        return;
    }

    VLList vertices;

    for (VisibilityList::iterator iter = this->intersections.begin();
         iter != this->intersections.end(); iter++)
    {
        vertices.push_back(Data::VertexList());

        ULInt id = 1;

        for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter3 = (*iter).edges.begin();
             iter3 != (*iter).edges.end(); iter3++)
        {
            Data::EPoint2D p1 = static_cast<Data::Point2D *>((*iter3).fel->getVertex(0)->getPoint())->epoint2D();
            Data::EPoint2D p2 = static_cast<Data::Point2D *>((*iter3).fel->getVertex(1)->getPoint())->epoint2D();
            Data::EVector2D v(p1, p2);

            for (UInt i = 0; i < (*iter3).visibility.size(); i += 2)
            {
                Data::EFReal t1 = (*iter3).visibility[i];
                Data::EFReal t2 = (*iter3).visibility[i + 1];

                Real t = 0.5*(t1 + t2);

                Real x = p1.getCoord(0) + t*v.getCoord(0);
                Real y = p1.getCoord(1) + t*v.getCoord(1);

                vertices.back().push_back(new MJMesh::Vertex(x, y, id));

                id++;
            }
        }
    }

    this->feFound = true;
    ULInt fel = this->best(vertices);

    ULInt id = 1;

    for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter3 = this->intersections.front().edges.begin();
         iter3 != this->intersections.front().edges.end(); iter3++)
    {
        if ((fel >= id) && (fel < id + (*iter3).visibility.size()/2))
        {
            this->fel = (*iter3).fel->getId();

            Data::EFReal t1 = (*iter3).visibility[2*(fel - id)];
            Data::EFReal t2 = (*iter3).visibility[2*(fel - id) + 1];

            this->param = 0.5*(t1 + t2);

            break;
        }

        id += (*iter3).visibility.size()/2;
    }

#if USE_GUI
    Data::MeshList::iterator miter = this->meshes.begin();
#endif //#if USE_GUI

    while (!vertices.empty())
    {
        while (!vertices.front().empty())
        {
#if USE_GUI
            if ((miter != this->meshes.end()) && (vertices.front().front()->getId() == id))
            {
                (*miter)->add(vertices.front().front());

                vertices.front().pop_front();

                continue;
            }
#endif//#if USE_GUI

            delete vertices.front().front();

            vertices.front().pop_front();
        }

        vertices.pop_front();

#if USE_GUI
        miter++;
#endif //#if USE_GUI
    }
}

ULInt Tools::VisibilityBasedVertexFinder::best(const VLList &vertices) const
{
    Data::RealVector sumDists(vertices.front().size(), 0.0);

    Data::Point2DObjList::const_iterator piter = this->points.begin();

    for (VLList::const_iterator iter = vertices.begin();
         iter != vertices.end(); iter++, piter++)
    {
        UInt i = 0;

        for (Data::VertexList::const_iterator iter2 = (*iter).begin();
             iter2 != (*iter).end(); iter2++, i++)
        {
            Real dist = (*iter2)->getPoint()->distance((*piter));

            sumDists[i] += dist;
        }
    }

    UInt min = 0;

    for (UInt i = 1; i < sumDists.size(); i++)
    {
        if (sumDists[i] < sumDists[min])
        {
            min = i;
        }
    }

#if USE_GUI
    {
        for (VLList::const_iterator iter = vertices.begin();
             iter != vertices.end(); iter++)
        {
            Data::VertexList::const_iterator iter2 = (*iter).begin();

            std::advance(iter2, min);

            (*iter2)->setColor(0.0, 1.0, 0.0);
        }
    }
#endif //#if USE_GUI

    Data::VertexList::const_iterator iter = vertices.front().begin();

    std::advance(iter, min);

    return (*iter)->getId();
}
