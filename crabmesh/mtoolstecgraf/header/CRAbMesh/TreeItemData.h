#ifndef _CRAB_MESH_TREE_ITEM_DATA_H_
#define _CRAB_MESH_TREE_ITEM_DATA_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

#include <wx/treebase.h>

namespace CRAbMesh
{
	class TreeItemData : public wxTreeItemData
	{
	public:

		TreeItemData(Data::Drawable *item);
		virtual ~TreeItemData();

		virtual Data::Drawable *GetItem() const;

		virtual void Highlight();
		virtual void Unhighlight();

		virtual wxString GetText() const;

	protected:

		Data::Drawable *item;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_TREE_ITEM_DATA_H_
