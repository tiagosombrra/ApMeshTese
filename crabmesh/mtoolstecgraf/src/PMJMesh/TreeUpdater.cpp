#include "PMJMesh/TreeUpdater.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "Parallel/Communicator.h"
#include "PMJMesh/TreeTransferable.h"

PMJMesh::TreeUpdater::TreeUpdater(UInt dimension, Data::BoxTree *tree, Parallel::Communicator *comm) :
	Parallel::Builder::Builder(comm)
{
	this->setDimension(dimension);
	this->setTree(tree);

	this->trees = NULL;
}

PMJMesh::TreeUpdater::~TreeUpdater()
{
	if (this->trees)
	{
		Int numProcesses = this->comm->numProcesses();

		for (Int i = 0; i < numProcesses; i++)
		{
			if (this->trees[i])
			{
				delete this->trees[i];
			}
		}

		delete [] this->trees;
	}
}

/*void print(Data::UInt dimension, const Data::BoxTreeCell *c, const Parallel::Communicator *comm)
{
	std::cout << "print, cell " << c->getId() << ", pos " << PMJMesh::getPosition(dimension, c) << ", proc " << comm->rank() << std::endl;

	for (Data::UInt i = 0; i < c->numChildren(); i++)
	{
		print(dimension, static_cast<Data::BoxTreeCell *>(c->child(i)), comm);
	}
}

void debug(Data::UInt dimension, const Data::BoxTreeCell *c, const Parallel::Communicator *comm)
{
	if (PMJMesh::getPosition(dimension, c) == Data::UNKNOWN)
	{
		std::cout << "debug, cell " << c->getId() << " has UNKNOWN position in proc " << comm->rank() << std::endl;
	}

	for (Data::UInt i = 0; i < c->numChildren(); i++)
	{
		debug(dimension, static_cast<Data::BoxTreeCell *>(c->child(i)), comm);
	}
}*/

void PMJMesh::TreeUpdater::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::TreeUpdater::setTree(Data::BoxTree *tree)
{
	this->tree = tree;
}

void PMJMesh::TreeUpdater::execute()
{
	//print(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->comm);

	if (this->comm->isSlave())
	{
#if USE_GUI
		static_cast<Data::BoxTree *>(this->tree)->freeze();
#endif //#if USE_GUI

		//std::cout << "before local refine, slave " << this->comm->rank() << std::endl;
		//debug(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->comm);

		this->refine(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), false);

#if (!USE_EXPERIMENTAL_TREE)
		this->tree->setLeaves(this->leaves);
#endif //#if (!USE_EXPERIMENTAL_TREE)

		//std::cout << "after local refine, slave " << this->comm->rank() << std::endl;
		//debug(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->comm);

#if USE_GUI
		static_cast<Data::BoxTree *>(this->tree)->unfreeze();
#endif //#if USE_GUI
	}

	if (this->comm->isParallel())
	{
		this->updateTree();
	}

	if (this->comm->isMaster())
	{
#if USE_GUI
		static_cast<Data::BoxTree *>(this->tree)->freeze();
#endif //#if USE_GUI

		//std::cout << "before local refine, master" << std::endl;
		//debug(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->comm);

		this->refine(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), false);

#if (!USE_EXPERIMENTAL_TREE)
		this->tree->setLeaves(this->leaves);
#endif //#if (!USE_EXPERIMENTAL_TREE)

		//std::cout << "after local refine, master" << std::endl;
		//debug(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->comm);

#if USE_GUI
		static_cast<Data::BoxTree *>(this->tree)->unfreeze();
#endif //#if USE_GUI
	}
}

void PMJMesh::TreeUpdater::gather()
{
	//tree will be in array trees, and does not need to be deleted...
	TreeTransferable *tree = new PMJMesh::TreeTransferable(this->dimension, this->tree);

	Parallel::Transferable **trees = this->comm->allGather(tree);

	Int numProcesses = this->comm->numProcesses();

	this->trees = new Data::BoxTree*[numProcesses];

	for (Int i = 0; i < numProcesses; i++)
	{
		this->trees[i] = static_cast<PMJMesh::TreeTransferable *>(trees[i])->getTree();

		//...because it will be deleted right here!
		delete trees[i];
	}

	delete [] trees;
}

void PMJMesh::TreeUpdater::update(Data::BoxTreeCell *cell, Data::BoxTreeCell **others, Int numOthers, Int rank, Int root)
{
	/*if (!others[root])
	{
		return;
	}*/

	enum Data::Position orig = others[root] ? PMJMesh::getPosition(this->dimension, others[root]) : PMJMesh::getPosition(this->dimension, cell);

	/*if (orig == Data::UNKNOWN)
	{
		std::stringstream str;
		str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, original = UNKNOWN" << std::endl;
		std::cout << str.str();
	}*/

	enum Data::Position curr = Data::UNKNOWN;

	bool subdivided = false;

	for (Int i = 0; i < numOthers; i++)
	{
		if ((!others[i]) || (i == root))
		{
			continue;
		}

		enum Data::Position pos = PMJMesh::getPosition(this->dimension, others[i]);

		/*if (pos == Data::UNKNOWN)
		{
			std::stringstream str;
			str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, other = UNKNOWN" << std::endl;
			std::cout << str.str();
		}*/

		if (curr == Data::UNKNOWN)
		{
			if (orig == Data::ON)
			{
				if (pos == Data::ON)
				{
					curr = Data::ON;

					if (others[i]->isSubdivided())
					{
						subdivided = true;
					}
				}
				/*else if (pos == Data::IN)
				{
					std::stringstream str;
					str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, original = ON, other = IN" << std::endl;
					std::cout << str.str();
				}*/
				else if (pos == Data::OUT)
				{
					curr = Data::OUT;

					subdivided = false;
				}
			}
			else if (orig == Data::IN)
			{
				if (pos == Data::ON)
				{
					curr = Data::ON;

					if (others[i]->isSubdivided())
					{
						subdivided = true;
					}
				}
				else if (pos == Data::IN)
				{
					curr = Data::IN;
				}
				else if (pos == Data::OUT)
				{
					curr = Data::OUT;

					subdivided = false;
				}
			}
			else if (orig == Data::OUT)
			{
				/*if (pos == Data::ON)
				{
					std::stringstream str;
					str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, original = OUT, other = ON" << std::endl;
					std::cout << str.str();
				}
				else if (pos == Data::IN)
				{
					std::stringstream str;
					str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, original = OUT, other = IN" << std::endl;
					std::cout << str.str();
				}
				else */if (pos == Data::OUT)
				{
					curr = Data::OUT;

					subdivided = false;
				}
			}
		}
		else
		{
			if (curr == Data::ON)
			{
				if (pos == Data::ON)
				{
					curr = Data::ON;

					if (others[i]->isSubdivided())
					{
						subdivided = true;
					}
				}
				else if (pos == Data::IN)
				{
					/*if (orig == Data::ON)
					{
						std::stringstream str;
						str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, original = ON, current = ON, other = IN" << std::endl;
						std::cout << str.str();
					}
					else*/
					{
						curr = Data::ON;

						if (others[i]->isSubdivided())
						{
							subdivided = true;
						}
					}
				}
				else if (pos == Data::OUT)
				{
					/*if (orig == Data::IN)
					{
						std::stringstream str;
						str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, original = IN, current = ON, other = OUT" << std::endl;
						std::cout << str.str();
					}
					else*/
					{
						curr = Data::OUT;

						subdivided = false;
					}
				}
			}
			else if (curr == Data::IN)
			{
				if (pos == Data::ON)
				{
					curr = Data::ON;

					if (others[i]->isSubdivided())
					{
						subdivided = true;
					}
				}
				else if (pos == Data::IN)
				{
					curr = Data::IN;
				}
				else if (pos == Data::OUT)
				{
					curr = Data::OUT;

					subdivided = false;
				}
			}
			else if (curr == Data::OUT)
			{
				if (pos == Data::ON)
				{
					/*if (orig == Data::IN)
					{
						std::stringstream str;
						str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, original = IN, current = OUT, other = ON" << std::endl;
						std::cout << str.str();
					}
					else if (orig == Data::OUT)
					{
						std::stringstream str;
						str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, original = OUT, current = OUT, other = ON" << std::endl;
						std::cout << str.str();
					}
					else*/
					{
						curr = Data::OUT;

						subdivided = false;
					}
				}
				else if (pos == Data::IN)
				{
					/*if (orig == Data::ON)
					{
						std::stringstream str;
						str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, original = ON, current = OUT, other = IN" << std::endl;
						std::cout << str.str();
					}
					else if (orig == Data::OUT)
					{
						std::stringstream str;
						str << "rank " << rank << ", cell = " << cell->getId() << ", ERROR, original = OUT, current = OUT, other = IN" << std::endl;
						std::cout << str.str();
					}
					else*/
					{
						curr = Data::OUT;

						subdivided = false;
					}
				}
			}
		}
	}

	//if (orig != curr)
	{
		/*std::stringstream str;
		str << "rank " << rank << ", cell = " << cell->getId() << ", setting position to " <<
			((curr == Data::ON) ? "ON" : ((curr == Data::IN) ? "IN" : ((curr == Data::OUT) ? "OUT" : "UNKNOWN"))) << std::endl;
		str << "rank " << rank << ", cell = " << cell->getId() << ", subdivided = " << std::boolalpha << subdivided << std::endl;*/

		if ((cell->isLeaf()) && (subdivided))
		{
			this->subdivide(cell);
			//str << "rank " << rank << ", cell = " << cell->getId() << ", subdividing" << std::endl;
		}
		else if ((curr == Data::OUT) && (cell->isSubdivided()))
		{
			cell->deleteChildren();
			//str << "rank " << rank << ", cell = " << cell->getId() << ", deleting children" << std::endl;
		}

		PMJMesh::setPosition(this->dimension, cell, curr);

		//std::cout << str.str();
	}

	if (cell->isLeaf())
	{
		return;
	}

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		for (Int j = 0; j < numOthers; j++)
		{
			others[numOthers + j] = ((!others[j]) || (others[j]->isLeaf())) ? NULL :
				static_cast<Data::BoxTreeCell *>(others[j]->child(i));
		}

		this->update(static_cast<Data::BoxTreeCell *>(cell->child(i)), others + numOthers, numOthers, rank, root);
	}
}

void PMJMesh::TreeUpdater::update()
{
	Int numProcesses = this->comm->numProcesses();
	Int rank = this->comm->rank();
	Int root = this->comm->root();

	//level 0 is for the root...
	UInt maxLevel = this->tree->depth();

	for (Int i = 0; i < numProcesses; i++)
	{
		UInt level = ((i == rank) || (!this->trees[i])) ? 0 : this->trees[i]->depth();

		if (level > maxLevel)
		{
			maxLevel = level;
		}
	}

	//...thus, the (maxLevel + 1) below
	Data::BoxTreeCell **others = new Data::BoxTreeCell*[numProcesses*(maxLevel + 1)];

	for (Int i = 0; i < numProcesses; i++)
	{
		others[i] = (/*(i == rank) || */(!this->trees[i])) ? NULL :
			static_cast<Data::BoxTreeCell *>(this->trees[i]->getRoot());
	}

	this->update(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), others, numProcesses, rank, root);

	delete [] others;

	for (Int i = 0; i < numProcesses; i++)
	{
		if ((i != rank) && (this->trees[i]))
		{
			delete this->trees[i];
		}
	}

	delete [] this->trees;

	this->trees = NULL;
}

void PMJMesh::TreeUpdater::updateTree()
{
	/*if (this->comm->isSlave())
	{
		std::cout << "before gather, slave " << this->comm->rank() << std::endl;
		debug(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->comm);
	}*/

	this->gather();

	/*if (this->comm->isSlave())
	{
		std::cout << "between gather and update, slave " << this->comm->rank() << std::endl;
		debug(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->comm);
	}*/

	this->update();

	/*if (this->comm->isSlave())
	{
		std::cout << "after update, slave " << this->comm->rank() << std::endl;
		debug(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()), this->comm);
	}*/
}

void PMJMesh::TreeUpdater::refine(Data::BoxTreeCell *c, bool force)
{
	enum Data::Position pos = PMJMesh::getPosition(this->dimension, c);

	/*//debug
	if (pos == Data::UNKNOWN)
	{
		std::cout << "PMJMesh::TreeUpdater::refine, cell " << c->getId() << " is UNKNOWN in proc " << this->comm->rank() << std::endl;
	}
	//endebug*/

	if ((pos != Data::ON) && (c->isSubdivided()))
	{
		for (UInt i = 0; i < c->numChildren(); i++)
		{
			if (PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(c->child(i))) != pos)
			{
				pos = Data::UNKNOWN;

				break;
			}
		}

		if (pos == Data::OUT)
		{
			c->deleteChildren();
		}
	}

	if ((!c->getTree()) || (force))
	{
		c->setTree(this->tree);

		force = true;
	}

	if (c->isLeaf())
	{
		this->leaves.push_back(c);

		return;
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->refine(static_cast<Data::BoxTreeCell *>(c->child(i)), force);
	}
}

void PMJMesh::TreeUpdater::subdivide(Data::TreeNode *n)
{
	static_cast<Data::BoxTreeCell *>(n)->subdivide();
}
