#include "PMJMesh/PosterioriLoadCalculator.h"

#include "Data/Box.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"

using namespace PMJMesh;

PMJMesh::PosterioriLoadCalculator::PosterioriLoadCalculator(const Data::BoxTree *tree,
	bool considerOn, bool heightDependent, UInt dimension, UInt minLevel,
	UInt numLayers, bool adjustInTree, bool adjustInBox) :
	PMJMesh::LoadCalculator::LoadCalculator(tree,
		considerOn, heightDependent, dimension, minLevel)
{
	this->setNumLayers(numLayers);
	this->setAdjustInTree(adjustInTree);
	this->setAdjustInBox(adjustInBox);

	/*if ((this->addingLoadForNodes) && (this->numLayers > 0))
	{
		this->addingLoadForNodes = false;
		this->setConsiderOn(considerOn);
	}*/
}

PMJMesh::PosterioriLoadCalculator::~PosterioriLoadCalculator()
{

}

void PMJMesh::PosterioriLoadCalculator::setNumLayers(UInt numLayers)
{
	this->numLayers = numLayers;
}

void PMJMesh::PosterioriLoadCalculator::setAdjustInTree(bool adjustInTree)
{
	this->adjustInTree = adjustInTree;
}

void PMJMesh::PosterioriLoadCalculator::setAdjustInBox(bool adjustInBox)
{
	this->adjustInBox = adjustInBox;
}

bool PMJMesh::PosterioriLoadCalculator::shouldRemoveFromLoad(const Data::BoxTreeCell *cell, const Data::BoxTreeCell *from) const
{
	enum Data::Position pos = PMJMesh::getPosition(this->dimension, cell);

	if (!from)
	{
		return ((pos != Data::OUT) &&
			(/*(this->considerOn) || */(pos != Data::ON)));
	}

	if (pos == Data::OUT)
	{
		return false;
	}

	if ((pos == Data::IN) || (pos == Data::UNKNOWN))
	{
		return true;
	}

	enum Data::Position fpos = PMJMesh::getPosition(this->dimension, from);

	return (fpos == Data::IN);
}

UInt PMJMesh::PosterioriLoadCalculator::layer(const Data::BoxTreeCell *cell, UInt layer, UInt side, ULIntUIntVectorMap &mapLayers) const
{
	//std::cout << "finding layer for cell " << cell->getId() << ", initial layer = " << layer
	//			<< ", side = " << side << ", mapLayers.size() = " << mapLayers.size() << std::endl;

	if (this->minLevel == 0)
	{
		//std::cout << "min level is " << this->minLevel << std::endl;

		return layer;
	}

	UInt level = cell->level();

	//std::cout << "cell level is " << level << std::endl;

	if (level >= this->minLevel)
	{
		//std::cout << "cell level is greater than or equal to min level" << std::endl;

		return layer;
	}

	//std::cout << "cell level is less than min level" << std::endl;

	Data::UIntVector layers(2*this->dimension, 0);

	ULIntUIntVectorMap::iterator ml = mapLayers.insert(std::make_pair(cell->getId(), layers)).first;

	//std::cout << "map of layers for cell " << cell->getId() << ":" << std::endl;
	//for (UInt i = 0; i < 2*this->dimension; i++) std::cout << "side " << i << ", numlayers = " << (*ml).second[i] << std::endl;

	UInt diff = this->minLevel - level;

	//std::cout << "difference of levels = " << diff << std::endl;

	UInt numLayersInCell = static_cast<UInt>(std::pow(2.0, diff));

	//std::cout << "num layers in cell = " << numLayersInCell << std::endl;

	UInt interestedLayers = std::min(numLayersInCell, this->numLayers - layer);

	//std::cout << "num layers in interest = " << interestedLayers << std::endl;

	(*ml).second[side] = interestedLayers;

	//std::cout << "new numlayers for side " << side << " = " << (*ml).second[side] << std::endl;

	layer += interestedLayers - 1;

	//std::cout << "found layer = " << layer << std::endl;

	return layer;
}

void PMJMesh::PosterioriLoadCalculator::findNeighboringCells(Data::GraphNodeList cells,
	UInt side, Data::GraphNodeSet descends[], ULIntUIntVectorMap &mapLayers, const Data::BoxTreeCell *cell)
{
	//std::cout << "finding neighboring cells for cell " << (cell ? cell->getId() : -1) << ", cells.size() = "
	//		  << cells.size() << ", side = " << side << "descends sizes = "
	//		  << descends[0].size() << ", " << descends[1].size() << ", "
	//		  << "mapLayers.size() = " << mapLayers.size() << std::endl;

	UInt numSides = 2*this->dimension;
	UInt opposite = (side + this->dimension)%numSides;

	//std::cout << "opposite = " << opposite << std::endl;

	for (UInt layer = 1; layer < this->numLayers; layer++)
	{
		//std::cout << "processing layer = " << layer << ", cells.size() = " << cells.size() << std::endl;

		Data::GraphNodeSet descendsLayer;

		for (Data::GraphNodeList::iterator iter = cells.begin();
			 iter != cells.end(); iter++)
		{
			Data::BoxTreeCell *descend = static_cast<Data::BoxTreeCell *>((*iter));

			//std::cout << "testing descend " << descend->getId() << std::endl;

			Data::GraphNodeList neighs = descend->neighbors(opposite);

			//std::cout << "descend has " << neighs.size() << " cells in opposite side" << std::endl;

			for (Data::GraphNodeList::iterator iter2 = neighs.begin();
				 iter2 != neighs.end(); iter2++)
			{
				Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>((*iter2));

				//std::cout << "testing neighbor " << n->getId() << ", position = " << (*this->position->find(n->getId())).second << std::endl;

				bool consider = cell ? cell->isDescendant(n->getId()) : this->inBox(n);

				//std::cout << "consider = " << std::boolalpha << consider << std::endl;

				if ((!consider) || (!this->shouldRemoveFromLoad(n, descend)))
				{
					//std::cout << "neighbor " << n->getId() << " should not be removed from load" << std::endl;

					continue;
				}

				UInt l = this->layer(n, layer, side, mapLayers);

				//std::cout << "will insert cell " << n->getId() << " in layer " << layer << std::endl;

				if (l == layer)
				{
					descendsLayer.insert(n);
				}
				else
				{
					descends[l].insert(n);
				}
			}
		}

		Data::GraphNodeList moreDescends;

		if ((layer < this->numLayers - 1) && (this->minLevel != 0))
		{
			//std::cout << "will find out more descends from " << descends[layer].size() << " descendents" << std::endl;

			for (Data::GraphNodeSet::iterator iter = descends[layer].begin();
				 iter != descends[layer].end(); iter++)
			{
				//std::cout << "will test " << (*iter)->getId() << std::endl;

				ULIntUIntVectorMap::iterator it = mapLayers.find((*iter)->getId());

				if (it == mapLayers.end())
				{
					//std::cout << "is not in mapLayers" << std::endl;

					continue;
				}

				if ((*it).second[side] == 0)
				{
					//std::cout << "has no layers in side " << side << std::endl;

					continue;
				}

				//std::cout << "has " << (*it).second[side] << " layers in side " << side << ", adding to moreDescends" << std::endl;

				moreDescends.push_back((*iter));
			}
		}

		descends[layer].insert(descendsLayer.begin(), descendsLayer.end());

		if (layer < this->numLayers - 1)
		{
			cells.assign(descendsLayer.begin(), descendsLayer.end());

			cells.splice(cells.end(), moreDescends);
		}
	}
}

Real PMJMesh::PosterioriLoadCalculator::sumLoads(Data::GraphNodeSet descends[], const ULIntUIntVectorMap &mapLayers) const
{
	//Data::GraphNodeSet allDescends;

	//allDescends.swap(descends[0]);

	for (UInt i = 1; i < this->numLayers; i++)
	{
		//allDescends.insert(descends[i].begin(), descends[i].end());
		descends[0].insert(descends[i].begin(), descends[i].end());
	}

	//std::cout << "allDescends.size() = " << allDescends.size() << std::endl;

	Real descendsLoad = 0.0;

	//for (Data::GraphNodeSet::iterator iter = allDescends.begin();
	for (Data::GraphNodeSet::iterator iter = descends[0].begin();
		 //iter != allDescends.end(); iter++)
		 iter != descends[0].end(); iter++)
	{
		Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>((*iter));

/*#if USE_GUI
		if (!this->adjustInTree)
		{
			cell->setColor(0.0, 0.5, 0.5);
			cell->highlight();
		}
#endif //#if USE_GUI*/

		//std::cout << "descend = " << cell->getId() << std::endl;

		Real load = (*this->treeLoad.find(cell->getId())).second;

		//std::cout << "initial load = " << load << std::endl;

		if (this->minLevel != 0)
		{
			//std::cout << "this->minLevel is not 0" << std::endl;

			ULIntUIntVectorMap::const_iterator it = mapLayers.find(cell->getId());

			if (it != mapLayers.end())
			{
				//std::cout << "descend is in mapLayers" << std::endl;

				UInt level = cell->level();

				//std::cout << "descend->level = " << level << std::endl;

				UInt diff = this->minLevel - level;

				//std::cout << "diff = " << diff << std::endl;

				UInt numLayersInCell = static_cast<UInt>(std::pow(2.0, diff));

				//std::cout << "number of layers in descend = " << numLayersInCell << std::endl;

				UInt numCells = 1;

				for (UInt i = 0; i < this->dimension; i++)
				{
					UInt numLayers = (*it).second[i] + (*it).second[i + this->dimension];

					//std::cout << "number of layers in side " << i << " = " << numLayers << std::endl;

					if (numLayers >= numLayersInCell)
					{
						//std::cout << "number of layers in side " << i << " is greater than num of layers in descend" << std::endl;

						numCells = 0;

						break;
					}

					numCells *= numLayersInCell - numLayers;

					//std::cout << "accumulated numCells = " << numCells << std::endl;
				}

				if (numCells > 0)
				{
					Real loadPerCell = load/std::pow(2.0, this->dimension*static_cast<Real>(diff)); //load per cell

					//std::cout << "load per cell = " << loadPerCell << std::endl;

					loadPerCell *= numCells;

					//std::cout << "load per cell x number of cells = " << loadPerCell << std::endl;

					load -= loadPerCell;
				}
			}
		}

		//std::cout << "final load = " << load << std::endl;

		descendsLoad += load;

		//std::cout << "accumulated load = " << descendsLoad << std::endl;
	}

	return descendsLoad;
}

void PMJMesh::PosterioriLoadCalculator::findDescendants(const Data::BoxTreeCell *cell,
	UInt side, Data::GraphNodeList &cells, Data::GraphNodeSet descends[], ULIntUIntVectorMap &mapLayers) const
{
	//std::cout << "cell " << cell->getId() << ", finding descendants for side " << side
	//		  << ", cells.size() = " << cells.size() << ", descends sizes = "
	//		  << descends[0].size() << ", " << descends[1].size() << ", "
	//		  << "mapLayers.size() = " << mapLayers.size()
	//		  << std::endl;

	cells = this->tree->children(cell, side);

	//std::cout << "children cells size = " << cells.size() << std::endl;

	for (Data::GraphNodeList::iterator iter = cells.begin();
		 iter != cells.end();)
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

		//std::cout << "child cell = " << c->getId() << ", pos = " << (*this->position->find(c->getId())).second << std::endl;

		if (!this->shouldRemoveFromLoad(c))
		{
			//std::cout << "should not be removed" << std::endl;

			iter = cells.erase(iter);

			continue;
		}

		UInt layer = this->layer(c, 0, side, mapLayers);

		//std::cout << "inserting cell " << c->getId() << " in layer " << layer << std::endl;

		descends[layer].insert(c);

		if (layer == 0)
		{
			iter++;
		}
		else
		{
			iter = cells.erase(iter);
		}
	}
}

void PMJMesh::PosterioriLoadCalculator::removeBoundingLoad(const Data::BoxTreeCell *cell)
{
	//std::cout << "cell = " << cell->getId() << ", pos = " << (*this->position->find(cell->getId())).second << ", initial load = " << this->treeLoad[cell->getId()] << std::endl;

	if (cell->isLeaf())
	{
		if ((cell->level() >= this->minLevel) ||
			(PMJMesh::getPosition(this->dimension, cell) != Data::IN))
		{
			//this->treeLoad[cell->getId()] = this->weight(this->loadFor(Data::OUT));
			this->treeLoad[cell->getId()] = this->loadFor(Data::OUT);
		}
		else
		{
			Real currLoad = this->treeLoad[cell->getId()];

			Real descendsLoad = 0.0;

			if (this->dimension == 2)
			{
				Real numDescsLayer = std::pow(2.0, this->minLevel - cell->level());

				if (this->dimension == 2)
				{
					descendsLoad = 4.0*static_cast<Real>(this->numLayers)*(numDescsLayer - static_cast<Real>(this->numLayers))*this->loadFor(Data::IN);
				}
				else
				{
					Real numDescsLayerEdge = numDescsLayer - 2.0*static_cast<Real>(this->numLayers);

					Real numDescsLayerFace = numDescsLayerEdge*numDescsLayerEdge;
					numDescsLayerFace *= 6.0*this->numLayers;

					numDescsLayerEdge *= 12.0*this->numLayers*this->numLayers;

					Real numDescsVert = 8.0*this->numLayers*this->numLayers*this->numLayers;

					descendsLoad = (numDescsVert + numDescsLayerEdge + numDescsLayerFace)*this->loadFor(Data::IN);
				}
			}

			Real diff = currLoad - descendsLoad;

			//std::cout << "load of cell - load of descendents = " << diff << std::endl;

			this->treeLoad[cell->getId()] = std::max(0.0, diff);
		}

		//std::cout << "cell " << cell->getId() << " is leaf, final load = " << this->treeLoad[cell->getId()] << std::endl;

		return;
	}

	if (cell->isRoot())
	{
		//std::cout << "cell " << cell->getId() << " is root, final load = " << this->treeLoad[cell->getId()] << std::endl;

		for (UInt i = 0; i < cell->numChildren(); i++)
		{
			this->removeBoundingLoad(static_cast<Data::BoxTreeCell *>(cell->child(i)));
		}

		return;
	}

	Real currLoad = this->treeLoad[cell->getId()];

	if (currLoad <= 0.0)
	{
		//std::cout << "cell " << cell->getId() << " has 0 load = " << currLoad << std::endl;

		return;
	}

	Data::GraphNodeSet descends[this->numLayers];

	ULIntUIntVectorMap mapLayers;

	UInt numSides = 2*this->dimension;

	for (UInt side = 0; side < numSides; side++)
	{
		Data::GraphNodeList ds;

		this->findDescendants(cell, side, ds, descends, mapLayers);

		this->findNeighboringCells(ds, side, descends, mapLayers, cell);
	}

	//std::cout << "will find out the amount of load that should be removed for cell " << cell->getId() << std::endl;

	Real descendsLoad = this->sumLoads(descends, mapLayers);

/*#if USE_GUI
	if (cell->getId() == 30)
	{
        for (Data::GraphNodeSet::iterator iter = descends[0].begin();
             iter != descends[0].end(); iter++)
        {
            Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>((*iter));

            cell->setColor(2.0, 2.0, 2.0);
        }
	}
#endif //#if USE_GUI*/

	//std::cout << "amount of load that should be removed for cell " << cell->getId() << " = " << descendsLoad << std::endl;

	Real diff = currLoad - descendsLoad;

	//std::cout << "load of cell - load of descendents = " << diff << std::endl;

	this->treeLoad[cell->getId()] = std::max(0.0, diff);

	/*//debug
	{
		std::cout << "Load for cell " << cell->getId() << " = " << std::max(0.0, diff) << std::endl;
	}
	//endebug*/

	//std::cout << "cell " << cell->getId() << " final load = " << this->treeLoad[cell->getId()] << std::endl;

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->removeBoundingLoad(static_cast<Data::BoxTreeCell *>(cell->child(i)));
	}
}

void PMJMesh::PosterioriLoadCalculator::adjustTreeLoad()
{
	this->removeBoundingLoad(static_cast<Data::BoxTreeCell *>(this->tree->getRoot()));
}

void PMJMesh::PosterioriLoadCalculator::makeTreeLoad()
{
	PMJMesh::LoadCalculator::makeTreeLoad();

	if ((this->adjustInTree) && (this->numLayers > 0))
	{
		this->adjustTreeLoad();

		//std::cout << "num layers = " << this->numLayers << std::endl;
		//std::cout << "min level  = " << this->minLevel << std::endl;
	}
}

void PMJMesh::PosterioriLoadCalculator::findOnCells(const Data::BoxTreeCell *cell, Data::GraphNodeList ds[],
	Data::GraphNodeSet descends[], ULIntUIntVectorMap &mapLayers, bool interTest)
{
	//if (!this->box->intercept(cell->box()))
	if ((interTest) && (!this->box->optIntercept(cell->box())))
	{
		return;
	}

	if ((cell->isLeaf()) && (!this->shouldRemoveFromLoad(cell)))
	{
		return;
	}

	UInt numSides = 2*this->dimension;
	bool include[numSides];
	bool includeAny = false;

	for (UInt side = 0; side < numSides; side++)
	{
		UInt coord = side%this->dimension;
		bool max = (side < this->dimension);

		include[side] = false;

		if (max)
		{
			if ((cell->box()->getMin()->getCoord(coord) > this->box->getMin()->getCoord(coord) + Data::getTolerance()) &&
				(std::fabs(cell->box()->getMax()->getCoord(coord) - this->box->getMax()->getCoord(coord)) <= Data::getTolerance()))
			{
				include[side] = true;
			}
		}
		else
		{
			if ((cell->box()->getMax()->getCoord(coord) < this->box->getMax()->getCoord(coord) - Data::getTolerance()) &&
				(std::fabs(cell->box()->getMin()->getCoord(coord) - this->box->getMin()->getCoord(coord)) <= Data::getTolerance()))
			{
				include[side] = true;
			}
		}

		if (include[side])
		{
			includeAny = true;
		}
	}

	if (includeAny)
	{
		if ((!interTest) || (this->inBox(cell)))
		{
			if (cell->isLeaf())
			{
				for (UInt side = 0; side < numSides; side++)
				{
					if (include[side])
					{
						UInt layer = this->layer(cell, 0, side, mapLayers);

						if (layer == 0)
						{
							ds[side].push_back(const_cast<Data::BoxTreeCell *>(cell));
						}
						else
						{
							descends[layer].insert(const_cast<Data::BoxTreeCell *>(cell));
						}
					}
				}

				return;
			}

			for (UInt i = 0; i < cell->numChildren(); i++)
			{
				this->findOnCells(static_cast<Data::BoxTreeCell *>(cell->child(i)), ds, descends, mapLayers, false);
			}

			return;
		}
	}

	if (cell->isLeaf())
	{
		//these are the leaf cells that are not in box though they do intercept it

		return;
	}

	for (UInt i = 0; i < cell->numChildren(); i++)
	{
		this->findOnCells(static_cast<Data::BoxTreeCell *>(cell->child(i)), ds, descends, mapLayers, interTest);
	}
}

void PMJMesh::PosterioriLoadCalculator::adjustBoxLoad()
{
	Data::BoxTreeCell *root = static_cast<Data::BoxTreeCell *>(this->tree->getRoot());

	UInt numSides = 2*this->dimension;

	Data::GraphNodeList ds[numSides];

	Data::GraphNodeSet descends[this->numLayers];

	ULIntUIntVectorMap mapLayers;

	this->findOnCells(root, ds, descends, mapLayers);

	for (UInt side = 0; side < numSides; side++)
	{
		descends[0].insert(ds[side].begin(), ds[side].end());

		this->findNeighboringCells(ds[side], side, descends, mapLayers);
	}

	/*//debug
	static UInt debug = 0;
	debug++;
	if (debug == 4)
	{
		for (UInt i = 0; i < this->numLayers; i++)
		{
			for (Data::GraphNodeSet::iterator iter = descends[i].begin();
				 iter != descends[i].end(); iter++)
			{
				static_cast<Data::BoxTreeCell *>((*iter))->setColor(0.0, 0.5, 0.5);
				static_cast<Data::BoxTreeCell *>((*iter))->highlight();
			}
		}
	}
	//endebug*/

	Real descendsLoad = this->sumLoads(descends, mapLayers);

	this->load -= descendsLoad;

	/*//debub
	if (debug == 4)
	{
		std::cout << "Load for given box = " << this->load << std::endl;
	}
	//endebug*/
}

void PMJMesh::PosterioriLoadCalculator::makeBoxLoad()
{
	PMJMesh::LoadCalculator::makeBoxLoad();

	if ((this->adjustInBox) && (this->numLayers > 0))
	{
		this->adjustBoxLoad();
	}
}

/*bool PMJMesh::PosterioriLoadCalculator::inBox(const Data::BoxTreeCell *cell)
{
	bool in = PMJMesh::LoadCalculator::inBox(cell);

	if ((!this->adjustInBox) || (!in) || (this->numLayers == 0))
	{
		return in;
	}

	if (this->box->on(cell->box()))
	{
		return false;
	}

	if (this->numLayers == 1)
	{
		return true;
	}

	UInt numSides = cell->numSides();

	Data::GraphNodeList neighs[numSides];

	for (UInt i = 0; i < numSides; i++)
	{
		neighs[i].push_back(const_cast<Data::BoxTreeCell *>(cell));
	}

	for (UInt layer = 1; layer < this->numLayers; layer++)
	{
		for (UInt side = 0; side < numSides; side++)
		{
			Data::GraphNodeList ns = neighs[side];

			neighs[side].clear();

			for (Data::GraphNodeList::iterator iter = ns.begin();
				 iter != ns.end(); iter++)
			{
				Data::BoxTreeCell *n = static_cast<Data::BoxTreeCell *>((*iter));

				Data::GraphNodeList nneighs = n->neighbors(side);

				for (Data::GraphNodeList::iterator iter2 = nneighs.begin();
					 iter2 != nneighs.end(); iter2++)
				{
					Data::BoxTreeCell *nn = static_cast<Data::BoxTreeCell *>((*iter2));

					if (this->box->on(nn->box()))
					{
						return false;
					}
				}

				neighs[side].splice(neighs[side].end(), nneighs);
			}
		}
	}

	return true;
}*/
