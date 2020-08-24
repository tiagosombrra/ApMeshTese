#include "PMJMesh/WorkerCellClassifier2.h"

#if (USE_NEW_FRONT_UPDATER)

#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/QuadTreeCell.h"
#include "PMJMesh/OctTreeCell.h"

PMJMesh::WorkerCellClassifier2::WorkerCellClassifier2(UInt dimension, const Data::BoxTree *tree,
	const Data::Box *box, const PMJMesh::Front *oldFront,
	const PMJMesh::Front *newFront, const PMJMesh::Front *commonFront,
	bool newFrontInverted) :
		PMJMesh::AngleTreeCellClassifier(dimension, static_cast<Parallel::ThreadManager *>(NULL), NULL, tree)
{
	this->setBox(box);

	this->oldFront = oldFront;
	this->newFront = newFront;
	this->commonFront = commonFront;

	this->newFrontInverted = newFrontInverted;
}

PMJMesh::WorkerCellClassifier2::~WorkerCellClassifier2()
{

}

void PMJMesh::WorkerCellClassifier2::setBox(const Data::Box *box)
{
	this->box = box;
}

/*void debug(Data::UInt dimension, const Data::BoxTreeCell *c)
{
	if (PMJMesh::getPosition(dimension, c) == Data::UNKNOWN)
	{
		std::cout << "debug, cell " << c->getId() << " has UNKNOWN position" << std::endl;
	}

	for (Data::UInt i = 0; i < c->numChildren(); i++)
	{
		debug(dimension, static_cast<Data::BoxTreeCell *>(c->child(i)));
	}
}*/

void PMJMesh::WorkerCellClassifier2::execute()
{
	if (!this->oldFront)
	{
		return;
	}

	//std::cout << "before" << std::endl;
	//debug(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));

	//std::cout << "WorkerCellClassifier2::execute" << std::endl;

	this->classifyBasedOnOldONCells();

	//std::cout << "performing outcells, size = " << this->outCells.size() << std::endl;

	while (!this->outCells.empty())
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(this->outCells.front());

		this->outCells.pop_front();

		//std::cout << "c->id = " << c->getId() << std::endl;

		if (PMJMesh::getPosition(this->dimension, c) == Data::OUT)
		{
			//std::cout << "cell already out" << std::endl;

			/*//debug
			if ((c->getId() == 807239) || (c->isDescendant(807239)))
			{
				std::cout << "cell " << c->getId() << " is already OUT" << std::endl;
			}
			//debug*/

			continue;
		}

		/*//debug
		if ((c->getId() == 807239) || (c->isDescendant(807239)))
		{
			std::cout << "cell " << c->getId() << ", setting position as OUT" << std::endl;
		}
		//debug*/

		PMJMesh::setPosition(this->dimension, c, Data::OUT);

#if USE_IN_CELLS_AS_LIST
		this->setNeighborsPosition(c, Data::OUT);
#else
		this->setNeighborsPosition(c, Data::OUT, true);
#endif //#if USE_IN_CELLS_AS_LIST

		//std::cout << "performing outcells, size = " << this->outCells.size() << std::endl;
	}

	//this->classifyBasedOnNormal();
	this->classifyBasedOnNormalFromInCells();

	this->classifyBasedOnPoint();

	//std::cout << "upmostcells.size = " << this->upmostCells.size() << std::endl;

	for (Data::GraphNodeList::iterator iter = this->upmostCells.begin();
		 iter != this->upmostCells.end(); iter++)
	{
		this->adjustPositionFromChildren(static_cast<Data::BoxTreeCell *>((*iter)));
	}

	for (Data::GraphNodeList::iterator iter = this->upmostCells.begin();
		 iter != this->upmostCells.end(); iter++)
	{
		if (this->dimension == 2)
		{
			static_cast<PMJMesh::QuadTreeCell *>((*iter))->adjustPositionFromChildren(false);
		}
		else
		{
			static_cast<PMJMesh::OctTreeCell *>((*iter))->adjustPositionFromChildren(false);
		}
	}

	/*while (!this->upmostCells.empty())
	{
		Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->upmostCells.front());

		this->upmostCells.pop_front();

		this->adjustPositionFromChildren(cell);
	}*/

	//std::cout << "after" << std::endl;
	//debug(this->dimension, static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));
}

bool PMJMesh::WorkerCellClassifier2::outOfBox(const Data::BoxTreeCell *c) const
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

void PMJMesh::WorkerCellClassifier2::neighbors(const Data::BoxTreeCell *c, UInt side, Data::GraphNodeList &neighs) const
{
	if (c->in(this->box, side))
	{
		this->tree->neighbors(c, side, neighs);
	}
}

void PMJMesh::WorkerCellClassifier2::neighbors(const Data::BoxTreeCell *c, Data::GraphNodeList &neighs) const
{
	UInt numSides = c->numSides();

	for (UInt i = 0; i < numSides; i++)
	{
		this->neighbors(c, i, neighs);
	}
}

#if USE_IN_CELLS_AS_LIST
void PMJMesh::WorkerCellClassifier2::setNeighborsPosition(Data::BoxTreeCell *c, enum Data::Position pos)
#else
void PMJMesh::WorkerCellClassifier2::setNeighborsPosition(Data::BoxTreeCell *c, enum Data::Position pos, bool removingFromIn)
#endif //#if USE_IN_CELLS_AS_LIST
{
	//std::cout << "WorkerCellClassifier2::setNeighborsPosition" << std::endl;
	//std::cout << "c->id = " << c->getId() << ", pos = " << pos << ", removingFromIn = " << std::boolalpha << removingFromIn << std::endl;

	/*//debug
	if ((c->getId() == 807239) || (c->isDescendant(807239)))
	{
		std::cout << "WorkerCellClassifier2::setNeighborsPosition" << std::endl;
		std::cout << "cell " << c->getId() << std::endl;
	}
	//debug*/

	Data::GraphNodeList neighs;

	this->neighbors(c, neighs);

	//std::cout << "neighs.size = " << neighs.size() << std::endl;

	while (!neighs.empty())
	{
		Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.back());

		neighs.pop_back();

		//std::cout << "neigh = " << neigh->getId() << std::endl;

		/*//debug
		if ((c->getId() == 807239) || (c->isDescendant(807239)))
		{
			std::cout << "WorkerCellClassifier2::setNeighborsPosition" << std::endl;
			std::cout << "cell " << c->getId() << ", neigh " << neigh->getId() << std::endl;
		}
		//debug*/

		enum Data::Position npos = PMJMesh::getPosition(this->dimension, neigh);

		//std::cout << "neigh->pos = " << npos << std::endl;

		/*//debug
		if ((c->getId() == 807239) || (c->isDescendant(807239)))
		{
			std::cout << "WorkerCellClassifier2::setNeighborsPosition" << std::endl;
			std::cout << "cell " << c->getId() << ", neighpos " << npos << std::endl;
		}
		//debug*/

		if ((npos == pos) || (npos == Data::ON))
		{
			//std::cout << "pos == pos || npos == on" << std::endl;

			/*//debug
			if ((c->getId() == 807239) || (c->isDescendant(807239)))
			{
				std::cout << "WorkerCellClassifier2::setNeighborsPosition" << std::endl;
				std::cout << "cell " << c->getId() << " equal or ON pos: " << npos << std::endl;
			}
			//debug*/

			continue;
		}

#if (!USE_IN_CELLS_AS_LIST)
		if (removingFromIn)
		{
			//std::cout << "incells.size = " << this->inCells.size() << std::endl;

			this->inCells.erase(neigh);

			//std::cout << "incells.size = " << this->inCells.size() << std::endl;

			/*//debug
			if ((c->getId() == 807239) || (c->isDescendant(807239)))
			{
				std::cout << "WorkerCellClassifier2::setNeighborsPosition" << std::endl;
				std::cout << "cell " << c->getId() << ", removing neigh " << neigh->getId() << " from inCells" << std::endl;
			}
			//debug*/
		}
#endif //#if (!USE_IN_CELLS_AS_LIST)

		PMJMesh::setPosition(this->dimension, neigh, pos);

		/*//debug
		if ((c->getId() == 807239) || (c->isDescendant(807239)))
		{
			std::cout << "WorkerCellClassifier2::setNeighborsPosition" << std::endl;
			std::cout << "cell " << c->getId() << ", setting neighpos as " << pos << std::endl;
		}
		//debug*/

		//std::cout << "position for cell " << neigh->getId() << ": " << pos << std::endl;

		this->neighbors(neigh, neighs);

		//std::cout << "neighs.size = " << neighs.size() << std::endl;
	}
}

void PMJMesh::WorkerCellClassifier2::classifyBasedOnOldONCells()
{
	//std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCells" << std::endl;

	Data::FrontElementList commonFront = this->commonFront->getAllElements();
	Data::FrontElementList newFront = this->newFront->getAllElements();

	//std::cout << "common.size = " << commonFront.size() << ", new.size = " << newFront.size() << std::endl;

	this->classifyBasedOnOldONCellsWithGeometricTests(
		static_cast<Data::BoxTreeCell *>(this->tree->getRoot()),
		commonFront, newFront);
}

void PMJMesh::WorkerCellClassifier2::classifyBasedOnOldONCellsWithGeometricTests(
	Data::BoxTreeCell *c, Data::FrontElementList &commonFront,
	Data::FrontElementList &newFront)
{
	//std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCellsWithGeometricTests" << std::endl;
	//std::cout << "c->id = " << c->getId() << ", level = " << c->level() << ", common.size = " << commonFront.size() << ", new.size = "  << newFront.size() << std::endl;

	/*//debug
	if ((c->getId() == 807239) || (c->isDescendant(807239)))
	{
		std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCellsWithGeometricTests" << std::endl;
		std::cout << "cell " << c->getId() << std::endl;
	}
	//debug*/

	if (!this->box->optIn(c->box()))
	{
		//std::cout << "not in" << std::endl;

		/*//debug
		if ((c->getId() == 807239) || (c->isDescendant(807239)))
		{
			std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCellsWithGeometricTests" << std::endl;
			std::cout << "cell " << c->getId() << " not in subdomain box" << std::endl;
		}
		//debug*/

		if (this->outOfBox(c))
		{
			//std::cout << "out" << std::endl;

			/*//debug
			if ((c->getId() == 807239) || (c->isDescendant(807239)))
			{
				std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCellsWithGeometricTests" << std::endl;
				std::cout << "cell " << c->getId() << " out of subdomain box" << std::endl;
			}
			//debug*/

			return;
		}

		//std::cout << "not out" << std::endl;

		for (UInt i = 0; i < c->numChildren(); i++)
		{
			this->classifyBasedOnOldONCellsWithGeometricTests(
				static_cast<Data::BoxTreeCell *>(c->child(i)),
				commonFront, newFront);
		}

		return;
	}

	//std::cout << "in, adding to upmostCells" << std::endl;

	this->upmostCells.push_back(c);

	this->classifyBasedOnOldONCells(c, commonFront, newFront);
}

void PMJMesh::WorkerCellClassifier2::classifyBasedOnOldONCells(
	Data::BoxTreeCell *c, Data::FrontElementList &commonFront,
	Data::FrontElementList &newFront)
{
	//std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCells rec" << std::endl;
	//std::cout << "c->id = " << c->getId() << ", level = " << c->level() << ", common.size = " << commonFront.size() << ", new.size = "  << newFront.size() << std::endl;

	/*//debug
	if ((c->getId() == 807239) || (c->isDescendant(807239)))
	{
		std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCells" << std::endl;
		std::cout << "cell " << c->getId() << std::endl;
	}
	//debug*/

	Data::FrontElementList interCommon, interNew;

	enum Data::Position oldpos = PMJMesh::getPosition(this->dimension, c);

	//std::cout << "oldpos = " << oldpos << std::endl;

	if (oldpos == Data::OUT)
	{
		//std::cout << "out cell" << std::endl;

		/*//debug
		if ((c->getId() == 807239) || (c->isDescendant(807239)))
		{
			std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCells" << std::endl;
			std::cout << "cell " << c->getId() << " was OUT" << std::endl;
		}
		//debug*/

		Data::GraphNodeList leaves;

		c->getLeaves(leaves);

		//std::cout << "leaves.size = " << leaves.size() << std::endl;

		for (Data::GraphNodeList::iterator iter = leaves.begin();
			 iter != leaves.end(); iter++)
		{
			PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>((*iter)), Data::UNKNOWN);

			//std::cout << "position for cell " << (*iter)->getId() << ": " << Data::UNKNOWN << std::endl;

			/*//debug
			if ((c->getId() == 807239) || (c->isDescendant(807239)))
			{
				std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCells" << std::endl;
				std::cout << "cell " << c->getId() << " setting UNKNOWN pos for " << (*iter)->getId() << std::endl;
				std::cout << "cell " << c->getId() << ", adding " << (*iter)->getId() << " to outCells" << std::endl;
			}
			//debug*/
		}

		this->outCells.splice(this->outCells.end(), leaves);

		//std::cout << "outcells.size = " << this->outCells.size() << std::endl;

		return;
	}

	//std::cout << "not out cell" << std::endl;

	bool leaf = c->isLeaf();

	//std::cout << "leaf = " << std::boolalpha << leaf << std::endl;

	bool oncommon = leaf ? this->on(c, &commonFront, interCommon) : this->onRemoving(c, commonFront, interCommon);
	//bool onnew = ((leaf) && (oncommon)) ? true :
	//	(leaf ? this->on(c, &newFront, interNew) : this->onRemoving(c, newFront, interNew, (!oncommon)));
	bool onnew = leaf ? this->on(c, &newFront, interNew) : this->onRemoving(c, newFront, interNew, (!oncommon));

	//std::cout << "oncommon = " << std::boolalpha << oncommon << ", onnew = " << std::boolalpha << onnew << std::endl;
	//std::cout << "intercommon.size = " << interCommon.size() << ", internew.size = " << interNew.size() << std::endl;

	if ((!oncommon) && (!onnew))
	{
		//std::cout << "not oncommon and not onnew" << std::endl;

		/*//debug
		if ((c->getId() == 807239) || (c->isDescendant(807239)))
		{
			std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCells" << std::endl;
			std::cout << "cell " << c->getId() << " not on common and not on new" << std::endl;
		}
		//debug*/

		Data::Position pos = (oldpos == Data::ON) ? Data::OUT : Data::UNKNOWN;

		//std::cout << "pos = " << pos << std::endl;

		if (leaf)
		{
			//PMJMesh::setPosition(this->dimension, c, pos);
			PMJMesh::setPosition(this->dimension, c, Data::UNKNOWN);

			//std::cout << "position for cell " << c->getId() << ": " << pos << std::endl;
			//std::cout << "position for cell " << c->getId() << ": " << Data::UNKNOWN << std::endl;

			/*//debug
			if ((c->getId() == 807239) || (c->isDescendant(807239)))
			{
				std::cout << "WorkerCellClassifier2::classifyBasedOnOldONCells" << std::endl;
				std::cout << "cell " << c->getId() << " setting UNKNOWN" << std::endl;
			}
			//debug*/

			if (pos == Data::OUT)
			{
				this->outCells.push_back(c);

				//std::cout << "outcells.size = " << this->outCells.size() << std::endl;

				/*//debug
				if ((c->getId() == 807239) || (c->isDescendant(807239)))
				{
					std::cout << "cell " << c->getId() << " adding to outCells" << std::endl;
				}
				//debug*/
			}
			else
			{
#if USE_IN_CELLS_AS_LIST
				this->inCells.push_back(c);
#else
				this->inCells.insert(c);
#endif //#if USE_IN_CELLS_AS_LIST

				//std::cout << "incells.size = " << this->inCells.size() << std::endl;

				/*//debug
				if ((c->getId() == 807239) || (c->isDescendant(807239)))
				{
					std::cout << "cell " << c->getId() << " adding to inCells" << std::endl;
				}
				//debug*/
			}
		}
		else
		{
			Data::GraphNodeList nodes, leaves;

			c->getInternalNodesAndLeaves(nodes, leaves);

			//std::cout << "nodes.size = " << nodes.size() << ", leaves.size = " << leaves.size() << std::endl;

			for (Data::GraphNodeList::iterator iter = nodes.begin();
				 iter != nodes.end(); iter++)
			{
				PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>((*iter)), pos);

				//std::cout << "position for cell " << (*iter)->getId() << ": " << pos << std::endl;

				/*//debug
				if ((c->getId() == 807239) || (c->isDescendant(807239)))
				{
					std::cout << "cell " << c->getId() << ", setting internal node " << (*iter)->getId() << " as " << pos << std::endl;
				}
				//debug*/
			}

			for (Data::GraphNodeList::iterator iter = leaves.begin();
				 iter != leaves.end(); iter++)
			{
				//PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>((*iter)), pos);
				PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>((*iter)), Data::UNKNOWN);

				//std::cout << "position for cell " << (*iter)->getId() << ": " << pos << std::endl;
				//std::cout << "position for cell " << (*iter)->getId() << ": " << Data::UNKNOWN << std::endl;

				/*//debug
				if ((c->getId() == 807239) || (c->isDescendant(807239)))
				{
					std::cout << "cell " << c->getId() << ", setting leaf " << (*iter)->getId() << " as UNKNOWN" << std::endl;
				}
				//debug*/
			}

			if (pos == Data::OUT)
			{
				this->outCells.splice(this->outCells.end(), leaves);

				//std::cout << "outcells.size = " << this->outCells.size() << std::endl;

				/*//debug
				if ((c->getId() == 807239) || (c->isDescendant(807239)))
				{
					std::cout << "cell " << c->getId() << ", adding leaves to outCells" << std::endl;
				}
				//debug*/
			}
			else
			{
#if USE_IN_CELLS_AS_LIST
				this->inCells.splice(this->inCells.end(), leaves);
#else
				this->inCells.insert(leaves.begin(), leaves.end());
#endif //#if USE_IN_CELLS_AS_LIST

				//std::cout << "incells.size = " << this->inCells.size() << std::endl;

				/*//debug
				if ((c->getId() == 807239) || (c->isDescendant(807239)))
				{
					std::cout << "cell " << c->getId() << ", adding leaves to inCells" << std::endl;
				}
				//debug*/
			}
		}

		return;
	}

	if (leaf)
	{
		Data::FrontElementList *fels = &(this->felsMap[c]);

		//fels->splice(fels->end(), interCommon);
		fels->splice(fels->end(), interNew);

		//std::cout << "felsmap.size = " << this->felsMap.size() << ", felsCrossingCell.size = " << fels->size() << std::endl;
	}

	PMJMesh::setPosition(this->dimension, c, Data::ON);

	//std::cout << "position for cell " << c->getId() << ": " << Data::ON << std::endl;

	/*//debug
	if ((c->getId() == 807239) || (c->isDescendant(807239)))
	{
		std::cout << "cell " << c->getId() << " is now ON" << std::endl;
	}
	//debug*/

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->classifyBasedOnOldONCells(static_cast<Data::BoxTreeCell *>(c->child(i)),
			interCommon, interNew);
	}
}

void PMJMesh::WorkerCellClassifier2::classifyBasedOnNormal()
{
	//std::cout << "WorkerCellClassifier2::classifyBasedOnNormal" << std::endl;

	//std::cout << "incells.size = " << this->inCells.size() << std::endl;

	if (this->inCells.empty())
	{
		return;
	}

	Int directions[this->dimension];

	for (UInt i = 0; i < this->dimension; i++)
	{
		directions[i] = 0;
	}

	//std::cout << "felsmap.size = " << this->felsMap.size() << std::endl;

#if USE_C__11
	for (CellFEListHashMap::iterator iter = this->felsMap.begin();
#else
	for (CellFEListMap::iterator iter = this->felsMap.begin();
#endif //#if USE_C__11
		 iter != this->felsMap.end(); iter++)
	{
		//std::cout << "testing on cell " << (*iter).first->getId() << ", fels.size = " << (*iter).second.size() << std::endl;

		/*if ((*iter).second.empty())
		{
			continue;
		}

		for (Data::FrontElementList::iterator iter2 = (*iter).second.begin();
			 iter2 != (*iter).second.end(); iter2++)
		{
			Data::FrontElement *fel = (*iter2);

			Data::Vector n = (this->dimension == 2) ?
				Data::Vector(static_cast<Data::Edge2D *>(fel->getShape())->normal()) :
				Data::Vector(static_cast<Data::Triangle3D *>(fel->getShape())->normal());

			//n.normalize();

			for (UInt i = 0; i < this->dimension; i++)
			{
				Real coord = n.getCoord(i);

				if (coord > Data::getTolerance())
				{
					directions[i]++;
				}
				else if (coord < -Data::getTolerance())
				{
					directions[i]--;
				}
			}
		}

		Data::BoxTreeCell *cell = NULL;

		Int size = static_cast<Int>((*iter).second.size());

		for (UInt i = 0; i < this->dimension; i++)
		{
			//std::cout << "directions[" << i << "] = " << directions[i] << std::endl;

			if ((directions[i] == size) ||
				(directions[i] == -size))
			{
				cell = (*iter).first;

				if (this->newFrontInverted)
				{
					directions[i] = -directions[i];
				}
			}
			else
			{
				directions[i] = 0;
			}

			//std::cout << "directions[" << i << "] = " << directions[i] << std::endl;
		}

		if (!cell)
		{
			//std::cout << "could not determine direction" << std::endl;

			continue;
		}*/

		if (!this->findClassificationDirections((*iter).second, directions))
		{
			continue;
		}

		Data::BoxTreeCell *cell = (*iter).first;

		for (UInt i = 0; i < this->dimension; i++)
		{
			if (directions[i] == 0)
			{
				continue;
			}

			if (this->newFrontInverted)
			{
				directions[i] = -directions[i];
			}

			UInt side = (directions[i] < 0) ? this->dimension + i : i;
			UInt opp = (directions[i] < 0) ? i : this->dimension + i;

			//std::cout << "side = " << side << ", opp = " << opp << std::endl;

			Data::GraphNodeList neighs;

			this->neighbors(cell, side, neighs);

			Data::Position pos = Data::IN;

			//std::cout << "side, neighs.size = " << neighs.size() << ", pos = " << pos << std::endl;

			while (!neighs.empty())
			{
				Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.front());

				neighs.pop_front();

				Data::Position oldpos = PMJMesh::getPosition(this->dimension, neigh);

				//std::cout << "neigh = " << neigh->getId() << ", oldpos = " << oldpos << std::endl;

				if ((oldpos == Data::ON) || (oldpos == pos))
				{
					//std::cout << "oldpos == ON or oldpos == pos" << std::endl;

					continue;
				}

				PMJMesh::setPosition(this->dimension, neigh, pos);

				//std::cout << "position for cell " << neigh->getId() << ": " << pos << std::endl;

#if (!USE_IN_CELLS_AS_LIST)
				//std::cout << "incells.size = " << this->inCells.size() << std::endl;

				this->inCells.erase(neigh);

				//std::cout << "incells.size = " << this->inCells.size() << std::endl;
#endif //#if (!USE_IN_CELLS_AS_LIST)

#if USE_IN_CELLS_AS_LIST
				this->setNeighborsPosition(neigh, pos);
#else
				this->setNeighborsPosition(neigh, pos, true);
#endif //#if USE_IN_CELLS_AS_LIST
			}

			this->neighbors(cell, opp, neighs);

			pos = Data::OUT;

			//std::cout << "opp, neighs.size = " << neighs.size() << ", pos = " << pos << std::endl;

			while (!neighs.empty())
			{
				Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.front());

				neighs.pop_front();

				Data::Position oldpos = PMJMesh::getPosition(this->dimension, neigh);

				//std::cout << "neigh = " << neigh->getId() << ", oldpos = " << oldpos << std::endl;

				if ((oldpos == Data::ON) || (oldpos == pos))
				{
					//std::cout << "oldpos == ON or oldpos == pos" << std::endl;

					continue;
				}

				PMJMesh::setPosition(this->dimension, neigh, pos);

				//std::cout << "position for cell " << neigh->getId() << ": " << pos << std::endl;

#if (!USE_IN_CELLS_AS_LIST)
				//std::cout << "incells.size = " << this->inCells.size() << std::endl;

				this->inCells.erase(neigh);

				//std::cout << "incells.size = " << this->inCells.size() << std::endl;
#endif //#if (!USE_IN_CELLS_AS_LIST)

#if USE_IN_CELLS_AS_LIST
				this->setNeighborsPosition(neigh, pos);
#else
				this->setNeighborsPosition(neigh, pos, true);
#endif //#if USE_IN_CELLS_AS_LIST
			}

#if (!USE_IN_CELLS_AS_LIST)
			if (this->inCells.empty())
			{
				//std::cout << "incells is now empty" << std::endl;

				return;
			}
#endif //#if (!USE_IN_CELLS_AS_LIST)

			directions[i] = 0;
		}
	}
}

void PMJMesh::WorkerCellClassifier2::classifyBasedOnNormalFromInCells()
{
	if (this->inCells.empty())
	{
		return;
	}

#if USE_IN_CELLS_AS_LIST
	for (Data::GraphNodeList::iterator iter = this->inCells.begin();
#else
#if USE_C__11
	for (Data::GraphNodeHash::iterator iter = this->inCells.begin();
#else
	for (Data::GraphNodeSet::iterator iter = this->inCells.begin();
#endif //#if USE_C__11
#endif //#if USE_IN_CELLS_AS_LIST
		 iter != this->inCells.end();)
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

		Data::Position pos = PMJMesh::getPosition(this->dimension, c);

		if (pos != Data::UNKNOWN)
		{
#if USE_IN_CELLS_AS_LIST
			iter = this->inCells.erase(iter);
#else
#if USE_C__11
			iter = this->inCells.erase(iter);
#else
			this->inCells.erase(iter++);
#endif //#if USE_C__11
#endif //#if USE_IN_CELLS_AS_LIST

			continue;
		}

		UInt numSides = c->numSides();

		bool changed = false;

		for (UInt i = 0; i < numSides; i++)
		{
			Data::GraphNodeList neighs;

			this->neighbors(c, i, neighs);

			/*for (Data::GraphNodeList::iterator iter2 = neighs.begin();
				 iter2 != neighs.end();)
			{
				Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>((*iter2));

				if (PMJMesh::getPosition(this->dimension, neigh) == Data::ON)
				{
					++iter2;

					continue;
				}

				iter2 = neighs.erase(iter2);
			}*/

			if (neighs.empty())
			{
				continue;
			}

			/*UInt dir = i%this->dimension;

			for (Data::GraphNodeList::iterator iter2 = neighs.begin();
				 iter2 != neighs.end(); iter2++)
			{
				Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>((*iter2));

#if USE_C__11
				CellFEListHashMap::iterator it = this->felsMap.find(neigh);
#else
				CellFEListMap::iterator it = this->felsMap.find(neigh);
#endif //#if USE_C__11

				if (it == this->felsMap.end())
				{
					continue;
				}

				if ((*it).second.empty())
				{
					continue;
				}

				Int count = 0;

				for (Data::FrontElementList::iterator iter3 = (*it).second.begin();
					 iter3 != (*it).second.end(); iter3++)
				{
					Data::FrontElement *fel = (*iter3);

					Data::Vector n = (this->dimension == 2) ?
						Data::Vector(static_cast<Data::Edge2D *>(fel->getShape())->normal()) :
						Data::Vector(static_cast<Data::Triangle3D *>(fel->getShape())->normal());

					//n.normalize();

					Real coord = n.getCoord(dir);

					if (coord > Data::getTolerance())
					{
						count++;
					}
					else if (coord < -Data::getTolerance())
					{
						count--;
					}
				}

				Int size = static_cast<Int>((*it).second.size());

				if ((count != size) && (count != -size))
				{
					continue;
				}

				if (this->newFrontInverted)
				{
					count = -count;
				}

				if (i >= this->dimension)
				{
					count = -count;
				}

				pos = (count > 0) ? Data::OUT : Data::IN;

				PMJMesh::setPosition(this->dimension, c, pos);

#if USE_IN_CELLS_AS_LIST
				this->setNeighborsPosition(c, pos);
#else
				this->setNeighborsPosition(c, pos, true);
#endif //#if USE_IN_CELLS_AS_LIST

				changed = true;

				break;
			}*/

			for (Data::GraphNodeList::iterator iter2 = neighs.begin();
				 iter2 != neighs.end(); iter2++)
			{
				Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>((*iter2));

#if USE_C__11
				CellFEListHashMap::iterator it = this->felsMap.find(neigh);
#else
				CellFEListMap::iterator it = this->felsMap.find(neigh);
#endif //#if USE_C__11

				if (it == this->felsMap.end())
				{
					continue;
				}

				enum Data::Position pos = this->classifyByOnNeighbor(i, (*it).second);

				if (pos == Data::UNKNOWN)
				{
					continue;
				}

				if (this->newFrontInverted)
				{
					pos = (pos == Data::IN) ? Data::OUT : Data::IN;
				}

				PMJMesh::setPosition(this->dimension, c, pos);

#if USE_IN_CELLS_AS_LIST
				this->setNeighborsPosition(c, pos);
#else
				this->setNeighborsPosition(c, pos, true);
#endif //#if USE_IN_CELLS_AS_LIST

				changed = true;

				break;
			}

			if (changed)
			{
				break;
			}
		}

		if (changed)
		{
#if USE_IN_CELLS_AS_LIST
			iter = this->inCells.erase(iter);
#else
#if USE_C__11
			iter = this->inCells.erase(iter);
#else
			this->inCells.erase(iter++);
#endif //#if USE_C__11
#endif //#if USE_IN_CELLS_AS_LIST

			continue;
		}

		iter++;
	}
}

void PMJMesh::WorkerCellClassifier2::classifyBasedOnPoint()
{
	//std::cout << "WorkerCellClassifier2::classifyBasedOnPoint" << std::endl;

	//std::cout << "incells.size = " << this->inCells.size() << std::endl;

	if (this->inCells.empty())
	{
		return;
	}

	if (!this->newFrontInverted)
	{
		//std::cout << "inverting new front" << std::endl;

		for (Data::Front::ConstAllFrontElementIterator iter = this->newFront->allEBegin(), end = this->newFront->allEEnd();
			 iter != end; iter++)
		{
			(*iter)->invert();
		}
	}

	Data::FrontElementList fels = this->oldFront->getAllElements();
	Data::FrontElementList moreFels = this->newFront->getAllElements();

	fels.splice(fels.end(), moreFels);

	//std::cout << "fels.size = " << fels.size() << std::endl;

	while (!this->inCells.empty())
	{
#if USE_IN_CELLS_AS_LIST
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(this->inCells.front());

		this->inCells.pop_front();
#else
#if USE_C__11
		Data::GraphNodeHash::iterator iter = this->inCells.begin();
#else
		Data::GraphNodeSet::iterator iter = this->inCells.begin();
#endif //#if USE_C__11

		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

		this->inCells.erase(iter);
#endif //#if USE_IN_CELLS_AS_LIST

		Data::Position pos = PMJMesh::getPosition(this->dimension, c);

		//std::cout << "c->id = " << c->getId() << ", pos = " << pos << std::endl;

		if (pos != Data::UNKNOWN)
		{
			//std::cout << "pos is not unknown" << std::endl;

			continue;
		}

		pos = this->classify(c, &fels);

		//std::cout << "cell classified as " << pos << std::endl;

		pos = (pos == Data::OUT) ? Data::IN : Data::OUT;

		//std::cout << "position changed to " << pos << std::endl;

		PMJMesh::setPosition(this->dimension, c, pos);

		//std::cout << "position for cell " << c->getId() << ": " << pos << std::endl;

#if USE_IN_CELLS_AS_LIST
		this->setNeighborsPosition(c, pos);
#else
		this->setNeighborsPosition(c, pos, true);
#endif //#if USE_IN_CELLS_AS_LIST
	}

	if (!this->newFrontInverted)
	{
		//std::cout << "deinverting new front" << std::endl;

		for (Data::Front::ConstAllFrontElementIterator iter = this->newFront->allEBegin(), end = this->newFront->allEEnd();
			 iter != end; iter++)
		{
			(*iter)->invert();
		}
	}
}

void PMJMesh::WorkerCellClassifier2::adjustPositionFromChildren(Data::BoxTreeCell *c)
{
	//std::cout << "WorkerCellClassifier2::adjustPositionFromChildren" << std::endl;

	Data::Position pos = PMJMesh::getPosition(this->dimension, c);

	//std::cout << "c->id = " << c->getId() << ", pos = " << pos << std::endl;

	if ((pos == Data::OUT) || (pos == Data::IN))
	{
		//std::cout << "pos is in or out" << std::endl;

		return;
	}

	if (pos == Data::UNKNOWN)
	{
		//std::cout << "pos is unknown, adjusting" << std::endl;

		if (this->dimension == 2)
		{
			static_cast<PMJMesh::QuadTreeCell *>(c)->adjustPositionFromChildren(true);
		}
		else
		{
			static_cast<PMJMesh::OctTreeCell *>(c)->adjustPositionFromChildren(true);
		}

		//std::cout << "pos is now " << PMJMesh::getPosition(this->dimension, c) << std::endl;

		return;
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->adjustPositionFromChildren(static_cast<Data::BoxTreeCell *>(c->child(i)));
	}
}

#endif //#if (USE_NEW_FRONT_UPDATER)
