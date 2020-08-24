#include "PMJMesh2/APrioriCrossingReplacer.h"

#include "Data/OStream.h"

#include "Data/BSPTree.h"
#include "Data/BSPTreeCell.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/MJMeshTask.h"

PMJMesh2::APrioriCrossingReplacer::APrioriCrossingReplacer(UInt dimension, bool onlyVertices, bool byTask,
    const Data::BSPTree *bsp, const Data::BSPTreeCell *parent,
    MJMesh::Boundary *boundary, const Parallel::TaskVector *tasks,
    const Data::VertexSet *vertices) :
        PMJMesh2::CrossingReplacer(dimension, byTask, bsp, parent, boundary, tasks)
{
    this->setOnlyVertices(onlyVertices);
    this->setVertices(vertices);
}

PMJMesh2::APrioriCrossingReplacer::~APrioriCrossingReplacer()
{

}

void PMJMesh2::APrioriCrossingReplacer::setOnlyVertices(bool onlyVertices)
{
    this->onlyVertices = onlyVertices;
}

void PMJMesh2::APrioriCrossingReplacer::setVertices(const Data::VertexSet *vertices)
{
    this->vertices = vertices;
}

void PMJMesh2::APrioriCrossingReplacer::backReplace(MJMesh::FrontElement *fel, Data::FrontElementList *temporaries, const Data::Boundary *delExcept)
{
    /*{
        std::stringstream str;
        str << "will replace back fel at " << fel << std::endl;
        std::cout << str.str();
    }
    {
        std::stringstream str;
        str << "will replace back fel " << fel->getId() << std::endl;
        std::cout << str.str();
    }*/

    while (!temporaries->empty())
    {
        MJMesh::FrontElement *tmp = static_cast<MJMesh::FrontElement *>(temporaries->front());

        temporaries->pop_front();

        /*{
            std::stringstream str;
            str << "found tmp fel at " << tmp << std::endl;
            std::cout << str.str();
        }
        {
            std::stringstream str;
            str << "found tmp fel " << tmp->getId() << std::endl;
            std::cout << str.str();
        }*/

        this->backReplace(fel, tmp, true, true, true, true);

        if ((!delExcept) || (!delExcept->have(tmp)))
        {
            /*{
                std::stringstream str;
                str << "deleting tmp fel at " << tmp << std::endl;
                std::cout << str.str();
            }*/

            delete tmp;
        }
        else
        {
            for (UInt i = 0; i < tmp->numVertices(); i++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(tmp->getVertex(i));

                v->remove(tmp);
            }
        }
    }

    delete temporaries;
}

void PMJMesh2::APrioriCrossingReplacer::backReplace(MJMesh::Vertex *v, Data::VertexList *temporaries, const Data::Boundary *delExcept)
{
    /*{
        Data::cout() << "will replace back v at " << v << Data::endl;
        Data::cout() << "will replace back v " << v->getId() << Data::endl;
    }*/

    while (!temporaries->empty())
    {
        MJMesh::Vertex *tmp = static_cast<MJMesh::Vertex *>(temporaries->front());

        temporaries->pop_front();

        /*{
            Data::cout() << "found tmp v at " << tmp << Data::endl;
            Data::cout() << "found tmp v " << tmp->getId() << Data::endl;
        }*/

        this->backReplace(v, tmp, true, true, true, true, delExcept);

        tmp->setPoint(NULL);

        if ((!delExcept) || (!delExcept->have(tmp)))
        {
            //Data::cout() << "deleting tmp v at " << tmp << Data::endl;

            delete tmp;
        }
    }

    delete temporaries;
}

void PMJMesh2::APrioriCrossingReplacer::replace()
{
    if (!this->onlyVertices)
    {
        PMJMesh::CrossingReplacer::replace();

        return;
    }

    if ((this->tasks->size() <= 1) || (!this->bsp) || (!this->parent) || (!this->vertices))
    {
        return;
    }

#if USE_GUI
    this->boundary->freeze();
#endif //#if USE_GUI

    for (Data::VertexSet::const_iterator iter = this->vertices->begin();
         iter != this->vertices->end(); iter++)
    {
        MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter));

        /*if (v->getId() == 558)
        {
            std::cout << "crossing replacer v " << v->getId() << " at " << v << std::endl;
        }*/

        for (Parallel::TaskVector::const_iterator iter2 = this->tasks->begin();
             iter2 != this->tasks->end(); iter2++)
        {
            PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>((*iter2));
            PMJMesh::Front *front = task->getFront();

            if (!front)
            {
                continue;
            }

            if (!front->have(v))
            {
                continue;
            }

            /*{
                std::stringstream str;
                str << "task " << task->getId() << ", will replace v at " << v << std::endl;
                std::cout << str.str();
            }
            {
                std::stringstream str;
                str << "task " << task->getId() << ", will replace v " << v->getId() << std::endl;
                std::cout << str.str();
            }*/

            MJMesh::Vertex *tmp = new MJMesh::Vertex(v->getPoint(), v->getId());

            if (this->boundary->have(v))
            {
                this->boundary->add(tmp);
                this->created.add(tmp);
            }

            /*{
                std::stringstream str;
                str << "task " << task->getId() << ", created tmp v at " << tmp << std::endl;
                std::cout << str.str();
            }
            {
                std::stringstream str;
                str << "task " << task->getId() << ", created tmp v " << tmp->getId() << std::endl;
                std::cout << str.str();
            }*/

            TaskVertexReplacementHashMap::iterator vMap = this->verticesOfTask.insert(std::make_pair(task->getId(), VertexReplacementHashMap())).first;

#if USE_C__11
            std::pair<VertexReplacementHashMap::iterator, bool> p2 = (*vMap).second.insert(std::make_pair(v->getId(), VertexReplacement()));
#else
            std::pair<VertexReplacementMap::iterator, bool> p2 = (*vMap).second.insert(std::make_pair(v->getId(), VertexReplacement()));
#endif //#if USE_C__11

            if (p2.second)
            {
                (*p2.first).second.original = v;

                (*p2.first).second.temporaries = new Data::VertexList();
            }

            (*p2.first).second.temporaries->push_back(tmp);

            //Data::FrontElementList adjs = v->getAdjacentFrontElements();

            //while (!adjs.empty())
            for (Data::FrontElementList::iterator iter3 = v->feBegin();
                 iter3 != v->feEnd();)
            {
                //Data::FrontElement *adj = adjs.front();
                Data::FrontElement *adj = (*iter3);

                //adjs.pop_front();

                /*if (adj == fel)
                {
                    iter3++;

                    continue;
                }*/

                if (!front->have(adj))
                {
                    iter3++;

                    continue;
                }

                /*if ((front->have(adj, 2)) || (front->have(adj, 3)))
                {
                    iter3++;

                    continue;
                }*/

                /*{
                    std::stringstream str;
                    str << "task " << task->getId() << ", adj at " << adj << ", replacing v for tmp" << std::endl;
                    std::cout << str.str();
                }
                {
                    std::stringstream str;
                    str << "task " << task->getId() << ", adj " << adj->getId() << ", replacing v for tmp" << std::endl;
                    std::cout << str.str();
                }*/

                adj->replace(v, tmp);

                //v->remove(adj);

                tmp->add(adj);

                iter3 = v->erase(iter3);
            }

            if (front->have(v))
            {
                front->remove(v);

                /*{
                    std::stringstream str;
                    str << "task " << task->getId() << ", front at " << front << ", replacing v for tmp" << std::endl;
                    std::cout << str.str();
                }*/

                front->add(tmp);
            }

            //this->tasksOfVertex[v->getId()].push_back(task->getId());
        }
    }

    for (Parallel::TaskVector::const_iterator iter = this->tasks->begin();
		 iter != this->tasks->end(); iter++)
	{
		PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>((*iter));
		PMJMesh::Front *front = task->getFront();

		if (!front)
		{
			continue;
		}

		TaskFrontElementReplacementHashMap::iterator felMap = this->frontElementsOfTask.insert(std::make_pair(task->getId(), FrontElementReplacementHashMap())).first;
		TaskVertexReplacementHashMap::iterator vMap = this->verticesOfTask.insert(std::make_pair(task->getId(), VertexReplacementHashMap())).first;

		Data::BSPTreeCell *cell = static_cast<Data::BSPTreeCell *>(static_cast<Data::BSPTreeCell *>(this->bsp->node(task->getId()))->parent());

		while (cell != this->parent)
        {
            if (!this->frontElementsOfTask.insert(std::make_pair(cell->getId(), FrontElementReplacementHashMap())).second)
            {
                break;
            }

            /*{
                std::stringstream str;
                str << "inserting for id " << cell->getId() << std::endl;
                std::cout << str.str();
            }*/

            this->verticesOfTask.insert(std::make_pair(cell->getId(), VertexReplacementHashMap()));

            cell = static_cast<Data::BSPTreeCell *>(cell->parent());
        }
	}

#if USE_GUI
    this->boundary->unfreeze();
#endif //#if USE_GUI
}

void PMJMesh2::APrioriCrossingReplacer::backReplace()
{
    if (!this->onlyVertices)
    {
        PMJMesh::CrossingReplacer::backReplace();

        return;
    }
}
