#include "PMJMesh/QuadTreeCell.h"

#include "Data/Tree.h"

#if USE_GUI
#include "PMJMesh/AngleTreeCellClassifier.h"
#endif //#if USE_GUI

PMJMesh::QuadTreeCell::QuadTreeCell(PMJMesh::QuadTreeCell *parent, ULInt id) :
	MJMesh::QuadTreeCell(parent, id)
{
	this->setPosition(
		((parent) && ((parent->getPosition() == Data::IN) || (parent->getPosition() == Data::OUT))) ?
			parent->getPosition() : Data::UNKNOWN);
}

PMJMesh::QuadTreeCell::~QuadTreeCell()
{

}

void PMJMesh::QuadTreeCell::setPosition(enum Data::Position position)
{
	this->position = position;

#if USE_GUI
	if ((!this->parent()) && (!this->getTree()))
	{
		return;
	}

	PMJMesh::AngleTreeCellClassifier::setColor(this, this->position);
#endif //#if USE_GUI
}

enum Data::Position PMJMesh::QuadTreeCell::getPosition() const
{
	return this->position;
}

void PMJMesh::QuadTreeCell::adjustPositionFromChildren()
{
	if (this->isLeaf())
	{
		return;
	}

	enum Data::Position pos = static_cast<PMJMesh::QuadTreeCell *>(this->child(0))->getPosition();

	if (pos == Data::UNKNOWN)
	{
		this->setPosition(Data::UNKNOWN);

		return;
	}

	bool on = (pos == Data::ON);

	bool allequal = true;

	for (UInt i = 1; i < this->numChildren(); i++)
	{
		enum Data::Position cpos = static_cast<PMJMesh::QuadTreeCell *>(this->child(i))->getPosition();

		if (cpos == Data::UNKNOWN)
		{
			this->setPosition(Data::UNKNOWN);

			return;
		}

		if (cpos == Data::ON)
		{
			on = true;
		}

		if (cpos != pos)
		{
			allequal = false;
		}
	}

	if (on)
	{
		this->setPosition(Data::ON);

		return;
	}

	if (allequal)
	{
		this->setPosition(pos);

		return;
	}

	this->setPosition(Data::UNKNOWN);
}

void PMJMesh::QuadTreeCell::adjustPositionFromChildren(bool down)
{
	if (down)
	{
		for (UInt i = 0; i < this->numChildren(); i++)
		{
			static_cast<PMJMesh::QuadTreeCell *>(this->child(i))->adjustPositionFromChildren(true);
		}
	}

	this->adjustPositionFromChildren();

	if (!down)
	{
		if (this->parent())
		{
			static_cast<PMJMesh::QuadTreeCell *>(this->parent())->adjustPositionFromChildren(false);
		}
	}
}

void PMJMesh::QuadTreeCell::subdivide()
{
	ULInt id = 4*this->getId();

	//bottom left
	new PMJMesh::QuadTreeCell(this, ++id);
	//bottom right
	new PMJMesh::QuadTreeCell(this, ++id);
	//top right
	new PMJMesh::QuadTreeCell(this, ++id);
	//top left
	new PMJMesh::QuadTreeCell(this, ++id);

#if (!USE_EXPERIMENTAL_TREE)
	if (this->getTree())
	{
		this->getTree()->addChildren(this);
	}
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

void PMJMesh::QuadTreeCell::positions(Data::PositionMap &positions, bool onlyLeaves) const
{
	if ((!onlyLeaves) || (this->isLeaf()))
	{
		positions[this->getId()] = this->position;
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		static_cast<PMJMesh::QuadTreeCell *>(this->child(i))->positions(positions, onlyLeaves);
	}
}

#if USE_ARRAY
PMJMesh::QuadTreeCell &PMJMesh::QuadTreeCell::operator=(const PMJMesh::QuadTreeCell &c)
{
	if (this != &c)
	{
		MJMesh::QuadTreeCell::operator=(c);

		this->position = c.position;
	}

	return *this;
}
#endif //#if USE_ARRAY

#if USE_GUI
void PMJMesh::QuadTreeCell::draw() const
{
	if ((this->isLeaf()) ||
		(!this->getTree())) //workaround
	{
		if ((this->getR() > 1.0) && (this->getG() > 1.0) && (this->getB() > 1.0))
        {
            return;
        }

		MJMesh::QuadTreeCell::draw();

		return;
	}

	/*Data::GraphNodeList in, on, out, unknown;

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		switch (static_cast<PMJMesh::QuadTreeCell *>(this->child(i))->position)
		{
		case Data::IN:
			in.push_back(this->child(i));
			break;
		case Data::OUT:
			out.push_back(this->child(i));
			break;
		case Data::ON:
			on.push_back(this->child(i));
			break;
		case Data::UNKNOWN:
			unknown.push_back(this->child(i));
			break;
		}
	}

	for (Data::GraphNodeList::iterator iter = unknown.begin();
		 iter != unknown.end(); iter++)
	{
		(*iter)->draw();
	}

	for (Data::GraphNodeList::iterator iter = out.begin();
		 iter != out.end(); iter++)
	{
		(*iter)->draw();
	}

	for (Data::GraphNodeList::iterator iter = in.begin();
		 iter != in.end(); iter++)
	{
		(*iter)->draw();
	}

	for (Data::GraphNodeList::iterator iter = on.begin();
		 iter != on.end(); iter++)
	{
		(*iter)->draw();
	}*/

	Data::GraphNodeList cells, in, on, out, unknown;

	cells.push_back(const_cast<PMJMesh::QuadTreeCell *>(this));

	while (!cells.empty())
	{
		Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(cells.front());

		cells.pop_front();

		if ((c->isLeaf()) ||
			(!c->getTree())) //workaround
		{
			switch (static_cast<PMJMesh::QuadTreeCell *>(c)->position)
			{
			case Data::IN:
				in.push_back(c);
				break;
			case Data::OUT:
				out.push_back(c);
				break;
			case Data::ON:
				on.push_back(c);
				break;
			case Data::UNKNOWN:
				unknown.push_back(c);
				break;
			}

			continue;
		}

		for (UInt i = 0; i < c->numChildren(); i++)
		{
			cells.push_back(c->child(i));
		}
	}

	for (Data::GraphNodeList::iterator iter = unknown.begin();
		 iter != unknown.end(); iter++)
	{
		(*iter)->draw();
	}

	for (Data::GraphNodeList::iterator iter = out.begin();
		 iter != out.end(); iter++)
	{
		(*iter)->draw();
	}

	for (Data::GraphNodeList::iterator iter = in.begin();
		 iter != in.end(); iter++)
	{
		(*iter)->draw();
	}

	for (Data::GraphNodeList::iterator iter = on.begin();
		 iter != on.end(); iter++)
	{
		(*iter)->draw();
	}
}
#endif //#if USE_GUI
