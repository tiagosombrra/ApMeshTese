#include "CRAbMesh/MeshModule.h"

#if USE_GUI

#include <wx/tokenzr.h>
#include <wx/gbsizer.h>

#include "CRAbMesh/Frame.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/Camera.h"
#include "CRAbMesh/TreeItemData.h"
#include "CRAbMesh/ViewModule.h"
#include "Performer/MainDrive.h"
#include "Data/Mesh.h"
#include "Data/Front.h"
#include "Data/Boundary.h"
#include "Data/Vertex.h"
#include "Data/FrontElement.h"
#include "Data/Element.h"
#include "Data/BoxTree.h"
#include "Data/BoxTreeCell.h"
#include "Data/Box.h"

CRAbMesh::MeshModule::MeshModule(Frame *frame, Canvas *canvas) :
	CRAbMesh::Module(frame, canvas)
{
	VERTICES       = "Vertices";
	FRONT_ELEMENTS = "Front elements";
	ELEMENTS       = "Elements";
	TREE_CELLS     = "Tree cells";

	HIGHLIGHT_ADJACENT_FRONT_ELEMENTS = "Highlight adjacent front elements";
	HIGHLIGHT_ADJACENT_ELEMENTS       = "Highlight adjacent elements";

	DRAW_NORMAL           = "Draw normals";
	DRAW_DISCRETIZATION   = "Draw discretization";
	DRAW_BOUNDARY         = "Draw boundary";
	DRAW_TREE             = "Draw tree";
	DRAW_FRONT            = "Draw front";
	DRAW_MESH             = "Draw mesh";
	NOT_ADDING_TREE_ITEMS = "Do not add items in tree";
	FORCE_ADD_TREE_ITEMS  = "Force add items in tree";

	this->AddOption(VERTICES,       TYPE_TREE_ITEM);
	this->AddOption(FRONT_ELEMENTS, TYPE_TREE_ITEM);
	this->AddOption(ELEMENTS,       TYPE_TREE_ITEM);
	this->AddOption(TREE_CELLS,     TYPE_TREE_ITEM);

	this->AddOption(HIGHLIGHT_ADJACENT_FRONT_ELEMENTS, TYPE_MENU_AND_CHECK_ITEM);
	this->AddOption(HIGHLIGHT_ADJACENT_ELEMENTS,       TYPE_MENU_AND_CHECK_ITEM);

	this->AddOption(DRAW_NORMAL,           TYPE_MENU_AND_CHECK_ITEM);
	this->AddOption(DRAW_DISCRETIZATION,   TYPE_MENU_AND_CHECK_ITEM);
	this->AddOption(DRAW_BOUNDARY,         TYPE_MENU_AND_CHECK_ITEM);
	this->AddOption(DRAW_TREE,             TYPE_MENU_AND_CHECK_ITEM);
	this->AddOption(DRAW_FRONT,            TYPE_MENU_AND_CHECK_ITEM);
	this->AddOption(DRAW_MESH,             TYPE_MENU_AND_CHECK_ITEM);
	this->AddOption(NOT_ADDING_TREE_ITEMS, TYPE_MENU_AND_CHECK_ITEM);
	this->AddOption(FORCE_ADD_TREE_ITEMS,  TYPE_MENU);

	this->SetOption(VERTICES, false, false);
	this->SetOption(FRONT_ELEMENTS, false, false);
	this->SetOption(ELEMENTS, false, false);
	this->SetOption(TREE_CELLS, false, false);

	this->SetOption(HIGHLIGHT_ADJACENT_FRONT_ELEMENTS, false, false);
	this->SetOption(HIGHLIGHT_ADJACENT_ELEMENTS, false, false);

	this->SetOption(DRAW_NORMAL, false, false);
	this->SetOption(DRAW_DISCRETIZATION, false, false);
	this->SetOption(DRAW_BOUNDARY, false, false);
	this->SetOption(DRAW_TREE, false, false);
	this->SetOption(DRAW_FRONT, false, false);
	this->SetOption(DRAW_MESH, false, false);
	this->SetOption(NOT_ADDING_TREE_ITEMS, false, false);
	this->SetOption(FORCE_ADD_TREE_ITEMS, false, false);

	this->SetConfiguration(HIGHLIGHT_ADJACENT_FRONT_ELEMENTS, true, false);
	this->SetConfiguration(HIGHLIGHT_ADJACENT_ELEMENTS, true, false);
	this->SetConfiguration(DRAW_NORMAL, false, false);
	this->SetConfiguration(DRAW_DISCRETIZATION, false, false);
	this->SetConfiguration(DRAW_BOUNDARY, true, false);
	this->SetConfiguration(DRAW_TREE, true, false);
	this->SetConfiguration(DRAW_FRONT, true, false);
	this->SetConfiguration(DRAW_MESH, true, false);
	this->SetConfiguration(NOT_ADDING_TREE_ITEMS, false, false);

	this->maxBoundingSize = 0.0;

	//this->tDrawingGauge.SetOwner(this, wxID_ANY);

	//this->Connect(this->tDrawingGauge.GetId(), wxEVT_TIMER, wxTimerEventHandler(CRAbMesh::MeshModule::OnDrawingTimer));
}

CRAbMesh::MeshModule::~MeshModule()
{

}

void CRAbMesh::MeshModule::AddOption(const std::string &option, enum OptionType type)
{
	/*Data::BoolVector vec(3, false);

	if (type == TYPE_MENU_AND_CHECK_ITEM)
	{
		vec[IS_MENU_OR_CHECK_ITEM] = true;
	}*/

	//this->options[option] = vec;
	this->options[option] = TypeOption(type);
	this->optionsMap.push_back(option);
}

void CRAbMesh::MeshModule::InsertOption(const std::string &option, const std::string &before, enum OptionType type)
{
	/*Data::BoolVector vec(3, false);

	if (type == TYPE_MENU_AND_CHECK_ITEM)
	{
		vec[IS_MENU_OR_CHECK_ITEM] = true;
	}*/

	//this->options[option] = vec;
	this->options[option] = TypeOption(type);

	unsigned int position = static_cast<unsigned int>(this->optionsMap.size());

	for (unsigned int i = 0; i < this->optionsMap.size(); i++)
	{
		if (this->optionsMap[i] == before)
		{
			position = i + 1;

			break;
		}
	}

	this->optionsMap.resize(this->optionsMap.size() + 1);

	for (unsigned int i = static_cast<unsigned int>(this->optionsMap.size()) - 1; i > position; i--)
	{
		this->optionsMap[i] = this->optionsMap[i - 1];
	}

	this->optionsMap[position] = option;
}

void CRAbMesh::MeshModule::SetOption(unsigned int option, bool menuOrTree, bool check)
{
	if (option < this->optionsMap.size())
	{
		this->SetOption(this->optionsMap[option], menuOrTree, check);
	}
}

void CRAbMesh::MeshModule::SetOption(const std::string &option, bool menuOrTree, bool check)
{
	//this->options[option][MENU_ITEM] = menuOrTree;
	//this->options[option][CHECK_BOX] = check;
	this->options[option].set(menuOrTree, check);
}

void CRAbMesh::MeshModule::SetConfiguration(unsigned int option, bool value, bool refresh)
{
	if (option < this->optionsMap.size())
	{
		this->SetConfiguration(this->optionsMap[option], value, refresh);
	}
}

void CRAbMesh::MeshModule::SetConfiguration(const std::string &option, bool value, bool refresh)
{
	this->configuration[option] = value;

	//if (this->IsEnabled(option, MENU_ITEM))
	if (this->IsEnabled(option, MENU_CHECK_ITEM))
	{
		this->miConfItems[option]->Check(value);
	}

	if (this->IsEnabled(option, CHECK_BOX))
	{
		this->cbConfItems[option]->SetValue(value);
	}

	if (refresh)
	{
		this->canvas->Refresh(false);
	}
}

bool CRAbMesh::MeshModule::IsConfiguration(unsigned int option) const
{
	return (option < this->optionsMap.size()) ? this->IsConfiguration(this->optionsMap[option]) : false;
}

bool CRAbMesh::MeshModule::IsConfiguration(const std::string &option) const
{
	StringBoolMap::const_iterator iter = this->configuration.find(option);

	return (iter == this->configuration.end()) ? false : (*iter).second;
}

//bool CRAbMesh::MeshModule::IsEnabled(unsigned int option, int item) const
bool CRAbMesh::MeshModule::IsEnabled(unsigned int option, enum ItemType item) const
{
	return (option >= this->optionsMap.size()) ? false : this->IsEnabled(this->optionsMap[option], item);
}

//bool CRAbMesh::MeshModule::IsEnabled(const std::string &option, int item) const
bool CRAbMesh::MeshModule::IsEnabled(const std::string &option, enum ItemType item) const
{
	//StringBoolVectorMap::const_iterator iter = this->options.find(option);
	StringTypeOptionMap::const_iterator iter = this->options.find(option);

	//return (iter == this->options.end()) ? false : (*iter).second[item];
	return (iter == this->options.end()) ? false : (*iter).second.is(item);
}

void CRAbMesh::MeshModule::Render() const
{
    if (this->IsDrawable())
	{
#if USE_SCREENSHOT
		glShadeModel(GL_FLAT);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_BLEND);
#endif //#if USE_SCREENSHOT

		if (this->IsTreeDrawable())
		{
			if (this->GetTree())
			{
#if USE_SCREENSHOT
				Data::GraphNodeList nodes, leaves;

				nodes.push_back(this->GetTree()->getRoot());

				while (!nodes.empty())
				{
					Data::TreeNode *n = static_cast<Data::TreeNode *>(nodes.front());

					nodes.pop_front();

					if (n->isLeaf())
					{
						leaves.push_back(n);
					}
					else
					{
						for (UInt i = 0; i < n->numChildren(); i++)
						{
							nodes.push_back(n->child(i));
						}
					}
				}

				for (Data::GraphNodeList::iterator iter = leaves.begin();
					 iter != leaves.end(); iter++)
				{
					Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>((*iter));

					if (c->box()->isHighlighted())
					{
						c->draw();
					}
				}

				for (Data::GraphNodeList::iterator iter = leaves.begin();
					 iter != leaves.end(); iter++)
				{
					Data::BoxTreeCell *c = (Data::BoxTreeCell *)(*iter);

					if (!c->box()->isHighlighted())
					{
						c->draw();
					}
				}
#else
				this->GetTree()->draw();
#endif //#if USE_SCREENSHOT
			}
		}

#if USE_SCREENSHOT
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_BLEND);
#endif //#if USE_SCREENSHOT

		Real factor = this->MakeNormalFactor();

		if (this->IsMeshDrawable())
		{
			if (this->GetMesh())
			{
				this->GetMesh()->draw();

				if (this->IsNormalDrawable())
				{
					this->GetMesh()->drawNormals(factor);
				}
			}
		}

		if (this->IsFrontDrawable())
		{
			if (this->GetFront())
			{
				this->GetFront()->draw();

				if (this->IsNormalDrawable())
				{
					this->GetFront()->drawNormals(factor);
				}

				if (this->IsDiscretizationDrawable())
				{
					this->GetFront()->drawDiscretization();
				}
			}
		}

		if (this->IsBoundaryDrawable())
		{
			if (this->GetBoundary())
			{
				this->GetBoundary()->draw();

				if (this->IsNormalDrawable())
				{
					this->GetBoundary()->drawNormals(factor);
				}

				if (this->IsDiscretizationDrawable())
				{
					this->GetBoundary()->drawDiscretization();
				}
			}
		}

		wxTreeItemId item = this->tree->GetSelection();

		if (item.IsOk())
		{
			if (((this->IsEnabled(VERTICES, TREE_ITEM))       && (this->tree->GetItemParent(item) == (*this->tiItems.find(VERTICES)).second)) ||
				((this->IsEnabled(FRONT_ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(item) == (*this->tiItems.find(FRONT_ELEMENTS)).second)) ||
				((this->IsEnabled(ELEMENTS, TREE_ITEM))       && (this->tree->GetItemParent(item) == (*this->tiItems.find(ELEMENTS)).second)) ||
				((this->IsEnabled(TREE_CELLS, TREE_ITEM))     && (this->tree->GetItemParent(item) == (*this->tiItems.find(TREE_CELLS)).second)))
			{
				if (this->tree->GetItemParent(item) == (*this->tiItems.find(VERTICES)).second)
				{
					if ((this->IsAdjacentFrontElementHighlighted()) || (this->IsAdjacentElementHighlighted()))
					{
						Data::Vertex *v = static_cast<Data::Vertex *>(static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem());

						if (this->IsAdjacentElementHighlighted())
						{
							Data::ElementList adjs = (this->GetMesh()) ?
								this->GetMesh()->adjacency(v) : Data::ElementList();

							for (Data::ElementList::const_iterator iter = adjs.begin();
								 iter != adjs.end(); iter++)
							{
								(*iter)->highlight();
								(*iter)->draw();
								(*iter)->unhighlight();
							}
						}

						if (this->IsAdjacentFrontElementHighlighted())
						{
							Data::FrontElementList adjs = (this->GetMesh()) ? this->GetMesh()->findAdjacentFrontElements(v) :
								((this->GetBoundary()) ? this->GetBoundary()->adjacency(v) : Data::FrontElementList());

							for (Data::FrontElementList::const_iterator iter = adjs.begin();
								 iter != adjs.end(); iter++)
							{
								(*iter)->highlight();
								(*iter)->draw();
								(*iter)->unhighlight();
							}
						}
					}
				}
				if (this->tree->GetItemParent(item) == (*this->tiItems.find(FRONT_ELEMENTS)).second)
				{
					if (this->IsAdjacentElementHighlighted())
					{
						Data::FrontElement *fel = static_cast<Data::FrontElement *>(static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem());

						Data::ElementList adjs = (this->GetMesh()) ?
							this->GetMesh()->adjacency(fel) : Data::ElementList();

						for (Data::ElementList::const_iterator iter = adjs.begin();
							 iter != adjs.end(); iter++)
						{
							(*iter)->highlight();
							(*iter)->draw();
							(*iter)->unhighlight();
						}
					}
				}
				/*
				else if (this->tree->GetItemParent(item) == (*this->tiItems.find(TREE_CELLS)).second)
				{
					Data::BoxTreeCell *c = static_cast<Data::BoxTreeCell *>(static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem());

					Data::UIntVector sides(3, 0);
					Data::GraphNodeList neighs;

					for (UInt side = 0; side < 2*c->dimension(); side++)
					{
						neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, side);

						for (Data::GraphNodeList::iterator iter = neighs.begin();
							 iter != neighs.end(); iter++)
						{
							Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
							Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
							Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

							Real col = static_cast<Real>(side + 1)/(2*c->dimension() + 2);

							static_cast<Data::BoxTreeCell *>((*iter))->setColor(col, col, col);

							static_cast<Data::BoxTreeCell *>((*iter))->highlight();
							static_cast<Data::BoxTreeCell *>((*iter))->draw();
							static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

							static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
						}
					}

					if (sides.size() == 1) {sides[0] = 0;}
					else if (c->dimension() == 2) {sides[0] = 0; sides[1] = 1;}
					else if (sides.size() == 2) {sides[0] = 0; sides[1] = 1;}
					else {sides[0] = 0; sides[1] = 1; sides[2] = 2;}

					neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

					for (Data::GraphNodeList::iterator iter = neighs.begin();
						 iter != neighs.end(); iter++)
					{
						Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
						Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
						Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

						static_cast<Data::BoxTreeCell *>((*iter))->setColor(1.0, 0.0, 0.0);

						static_cast<Data::BoxTreeCell *>((*iter))->highlight();
						static_cast<Data::BoxTreeCell *>((*iter))->draw();
						static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

						static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
						static_cast<Data::BoxTreeCell *>((*iter))->draw();
					}

					if (sides.size() == 1) {sides[0] = 1;}
					else if (c->dimension() == 2) {sides[0] = 1; sides[1] = 2;}
					else if (sides.size() == 2) {sides[0] = 0; sides[1] = 2;}
					else {sides[0] = 0; sides[1] = 1; sides[2] = 5;}

					neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

					for (Data::GraphNodeList::iterator iter = neighs.begin();
						 iter != neighs.end(); iter++)
					{
						Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
						Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
						Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

						static_cast<Data::BoxTreeCell *>((*iter))->setColor(0.0, 1.0, 0.0);

						static_cast<Data::BoxTreeCell *>((*iter))->highlight();
						static_cast<Data::BoxTreeCell *>((*iter))->draw();
						static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

						static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
						static_cast<Data::BoxTreeCell *>((*iter))->draw();
					}

					if (sides.size() == 1) {sides[0] = 2;}
					else if (c->dimension() == 2) {sides[0] = 2; sides[1] = 3;}
					else if (sides.size() == 2) {sides[0] = 0; sides[1] = 4;}
					else {sides[0] = 0; sides[1] = 2; sides[2] = 4;}

					neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

					for (Data::GraphNodeList::iterator iter = neighs.begin();
						 iter != neighs.end(); iter++)
					{
						Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
						Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
						Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

						static_cast<Data::BoxTreeCell *>((*iter))->setColor(0.0, 0.0, 1.0);

						static_cast<Data::BoxTreeCell *>((*iter))->highlight();
						static_cast<Data::BoxTreeCell *>((*iter))->draw();
						static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

						static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
						static_cast<Data::BoxTreeCell *>((*iter))->draw();
					}

					if (sides.size() == 1) {sides[0] = 3;}
					else if (c->dimension() == 2) {sides[0] = 3; sides[1] = 0;}
					else if (sides.size() == 2) {sides[0] = 0; sides[1] = 5;}
					else {sides[0] = 0; sides[1] = 4; sides[2] = 5;}

					neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

					for (Data::GraphNodeList::iterator iter = neighs.begin();
						 iter != neighs.end(); iter++)
					{
						Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
						Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
						Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

						static_cast<Data::BoxTreeCell *>((*iter))->setColor(1.0, 1.0, 0.0);

						static_cast<Data::BoxTreeCell *>((*iter))->highlight();
						static_cast<Data::BoxTreeCell *>((*iter))->draw();
						static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

						static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
						static_cast<Data::BoxTreeCell *>((*iter))->draw();
					}

					if (c->dimension() == 3)
					{
						if (sides.size() == 1) {sides[0] = 4;}
						else if (sides.size() == 2) {sides[0] = 1; sides[1] = 2;}
						else {sides[0] = 1; sides[1] = 2; sides[2] = 3;}

						neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

						for (Data::GraphNodeList::iterator iter = neighs.begin();
							 iter != neighs.end(); iter++)
						{
							Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
							Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
							Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

							static_cast<Data::BoxTreeCell *>((*iter))->setColor(1.0, 0.0, 1.0);

							static_cast<Data::BoxTreeCell *>((*iter))->highlight();
							static_cast<Data::BoxTreeCell *>((*iter))->draw();
							static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

							static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
							static_cast<Data::BoxTreeCell *>((*iter))->draw();
						}

						if (sides.size() == 1) {sides[0] = 5;}
						else if (sides.size() == 2) {sides[0] = 1; sides[1] = 3;}
						else {sides[0] = 1; sides[1] = 3; sides[2] = 5;}

						neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

						for (Data::GraphNodeList::iterator iter = neighs.begin();
							 iter != neighs.end(); iter++)
						{
							Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
							Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
							Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

							static_cast<Data::BoxTreeCell *>((*iter))->setColor(0.0, 1.0, 1.0);

							static_cast<Data::BoxTreeCell *>((*iter))->highlight();
							static_cast<Data::BoxTreeCell *>((*iter))->draw();
							static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

							static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
							static_cast<Data::BoxTreeCell *>((*iter))->draw();
						}

						if (sides.size() > 1)
						{
							if (sides.size() == 2) {sides[0] = 1; sides[1] = 5;}
							else {sides[0] = 2; sides[1] = 3; sides[2] = 4;}

							neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

							for (Data::GraphNodeList::iterator iter = neighs.begin();
								 iter != neighs.end(); iter++)
							{
								Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
								Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
								Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

								static_cast<Data::BoxTreeCell *>((*iter))->setColor(0.5, 0.0, 0.0);

								static_cast<Data::BoxTreeCell *>((*iter))->highlight();
								static_cast<Data::BoxTreeCell *>((*iter))->draw();
								static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

								static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
								static_cast<Data::BoxTreeCell *>((*iter))->draw();
							}

							if (sides.size() == 2) {sides[0] = 2; sides[1] = 3;}
							else {sides[0] = 3; sides[1] = 4; sides[2] = 5;}

							neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

							for (Data::GraphNodeList::iterator iter = neighs.begin();
								 iter != neighs.end(); iter++)
							{
								Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
								Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
								Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

								static_cast<Data::BoxTreeCell *>((*iter))->setColor(0.0, 0.5, 0.0);

								static_cast<Data::BoxTreeCell *>((*iter))->highlight();
								static_cast<Data::BoxTreeCell *>((*iter))->draw();
								static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

								static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
								static_cast<Data::BoxTreeCell *>((*iter))->draw();
							}

							if (sides.size() == 2)
							{
								sides[0] = 2; sides[1] = 4;

								neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

								for (Data::GraphNodeList::iterator iter = neighs.begin();
									 iter != neighs.end(); iter++)
								{
									Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
									Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
									Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

									static_cast<Data::BoxTreeCell *>((*iter))->setColor(0.0, 0.0, 0.5);

									static_cast<Data::BoxTreeCell *>((*iter))->highlight();
									static_cast<Data::BoxTreeCell *>((*iter))->draw();
									static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

									static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
									static_cast<Data::BoxTreeCell *>((*iter))->draw();
								}

								sides[0] = 3; sides[1] = 4;

								neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

								for (Data::GraphNodeList::iterator iter = neighs.begin();
									 iter != neighs.end(); iter++)
								{
									Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
									Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
									Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

									static_cast<Data::BoxTreeCell *>((*iter))->setColor(0.5, 0.5, 0.0);

									static_cast<Data::BoxTreeCell *>((*iter))->highlight();
									static_cast<Data::BoxTreeCell *>((*iter))->draw();
									static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

									static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
									static_cast<Data::BoxTreeCell *>((*iter))->draw();
								}

								sides[0] = 3; sides[1] = 5;

								neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

								for (Data::GraphNodeList::iterator iter = neighs.begin();
									 iter != neighs.end(); iter++)
								{
									Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
									Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
									Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

									static_cast<Data::BoxTreeCell *>((*iter))->setColor(0.5, 0.0, 0.5);

									static_cast<Data::BoxTreeCell *>((*iter))->highlight();
									static_cast<Data::BoxTreeCell *>((*iter))->draw();
									static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

									static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
									static_cast<Data::BoxTreeCell *>((*iter))->draw();
								}

								sides[0] = 4; sides[1] = 5;

								neighs = static_cast<Data::BoxTree *>(c->getTree())->neighbors(c, sides);

								for (Data::GraphNodeList::iterator iter = neighs.begin();
									 iter != neighs.end(); iter++)
								{
									Real r = static_cast<Data::BoxTreeCell *>((*iter))->getR();
									Real g = static_cast<Data::BoxTreeCell *>((*iter))->getG();
									Real b = static_cast<Data::BoxTreeCell *>((*iter))->getB();

									static_cast<Data::BoxTreeCell *>((*iter))->setColor(0.0, 0.5, 0.5);

									static_cast<Data::BoxTreeCell *>((*iter))->highlight();
									static_cast<Data::BoxTreeCell *>((*iter))->draw();
									static_cast<Data::BoxTreeCell *>((*iter))->unhighlight();

									static_cast<Data::BoxTreeCell *>((*iter))->setColor(r, g, b);
									static_cast<Data::BoxTreeCell *>((*iter))->draw();
								}
							}
						}
					}
				}
				//*/

				static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem()->highlight();
				static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem()->draw();
				static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem()->unhighlight();
			}
		}
	}
	//else
	{
		for (Data::DrawableList::const_iterator iter = this->picked.begin();
			 iter != this->picked.end(); iter++)
		{
			(*iter)->draw();
		}
	}
}

void CRAbMesh::MeshModule::SetOpenGLStateBeforeRendering(/*const Camera *camera*/) const
{
	if (!this->camera)
	{
		return;
	}

	//glDisable(GL_CLIP_PLANE0);

	if (this->GetDimension() == 3)
	{
		glDepthFunc(GL_LEQUAL);

		/*if (this->frame->GetViewModule()->IsWireframeEnabled())
		{
			glDisable(GL_LIGHTING);
		}
		else*/
		{
		    GLfloat global_amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_amb);
		    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

			GLfloat pos[] = {0.0f, 0.0f, 0.0f, 0.0f};
			//GLfloat amb[] = {0.8f, 0.8f, 0.8f, 1.0f};
			GLfloat amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
			//GLfloat dif[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLfloat dif[] = {0.8f, 0.8f, 0.8f, 1.0f};
			GLfloat spe[] = {0.2f, 0.2f, 0.2f, 1.0f};

			double dist = 0.0;

			for (unsigned int i = 0; i < 3; i++)
			{
				//dist += std::pow(this->camera->GetDefaultPosition(i) - this->camera->GetDefaultFront(i), 2.0);
				dist += std::pow(this->camera->GetPosition(i) - this->camera->GetFront(i), 2.0);
			}

			dist = std::sqrt(dist);
			dist *= 4.0;

			//pos[0] += static_cast<GLfloat>(this->camera->GetDefaultPosition(0) + dist*this->camera->GetDefaultUp(0));
			pos[0] += static_cast<GLfloat>(this->camera->GetPosition(0) + dist*this->camera->GetUp(0));
			//pos[1] += static_cast<GLfloat>(this->camera->GetDefaultPosition(1) + dist*this->camera->GetDefaultUp(1));
			pos[1] += static_cast<GLfloat>(this->camera->GetPosition(1) + dist*this->camera->GetUp(1));
			//pos[2] += static_cast<GLfloat>(this->camera->GetDefaultPosition(2) + dist*this->camera->GetDefaultUp(2));
			pos[2] += static_cast<GLfloat>(this->camera->GetPosition(2) + dist*this->camera->GetUp(2));

			//std::cout << "setpos " << pos[0] << ", " << pos[1] << ", " << pos[2] << ", " << pos[3] << std::endl;

			glPushMatrix();
                this->frame->GetCurrentOrViewCamera()->DrawModelview();

                glLightfv(GL_LIGHT0, GL_POSITION, pos);
            glPopMatrix();

			//glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, amb);
			glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
			glLightfv(GL_LIGHT0, GL_SPECULAR, spe);

			//GLfloat spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
			//GLfloat spec[] = {0.8f, 0.8f, 0.8f, 1.0f};
			GLfloat spec[] = {0.2f, 0.2f, 0.2f, 1.0f};

			glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
			//glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);
			glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);

			glEnable(GL_NORMALIZE);
		}
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	/*double eq[4] = {1.0, 0.0, 0.0, 0.0};
	glClipPlane(GL_CLIP_PLANE0, eq);
	glEnable(GL_CLIP_PLANE0);*/
}

bool CRAbMesh::MeshModule::BoundingBox(double &minx, double &miny, double &minz, double &maxx, double &maxy, double &maxz) const
{
	minx = miny = minz = DBL_MAX;
	maxx = maxy = maxz = -DBL_MAX;

	if ((this->GetTree()) && (this->GetTree()->getShape()))
	{
		minx = this->GetTree()->getShape()->getPoint(0)->getCoord(0);
		miny = this->GetTree()->getShape()->getPoint(0)->getCoord(1);
		minz = (this->GetTree()->getShape()->getPoint(0)->dimension() == 2) ? 0.0 :
			this->GetTree()->getShape()->getPoint(0)->getCoord(2);

		maxx = this->GetTree()->getShape()->getPoint(1)->getCoord(0);
		maxy = this->GetTree()->getShape()->getPoint(1)->getCoord(1);
		maxz = (this->GetTree()->getShape()->getPoint(1)->dimension() == 2) ? 0.0 :
			this->GetTree()->getShape()->getPoint(1)->getCoord(2);

		this->maxBoundingSize = std::max(maxx - minx, std::max(maxy - miny, maxz - minz));

		return true;
	}

	if ((this->GetBoundary()) && (this->GetBoundary()->size() > 0))
	{
		Data::Box *box = this->GetBoundary()->box();

		UInt dim = box->dimension();

		minx = box->getMin()->getCoord(0);
		miny = box->getMin()->getCoord(1);
		minz = (dim == 2) ? 0.0 : box->getMin()->getCoord(2);

		maxx = box->getMax()->getCoord(0);
		maxy = box->getMax()->getCoord(1);
		maxz = (dim == 2) ? 0.0 : box->getMax()->getCoord(2);

		delete box;

		this->maxBoundingSize = std::max(maxx - minx, std::max(maxy - miny, maxz - minz));

		return true;
	}

	if (this->GetMesh())
	{
		//Data::VertexList vertices = this->GetMesh()->getVertices();

		//while (!vertices.empty())
		for (Data::Mesh::ConstVertexIterator iter = this->GetMesh()->vBegin();
			 iter != this->GetMesh()->vEnd(); iter++)
		{
			//Data::Vertex *v = vertices.front();
			Data::Vertex *v = (*iter);
			//vertices.pop_front();

			if (v->getPoint()->getCoord(0) < minx)
			{
				minx = v->getPoint()->getCoord(0);
			}

			if (v->getPoint()->getCoord(1) < miny)
			{
				miny = v->getPoint()->getCoord(1);
			}

			if (v->getPoint()->getCoord(0) > maxx)
			{
				maxx = v->getPoint()->getCoord(0);
			}

			if (v->getPoint()->getCoord(1) > maxy)
			{
				maxy = v->getPoint()->getCoord(1);
			}

			if (v->getPoint()->dimension() == 2)
            {
                if (0.0 < minz)
                {
                    minz = 0.0;
                }

                if (0.0 > maxz)
                {
                    maxz = 0.0;
                }
            }
            else
            {
                if (v->getPoint()->getCoord(2) < minz)
                {
                    minz = v->getPoint()->getCoord(2);
                }

                if (v->getPoint()->getCoord(2) > maxz)
                {
                    maxz = v->getPoint()->getCoord(2);
                }
            }
		}

		this->maxBoundingSize = std::max(maxx - minx, std::max(maxy - miny, maxz - minz));

		return true;
	}

	this->maxBoundingSize = 0.0;

	return false;
}

void CRAbMesh::MeshModule::SetAdjacentFrontElementHighlighted(bool adjacentFrontElementHighlighted)
{
	this->SetConfiguration(HIGHLIGHT_ADJACENT_FRONT_ELEMENTS, adjacentFrontElementHighlighted);
}

bool CRAbMesh::MeshModule::IsAdjacentFrontElementHighlighted() const
{
	return this->IsConfiguration(HIGHLIGHT_ADJACENT_FRONT_ELEMENTS);
}

void CRAbMesh::MeshModule::SetAdjacentElementHighlighted(bool adjacentElementHighlighted)
{
	this->SetConfiguration(HIGHLIGHT_ADJACENT_ELEMENTS, adjacentElementHighlighted);
}

bool CRAbMesh::MeshModule::IsAdjacentElementHighlighted() const
{
	return this->IsConfiguration(HIGHLIGHT_ADJACENT_ELEMENTS);
}

void CRAbMesh::MeshModule::SetNormalDrawable(bool normalDrawable)
{
    this->SetConfiguration(DRAW_NORMAL, normalDrawable);
}

bool CRAbMesh::MeshModule::IsNormalDrawable() const
{
    return this->IsConfiguration(DRAW_NORMAL);
}

void CRAbMesh::MeshModule::SetDiscretizationDrawable(bool discretizationDrawable)
{
    this->SetConfiguration(DRAW_DISCRETIZATION, discretizationDrawable);
}

bool CRAbMesh::MeshModule::IsDiscretizationDrawable() const
{
    return this->IsConfiguration(DRAW_DISCRETIZATION);
}

void CRAbMesh::MeshModule::SetBoundaryDrawable(bool boundaryDrawable)
{
	this->SetConfiguration(DRAW_BOUNDARY, boundaryDrawable);
}

bool CRAbMesh::MeshModule::IsBoundaryDrawable() const
{
	return this->IsConfiguration(DRAW_BOUNDARY);
}

void CRAbMesh::MeshModule::SetTreeDrawable(bool treeDrawable)
{
    this->SetConfiguration(DRAW_TREE, treeDrawable);
}

bool CRAbMesh::MeshModule::IsTreeDrawable() const
{
    return this->IsConfiguration(DRAW_TREE);
}

void CRAbMesh::MeshModule::SetFrontDrawable(bool frontDrawable)
{
	this->SetConfiguration(DRAW_FRONT, frontDrawable);
}

bool CRAbMesh::MeshModule::IsFrontDrawable() const
{
	return this->IsConfiguration(DRAW_FRONT);
}

void CRAbMesh::MeshModule::SetMeshDrawable(bool meshDrawable)
{
    this->SetConfiguration(DRAW_MESH, meshDrawable);
}

bool CRAbMesh::MeshModule::IsMeshDrawable() const
{
    return this->IsConfiguration(DRAW_MESH);
}

bool CRAbMesh::MeshModule::IsDrawable() const
{
    return (this->IsBoundaryDrawable() ||
			this->IsTreeDrawable() ||
			this->IsFrontDrawable() ||
			this->IsMeshDrawable());
}

void CRAbMesh::MeshModule::SetNotAddingTreeItems(bool notAddingTreeItems)
{
	this->SetConfiguration(NOT_ADDING_TREE_ITEMS, notAddingTreeItems, false);
}

bool CRAbMesh::MeshModule::IsNotAddingTreeItems() const
{
	return this->IsConfiguration(NOT_ADDING_TREE_ITEMS);
}

void CRAbMesh::MeshModule::ClearDisplayLists(bool force) const
{
	if (this->GetTree())
	{
		if ((force) || (this->GetTree()->isOutdated()))
		{
			this->GetTree()->clearLists();
		}
	}

	if (this->GetMesh())
	{
		if ((force) || (this->GetMesh()->isOutdated()))
		{
			this->GetMesh()->clearLists();
		}
	}

	if (this->GetFront())
	{
		if ((force) || (this->GetFront()->isOutdated()))
		{
			this->GetFront()->clearLists();
		}
	}

	if (this->GetBoundary())
	{
		if ((force) || (this->GetBoundary()->isOutdated()))
		{
			this->GetBoundary()->clearLists();
		}
	}
}

/*void CRAbMesh::MeshModule::StartGaugeTimer()
{
	if (!this->tDrawingGauge.IsRunning())
	{
		this->tDrawingGauge.Start(100);
	}
}

void CRAbMesh::MeshModule::EndGaugeTimer()
{
    this->tDrawingGauge.Stop();
}*/

bool CRAbMesh::MeshModule::CreateMenuItem(unsigned int option, wxMenu *menu, bool prependSeparator, bool appendSeparator)
{
	return (option >= this->optionsMap.size()) ? false : this->CreateMenuItem(this->optionsMap[option], menu, prependSeparator, appendSeparator);
}

bool CRAbMesh::MeshModule::CreateMenuItem(const std::string &option, wxMenu *menu, bool prependSeparator, bool appendSeparator)
{
	if (!this->IsEnabled(option, MENU_ITEM))
	{
		this->miConfItems[option] = NULL;

		return false;
	}

	if (prependSeparator)
	{
		menu->AppendSeparator();
	}

	wxString text(option.c_str(), wxMBConvLibc());

	this->miConfItems[option] = this->IsEnabled(option, MENU_CHECK_ITEM) ?
        menu->AppendCheckItem(wxID_ANY, text) :
        menu->Append(wxID_ANY, text);

	if (appendSeparator)
	{
		menu->AppendSeparator();
	}

	if (this->IsEnabled(option, MENU_CHECK_ITEM))
    {
        this->miConfItems[option]->Check(this->IsConfiguration(option));
    }

	this->Connect(this->miConfItems[option]->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MeshModule::OnConfItem));

	return true;
}

bool CRAbMesh::MeshModule::CreateCheckBox(unsigned int option, wxPanel *panel, wxGridBagSizer *sizer, int &row, bool prependSeparator, bool appendSeparator)
{
	return (option >= this->optionsMap.size()) ? false : this->CreateCheckBox(this->optionsMap[option], panel, sizer, row, prependSeparator, appendSeparator);
}

bool CRAbMesh::MeshModule::CreateCheckBox(const std::string &option, wxPanel *panel, wxGridBagSizer *sizer, int &row, bool prependSeparator, bool appendSeparator)
{
	if (!this->IsEnabled(option, CHECK_BOX))
	{
		this->cbConfItems[option] = NULL;

		return false;
	}

	wxString text(option.c_str(), wxMBConvLibc());

	this->cbConfItems[option] = new wxCheckBox(panel, wxID_ANY, text);

	this->cbConfItems[option]->SetValue(this->IsConfiguration(option));

	if (prependSeparator)
	{
		sizer->Add(0, 4, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
	}

	sizer->Add(this->cbConfItems[option], wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);

	if (appendSeparator)
	{
		sizer->Add(0, 4, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxALL, 0);
	}

	this->Connect(this->cbConfItems[option]->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(MeshModule::OnConfItem));

	return true;
}

bool CRAbMesh::MeshModule::CreateTreeItem(unsigned int option)
{
	return (option >= this->optionsMap.size()) ? false : this->CreateTreeItem(this->optionsMap[option]);
}

bool CRAbMesh::MeshModule::CreateTreeItem(const std::string &option)
{
	if (!this->IsEnabled(option, TREE_ITEM))
	{
		this->tiItems[option] = 0;

		return false;
	}

	wxString text(option.c_str(), wxMBConvLibc());

	this->tiItems[option] = this->tree->AppendItem(this->tiRoot, text);

	return true;
}

wxMenu *CRAbMesh::MeshModule::MakeConfigurationMenu()
{
	wxMenu *mConfiguration = new wxMenu();

	bool addedBefore = false;

	for (StringVector::iterator iter = this->optionsMap.begin();
		 iter != this->optionsMap.end(); iter++)
	{
		std::string option = (*iter);

		bool prepend = (addedBefore) && ((option == NOT_ADDING_TREE_ITEMS) || (option == DRAW_NORMAL));

		//if (this->options[option][IS_MENU_OR_CHECK_ITEM])
		if (this->options[option].is(MENU_ITEM))
		{
			addedBefore = this->CreateMenuItem(option, mConfiguration, prepend) || addedBefore;
		}
	}

	return mConfiguration;
}

wxPanel *CRAbMesh::MeshModule::MakeConfigurationPanel()
{
	wxPanel *pConfiguration = new wxScrolledWindow(this->frame, wxID_ANY, wxDefaultPosition, wxSize(210, 440), wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL);
	static_cast<wxScrolledWindow *>(pConfiguration)->SetScrollRate(5, 5);

	this->sbsQuality = NULL;
	this->sbsDrawOptions = NULL;
	//this->gDrawing = NULL;
	wxGridBagSizer *gbsConf = NULL;

	int row = 0;

	bool any = false;

	for (StringVector::iterator iter = this->optionsMap.begin();
		 iter != this->optionsMap.end(); iter++)
	{
		std::string option = (*iter);

		//if (this->options[option][IS_MENU_OR_CHECK_ITEM])
		if (this->options[option].is(MENU_ITEM))
		{
			any = (any || this->IsEnabled(option, CHECK_BOX));

			if (any)
			{
				break;
			}
		}
	}

    if (any)
	{
		gbsConf = new wxGridBagSizer();
		gbsConf->SetFlexibleDirection(wxBOTH);
		gbsConf->AddGrowableCol(0);

		any = false;
	}

	for (StringVector::iterator iter = this->optionsMap.begin();
		 iter != this->optionsMap.end(); iter++)
	{
		std::string option = (*iter);

		bool prepend = (any) && ((option == NOT_ADDING_TREE_ITEMS) || (option == DRAW_NORMAL));

		if (this->options[option].is(CHECK_BOX))
		{
			any = ((this->CreateCheckBox(option, pConfiguration, gbsConf, row, prepend)) || (any));
		}
	}

    if (any)
	{
		//this->gDrawing = new wxGauge(pConfiguration, wxID_ANY, 0);

		this->sbsDrawOptions = new wxStaticBoxSizer(wxVERTICAL, pConfiguration, _("Drawing"));
		this->sbsDrawOptions->Add(gbsConf, 0, wxEXPAND | wxALL, 1);
		//this->sbsDrawOptions->Add(this->gDrawing, 0, wxEXPAND | wxALL, 1);
		pConfiguration->SetSizer(this->sbsDrawOptions);
	}

	return pConfiguration;
}

wxPanel *CRAbMesh::MeshModule::MakeTreePanel()
{
	bool any = false;

	for (StringVector::iterator iter = this->optionsMap.begin();
		 iter != this->optionsMap.end(); iter++)
	{
		std::string option = (*iter);

		//if (!this->options[option][IS_MENU_OR_CHECK_ITEM])
		{
			any = (any || this->IsEnabled(option, TREE_ITEM));

			if (any)
			{
				break;
			}
		}
	}

	if (!any)
	{
		return NULL;
	}

	wxPanel *pTree = new wxScrolledWindow(this->frame, wxID_ANY, wxDefaultPosition, wxSize(200, 440));

	this->tree = new wxTreeCtrl(pTree, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxTR_DEFAULT_STYLE | wxTR_HAS_BUTTONS | wxTR_HAS_VARIABLE_ROW_HEIGHT);

	this->tiRoot = this->tree->AddRoot(_("Mesh items"));

	for (StringVector::iterator iter = this->optionsMap.begin();
		 iter != this->optionsMap.end(); iter++)
	{
		std::string option = (*iter);

		//if (!this->options[option][IS_MENU_OR_CHECK_ITEM])
		if (this->IsEnabled(option, TREE_ITEM))
		{
			this->CreateTreeItem(option);
		}
	}

	this->tree->Expand(this->tiRoot);

	wxBoxSizer *bsTree = new wxBoxSizer(wxVERTICAL);
	bsTree->Add(this->tree, 1, wxEXPAND | wxALL, 2);
	bsTree->SetMinSize(190, 440);
	pTree->SetSizer(bsTree);

	this->Connect(this->tree->GetId(), wxEVT_COMMAND_TREE_SEL_CHANGED,       wxTreeEventHandler(MeshModule::OnTreeSelectionChanged));
	this->Connect(this->tree->GetId(), wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK, wxTreeEventHandler(MeshModule::OnTreeMiddleClick));
	this->Connect(this->tree->GetId(), wxEVT_TREE_KEY_DOWN,                  wxTreeEventHandler(MeshModule::OnTreeKeyDown));

    return pTree;
}

void CRAbMesh::MeshModule::RenderPick() const
{
	if ((this->IsMeshDrawable()) && (this->GetMesh()))
	{
		this->GetMesh()->drawPick();
	}

	if ((this->IsFrontDrawable()) && (this->GetFront()))
	{
		this->GetFront()->drawPick();
	}

	if ((this->IsBoundaryDrawable()) && (this->GetBoundary()))
	{
		this->GetBoundary()->drawPick();
	}
}

unsigned int CRAbMesh::MeshModule::PickBufferSize() const
{
	return 512*(Data::Drawable::numElems + 3);
}

Data::DrawableList CRAbMesh::MeshModule::Picked(int hits, GLuint buffer[])
{
	if (((this->IsMeshDrawable()) || (!this->IsDrawable())) && (this->GetMesh()))
	{
		return this->GetMesh()->picked(static_cast<GLint>(hits), buffer);
	}

	if ((this->IsFrontDrawable()) && (this->GetFront()))
	{
		return this->GetFront()->picked(static_cast<GLint>(hits), buffer);
	}

	if ((this->IsBoundaryDrawable()) && (this->GetBoundary()))
	{
		return this->GetBoundary()->picked(static_cast<GLint>(hits), buffer);
	}

	return Data::DrawableList();
}

void CRAbMesh::MeshModule::OnConfItem(wxCommandEvent &event)
{
	bool done = false;

	for (StringMenuItemMap::iterator iter = this->miConfItems.begin();
		 (!done) && (iter != miConfItems.end()); iter++)
	{
		wxMenuItem *mi = (*iter).second;

		if ((mi) && (event.GetId() == mi->GetId()))
		{
		    if ((*iter).first == FORCE_ADD_TREE_ITEMS)
            {
                this->ForceFillTree(true);

                return;
            }

			bool refresh = ((*iter).first != NOT_ADDING_TREE_ITEMS);

			this->SetConfiguration((*iter).first, !this->IsConfiguration((*iter).first), refresh);

			done = true;
		}
	}

	for (StringCheckBoxMap::iterator iter = this->cbConfItems.begin();
		 (!done) && (iter != cbConfItems.end()); iter++)
	{
		wxCheckBox *cb = (*iter).second;

		if ((cb) && (event.GetId() == cb->GetId()))
		{
			bool refresh = ((*iter).first != NOT_ADDING_TREE_ITEMS);

			this->SetConfiguration((*iter).first, !this->IsConfiguration((*iter).first), refresh);

			done = true;
		}
	}
}

/*void CRAbMesh::MeshModule::OnConfDrawNormals(wxCommandEvent &WXUNUSED(event))
{
	this->SetNormalDrawable(!this->IsNormalDrawable());
}

void CRAbMesh::MeshModule::OnConfDrawDiscretization(wxCommandEvent &WXUNUSED(event))
{
	this->SetDiscretizationDrawable(!this->IsDiscretizationDrawable());
}

void CRAbMesh::MeshModule::OnConfDrawBoundary(wxCommandEvent &WXUNUSED(event))
{
	this->SetBoundaryDrawable(!this->IsBoundaryDrawable());
}

void CRAbMesh::MeshModule::OnConfDrawTree(wxCommandEvent &WXUNUSED(event))
{
	this->SetTreeDrawable(!this->IsTreeDrawable());
}

void CRAbMesh::MeshModule::OnConfDrawFront(wxCommandEvent &WXUNUSED(event))
{
	this->SetFrontDrawable(!this->IsFrontDrawable());
}

void CRAbMesh::MeshModule::OnConfDrawMesh(wxCommandEvent &WXUNUSED(event))
{
	this->SetMeshDrawable(!this->IsMeshDrawable());
}

void CRAbMesh::MeshModule::OnConfDoNotAdTreeItems(wxCommandEvent &WXUNUSED(event))
{
	this->SetNotAddingTreeItems(!this->IsNotAddingTreeItems());
}*/

void CRAbMesh::MeshModule::OnTreeSelectionChanged(wxTreeEvent &event)
{
	this->canvas->Refresh(false);
	this->canvas->Update();

	return;

	wxTreeItemId oldItem = event.GetOldItem();
    wxTreeItemId currentItem = event.GetItem();

    bool changed = false;

    if (oldItem.IsOk())
    {
    	bool found = false;

    	for (StringVector::iterator iter = this->optionsMap.begin();
			 iter != this->optionsMap.end(); iter++)
		{
			std::string option = (*iter);

			if (/*(!this->options[option][IS_MENU_OR_CHECK_ITEM]) &&*/
				(this->IsEnabled(option, TREE_ITEM)) &&
				(this->tree->GetItemParent(oldItem) == this->tiItems[option]))
			{
				found = true;

				break;
			}
		}

		if (found)
		{
			static_cast<TreeItemData *>(this->tree->GetItemData(oldItem))->Unhighlight();

			changed = true;
		}
    }

    if (currentItem.IsOk())
    {
    	bool found = false;

    	for (StringVector::iterator iter = this->optionsMap.begin();
			 iter != this->optionsMap.end(); iter++)
		{
			std::string option = (*iter);

			if (/*(!this->options[option][IS_MENU_OR_CHECK_ITEM]) &&*/
				(this->IsEnabled(option, TREE_ITEM)) &&
				(this->tree->GetItemParent(currentItem) == this->tiItems[option]))
			{
				found = true;

				break;
			}
		}

		if (found)
		{
			static_cast<TreeItemData *>(this->tree->GetItemData(currentItem))->Highlight();

			changed = true;
		}
    }

    /*if ((oldItem.IsOk()) &&
		(((this->IsEnabled(VERTICES, TREE_ITEM)) && (this->tree->GetItemParent(oldItem) == this->tiItems[VERTICES])) ||
		 ((this->IsEnabled(FRONT_ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(oldItem) == this->tiItems[FRONT_ELEMENTS])) ||
		 ((this->IsEnabled(ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(oldItem) == this->tiItems[ELEMENTS])) ||
		 ((this->IsEnabled(TREE_CELLS, TREE_ITEM)) && (this->tree->GetItemParent(oldItem) == this->tiItems[TREE_CELLS]))))
    {
		static_cast<TreeItemData *>(this->tree->GetItemData(oldItem))->Unhighlight();

		changed = true;
    }

	if ((currentItem.IsOk()) &&
		(((this->IsEnabled(VERTICES, TREE_ITEM)) && (this->tree->GetItemParent(currentItem) == this->tiItems[VERTICES])) ||
		((this->IsEnabled(FRONT_ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(currentItem) == this->tiItems[FRONT_ELEMENTS])) ||
		((this->IsEnabled(ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(currentItem) == this->tiItems[ELEMENTS])) ||
		((this->IsEnabled(TREE_CELLS, TREE_ITEM)) && (this->tree->GetItemParent(currentItem) == this->tiItems[TREE_CELLS]))))
    {
		static_cast<TreeItemData *>(this->tree->GetItemData(currentItem))->Highlight();

		changed = true;
    }*/

	if (changed)
	{
		this->frame->GetViewModule()->ClearDisplayLists();

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
}

void CRAbMesh::MeshModule::OnTreeMiddleClick(wxTreeEvent &event)
{
	wxTreeItemId item = event.GetItem();

	if (!item.IsOk())
    {
        return;
    }

	if ((this->IsEnabled(VERTICES, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[VERTICES]))
	{
	    const Data::Drawable *d = static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem();

		Data::Point *p = static_cast<const Data::Vertex *>(d)->getPoint();

		this->canvas->Center(p);

		this->canvas->Refresh(false);
		this->canvas->Update();
	}
	else if (((this->IsEnabled(FRONT_ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[FRONT_ELEMENTS])) ||
			 ((this->IsEnabled(ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[ELEMENTS])) ||
			 ((this->IsEnabled(TREE_CELLS, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[TREE_CELLS])))
	{
	    const Data::Drawable *d = static_cast<TreeItemData *>(this->tree->GetItemData(item))->GetItem();

		Data::GeometricShape *s = NULL;

		if (((this->IsEnabled(FRONT_ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[FRONT_ELEMENTS])) ||
			((this->IsEnabled(ELEMENTS, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[ELEMENTS])))
		{
			s = static_cast<const Data::Element *>(d)->getShape();
		}
		else if ((this->IsEnabled(TREE_CELLS, TREE_ITEM)) && (this->tree->GetItemParent(item) == this->tiItems[TREE_CELLS]))
		{
			s = static_cast<const Data::BoxTreeCell *>(d)->shape();
		}

		if (s)
		{
			this->canvas->Center(s);

			this->canvas->Refresh(false);
			this->canvas->Update();
		}
	}
}

void CRAbMesh::MeshModule::OnTreeKeyDown(wxTreeEvent &event)
{
    //wxChar uc = event.GetKeyEvent().GetUnicodeKey();

    enum wxKeyCode code = static_cast<enum wxKeyCode>(event.GetKeyEvent().GetKeyCode());

    //if (uc != WXK_NONE)
    {
        if ((code == WXK_RETURN) || (code == WXK_NUMPAD_ENTER))
        {
            this->OnTreeMiddleClick(event);
        }
    }

    event.Skip();
}

/*void CRAbMesh::MeshModule::OnDrawingTimer(wxTimerEvent &WXUNUSED(event))
{
    if (!this->gDrawing)
    {
        return;
    }

    ULInt total = 0;
    ULInt measure = 0;

    if (this->GetTree())
    {
        total += this->GetTree()->getDrawingTotal();
        measure += this->GetTree()->getDrawingMeasure();
    }

    if (this->GetBoundary())
    {
        total += this->GetBoundary()->getDrawingTotal();
        measure += this->GetBoundary()->getDrawingMeasure();
    }

    if (this->GetFront())
    {
        total += this->GetFront()->getDrawingTotal();
        measure += this->GetFront()->getDrawingMeasure();
    }

    if (this->GetMesh())
    {
        total += this->GetMesh()->getDrawingTotal();
        measure += this->GetMesh()->getDrawingMeasure();
    }

    this->gDrawing->SetRange(total);
    this->gDrawing->SetValue((total == 0) ? 0 : measure);
}*/

double CRAbMesh::MeshModule::MakeNormalFactor() const
{
	return 1.0;

	if ((!this->IsNormalDrawable()) || (!this->drive))
	{
		return 1.0;
	}

	unsigned int dim = this->GetDimension();

	double factor = std::pow(10.0, static_cast<double>(4.0 - dim))/this->maxBoundingSize;

	if (dim == 2)
	{
		if (factor > 2.5)
		{
			factor = 2.5;
		}
		else if (factor < 0.01)
		{
			factor = 0.01;
		}
	}
	else
	{
		if (factor > std::sqrt(2.5))
		{
			factor = std::sqrt(2.5);
		}
		else if (factor < 0.1)
		{
			factor = 0.1;
		}
	}

	return factor;
}

void CRAbMesh::MeshModule::AddQualityString()
{
	wxString s = this->QualityString();

	if (s.IsEmpty())
	{
		return;
	}

	std::stringstream e;

	e << std::endl;

	wxString endl(e.str().c_str(), wxMBConvLibc());

	wxStringTokenizer tokenizer(s, endl);

	this->sbsQuality = new wxStaticBoxSizer(wxVERTICAL, this->GetPanel(0), _("Results"));

    while (tokenizer.HasMoreTokens())
    {
    	wxStaticText *stQuality = new wxStaticText(this->GetPanel(0), wxID_ANY, tokenizer.GetNextToken());

    	this->sbsQuality->Add(stQuality, 0, wxEXPAND | wxALL, 0);
    }

    wxBoxSizer *pConfSizer = static_cast<wxBoxSizer *>(this->GetPanel(0)->GetSizer());

    //pConfSizer->AddSpacer(2);
    pConfSizer->Add(this->sbsQuality, 0, wxEXPAND | wxALL, 1);

    this->GetPanel(0)->FitInside();
    this->frame->UpdateAui();
}

void CRAbMesh::MeshModule::AddCellToTree(Data::BoxTreeCell *cell)
{
    if (cell)
    {
        if (cell->isLeaf())
        {
			this->AddTreeItem(cell);
        }

        if (cell->isSubdivided())
        {
            for (unsigned int i = 0; i < cell->numChildren(); i++)
            {
            	this->AddCellToTree(static_cast<Data::BoxTreeCell *>(cell->child(i)));
            }
        }
    }
}

void CRAbMesh::MeshModule::FillTree(bool clear)
{
    if (this->IsNotAddingTreeItems())
    {
    	this->canvas->Refresh(false);

    	return;
    }

    this->ForceFillTree(clear);
}

void CRAbMesh::MeshModule::ForceFillTree(bool clear)
{
	if (clear)
	{
		this->ClearTree();
	}

	if (this->IsEnabled(VERTICES, TREE_ITEM))
	{
		if (this->GetMesh())
		{
			//Data::VertexList vertices = this->GetMesh()->getVertices(true);
			//Data::VertexList vertices = this->GetMesh()->getSortedVertices();
			Data::VertexList vertices = this->GetMesh()->getVertices();
			vertices.sort(Data::Identifiable::less);

			while (!vertices.empty())
			{
				this->AddTreeItem(vertices.front());

				vertices.pop_front();
			}
		}
		else if (this->GetBoundary())
		{
			Data::VertexList vertices = this->GetBoundary()->getVertices();

			vertices.sort(Data::Identifiable::less);

			while (!vertices.empty())
			{
				this->AddTreeItem(vertices.front());

				vertices.pop_front();
			}
		}
	}

	if (this->IsEnabled(FRONT_ELEMENTS, TREE_ITEM))
	{
		if (this->GetMesh())
		{
			//Data::FrontElementList frontElements = this->GetMesh()->getFrontElements(true);
			//Data::FrontElementList frontElements = this->GetMesh()->getSortedFrontElements();
			Data::FrontElementList frontElements = this->GetMesh()->getFrontElements();
			frontElements.sort(Data::Identifiable::less);

			while (!frontElements.empty())
			{
				this->AddTreeItem(frontElements.front());

				frontElements.pop_front();
			}
		}
		else if (this->GetBoundary())
		{
			Data::FrontElementList frontElements = this->GetBoundary()->getElements();
			frontElements.sort(Data::Identifiable::less);

			while (!frontElements.empty())
			{
				this->AddTreeItem(frontElements.front());

				frontElements.pop_front();
			}
		}
	}

	if (this->IsEnabled(ELEMENTS, TREE_ITEM))
	{
		if (this->GetMesh())
		{
			//Data::ElementList elements = this->GetMesh()->getElements(true);
			//Data::ElementList elements = this->GetMesh()->getSortedElements();
			Data::ElementList elements = this->GetMesh()->getElements();

			elements.sort(Data::Identifiable::less);

			while (!elements.empty())
			{
				this->AddTreeItem(elements.front());

				elements.pop_front();
			}
		}
	}

	if (this->IsEnabled(TREE_CELLS, TREE_ITEM))
	{
		if (this->GetTree())
		{
			Data::BoxTreeCell *cell = static_cast<Data::BoxTreeCell *>(this->GetTree()->getRoot());

			if (cell)
			{
				this->AddCellToTree(cell);
			}
		}
	}

    this->canvas->Refresh(false);
}

void CRAbMesh::MeshModule::AddTreeItem(Data::Vertex *v)
{
	if (this->IsEnabled(VERTICES, TREE_ITEM))
	{
		TreeItemData *item = new TreeItemData(v);
		this->tree->AppendItem(this->tiItems[VERTICES], item->GetText(), -1, -1, item);
	}
}

void CRAbMesh::MeshModule::AddTreeItem(Data::FrontElement *e)
{
	if (this->IsEnabled(FRONT_ELEMENTS, TREE_ITEM))
	{
		TreeItemData *item = new TreeItemData(e);
		this->tree->AppendItem(this->tiItems[FRONT_ELEMENTS], item->GetText(), -1, -1, item);
	}
}

void CRAbMesh::MeshModule::AddTreeItem(Data::Element *e)
{
	if (this->IsEnabled(ELEMENTS, TREE_ITEM))
	{
		TreeItemData *item = new TreeItemData(e);
		this->tree->AppendItem(this->tiItems[ELEMENTS], item->GetText(), -1, -1, item);
	}
}

void CRAbMesh::MeshModule::AddTreeItem(Data::BoxTreeCell *c)
{
	if (this->IsEnabled(TREE_CELLS, TREE_ITEM))
	{
		TreeItemData *item = new TreeItemData(c);
		this->tree->AppendItem(this->tiItems[TREE_CELLS], item->GetText(), -1, -1, item);
	}
}

void CRAbMesh::MeshModule::ClearTree()
{
	for (StringVector::iterator iter = this->optionsMap.begin();
		 iter != this->optionsMap.end(); iter++)
	{
		std::string option = (*iter);

		if (/*(!this->options[option][IS_MENU_OR_CHECK_ITEM]) &&*/
			(this->IsEnabled(option, TREE_ITEM)))
		{
			this->tree->DeleteChildren(this->tiItems[option]);
		}
	}
}

#endif //#if USE_GUI

