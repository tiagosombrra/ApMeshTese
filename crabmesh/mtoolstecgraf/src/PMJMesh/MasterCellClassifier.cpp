#include "PMJMesh/MasterCellClassifier.h"

#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"

PMJMesh::MasterCellClassifier::MasterCellClassifier(UInt dimension,
	const Parallel::ThreadManager *tm, const Data::BoxTree *tree) :
		//PMJMesh::AngleTreeCellClassifier(dimension, tm, static_cast<Data::Front *>(NULL), tree)
		PMJMesh::AngleTreeCellClassifier(dimension, tm, NULL, tree)
{
	this->slavePosition = NULL;
}

PMJMesh::MasterCellClassifier::~MasterCellClassifier()
{

}

void PMJMesh::MasterCellClassifier::setSlavePosition(const Data::PositionMap *position)
{
	this->slavePosition = position;
}

void PMJMesh::MasterCellClassifier::execute()
{
	for (Data::PositionMap::const_iterator iter = this->slavePosition->begin();
		 iter != this->slavePosition->end(); iter++)
	{
		ULInt id = (*iter).first;
		enum Data::Position pos = (*iter).second;

		//Data::PositionMap::iterator piter = this->position.find(id);
		Data::PositionMap::iterator piter = this->position.insert(std::make_pair(id, Data::UNKNOWN)).first;

		/*std::cout << "cell " << id << " has position " <<
			((pos == Data::IN) ? "IN" : ((pos == Data::OUT) ? "OUT" : ((pos == Data::ON) ? "ON" : "UNKNOWN")))
			<< " in slave and position " <<
			(((*piter).second == Data::IN) ? "IN" : (((*piter).second == Data::OUT) ? "OUT" : (((*piter).second == Data::ON) ? "ON" : "UNKNOWN")))
			<< " originally, existed before = " << std::boolalpha << (piter != this->position.end()) << std::endl;*/

		//this->position[id] == Data::OUT -> the cell is outside the domain -> do nothing
		//pos == Data::IN -> flag to indicate the position did not change
		if (((*piter).second != Data::OUT) && (pos != Data::IN))
		{
			//if a cell was ON the boundary but now is not, it can only be
			// outside the domain, because it cannot be changed to inside the
			// domain, since the front (initially the boundary) only advances.
			if (((*piter).second == Data::ON) && (pos != Data::ON))
			{
				pos = Data::OUT;
			}

			/*std::cout << "will replace position of cell " << id << " from " <<
				(((*piter).second == Data::IN) ? "IN" : (((*piter).second == Data::OUT) ? "OUT" : (((*piter).second == Data::ON) ? "ON" : "UNKNOWN")))
				<< " to " <<
				((pos == Data::IN) ? "IN" : ((pos == Data::OUT) ? "OUT" : ((pos == Data::ON) ? "ON" : "UNKNOWN")))
				<< std::endl;*/

			Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(this->tree->node(id));

			if (c)
			{
				this->setChildrenPosition(c, pos);
				this->setParentPosition(c, pos);
			}
			else
			{
				this->position[id] = pos;
				this->setParentPosition(id, pos);
			}
		}
	}

	this->slavePosition = NULL;
}

void PMJMesh::MasterCellClassifier::adjustPosition()
{
	PMJMesh::AngleTreeCellClassifier::adjustPosition();

	this->insertMissingPositions();
}

void PMJMesh::MasterCellClassifier::setParentPosition(ULInt id, enum Data::Position pos)
{
	if (id == 0)
	{
		return;
	}

	UInt numChildren = (this->dimension == 2) ? 4 : 8;

	ULInt child = (id - 1)%numChildren;
	ULInt parent = (id - 1 - child)/numChildren;

	if (this->position[parent] == pos)
	{
		return;
	}

	bool setting = true;

	if (pos != Data::ON)
	{
		for (UInt i = 0; i < numChildren; i++)
		{
			child = numChildren*parent + i + 1;

			if (child == id)
			{
				continue;
			}

			if (this->position[child] != pos)
			{
				setting = false;

				break;
			}
		}
	}

	if (setting)
	{
		this->position[parent] = pos;

		this->setParentPosition(parent, pos);
	}
}

void PMJMesh::MasterCellClassifier::setParentPosition(Data::BoxTreeCell *c, enum Data::Position pos)
{
	Data::BoxTreeCell *parent = static_cast<Data::BoxTreeCell *>(c->parent());

	if (!parent)
	{
		return;
	}

	if (this->position[parent->getId()] == pos)
	{
		return;
	}

	bool setting = true;

	if (pos != Data::ON)
	{
		for (UInt i = 0; i < parent->numChildren(); i++)
		{
			if (parent->child(i) == c)
			{
				continue;
			}

			if (this->position[parent->child(i)->getId()] != pos)
			{
				setting = false;

				break;
			}
		}
	}

	if (setting)
	{
		//Performer::AngleTreeCellClassifier::setPosition(parent, pos);
		this->setPosition(parent, pos);

		this->setParentPosition(parent, pos);
	}
}

/*bool PMJMesh::MasterCellClassifier::adjustPosition(ULInt id, UInt numChildren, Data::Position pos, ULInt max)
{
	//this prevents 2:1 refined cells inside the domain to be derefined.
	if (pos == Data::IN)
	{
		return false;
	}

	return PMJMesh::AngleTreeCellClassifier::adjustPosition(id, numChildren, pos, max);
}*/

void PMJMesh::MasterCellClassifier::insertMissingPositions()
{
	//Data::PositionMap missing;
	Data::ULIntVector missing;

	UInt numChildren = (this->dimension == 2) ? 4 : 8;

	for (Data::PositionMap::const_iterator iter = ++this->position.begin();
		 iter != this->position.end(); iter++)
	{
		ULInt id = (*iter).first;

		ULInt child = (id - 1)%numChildren;
		ULInt parent = (id - 1 - child)/numChildren;

		ULInt first = parent*numChildren + 1;
		ULInt last = first + numChildren;

		for (child = first; child != last; child++)
		{
			if (child == id)
			{
				continue;
			}

			if (this->position.find(child) == this->position.end())
			{
				//missing[child] = Data::IN;
				missing.push_back(child);
			}
			else
			{
				iter++;
			}
		}
	}

	/*for (Data::PositionMap::iterator iter = missing.begin();
		 iter != missing.end(); iter++)*/
	for (Data::ULIntVector::iterator iter = missing.begin();
		 iter != missing.end(); iter++)
	{
		//this->position.insert(std::make_pair((*iter).first, (*iter).second));
		this->position.insert(std::make_pair((*iter), Data::IN));
	}
}

bool PMJMesh::MasterCellClassifier::ignore(ULInt /*id*/, Data::Position pos) const
{
	//this prevents 2:1 refined cells inside the domain to be derefined.
	return (pos == Data::IN);
}
