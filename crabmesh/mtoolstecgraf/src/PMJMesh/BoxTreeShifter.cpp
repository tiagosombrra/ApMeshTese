#include "PMJMesh/BoxTreeShifter.h"

#include "MJMesh/QuadTree.h"
#include "MJMesh/QuadTreeCell.h"
#include "MJMesh/OctTree.h"
#include "MJMesh/OctTreeCell.h"
#include "Data/Box.h"

using namespace PMJMesh;

PMJMesh::BoxTreeShifter::BoxTreeShifter(UInt dimension, Data::BoxTree *tree,
	enum ShiftType type, Real factor, UInt shift, bool skippingCenter) :
	Performer::TreeRefiner(tree)
{
	this->dimension = dimension;

	this->setRestore(false);

	this->setFactor(factor);
	this->setSkippingCenter(skippingCenter);

	this->lateral = true;
	this->diagonal = false;
	this->diagonalVertex = false;

	this->setType(type);

	this->setInitialShift(shift);

	this->odd = false;

	this->numDirs = 0;
	this->coord = NULL;
	this->positive = NULL;
	this->side = NULL;
	//this->opposite = NULL;

	this->adjustSmallerCells = false;
}

PMJMesh::BoxTreeShifter::~BoxTreeShifter()
{
	this->clearOriginalPoints();
	this->clearArrays();
}

void PMJMesh::BoxTreeShifter::setType(enum ShiftType type)
{
	switch (type)
	{
	case NONE:
		this->setLateral(false);
		this->setDiagonal(false);
		this->setDiagonalVertex(false);
		break;

	case LATERAL:
		this->setLateral(true);
		this->setDiagonal(false);
		this->setDiagonalVertex(false);
		break;

	case DIAGONAL:
		this->setLateral(false);
		this->setDiagonal(true);
		this->setDiagonalVertex(false);
		break;

	case LATERAL_DIAGONAL:
		this->setLateral(true);
		this->setDiagonal(true);
		this->setDiagonalVertex(false);
		break;

	case DIAGONAL_VERTEX:
		this->setLateral(false);
		this->setDiagonal(false);
		this->setDiagonalVertex(true);
		break;

	case LATERAL_DIAGONAL_VERTEX:
		this->setLateral(true);
		this->setDiagonal(false);
		this->setDiagonalVertex(true);
		break;

	case DIAGONAL_DIAGONAL_VERTEX:
		this->setLateral(false);
		this->setDiagonal(true);
		this->setDiagonalVertex(true);
		break;

	case LATERAL_DIAGONAL_DIAGONAL_VERTEX:
		this->setLateral(true);
		this->setDiagonal(true);
		this->setDiagonalVertex(true);
		break;
	}
}

enum ShiftType PMJMesh::BoxTreeShifter::getType() const
{
	enum ShiftType type = NONE;

	if (this->isLateral())
	{
		if (this->isDiagonal())
		{
			if (this->isDiagonalVertex())
			{
				type = LATERAL_DIAGONAL_DIAGONAL_VERTEX;
			}
			else
			{
				type = LATERAL_DIAGONAL;
			}
		}
		else if (this->isDiagonalVertex())
		{
			type = LATERAL_DIAGONAL_VERTEX;
		}
		else
		{
			type = LATERAL;
		}
	}
	else if (this->isDiagonal())
	{
		if (this->isDiagonalVertex())
		{
			type = DIAGONAL_DIAGONAL_VERTEX;
		}
		else
		{
			type = DIAGONAL;
		}
	}
	else if (this->isDiagonalVertex())
	{
		type = DIAGONAL_VERTEX;
	}

	return type;
}

void PMJMesh::BoxTreeShifter::setRestore(bool restore)
{
	this->restore = restore;
}

void PMJMesh::BoxTreeShifter::setFactor(Real factor)
{
	this->factor = factor;
}

void PMJMesh::BoxTreeShifter::setSkippingCenter(bool skippingCenter)
{
	this->skippingCenter = skippingCenter;
}

bool PMJMesh::BoxTreeShifter::isSkippingCenter() const
{
	return this->skippingCenter;
}

void PMJMesh::BoxTreeShifter::setLateral(bool lateral)
{
	this->lateral = lateral;
}

bool PMJMesh::BoxTreeShifter::isLateral() const
{
	/*return ((this->lateral) ||
			 ((!this->isDiagonal()) && (!this->isDiagonalVertex())));*/
	return this->lateral;
}

void PMJMesh::BoxTreeShifter::setDiagonal(bool diagonal)
{
	this->diagonal = diagonal;
}

bool PMJMesh::BoxTreeShifter::isDiagonal() const
{
	return this->diagonal;
}

void PMJMesh::BoxTreeShifter::setDiagonalVertex(bool diagonalVertex)
{
	this->diagonalVertex = diagonalVertex;
}

bool PMJMesh::BoxTreeShifter::isDiagonalVertex() const
{
	return (this->dimension > 2) ? this->diagonalVertex : false;
}

UInt PMJMesh::BoxTreeShifter::getShift() const
{
	return this->shift;
}

UInt PMJMesh::BoxTreeShifter::state() const
{
	UInt shift = this->getShift();

	return ((shift%2) == 0) ? 0 : shift/2 + 1;
}

UInt PMJMesh::BoxTreeShifter::oppositeState() const
{
	if ((this->shift%2) == 0)
	{
		return 0;
	}

	if (this->shift < this->maxShift(0))
	{
		return (this->getShift()/2 + this->dimension)%(2*this->dimension) + 1;
	}

	if (this->shift < this->maxShift(1))
	{
		UInt twoDim = 2*this->dimension;
		UInt dimDimMinus1 = this->dimension*(this->dimension - 1);

		return (this->getShift()/2 - twoDim + dimDimMinus1)%(2*dimDimMinus1) + twoDim + 1;
	}

	UInt twoDim = 2*this->dimension;
	UInt twoDimDimMinus1 = 2*this->dimension*(this->dimension - 1);
	UInt twoPowDimMinus1 = static_cast<UInt>(std::pow(2, this->dimension - 1));

	return (this->getShift()/2 - twoDim - twoDimDimMinus1 + twoPowDimMinus1)%(2*twoPowDimMinus1) + twoDim + twoDimDimMinus1 + 1;
}

UInt PMJMesh::BoxTreeShifter::numStates() const
{
	return this->numShiftedStates() + 1;
}

UInt PMJMesh::BoxTreeShifter::maxStates() const
{
	return this->maxShift(2)/2 + 1;
}

UInt PMJMesh::BoxTreeShifter::minState(UInt shiftType) const
{
	return this->minShift(shiftType)/2 + 1;
}

UInt PMJMesh::BoxTreeShifter::maxState(UInt shiftType) const
{
	return this->maxShift(shiftType)/2 + 1;
}

UInt PMJMesh::BoxTreeShifter::numShiftedStates() const
{
	UInt count = 0;

	if (this->isLateral())
	{
		count += 2*this->dimension;
	}

	if (this->isDiagonal())
	{
		count += 2*this->dimension*(this->dimension - 1);
	}

	if (this->isDiagonalVertex())
	{
		count += static_cast<UInt>(std::pow(2, this->dimension));
	}

	//return (count > 0) ? count : 2*this->dimension;
	return count;
}

void PMJMesh::BoxTreeShifter::setInitialShift(UInt shift)
{
	//this->shift = 2*(shift%(2*this->dimension));

	if ((this->dimension < 2) || (this->dimension > 3))
	{
		this->shift = 0;

		return;
	}

	if ((!this->isLateral()) && (!this->isDiagonal()) && (!this->isDiagonalVertex()))
	{
		this->shift = 0;

		return;
	}

	UInt s = 0;

	UInt m = this->numShiftedStates();

	if (this->isDiagonalVertex())
	{
		if ((!this->isLateral()) && (!this->isDiagonal()))
		{
			s = this->minShift(2);
		}
		else if ((this->isLateral()) && (!this->isDiagonal()))
		{
			shift = shift%m;

			if (shift > 5)
			{
				s = this->minShift(2) - this->minShift(1);
			}
		}
		else if ((!this->isLateral()) && (this->isDiagonal()))
		{
			s = this->minShift(1);
		}
	}
	else if (!this->isLateral())
	{
		s = this->minShift(1);
	}

	this->shift = s + 2*(shift%m);
}

Data::UIntVector PMJMesh::BoxTreeShifter::sides(UInt state) const
{
	if (state == 0)
	{
		return Data::UIntVector();
	}

	UInt shifts[3];

	UInt used = this->sides(2*state - 1, shifts);

	Data::UIntVector sides(used);

	for (UInt i = 0; i < used; i++)
	{
		sides[i] = shifts[i]/2;
	}

	return sides;
}

void PMJMesh::BoxTreeShifter::displace(const Data::BoxTreeCell *c, const Data::Point *min, const Data::Point *max,
	const Data::GraphNodeList &moreNeighs, Data::Point *newMin, Data::Point *newMax) const
{
	for (UInt i = 0; i < this->numDirs; i++)
	{
		Real size = this->factor*(max->getCoord(this->coord[i]) - min->getCoord(this->coord[i]));

		//static const bool refinement21 = false;
		Real disp = size;

		/*if (refinement21)
		{
			Data::GraphNodeList neighs = static_cast<Data::BoxTree *>(this->tree)->neighbors(c, this->side[i]);

			if (neighs.size() > 1)
			{
				disp = 0.5*size;
			}
			else if ((this->adjustSmallerCells) &&
					 (neighs.size() == 1) &&
					 (static_cast<Data::TreeNode *>(neighs.front())->level() < c->level()))
			{
				disp = 2.0*size;
			}
		}
		else*/
		{
			Data::GraphNodeList neighs = static_cast<Data::BoxTree *>(this->tree)->neighbors(c, this->side[i]);

			neighs.insert(neighs.end(), moreNeighs.begin(), moreNeighs.end());

			if (neighs.size() > 1)
			{
				UInt maxLevel = 0;

				for (Data::GraphNodeList::iterator iter = neighs.begin();
					 iter != neighs.end(); iter++)
				{
					UInt level = static_cast<Data::TreeNode *>((*iter))->level();

					if (level > maxLevel)
					{
						maxLevel = level;
					}
				}

				UInt level = c->level();

				if (maxLevel > level)
				{
					UInt diff = maxLevel - level;

					disp = size/std::pow(2.0, diff);
				}
			}
			else if ((this->adjustSmallerCells) && (neighs.size() == 1))
			{
				UInt minLevel = static_cast<Data::TreeNode *>(neighs.front())->level();

				UInt level = c->level();

				if (minLevel < level)
				{
					UInt diff = level - minLevel;

					disp = size*std::pow(2.0, diff);
				}
			}
		}

		//if the shifting direction points to the outside of the domain, then
		// it does not matter how it shifts, if distorted or not.
		//compensa?
		/*UIntVector opposite(this->opposite, this->opposite + this->numDirs);

		bool empty = static_cast<Data::BoxTree *>(this->tree)->neighbors(c, opposite).empty();

		if ((empty) && (this->numDirs > 1))
		{
			for (UInt i = 0; i < this->numDirs; i++)
			{
				empty = static_cast<Data::BoxTree *>(this->tree)->neighbors(c, this->opposite[i]).empty();

				if (!empty)
				{
					break;
				}
			}
		}

		if (empty)
		{
			//com esse if, se nao tiver vizinhos do lado oposto, esse lado
			// permanece na mesma posicao.
			size = 0.0;
		}*/

		if (!this->positive[i])
		{
			Real tmp = size;
			size = -disp;
			disp = -tmp;
		}

		newMin->setCoord(this->coord[i], newMin->getCoord(this->coord[i]) + size);
		newMax->setCoord(this->coord[i], newMax->getCoord(this->coord[i]) + disp);
	}
}

void PMJMesh::BoxTreeShifter::refine(Data::TreeNode *n)
{
	Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(n);

	if (this->odd)
	{
		Data::Point *min, *max;
		this->originalMinPoints[c->getId()] = min = c->box()->getMin();
		this->originalMaxPoints[c->getId()] = max = c->box()->getMax();

		Data::Point *newMin = this->makePoint(min);
		Data::Point *newMax = this->makePoint(max);

		Data::GraphNodeList moreNeighs;
		//Data::GraphNodeList moreOppNeighs;

		//if (this->numDirs != 0)
		if (this->numDirs != 1)
		{
			Data::UIntVector sides(this->side, this->side + this->numDirs);
			//Data::UIntVector opposites(this->opposite, this->opposite + this->numDirs);

			moreNeighs = static_cast<Data::BoxTree *>(this->tree)->neighbors(c, sides);
			//moreOppNeighs = static_cast<Data::BoxTree *>(this->tree)->neighbors(c, opposites);
		}

		this->displace(c, min, max, moreNeighs, newMin, newMax);

		c->box()->setBox(newMin, newMax);
	}
	else
	{
#if USE_C__11
		Data::PointHashMap::iterator min = this->originalMinPoints.find(c->getId());
		Data::PointHashMap::iterator max = this->originalMaxPoints.find(c->getId());
#else
		Data::PointMap::iterator min = this->originalMinPoints.find(c->getId());
		Data::PointMap::iterator max = this->originalMaxPoints.find(c->getId());
#endif //#if USE_C__11

		Data::Point *oldMin = c->box()->getMin();
		Data::Point *oldMax = c->box()->getMax();

		c->box()->setBox((*min).second, (*max).second);

		delete oldMin;
		delete oldMax;

		this->originalMinPoints.erase(min);
		this->originalMaxPoints.erase(max);
	}
}

void PMJMesh::BoxTreeShifter::execute()
{
	if ((this->restore) && (this->shift%2 == 0))
	{
		return;
	}

	this->shift++;

	if (this->shift >= this->maxShift())
	{
		this->shift = 0;
	}

	//std::cout << "shift = " << this->shift << std::endl;

	this->odd = ((this->shift%2) == 1);

	if ((this->shift < this->maxShift(0)) && (this->isLateral()))
	{
		//std::cout << "executing lateral" << std::endl;

		this->executeLateral();
	}
	else if ((this->shift < this->maxShift(1)) && (this->isDiagonal()))
	{
		//std::cout << "executing diagonal" << std::endl;

		this->executeDiagonal();
	}
	else if ((this->shift < this->maxShift(2)) && (this->isDiagonalVertex()))
	{
		//std::cout << "executing diagonalVertex" << std::endl;

		this->executeDiagonalVertex();
	}

	if ((this->isSkippingCenter()) && (!this->odd) && (this->shift != 0))
	{
		this->execute();
	}
}

UInt PMJMesh::BoxTreeShifter::sides(UInt shift, UInt shifts[]) const
{
	if (this->dimension == 2)
	{
		switch (shift)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			shifts[0] = shift;
			return 1;
		case 8:
		case 9:
			shifts[0] = 1;
			shifts[1] = 3;
			return 2;
		case 10:
		case 11:
			shifts[0] = 5;
			shifts[1] = 3;
			return 2;
		case 12:
		case 13:
			shifts[0] = 5;
			shifts[1] = 7;
			return 2;
		case 14:
		case 15:
			shifts[0] = 1;
			shifts[1] = 7;
			return 2;
		}
	}
	else if (this->dimension == 3)
	{
		switch (shift)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			shifts[0] = shift;
			return 1;
		case 12:
		case 13:
			shifts[0] = 1;
			shifts[1] = 3;
			return 2;
		case 14:
		case 15:
			shifts[0] = 3;
			shifts[1] = 5;
			return 2;
		case 16:
		case 17:
			shifts[0] = 5;
			shifts[1] = 1;
			return 2;
		case 18:
		case 19:
			shifts[0] = 7;
			shifts[1] = 3;
			return 2;
		case 20:
		case 21:
			shifts[0] = 9;
			shifts[1] = 5;
			return 2;
		case 22:
		case 23:
			shifts[0] = 11;
			shifts[1] = 1;
			return 2;
		case 24:
		case 25:
			shifts[0] = 7;
			shifts[1] = 9;
			return 2;
		case 26:
		case 27:
			shifts[0] = 9;
			shifts[1] = 11;
			return 2;
		case 28:
		case 29:
			shifts[0] = 11;
			shifts[1] = 7;
			return 2;
		case 30:
		case 31:
			shifts[0] = 1;
			shifts[1] = 9;
			return 2;
		case 32:
		case 33:
			shifts[0] = 3;
			shifts[1] = 11;
			return 2;
		case 34:
		case 35:
			shifts[0] = 5;
			shifts[1] = 7;
			return 2;
		case 36:
		case 37:
			shifts[0] = 1;
			shifts[1] = 3;
			shifts[2] = 5;
			return 3;
		case 38:
		case 39:
			shifts[0] = 7;
			shifts[1] = 3;
			shifts[2] = 5;
			return 3;
		case 40:
		case 41:
			shifts[0] = 7;
			shifts[1] = 9;
			shifts[2] = 5;
			return 3;
		case 42:
		case 43:
			shifts[0] = 1;
			shifts[1] = 9;
			shifts[2] = 5;
			return 3;
		case 44:
		case 45:
			shifts[0] = 7;
			shifts[1] = 9;
			shifts[2] = 11;
			return 3;
		case 46:
		case 47:
			shifts[0] = 1;
			shifts[1] = 9;
			shifts[2] = 11;
			return 3;
		case 48:
		case 49:
			shifts[0] = 1;
			shifts[1] = 3;
			shifts[2] = 11;
			return 3;
		case 50:
		case 51:
			shifts[0] = 7;
			shifts[1] = 3;
			shifts[2] = 11;
			return 3;
		}
	}

	return 0;
}

UInt PMJMesh::BoxTreeShifter::minShift(UInt shiftType) const
{
	if (shiftType == 0)
	{
		return 0;
	}

	return this->maxShift(shiftType - 1);
}

UInt PMJMesh::BoxTreeShifter::maxShift(UInt shiftType) const
{
	if (shiftType == 0)
	{
		return 4*this->dimension;
	}

	if ((shiftType == 1) || (this->dimension == 2))
	{
		return 4*this->dimension*this->dimension;
	}

	return 52;
}

UInt PMJMesh::BoxTreeShifter::maxShift() const
{
	if ((!this->lateral) && (!this->diagonal) && (!this->diagonalVertex))
	{
		return 0;
	}

	if ((!this->diagonal) &&
		((this->dimension != 3) || (!this->diagonalVertex)))
	{
		return this->maxShift(0);
	}

	if ((this->dimension != 3) || (!this->diagonalVertex))
	{
		return this->maxShift(1);
	}

	return this->maxShift(2);
}

void PMJMesh::BoxTreeShifter::fillArrays(UInt i, UInt shift)
{
	if (shift == 0)
	{
		this->coord[i] = this->dimension - 1;
	}
	else
	{
		this->coord[i] = ((shift - 1)/2)%this->dimension;
	}

	this->positive[i] = (shift <= 2*this->dimension);
	this->side[i] = shift/2;
	//this->opposite[i] = (this->side[i] + this->dimension)%(2*this->dimension);
}

void PMJMesh::BoxTreeShifter::executeForLeaves()
{
	Data::GraphNodeList leaves = this->tree->getLeaves();

#if USE_C_11
	if (this->odd)
	{
		this->originalMinPoints.reserve(leaves.size());
		this->originalMaxPoints.reserve(leaves.size());
	}
#endif //#if USE_C_11

	for (Data::GraphNodeList::iterator iter = leaves.begin();
		 iter != leaves.end(); iter++)
	{
		this->refine(static_cast<Data::TreeNode *>((*iter)));
	}

	if (!this->odd)
	{
		this->clearOriginalPoints();
	}
}

void PMJMesh::BoxTreeShifter::executeLateral()
{
	if (this->numDirs != 1)
	{
		this->clearArrays();

		this->makeArrays(1);
	}

	this->fillArrays(0, this->shift);

	this->executeForLeaves();
}

void PMJMesh::BoxTreeShifter::executeDiagonal()
{
	if ((this->shift != 0) && (this->shift < this->minShift(1)))
	{
		this->shift = this->minShift(1);

		if (this->odd)
		{
			this->shift++;
		}
	}

	if (this->numDirs != 2)
	{
		this->clearArrays();

		this->makeArrays(2);
	}

	UInt shifts[2] = {0, 0};

	this->sides(this->shift, shifts);

	this->fillArrays(0, shifts[0]);
	this->fillArrays(1, shifts[1]);

	this->executeForLeaves();
}

void PMJMesh::BoxTreeShifter::executeDiagonalVertex()
{
	if ((this->shift != 0) && (this->shift < this->minShift(2)))
	{
		this->shift = this->minShift(2);

		if (this->odd)
		{
			this->shift++;
		}
	}

	if (this->numDirs != 3)
	{
		this->clearArrays();

		this->makeArrays(3);
	}

	UInt shifts[3] = {0, 0, 0};

	this->sides(this->shift, shifts);

	this->fillArrays(0, shifts[0]);
	this->fillArrays(1, shifts[1]);
	this->fillArrays(2, shifts[2]);

	this->executeForLeaves();
}

Data::Point *PMJMesh::BoxTreeShifter::makePoint(const Data::Point *p) const
{
	return (this->dimension == 2) ? static_cast<Data::Point *>(new Data::Point2D(p)) :
#if USE_EXPERIMENTAL_3
			static_cast<Data::Point *>(new Data::Point3D(p));
#else
			((this->dimension == 3) ? static_cast<Data::Point *>(new Data::Point3D(p)) : new Data::Point(p));
#endif //#if USE_EXPERIMENTAL_3
}

void PMJMesh::BoxTreeShifter::clearOriginalPoints()
{
#if USE_C__11
	for (Data::PointHashMap::iterator iter = this->originalMinPoints.begin();
#else
	for (Data::PointMap::iterator iter = this->originalMinPoints.begin();
#endif //#if USE_C__11
		 iter != this->originalMinPoints.end(); iter++)
	{
		delete (*iter).second;
	}

#if USE_C__11
	for (Data::PointHashMap::iterator iter = this->originalMaxPoints.begin();
#else
	for (Data::PointMap::iterator iter = this->originalMaxPoints.begin();
#endif //#if USE_C__11
		 iter != this->originalMaxPoints.end(); iter++)
	{
		delete (*iter).second;
	}

	this->originalMinPoints.clear();
	this->originalMaxPoints.clear();
}

void PMJMesh::BoxTreeShifter::makeArrays(UInt numDirs)
{
	this->numDirs = numDirs;

	this->coord = new UInt[this->numDirs];
	this->positive = new bool[this->numDirs];
	this->side = new UInt[this->numDirs];
	//this->opposite = new UInt[this->numDirs];
}

void PMJMesh::BoxTreeShifter::clearArrays()
{
	if (this->coord)
	{
		delete [] this->coord;

		this->coord = NULL;
	}

	if (this->positive)
	{
		delete [] this->positive;

		this->positive = NULL;
	}

	if (this->side)
	{
		delete [] this->side;

		this->side = NULL;
	}

	/*if (this->opposite)
	{
		delete [] this->opposite;

		this->opposite = NULL;
	}*/

	this->numDirs = 0;
}

void PMJMesh::BoxTreeShifter::subdivide(Data::TreeNode */*n*/)
{

}
