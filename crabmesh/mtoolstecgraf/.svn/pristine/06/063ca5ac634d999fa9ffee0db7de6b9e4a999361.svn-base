#include "PMJMesh2/CrossingReplacer.h"

#include "Data/OStream.h"
#include "Data/BSPTree.h"
#include "Data/BSPTreeCell.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/MJMeshTask.h"

using namespace PMJMesh2;

PMJMesh2::CrossingReplacer::CrossingReplacer(UInt dimension, bool byTask,
    const Data::BSPTree *bsp, const Data::BSPTreeCell *parent,
    MJMesh::Boundary *boundary, const Parallel::TaskVector *tasks) :
        PMJMesh::CrossingReplacer::CrossingReplacer(dimension, tasks)
{
    this->setByTask(byTask);
    this->setBSP(bsp);
    this->setParent(parent);
    this->setBoundary(boundary);
}

PMJMesh2::CrossingReplacer::~CrossingReplacer()
{
#if USE_C__11
	for (TaskFrontElementReplacementHashMap::iterator iter2 = this->frontElementsOfTask.begin();
#else
	for (TaskFrontElementReplacementMap::iterator iter2 = this->frontElementsOfTask.begin();
#endif //#if USE_C__11
		 iter2 != this->frontElementsOfTask.end(); iter2++)
	{
#if USE_C__11
        for (FrontElementReplacementHashMap::iterator iter = (*iter2).second.begin();
#else
        for (FrontElementReplacementMap::iterator iter = (*iter2).second.begin();
#endif //#if USE_C__11
            iter != (*iter2).second.end(); iter++)
        {
            while (!(*iter).second.temporaries->empty())
            {
                MJMesh::FrontElement *tmp = static_cast<MJMesh::FrontElement *>((*iter).second.temporaries->front());

                (*iter).second.temporaries->pop_front();

                while (!tmp->getData().boundaries.empty())
                {
                    const_cast<MJMesh::Boundary *>((*tmp->getData().boundaries.begin()).first)->remove(tmp);
                }

                while (!tmp->getData().fronts.empty())
                {
                    const_cast<MJMesh::Front *>((*tmp->getData().fronts.begin()).first)->remove(tmp);
                }

                while (!tmp->getData().meshes.empty())
                {
                    const_cast<MJMesh::Mesh *>((*tmp->getData().meshes.begin()).first)->remove(tmp);
                }

                delete tmp;
            }

            delete (*iter).second.temporaries;
        }
	}

	this->frontElementsOfTask.clear();

#if USE_C__11
	for (TaskVertexReplacementHashMap::iterator iter2 = this->verticesOfTask.begin();
#else
	for (TaskVertexReplacementMap::iterator iter2 = this->verticesOfTask.begin();
#endif //#if USE_C__11
		 iter2 != this->verticesOfTask.end(); iter2++)
	{
#if USE_C__11
        for (VertexReplacementHashMap::iterator iter = this->vertices.begin();
#else
        for (VertexReplacementMap::iterator iter = this->vertices.begin();
#endif //#if USE_C__11
             iter != this->vertices.end(); iter++)
        {
            while (!(*iter).second.temporaries->empty())
            {
                MJMesh::Vertex *tmp = static_cast<MJMesh::Vertex *>((*iter).second.temporaries->front());

                (*iter).second.temporaries->pop_front();

                //Data::ElementList adjs = tmp->getAdjacentElements();

                //while (!adjs.empty())
                for (Data::ElementList::iterator iter3 = tmp->eBegin();
                     iter3 != tmp->eEnd(); iter3 = tmp->erase(iter3))
                {
                    //adjs.front()->replace(tmp, NULL);
                    (*iter3)->replace(tmp, NULL);

                    //adjs.pop_front();
                }

                //Data::FrontElementList fadjs = tmp->getAdjacentFrontElements();

                //while (!fadjs.empty())
                for (Data::FrontElementList::iterator iter3 = tmp->feBegin();
                     iter3 != tmp->feEnd(); iter3 = tmp->erase(iter3))
                {
                    //fadjs.front()->replace(tmp, NULL);
                    (*iter3)->replace(tmp, NULL);

                    //fadjs.pop_front();
                }

                while (!tmp->getData().boundaries.empty())
                {
                    const_cast<MJMesh::Boundary *>((*tmp->getData().boundaries.begin()).first)->remove(tmp);
                }

                while (!tmp->getData().fronts.empty())
                {
                    const_cast<MJMesh::Front *>((*tmp->getData().fronts.begin()).first)->remove(tmp);
                }

                while (!tmp->getData().meshes.empty())
                {
                    const_cast<MJMesh::Mesh *>((*tmp->getData().meshes.begin()).first)->remove(tmp);
                }

                tmp->setPoint(NULL);

                delete tmp;
            }

            delete (*iter).second.temporaries;
        }
	}

	this->verticesOfTask.clear();
}

void PMJMesh2::CrossingReplacer::setByTask(bool byTask)
{
    this->byTask = byTask;
}

bool PMJMesh2::CrossingReplacer::isByTask() const
{
    return this->byTask;
}

void PMJMesh2::CrossingReplacer::setBSP(const Data::BSPTree *bsp)
{
    this->bsp = bsp;
}

void PMJMesh2::CrossingReplacer::setParent(const Data::BSPTreeCell *parent)
{
    this->parent = parent;
}

void PMJMesh2::CrossingReplacer::setBoundary(MJMesh::Boundary *boundary)
{
    this->boundary = boundary;
}

Data::Vertex *PMJMesh2::CrossingReplacer::getTemporaryVertex(ULInt idTask, ULInt idVertex) const
{
    TaskVertexReplacementHashMap::const_iterator vMap = this->verticesOfTask.find(idTask);

    if (vMap == this->verticesOfTask.end())
    {
        return NULL;
    }

    VertexReplacementHashMap::const_iterator vIter = (*vMap).second.find(idVertex);

    if (vIter == (*vMap).second.end())
    {
        return NULL;
    }

    if (!(*vIter).second.temporaries)
    {
        return NULL;
    }

    return (*vIter).second.temporaries->front();
}

Data::FrontElement *PMJMesh2::CrossingReplacer::getTemporaryFrontElement(ULInt idTask, ULInt idFrontElement) const
{
    TaskFrontElementReplacementHashMap::const_iterator felMap = this->frontElementsOfTask.find(idTask);

    if (felMap == this->frontElementsOfTask.end())
    {
        return NULL;
    }

    FrontElementReplacementHashMap::const_iterator felIter = (*felMap).second.find(idFrontElement);

    if (felIter == (*felMap).second.end())
    {
        return NULL;
    }

    if (!(*felIter).second.temporaries)
    {
        return NULL;
    }

    return (*felIter).second.temporaries->front();
}

/*void PMJMesh2::CrossingReplacer::execute()
{
    if (!this->byTask)
    {
        PMJMesh::CrossingReplacer::execute();

        return;
    }

    if (this->back)
    {
        this->backReplace();
    }
    else
    {
        if (!this->tasks)
        {
            return;
        }

        this->replace();

        this->tasks = NULL;

        this->back = !this->back;
    }
}*/

void PMJMesh2::CrossingReplacer::replace()
{
    if (!this->byTask)
    {
        PMJMesh::CrossingReplacer::replace();

        return;
    }

    if ((this->tasks->size() <= 1) || (!this->bsp) || (!this->parent))
    {
        return;
    }

#if USE_GUI
    this->boundary->freeze();
#endif //#if USE_GUI

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

#if USE_C__11
		Data::VertexHashMap vertices;
#else
		Data::VertexMap vertices;
#endif //#if USE_C__11

		for (UShort state = 2; state < 4; state++) //do it for rejected and crossing
		{
			Data::FrontElementList frontElements;

			for (Data::Front::ConstFrontElementIterator iter = front->eBegin(state);
				 iter != front->eEnd(state); iter = front->erase(iter, state))
			{
				MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter));

				/*{
                    std::stringstream str;
                    str << "task " << task->getId() << ", will replace fel at " << fel << std::endl;
                    std::cout << str.str();
				}
				{
                    std::stringstream str;
                    str << "task " << task->getId() << ", will replace fel " << fel->getId() << std::endl;
                    std::cout << str.str();
				}*/

				Data::VertexVector replacements(fel->numVertices(), NULL);

				for (UInt i = 0; i < fel->numVertices(); i++)
				{
					MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(fel->getVertex(i));

#if USE_C__11
					std::pair<Data::VertexHashMap::iterator, bool> p = vertices.insert(std::make_pair(v->getId(), static_cast<Data::Vertex *>(NULL)));
#else
					std::pair<Data::VertexMap::iterator, bool> p = vertices.insert(std::make_pair(v->getId(), static_cast<Data::Vertex *>(NULL)));
#endif //#if USE_C__11

					if (p.second)
					{
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

						(*p.first).second = tmp;

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
						for (Data::FrontElementList::iterator iter = v->feBegin();
                             iter != v->feEnd();)
						{
							//Data::FrontElement *adj = adjs.front();
							Data::FrontElement *adj = (*iter);

							//adjs.pop_front();

							if (adj == fel)
							{
							    iter++;

								continue;
							}

							if (!front->have(adj))
							{
							    iter++;

								continue;
							}

							if ((front->have(adj, 2)) || (front->have(adj, 3)))
							{
							    iter++;

								continue;
							}

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

							iter = v->erase(iter);
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

					replacements[i] = (*p.first).second;
				}

				Data::FrontElement *tmp = (this->dimension == 2) ?
					static_cast<Data::FrontElement *>(new MJMesh::FrontElement2D(static_cast<MJMesh::Vertex *>(replacements[0]), static_cast<MJMesh::Vertex *>(replacements[1]), fel->getId())) :
					static_cast<Data::FrontElement *>(new MJMesh::FrontElement3D(static_cast<MJMesh::Vertex *>(replacements[0]), static_cast<MJMesh::Vertex *>(replacements[1]), static_cast<MJMesh::Vertex *>(replacements[2]), fel->getId()));

				/*{
                    std::stringstream str;
                    str << "task " << task->getId() << ", created tmp fel at " << tmp << std::endl;
                    std::cout << str.str();
				}
				{
                    std::stringstream str;
                    str << "task " << task->getId() << ", created tmp fel " << tmp->getId() << std::endl;
                    std::cout << str.str();
				}*/

				if (this->boundary->have(fel))
                {
                    this->boundary->add(tmp);
                    this->created.add(tmp);
                }

#if USE_C__11
				std::pair<FrontElementReplacementHashMap::iterator, bool> p2 = (*felMap).second.insert(std::make_pair(fel->getId(), FrontElementReplacement()));
#else
				std::pair<FrontElementReplacementMap::iterator, bool> p2 = (*felMap).second.insert(std::make_pair(fel->getId(), FrontElementReplacement()));
#endif //#if USE_C__11

				if (p2.second)
				{
					(*p2.first).second.original = fel;

					(*p2.first).second.temporaries = new Data::FrontElementList();
				}

				(*p2.first).second.temporaries->push_back(tmp);

				frontElements.push_back(tmp);

				/*{
                    std::stringstream str;
                    str << "task " << task->getId() << ", front at " << front << ", removing fel at " << fel << std::endl;
                    std::cout << str.str();
				}
				{
                    std::stringstream str;
                    str << "task " << task->getId() << ", front at " << front << ", removing fel " << fel->getId() << std::endl;
                    std::cout << str.str();
				}*/

				//this->tasksOfFrontElement[fel->getId()].push_back(task->getId());
			}

			while (!frontElements.empty())
			{
				/*{
                    std::stringstream str;
                    str << "task " << task->getId() << ", front at " << front << ", adding tmp fel at " << frontElements.front() << std::endl;
                    std::cout << str.str();
				}
				{
                    std::stringstream str;
                    str << "task " << task->getId() << ", front at " << front << ", adding tmp fel " << frontElements.front()->getId() << std::endl;
                    std::cout << str.str();
				}*/

				if (state == 2)
				{
					front->addRejected(frontElements.front());
				}
				else if (state == 3)
				{
					front->addCrossing(frontElements.front());
				}

				frontElements.pop_front();
			}
		}

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

	//this->frontElementsOfTask.insert(std::make_pair(this->parent->getId(), FrontElementReplacementHashMap()));
    //this->verticesOfTask.insert(std::make_pair(this->parent->getId(), VertexReplacementHashMap()));

#if USE_GUI
    this->boundary->unfreeze();
#endif //#if USE_GUI
}

void PMJMesh2::CrossingReplacer::backReplace()
{
    if (!this->byTask)
    {
        PMJMesh::CrossingReplacer::backReplace();

        return;
    }

    //Data::cout() << "(2) this->created.size() = " << this->created.size() << Data::endl;
    //Data::cout() << "(2) this->created.vsize() = " << this->created.verticesSize() << Data::endl;

/*#if USE_GUI
    this->boundary->freeze();
#endif //#if USE_GUI

    for (Data::Boundary::FrontElementIterator iter = this->created.eBegin();
         iter != this->created.eEnd();)
    {
        Data::FrontElement *fel = (*iter);

        iter = this->created.erase(iter);

        if (this->boundary->have(fel))
        {
            this->boundary->remove(fel);
        }

        delete fel;
    }

    for (Data::Boundary::VertexIterator iter = this->created.vBegin();
         iter != this->created.vEnd();)
    {
        Data::Vertex *v = (*iter);

        iter = this->created.erase(iter);

        if (this->boundary->have(v))
        {
            this->boundary->remove(v);
        }

        delete v;
    }

#if USE_GUI
    this->boundary->unfreeze();
#endif //#if USE_GUI*/
}

void PMJMesh2::CrossingReplacer::backReplaceToTemporary(ULInt id1, ULInt id2, ULInt idparent)
{
#if USE_C__11
    TaskVertexReplacementHashMap::iterator vMap0 = this->verticesOfTask.find(id1);
    TaskVertexReplacementHashMap::iterator vMap1 = this->verticesOfTask.find(id2);
    TaskVertexReplacementHashMap::iterator vMapP = this->verticesOfTask.find(idparent);

    TaskFrontElementReplacementHashMap::iterator felMap0 = this->frontElementsOfTask.find(id1);
    TaskFrontElementReplacementHashMap::iterator felMap1 = this->frontElementsOfTask.find(id2);
    TaskFrontElementReplacementHashMap::iterator felMapP = this->frontElementsOfTask.find(idparent);
#else
    TaskVertexReplacementMap::iterator vMap0 = this->verticesOfTask.find(id1);
    TaskVertexReplacementMap::iterator vMap1 = this->verticesOfTask.find(id2);
    TaskVertexReplacementMap::iterator vMapP = this->verticesOfTask.find(idparent);

    TaskFrontElementReplacementMap::iterator felMap0 = this->frontElementsOfTask.find(id1);
    TaskFrontElementReplacementMap::iterator felMap1 = this->frontElementsOfTask.find(id2);
    TaskFrontElementReplacementMap::iterator felMapP = this->frontElementsOfTask.find(idparent);
#endif //#if USE_C__11

    /*if (vMap0 == this->verticesOfTask.end())
    {
        std::cout << "vmap for id0 " << id1 << " is end()" << std::endl;
    }

    if (felMap0 == this->frontElementsOfTask.end())
    {
        std::cout << "felmap for id0 " << id1 << " is end()" << std::endl;
    }

    if (vMap1 == this->verticesOfTask.end())
    {
        std::cout << "vmap for id1 " << id2 << " is end()" << std::endl;
    }

    if (felMap1 == this->frontElementsOfTask.end())
    {
        std::cout << "felmap for id1 " << id2 << " is end()" << std::endl;
    }

    if (vMapP == this->verticesOfTask.end())
    {
        std::cout << "vmap for idp " << idparent << " is end()" << std::endl;
    }

    if (felMapP == this->frontElementsOfTask.end())
    {
        std::cout << "felmap for idp " << idparent << " is end()" << std::endl;
    }*/

    (*vMap0).second.swap((*vMapP).second);
    (*felMap0).second.swap((*felMapP).second);

#if USE_C__11
    for (FrontElementReplacementHashMap::iterator iter = (*felMap1).second.begin();
#else
    for (FrontElementReplacementMap::iterator iter = (*felMap1).second.begin();
#endif //#if USE_C__11
         iter != (*felMap1).second.end(); iter++)
    {
        ULInt idFel = (*iter).first;

#if USE_C__11
        FrontElementReplacementHashMap::iterator it = (*felMapP).second.find(idFel);
#else
        FrontElementReplacementMap::iterator it = (*felMapP).second.find(idFel);
#endif //#if USE_C__11

        if (it == (*felMapP).second.end())
        {
            (*felMapP).second.insert((*iter));

            continue;
        }

        this->backReplace(static_cast<MJMesh::FrontElement *>((*it).second.temporaries->front()), (*iter).second.temporaries, &this->created);
    }

    (*felMap1).second.clear();

#if USE_C__11
    for (VertexReplacementHashMap::iterator iter = (*vMap1).second.begin();
#else
    for (VertexReplacementMap::iterator iter = (*vMap1).second.begin();
#endif //#if USE_C__11
         iter != (*vMap1).second.end(); iter++)
    {
        ULInt idV = (*iter).first;

        /*if (idV == 35)
        {
            std::cout << "found " << idV << std::endl;
        }*/

#if USE_C__11
        VertexReplacementHashMap::iterator it = (*vMapP).second.find(idV);
#else
        VertexReplacementMap::iterator it = (*vMapP).second.find(idV);
#endif //#if USE_C__11

        if (it == (*vMapP).second.end())
        {
            (*vMapP).second.insert((*iter));

            continue;
        }

        this->backReplace(static_cast<MJMesh::Vertex *>((*it).second.temporaries->front()), (*iter).second.temporaries, &this->created);
    }

    (*vMap1).second.clear();
}

void PMJMesh2::CrossingReplacer::backReplaceToOriginal(ULInt id, const Data::Boundary *boundary)
{
    /*{
        std::stringstream str;
        str << "verticesOfTask.size = " << this->verticesOfTask.size() << std::endl;
        std::cout << str.str();
    }

    for (TaskVertexReplacementHashMap::iterator iter = this->verticesOfTask.begin();
         iter != this->verticesOfTask.end(); iter++)
    {
        {
            std::stringstream str;
            str << "size if vertices of task " << (*iter).first << " = " << (*iter).second.size() << std::endl;
            std::cout << str.str();
        }
    }*/

#if USE_C__11
    TaskVertexReplacementHashMap::iterator vMap = this->verticesOfTask.find(id);
    TaskFrontElementReplacementHashMap::iterator felMap = this->frontElementsOfTask.find(id);
#else
    TaskVertexReplacementMap::iterator vMap = this->verticesOfTask.find(id);
    TaskFrontElementReplacementMap::iterator felMap = this->frontElementsOfTask.find(id);
#endif //#if USE_C__11

    /*if (vMap == this->verticesOfTask.end())
    {
        {
            std::stringstream str;
            str << "vmap for id " << id << " is end()" << std::endl;
            std::cout << str.str();
        }
    }

    if (felMap == this->frontElementsOfTask.end())
    {
        {
            std::stringstream str;
            str << "felmap for id " << id << " is end()" << std::endl;
            std::cout << str.str();
        }
    }*/

    if (felMap != this->frontElementsOfTask.end())
    {
#if USE_C__11
        for (FrontElementReplacementHashMap::iterator iter = (*felMap).second.begin();
#else
        for (FrontElementReplacementMap::iterator iter = (*felMap).second.begin();
#endif //#if USE_C__11
             iter != (*felMap).second.end(); iter++)
        {
            MJMesh::FrontElement *fel = static_cast<MJMesh::FrontElement *>((*iter).second.original);

            for (Data::FrontElementList::iterator iter2 = (*iter).second.temporaries->begin();
                 iter2 != (*iter).second.temporaries->end(); iter2++)
            {
                MJMesh::FrontElement *tmp = static_cast<MJMesh::FrontElement *>((*iter2));

                Data::BoundaryList boundaries;

                for (MJMesh::FrontElementSpecificData::BoundaryDataMap::const_iterator iter3 = tmp->getData().boundaries.begin();
                     iter3 != tmp->getData().boundaries.end(); iter3++)
                {
                    MJMesh::Boundary *boundary = const_cast<MJMesh::Boundary *>((*iter3).first);

                    if (boundary == &this->created)
                    {
                        continue;
                    }

                    boundaries.push_back(boundary);
                }

                while (!boundaries.empty())
                {
                    MJMesh::Boundary *boundary = static_cast<MJMesh::Boundary *>(boundaries.front());

                    boundaries.pop_front();

#if USE_GUI
                    boundary->freeze();
#endif //#if USE_GUI

                    boundary->remove(tmp);

                    if (!boundary->have(fel))
                    {
                        boundary->add(fel);
                    }

#if USE_GUI
                    boundary->unfreeze();
#endif //#if USE_GUI
                }
            }

#if USE_GUI
            if (!(*iter).second.temporaries->empty())
            {
                Data::FrontElement *tmp = (*iter).second.temporaries->front();

                fel->setColor(tmp->getR(), tmp->getG(), tmp->getB());
            }
#endif //#if USE_GUI

            this->backReplace(fel, (*iter).second.temporaries, &this->created);
        }

        (*felMap).second.clear();
    }

    if (vMap != this->verticesOfTask.end())
    {
#if USE_C__11
        for (VertexReplacementHashMap::iterator iter = (*vMap).second.begin();
#else
        for (VertexReplacementMap::iterator iter = (*vMap).second.begin();
#endif //#if USE_C__11
             iter != (*vMap).second.end(); iter++)
        {
            MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).second.original);

            /*if (v->getId() == 35)
            {
                std::cout << "found " << v->getId() << std::endl;
            }*/

            for (Data::VertexList::iterator iter2 = (*iter).second.temporaries->begin();
                 iter2 != (*iter).second.temporaries->end(); iter2++)
            {
                MJMesh::Vertex *tmp = static_cast<MJMesh::Vertex *>((*iter2));

                Data::BoundaryList boundaries;

                for (MJMesh::VertexSpecificData::BoundaryDataMap::const_iterator iter3 = tmp->getData().boundaries.begin();
                     iter3 != tmp->getData().boundaries.end(); iter3++)
                {
                    MJMesh::Boundary *boundary = const_cast<MJMesh::Boundary *>((*iter3).first);

                    if (boundary == &this->created)
                    {
                        continue;
                    }

                    boundaries.push_back(boundary);
                }

                while (!boundaries.empty())
                {
                    MJMesh::Boundary *boundary = static_cast<MJMesh::Boundary *>(boundaries.front());

                    boundaries.pop_front();

#if USE_GUI
                    boundary->freeze();
#endif //#if USE_GUI

                    boundary->remove(tmp);

                    if (!boundary->have(v))
                    {
                        boundary->add(v);
                    }

#if USE_GUI
                    boundary->unfreeze();
#endif //#if USE_GUI
                }
            }

#if USE_GUI
            if (!(*iter).second.temporaries->empty())
            {
                Data::Vertex *tmp = (*iter).second.temporaries->front();

                v->setColor(tmp->getR(), tmp->getG(), tmp->getB());
            }
#endif //#if USE_GUI

            this->backReplace(v, (*iter).second.temporaries, &this->created);

            for (MJMesh::VertexSpecificData::MeshDataMap::iterator iter = v->getData().meshes.begin();
                 iter != v->getData().meshes.end(); iter++)
            {
                MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>((*iter).first);

                /*{
                    std::stringstream str;
                    str << "removing tmp from mesh at " << mesh << " and adding v to it" << std::endl;
                    std::cout << str.str();
                }*/

#if USE_GUI
                mesh->freeze();
#endif //#if USE_GUI

                if ((boundary->have(v)) && (mesh->haveInner(v)))
                {
                    mesh->removeInner(v);
                }

#if USE_GUI
                mesh->unfreeze();
#endif //#if USE_GUI
            }
        }

        (*vMap).second.clear();
    }
}

MJMesh::Vertex *PMJMesh2::CrossingReplacer::backReplaceToOriginal(ULInt idTask, MJMesh::Vertex *temporary, const Data::Boundary *boundary)
{
#if USE_C__11
    TaskVertexReplacementHashMap::iterator vMap = this->verticesOfTask.find(idTask);
#else
    TaskVertexReplacementMap::iterator vMap = this->verticesOfTask.find(idTask);
#endif //#if USE_C__11

    if (vMap == this->verticesOfTask.end())
    {
        return NULL;
    }

#if USE_C__11
    VertexReplacementHashMap::iterator iter = (*vMap).second.find(temporary->getId());
#else
    VertexReplacementMap::iterator iter = (*vMap).second.find(temporary->getId());
#endif //#if USE_C__11

    if (iter == (*vMap).second.end())
    {
        return NULL;
    }

    MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>((*iter).second.original);

    /*if (v->getId() == 207)
    {
        std::cout << "found " << v->getId() << std::endl;
    }*/

    bool found = false;

    for (Data::VertexList::iterator iter2 = (*iter).second.temporaries->begin();
         iter2 != (*iter).second.temporaries->end(); iter2++)
    {
        if ((*iter2) == temporary)
        {
            found = true;

            break;
        }
    }

    if (!found)
    {
        return NULL;
    }

    for (Data::VertexList::iterator iter2 = (*iter).second.temporaries->begin();
         iter2 != (*iter).second.temporaries->end(); iter2++)
    {
        MJMesh::Vertex *tmp = static_cast<MJMesh::Vertex *>((*iter2));

        Data::BoundaryList boundaries;

        for (MJMesh::VertexSpecificData::BoundaryDataMap::const_iterator iter3 = tmp->getData().boundaries.begin();
             iter3 != tmp->getData().boundaries.end(); iter3++)
        {
            MJMesh::Boundary *boundary = const_cast<MJMesh::Boundary *>((*iter3).first);

            if (boundary == &this->created)
            {
                continue;
            }

            boundaries.push_back(boundary);
        }

        while (!boundaries.empty())
        {
            MJMesh::Boundary *boundary = static_cast<MJMesh::Boundary *>(boundaries.front());

            boundaries.pop_front();

#if USE_GUI
            boundary->freeze();
#endif //#if USE_GUI

            boundary->remove(tmp);

            if (!boundary->have(v))
            {
                boundary->add(v);
            }

#if USE_GUI
            boundary->unfreeze();
#endif //#if USE_GUI
        }
    }

#if USE_GUI
    if (!(*iter).second.temporaries->empty())
    {
        Data::Vertex *tmp = (*iter).second.temporaries->front();

        v->setColor(tmp->getR(), tmp->getG(), tmp->getB());
    }
#endif //#if USE_GUI

    this->backReplace(v, (*iter).second.temporaries, &this->created);

    for (MJMesh::VertexSpecificData::MeshDataMap::iterator iter = v->getData().meshes.begin();
         iter != v->getData().meshes.end(); iter++)
    {
        MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>((*iter).first);

        /*{
            std::stringstream str;
            str << "removing tmp from mesh at " << mesh << " and adding v to it" << std::endl;
            std::cout << str.str();
        }*/

#if USE_GUI
        mesh->freeze();
#endif //#if USE_GUI

        if ((boundary->have(v)) && (mesh->haveInner(v)))
        {
            mesh->removeInner(v);
        }

#if USE_GUI
        mesh->unfreeze();
#endif //#if USE_GUI
    }

    (*vMap).second.erase(iter);

    return v;
}

void PMJMesh2::CrossingReplacer::backReplace(ULInt id1, ULInt id2, const Data::Boundary *boundary)
{
    if ((!this->byTask) || (!this->back))
    {
        return;
    }

    ULInt idparent = static_cast<Data::BSPTreeCell *>(this->bsp->node(id1))->parent()->getId();

    ULInt tasks[2];

    if (id1 < id2)
    {
        tasks[0] = id1;
        tasks[1] = id2;
    }
    else
    {
        tasks[0] = id2;
        tasks[1] = id1;
    }

    if (idparent != this->parent->getId())
    {
        this->backReplaceToTemporary(tasks[0], tasks[1], idparent);

        return;
    }

    //std::stringstream str;
    //str << Data::trace() << std::endl;
    //std::cout << str.str();

    for (UInt i = 0; i < 2; i++)
    {
        this->backReplaceToOriginal(tasks[i], boundary);
    }

    this->verticesOfTask.clear();
    this->frontElementsOfTask.clear();

    //Data::cout() << "(1) this->created.size() = " << this->created.size() << Data::endl;
    //Data::cout() << "(1) this->created.vsize() = " << this->created.verticesSize() << Data::endl;

#if USE_GUI
    this->boundary->freeze();
#endif //#if USE_GUI

    for (Data::Boundary::FrontElementIterator iter = this->created.eBegin();
         iter != this->created.eEnd();)
    {
        Data::FrontElement *fel = (*iter);

        iter = this->created.erase(iter);

        if (this->boundary->have(fel))
        {
            this->boundary->remove(fel);
        }

        delete fel;
    }

    for (Data::Boundary::VertexIterator iter = this->created.vBegin();
         iter != this->created.vEnd();)
    {
        Data::Vertex *v = (*iter);

        //Data::cout() << "removing vertex at " << v << " from created" << Data::endl;
        //Data::cout() << "removing vertex " << v->getId() << " from created at " << &this->created << Data::endl;

        iter = this->created.erase(iter);

        if (this->boundary->have(v))
        {
            //Data::cout() << "removing vertex " << v->getId() << " from this->boundary at " << this->boundary << Data::endl;

            this->boundary->remove(v);
        }

        delete v;
    }

#if USE_GUI
    this->boundary->unfreeze();
#endif //#if USE_GUI
}
