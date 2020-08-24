#include "PMJMesh/NeighborTreeRefiner.h"

#include "Data/BoxTreeCell.h"
#include "Data/Tree.h"
#include "Data/BoxTree.h"
#include "MJMesh/Boundary.h"
#include "PMJMesh/AngleTreeCellClassifier.h"

#if USE_GUI
#include "Performer/TreeCellClassifier.h"
#endif //#if USE_GUI

PMJMesh::NeighborTreeRefiner::NeighborTreeRefiner(UInt dimension,
	Data::BoxTree *tree, const MJMesh::Boundary *boundary) :
		MJMesh::NeighborTreeRefiner(tree)
{
	this->setDimension(dimension);

	this->setBoundary(boundary);

	this->tcc = NULL;

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshGeneration = this->timePMJMeshClassification = 0.0;
#endif //#if DEBUG_TIMING_PMJMESH
}

PMJMesh::NeighborTreeRefiner::~NeighborTreeRefiner()
{
	//this->position.clear();

	if (this->tcc)
	{
		this->tcc = NULL;
	}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if (this->elementsMap)
	{
		delete this->elementsMap;
	}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
}

void PMJMesh::NeighborTreeRefiner::setDimension(UInt dimension)
{
	this->dimension = dimension;
}

void PMJMesh::NeighborTreeRefiner::setBoundary(const MJMesh::Boundary *boundary)
{
	this->boundary = boundary;
}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_C__11
void PMJMesh::NeighborTreeRefiner::setElementsMap(PMJMesh::FEListHashMap *elementsMap)
#else
void PMJMesh::NeighborTreeRefiner::setElementsMap(PMJMesh::FEListMap *elementsMap)
#endif //#if USE_C__11
{
	this->elementsMap = elementsMap;
}

#if USE_C__11
PMJMesh::FEListHashMap *PMJMesh::NeighborTreeRefiner::getElementsMap() const
#else
PMJMesh::FEListMap *PMJMesh::NeighborTreeRefiner::getElementsMap() const
#endif //#if USE_C__11
{
	return this->elementsMap;
}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

void PMJMesh::NeighborTreeRefiner::execute()
{
	if (!this->tree)
	{
		return;
	}

#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	//this->elements = this->boundary->getElements(); ///TODO optimize it (pointer to list)
	this->elements = &this->boundary->getElements(); ///TODO optimize it (pointer to list)

	this->tcc = this->makeTCC();

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	this->buildElementsMap();
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

/*#if USE_GUI
	static_cast<Data::BoxTree *>(this->tree)->freeze();
#endif //#if USE_GUI*/

#if DEBUG_TIMING_PMJMESH
	Real init = Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	/*Data::GraphNodeList leaves = this->tree->getLeaves();

    for (Data::GraphNodeList::iterator iter = leaves.begin();
		 iter != leaves.end(); iter++)
    {
        this->refine(static_cast<Data::BoxTreeCell *>((*iter)));
    }*/

    MJMesh::NeighborTreeRefiner::execute();

#if DEBUG_TIMING_PMJMESH
	Real exec = Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

/*#if USE_GUI
	static_cast<Data::BoxTree *>(this->tree)->unfreeze();
#endif //#if USE_GUI*/

    delete this->tcc;

	this->tcc = NULL;

	//this->elements.clear();

#if DEBUG_TIMING_PMJMESH
	Real end = Data::time() - debugstart;

	this->timePMJMeshGeneration += exec - this->timePMJMeshClassification;
	this->timePMJMeshClassification += init + end;
#endif //#if DEBUG_TIMING_PMJMESH

	this->status = MJMesh::NeighborTreeRefiner::ended;
}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
void PMJMesh::NeighborTreeRefiner::buildElementsMap() const
{
	if (this->elementsMap)
	{
		return;
	}

#if USE_C__11
	this->elementsMap = new FEListHashMap();
#else
	this->elementsMap = new FEListMap();
#endif //#if USE_C__11

	this->buildElementsMap(this->tree->getRoot(), this->elements);
}

//void PMJMesh::NeighborTreeRefiner::buildElementsMap(const Data::TreeNode *n, const Data::FrontElementList &elements) const
void PMJMesh::NeighborTreeRefiner::buildElementsMap(const Data::TreeNode *n, const Data::FrontElementList *elements) const
{
	if (n->isLeaf())
	{
		// (*this->elementsMap)[n->getId()] = elements;
		(*this->elementsMap)[n->getId()] = *elements;

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
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

PMJMesh::AngleTreeCellClassifier *PMJMesh::NeighborTreeRefiner::makeTCC() const
{
	return new PMJMesh::AngleTreeCellClassifier(this->dimension, NULL, this->boundary,
		const_cast<Data::BoxTree *>(static_cast<const Data::BoxTree *>(this->tree)), Data::UNKNOWN, true);
}

void PMJMesh::NeighborTreeRefiner::refine(Data::BoxTreeCell *c)
{
	if ((c->isLeaf()) && (PMJMesh::getPosition(this->dimension, c) == Data::OUT))
	{
		return;
	}

	MJMesh::NeighborTreeRefiner::refine(c);
}

void PMJMesh::NeighborTreeRefiner::fillSet(Data::BoxTreeCell *cell)
{
	if ((cell->isLeaf()) && (PMJMesh::getPosition(this->dimension, cell) == Data::OUT))
	{
		return;
	}

	MJMesh::NeighborTreeRefiner::fillSet(cell);
}

Data::Position PMJMesh::NeighborTreeRefiner::classifyChild(Data::BoxTreeCell *c, UInt i,
	//const Data::FrontElementList &elements, Data::FrontElementList &intercepting) const
	const Data::FrontElementList *elements, Data::FrontElementList &intercepting) const
{
	Data::Position childPos = Data::UNKNOWN;

	if (this->tcc->on(static_cast<Data::BoxTreeCell *>(c->child(i)), elements, intercepting))
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
				if (n->parent() != c)
				{
					continue;
				}

				if (pos == Data::ON)
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

#if USE_C__11
				FEListHashMap::iterator it2 = this->elementsMap->find(n->getId());
#else
				FEListMap::iterator it2 = this->elementsMap->find(n->getId());
#endif //#if USE_C__11

				if (it2 != this->elementsMap->end())
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

void PMJMesh::NeighborTreeRefiner::subdivide(Data::BoxTreeCell *c)
{
	MJMesh::NeighborTreeRefiner::subdivide(c);

#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	Data::Position pos = PMJMesh::getPosition(this->dimension, c);

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_C__11
	FEListHashMap::iterator it = (pos != Data::IN) ? this->elementsMap->find(c->getId()) : FEListHashMap::iterator();
#else
	FEListMap::iterator it = (pos != Data::IN) ? this->elementsMap->find(c->getId()) : FEListMap::iterator();
#endif //#if USE_C__11
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
			this->tcc->on(static_cast<Data::BoxTreeCell *>(c->child(i)), (*it).second, intercepting);
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION*/

			PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>(c->child(i)), pos);
		}
		else
		{
#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
			//childPos = this->classifyChild(c, i, (*it).second, intercepting);
			childPos = this->classifyChild(c, i, &(*it).second, intercepting);
#else
			Data::FrontElementList intercepting;
			Data::Position childPos = Data::UNKNOWN;

			childPos = this->classifyChild(c, i, this->elements, intercepting);
#endif //#if CLASSIFY_ON_REFINEMENT

			PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>(c->child(i)), childPos);
		}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
		if (childPos == Data::ON)
		{
			(*this->elementsMap)[c->child(i)->getId()].swap(intercepting);
		}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if (pos != Data::IN)
	{
		this->elementsMap->erase(it);
	}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshClassification += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH
}

void PMJMesh::NeighborTreeRefiner::refineChildren(Data::BoxTreeCell *c)
{
	UInt numChildren = c->numChildren();

	for (UInt i = 0; i < numChildren; i++)
	{
		this->refine(static_cast<Data::BoxTreeCell *>(c->child(i)));
	}
}
