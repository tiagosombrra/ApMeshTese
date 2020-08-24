#include "PMJMesh/AbstractMeshUpdater.h"

#if USE_NEW_FRONT_UPDATER

#include "PMJMesh/Communicator.h"
#include "PMJMesh/Front.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement.h"
#include "MJMesh/Element.h"

PMJMesh::AbstractMeshUpdater::AbstractMeshUpdater(UInt dimension,
	Data::AbstractMesh *mesh, Parallel::Communicator *comm, bool external)
{
	this->setDimension(dimension);
	this->setMesh(mesh);
	this->setComm(comm);
	this->setExternal(external);

	this->setMasterTask(NULL);
	this->setWorkerTask(NULL);

	this->updated = false;
}

PMJMesh::AbstractMeshUpdater::~AbstractMeshUpdater()
{

}

void PMJMesh::AbstractMeshUpdater::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::AbstractMeshUpdater::setMesh(Data::AbstractMesh *mesh)
{
	this->mesh = mesh;
}

void PMJMesh::AbstractMeshUpdater::setComm(Parallel::Communicator *comm)
{
	this->comm = comm;
}

void PMJMesh::AbstractMeshUpdater::setExternal(bool external)
{
	this->external = external;
}

void PMJMesh::AbstractMeshUpdater::setMasterTask(MJMeshTask *masterTask)
{
	this->masterTask = masterTask;
}

void PMJMesh::AbstractMeshUpdater::setWorkerTask(MJMeshTask *workerTask)
{
	this->workerTask = workerTask;
}

bool PMJMesh::AbstractMeshUpdater::isUpdated() const
{
	return this->updated;
}

void PMJMesh::AbstractMeshUpdater::initialize()
{
	this->updated = false;

	if ((!this->comm) || (!this->comm->isParallel()))
	{
		if (!this->mesh)
		{
			return;
		}

		this->initializeShared();

		return;
	}

	if (this->comm->isMaster())
	{
		if (!this->mesh)
		{
			return;
		}

		this->initializeMaster();

		return;
	}

	this->initializeWorker();
}

void PMJMesh::AbstractMeshUpdater::finalize()
{
	if ((!this->comm) || (!this->comm->isParallel()))
	{
		if (!this->mesh)
		{
			return;
		}

		this->finalizeShared();

		return;
	}

	if (this->comm->isMaster())
	{
		if (!this->mesh)
		{
			return;
		}

		this->finalizeMaster();

		return;
	}

	this->finalizeWorker();
}

void PMJMesh::AbstractMeshUpdater::finalizeMesh()
{
	if ((!this->comm) || (!this->comm->isParallel()))
	{
		if (!this->mesh)
		{
			return;
		}

		this->finalizeMeshShared();

		return;
	}

	if (this->comm->isMaster())
	{
		if (!this->mesh)
		{
			return;
		}

		this->finalizeMeshMaster();

		return;
	}

	this->finalizeMeshWorker();
}

void PMJMesh::AbstractMeshUpdater::execute()
{
	if ((!this->comm) || (!this->comm->isParallel()))
	{
		if ((!this->mesh) || (!this->masterTask))
		{
			return;
		}

		this->executeShared();

		return;
	}

	if (this->comm->isMaster())
	{
		if ((!this->mesh) || (!this->masterTask) || (!this->workerTask))
		{
			return;
		}

		this->executeMaster();

		return;
	}

	this->executeWorker();
}

void PMJMesh::AbstractMeshUpdater::replace(MJMesh::Vertex *temporary, MJMesh::Vertex *definitive, bool checkExistence, PMJMesh::Front *except) const
{
#if USE_THREAD_COLOR
	definitive->setColor(temporary->getR(), temporary->getG(), temporary->getB());
#endif //#if USE_THREAD_COLOR

	//Data::ElementList els = temporary->getAdjacentElements();

	//Data::ElementList defEls = checkExistence ? definitive->getAdjacentElements() : Data::ElementList();

	//while (!els.empty())
    for (Data::ElementList::iterator iter = temporary->eBegin();
         iter != temporary->eEnd(); iter = temporary->erase(iter))
	{
		//Data::Element *e = els.front();
		Data::Element *e = (*iter);

		//els.pop_front();

		/*bool found = false;

		for (Data::ElementList::iterator iter = defEls.begin();
			 iter != defEls.end(); iter++)
		{
			if ((*iter)->getId() == e->getId())
			{
				found = true;

				break;
			}
		}

		if (found)
		{
			continue;
		}*/

		e->replace(temporary, definitive);

		//temporary->remove(e);

		definitive->add(e);
	}

	//Data::FrontElementList fels = temporary->getAdjacentFrontElements();

	//Data::FrontElementList defFEls = checkExistence ? definitive->getAdjacentFrontElements() : Data::FrontElementList();

	//while (!fels.empty())
	for (Data::FrontElementList::iterator iter = temporary->feBegin();
         iter != temporary->feEnd();)
	{
		//Data::FrontElement *e = fels.front();
		Data::FrontElement *e = (*iter);

		//fels.pop_front();

		if ((except) && (except->have(e)))
		{
		    iter++;

			continue;
		}

		if (checkExistence)
        {
            bool found = false;

            //for (Data::FrontElementList::iterator iter = defFEls.begin();
            for (Data::FrontElementList::iterator iter2 = definitive->feBegin();
                 //iter != defFEls.end(); iter++)
                 iter2 != definitive->feEnd(); iter2++)
            {
                if ((*iter2)->getId() == e->getId())
                {
                    found = true;

                    break;
                }
            }

            if (found)
            {
                iter++;

                continue;
            }
        }

		e->replace(temporary, definitive);

		//temporary->remove(e);

		definitive->add(e);

		iter = temporary->erase(iter);
	}
}

void PMJMesh::AbstractMeshUpdater::replace(MJMesh::FrontElement *temporary, MJMesh::FrontElement *definitive/*, bool checkExistence*/) const
{
#if USE_THREAD_COLOR
	definitive->setColor(temporary->getR(), temporary->getG(), temporary->getB());
#endif //#if USE_THREAD_COLOR

	//Data::ElementList els = temporary->getAdjacency();
	//Data::ElementList defEls = checkExistence ? definitive->getAdjacency() : Data::ElementList();

	//while (!els.empty())
	for (Data::ElementList::iterator iter = temporary->eBegin();
         iter != temporary->eEnd(); iter = temporary->erase(iter))
	{
		//MJMesh::Element *e = static_cast<MJMesh::Element *>(els.front());
		MJMesh::Element *e = static_cast<MJMesh::Element *>((*iter));

		//els.pop_front();

		/*bool found = false;

		for (Data::ElementList::iterator iter = defEls.begin();
			 iter != defEls.end(); iter++)
		{
			if ((*iter)->getId() == e->getId())
			{
				found = true;

				break;
			}
		}

		if (found)
		{
			continue;
		}*/

		//temporary->remove(e);
		definitive->add(e);
	}

	for (UInt i = 0; i < temporary->numVertices(); i++)
	{
		MJMesh::Vertex *v = static_cast<MJMesh::Vertex *>(temporary->getVertex(i));

		if (v)
		{
			v->remove(temporary);
		}

		temporary->setVertex(i, NULL);
	}
}

#endif //#if USE_NEW_FRONT_UPDATER
