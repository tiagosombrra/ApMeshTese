#include "PMJMesh/FrontPartitioner.h"

#include "PMJMesh/Front.h"
#include "PMJMesh/Partition.h"
#include "PMJMesh/MJMeshTask.h"
#include "MJMesh/FrontElement.h"
#include "Data/Vertex.h"
#include "PMJMesh/CrossingReplacer.h"
#if USE_EXPERIMENTAL_3
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#endif //#if USE_EXPERIMENTAL_3

PMJMesh::FrontPartitioner::FrontPartitioner(UInt dimension, const MJMesh::Front *front,
	const Parallel::PartitionVector &partitions) :
	Parallel::Partitioner()
{
	this->setDimension(dimension);
	this->setFront(front);
	this->setPartitions(partitions);

	this->setShift(0);

	this->setReplaceCrossing(false);
	this->replacer = NULL;
}

PMJMesh::FrontPartitioner::FrontPartitioner(UInt dimension, const MJMesh::Front *front,
	const Parallel::TaskVector &tasks, UInt shift, bool replaceCrossing) :
	Parallel::Partitioner()
{
	this->setDimension(dimension);
	this->setFront(front);
	this->setTasks(tasks);

	this->setShift(shift);

	this->setReplaceCrossing(replaceCrossing);
	this->replacer = NULL;
}

PMJMesh::FrontPartitioner::~FrontPartitioner()
{
	//this->front = NULL;

	if (this->replacer)
	{
		delete this->replacer;
	}
}

void PMJMesh::FrontPartitioner::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::FrontPartitioner::setFront(const MJMesh::Front *front)
{
	this->front = front;
}

void PMJMesh::FrontPartitioner::setPartitions(const Parallel::PartitionVector &partitions)
{
	this->partitions = partitions;
}

void PMJMesh::FrontPartitioner::setTasks(const Parallel::TaskVector &tasks)
{
	this->tasks = tasks;
}

void PMJMesh::FrontPartitioner::setShift(UInt shift)
{
	this->shift = shift;
}

void PMJMesh::FrontPartitioner::setReplaceCrossing(bool replaceCrossing)
{
	this->replaceCrossing = replaceCrossing;
}

void PMJMesh::FrontPartitioner::setReplacer(PMJMesh::CrossingReplacer *replacer)
{
	this->replacer = replacer;
}

PMJMesh::CrossingReplacer *PMJMesh::FrontPartitioner::getReplacer() const
{
	return this->replacer;
}

void PMJMesh::FrontPartitioner::execute()
{
	if ((!this->front) || ((this->partitions.empty()) && (this->tasks.empty())))
	{
		return;
	}

	//std::cout << "beginning front partitioner" << std::endl;

	//std::cout << "front->size = " << this->front->size() << std::endl;

	//Data::FrontElementList fels = this->front->getBoundary();

	//std::cout << "front->boundary->size = " << fels.size() << std::endl;

	//this->findPartitions(fels, true);
	this->findPartitions(0, true);

	//fels = this->front->getElements();
	//Data::FrontElementList fels = this->front->getElements();

	//std::cout << "front->elements->size = " << fels.size() << std::endl;

	//this->findPartitions(fels, false);
	this->findPartitions(1, false);

	//fels = this->front->getRejected();
	//Data::FrontElementList fels = this->front->getRejected();

	//std::cout << "front->rejected->size = " << fels.size() << std::endl;

	//this->findPartitions(fels, false);
	this->findPartitions(2, false);

	/*for (UInt i = 0; i < this->partitions.size(); i++)
	{
		PMJMesh::Partition *p = (PMJMesh::Partition *)this->partitions[i];

		std::cout << "partition " << p->getId() << " has " << (p->getSubfront() ? p->getSubfront()->size() : 0) << " elements" << std::endl;
		std::cout << "partition " << p->getId() << " has " << (p->getSubfront() ? p->getSubfront()->getBoundary().size() : 0) << " elements in boundary" << std::endl;
		std::cout << "partition " << p->getId() << " has " << (p->getSubfront() ? p->getSubfront()->getElements().size() : 0) << " elements in elements" << std::endl;
		std::cout << "partition " << p->getId() << " has " << (p->getSubfront() ? p->getSubfront()->getRejected().size() : 0) << " elements in rejected" << std::endl;
	}*/

	if (this->replaceCrossing)
	{
		if (this->replacer)
		{
			this->replacer->setDimension(this->dimension);
			this->replacer->setTasks(&this->tasks);
		}
		else
		{
			this->replacer = new CrossingReplacer(this->dimension, &this->tasks);
		}

		this->replacer->execute();
	}
}

bool PMJMesh::FrontPartitioner::out(const Data::Box *box, const MJMesh::FrontElement *e) const
{
	//test if the two bounding boxes intercept
	//if (!box.intercept(e->getShape()->box()))
	if (!e->getShape()->box().optIntercept(box))
	{
		//the two bounding boxes do not intercept each other
		return true;
	}

#if USE_EXPERIMENTAL_3
	return (this->dimension == 2) ?
        this->clip2D(box, e->getShape()).empty() :
        this->clip3D(box, e->getShape()).empty();
#else
	return this->clip(box, e->getShape()).empty();
#endif //#if USE_EXPERIMENTAL_3
}

bool PMJMesh::FrontPartitioner::completelyIn(const Data::Box *box, const MJMesh::FrontElement *e) const
{
	bool in = true;

	//Data::VertexVector vertices = e->getVertices();

	//for (Data::VertexVector::iterator iter = vertices.begin();
	//	 iter != vertices.end(); iter++)
	for (UInt i = 0; i < e->numVertices(); i++)
	{
		//if (!box->in((*iter)->getPoint()))
		//if (!box->in(e->getVertex(i)->getPoint()))
		if (!box->optIn(e->getVertex(i)->getPoint()))
		{
			in = false;

			break;
		}
	}

	return in;
}

#if USE_EXPERIMENTAL_3
Data::Point2DObjList PMJMesh::FrontPartitioner::clip2D(const Data::Box *box, const Data::GeometricShape *s) const
{
	Performer::Clipper clipper(this->dimension, *box, s, Performer::Clipper::EDGE);

	clipper.execute();

	return clipper.getPoints2D();
}

Data::Point3DObjList PMJMesh::FrontPartitioner::clip3D(const Data::Box *box, const Data::GeometricShape *s) const
{
	Performer::Clipper clipper(this->dimension, *box, s, Performer::Clipper::POLYGON);

	clipper.execute();

	return clipper.getPoints3D();
}
#else
Data::PointObjList PMJMesh::FrontPartitioner::clip(const Data::Box *box, const Data::GeometricShape *s) const
{
	Performer::Clipper clipper(this->dimension, *box, s,
		(this->dimension == 2) ? Performer::Clipper::EDGE : Performer::Clipper::POLYGON);

	clipper.execute();

	return clipper.getPoints();
}
#endif //#if USE_EXPERIMENTAL_3

void PMJMesh::FrontPartitioner::ensureExistenceOfSubfront(PMJMesh::Partition *p) const
{
	if (!p->getSubfront())
	{
		p->setSubfront(new PMJMesh::Front(this->front->isSorted()));

#if USE_SET
		static_cast<PMJMesh::Front *>(p->getSubfront())->initialize();
#endif //#if USE_SET
	}
}

bool PMJMesh::FrontPartitioner::belongTo(const MJMesh::FrontElement *e, PMJMesh::Partition *p, Parallel::PartitionList &partitions, bool &exclusive) const
{
	const Data::Box *box = p->getBox();

	if ((partitions.empty()) && (this->completelyIn(box, e)))
	{
		partitions.push_back(p);

		return true;
	}

	if (!this->out(box, e))
	{
		partitions.push_back(p);

		exclusive = false;
	}

	return false;
}

//Parallel::PartitionList PMJMesh::FrontPartitioner::findPartitions(const MJMesh::FrontElement *e) const
bool PMJMesh::FrontPartitioner::findPartitions(const MJMesh::FrontElement *e, Parallel::PartitionList &partitions) const
{
	//Parallel::PartitionList partitions;

	bool exclusive = true;

	if (this->tasks.empty())
	{
		for (Parallel::PartitionVector::const_iterator iter = this->partitions.begin();
			 iter != this->partitions.end(); iter++)
		{
			PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>((*iter));

			if (this->belongTo(e, p, partitions, exclusive))
			{
				break;
			}
		}
	}
	else
	{
		for (Parallel::TaskVector::const_iterator iter = this->tasks.begin();
			 iter != this->tasks.end(); iter++)
		{
			if (static_cast<PMJMesh::MJMeshTask *>((*iter))->isGenerated(this->shift))
			{
				continue;
			}

			PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>((*iter)->getPartition());

			if (this->belongTo(e, p, partitions, exclusive))
			{
				break;
			}
		}
	}

	//return partitions;
	return exclusive;
}

//void PMJMesh::FrontPartitioner::findPartitions(Data::FrontElementList &fels, bool boundary)
void PMJMesh::FrontPartitioner::findPartitions(UShort state, bool boundary)
{
	//std::cout << "beginning front partitioner for state " << static_cast<UInt>(state) << ", of size " << this->front->size(state) << std::endl;

	//while (!fels.empty())
	for (Data::Front::ConstFrontElementIterator iter = this->front->eBegin(state);
		 iter != this->front->eEnd(state); iter++)
	{
		//MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>(fels.front());
		MJMesh::FrontElement *e = static_cast<MJMesh::FrontElement *>((*iter));

		//fels.pop_front();

		//std::cout << "testing fel " << e->text() << std::endl;

		//Parallel::PartitionList partitions = this->findPartitions(e);
		Parallel::PartitionList partitions;

		bool crossing = !this->findPartitions(e, partitions);

		if (partitions.empty())
		{
			//std::cout << "fel " << e->text() << " is not in any partition" << std::endl;

			continue;
		}

		if (partitions.size() == 1)
		{
			PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>(partitions.back());

			this->ensureExistenceOfSubfront(p);

			//std::cout << "fel " << e->text() << " is only in partition " << p->getId() << ", whose front has size " << p->getSubfront()->size() << std::endl;

			/*if (crossing)
			{
				p->getSubfront()->addRejected(e);
			}
			else if (boundary)
			{
				p->getSubfront()->addBoundary(e);
			}*/
			if (boundary)
			{
				p->getSubfront()->addBoundary(e);
			}
			else if (crossing)
			{
				p->getSubfront()->addRejected(e);
			}
			else
			{
				p->getSubfront()->addElement(e);
			}
		}
		else
		{
			for (Parallel::PartitionList::iterator iter = partitions.begin();
				 iter != partitions.end(); iter++)
			{
				PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>((*iter));

				this->ensureExistenceOfSubfront(p);

				//std::cout << "fel " << e->text() << " is also in partition " << p->getId() << ", whose front has size " << p->getSubfront()->size() << std::endl;

				p->getSubfront()->addRejected(e);
				//p->getSubfront()->addCrossing(e);
			}
		}

		//Data::VertexVector vertices = e->getVertices();

		while (!partitions.empty())
		{
			PMJMesh::Partition *p = static_cast<PMJMesh::Partition *>(partitions.back());

			//debug
			//std::cout << "element " << e->getId() << " goes to partition " << p->getId() << std::endl;
			//endebug

			partitions.pop_back();

			//for (Data::VertexVector::iterator iter = vertices.begin();
			//	 iter != vertices.end(); iter++)
			for (UInt i = 0; i < e->numVertices(); i++)
			{
				//Data::Vertex *v = (*iter);
				Data::Vertex *v = e->getVertex(i);

				if (!p->getSubfront()->have(v))
				{
					p->getSubfront()->add(v);
				}
			}
		}
	}
}
