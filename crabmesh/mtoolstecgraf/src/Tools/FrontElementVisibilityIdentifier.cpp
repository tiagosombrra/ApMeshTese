#include "Tools/FrontElementVisibilityIdentifier.h"

#include "Data/GeometricShape.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/Boundary.h"

#include "Performer/SequentialIdManager.h"

using namespace Tools;

Tools::FrontElementVisibilityIdentifier::FrontElementVisibilityIdentifier(const Tool *tool) :
    Tools::ExternalTool::ExternalTool(tool)
{
    this->boundary = NULL;
    this->fel = NULL;

#if USE_GUI
    this->mesh = NULL;
    this->idManager = NULL;
#endif //#if USE_GUI
}

Tools::FrontElementVisibilityIdentifier::~FrontElementVisibilityIdentifier()
{
#if USE_GUI
    if (this->idManager)
    {
        delete this->idManager;
    }
#endif //#if USE_GUI
}

#if USE_GUI
void Tools::FrontElementVisibilityIdentifier::setMesh(MJMesh::Mesh *mesh)
{
    this->mesh = mesh;
}
#endif //#if USE_GUI

void Tools::FrontElementVisibilityIdentifier::setBoundary(const MJMesh::Boundary *boundary)
{
    this->boundary = boundary;
}

void Tools::FrontElementVisibilityIdentifier::setFrontElement(const MJMesh::FrontElement *fel)
{
    this->fel = fel;
}

Tools::VisibilityIdentifier::Visibility Tools::FrontElementVisibilityIdentifier::getVisibility() const
{
    return this->vis;
}

void Tools::FrontElementVisibilityIdentifier::execute()
{
    if ((!this->boundary) ||
        (!this->fel) ||
        (!this->boundary->have(this->fel)) ||
        (this->fel->getShape()->dimension() != 2))
    {
        return;
    }

    this->vis1 = this->findVisibility(static_cast<MJMesh::Vertex *>(this->fel->getVertex(0)));
    this->vis2 = this->findVisibility(static_cast<MJMesh::Vertex *>(this->fel->getVertex(1)));

    this->vis = this->mergeVisibilities(this->vis1, this->vis2);

#if USE_GUI
    if (this->mesh)
    {
        this->idManager = new Performer::SequentialIdManager(1, 2);

        for (Data::Mesh::FrontElementIterator iter = this->mesh->feBegin();
             iter != this->mesh->feEnd();)
        {
            Data::FrontElement *fel = (*iter);

            if (this->boundary->have(fel))
            {
                (*iter)->setColor(0.0, 0.0, 0.0);

                if ((*iter)->getId() >= this->idManager->getId(1))
                {
                    this->idManager->setId(1, (*iter)->getId() + 1);
                }

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
                (*iter)->setColor(0.0, 0.0, 0.0);

                if ((*iter)->getId() >= this->idManager->getId(0))
                {
                    this->idManager->setId(0, (*iter)->getId() + 1);
                }

                iter++;

                continue;
            }

            iter = this->mesh->erase(iter);

            delete v;
        }

        for (Data::VertexList::iterator iter = this->vis.vertices.begin();
             iter != this->vis.vertices.end(); iter++)
        {
            (*iter)->setColor(0.0, 1.0, 0.0);
        }

        for (Tools::VisibilityIdentifier::EdgeVisibilityList::iterator iter = this->vis.edges.begin();
             iter != this->vis.edges.end(); iter++)
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
}

Tools::VisibilityIdentifier::Visibility Tools::FrontElementVisibilityIdentifier::findVisibility(const MJMesh::Vertex *v) const
{
    Tools::VisibilityIdentifier identifier;

    identifier.setBoundary(this->boundary);
    identifier.setVertex(v, static_cast<const MJMesh::FrontElement2D *>(this->fel));

    identifier.execute();

    return identifier.getVisibility();
}

Tools::VisibilityIdentifier::Visibility Tools::FrontElementVisibilityIdentifier::mergeVisibilities(
    const Tools::VisibilityIdentifier::Visibility &vis1, const Tools::VisibilityIdentifier::Visibility &vis2) const
{
    Tools::VisibilityIdentifier::Visibility vis;

    Data::VertexHash vertices1;

    for (Data::VertexList::const_iterator iter = vis1.vertices.begin();
         iter != vis1.vertices.end(); iter++)
    {
        vertices1.insert((*iter));
    }

    for (Data::VertexList::const_iterator iter = vis2.vertices.begin();
         iter != vis2.vertices.end(); iter++)
    {
        if (vertices1.find((*iter)) != vertices1.end())
        {
            vis.vertices.push_back((*iter));
        }
    }

    typedef std::unordered_map<Data::FrontElement *, Data::EFRealVector> FEEFRealVectorMap;

    FEEFRealVectorMap edges1;

    for (Tools::VisibilityIdentifier::EdgeVisibilityList::const_iterator iter = vis1.edges.begin();
         iter != vis1.edges.end(); iter++)
    {
        if ((*iter).fel == this->fel)
        {
            continue;
        }

        edges1[(*iter).fel] = (*iter).visibility;
    }

    for (Tools::VisibilityIdentifier::EdgeVisibilityList::const_iterator iter = vis2.edges.begin();
         iter != vis2.edges.end(); iter++)
    {
        if ((*iter).fel == this->fel)
        {
            continue;
        }

        FEEFRealVectorMap::iterator iter2 = edges1.find((*iter).fel);

        if (iter2 == edges1.end())
        {
            continue;
        }

        Tools::VisibilityIdentifier::EdgeVisibility evis;

        evis.fel = (*iter).fel;
        evis.visibility = this->mergeVisibilities((*iter2).second, (*iter).visibility);

        vis.edges.push_back(evis);
    }

    return vis;
}

Data::EFRealVector Tools::FrontElementVisibilityIdentifier::mergeVisibilities(const Data::EFRealVector &vis1, const Data::EFRealVector &vis2) const
{
    if ((vis1.empty()) || (vis2.empty()))
    {
        return Data::EFRealVector();
    }

    struct EFRealPos
    {
        Data::EFReal t;
        Data::Position pos;
    };

    typedef std::list<EFRealPos> EFRealPosList;

    EFRealPosList v1, v2;

    for (UInt i = 0; i != vis1.size(); i++)
    {
        if ((i == 0) && (vis1[i] != 0.0))
        {
            EFRealPos rp;

            rp.t = 0.0;
            rp.pos = Data::OUT;

            v1.push_back(rp);
        }

        EFRealPos rp;

        rp.t = vis1[i];
        rp.pos = ((i%2) == 0) ? Data::IN : Data::OUT;

        v1.push_back(rp);

        if ((i == vis1.size() - 1) && (vis1[i] != 1.0))
        {
            EFRealPos rp;

            rp.t = 1.0;
            rp.pos = Data::OUT;

            v1.push_back(rp);
        }
    }

    for (UInt i = 0; i != vis2.size(); i++)
    {
        if ((i == 0) && (vis2[i] != 0.0))
        {
            EFRealPos rp;

            rp.t = 0.0;
            rp.pos = Data::OUT;

            v2.push_back(rp);
        }

        EFRealPos rp;

        rp.t = vis2[i];
        rp.pos = ((i%2) == 0) ? Data::IN : Data::OUT;

        v2.push_back(rp);

        if ((i == vis2.size() - 1) && (vis2[i] != 1.0))
        {
            EFRealPos rp;

            rp.t = 1.0;
            rp.pos = Data::OUT;

            v2.push_back(rp);
        }
    }

    EFRealPosList v;

    EFRealPosList::iterator iter1 = v1.begin();
    EFRealPosList::iterator iter2 = v2.begin();

    while ((iter1 != v1.end()) || (iter2 != v2.end()))
    {
        EFRealPos p;

        if (iter1 == v1.end())
        {
            p.t = (*iter2).t;

            iter2++;
        }
        else if (iter2 == v2.end())
        {
            p.t = (*iter1).t;

            iter1++;
        }
        else
        {
            if ((*iter1).t == (*iter2).t)
            {
                p.t = (*iter1).t;

                iter1++;
                iter2++;
            }
            else if ((*iter1).t < (*iter2).t)
            {
                p.t = (*iter1).t;

                iter1++;
            }
            else
            {
                p.t = (*iter2).t;

                iter2++;
            }
        }

        p.pos = Data::UNKNOWN;

        v.push_back(p);
    }

    iter1 = v1.begin();
    iter2 = v2.begin();

    EFRealPosList::iterator iter = v.begin();

    while (true)
    {
        (*iter).pos = this->intersection((*iter1).pos, (*iter2).pos);

        iter++;

        if (iter == v.end())
        {
            break;
        }

        iter1++;

        if ((*iter1).t > (*iter).t)
        {
            iter1--;
        }

        iter2++;

        if ((*iter2).t > (*iter).t)
        {
            iter2--;
        }
    }

    Data::EFRealVector vis;

    Data::Position oldPos = Data::UNKNOWN;

    for (iter = v.begin(); iter != v.end(); iter++)
    {
        Data::Position pos = (*iter).pos;

        if (((oldPos != Data::IN) && (pos == Data::IN)) ||
            ((oldPos == Data::IN) && (pos != Data::IN)))
        {
            vis.push_back((*iter).t);
        }

        oldPos = pos;
    }

    return vis;
}

Data::Position Tools::FrontElementVisibilityIdentifier::intersection(Data::Position pos1, Data::Position pos2) const
{
    return ((pos1 == Data::IN) && (pos2 == Data::IN)) ? Data::IN : Data::OUT;
}
