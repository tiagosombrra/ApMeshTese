#ifndef _CRAB_MESH_MESH_MODULE_H_
#define _CRAB_MESH_MESH_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

#include <wx/treectrl.h>

#if wxCHECK_VERSION(2, 9, 0)
#include <wx/gbsizer.h>
#endif //#if wxCHECK_VERSION(2, 9, 0)

#include "CRAbMesh/Module.h"

namespace CRAbMesh
{
	class MeshModule : public CRAbMesh::Module
	{
	friend class CRAbMesh::MeshesModule;
	friend class CRAbMesh::ViewModule;

	public:

		MeshModule(Frame *frame = NULL, Canvas *canvas = NULL);
		virtual ~MeshModule();

		virtual void Render() const;
		virtual void SetOpenGLStateBeforeRendering(/*const Camera *camera*/) const;
		virtual bool BoundingBox(double &minx, double &miny, double &minz, double &maxx, double &maxy, double &maxz) const;

		virtual void SetAdjacentFrontElementHighlighted(bool adjacentFrontElementHighlighted);
		virtual bool IsAdjacentFrontElementHighlighted() const;
		virtual void SetAdjacentElementHighlighted(bool adjacentElementHighlighted);
		virtual bool IsAdjacentElementHighlighted() const;

		virtual void SetNormalDrawable(bool normalDrawable);
		virtual bool IsNormalDrawable() const;
		virtual void SetDiscretizationDrawable(bool discretizationDrawable);
		virtual bool IsDiscretizationDrawable() const;
		virtual void SetBoundaryDrawable(bool boundaryDrawable);
		virtual bool IsBoundaryDrawable() const;
		virtual void SetTreeDrawable(bool treeDrawable);
		virtual bool IsTreeDrawable() const;
		virtual void SetFrontDrawable(bool frontDrawable);
		virtual bool IsFrontDrawable() const;
		virtual void SetMeshDrawable(bool meshDrawable);
		virtual bool IsMeshDrawable() const;
		virtual bool IsDrawable() const;

		virtual void SetNotAddingTreeItems(bool notAddingTreeItems);
		virtual bool IsNotAddingTreeItems() const;

		virtual void ClearDisplayLists(bool force = true) const;

		//virtual void StartGaugeTimer();
		//virtual void EndGaugeTimer();

	protected:

		enum OptionType
		{
		    UNKNOWN = 0,
			TYPE_MENU_AND_CHECK_ITEM,
			TYPE_MENU,
			TYPE_TREE_ITEM
		};

		enum ItemType
		{
		    MENU_CHECK_ITEM,
		    CHECK_BOX,
		    MENU_ITEM,
		    TREE_ITEM
		};

	protected:

		virtual void AddOption(const std::string &option, enum OptionType type = TYPE_MENU_AND_CHECK_ITEM);
		virtual void InsertOption(const std::string &option, const std::string &before, enum OptionType type = TYPE_MENU_AND_CHECK_ITEM);

		virtual void SetOption(unsigned int option, bool menuOrTree, bool check = false);
		virtual void SetOption(const std::string &option, bool menuOrTree, bool check = false);
		virtual void SetConfiguration(unsigned int option, bool value, bool refresh = true);
		virtual void SetConfiguration(const std::string &option, bool value, bool refresh = true);
		virtual bool IsConfiguration(unsigned int option) const;
		virtual bool IsConfiguration(const std::string &option) const;
		//virtual bool IsEnabled(unsigned int option, int item) const;
		virtual bool IsEnabled(unsigned int option, enum ItemType item) const;
		//virtual bool IsEnabled(const std::string &option, int item) const;
		virtual bool IsEnabled(const std::string &option, enum ItemType item) const;

		virtual bool CreateMenuItem(unsigned int option, wxMenu *menu, bool prependSeparator = false, bool appendSeparator = false);
		virtual bool CreateMenuItem(const std::string &option, wxMenu *menu, bool prependSeparator = false, bool appendSeparator = false);
		virtual bool CreateCheckBox(unsigned int option, wxPanel *panel, wxGridBagSizer *sizer, int &row, bool prependSeparator = false, bool appendSeparator = false);
		virtual bool CreateCheckBox(const std::string &option, wxPanel *panel, wxGridBagSizer *sizer, int &row, bool prependSeparator = false, bool appendSeparator = false);
		virtual bool CreateTreeItem(unsigned int option);
		virtual bool CreateTreeItem(const std::string &option);

		virtual wxMenu *MakeConfigurationMenu();
		virtual wxPanel *MakeConfigurationPanel();
		virtual wxPanel *MakeTreePanel();

		virtual void RenderPick() const;
		virtual unsigned int PickBufferSize() const;
		virtual Data::DrawableList Picked(int hits, GLuint buffer[]);

	protected:

		virtual void OnConfItem(wxCommandEvent &event);

		virtual void OnTreeSelectionChanged(wxTreeEvent &event);
		virtual void OnTreeMiddleClick(wxTreeEvent &event);
		virtual void OnTreeKeyDown(wxTreeEvent &event);

		//virtual void OnDrawingTimer(wxTimerEvent &event);

	protected:

		//virtual unsigned int GetDimension() const = 0;

		virtual void SetDimension(unsigned int dimension) = 0;
		virtual void SetTree(Data::BoxTree *tree) = 0;
		virtual const Data::BoxTree *GetTree() const = 0;
		virtual void SetBoundary(Data::Boundary *boundary) = 0;
		virtual const Data::Boundary *GetBoundary() const = 0;
		virtual void SetFront(Data::Front *front) = 0;
		virtual const Data::Front *GetFront() const = 0;
		virtual void SetMesh(Data::Mesh *mesh) = 0;
		virtual const Data::Mesh *GetMesh() const = 0;

		virtual double MakeNormalFactor() const;

		virtual wxString QualityString() const = 0;
		virtual void AddQualityString();

		virtual void AddCellToTree(Data::BoxTreeCell *cell);
		virtual void FillTree(bool clear = true);
		virtual void ForceFillTree(bool clear = true);
		virtual void AddTreeItem(Data::Vertex *v);
		virtual void AddTreeItem(Data::FrontElement *e);
		virtual void AddTreeItem(Data::Element *e);
		virtual void AddTreeItem(Data::BoxTreeCell *c);
		virtual void ClearTree();

	protected:

		/*enum
		{
			VERTICES = 0,
			FRONT_ELEMENTS,
			ELEMENTS,
			TREE_CELLS,

			TREE_ITEMS_COUNT,

			NORMAL = TREE_ITEMS_COUNT,
			DISCRETIZATION,

			BOUNDARY,
			TREE,
			FRONT,
			MESH,

			NOT_ADDING_TREE_ITEMS,

			OPTIONS_COUNT,

			IS_MENU_OR_CHECK_ITEM = 0,
			MENU_ITEM,
			CHECK_BOX,

			TREE_ITEM = MENU_ITEM,
		};*/

		std::string VERTICES;
		std::string FRONT_ELEMENTS;
		std::string ELEMENTS;
		std::string TREE_CELLS;

		std::string HIGHLIGHT_ADJACENT_FRONT_ELEMENTS;
		std::string HIGHLIGHT_ADJACENT_ELEMENTS;

		std::string DRAW_NORMAL;
		std::string DRAW_DISCRETIZATION;
		std::string DRAW_BOUNDARY;
		std::string DRAW_TREE;
		std::string DRAW_FRONT;
		std::string DRAW_MESH;
		std::string NOT_ADDING_TREE_ITEMS;
		std::string FORCE_ADD_TREE_ITEMS;

		/*enum
		{
			IS_MENU_OR_CHECK_ITEM = 0,
			MENU_ITEM,
			CHECK_BOX,

			TREE_ITEM = MENU_ITEM,
		};*/

	protected:

	    struct MenuAndCheckItemTypeOption
	    {
	        bool menu;
	        bool check;
	    };

	    struct MenuItemTypeOption
	    {
	        bool menu;
	    };

	    struct TreeItemTypeOption
	    {
	        bool tree;
	    };

	    union ItemTypeOption
	    {
	        MenuAndCheckItemTypeOption menuAndCheck;
	        MenuItemTypeOption menu;
	        TreeItemTypeOption tree;
	    };

	    struct TypeOption
	    {
	        TypeOption(enum OptionType type = UNKNOWN)
	        {
	            this->type = type;

	            this->set(false, false);
	        };

	        void set(bool menuOrTree, bool check)
	        {
	            switch (this->type)
                {
                case TYPE_MENU_AND_CHECK_ITEM:
                    this->options.menuAndCheck.menu = menuOrTree;
                    this->options.menuAndCheck.check = check;
                    break;
                case TYPE_MENU:
                    this->options.menu.menu = menuOrTree;
                    break;
                case TYPE_TREE_ITEM:
                    this->options.tree.tree = menuOrTree;
                    break;
                default:
                    break;
                }
	        };

	        bool is(enum ItemType type) const
	        {
	            return
                    (((this->type == TYPE_MENU_AND_CHECK_ITEM) && (type == MENU_CHECK_ITEM) && (this->options.menuAndCheck.menu)) ||
                     ((this->type == TYPE_MENU_AND_CHECK_ITEM) && (type == MENU_ITEM)       && (this->options.menuAndCheck.menu)) ||
                     ((this->type == TYPE_MENU_AND_CHECK_ITEM) && (type == CHECK_BOX)       && (this->options.menuAndCheck.check)) ||
                     ((this->type == TYPE_MENU)                && (type == MENU_ITEM)       && (this->options.menu.menu)) ||
                     ((this->type == TYPE_TREE_ITEM)           && (type == TREE_ITEM)       && (this->options.tree.tree)));
	        }

	        enum OptionType type;

	        union ItemTypeOption options;
	    };

		typedef std::map<std::string, struct TypeOption> StringTypeOptionMap;
		typedef std::map<std::string, bool> StringBoolMap;
		typedef std::map<std::string, wxMenuItem *> StringMenuItemMap;
		typedef std::map<std::string, wxCheckBox *> StringCheckBoxMap;
		typedef std::map<std::string, wxTreeItemId> StringTreeItemIdMap;
		typedef std::vector<std::string> StringVector;

		StringVector optionsMap;
		StringTypeOptionMap options;

		StringMenuItemMap miConfItems;

		StringCheckBoxMap cbConfItems;

		wxStaticBoxSizer *sbsQuality;
		wxStaticBoxSizer *sbsDrawOptions;
		//wxGauge *gDrawing;
		//mutable wxTimer tDrawingGauge;

		wxTreeCtrl *tree;
		wxTreeItemId tiRoot;
		StringTreeItemIdMap tiItems;

		mutable double maxBoundingSize;

	protected:
		///attributes for configuration

		StringBoolMap configuration;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_MESH_MODULE_H_
