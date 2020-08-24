#include "PMJMesh/AngleTreeCellClassifier.h"

#include "Data/Edge2D.h"
#include "Data/Triangle3D.h"
#include "Data/Boundary.h"
#include "Data/BoxTreeCell.h"
#include "Parallel/ThreadManager.h"
#include "MJMesh/AnglePointInObjectDecider.h"
#include "PMJMesh/Front.h"
#include "PMJMesh/QuadTreeCell.h"
#include "PMJMesh/OctTreeCell.h"

PMJMesh::AngleTreeCellClassifier::AngleTreeCellClassifier(UInt dimension,
	const Parallel::ThreadManager *tm, const Data::Boundary *boundary, const Data::BoxTree *tree,
	Data::Position doNotClassify, bool classifyChildren) :
	Performer::AngleTreeCellClassifier(dimension, boundary, tree)
{
	this->setDoNotClassify(doNotClassify);
	this->setClassifyChildren(classifyChildren);
	this->setThreadManager(tm);
}

/*PMJMesh::AngleTreeCellClassifier::AngleTreeCellClassifier(UInt dimension,
	const Parallel::ThreadManager *tm, const Data::Front *front, const Data::BoxTree *tree,
	Data::Position doNotClassify, bool classifyChildren) :
	Performer::AngleTreeCellClassifier(dimension, front, tree)
{
	this->setDoNotClassify(doNotClassify);
	this->setClassifyChildren(classifyChildren);
	this->setThreadManager(tm);
}*/

PMJMesh::AngleTreeCellClassifier::~AngleTreeCellClassifier()
{

}

void PMJMesh::AngleTreeCellClassifier::setPosition(const Data::PositionMap &pos)
{
	this->position = pos;
}

void PMJMesh::AngleTreeCellClassifier::setDoNotClassify(Data::Position doNotClassify)
{
	this->doNotClassify = doNotClassify;
}

void PMJMesh::AngleTreeCellClassifier::setClassifyChildren(bool classifyChildren)
{
	this->classifyChildrenEnabled = classifyChildren;
}

void PMJMesh::AngleTreeCellClassifier::setThreadManager(const Parallel::ThreadManager *tm)
{
	this->tm = tm;
}

void PMJMesh::AngleTreeCellClassifier::setChildrenPosition(const Data::BoxTreeCell *cell,
	enum Data::Position pos)
{
	Data::PositionMap::iterator it = this->position.find(cell->getId());

	if ((it != this->position.end()) &&
		((*it).second == pos))
	{
		return;
	}

	Performer::AngleTreeCellClassifier::setChildrenPosition(cell, pos);
}

Performer::AnglePointInObjectDecider *PMJMesh::AngleTreeCellClassifier::makeApiod(Data::Point *p,
	const Data::FrontElementList *elements, bool canBeUnknown) const
{
	return new MJMesh::AnglePointInObjectDecider(this->dimension, p, elements, canBeUnknown);
}

//Data::FrontElementList PMJMesh::AngleTreeCellClassifier::findElements() const
const Data::FrontElementList *PMJMesh::AngleTreeCellClassifier::findElements() const
{
	if (this->boundary)
	{
		//return this->boundary->getElements(); ///TODO optimize it (pointer to list)
		return &this->boundary->getElements(); ///TODO optimize it (pointer to list)
		/*Data::FrontElementVector els = this->boundary->getElements();

		Data::FrontElementList elements(els.begin(), els.end());

		return elements;*/
	}

	/*if (this->front)
	{
		return static_cast<const PMJMesh::Front *>(this->front)->getAllElements();
	}*/

	//return Data::FrontElementList();
	return NULL;
}

void PMJMesh::AngleTreeCellClassifier::classifyChildren(
	//const Data::BoxTreeCell *cell, const Data::FrontElementList &elements)
	const Data::BoxTreeCell *cell, const Data::FrontElementList *elements)
{
	Int numChildren = cell->numChildren();

#if USE_OPENMP
	/*//#pragma omp parallel for shared(cell) schedule(dynamic) \*/
	#pragma omp parallel for firstprivate(cell) schedule(dynamic) \
		if((this->tm) && (this->tm->isSharedParallelismEnabled()) && (this->tm->getMaxThreads() != this->tm->numThreads())) \
		num_threads((this->tm ? (this->tm->getMaxThreads() - this->tm->numThreads()) : 0) + 1)
#endif //#if USE_OPENMP
	for (Int i = 0; i < numChildren; i++)
	{
		this->classify(static_cast<Data::BoxTreeCell *>(cell->child(i)), elements);
	}
}

bool PMJMesh::AngleTreeCellClassifier::findClassificationDirections(
	const Data::FrontElementList &felsInCell, Int directions[])
{
	if (felsInCell.empty())
	{
		return false;
	}

	for (Data::FrontElementList::const_iterator iter = felsInCell.begin();
		 iter != felsInCell.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

#if USE_EXPERIMENTAL_3
        struct Directions
        {
            static void directions(const UInt &dimension, const Data::Vector &v, Int directions[])
            {
                for (UInt i = 0; i < dimension; i++)
                {
                    Real coord = v.getCoord(i);

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
        };

        if (this->dimension == 2)
        {
            Data::Vector2D n = static_cast<Data::Edge2D *>(fel->getShape())->normal();

            Directions::directions(this->dimension, n, directions);
        }
        else
        {
            Data::Vector3D n = static_cast<Data::Triangle3D *>(fel->getShape())->normal();

            Directions::directions(this->dimension, n, directions);
        }
#else
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
#endif //#if USE_EXPERIMENTAL_3
	}

	bool foundDirection = false;

	Int size = static_cast<Int>(felsInCell.size());

	for (UInt i = 0; i < this->dimension; i++)
	{
		if ((directions[i] == size) ||
			(directions[i] == -size))
		{
			foundDirection = true;
		}
		else
		{
			directions[i] = 0;
		}
	}

	return foundDirection;
}

void PMJMesh::AngleTreeCellClassifier::findOnNeighbors(Data::GraphNodeList &neighsInSide) const
{
	for (Data::GraphNodeList::iterator iter = neighsInSide.begin();
		 iter != neighsInSide.end();)
	{
		Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>((*iter));

		if (PMJMesh::getPosition(this->dimension, neigh) == Data::ON)
		{
			++iter;

			continue;
		}

		iter = neighsInSide.erase(iter);
	}
}

enum Data::Position PMJMesh::AngleTreeCellClassifier::classifyByOnNeighbor(UInt side,
	const Data::FrontElementList &felsInNeighbor) const
{
	if (felsInNeighbor.empty())
	{
		return Data::UNKNOWN;
	}

	UInt dir = side%this->dimension;

	Int count = 0;

	for (Data::FrontElementList::const_iterator iter = felsInNeighbor.begin();
		 iter != felsInNeighbor.end(); iter++)
	{
		Data::FrontElement *fel = (*iter);

#if USE_EXPERIMENTAL_3
        struct Coord
        {
            static void coord(const UInt &dir, const Data::Vector &v, Int &count)
            {
                Real coord = v.getCoord(dir);

                if (coord > Data::getTolerance())
                {
                    count++;
                }
                else if (coord < -Data::getTolerance())
                {
                    count--;
                }
            }
        };

        if (this->dimension == 2)
        {
            Data::Vector2D n = static_cast<Data::Edge2D *>(fel->getShape())->normal();

            Coord::coord(dir, n, count);
        }
        else
        {
            Data::Vector3D n = static_cast<Data::Triangle3D *>(fel->getShape())->normal();

            Coord::coord(dir, n, count);
        }
#else
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
#endif //#if USE_EXPERIMENTAL_3
	}

	Int size = static_cast<Int>(felsInNeighbor.size());

	if ((count != size) && (count != -size))
	{
		return Data::UNKNOWN;
	}

	if (side >= this->dimension)
	{
		count = -count;
	}

	return (count > 0) ? Data::OUT : Data::IN;
}

enum Data::Position PMJMesh::AngleTreeCellClassifier::classifyByOnNeighbor(
	const Data::FrontElementList *elements, UInt side, Data::GraphNodeList &neighsInSide) const
{
	this->findOnNeighbors(neighsInSide);

	if (neighsInSide.empty())
	{
		return Data::UNKNOWN;
	}

	for (Data::GraphNodeList::iterator iter = neighsInSide.begin();
		 iter != neighsInSide.end(); iter++)
	{
		Data::BoxTreeCell *neigh = static_cast<Data::BoxTreeCell *>((*iter));

		Data::FrontElementList felsInNeigh;

		this->on(neigh, elements, felsInNeigh, false);

		enum Data::Position pos = this->classifyByOnNeighbor(side, felsInNeigh);

		if (pos != Data::UNKNOWN)
		{
			return pos;
		}
	}

	return Data::UNKNOWN;
}

enum Data::Position PMJMesh::AngleTreeCellClassifier::classifyByOnNeighbor(const Data::BoxTreeCell *c,
	const Data::FrontElementList *elements) const
{
	UInt numSides = c->numSides();

	for (UInt i = 0; i < numSides; i++)
	{
		Data::GraphNodeList neighs;

		c->neighbors(i, neighs);

		enum Data::Position pos = this->classifyByOnNeighbor(elements, i, neighs);

		if (pos != Data::UNKNOWN)
		{
			return pos;
		}
	}

	return Data::UNKNOWN;
}

enum Data::Position PMJMesh::AngleTreeCellClassifier::classify(
	//const Data::BoxTreeCell *cell, const Data::FrontElementList &elements)
	const Data::BoxTreeCell *cell, const Data::FrontElementList *elements)
{
	Data::PositionMap::iterator it = this->position.find(cell->getId());

	if ((it != this->position.end()) &&
		((*it).second == this->doNotClassify))
	{
		if (this->classifyChildrenEnabled)
		{
#if USE_GUI
			this->setPosition(cell, (*it).second);
#endif //#if USE_GUI

			this->classifyChildren(cell, elements);
		}

		return (*it).second;
	}

	return Performer::AngleTreeCellClassifier::classify(cell, elements);
}

void PMJMesh::AngleTreeCellClassifier::adjustPosition()
{
	UInt numChildren = (this->dimension == 2) ? 4 : 8;

	/*for (Data::PositionMap::reverse_iterator iter = this->position.rbegin();
		 iter != --this->position.rend(); iter++)
	{
		ULInt parent = ((*iter).first - 1)/numChildren;

		this->position.insert(std::make_pair(parent, Data::UNKNOWN));
	}

	bool changed = true;

	while (changed)
	{
		changed = false;

//		ULInt max = 0;
//
//		for (Data::PositionMap::iterator iter = this->position.begin();
//			 iter != this->position.end(); iter++)
//		{
//			if ((*iter).first > max)
//			{
//				max = (*iter).first;
//			}
//		}

		//std::cout << "max = " << max << ", rbegin = " << (*this->position.rbegin()).first << std::endl;

		ULInt max = (*this->position.rbegin()).first;

		for (Data::PositionMap::reverse_iterator iter = this->position.rbegin();
			 iter != this->position.rend();)
		{
			ULInt id = (*iter).first;

			if (this->adjustPosition(id, numChildren, (*iter).second, max))
			{
				changed = true;

				iter = this->position.rbegin();

				max = (*this->position.rbegin()).first;
			}
			else
			{
				iter++;
			}
		}
	}*/

	for (Data::PositionMap::reverse_iterator iter = this->position.rbegin();
		 iter != this->position.rend();)
	{
		ULInt id = (*iter).first;

		if ((id%numChildren) != 0)
		{
			iter++;

			continue;
		}

		Data::Position pos = (*iter).second;

		if (this->ignore(id, pos))
		{
			iter++;

			continue;
		}

		bool equal = true;

		bool allChildren = true;

		for (UInt i = 1; i < numChildren; i++)
		{
			iter++;

			if (((*iter).first != id - i) ||
				(iter == this->position.rend()))
			{
				allChildren = false;

				break;
			}

			if ((*iter).second != pos)
			{
				equal = false;
			}
		}

		if (allChildren)
		{
			ULInt parent = (id - 1)/numChildren;

			if ((equal) && (pos != Data::ON))
			{
				iter++;

				id = (*iter).first;

				/*std::cout << "replacing children " << id << " to " << id + numChildren << " for its parent " << parent << " with position " <<
					((pos == Data::IN) ? "IN" : ((pos == Data::OUT) ? "OUT" : ((pos == Data::ON) ? "ON" : "UNKNOWN"))) << std::endl;*/

				Data::PositionMap::iterator first = iter.base();

				Data::PositionMap::iterator last = first;

				std::advance(last, numChildren);

				this->position.erase(first, last);

				this->position[parent] = pos;

				iter = Data::PositionMap::reverse_iterator(this->position.find(id));
				iter--;
			}
			else
			{
				Data::PositionMap::iterator it = this->position.find(parent);

				if (it != this->position.end())
				{
					/*std::cout << "replacing parent " << parent << " from position " <<
						(((*it).second == Data::IN) ? "IN" : (((*it).second == Data::OUT) ? "OUT" : (((*it).second == Data::ON) ? "ON" : "UNKNOWN"))) <<
						" to position ON" << std::endl;*/

					(*it).second = Data::ON;
				}

				iter++;
			}
		}
	}
}

/*bool PMJMesh::AngleTreeCellClassifier::adjustPosition(ULInt id, UInt numChildren, Data::Position pos, ULInt max)
{
	if ((id == 0) || (pos == Data::ON))
	{
		return false;
	}

	ULInt child = (id - 1)%numChildren;

	ULInt parent = (id - 1 - child)/numChildren;

	ULInt first = parent*numChildren + 1;
	ULInt last = first + numChildren;

	for (ULInt i = first; i < last; i++)
	{
		if (i == id)
		{
			continue;
		}

		Data::PositionMap::iterator it = this->position.find(i);

		if (it == this->position.end())
		{
			return false;
		}

		if ((*it).second != pos)
		{
			return false;
		}
	}

	//this->removeChildrenPosition(parent, max, numChildren);
	this->removeDescendantsPositions(parent, max, numChildren);

	this->position[parent] = pos;

	//std::cout << "now adding position[ " << parent << " ] as " << pos << std::endl << std::endl << std::endl;

	return true;
}

void PMJMesh::AngleTreeCellClassifier::removeChildrenPosition(ULInt id, ULInt max, UInt numChildren)
{
	ULInt first = id*numChildren + 1;

	if (first <= max)
	{
		ULInt last = first + numChildren;

		for (ULInt i = first; i < last; i++)
		{
			if (i > max)
			{
				break;
			}

			this->removeChildrenPosition(i, max, numChildren);

			this->position.erase(i);
		}
	}
}

void PMJMesh::AngleTreeCellClassifier::removeDescendantsPositions(ULInt id, ULInt max, UInt numChildren)
{
	Data::ULIntVector descendants;

	for (Data::PositionMap::iterator iter = this->position.begin();
		 iter != this->position.end(); iter++)
	{
		ULInt testing = (*iter).first;

		if (testing <= id)
		{
			continue;
		}

		ULInt parent = testing;

		while (parent > id)
		{
			//ULInt pos = (testing - 1)%numChildren;
			//parent = (testing - 1 - pos)/numChildren;
			parent = (parent - 1)/numChildren;

			if (parent == id)
			{
				descendants.push_back(testing);
			}
		}
	}

	while (!descendants.empty())
	{
		this->position.erase(descendants.back());

		descendants.pop_back();
	}
}*/

void PMJMesh::AngleTreeCellClassifier::setPosition(const Data::BoxTreeCell *cell, enum Data::Position pos)
{
	Performer::AngleTreeCellClassifier::setPosition(cell, pos);

	PMJMesh::setPosition(this->dimension, const_cast<Data::BoxTreeCell *>(cell), pos);
}

bool PMJMesh::AngleTreeCellClassifier::ignore(ULInt /*id*/, Data::Position /*pos*/) const
{
	return false;
}
