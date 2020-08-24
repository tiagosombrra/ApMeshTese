#include "PMJMesh/WorkerCellClassifier.h"

#if (!USE_WORKER_CELL_CLASSIFIER_2)

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "Data/Box.h"
#include "Data/FrontElement.h"

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsInit = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsInTest = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsOutOfBoxTest = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsOldPosOn = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsNotOnCommonAndNotNewTest = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsNotOnCommonAndNotNew = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsCommonNewOld = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsOnCommonOrNewTest = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsOnCommonOrNew = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsOnOldTest = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsOnOld = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOnCellsElse = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindInCellsCells = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindInCellsNeighbors = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindInCellsParents = 0.0;
Data::Real PMJMesh::WorkerCellClassifier::timeFindOutCells = 0.0;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

PMJMesh::WorkerCellClassifier::WorkerCellClassifier(UInt dimension,
	const Data::BoxTree *tree, const Data::Box *box,
	const Data::FrontElementList &oldFront,
	const Data::FrontElementList &newFront,
	const Data::FrontElementList &commonFront) :
	//PMJMesh::AngleTreeCellClassifier(dimension, static_cast<Parallel::ThreadManager *>(NULL), static_cast<Data::Front *>(NULL), tree)
	PMJMesh::AngleTreeCellClassifier(dimension, static_cast<Parallel::ThreadManager *>(NULL), NULL, tree)
{
	this->setBox(box);

	this->oldFront = oldFront;
	this->newFront = newFront;
	this->commonFront = commonFront;
}

PMJMesh::WorkerCellClassifier::~WorkerCellClassifier()
{

}

void PMJMesh::WorkerCellClassifier::setBox(const Data::Box *box)
{
	this->box = box;
}

void PMJMesh::WorkerCellClassifier::findCells(const Data::BoxTreeCell *c, const Data::FrontElementList &fels,
	bool geometricTests, Data::GraphNodeSet &cells) const
{
	for (Data::FrontElementList::const_iterator iter = fels.begin();
		 iter != fels.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(c->find(fel->getShape()->mid()));

		if ((!geometricTests) || (!this->outOfBox(cell)))
		{
			cells.insert(cell);
		}

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(c->find(fel->getVertex(i)->getPoint()));

			if ((!geometricTests) || (!this->outOfBox(cell)))
			{
				cells.insert(cell);
			}
		}
	}
}

void PMJMesh::WorkerCellClassifier::findOnCells()
{
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

	Data::BoxTreeCell *root = static_cast<Data::BoxTreeCell *>(this->tree->getRoot());

	//std::cout << "PMJMesh::WorkerCellClassifier::findOnCells, time for getting root = " << Data::time() - debugstart << std::endl;

	//Data::GraphNodeSet cells;
	Data::GraphNodeList cells;

	//debugstart = Data::time();

	for (Data::FrontElementList::const_iterator iter = this->newFront.begin();
		 iter != this->newFront.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

		Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(root->find(fel->getShape()->mid()));

		//std::cout << "(on) cell " << cell->getId() << " was classified as ON" << std::endl;

		if (PMJMesh::getPosition(this->dimension, cell) != Data::ON)
		{
			PMJMesh::setPosition(this->dimension, cell, Data::ON);

			//cells.insert(cell->parent());
			cells.push_back(cell->parent());
		}

		for (UInt i = 0; i < fel->numVertices(); i++)
		{
			Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(root->find(fel->getVertex(i)->getPoint()));

			//std::cout << "(on) cell " << cell->getId() << " was classified as ON" << std::endl;

			if (PMJMesh::getPosition(this->dimension, cell) != Data::ON)
			{
				PMJMesh::setPosition(this->dimension, cell, Data::ON);

				//cells.insert(cell->parent());
				cells.push_back(cell->parent());
			}
		}
	}

	//std::cout << "PMJMesh::WorkerCellClassifier::findOnCells, time for setting ON in newFront cells = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	//for (Data::GraphNodeSet::iterator iter = cells.begin();
	for (Data::GraphNodeList::iterator iter = cells.begin();
		 iter != cells.end(); iter++)
	{
		Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>((*iter));

		while ((cell) && (PMJMesh::getPosition(this->dimension, cell) != Data::ON))
		{
			//std::cout << "(on, parent) cell " << cell->getId() << " was classified as ON" << std::endl;

			PMJMesh::setPosition(this->dimension, cell, Data::ON);

			cell = static_cast<Data::BoxTreeCell *>(cell->parent());
		}
	}

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	this->timeFindOnCellsInit += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	//std::cout << "PMJMesh::WorkerCellClassifier::findOnCells, time for setting ON in newFront cells parents = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	//this->timeInTest = this->timeOutOfBoxTest = this->timeOldPosOn =
	//	this->timeNotOnCommonAndNotNewTest = this->timeNotOnCommonAndNotNew =
	//	this->timeCommonNewOld = this->timeOnCommonOrNewTest =
	//	this->timeOnCommonOrNew = this->timeOnOldTest = this->timeOnOld =
	//	this->timeElse = 0.0;

	for (UInt i = 0; i < root->numChildren(); i++)
	{
		this->findOnCells(static_cast<Data::BoxTreeCell *>(root->child(i)), true,
			this->commonFront, this->newFront, this->oldFront);
	}

	//std::cout << "PMJMesh::WorkerCellClassifier::findOnCells, time for recursive findOnCells = " << Data::time() - debugstart << std::endl;

	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for geometric in tests = " << this->timeFindOnCellsInTest << std::endl;
	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for geometric out tests = " << this->timeFindOnCellsOutOfBoxTest << std::endl;
	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for checking if old position is ON = " << this->timeFindOnCellsOldPosOn << std::endl;
	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for checking if leaf does not crosses common and does not crosses new = " << this->timeFindOnCellsNotOnCommonAndNotNewTest << std::endl;
	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for working when previous check is true = " << this->timeFindOnCellsNotOnCommonAndNotNew << std::endl;
	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for narrowing lists (not leaf) = " << this->timeFindOnCellsCommonNewOld << std::endl;
	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for checking if cell crosses common or crosses new = " << this->timeFindOnCellsOnCommonOrNewTest << std::endl;
	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for working when previous check is true = " << this->timeFindOnCellsOnCommonOrNew << std::endl;
	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for checking if cell crosses old = " << this->timeFindOnCellsOnOldTest << std::endl;
	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for working when previous check is true = " << this->timeFindOnCellsOnOld << std::endl;
	//std::cout << "recursive PMJMesh::WorkerCellClassifier::findOnCells, time for working when neither of the two previous checks is true = " << this->timeFindOnCellsElse << std::endl;
}

bool PMJMesh::WorkerCellClassifier::outOfBox(const Data::BoxTreeCell *c) const
{
	return c->outOf(this->box);

	/*//if (this->box->out(c->box()))
	if (this->box->optOut(c->box()))
	{
		return true;
	}

	//if (!this->box->intercept(c->box()))
	if (!this->box->optIntercept(c->box()))
	{
		return true;
	}

	for (UInt i = 0; i < this->dimension; i++)
	{
		if (((c->box()->getMin()->getCoord(i) <  this->box->getMin()->getCoord(i)) &&
			 (c->box()->getMax()->getCoord(i) <= this->box->getMin()->getCoord(i))) ||
			((c->box()->getMax()->getCoord(i) >  this->box->getMax()->getCoord(i)) &&
			 (c->box()->getMin()->getCoord(i) >= this->box->getMax()->getCoord(i))))
		{
			return true;
		}
	}

	return false;*/
}

void PMJMesh::WorkerCellClassifier::findOnCells(Data::BoxTreeCell *c, bool geometricTests,
	Data::FrontElementList &commonFront, Data::FrontElementList &newFront,
	Data::FrontElementList &oldFront, bool testOnlyOldFront)
{
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

	/*if (!c->box())
	{
		std::cout << "ERROR in PMJMesh::WorkerCellClassifier::findOnCells: " << c->box() << ", cell->id = " << c->getId() << std::endl;

		return;
	}*/

	//if ((geometricTests) && (!this->box->in(c->box())))
	if ((geometricTests) && (!this->box->optIn(c->box())))
	{
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
		this->timeFindOnCellsInTest += Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

		if (this->outOfBox(c))
		{
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
			this->timeFindOnCellsOutOfBoxTest += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

			return;
		}

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
		this->timeFindOnCellsOutOfBoxTest += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

		for (UInt i = 0; i < c->numChildren(); i++)
		{
			this->findOnCells(static_cast<Data::BoxTreeCell *>(c->child(i)), true,
				commonFront, newFront, oldFront, testOnlyOldFront);
		}

		return;
	}

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	this->timeFindOnCellsInTest += Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

	Data::FrontElementList interCommon, interNew, interOld;

	enum Data::Position oldpos = PMJMesh::getPosition(this->dimension, c);

	bool on = (oldpos == Data::ON);

	if (on)
	{
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
		this->timeFindOnCellsOldPosOn += Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

		if (c->isLeaf())
		{
			//if ((!this->on(c, commonFront)) && (!this->on(c, newFront)))
			if ((!this->on(c, &commonFront)) && (!this->on(c, &newFront)))
			{
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
				this->timeFindOnCellsNotOnCommonAndNotNewTest += Data::time() - debugstart;
				debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

				this->outCells.push_back(c);

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
				this->timeFindOnCellsNotOnCommonAndNotNew += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
			}
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
			else
			{
				this->timeFindOnCellsNotOnCommonAndNotNewTest += Data::time() - debugstart;
			}
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
		}
		else
		{
			this->onRemoving(c, commonFront, interCommon, false);
			this->onRemoving(c, newFront, interNew, false);

			if ((interCommon.empty()) && (interNew.empty()))
			{
				c->getLeaves(this->outCells);

				on = false;
			}
			else
			{
				this->onRemoving(c, oldFront, interOld, false);
			}

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
			this->timeFindOnCellsCommonNewOld += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
		}
	}
	else
	{
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
		this->timeFindOnCellsOldPosOn += Data::time() - debugstart;
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

		{
			bool inter = false;

			if ((!testOnlyOldFront) &&
				(c->isLeaf() ?
					//((inter = this->on(c, commonFront, interCommon)) || (this->on(c, newFront, interNew))) :
					((inter = this->on(c, &commonFront, interCommon)) || (this->on(c, &newFront, interNew))) :
					((inter = this->onRemoving(c, commonFront, interCommon)) || (this->onRemoving(c, newFront, interNew)))))
			{
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
				this->timeFindOnCellsOnCommonOrNewTest += Data::time() - debugstart;
				debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

				on = true;

				if (c->isLeaf())
				{
					PMJMesh::setPosition(this->dimension, c, Data::ON);

					//std::cout << "(on, recursive) cell " << c->getId() << " was classified as ON" << std::endl;

					Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(c->parent());

					while ((cell) && (PMJMesh::getPosition(this->dimension, cell) != Data::ON))
					{
						//std::cout << "(on, recursive, parent) cell " << cell->getId() << " was classified as ON" << std::endl;

						PMJMesh::setPosition(this->dimension, cell, Data::ON);

						cell = static_cast<Data::BoxTreeCell *>(cell->parent());
					}
				}
				else
				{
					this->onRemoving(c, oldFront, interOld, false);

					if (inter)
					{
						this->onRemoving(c, newFront, interNew, false);
					}
					else
					{
						interCommon.clear();
					}
				}

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
				this->timeFindOnCellsOnCommonOrNew += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
			}
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
			else if ((this->timeFindOnCellsOnCommonOrNewTest += Data::time() - debugstart, debugstart = Data::time(), this->onRemoving(c, oldFront, interOld)))
#else
			else if (this->onRemoving(c, oldFront, interOld))
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
			{
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
				this->timeFindOnCellsOnOldTest += Data::time() - debugstart;
				debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

				//on = true;

				testOnlyOldFront = true;

				if (c->isLeaf())
				{
					this->outCells.push_back(c);
				}
				else
				{
					c->getLeaves(this->outCells);
				}

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
				this->timeFindOnCellsOnOld += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
			}
			else
			{
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
				this->timeFindOnCellsOnOldTest += Data::time() - debugstart;
				debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

				if (oldpos != Data::OUT)
				{
					//PMJMesh::setPosition(this->dimension, c, Data::UNKNOWN);

					//this->inCells.push_back(const_cast<Data::BoxTreeCell *>(c));
					//this->inCells.splice(this->inCells.end(), c->getLeaves());
					//c->getLeaves(this->inCells);

					Data::GraphNodeList newInCells;

					c->getLeaves(newInCells);

					for (Data::GraphNodeList::iterator iter = newInCells.begin();
						 iter != newInCells.end(); iter++)
					{
						PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>((*iter)), Data::UNKNOWN);
					}

					this->inCells.splice(this->inCells.end(), newInCells);
				}

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
				this->timeFindOnCellsElse += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
			}
		}
	}

	if ((!on) || (c->isLeaf()))
	{
		return;
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->findOnCells(static_cast<Data::BoxTreeCell *>(c->child(i)), false,
			interCommon, interNew, interOld, testOnlyOldFront);
	}
}

void PMJMesh::WorkerCellClassifier::setChildrenPosition(const Data::BoxTreeCell *cell,
	enum Data::Position pos)
{
	if (PMJMesh::getPosition(this->dimension, cell) == pos)
	{
		return;
	}

	//std::cout << "(children) cell " << cell->getId() << " was classified as " <<
	//	((pos == Data::OUT) ? "OUT" : ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : "UNKNOWN"))) << std::endl;

	Performer::AngleTreeCellClassifier::setChildrenPosition(cell, pos);
}

void PMJMesh::WorkerCellClassifier::findInCells()
{
	//Real timePop, timeGetPos, timeClassify, timeSetPos, timeSetChildPos, timeSetNeighPos;
	//timePop = timeGetPos = timeClassify = timeSetPos = timeSetChildPos = timeSetNeighPos = 0.0;
	//UInt debugNumClassifies = 0, debugNumNotClassifies = 0, debugNumLeaves = 0, debugNumNotLeaves = 0;
	//UInt debugLeavesClassifies = 0, debugLeavesNotClassifies = 0, debugNotLeavesClassifies = 0, debugNotLeavesNotClassifies = 0;

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

	while (!this->inCells.empty())
	{
		//Real debugstart = Data::time();

		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(this->inCells.front());

		this->inCells.pop_front();

		//timePop += Data::time() - debugstart;

		/*if (c->isLeaf())
		{
			debugNumLeaves++;
		}
		else
		{
			debugNumNotLeaves++;
		}*/

		//debugstart = Data::time();

		//if (PMJMesh::getPosition(this->dimension, c) == Data::OUT)
		if (PMJMesh::getPosition(this->dimension, c) != Data::UNKNOWN)
		{
			//timeGetPos += Data::time() - debugstart;

			/*debugNumNotClassifies++;

			if (c->isLeaf())
			{
				debugLeavesNotClassifies++;
			}
			else
			{
				debugNotLeavesNotClassifies++;
			}*/

			continue;
		}

		//timeGetPos += Data::time() - debugstart;

		/*debugNumClassifies++;

		if (c->isLeaf())
		{
			debugLeavesClassifies++;
		}
		else
		{
			debugNotLeavesClassifies++;
		}*/

		//debugstart = Data::time();

		//enum Data::Position pos = this->classify(this->mid(c), this->elements, false);
		enum Data::Position pos = this->classify(this->mid(c), &this->elements, false);

		pos = (pos == Data::OUT) ? Data::IN : Data::OUT;

		//timeClassify += Data::time() - debugstart;
		//debugstart = Data::time();

		//std::cout << "cell " << c->getId() << " was classified as " <<
		//	((pos == Data::OUT) ? "OUT" : ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : "UNKNOWN"))) << std::endl;

		//this->setChildrenPosition(c, pos);

		//timeSetChildPos += Data::time() - debugstart;
		//debugstart = Data::time();

		PMJMesh::setPosition(this->dimension, c, pos);

		//timeSetPos += Data::time() - debugstart;
		//debugstart = Data::time();

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
		this->timeFindInCellsCells += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

		this->setNeighborsPosition(c, pos);

		//timeSetNeighPos += Data::time() - debugstart;

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
		debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	}

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	this->timeFindInCellsCells += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

	/*std::cout << "PMJMesh::WorkerCellClassifier::findInCells, time for popping = " << timePop << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, time for getting position = " << timeGetPos << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, time for classifying = " << timeClassify << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, time for setting position = " << timeSetPos << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, time for setting children position = " << timeSetChildPos << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, time for setting neighbors position = " << timeSetNeighPos << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, number of cells = " << debugNumClassifies + debugNumNotClassifies << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, number of classifies = " << debugNumClassifies << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, number of not-classifies = " << debugNumNotClassifies << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, number of leaves = " << debugNumLeaves << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, number of internal nodes = " << debugNumNotLeaves << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, number of leaves classifies = " << debugLeavesClassifies << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, number of leaves not classifies = " << debugLeavesNotClassifies << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, number of internal nodes classifies = " << debugNotLeavesClassifies << std::endl;
	std::cout << "PMJMesh::WorkerCellClassifier::findInCells, number of internal nodes not classifies = " << debugNotLeavesNotClassifies << std::endl;*/
}

void PMJMesh::WorkerCellClassifier::findOutCells()
{
	while (!this->outCells.empty())
	{
		Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->outCells.back());

		this->outCells.pop_back();

		if (PMJMesh::getPosition(this->dimension, cell) == Data::OUT)
		{
			continue;
		}

		PMJMesh::setPosition(this->dimension, cell, Data::OUT);

		//std::cout << "(out) cell " << cell->getId() << " was classified as OUT" << std::endl;

		Data::BoxTreeCell *parent = static_cast<Data::BoxTreeCell *>(cell->parent());

		while (parent)
		{
			enum Data::Position ppos = PMJMesh::getPosition(this->dimension, parent);

			if (/*(ppos == Data::ON) || */(ppos == Data::OUT))
			{
				break;
			}

			bool allequal = true;

			for (UInt i = 0; i < parent->numChildren(); i++)
			{
				Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(parent->child(i));

				if (child == cell)
				{
					continue;
				}

				enum Data::Position cpos = PMJMesh::getPosition(this->dimension, child);

				if (cpos != Data::OUT)
				{
					allequal = false;

					break;
				}
			}

			if (!allequal)
			{
				break;
			}

			//std::cout << "(out, parent) cell " << parent->getId() << " was classified as OUT" << std::endl;

			PMJMesh::setPosition(this->dimension, parent, Data::OUT);

			cell = parent;
			parent = static_cast<Data::BoxTreeCell *>(cell->parent());
		}

		//std::cout << "classifying cell " << this->outCells.back() << " as OUT" << std::endl;
	}
}

void PMJMesh::WorkerCellClassifier::neighbors(const Data::BoxTreeCell *c, Data::GraphNodeList &neighs) const
{
	UInt numSides = c->numSides();

	for (UInt i = 0; i < numSides; i++)
	{
		if (c->in(this->box, i))
		{
			this->tree->neighbors(c, i, neighs);
		}
	}
}

Data::GraphNodeList PMJMesh::WorkerCellClassifier::neighbors(const Data::BoxTreeCell *c) const
{
	Data::GraphNodeList neighs;

	this->neighbors(c, neighs);

	return neighs;
}

void PMJMesh::WorkerCellClassifier::setNeighborsPosition(Data::BoxTreeCell *c, enum Data::Position pos)
{
	/*Data::BoxTreeCell *cell = c;
	Data::BoxTreeCell *parent = static_cast<Data::BoxTreeCell *>(cell->parent());

	while (parent)
	{
		enum Data::Position ppos = PMJMesh::getPosition(this->dimension, parent);

		if ((ppos == Data::ON) || (ppos == pos))
		{
			break;
		}

		bool allequal = true;

		for (UInt i = 0; i < parent->numChildren(); i++)
		{
			Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(parent->child(i));

			if (child == cell)
			{
				continue;
			}

			enum Data::Position cpos = PMJMesh::getPosition(this->dimension, child);

			if (cpos != pos)
			{
				allequal = false;

				break;
			}
		}

		if (!allequal)
		{
			break;
		}

		PMJMesh::setPosition(this->dimension, parent, pos);

		//std::cout << "(neighbor, parent of original) cell " << parent->getId() << " was classified as " <<
		//	((pos == Data::OUT) ? "OUT" : ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : "UNKNOWN"))) << std::endl;

		cell = parent;
		parent = static_cast<Data::BoxTreeCell *>(cell->parent());
	}*/

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

#if USE_C__11
	Data::GraphNodeHash parents;
#else
	Data::GraphNodeSet parents;
#endif //#if USE_C__11

	parents.insert(c->parent());

	//Data::GraphNodeList neighs = this->neighbors(c);
	Data::GraphNodeList neighs;
	this->neighbors(c, neighs);

	while (!neighs.empty())
	{
		Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.back());

		neighs.pop_back();

		enum Data::Position npos = PMJMesh::getPosition(this->dimension, neigh);

		if ((npos == pos) || (npos == Data::ON))
		{
			continue;
		}

		PMJMesh::setPosition(this->dimension, neigh, pos);

		//std::cout << "(neighbor) cell " << neigh->getId() << " was classified as " <<
		//	((pos == Data::OUT) ? "OUT" : ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : "UNKNOWN"))) << std::endl;

		/*cell = neigh;
		parent = static_cast<Data::BoxTreeCell *>(cell->parent());

		while (parent)
		{
			enum Data::Position ppos = PMJMesh::getPosition(this->dimension, parent);

			if ((ppos == Data::ON) || (ppos == pos))
			{
				break;
			}

			bool allequal = true;

			for (UInt i = 0; i < parent->numChildren(); i++)
			{
				Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(parent->child(i));

				if (child == cell)
				{
					continue;
				}

				enum Data::Position cpos = PMJMesh::getPosition(this->dimension, child);

				if (cpos != pos)
				{
					allequal = false;

					break;
				}
			}

			if (!allequal)
			{
				break;
			}

			PMJMesh::setPosition(this->dimension, parent, pos);

			//std::cout << "(neighbor, parent of neighbor) cell " << parent->getId() << " was classified as " <<
			//	((pos == Data::OUT) ? "OUT" : ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : "UNKNOWN"))) << std::endl;

			cell = parent;
			parent = static_cast<Data::BoxTreeCell *>(cell->parent());
		}*/

		parents.insert(neigh->parent());

		this->neighbors(neigh, neighs);
	}

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	this->timeFindInCellsNeighbors += Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

#if USE_C__11
	for (Data::GraphNodeHash::iterator iter = parents.begin();
#else
	for (Data::GraphNodeSet::iterator iter = parents.begin();
#endif //#if USE_C__11
		 iter != parents.end(); iter++)
	{
		Data::BoxTreeCell *parent = static_cast<Data::BoxTreeCell *>((*iter));

		while (parent)
		{
			enum Data::Position ppos = PMJMesh::getPosition(this->dimension, parent);

			if ((ppos == Data::ON) || (ppos == pos))
			{
				break;
			}

			bool allequal = true;

			for (UInt i = 0; i < parent->numChildren(); i++)
			{
				Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(parent->child(i));

				enum Data::Position cpos = PMJMesh::getPosition(this->dimension, child);

				if (cpos != pos)
				{
					allequal = false;

					break;
				}
			}

			if (!allequal)
			{
				break;
			}

			PMJMesh::setPosition(this->dimension, parent, pos);

			//std::cout << "(neighbor, parent of original) cell " << parent->getId() << " was classified as " <<
			//	((pos == Data::OUT) ? "OUT" : ((pos == Data::IN) ? "IN" : ((pos == Data::ON) ? "ON" : "UNKNOWN"))) << std::endl;

			parent = static_cast<Data::BoxTreeCell *>(parent->parent());
		}
	}

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	this->timeFindInCellsParents += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
}

void PMJMesh::WorkerCellClassifier::classify()
{
	//Real debugstart = Data::time();

	this->findOnCells();

	//std::cout << "PMJMesh::WorkerCellClassifier::classify, time to find on cells = " << Data::time() - debugstart << std::endl;
	//Real debugstart = Data::time();

	this->findInCells();

	//this->timeFindInCells += Data::time() - debugstart;
	//std::cout << "PMJMesh::WorkerCellClassifier::classify, time to find in cells = " << Data::time() - debugstart << std::endl;
#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER

	this->findOutCells();

#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	this->timeFindOutCells += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_WORKER_CELL_CLASSIFIER
	//std::cout << "PMJMesh::WorkerCellClassifier::classify, time to find out cells = " << Data::time() - debugstart << std::endl;
}

void PMJMesh::WorkerCellClassifier::adjustPosition()
{
	PMJMesh::AngleTreeCellClassifier::adjustPosition();

	//Data::ULIntVector removed;

	for (Data::PositionMap::iterator iter = this->position.begin();
		 iter != this->position.end();)
	{
		if ((*iter).second == Data::IN)
		{
			//removed.push_back((*iter).first);
			//Data::PositionMap::iterator iter2 = iter;
			//iter2++;

			//this->position.erase(iter);
			this->position.erase(iter++);

			//iter = iter2;
		}
		else
		{
			iter++;
		}
	}

	/*while (!removed.empty())
	{
		this->position.erase(removed.back());

		removed.pop_back();
	}*/
}

void PMJMesh::WorkerCellClassifier::execute()
{
	if (this->oldFront.empty())
	{
		return;
	}

	//Real debugstart = Data::time();

	this->elements = this->newFront;
	this->elements.insert(this->elements.end(), this->oldFront.begin(), this->oldFront.end());

	//std::cout << "PMJMesh::WorkerCellClassifier::execute, time to make list of front elements = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	this->classify();

	//std::cout << "PMJMesh::WorkerCellClassifier::execute, time to classify = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

	this->adjustPosition();

	//std::cout << "PMJMesh::WorkerCellClassifier::execute, time to adjust position = " << Data::time() - debugstart << std::endl;
}

#endif //#if (!USE_WORKER_CELL_CLASSIFIER_2)
