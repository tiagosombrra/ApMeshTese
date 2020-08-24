#include "PMJMesh/BSPTreeShifter.h"

#include "Data/Point.h"
#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"

PMJMesh::BSPTreeShifter::BSPTreeShifter(UInt dimension, Data::BoxTree *tree,
	enum ShiftType type, Real factor, UInt shift, bool skippingCenter) :
		PMJMesh::BoxTreeShifter::BoxTreeShifter(dimension, tree, type, factor, shift, skippingCenter)
{
	this->adjustSmallerCells = true;
}

PMJMesh::BSPTreeShifter::~BSPTreeShifter()
{

}

void PMJMesh::BSPTreeShifter::displace(const Data::BoxTreeCell *c, const Data::Point *min, const Data::Point *max,
	const Data::GraphNodeList &moreNeighs, Data::Point *newMin, Data::Point *newMax) const
{
	for (UInt i = 0; i < this->numDirs; i++)
	{
		Real size = this->factor*(max->getCoord(this->coord[i]) - min->getCoord(this->coord[i]));

		Real disp = size;

		Data::GraphNodeList neighs = static_cast<Data::BoxTree *>(this->tree)->neighbors(c, this->side[i]);

		neighs.insert(neighs.end(), moreNeighs.begin(), moreNeighs.end());

		if ((this->adjustSmallerCells) && (!neighs.empty()))
		{
			disp = REAL_MAX;
		}

		for (Data::GraphNodeList::const_iterator iter = neighs.begin();
			 iter != neighs.end(); iter++)
		{
			Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>((*iter));

			Data::Point *nmin, *nmax;

#if USE_C__11
			Data::PointHashMap::const_iterator it = this->originalMinPoints.find(n->getId());
#else
			Data::PointMap::const_iterator it = this->originalMinPoints.find(n->getId());
#endif //#if USE_C__11

			if (it == this->originalMinPoints.end())
			{
				nmin = n->box()->getMin();
				nmax = n->box()->getMax();
			}
			else
			{
				nmin = (*it).second;
				nmax = (*this->originalMaxPoints.find(n->getId())).second;
			}

			Real nsize = this->factor*(nmax->getCoord(this->coord[i]) - nmin->getCoord(this->coord[i]));

			if (nsize < disp)
			{
				disp = nsize;
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
