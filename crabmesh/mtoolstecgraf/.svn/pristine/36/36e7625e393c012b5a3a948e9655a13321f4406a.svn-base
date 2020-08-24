#include "CRAbMesh/TreeItemData.h"

#if USE_GUI

#include "Data/Drawable.h"

CRAbMesh::TreeItemData::TreeItemData(Data::Drawable *item) :
	wxTreeItemData()
{
    this->item = item;
}

CRAbMesh::TreeItemData::~TreeItemData()
{
	//this->item = NULL;
}

Data::Drawable *CRAbMesh::TreeItemData::GetItem() const
{
    return this->item;
}

void CRAbMesh::TreeItemData::Highlight()
{
    this->item->highlight();
}

void CRAbMesh::TreeItemData::Unhighlight()
{
    this->item->unhighlight();
}

wxString CRAbMesh::TreeItemData::GetText() const
{
    std::string str = item->text();

    wxString s;

    for (unsigned int i = 0; str[i]; i++)
    {
        s = s.Append(str[i]);
    }

    return s;
}

#endif //#if USE_GUI

