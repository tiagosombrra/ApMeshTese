#include "PMJMesh/MinTreeRefiner.h"

#include "Data/OStream.h"
#include "Data/BoxTreeCell.h"
#include "Data/Tree.h"
#include "Data/BoxTree.h"
#include "Parallel/ThreadManager.h"
#include "MJMesh/Boundary.h"
#include "PMJMesh/AngleTreeCellClassifier.h"

#if USE_GUI
#include "Performer/TreeCellClassifier.h"
#endif //#if USE_GUI

using namespace PMJMesh;

PMJMesh::MinTreeRefiner::MinTreeRefiner(UInt dimension, Data::BoxTree *tree,
	const MJMesh::Boundary *boundary, UInt minLevel, bool postponeMinRefinement,
	const Parallel::ThreadManager *tm) :
		MJMesh::MinTreeRefiner(tree, boundary)
{
    this->setMinLevel(minLevel);

	this->setDimension(dimension);

	this->tcc = NULL;

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	this->elementsMap = NULL;
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

	this->setThreadManager(tm);

	this->setPostponeMinRefinement(postponeMinRefinement);

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshGeneration = this->timePMJMeshClassification = 0.0;
#endif //#if DEBUG_TIMING_PMJMESH
}

PMJMesh::MinTreeRefiner::~MinTreeRefiner()
{
	/*this->position.clear();
	this->elements.clear();

	if (this->tcc)
	{
		this->tcc = NULL;
	}*/

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if (this->elementsMap)
	{
		delete this->elementsMap;
	}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
}

void PMJMesh::MinTreeRefiner::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::MinTreeRefiner::setThreadManager(const Parallel::ThreadManager *tm)
{
	this->tm = tm;

	this->nthreads = (this->tm) ? this->tm->getMaxThreads() : 1;
}

void PMJMesh::MinTreeRefiner::setPostponeMinRefinement(bool postponeMinRefinement)
{
	this->postponeMinRefinement = postponeMinRefinement;
}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_C__11
void PMJMesh::MinTreeRefiner::setElementsMap(PMJMesh::FEListHashMap *elementsMap)
#else
void PMJMesh::MinTreeRefiner::setElementsMap(PMJMesh::FEListMap *elementsMap)
#endif //#if USE_C__11
{
	this->elementsMap = elementsMap;
}

#if USE_C__11
PMJMesh::FEListHashMap *PMJMesh::MinTreeRefiner::getElementsMap() const
#else
PMJMesh::FEListMap *PMJMesh::MinTreeRefiner::getElementsMap() const
#endif //#if USE_C__11
{
	return this->elementsMap;
}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

void PMJMesh::MinTreeRefiner::execute()
{
	/*//debug
	{
		std::ofstream file("/tmp/description");
		file << "describing tree:" << std::endl;
		Data::GraphNodeList cells;
		cells.push_back(this->tree->getRoot());

		while (!cells.empty())
		{
			Data::TreeNode *n = static_cast<Data::TreeNode *>(cells.front());

			cells.pop_front();

			file << "Cell " << n->getId() << ": ";

			file << "Leaf " << std::boolalpha << n->isLeaf() << ", ";

			Data::Position pos = PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n));

			file << "Position " << pos << ", ";

#if USE_C__11
			FEListHashMap::iterator iter2 = this->elementsMap->find(n->getId());
#else
			FEListMap::iterator iter2 = this->elementsMap->find(n->getId());
#endif //#if USE_C__11

			if (iter2 == this->elementsMap->end())
			{
				file << "No elements, ";
			}
			else
			{
				file << (*iter2).second.size() << " elements, ";
			}

			if (n->isLeaf())
			{
				if ((pos == Data::ON) && (iter2 == this->elementsMap->end()))
				{
					file << "error in ON leaf";
				}
				else if ((pos != Data::ON) && (iter2 != this->elementsMap->end()))
				{
					file << "error in IN or OUT leaf";
				}
				else
				{
					file << "leaf ok";
				}
			}
			else
			{
				if (iter2 != this->elementsMap->end())
				{
					file << "error on internal node";
				}
				else
				{
					file << "internal node ok";
				}
			}

			file << std::endl;

			for (UInt i = 0; i < n->numChildren(); i++)
			{
				cells.push_back(n->child(i));
			}
		}
	}
	//endebug*/

#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	if (this->boundary)
	{
		//Data::FrontElementVector els = this->boundary->getElements();
		//this->elements.insert(this->elements.end(), els.begin(), els.end());
		this->elements = &this->boundary->getElements(); ///TODO optimize it (pointer to list)

		this->tcc = this->makeTCC();
	}

#if USE_OPENMP
#if (!USE_EXPERIMENTAL_TREE)
	Data::GraphNodeVector leaves;

	if (this->nthreads > 1)
	{
		//Data::GraphNodeList ls = this->tree->getLeaves();

		//leaves.reserve(ls.size());
		leaves.reserve(this->tree->getLeavesSet().size());

		//leaves.assign(ls.begin(), ls.end());
		leaves.assign(this->tree->getLeavesSet().begin(), this->tree->getLeavesSet().end());

		//ls.clear();

		this->unsetTree(leaves);
	}
#endif //#if (!USE_EXPERIMENTAL_TREE)

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if (this->tcc)
	{
		this->buildElementsMap();
	}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#endif //#if USE_OPENMP

#if DEBUG_TIMING_PMJMESH
	Real init = Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	MJMesh::MinTreeRefiner::execute();

#if DEBUG_TIMING_PMJMESH
	Real exec = Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

#if USE_OPENMP
#if (!USE_EXPERIMENTAL_TREE)
	if (this->nthreads > 1)
	{
		this->setTree(leaves);
	}
#endif //#if (!USE_EXPERIMENTAL_TREE)

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if ((this->tcc) && (this->nthreads > 1) && (this->elementsMap))
	{
		for (UInt i = 0; i < static_cast<UInt>(this->tempElementsMap.size()); i++)
		{
#if USE_C__11
			for (FEListHashMap::iterator iter = this->tempElementsMap[i].begin();
#else
			for (FEListMap::iterator iter = this->tempElementsMap[i].begin();
#endif //#if USE_C__11
				 iter != this->tempElementsMap[i].end(); iter++)
			{
				ULInt id = (*iter).first;

				if (static_cast<Data::TreeNode *>(this->tree->node(id))->isLeaf())
				{
					(*this->elementsMap)[id].swap((*iter).second);
				}
				else
				{
					this->elementsMap->erase(id);
				}
			}
		}

		this->tempElementsMap.clear();
	}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#endif //#if USE_OPENMP

	if (this->boundary)
	{
		delete this->tcc;

		this->tcc = NULL;

		//this->elements.clear();
	}

#if DEBUG_TIMING_PMJMESH
	Real end = Data::time() - debugstart;

	this->timePMJMeshGeneration += exec - this->timePMJMeshClassification;
	this->timePMJMeshClassification += init + end;
#endif //#if DEBUG_TIMING_PMJMESH

	/*std::cout << "describing tree:" << std::endl;
	Data::GraphNodeList cells;
	cells.push_back(this->tree->getRoot());

	while (!cells.empty())
	{
		Data::TreeNode *n = static_cast<Data::TreeNode *>(cells.front());

		cells.pop_front();

		std::cout << "Cell " << n->getId() << ": ";

		Data::PositionMap::iterator iter = this->position.find(n->getId());

		if (iter == position.end())
		{
			std::cout << "No position, ";
		}
		else
		{
			std::cout << "Position " << (*iter).second << ", ";
		}

		FEListMap::iterator iter2 = this->elementsMap->find(n->getId());

		if (iter2 == this->elementsMap->end())
		{
			std::cout << "No elements, ";
		}
		else
		{
			std::cout << (*iter2).second.size() << " elements, ";
		}

		if (n->isLeaf())
		{
			if (((*iter).second == Data::ON) && (iter2 == this->elementsMap->end()))
			{
				std::cout << "error in ON leaf";
			}
			else if (((*iter).second != Data::ON) && (iter2 != this->elementsMap->end()))
			{
				std::cout << "error in IN or OUT leaf";
			}
			else
			{
				std::cout << "leaf ok";
			}
		}
		else
		{
			if (iter2 != this->elementsMap->end())
			{
				std::cout << "error on internal node";
			}
			else
			{
				std::cout << "internal node ok";
			}
		}

		std::cout << std::endl;

		for (UInt i = 0; i < n->numChildren(); i++)
		{
			cells.push_back(n->child(i));
		}
	}*/
}

UInt PMJMesh::MinTreeRefiner::findMinLevel() const
{
    return (this->getMinLevel() != 0) ? this->getMinLevel() : MJMesh::MinTreeRefiner::findMinLevel();
}

#if ((USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
void PMJMesh::MinTreeRefiner::buildElementsMap() const
{
	if (this->elementsMap)
	{
		if (this->nthreads <= 1)
		{
			return;
		}

		this->tempElementsMap.resize(this->nthreads);

		for (UInt i = 0; i < static_cast<UInt>(this->tempElementsMap.size()); i++)
		{
			this->tempElementsMap[i] = *this->elementsMap;
		}

		return;
	}

	//this->elementsMap.resize(this->nthreads);
	this->tempElementsMap.resize(this->nthreads);

	//UInt depth = this->tree->depth();

	/*//for (UInt i = 0; i < static_cast<UInt>(this->elementsMap.size()); i++)
	for (UInt i = 0; i < static_cast<UInt>(this->tempElementsMap.size()); i++)
	{
		//this->elementsMap[i].resize(depth + 1);
		//this->elementsMap[i][0] = this->elements;
		this->tempElementsMap[i][tree->getRoot()->getId()] = this->elements;
	}*/

	for (UInt i = 0; i < tree->getRoot()->numChildren(); i++)
	{
		Data::Position pos = PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(tree->getRoot()->child(i)));

		if ((pos == Data::UNKNOWN) || (pos == Data::OUT))
		{
			continue;
		}

		Data::FrontElementList intercepting;

		this->tcc->on(static_cast<Data::BoxTreeCell *>(tree->getRoot()->child(i)), this->elements, intercepting);

		//this->buildElementsMap(tree->getRoot()->child(i), intercepting);
		this->buildElementsMap(tree->getRoot()->child(i), &intercepting);
	}

	for (UInt i = 1; i < static_cast<UInt>(this->tempElementsMap.size()); i++)
	{
		this->tempElementsMap[i] = this->tempElementsMap[0];
	}
}

//void PMJMesh::MinTreeRefiner::buildElementsMap(const Data::TreeNode *n, const Data::FrontElementList &elements) const
void PMJMesh::MinTreeRefiner::buildElementsMap(const Data::TreeNode *n, const Data::FrontElementList *elements) const
{
	if (n->isLeaf())
	{
		//this->tempElementsMap[0][n->getId()] = elements;
		this->tempElementsMap[0][n->getId()] = *elements;

		return;
	}

	for (UInt i = 0; i < n->numChildren(); i++)
	{
		Data::Position pos = PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n->child(i)));

		if ((pos == Data::UNKNOWN) || (pos == Data::OUT))
		{
			continue;
		}

		Data::FrontElementList intercepting;

		this->tcc->on(static_cast<Data::BoxTreeCell *>(n->child(i)), elements, intercepting);

		//this->buildElementsMap(n->child(i), intercepting);
		this->buildElementsMap(n->child(i), &intercepting);
	}
}
#endif //#if ((USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))

#if USE_OPENMP
#if (!USE_EXPERIMENTAL_TREE)
void PMJMesh::MinTreeRefiner::unsetTree(Data::GraphNodeVector &leaves)
{
	//for (Data::GraphNodeVector::iterator iter = leaves.begin();
	//	 iter != leaves.end(); iter++)
	#pragma omp parallel for if(this->nthreads > 1)
	for (UInt i = 0; i < leaves.size(); i++)
	{
		//static_cast<Data::TreeNode *>((*iter))->setTree(NULL);
		static_cast<Data::TreeNode *>(leaves[i])->setTree(NULL);
	}
}

void PMJMesh::MinTreeRefiner::setTree(Data::GraphNodeVector &leaves)
{
	Data::GraphNodeList newLeaves[this->nthreads];

	#pragma omp parallel for if(this->nthreads > 1)
	for (UInt i = 0; i < leaves.size(); i++)
	{
		this->addTree(static_cast<Data::BoxTreeCell *>(leaves[i]),
			newLeaves[(this->nthreads > 1) ? this->tm->threadId() : 0]);
	}

	for (Int i = 1; i < this->nthreads; i++)
	{
		newLeaves[0].splice(newLeaves[0].end(), newLeaves[i]);
	}

	this->tree->setLeaves(newLeaves[0]);
}
#endif //#if (!USE_EXPERIMENTAL_TREE)
#endif //#if USE_OPENMP

#if (!USE_EXPERIMENTAL_TREE)
void PMJMesh::MinTreeRefiner::addTree(Data::BoxTreeCell *c, Data::GraphNodeList &newLeaves)
{
	c->setTree(this->tree);

	if (c->isLeaf())
	{
		newLeaves.push_back(c);

		return;
	}

	//this->tree->addChildren(c);

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		this->addTree(static_cast<Data::BoxTreeCell *>(c->child(i)), newLeaves);
	}
}
#endif //#if (!USE_EXPERIMENTAL_TREE)

PMJMesh::AngleTreeCellClassifier *PMJMesh::MinTreeRefiner::makeTCC() const
{
	return new PMJMesh::AngleTreeCellClassifier(this->dimension, this->tm, this->boundary,
		const_cast<Data::BoxTree *>(static_cast<const Data::BoxTree *>(this->tree)), Data::UNKNOWN, true);
}

Data::Position PMJMesh::MinTreeRefiner::classifyChild(Data::BoxTreeCell *c, UInt i,
#if ((USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
	UInt id,
#endif //#if ((USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
	//const Data::FrontElementList &elements, Data::FrontElementList &intercepting) const
	const Data::FrontElementList *elements, Data::FrontElementList &intercepting) const
{
	Data::Position childPos = Data::UNKNOWN;

	/*if ((c->child(i)->getId() == 46860) ||
        (c->child(i)->getId() == 47324))
    {
        std::cout << "debug " << c->child(i)->getId() << std::endl;
    }*/

	if ((!this->tcc) ||
		(this->tcc->on(static_cast<Data::BoxTreeCell *>(c->child(i)), elements, intercepting)))
	{
		childPos = Data::ON;
	}
	else
	{
		Data::GraphNodeList onNeighs[c->numSides()];

        bool foundIN = false, foundOUT = false;

		for (UInt side = 0; side < c->numSides(); side++)
		{
			Data::GraphNodeList neighs;

			static_cast<Data::BoxTree *>(this->tree)->neighbors(static_cast<Data::BoxTreeCell *>(c->child(i)), side, neighs);

			bool neighPos = false;

			for (Data::GraphNodeList::iterator iter = neighs.begin();
				 iter != neighs.end(); iter++)
			{
				Data::TreeNode *n = static_cast<Data::TreeNode *>((*iter));

				Data::Position pos = PMJMesh::getPosition(this->dimension, static_cast<Data::BoxTreeCell *>(n));

				if (n->parent() == c)
				{
				    if (pos == Data::IN)
                    {
                        foundIN = true;
                    }

                    if (pos == Data::OUT)
                    {
                        foundOUT = true;
                    }
				}
                else
				{
					if ((pos == Data::IN) || (pos == Data::OUT))
                    {
                        childPos = pos;
                        neighPos = true;

                        break;
                    }
				}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
				if ((this->tcc) &&
					(pos == Data::ON))
				{
					onNeighs[side].push_back(n);
				}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
			}

			if (neighPos ? (childPos != Data::UNKNOWN) : (childPos == Data::IN))
			{
				break;
			}
		}

		if ((childPos == Data::UNKNOWN) && (foundIN) && (!foundOUT))
        {
            childPos = Data::IN;
        }

        if ((childPos == Data::UNKNOWN) && (!foundIN) && (foundOUT))
        {
            childPos = Data::OUT;
        }

		if (childPos == Data::UNKNOWN)
		{
			while (true)
			{
				UInt maxSide = c->numSides();
				Data::GraphNodeList::iterator maxIter;

				for (UInt side = 0; side < c->numSides(); side++)
				{
					for (Data::GraphNodeList::iterator iter = onNeighs[side].begin();
						 iter != onNeighs[side].end(); iter++)
					{
						Data::TreeNode *n = static_cast<Data::TreeNode *>((*iter));

						if ((maxSide == c->numSides()) ||
							(n->level() > static_cast<Data::TreeNode *>((*maxIter))->level()))
						{
							maxIter = iter;
							maxSide = side;
						}
					}
				}

				if (maxSide == c->numSides())
				{
					break;
				}

				Data::TreeNode *n = static_cast<Data::TreeNode *>((*maxIter));

				onNeighs[maxSide].erase(maxIter);

#if USE_OPENMP
				bool useGlobal = this->tempElementsMap.empty();
#endif //#if USE_OPENMP

#if USE_C__11
				FEListHashMap::iterator it2 =
#else
				FEListMap::iterator it2 =
#endif //#if USE_C__11
#if USE_OPENMP
					(useGlobal ? this->elementsMap->find(n->getId()) : this->tempElementsMap[id].find(n->getId()));
#else
					this->elementsMap->find(n->getId());
#endif //#if USE_OPENMP

#if USE_OPENMP
				if (useGlobal ? (it2 != this->elementsMap->end()) : (it2 != this->tempElementsMap[id].end()))
#else
				if (it2 != this->elementsMap->end())
#endif //#if USE_OPENMP
				{
					Data::Position pos = this->tcc->classifyByOnNeighbor(maxSide, (*it2).second);

					if (pos == Data::UNKNOWN)
					{
						continue;
					}

					/*if (((childPos == Data::OUT) && (pos == Data::IN)) ||
                        ((childPos == Data::IN) && (pos == Data::OUT)))
                    {
                        childPos = Data::UNKNOWN;

                        break;
                    }
                    else
                    {
                        childPos = pos;
                    }*/

					childPos = pos;

					if (childPos == Data::IN)
					{
						break;
					}
				}
			}
		}

		if (childPos == Data::UNKNOWN)
		{
			childPos = this->tcc->classify(this->tcc->mid(static_cast<Data::BoxTreeCell *>(c->child(i))), this->elements);
		}
	}

	return childPos;
}

void PMJMesh::MinTreeRefiner::subdivide(Data::BoxTreeCell *c)
{
    /*Data::cout() << "subdividing cell " << c->getId()
        << ", level " << c->level()
        << ", position " << Data::position(PMJMesh::getPosition(this->dimension, c)) << Data::endl;*/

	MJMesh::MinTreeRefiner::subdivide(c);

#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

#if ((USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
	UInt id = this->tm ? this->tm->threadId() : 0;
#endif //#if ((USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))

	Data::Position pos = PMJMesh::getPosition(this->dimension, c);

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_OPENMP
	bool useGlobal = (this->tcc) ? this->tempElementsMap.empty() : false;

#if USE_C__11
	FEListHashMap::iterator it =
#else
	FEListMap::iterator it =
#endif //#if USE_C__11
		((this->tcc) && (pos != Data::IN)) ?
		(useGlobal ? this->elementsMap->find(c->getId()) : this->tempElementsMap[id].find(c->getId())) :
#if USE_C__11
		FEListHashMap::iterator();
#else
		FEListMap::iterator();
#endif //#if USE_C__11

	/*//debug
	if ((this->tcc) && (pos != Data::IN) && ((useGlobal) ? (it == this->elementsMap->end()) : (it == this->tempElementsMap[id].end())))
	{
		std::cout << "ERROR: cell " << c->getId() << " has no list of elements associated (PMJMesh::MinTreeRefiner::subdivide)" << std::endl;

		if (this->elementsMap->find(c->getId()) == this->elementsMap->end())
		{
			std::cout << "cell is NOT in global" << std::endl;
		}
		else
		{
			std::cout << "cell is in global" << std::endl;
		}

		if (this->tempElementsMap.empty())
		{
			std::cout << "locals are empty" << std::endl;
		}
		else
		{
			for (UInt i = 0; i < static_cast<UInt>(this->tempElementsMap.size()); i++)
			{
				if (this->tempElementsMap[i].find(c->getId()) == this->tempElementsMap[i].end())
				{
					std::cout << "cell is NOT in local " << i << std::endl;
				}
				else
				{
					std::cout << "cell is in local " << i << std::endl;
				}
			}
		}
	}
	//endebug*/
#else
#if USE_C__11
	FEListHashMap::iterator it =
#else
	FEListMap::iterator it =
#endif //#if USE_C__11
		((this->tcc) && (pos != Data::IN)) ?
		this->elementsMap->find(c->getId()) :
#if USE_C__11
		FEListHashMap::iterator();
#else
		FEListMap::iterator();
#endif //#if USE_C__11
#endif //#if USE_OPENMP
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

	for (UInt i = 0; i < c->numChildren(); i++)
	{
#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
		Data::FrontElementList intercepting;
		Data::Position childPos = Data::UNKNOWN;
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

		if (pos == Data::IN)
		{
/*#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
			if (this->tcc)
			{
				this->tcc->on(static_cast<Data::BoxTreeCell *>(c->child(i)), (*it).second, intercepting);
			}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION*/

			PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>(c->child(i)), pos);
		}
		else
		{
			//this->position[c->child(i)->getId()] = this->tcc->classify(static_cast<Data::BoxTreeCell *>(c->child(i)), this->elements);

			//Data::Position childPos = this->tcc ? this->tcc->classify(static_cast<Data::BoxTreeCell *>(c->child(i)), this->elements) : Data::ON;

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_OPENMP
			//childPos = this->classifyChild(c, i, id, (*it).second, intercepting);
			childPos = this->classifyChild(c, i, id, &(*it).second, intercepting);
#else
			//childPos = this->classifyChild(c, i, (*it).second, intercepting);
			childPos = this->classifyChild(c, i, &(*it).second, intercepting);
#endif //#if USE_OPENMP
#else
			Data::Position childPos = Data::UNKNOWN;
			Data::FrontElementList intercepting;

			childPos = this->classifyChild(c, i, this->elements, intercepting);
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

			PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>(c->child(i)), childPos);
		}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
		if ((this->tcc) && (childPos == Data::ON))
		{
#if USE_OPENMP
			if (useGlobal)
#endif //#if USE_OPENMP
			{
				(*this->elementsMap)[c->child(i)->getId()].swap(intercepting);
			}
#if USE_OPENMP
			else
			{
				this->tempElementsMap[id][c->child(i)->getId()].swap(intercepting);
			}
#endif //#if USE_OPENMP
		}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if ((this->tcc) && (pos != Data::IN))
	{
#if USE_OPENMP
		if (useGlobal)
#endif //#if USE_OPENMP
		{
			this->elementsMap->erase(it);
		}
#if USE_OPENMP
		else
		{
			this->tempElementsMap[id].erase(it);
		}
#endif //#if USE_OPENMP
	}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshClassification += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH
}

void PMJMesh::MinTreeRefiner::refine(Data::BoxTreeCell *c)
{
	//Data::Position pos = this->position[c->getId()];
	/*Data::Position pos = (this->positions.empty()) ?
		this->position[c->getId()] : this->positions[this->tm ? this->tm->threadId() : 0][c->getId()];

	if ((pos == Data::OUT) ||
		((this->postponeMinRefinement) && (pos == Data::IN)))
	{
		return;
	}*/

#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	/*if (((c->getId() == 46860) || (c->isDescendant(46860))) ||
		((c->getId() == 46881) || (c->isDescendant(46881))) ||
		((c->getId() == 46884) || (c->isDescendant(46884))) ||
		((c->getId() == 47315) || (c->isDescendant(47315))) ||
		((c->getId() == 47322) || (c->isDescendant(47322))) ||
		((c->getId() == 47323) || (c->isDescendant(47323))) ||
		((c->getId() == 47324) || (c->isDescendant(47324))) ||
		((c->getId() == 47331) || (c->isDescendant(47331))))
	{
		std::cout << "debug " << c->getId() << std::endl;
	}*/

	Data::Position pos = Data::UNKNOWN;

	pos = PMJMesh::getPosition(this->dimension, c);

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshClassification += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH

	if ((pos == Data::OUT) ||
		((this->postponeMinRefinement) && (pos == Data::IN)))
	{
		return;
	}

	MJMesh::MinTreeRefiner::refine(c);
}

void PMJMesh::MinTreeRefiner::refineChildren(Data::BoxTreeCell *c)
{
	UInt numChildren = c->numChildren();

#if USE_OPENMP
	//#pragma omp parallel for shared(c) if(this->parallel)
	//#pragma omp parallel for firstprivate(c) if(this->nthreads > 1)
#endif //#if USE_OPENMP
	for (UInt i = 0; i < numChildren; i++)
	{
		this->refine(static_cast<Data::BoxTreeCell *>(c->child(i)));
	}
}
