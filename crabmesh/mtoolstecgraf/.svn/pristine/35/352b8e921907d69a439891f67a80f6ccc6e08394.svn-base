#include "PMJMesh/TreeGenerator.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "MJMesh/Boundary.h"
#include "MJMesh/FrontElement.h"
#include "PMJMesh/AngleTreeCellClassifier.h"

#if ((USE_ATTRIBS) && (!USE_RECALC_ATTRIBS))
#include "Data/Box.h"
#endif //#if ((USE_ATTRIBS) && (!USE_RECALC_ATTRIBS))

#include "PMJMesh/QuadTree.h"
#include "PMJMesh/OctTree.h"

#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
#include "Parallel/ThreadManager.h"
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))

PMJMesh::TreeGenerator::TreeGenerator(UInt dimension, MJMesh::Boundary *boundary,
	Real factor
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
	, const Parallel::ThreadManager *tm
#endif //
	) :
		MJMesh::TreeGenerator(dimension, boundary, factor)
{
	this->tcc = NULL;

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	this->elementsMap = NULL;
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
	this->setThreadManager(tm);
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshGeneration = this->timePMJMeshClassification = 0.0;
#endif //#if DEBUG_TIMING_PMJMESH
}

PMJMesh::TreeGenerator::~TreeGenerator()
{
	if (this->tcc)
	{
		delete this->tcc;
	}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if (this->elementsMap)
	{
		delete this->elementsMap;
	}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
}

#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
void PMJMesh::TreeGenerator::setThreadManager(const Parallel::ThreadManager *tm)
{
	this->tm = tm;
}
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
#if USE_C__11
void PMJMesh::TreeGenerator::setElementsMap(FEListHashMap *elementsMap) const
#else
void PMJMesh::TreeGenerator::setElementsMap(FEListMap *elementsMap) const
#endif //#if USE_C__11
{
	this->elementsMap = elementsMap;
}

#if USE_C__11
PMJMesh::FEListHashMap *PMJMesh::TreeGenerator::getElementsMap() const
#else
PMJMesh::FEListMap *PMJMesh::TreeGenerator::getElementsMap() const
#endif //#if USE_C__11
{
	return this->elementsMap;
}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

void PMJMesh::TreeGenerator::execute()
{
#if DEBUG_TIMING_PMJMESH
	Real debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	this->tcc = this->makeTCC();

#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP) && (USE_ATTRIBS) && (!USE_RECALC_ATTRIBS))
	//to avoid the recreation of attribs in MJMesh::Edge2D and in
	//  MJMesh::Triangle3D
	//Data::FrontElementList fels = this->findElements();
	const Data::FrontElementList *fels = this->findElements();

//#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	//for (Data::FrontElementList::iterator iter = fels.begin();
	for (Data::FrontElementList::const_iterator iter = fels->begin();
		 //iter != fels.end(); iter++)
		 iter != fels->end(); iter++)
	{
		(*iter)->getShape()->box();
	}
/*#else
	while (!fels.empty())
	{
		fels.front()->getShape()->box();

		fels.pop_front();
	}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION*/
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP) && (USE_ATTRIBS) && (!USE_RECALC_ATTRIBS))

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshClassification += Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

	MJMesh::TreeGenerator::execute();

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshGeneration += Data::time() - debugstart;
	debugstart = Data::time();
#endif //#if DEBUG_TIMING_PMJMESH

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if (!this->elementsMap)
	{
#if USE_C__11
		this->elementsMap = new FEListHashMap();
#else
		this->elementsMap = new FEListMap();
#endif //#if USE_C__11
	}

	//this->elementsMap->insert(std::make_pair(this->getTree()->getRoot()->getId(), *this->findElements()));
/*#else
#if ((USE_OPENMP) && (USE_ATTRIBS) && (!USE_RECALC_ATTRIBS))
	//this->elementsMap->insert(std::make_pair(this->getTree()->getRoot()->getId(), fels));
	this->elementsMap->insert(std::make_pair(this->getTree()->getRoot()->getId(), *fels));

	//fels.clear();
#else
	this->elementsMap->insert(std::make_pair(this->getTree()->getRoot()->getId(), *this->findElements()));
#endif //#if ((USE_OPENMP) && (USE_ATTRIBS) && (!USE_RECALC_ATTRIBS))*/
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

#if USE_C__11
	this->elementsMap->reserve(this->getTree()->numLeaves());
#endif //#if USE_C__11

	if (this->status.getType() == MJMesh::TreeGenerator::ended.getType())
	{
#if USE_GUI
		static_cast<Data::BoxTree *>(this->getTree())->freeze();
#endif //#if USE_GUI

		if (this->boundary->size() == 0)
		{
			PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>(this->getTree()->getRoot()), Data::OUT);
		}
		else
		{
			this->classify();
		}

#if USE_GUI
		static_cast<Data::BoxTree *>(this->getTree())->unfreeze();
#endif //#if USE_GUI

		delete this->tcc;

		this->tcc = NULL;
	}

#if DEBUG_TIMING_PMJMESH
	this->timePMJMeshClassification += Data::time() - debugstart;
#endif //#if DEBUG_TIMING_PMJMESH

	/*//debug
	{
		std::ofstream file("/tmp/description");
		file << "describing tree:" << std::endl;
		Data::GraphNodeList cells;
		cells.push_back(this->getTree()->getRoot());

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
}

//Data::FrontElementList PMJMesh::TreeGenerator::findElements() const
const Data::FrontElementList *PMJMesh::TreeGenerator::findElements() const
{
	//return this->tcc ? this->tcc->findElements() : Data::FrontElementList();
	return this->tcc ? this->tcc->findElements() : NULL;
}

/*bool PMJMesh::TreeGenerator::subdivide(Data::BoxTreeCell *c, MJMesh::FrontElement *e)
{
	bool value = MJMesh::TreeGenerator::subdivide(c, e);

	if ((value) &&
		(this->position[c->getId()] == Data::UNKNOWN)) //&&
		//(this->tcc->on(c, Data::FrontElementList(1, e))))
	{
#if USE_GUI
		PMJMesh::AngleTreeCellClassifier::setPosition(this->position, c, Data::ON);
#else
		this->position[c->getId()] = Data::ON;
#endif //#if USE_GUI
	}

	return value;
}*/

Data::Tree *PMJMesh::TreeGenerator::makeTree(const Data::Point2D &min, const Data::Point2D &max) const
{
	return new PMJMesh::QuadTree(min, max);
}

Data::Tree *PMJMesh::TreeGenerator::makeTree(const Data::Point3D &min, const Data::Point3D &max) const
{
	return new PMJMesh::OctTree(min, max);
}

void PMJMesh::TreeGenerator::subdivide(Data::BoxTreeCell *c) const
{
	MJMesh::TreeGenerator::subdivide(c);

	this->classifySubdividedCell(c);
}

void PMJMesh::TreeGenerator::classifySubdividedCell(Data::BoxTreeCell *c) const
{
	PMJMesh::setPosition(this->dimension, c, Data::ON);

/*#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	FEListMap::iterator it = this->elementsMap->find(c->getId());
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION*/

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		Data::Position pos = Data::UNKNOWN;

/*#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
		if (this->tcc->on(static_cast<Data::BoxTreeCell *>(c->child(i)), (*it).second, (*this->elementsMap)[c->child(i)->getId()]))
		{
			pos = Data::ON;
		}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION*/

		PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>(c->child(i)), pos);
	}

/*#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	this->elementsMap->erase(it);
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION*/
}

//void PMJMesh::TreeGenerator::findOnCells(const Data::BoxTreeCell *c, const Data::FrontElementList &elements) const
void PMJMesh::TreeGenerator::findOnCells(const Data::BoxTreeCell *c, const Data::FrontElementList *elements) const
{
	Data::FrontElementList intercepting;

	if ((this->tcc->on(c, elements, intercepting)) &&
		(PMJMesh::getPosition(this->dimension, c) == Data::UNKNOWN) &&
		(c->isLeaf())) //&&
		//(this->tcc->on(c, elements)))
	{
		PMJMesh::setPosition(this->dimension, const_cast<Data::BoxTreeCell *>(c), Data::ON);

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
		if (this->elementsMap)
		{
			(*this->elementsMap)[c->getId()].swap(intercepting);
		}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	}

	if ((c->isLeaf()) || (intercepting.empty()))
	{
		return;
	}

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		//this->findOnCells(static_cast<Data::BoxTreeCell *>(c->child(i)), intercepting);
		this->findOnCells(static_cast<Data::BoxTreeCell *>(c->child(i)), &intercepting);
	}
}

PMJMesh::AngleTreeCellClassifier *PMJMesh::TreeGenerator::makeTCC() const
{
	return new PMJMesh::AngleTreeCellClassifier(this->dimension,
#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		this->tm,
#else
		NULL,
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
		this->boundary, static_cast<Data::BoxTree *>(this->getTree()), Data::ON, true);
}

void PMJMesh::TreeGenerator::classify() const
{
	//Real debugstart = Data::time();

	//Data::FrontElementList elements = this->findElements();
	const Data::FrontElementList *elements = this->findElements();

	//Data::FrontElementList fels = elements;

	//this->findOnCells(static_cast<Data::BoxTreeCell *>(this->getTree()->getRoot()), fels);
	//this->findOnCells(static_cast<Data::BoxTreeCell *>(this->getTree()->getRoot()), elements);
	Data::TreeNode *root = this->getTree()->getRoot();
	UInt numChildren = root->numChildren();

	if (numChildren == 0)
	{
		this->elementsMap->insert(std::make_pair(this->getTree()->getRoot()->getId(), *elements));

		PMJMesh::setPosition(this->dimension, static_cast<Data::BoxTreeCell *>(root), Data::ON);

		return;
	}

	//std::cout << "PMJMesh::TreeGenerator::classify(), time for initializing classification = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
	//#pragma omp parallel for shared(c) schedule(dynamic) if(((this->tm) && (this->tm->isSharedParallelismEnabled())))
	#pragma omp parallel for schedule(dynamic) if(((this->tm) && (this->tm->isSharedParallelismEnabled()))) shared(elements)
#endif //#if ((!USE_FEL_MAP_IN_TREE_CONSTRUCTION) && (USE_OPENMP))
	for (UInt i = 0; i < numChildren; i++)
	{
		this->findOnCells(static_cast<Data::BoxTreeCell *>(root->child(i)), elements);
	}

	//std::cout << "PMJMesh::TreeGenerator::classify(), time for finding on cells = " << Data::time() - debugstart << std::endl;
	//debugstart = Data::time();

#if (USE_FEL_MAP_IN_TREE_CONSTRUCTION)
	bool ok = this->classifyByOnNeighbor(static_cast<Data::BoxTreeCell *>(this->getTree()->getRoot()));

	if (!ok)
	{
		this->classifyUnknowns(elements);
	}
#else
	this->classify(static_cast<Data::BoxTreeCell *>(this->getTree()->getRoot()), elements);
#endif //#if (USE_FEL_MAP_IN_TREE_CONSTRUCTION)

	//std::cout << "PMJMesh::TreeGenerator::classify(), time for classifying other cells = " << Data::time() - debugstart << std::endl;
}

#if (USE_FEL_MAP_IN_TREE_CONSTRUCTION)
bool PMJMesh::TreeGenerator::classifyByOnNeighbor(const Data::BoxTreeCell *c) const
{
	/*bool print = false;

	{
		std::cout << "cell " << c->getId() << std::endl;

		print = true;
	}*/

	if (c->isLeaf())
	{
		/*if (print)
		{
			std::cout << "cell is leaf" << std::endl;
		}*/

		enum Data::Position pos = PMJMesh::getPosition(this->dimension, c);

		if (pos != Data::UNKNOWN)
		{
			/*if (print)
			{
				std::cout << "cell is already classified: " << pos << std::endl;
			}*/

			return true;
		}

		//if it's IN by any neighbor, it will be in;
		//  if it's OUT by all neighbors, it will be out;
		//  UNKNOWNs are discarded

		for (UInt side = 0; side < c->numSides(); side++)
		{
			Data::GraphNodeList neighs;

			c->neighbors(side, neighs);

			for (Data::GraphNodeList::iterator iter = neighs.begin();
				 iter != neighs.end(); iter++)
			{
				Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>((*iter));

				if (n->parent() != c->parent())
				{
					continue;
				}

				enum Data::Position npos = PMJMesh::getPosition(this->dimension, n);

				if (npos != Data::ON)
				{
					if (npos != Data::UNKNOWN)
					{
						pos = npos;

						break;
					}

					continue;
				}

#if USE_C__11
				FEListHashMap::iterator it = this->elementsMap->find(n->getId());
#else
				FEListMap::iterator it = this->elementsMap->find(n->getId());
#endif //#if USE_C__11

				if ((it == this->elementsMap->end()) || ((*it).second.empty()))
				{
					continue;
				}

				npos = this->tcc->classifyByOnNeighbor(side, (*it).second);

				if (npos == Data::UNKNOWN)
				{
					continue;
				}

				pos = npos;

				if (pos == Data::IN)
				{
					break;
				}
			}

			if (pos == Data::IN)
			{
				break;
			}
		}

		if (pos == Data::UNKNOWN)
		{
			this->unknowns.push_back(const_cast<Data::BoxTreeCell *>(c));

			return false;
		}

		PMJMesh::setPosition(this->dimension, const_cast<Data::BoxTreeCell *>(c), pos);

		this->setNeighborsPosition(c, pos);

		/*if (print)
		{
			std::cout << "cell tried to be classified: " << pos << std::endl;
		}*/

		return true;
	}

	/*if (print)
	{
		std::cout << "cell is not leaf " << std::endl;
	}*/

	enum Data::Position pos = PMJMesh::getPosition(this->dimension, c);

	if ((pos != Data::UNKNOWN) && ((pos != Data::ON)))
	{
		/*if (print)
		{
			std::cout << "cell was classified: " << pos << std::endl;
		}*/

		return true;
	}

	Data::GraphNodeList on, unknowns;

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		Data::BoxTreeCell *child = static_cast<Data::BoxTreeCell *>(c->child(i));

		enum Data::Position cpos = PMJMesh::getPosition(this->dimension, child);

		if (cpos == Data::ON)
		{
			on.push_back(child);
		}
		else if (cpos == Data::UNKNOWN)
		{
			unknowns.push_back(child);
		}
	}

	bool unknown = false;

	for (Data::GraphNodeList::iterator iter = on.begin();
		 iter != on.end(); iter++)
	{
		/*if (print)
		{
			std::cout << "will try to classify ON child " << (*iter)->getId() << std::endl;
		}*/

		if (!this->classifyByOnNeighbor(static_cast<Data::BoxTreeCell *>((*iter))))
		{
			unknown = true;
		}
	}

	//do it multiple times because the first one(s) might not be neighbor of a ON cell
	for (UInt i = 0; i < this->dimension; i++)
	{
		Data::GraphNodeList newUnknowns;

		for (Data::GraphNodeList::iterator iter = unknowns.begin();
			 iter != unknowns.end(); iter++)
		{
			/*if (print)
			{
				std::cout << "will try to classify UNKNOWN child " << (*iter)->getId() << "for the time " << i + 1 << std::endl;
			}*/

			if (!this->classifyByOnNeighbor(static_cast<Data::BoxTreeCell *>((*iter))))
			{
				if ((i + 1) == this->dimension)
				{
					unknown = true;
				}
				else
				{
					newUnknowns.push_back((*iter));
				}
			}
		}

		if (newUnknowns.empty())
		{
			break;
		}

		unknowns.swap(newUnknowns);
	}

	return (!unknown);
}

void PMJMesh::TreeGenerator::classifyUnknowns(const Data::FrontElementList *elements) const
{
	while (!this->unknowns.empty())
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(this->unknowns.front());

		this->unknowns.pop_front();

		enum Data::Position pos = PMJMesh::getPosition(this->dimension, c);

		if (pos != Data::UNKNOWN)
		{
			continue;
		}

		pos = this->tcc->classify(this->tcc->mid(c), elements);

		PMJMesh::setPosition(this->dimension, c, pos);

		this->setNeighborsPosition(c, pos);
	}
}
#else
//enum Data::Position PMJMesh::TreeGenerator::classify(const Data::BoxTreeCell *c, const Data::FrontElementList &elements) const
enum Data::Position PMJMesh::TreeGenerator::classify(const Data::BoxTreeCell *c, const Data::FrontElementList *elements) const
{
	enum Data::Position pos = PMJMesh::getPosition(this->dimension, c);

	if (c->isLeaf())
	{
		if (pos == Data::UNKNOWN)
		{
			pos = this->tcc->classify(this->tcc->mid(c), elements);

			PMJMesh::setPosition(this->dimension, const_cast<Data::BoxTreeCell *>(c), pos);

			this->setNeighborsPosition(c, pos);
		}

		return pos;
	}

	pos = Data::UNKNOWN;

	for (UInt i = 0; i < c->numChildren(); i++)
	{
		enum Data::Position cpos = this->classify(static_cast<Data::BoxTreeCell *>(c->child(i)), elements);

		if (pos == Data::UNKNOWN)
		{
			pos = cpos;
		}
		else if (cpos != pos)
		{
			pos = Data::ON;
		}
	}

	PMJMesh::setPosition(this->dimension, const_cast<Data::BoxTreeCell *>(c), pos);

	return pos;
}
#endif //#if (USE_FEL_MAP_IN_TREE_CONSTRUCTION)

void PMJMesh::TreeGenerator::setNeighborsPosition(const Data::BoxTreeCell *c, enum Data::Position pos) const
{
	Data::GraphNodeList neighs = static_cast<Data::BoxTree *>(this->getTree())->neighbors(c);

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
	if (this->elementsMap)
	{
		this->elementsMap->erase(c->getId());
	}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

	while (!neighs.empty())
	{
		Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>(neighs.back());

		neighs.pop_back();

		if (PMJMesh::getPosition(this->dimension, neigh) != Data::UNKNOWN)
		{
			continue;
		}

#if USE_FEL_MAP_IN_TREE_CONSTRUCTION
		if (this->elementsMap)
		{
			this->elementsMap->erase(neigh->getId());
		}
#endif //#if USE_FEL_MAP_IN_TREE_CONSTRUCTION

		PMJMesh::setPosition(this->dimension, neigh, pos);

		//Data::GraphNodeList moreNeighs = static_cast<Data::BoxTree *>(this->getTree())->neighbors(neigh);
		//neighs.insert(neighs.end(), moreNeighs.begin(), moreNeighs.end());

		static_cast<Data::BoxTree *>(this->getTree())->neighbors(neigh, neighs);

		//this->setNeighborsPosition(neigh, pos);
	}
}
