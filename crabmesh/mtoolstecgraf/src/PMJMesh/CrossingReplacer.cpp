#include "PMJMesh/CrossingReplacer.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/MJMeshTask.h"

PMJMesh::CrossingReplacer::CrossingReplacer(UInt dimension, const Parallel::TaskVector *tasks) :
	Performer::Builder()
{
	this->setDimension(dimension);
	this->setTasks(tasks);
	this->back = false;
}

PMJMesh::CrossingReplacer::~CrossingReplacer()
{
#if USE_C__11
	for (FrontElementReplacementHashMap::iterator iter = this->frontElements.begin();
#else
	for (FrontElementReplacementMap::iterator iter = this->frontElements.begin();
#endif //#if USE_C__11
		 iter != this->frontElements.end(); iter++)
	{
		MJMesh::FrontElement *orig = static_cast<MJMesh::FrontElement *>((*iter).second.original);

		while (!orig->getData().boundaries.empty())
		{
			const_cast<MJMesh::Boundary *>((*orig->getData().boundaries.begin()).first)->remove(orig);
		}

		while (!orig->getData().fronts.empty())
		{
			const_cast<MJMesh::Front *>((*orig->getData().fronts.begin()).first)->remove(orig);
		}

		while (!orig->getData().meshes.empty())
		{
			const_cast<MJMesh::Mesh *>((*orig->getData().meshes.begin()).first)->remove(orig);
		}

		delete orig;

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

	this->frontElements.clear();

#if USE_C__11
	for (VertexReplacementHashMap::iterator iter = this->vertices.begin();
#else
	for (VertexReplacementMap::iterator iter = this->vertices.begin();
#endif //#if USE_C__11
		 iter != this->vertices.end(); iter++)
	{
		MJMesh::Vertex *orig = static_cast<MJMesh::Vertex *>((*iter).second.original);

		//Data::ElementList adjs = orig->getAdjacentElements();

		//while (!adjs.empty())
		for (Data::ElementList::iterator iter = orig->eBegin();
             iter != orig->eEnd(); iter = orig->erase(iter))
		{
			//adjs.front()->replace(orig, NULL);
			(*iter)->replace(orig, NULL);

			//adjs.pop_front();
		}

		//Data::FrontElementList fadjs = orig->getAdjacentFrontElements();

		//while (!fadjs.empty())
		for (Data::FrontElementList::iterator iter = orig->feBegin();
             iter != orig->feEnd(); iter = orig->erase(iter))
		{
			//fadjs.front()->replace(orig, NULL);
			(*iter)->replace(orig, NULL);

			//fadjs.pop_front();
		}

		while (!orig->getData().boundaries.empty())
		{
			const_cast<MJMesh::Boundary *>((*orig->getData().boundaries.begin()).first)->remove(orig);
		}

		while (!orig->getData().fronts.empty())
		{
			const_cast<MJMesh::Front *>((*orig->getData().fronts.begin()).first)->remove(orig);
		}

		while (!orig->getData().meshes.empty())
		{
			const_cast<MJMesh::Mesh *>((*orig->getData().meshes.begin()).first)->remove(orig);
		}

		delete orig;

		while (!(*iter).second.temporaries->empty())
		{
			MJMesh::Vertex *tmp = static_cast<MJMesh::Vertex *>((*iter).second.temporaries->front());

			(*iter).second.temporaries->pop_front();

			//adjs = tmp->getAdjacentElements();

			//while (!adjs.empty())
			for (Data::ElementList::iterator iter = tmp->eBegin();
                 iter != tmp->eEnd(); iter = tmp->erase(iter))
			{
				//adjs.front()->replace(tmp, NULL);
				(*iter)->replace(tmp, NULL);

				//adjs.pop_front();
			}

			//fadjs = tmp->getAdjacentFrontElements();

			//while (!fadjs.empty())
			for (Data::FrontElementList::iterator iter = tmp->feBegin();
                 iter != tmp->feEnd(); iter = tmp->erase(iter))
			{
				//fadjs.front()->replace(tmp, NULL);
				(*iter)->replace(tmp, NULL);

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

	this->vertices.clear();
}

void PMJMesh::CrossingReplacer::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::CrossingReplacer::setTasks(const Parallel::TaskVector *tasks)
{
	this->tasks = tasks;
}

void PMJMesh::CrossingReplacer::execute()
{
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
	}

	this->back = !this->back;
}

void PMJMesh::CrossingReplacer::replace()
{
	for (Parallel::TaskVector::const_iterator iter = this->tasks->begin();
		 iter != this->tasks->end(); iter++)
	{
		PMJMesh::MJMeshTask *task = static_cast<PMJMesh::MJMeshTask *>((*iter));
		PMJMesh::Front *front = task->getFront();

		if (!front)
		{
			continue;
		}

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

				//std::cout << "task " << task->getId() << ", will replace fel at " << fel << std::endl;
				//std::cout << "task " << task->getId() << ", will replace fel " << fel->getId() << std::endl;

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
						//std::cout << "task " << task->getId() << ", will replace v at " << v << std::endl;
						//std::cout << "task " << task->getId() << ", will replace v " << v->getId() << std::endl;

						MJMesh::Vertex *tmp = new MJMesh::Vertex(v->getPoint(), v->getId());

						//std::cout << "task " << task->getId() << ", created tmp v at " << tmp << std::endl;
						//std::cout << "task " << task->getId() << ", created tmp v " << tmp->getId() << std::endl;

						(*p.first).second = tmp;

#if USE_C__11
						std::pair<VertexReplacementHashMap::iterator, bool> p2 = this->vertices.insert(std::make_pair(v->getId(), VertexReplacement()));
#else
						std::pair<VertexReplacementMap::iterator, bool> p2 = this->vertices.insert(std::make_pair(v->getId(), VertexReplacement()));
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

							//std::cout << "task " << task->getId() << ", adj at " << adj << ", replacing v for tmp" << std::endl;
							//std::cout << "task " << task->getId() << ", adj " << adj->getId() << ", replacing v for tmp" << std::endl;

							adj->replace(v, tmp);

							//v->remove(adj);

							tmp->add(adj);

							iter = v->erase(iter);
						}

						if (front->have(v))
						{
							front->remove(v);

							//std::cout << "task " << task->getId() << ", front at " << front << ", replacing v for tmp" << std::endl;

							front->add(tmp);
						}
					}

					replacements[i] = (*p.first).second;
				}

				Data::FrontElement *tmp = (this->dimension == 2) ?
					static_cast<Data::FrontElement *>(new MJMesh::FrontElement2D(static_cast<MJMesh::Vertex *>(replacements[0]), static_cast<MJMesh::Vertex *>(replacements[1]), fel->getId())) :
					static_cast<Data::FrontElement *>(new MJMesh::FrontElement3D(static_cast<MJMesh::Vertex *>(replacements[0]), static_cast<MJMesh::Vertex *>(replacements[1]), static_cast<MJMesh::Vertex *>(replacements[2]), fel->getId()));

				//std::cout << "task " << task->getId() << ", created tmp fel at " << tmp << std::endl;
				//std::cout << "task " << task->getId() << ", created tmp fel " << tmp->getId() << std::endl;

#if USE_C__11
				std::pair<FrontElementReplacementHashMap::iterator, bool> p2 = this->frontElements.insert(std::make_pair(fel->getId(), FrontElementReplacement()));
#else
				std::pair<FrontElementReplacementMap::iterator, bool> p2 = this->frontElements.insert(std::make_pair(fel->getId(), FrontElementReplacement()));
#endif //#if USE_C__11

				if (p2.second)
				{
					(*p2.first).second.original = fel;

					(*p2.first).second.temporaries = new Data::FrontElementList();
				}

				(*p2.first).second.temporaries->push_back(tmp);

				frontElements.push_back(tmp);

				//std::cout << "task " << task->getId() << ", front at " << front << ", removing fel at " << fel << std::endl;
				//std::cout << "task " << task->getId() << ", front at " << front << ", removing fel " << fel->getId() << std::endl;
			}

			while (!frontElements.empty())
			{
				//std::cout << "task " << task->getId() << ", front at " << front << ", adding tmp fel at " << frontElements.front() << std::endl;
				//std::cout << "task " << task->getId() << ", front at " << front << ", adding tmp fel " << frontElements.front()->getId() << std::endl;

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
	}
}

void PMJMesh::CrossingReplacer::backReplace(MJMesh::FrontElement *fel, MJMesh::FrontElement *tmp, bool fronts, bool meshes, bool boundaries, bool check, const Data::Boundary *except) const
{
    for (Data::ElementList::iterator iter = tmp->eBegin();
         iter != tmp->eEnd(); iter = tmp->erase(iter))
    {
        MJMesh::Element *adj = static_cast<MJMesh::Element *>((*iter));

        fel->add(adj);
    }

    if (fronts)
    {
        while (!tmp->getData().fronts.empty())
        {
            PMJMesh::Front *front = static_cast<PMJMesh::Front *>(const_cast<MJMesh::Front *>((*tmp->getData().fronts.begin()).first));

            UShort state = (*tmp->getData().fronts.begin()).second.state;

#if USE_GUI
            front->freeze();
#endif //#if USE_GUI

            front->remove(tmp);

            if ((!check) || (!front->have(fel)))
            {
                if (state == 0)
                {
                    front->addBoundary(fel);
                }
                else if (state == 1)
                {
                    front->addElement(fel);
                }
                else if (state == 2)
                {
                    front->addRejected(fel);
                }
                else if (state == 3)
                {
                    front->addCrossing(fel);
                }
            }

#if USE_GUI
            front->unfreeze();
#endif //#if USE_GUI
        }
    }

    if (meshes)
    {
        while (!tmp->getData().meshes.empty())
        {
            MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>((*tmp->getData().meshes.begin()).first);

#if USE_GUI
            mesh->freeze();
#endif //#if USE_GUI

            mesh->remove(tmp, false);

            mesh->add(fel);

#if USE_GUI
            mesh->unfreeze();
#endif //#if USE_GUI
        }
    }

    if (boundaries)
    {
        while (!tmp->getData().boundaries.empty())
        {
            MJMesh::Boundary *boundary = const_cast<MJMesh::Boundary *>((*tmp->getData().boundaries.begin()).first);

#if USE_GUI
            boundary->freeze();
#endif //#if USE_GUI

            if ((!except) || (boundary != except))
            {
                boundary->remove(tmp);

                if ((!check) || (!boundary->have(fel)))
                {
                    boundary->add(fel);
                }
            }

#if USE_GUI
            boundary->unfreeze();
#endif //#if USE_GUI
        }
    }
}

void PMJMesh::CrossingReplacer::backReplace(MJMesh::Vertex *v, MJMesh::Vertex *tmp, bool fronts, bool meshes, bool boundaries, bool check, const Data::Boundary *except) const
{
    for (Data::FrontElementList::iterator iter = tmp->feBegin();
         iter != tmp->feEnd(); iter = tmp->erase(iter))
    {
        Data::FrontElement *adj = (*iter);

        adj->replace(tmp, v);

        v->add(adj);
    }

    for (Data::ElementList::iterator iter = tmp->eBegin();
         iter != tmp->eEnd(); iter = tmp->erase(iter))
    {
        Data::Element *adj = (*iter);

        adj->replace(tmp, v);

        v->add(adj);
    }

    if (fronts)
    {
        while (!tmp->getData().fronts.empty())
        {
            PMJMesh::Front *front = static_cast<PMJMesh::Front *>(const_cast<MJMesh::Front *>((*tmp->getData().fronts.begin()).first));

#if USE_GUI
            front->freeze();
#endif //#if USE_GUI

            front->remove(tmp);

            if ((!check) || (!front->have(v)))
            {
                front->add(v);
            }

#if USE_GUI
            front->unfreeze();
#endif //#if USE_GUI
        }
    }

    if (meshes)
    {
        while (!tmp->getData().meshes.empty())
        {
            MJMesh::Mesh *mesh = const_cast<MJMesh::Mesh *>((*tmp->getData().meshes.begin()).first);

#if USE_GUI
            mesh->freeze();
#endif //#if USE_GUI

            mesh->remove(tmp);

            mesh->add(v);

#if USE_GUI
            mesh->unfreeze();
#endif //#if USE_GUI
        }
    }

    if (boundaries)
    {
        //while (!tmp->getData().boundaries.empty())
        for (MJMesh::VertexSpecificData::BoundaryDataMap::iterator iter = tmp->getData().boundaries.begin();
             iter != tmp->getData().boundaries.end();)
        {
            //MJMesh::Boundary *boundary = const_cast<MJMesh::Boundary *>((*tmp->getData().boundaries.begin()).first);
            MJMesh::Boundary *boundary = const_cast<MJMesh::Boundary *>((*iter).first);

#if USE_GUI
            boundary->freeze();
#endif //#if USE_GUI

            if ((!except) || (boundary != except))
            {
                boundary->remove(tmp);

                if ((!check) || (!boundary->have(v)))
                {
                    boundary->add(v);
                }

                iter = tmp->getData().boundaries.begin();
            }
            else
            {
                iter++;
            }

#if USE_GUI
            boundary->unfreeze();
#endif //#if USE_GUI
        }
    }
}

void PMJMesh::CrossingReplacer::backReplace(MJMesh::FrontElement *fel, Data::FrontElementList *temporaries, const Data::Boundary *delExcept)
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

        this->backReplace(fel, tmp, true, true, false, false);

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

void PMJMesh::CrossingReplacer::backReplace(MJMesh::Vertex *v, Data::VertexList *temporaries, const Data::Boundary *delExcept)
{
    /*{
        std::stringstream str;
        str << "will replace back v at " << v << std::endl;
        std::cout << str.str();
    }
    {
        std::stringstream str;
        str << "will replace back v " << v->getId() << std::endl;
        std::cout << str.str();
    }*/

    while (!temporaries->empty())
    {
        MJMesh::Vertex *tmp = static_cast<MJMesh::Vertex *>(temporaries->front());

        temporaries->pop_front();

        /*{
            std::stringstream str;
            str << "found tmp v at " << tmp << std::endl;
            std::cout << str.str();
        }
        {
            std::stringstream str;
            str << "found tmp v " << tmp->getId() << std::endl;
            std::cout << str.str();
        }*/

        this->backReplace(v, tmp, true, true, false, false);

        tmp->setPoint(NULL);

        if ((!delExcept) || (!delExcept->have(tmp)))
        {
            /*{
                std::stringstream str;
                str << "deleting tmp v at " << tmp << std::endl;
                std::cout << str.str();
            }*/

            delete tmp;
        }
    }

    delete temporaries;
}

void PMJMesh::CrossingReplacer::backReplace()
{
#if USE_C__11
	for (FrontElementReplacementHashMap::iterator iter = this->frontElements.begin();
#else
	for (FrontElementReplacementMap::iterator iter = this->frontElements.begin();
#endif //#if USE_C__11
		 iter != this->frontElements.end(); iter++)
	{
		this->backReplace(static_cast<MJMesh::FrontElement *>((*iter).second.original), (*iter).second.temporaries);
	}

	this->frontElements.clear();

#if USE_C__11
	for (VertexReplacementHashMap::iterator iter = this->vertices.begin();
#else
	for (VertexReplacementMap::iterator iter = this->vertices.begin();
#endif //#if USE_C__11
		 iter != this->vertices.end(); iter++)
	{
		this->backReplace(static_cast<MJMesh::Vertex *>((*iter).second.original), (*iter).second.temporaries);
	}

	this->vertices.clear();
}
