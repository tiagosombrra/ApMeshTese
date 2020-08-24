#include "PMJMesh/OctTreeCell.h"

#include "Data/Tree.h"

#if USE_GUI
#include "PMJMesh/AngleTreeCellClassifier.h"
#endif //#if USE_GUI

PMJMesh::OctTreeCell::OctTreeCell(PMJMesh::OctTreeCell *parent, ULInt id) :
	MJMesh::OctTreeCell(parent, id)
{
	this->setPosition(
		((parent) && ((parent->getPosition() == Data::IN) || (parent->getPosition() == Data::OUT))) ?
			parent->getPosition() : Data::UNKNOWN);
}

PMJMesh::OctTreeCell::~OctTreeCell()
{

}

void PMJMesh::OctTreeCell::setPosition(enum Data::Position position)
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

enum Data::Position PMJMesh::OctTreeCell::getPosition() const
{
	return this->position;
}

void PMJMesh::OctTreeCell::adjustPositionFromChildren()
{
	if (this->isLeaf())
	{
		return;
	}

	enum Data::Position pos = static_cast<PMJMesh::OctTreeCell *>(this->child(0))->getPosition();

	if (pos == Data::UNKNOWN)
	{
		this->setPosition(Data::UNKNOWN);

		return;
	}

	bool on = (pos == Data::ON);

	bool allequal = true;

	for (UInt i = 1; i < this->numChildren(); i++)
	{
		enum Data::Position cpos = static_cast<PMJMesh::OctTreeCell *>(this->child(i))->getPosition();

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

void PMJMesh::OctTreeCell::adjustPositionFromChildren(bool down)
{
	if (down)
	{
		for (UInt i = 0; i < this->numChildren(); i++)
		{
			static_cast<PMJMesh::OctTreeCell *>(this->child(i))->adjustPositionFromChildren(true);
		}
	}

	this->adjustPositionFromChildren();

	if (!down)
	{
		if (this->parent())
		{
			static_cast<PMJMesh::OctTreeCell *>(this->parent())->adjustPositionFromChildren(false);
		}
	}
}

void PMJMesh::OctTreeCell::subdivide()
{
	ULInt id = 8*this->getId();

	//bottom left far
	new PMJMesh::OctTreeCell(this, ++id);
	//bottom right far
	new PMJMesh::OctTreeCell(this, ++id);
	//top right far
	new PMJMesh::OctTreeCell(this, ++id);
	//top left far
	new PMJMesh::OctTreeCell(this, ++id);
	//bottom left near
	new PMJMesh::OctTreeCell(this, ++id);
	//bottom right near
	new PMJMesh::OctTreeCell(this, ++id);
	//top right near
	new PMJMesh::OctTreeCell(this, ++id);
	//top left near
	new PMJMesh::OctTreeCell(this, ++id);

#if (!USE_EXPERIMENTAL_TREE)
	if (this->getTree())
	{
		this->getTree()->addChildren(this);
	}
#endif //#if (!USE_EXPERIMENTAL_TREE)
}

void PMJMesh::OctTreeCell::positions(Data::PositionMap &positions, bool onlyLeaves) const
{
	if ((!onlyLeaves) || (this->isLeaf()))
	{
		positions[this->getId()] = this->position;
	}

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		static_cast<PMJMesh::OctTreeCell *>(this->child(i))->positions(positions, onlyLeaves);
	}
}

#if USE_ARRAY
PMJMesh::OctTreeCell &PMJMesh::OctTreeCell::operator=(const PMJMesh::OctTreeCell &c)
{
	if (this != &c)
	{
		MJMesh::OctTreeCell::operator=(c);

		this->position = c.position;
	}

	return *this;
}
#endif //#if USE_ARRAY

#if USE_GUI
void PMJMesh::OctTreeCell::draw() const
{
	if ((this->isLeaf()) ||
		(!this->getTree())) //workaround
	{
		if ((this->getR() > 1.0) && (this->getG() > 1.0) && (this->getB() > 1.0))
        {
            return;
        }

		MJMesh::OctTreeCell::draw();

		return;
	}

	Data::GraphNodeList in, on, out, unknown;

	for (UInt i = 0; i < this->numChildren(); i++)
	{
		switch (static_cast<PMJMesh::OctTreeCell *>(this->child(i))->position)
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
	}
}
#endif //#if USE_GUI
