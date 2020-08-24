#include "PMJMesh2/FrontPartitioner.h"

#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/MJMeshTask.h"
#include "PMJMesh2/Communicator.h"

PMJMesh2::FrontPartitioner::FrontPartitioner(UInt dimension, bool addInRejected, PMJMesh::MJMeshTask *task,
            const MJMesh::Front *front) :
    PMJMesh::FrontPartitioner::FrontPartitioner(dimension, front, Parallel::TaskVector(), 0)
{
    this->setTask(task);
    this->setAddInRejected(addInRejected);

    this->rejected = NULL;
}

PMJMesh2::FrontPartitioner::~FrontPartitioner()
{

}

void PMJMesh2::FrontPartitioner::setTask(PMJMesh::MJMeshTask *task)
{
    this->task = task;
}

void PMJMesh2::FrontPartitioner::setAddInRejected(bool addInRejected)
{
    this->addInRejected = addInRejected;
}

#if USE_C__11
Data::FrontElementHash *PMJMesh2::FrontPartitioner::getRejected() const
#else
Data::FrontElementSet *PMJMesh2::FrontPartitioner::getRejected() const
#endif //#if USE_C__11
{
    return this->rejected;
}

bool PMJMesh2::FrontPartitioner::belongTo(const MJMesh::FrontElement *e, PMJMesh::Partition *p, bool &exclusive) const
{
    exclusive = true;

    const Data::Box *box = p->getBox();

	if (this->completelyIn(box, e))
	{
		return true;
	}

	if (!this->out(box, e))
	{
		exclusive = false;

		return true;
	}

	return false;
}

void PMJMesh2::FrontPartitioner::execute()
{
    if ((this->dimension != 2) && (this->dimension != 3))
    {
        return;
    }

    if (!this->task)
    {
        return;
    }

    //if (this->addInRejected)
    {
#if USE_C__11
        this->rejected = new Data::FrontElementHash();
#else
        this->rejected = new Data::FrontElementSet();
#endif //#if USE_C__11
    }

    PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>(this->task->getPartition());

    this->ensureExistenceOfSubfront(p);

    PMJMesh::Front *front = p->getSubfront();

    for (UShort state = 0; state < 4; state++)
    {
        //std::cout << "debug pmjmesh2::frontpartitioner fels at state " << static_cast<UInt>(state) << std::endl;

        bool boundary = (state == 0);

        for (Data::Front::ConstFrontElementIterator iter = this->front->eBegin(state);
             iter != this->front->eEnd(state); iter++)
        {
            MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>((*iter));

            /*if (e->getId() == 740)
            {
                std::cout << "debug pmjmesh2::frontpartitioner found fel " << e->getId() << " at " << e << std::endl;
            }*/

            bool exclusive = true;

            bool belong = this->belongTo(e, p, exclusive);

            if (!belong)
            {
                continue;
            }

            //std::cout << " " << e->text();

            if (exclusive)
            {
                if (boundary)
                {
                    front->addBoundary(e);

                    //std::cout << " in boundary added" << std::endl;
                }
                else
                {
                    front->addElement(e);

                    //std::cout << " in elements added" << std::endl;
                }
            }
            else
            {
                /*if (this->addInRejected)
                {
                    this->rejected->insert(e);
                }
                else
                {
                    front->addRejected(e);
                }*/

                this->rejected->insert(e);

                //std::cout << " in rejected";

                if (this->addInRejected)
                {
                    front->addRejected(e);

                    //std::cout << " added";
                }

                //std::cout << std::endl;
            }
        }

        //std::cout << std::endl;
    }

    for (UShort state = 0; state < 4; state++)
    {
        for (Data::Front::ConstFrontElementIterator iter = front->eBegin(state);
             iter != front->eEnd(state); iter++)
        {
            MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>((*iter));

/*#if USE_GUI
            p->boundary.add(e);
#endif //#if USE_GUI*/

            for (UInt i = 0; i < e->numVertices(); i++)
            {
                MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(e->getVertex(i));

/*#if USE_GUI
                if (!p->boundary.have(v))
                {
                    p->boundary.add(v);
                }
#endif //#if USE_GUI*/

                if (front->have(v))
                {
                    continue;
                }

                if (this->addInRejected)
                {
                    front->add(v);

                    continue;
                }

                //Data::FrontElementList adjs = v->getAdjacentFrontElements();

                bool found = false;

                //while (!adjs.empty())
                for (Data::FrontElementList::iterator iter = v->feBegin();
                     iter != v->feEnd(); iter++)
                {
                    //Data::FrontElement *adj = adjs.front();
                    Data::FrontElement *adj = (*iter);

                    //adjs.pop_front();

                    if (adj == e)
                    {
                        continue;
                    }

                    if (this->rejected->find(adj) != this->rejected->end())
                    {
                        found = true;

                        break;
                    }
                }

                if (found)
                {
                    continue;
                }

                front->add(v);
            }
        }
    }
}
