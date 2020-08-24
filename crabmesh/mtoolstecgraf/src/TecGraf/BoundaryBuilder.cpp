#include "TecGraf/BoundaryBuilder.h"

#if USE_TECGRAF

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement3D.h"
#include "TecGraf/Region.h"
#include "TecGraf/OrientedSurface.h"
#include "TecGraf/Surface.h"

using namespace TecGraf;

TecGraf::BoundaryBuilder::BoundaryBuilder(bool sorted) :
	MJMesh::BoundaryBuilder::BoundaryBuilder(sorted)
{
	this->region = NULL;

	this->lastIdVertex = 0;
    this->lastIdFrontElement = 0;
}

TecGraf::BoundaryBuilder::~BoundaryBuilder()
{

}

void TecGraf::BoundaryBuilder::setRegion(Region *region)
{
	this->region = region;
}

const Data::ULIntMap &TecGraf::BoundaryBuilder::getVIdMap() const
{
    return this->vidMap;
}

const Data::ULIntMap &TecGraf::BoundaryBuilder::getFEIdMap() const
{
    return this->feidMap;
}

void TecGraf::BoundaryBuilder::clear()
{
	this->region = NULL;

	MJMesh::BoundaryBuilder::clear();
}

void TecGraf::BoundaryBuilder::execute()
{
	if (!this->region)
	{
		return;
	}

	for (OrientedSurfaceList::iterator iter = this->region->begin();
		 iter != this->region->end(); iter++)
	{
		OrientedSurface *s = (*iter);

		for (Data::Boundary::VertexIterator iter2 = s->getSurface()->vBegin();
			 iter2 != s->getSurface()->vEnd(); iter2++)
		{
			Data::Vertex *v = (*iter2);

			if (this->verticesMap.find(v->getId()) != this->verticesMap.end())
			{
				continue;
			}

#if USE_GUI
			//Data::Vertex *newv =
#endif //#if USE_GUI
				this->add(v->getId(), v->getPoint()->getCoord(0), v->getPoint()->getCoord(1), v->getPoint()->getCoord(2));

#if USE_GUI
			//newv->setColor(s->getR(), s->getG(), s->getB());
#endif //#if USE_GUI
		}

		bool positive = s->isPositive();

		for (Data::Boundary::FrontElementIterator iter2 = s->getSurface()->eBegin();
			 iter2 != s->getSurface()->eEnd(); iter2++)
		{
			Data::FrontElement *fel = (*iter2);

#if USE_QUADS
			if (fel->numVertices() == 4)
			{
#if USE_GUI
				//Data::FrontElement *newfel = NULL;
#endif //#if USE_GUI

				if (positive)
				{
#if USE_GUI
					//newfel =
#endif //#if USE_GUI
						this->add(fel->getId(), fel->getVertex(0)->getId(), fel->getVertex(1)->getId(), fel->getVertex(2)->getId(), fel->getVertex(3)->getId());
				}
				else
				{
#if USE_GUI
					//newfel =
#endif //#if USE_GUI
						this->add(fel->getId(), fel->getVertex(0)->getId(), fel->getVertex(3)->getId(), fel->getVertex(2)->getId(), fel->getVertex(1)->getId());
				}

#if USE_GUI
				//newfel->setColor(s->getR(), s->getG(), s->getB());
#endif //#if USE_GUI

				continue;
			}
#endif //#if USE_QUADS

#if USE_GUI
			//Data::FrontElement *newfel = NULL;
#endif //#if USE_GUI

			if (positive)
			{
#if USE_GUI
				//newfel =
#endif //#if USE_GUI
					this->add(fel->getId(), fel->getVertex(0)->getId(), fel->getVertex(1)->getId(), fel->getVertex(2)->getId());
			}
			else
			{
#if USE_GUI
				//newfel =
#endif //#if USE_GUI
					this->add(fel->getId(), fel->getVertex(0)->getId(), fel->getVertex(2)->getId(), fel->getVertex(1)->getId());
			}

#if USE_GUI
			//newfel->setColor(s->getR(), s->getG(), s->getB());
#endif //#if USE_GUI
		}
	}

	this->adjustForCracks();

	for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
         iter != this->boundary->vEnd(); iter++)
    {
        this->vidMap[++this->lastIdVertex] = (*iter)->getId();

        (*iter)->setId(this->lastIdVertex);
    }

    for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin();
         iter != this->boundary->eEnd(); iter++)
    {
        this->feidMap[++this->lastIdFrontElement] = (*iter)->getId();

        (*iter)->setId(this->lastIdFrontElement);
    }

	MJMesh::BoundaryBuilder::execute();
}

bool TecGraf::BoundaryBuilder::buildMap(EdgeFrontElementMap &eMap) const
{
    bool shouldMakeCrack = false;

    for (Data::Boundary::VertexIterator iter = this->boundary->vBegin();
         iter != this->boundary->vEnd(); iter++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

        for (Data::FrontElementList::iterator iter2 = v->feBegin();
             iter2 != v->feEnd(); iter2++)
        {
            MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter2));

            UInt i = 0;

            for (; i < fel->numVertices(); i++)
            {
                if (fel->getVertex(i) == v)
                {
                    break;
                }
            }

            if (i >= fel->numVertices())
            {
                continue;
            }

            UInt j1 = (i + 1)%fel->numVertices();
            UInt j2 = (i + fel->numVertices() - 1)%fel->numVertices();

            MJMesh::Vertex *o1 = static_cast<MJMesh::Vertex *>(fel->getVertex(j1));
            MJMesh::Vertex *o2 = static_cast<MJMesh::Vertex *>(fel->getVertex(j2));

            if (std::find(eMap[v][o1].begin(), eMap[v][o1].end(), fel) == eMap[v][o1].end())
            {
                eMap[v][o1].push_back(fel);
            }

            if (std::find(eMap[o1][v].begin(), eMap[o1][v].end(), fel) == eMap[o1][v].end())
            {
                eMap[o1][v].push_back(fel);
            }

            if (std::find(eMap[v][o2].begin(), eMap[v][o2].end(), fel) == eMap[v][o2].end())
            {
                eMap[v][o2].push_back(fel);
            }

            if (std::find(eMap[o2][v].begin(), eMap[o2][v].end(), fel) == eMap[o2][v].end())
            {
                eMap[o2][v].push_back(fel);
            }
        }
    }

    for (EdgeFrontElementMap::iterator iter = eMap.begin();
         iter != eMap.end(); iter++)
    {
        for (VertexFrontElementListMap::iterator iter2 = (*iter).second.begin();
             iter2 != (*iter).second.end(); iter2++)
        {
            if ((*iter2).second.size() > 2)
            {
                //(*iter).first->setColor(1.0, 0.0, 0.0);
                //(*iter).first->highlight();

                //(*iter2).first->setColor(1.0, 0.0, 0.0);
                //(*iter2).first->highlight();

/*//debug
                MJMesh::FrontElement3D *newFel = new MJMesh::FrontElement3D(
                    static_cast<MJMesh::Vertex *>((*iter).first), static_cast<MJMesh::Vertex *>((*iter).first), static_cast<MJMesh::Vertex *>((*iter2).first));

                this->boundary->add(newFel);

                newFel->setColor(1.0, 0.0, 0.0);
                newFel->highlight();
//endebug*/

                shouldMakeCrack = true;
            }
        }
    }

    return shouldMakeCrack;
}

bool TecGraf::BoundaryBuilder::makeCracks(const Surface *s, const EdgeFrontElementMap &eMap)
{
    Data::VertexMap svMap;

    typedef std::map<ULInt, Data::VertexList> VListMap;
    VListMap evMap;

    for (Surface::ConstVertexIterator iter = s->vBegin();
         iter != s->vEnd(); iter++)
    {
        svMap[(*iter)->getId()] = (*iter);
    }

    for (EdgeFrontElementMap::const_iterator iter = eMap.begin();
         iter != eMap.end(); iter++)
    {
        Data::Vertex *v = (*iter).first;

        if (svMap.find(v->getId()) != svMap.end())
        {
            evMap[v->getId()].push_back(v);
        }
    }

    if (evMap.empty())
    {
        return false;
    }

    Data::FrontElementMap sfeMap;

    for (Surface::ConstFrontElementIterator iter = s->eBegin();
         iter != s->eEnd(); iter++)
    {
        sfeMap[(*iter)->getId()] = (*iter);
    }

    typedef std::map<ULInt, Data::FrontElementList> FrontElementListMap;

    FrontElementListMap efeMap;

    for (VListMap::const_iterator iter = evMap.begin();
         iter != evMap.end(); iter++)
    {
        for (Data::VertexList::const_iterator iter3 = (*iter).second.begin();
             iter3 != (*iter).second.end(); iter3++)
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter3));

            for (Data::FrontElementList::iterator iter2 = v->feBegin();
                 iter2 != v->feEnd(); iter2++)
            {
                Data::FrontElement *fel = (*iter2);

                if (sfeMap.find(fel->getId()) == sfeMap.end())
                {
                    continue;
                }

                Data::FrontElementList *feList = &efeMap[fel->getId()];

                if (std::find(feList->begin(), feList->end(), fel) == feList->end())
                {
                    feList->push_back(fel);
                }
            }
        }
    }

    struct Sides
    {
        Data::FrontElementList side1;
        Data::FrontElementList side2;
        bool surrounded;
        bool allEdges;
    };

    typedef std::map<Data::Vertex *, Sides> SideMap;

    SideMap sides;

    for (VListMap::iterator iter = evMap.begin();
         iter != evMap.end(); iter++)
    {
        for (Data::VertexList::iterator it = (*iter).second.begin();
             it != (*iter).second.end(); it++)
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*it));

            if (sides.find(v) != sides.end())
            {
                continue;
            }

            Data::VertexList front;

            front.push_back(v);

            while (!front.empty())
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(front.front());

                front.pop_front();

                if (sides.find(v) != sides.end())
                {
                    continue;
                }

                bool surrounded = false;
                bool allEdges = false;

                Data::FrontElementList side1, side2;

                //determining if the vertex is surrounded by fels in surface and side1 and side2
                {
                    Data::FrontElementList adjsInSurface;

                    for (Data::FrontElementList::iterator iter2 = v->feBegin();
                         iter2 != v->feEnd(); iter2++)
                    {
                        Data::FrontElement *fel = (*iter2);

                        if (sfeMap.find(fel->getId()) == sfeMap.end())
                        {
                            continue;
                        }

                        adjsInSurface.push_back(fel);
                    }

                    //while (!adjsInSurface.empty())
                    {
                        Data::FrontElement *fel = adjsInSurface.front();

                        adjsInSurface.pop_front();

                        Data::FrontElement *other = NULL;

                        for (Data::FrontElementList::iterator iter2 = adjsInSurface.begin();
                             iter2 != adjsInSurface.end(); iter2++)
                        {
                            Data::FrontElement *o = (*iter2);

                            if (o->match(fel->getVertices()))
                            {
                                other = o;

                                adjsInSurface.erase(iter2);

                                break;
                            }
                        }

                        if (!other)
                        {
                            return false;
                        }

                        if (side1.empty())
                        {
                            side1.push_back(fel);
                            side2.push_back(other);
                        }
                    }

                    while (!adjsInSurface.empty())
                    {
                        UInt i = 0;

                        for (; i < side1.back()->numVertices(); i++)
                        {
                            if (side1.back()->getVertex(i) == v)
                            {
                                break;
                            }
                        }

                        Data::Vertex *p = side1.back()->getVertex((i + side1.back()->numVertices() - 1)%side1.back()->numVertices());

                        Data::FrontElement *next1 = NULL, *next2 = NULL;

                        for (Data::FrontElementList::iterator iter2 = adjsInSurface.begin();
                             iter2 != adjsInSurface.end(); iter2++)
                        {
                            Data::FrontElement *fel = (*iter2);

                            i = 0;

                            for (; i < fel->numVertices(); i++)
                            {
                                if (fel->getVertex(i) == v)
                                {
                                    break;
                                }
                            }

                            if (fel->getVertex((i + 1)%fel->numVertices()) == p)
                            {
                                adjsInSurface.erase(iter2);

                                next1 = fel;

                                break;
                            }
                        }

                        if (next1)
                        {
                            for (Data::FrontElementList::iterator iter2 = adjsInSurface.begin();
                                 iter2 != adjsInSurface.end(); iter2++)
                            {
                                Data::FrontElement *fel = (*iter2);

                                if (fel->match(next1->getVertices()))
                                {
                                    next2 = fel;

                                    iter2 = adjsInSurface.erase(iter2);

                                    break;
                                }
                            }

                            side1.push_back(next1);
                            side2.push_back(next2);

                            continue;
                        }

                        i = 0;

                        for (; i < side1.front()->numVertices(); i++)
                        {
                            if (side1.front()->getVertex(i) == v)
                            {
                                break;
                            }
                        }

                        p = side1.front()->getVertex((i + 1)%side1.front()->numVertices());

                        for (Data::FrontElementList::iterator iter2 = adjsInSurface.begin();
                             iter2 != adjsInSurface.end(); iter2++)
                        {
                            Data::FrontElement *fel = (*iter2);

                            i = 0;

                            for (; i < fel->numVertices(); i++)
                            {
                                if (fel->getVertex(i) == v)
                                {
                                    break;
                                }
                            }

                            if (fel->getVertex((i + fel->numVertices() - 1)%fel->numVertices()) == p)
                            {
                                adjsInSurface.erase(iter2);

                                next1 = fel;

                                break;
                            }
                        }

                        for (Data::FrontElementList::iterator iter2 = adjsInSurface.begin();
                             iter2 != adjsInSurface.end(); iter2++)
                        {
                            Data::FrontElement *fel = (*iter2);

                            if (fel->match(next1->getVertices()))
                            {
                                next2 = fel;

                                adjsInSurface.erase(iter2);

                                break;
                            }
                        }

                        side1.push_front(next1);
                        side2.push_front(next2);
                    }

                    if (side1.size() > 1)
                    {
                        Data::FrontElement *first = side1.front();
                        Data::FrontElement *last = side1.back();

                        UInt i = 0;

                        for (; i < first->numVertices(); i++)
                        {
                            if (first->getVertex(i) == v)
                            {
                                break;
                            }
                        }

                        Data::Vertex *n = first->getVertex((i + 1)%first->numVertices());

                        i = 0;

                        for (; i < last->numVertices(); i++)
                        {
                            if (last->getVertex(i) == v)
                            {
                                break;
                            }
                        }

                        if (last->getVertex((i + last->numVertices() - 1)%last->numVertices()) == n)
                        {
                            surrounded = true;
                        }
                    }

                    bool shouldSwap = false;

                    for (Data::FrontElementList::iterator iter = side1.begin();
                         iter != side1.end(); iter++)
                    {
                        Data::FrontElement *fel = (*iter);

                        bool shouldBreak = false;

                        for (UInt i = 0; i < fel->numVertices(); i++)
                        {
                            Data::Vertex *o = fel->getVertex(i);

                            if (o == v)
                            {
                                continue;
                            }

                            SideMap::iterator it = sides.find(o);

                            if (it == sides.end())
                            {
                                continue;
                            }

                            shouldBreak = true;

                            if (std::find((*it).second.side1.begin(), (*it).second.side1.end(), fel) == (*it).second.side1.end())
                            {
                                shouldSwap = true;
                            }

                            break;
                        }

                        if (shouldBreak)
                        {
                            break;
                        }
                    }

                    if (shouldSwap)
                    {
                        side1.swap(side2);
                    }

//debug
#if USE_GUI
                    for (Data::FrontElementList::iterator iter = side1.begin();
                         iter != side1.end(); iter++)
                    {
                        (*iter)->setColor(0.0, 1.0, 0.0);
                    }

                    for (Data::FrontElementList::iterator iter = side2.begin();
                         iter != side2.end(); iter++)
                    {
                        (*iter)->setColor(0.0, 0.0, 1.0);
                    }
#endif //#if USE_GUI
//endebug
                }

                if (!surrounded)
                {
                    allEdges = true;

                    for (Data::FrontElementList::iterator iter = side1.begin();
                         iter != side1.end(); iter++)
                    {
                        for (UInt i = 0; i < (*iter)->numVertices(); i++)
                        {
                            Data::Vertex *o = (*iter)->getVertex(i);

                            if (o == v)
                            {
                                continue;
                            }

                            if ((*(*eMap.find(v)).second.find(o)).second.size() <= 2)
                            {
                                allEdges = false;

                                break;
                            }
                        }

                        if (!allEdges)
                        {
                            break;
                        }
                    }
                }

                for (Data::FrontElementList::iterator iter = side1.begin();
                     iter != side1.end(); iter++)
                {
                    for (UInt i = 0; i < (*iter)->numVertices(); i++)
                    {
                        if ((*iter)->getVertex(i) != v)
                        {
                            front.push_back((*iter)->getVertex(i));
                        }
                    }
                }

                SideMap::iterator sit = sides.insert(std::make_pair(v, Sides())).first;

                (*sit).second.side1.swap(side1);
                (*sit).second.side2.swap(side2);
                (*sit).second.surrounded = surrounded;
                (*sit).second.allEdges = allEdges;

//debug
#if USE_GUI
                v->highlight();

                if (surrounded)
                {
                    v->setColor(1.0, 0.0, 0.0);
                }
                else if (allEdges)
                {
                    v->setColor(0.0, 1.0, 0.0);
                }
                else
                {
                    v->setColor(0.0, 0.0, 1.0);
                }
#endif //#if USE_GUI
//endebug
            }
        }
    }

    ///Bug quando se tem um número ímpar de crack surfaces em uma mesma aresta

    SideMap more;
    Data::ULIntSet potentiallyOddNumberOfSurfacesInEdge;

    for (SideMap::iterator iter = sides.begin();
         iter != sides.end(); iter++)
    {
        MJMesh::Vertex *v1 = static_cast<MJMesh::Vertex *>((*iter).first);

        SideMap::iterator mit = more.insert(std::make_pair(v1, Sides())).first;

        bool sur1 = ((*iter).second.surrounded);
        bool edg1 = ((*iter).second.allEdges);
        bool dup1 = ((sur1) || (edg1));

        if (!dup1)
        {
            continue;
        }

        if (edg1)
        {
            potentiallyOddNumberOfSurfacesInEdge.insert(v1->getId());
        }

        Data::FrontElementList adjs = v1->getAdjacentFrontElements();

        Data::VertexSet done;

        for (Data::FrontElementList::iterator iter2 = (*iter).second.side1.begin();
             iter2 != (*iter).second.side1.end(); iter2++)
        {
            Data::FrontElement *fel = (*iter2);

            UInt i = 0;

            for (; i < fel->numVertices(); i++)
            {
                if (fel->getVertex(i) == v1)
                {
                    break;
                }
            }

            for (UInt j = 0; j < 2; j++)
            {
                UInt k = (j == 0) ? ((i + 1)%fel->numVertices()) : ((i + fel->numVertices() - 1)%fel->numVertices());

                MJMesh::Vertex *v2 = static_cast<MJMesh::Vertex *>(fel->getVertex(k));

                if (!done.insert(v2).second)
                {
                    continue;
                }

                SideMap::iterator it = sides.find(v2);

                bool sur2 = ((*it).second.surrounded);
                bool edg2 = ((*it).second.allEdges);
                bool dup2 = ((sur2) || (edg2));

                if ((!dup1) && (!dup2))
                {
                    continue;
                }

                Data::FrontElementList sorted = Data::Mesh::findSortedAdjacentFrontElements(fel,
                    (j == 0) ? v1 : v2, (j == 0) ? adjs : v2->getAdjacentFrontElements());

                while (sorted.front() != fel)
                {
                    sorted.push_back(sorted.front());

                    sorted.pop_front();
                }

                while (!sorted.empty())
                {
                    Data::FrontElement *front = sorted.front();
                    Data::FrontElement *back = sorted.back();

                    sorted.pop_front();
                    sorted.pop_back();

                    bool h1 = (std::find((*iter).second.side1.begin(), (*iter).second.side1.end(), front) != (*iter).second.side1.end());
                    bool h2 = (std::find((*iter).second.side2.begin(), (*iter).second.side2.end(), back) != (*iter).second.side2.end());

                    if (!h1)
                    {
                        (*mit).second.side1.push_back(front);
                    }

                    if (!h2)
                    {
                        (*mit).second.side2.push_back(back);
                    }
                }
            }
        }
    }

    for (SideMap::iterator iter = more.begin();
         iter != more.end(); iter++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).first);

        SideMap::iterator sit = sides.find(v);

        /*if (v->getId() == 136)
        {
            std::cout << "debug" << std::endl;
        }*/

        if ((!(*sit).second.surrounded) && (!(*sit).second.allEdges))
        {
            continue;
        }

        Data::FrontElementList adjs = v->getAdjacentFrontElements();

        while (!adjs.empty())
        {
            Data::FrontElement *fel = adjs.front();

            adjs.pop_front();

            if ((std::find((*iter).second.side1.begin(), (*iter).second.side1.end(), fel) != (*iter).second.side1.end()) ||
                (std::find((*iter).second.side2.begin(), (*iter).second.side2.end(), fel) != (*iter).second.side2.end()) ||
                (std::find((*sit).second.side1.begin(), (*sit).second.side1.end(), fel) != (*sit).second.side1.end()) ||
                (std::find((*sit).second.side2.begin(), (*sit).second.side2.end(), fel) != (*sit).second.side2.end()))
            {
                continue;
            }

            UInt i = 0;

            for (; i < fel->numVertices(); i++)
            {
                if (fel->getVertex(i) == v)
                {
                    break;
                }
            }

            UInt j1 = ((i + 1)%fel->numVertices());
            UInt j2 = ((i + fel->numVertices() - 1)%fel->numVertices());

            Data::Vertex *v1 = fel->getVertex(j1);
            Data::Vertex *v2 = fel->getVertex(j2);

            bool found = false;

            for (Data::FrontElementList::iterator iter2 = v->feBegin();
                 iter2 != v->feEnd(); iter2++)
            {
                Data::FrontElement *adj = (*iter2);

                if (adj == fel)
                {
                    continue;
                }

                for (UInt j = 0; j < adj->numVertices(); j++)
                {
                    if (adj->getVertex(j) != v)
                    {
                        continue;
                    }

                    if ((adj->getVertex((j + 1)%adj->numVertices()) == v1) ||
                        (adj->getVertex((j + adj->numVertices() - 1)%adj->numVertices()) == v1))
                    {
                        if (((std::find((*iter).second.side1.begin(), (*iter).second.side1.end(), adj) != (*iter).second.side1.end())) ||
                            ((std::find((*sit).second.side1.begin(), (*sit).second.side1.end(), adj) != (*sit).second.side1.end())))
                        {
                            (*iter).second.side1.push_back(fel);

                            found = true;

                            break;
                        }
                    }

                    if ((adj->getVertex((j + 1)%adj->numVertices()) == v2) ||
                        (adj->getVertex((j + adj->numVertices() - 1)%adj->numVertices()) == v2))
                    {
                        if (((std::find((*iter).second.side2.begin(), (*iter).second.side2.end(), adj) != (*iter).second.side2.end())) ||
                            ((std::find((*sit).second.side2.begin(), (*sit).second.side2.end(), adj) != (*sit).second.side2.end())))
                        {
                            (*iter).second.side2.push_back(fel);

                            found = true;

                            break;
                        }
                    }

                    break;
                }

                if (found)
                {
                    break;
                }
            }

            if (!found)
            {
                adjs.push_back(fel);
            }
        }
    }

    //debug
#if USE_GUI
    for (SideMap::iterator iter = more.begin();
         iter != more.end(); iter++)
    {
        for (Data::FrontElementList::iterator iter2 = (*iter).second.side1.begin();
             iter2 != (*iter).second.side1.end(); iter2++)
        {
            (*iter2)->setColor(0.0, 1.0, 0.0);
        }

        for (Data::FrontElementList::iterator iter2 = (*iter).second.side2.begin();
             iter2 != (*iter).second.side2.end(); iter2++)
        {
            (*iter2)->setColor(0.0, 0.0, 1.0);
        }
    }
#endif //#if USE_GUI
    //endebug

    for (SideMap::iterator iter = sides.begin();
         iter != sides.end(); iter++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).first);

        if ((!(*iter).second.surrounded) && (!(*iter).second.allEdges))
        {
            continue;
        }

        //debug
        //if ((*iter).second.surrounded) continue;
        //endebug

        MJMesh::Vertex *newv = static_cast<MJMesh::Vertex *>(
            this->add(v->getId(), v->getPoint()->getCoord(0), v->getPoint()->getCoord(1), v->getPoint()->getCoord(2)));

#if USE_GUI
        newv->setColor(v->getR(), v->getG(), v->getB());
#endif //#if USE_GUI

        for (Data::FrontElementList::iterator iter2 = (*iter).second.side1.begin();
             iter2 != (*iter).second.side1.end(); iter2++)
        {
            (*iter2)->replace(v, newv);

            v->remove((*iter2));
            newv->add((*iter2));
        }

        SideMap::iterator mit = more.find(v);

        if (mit == more.end())
        {
            continue;
        }

        for (Data::FrontElementList::iterator iter2 = (*mit).second.side1.begin();
             iter2 != (*mit).second.side1.end(); iter2++)
        {
            (*iter2)->replace(v, newv);

            v->remove((*iter2));
            newv->add((*iter2));
        }
    }

    return true;
}

void TecGraf::BoundaryBuilder::adjustForCracks()
{
    bool found = false;

    for (OrientedSurfaceList::iterator iter = this->region->begin();
		 iter != this->region->end(); iter++)
    {
        OrientedSurfaceList::iterator iter2 = iter;

        for (++iter2; iter2 != this->region->end(); iter2++)
        {
            if ((*iter2)->getSurface() == (*iter)->getSurface())
            {
                found = true;

                break;
            }
        }

        if (found)
        {
            break;
        }
    }

    if (!found)
    {
        return;
    }

    EdgeFrontElementMap eMap;

    typedef std::set<Surface *> SurfaceMap;

    SurfaceMap sMap;

    //UInt debug = 0;

    for (OrientedSurfaceList::iterator iter = this->region->begin();
		 iter != this->region->end(); iter++)
    {
        Surface *s = (*iter)->getSurface();

        //if (!sMap.insert(s).second)
        if (sMap.insert(s).second)
        {
            continue;
        }

        if ((eMap.empty()) && (!this->buildMap(eMap)))
        {
            break;
        }

        /*if (debug == 0)
        {
            debug++;

            //eMap.clear();

            //continue;
        }*/

        if (this->makeCracks(s, eMap))
        {
            eMap.clear();

/*//debug
            for (Data::Boundary::FrontElementIterator iter = this->boundary->eBegin();
                 iter != this->boundary->eEnd();)
            {
                Data::FrontElement *fel = (*iter);

                if (fel->getId() > 0)
                {
                    iter++;

                    continue;
                }

                iter = this->boundary->erase(iter);

                delete fel;
            }
//emdebug*/

//debug
            //break;
//endebug
        }
    }
}

#endif //#if USE_TECGRAF
