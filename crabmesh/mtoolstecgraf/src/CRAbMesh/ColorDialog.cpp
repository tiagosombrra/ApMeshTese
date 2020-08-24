#include "CRAbMesh/ColorDialog.h"

#if USE_GUI

#include <wx/listbook.h>
#include <wx/notebook.h>

CRAbMesh::ColorDialog::IColorPanel::IColorPanel(wxWindow *parent, wxWindowID id) :
    wxPanel(parent, id)
{

}

CRAbMesh::ColorDialog::IColorPanel::~IColorPanel()
{

}

CRAbMesh::ColorDialog::ColorDialog(Frame *frame, wxWindow *parent) :
	wxDialog(parent, wxID_ANY, _("Colors"))
{
	this->frame = frame;

	/*wxPanel *pBGColor = new wxPanel(this, wxID_ANY);

	this->lbSchemes = new wxListBox(pBGColor, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_NEEDED_SB | wxLB_SINGLE);

	this->FillSchemes();

	this->bAdd = new wxButton(pBGColor, wxID_ANY, _("+"));
	this->bRemove = new wxButton(pBGColor, wxID_ANY, _("-"));

	this->cpc1 = new wxColourPickerCtrl(pBGColor, wxID_ANY);
	this->cpc2 = new wxColourPickerCtrl(pBGColor, wxID_ANY);
	this->cpc3 = new wxColourPickerCtrl(pBGColor, wxID_ANY);
	this->cpc4 = new wxColourPickerCtrl(pBGColor, wxID_ANY);

	this->SetScheme(this->frame->GetConfiguration().background.color);

	wxBoxSizer *bsButtons = new wxBoxSizer(wxHORIZONTAL);
	bsButtons->Add(this->bAdd);
	bsButtons->Add(this->bRemove);

	wxBoxSizer *bsScheme = new wxBoxSizer(wxVERTICAL);
	bsScheme->Add(this->lbSchemes, 1, wxEXPAND | wxALL, 0);
	bsScheme->Add(bsButtons, 0, wxALIGN_CENTER_HORIZONTAL, 0);

	wxBoxSizer *bsColor1 = new wxBoxSizer(wxHORIZONTAL);
	bsColor1->Add(this->cpc1);
	bsColor1->Add(this->cpc2);

	wxBoxSizer *bsColor2 = new wxBoxSizer(wxHORIZONTAL);
	bsColor2->Add(this->cpc3);
	bsColor2->Add(this->cpc4);

	wxBoxSizer *bsColors = new wxBoxSizer(wxVERTICAL);
	bsColors->Add(bsColor1);
	bsColors->Add(bsColor2);

	wxBoxSizer *bsPanel = new wxBoxSizer(wxHORIZONTAL);
	bsPanel->Add(bsScheme, 1, wxEXPAND | wxALL, 0);
	bsPanel->Add(bsColors, 0, wxALIGN_CENTER_VERTICAL, 0);

	pBGColor->SetSizer(bsPanel);*/

	this->nbColors = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_LEFT);

	unsigned int *bgChange = new unsigned int[4];
	bgChange[0] = 2;
	bgChange[1] = 1;
	bgChange[2] = 3;
	bgChange[3] = 0;

	this->background = new ColorPanel<4>(this->nbColors, wxID_ANY, this, this->frame, &this->frame->GetConfiguration().background, 2, bgChange);

	wxArrayString boundaryLabels;
	boundaryLabels.Add(_("Vertex"));
	boundaryLabels.Add(_("Front element"));

	this->boundary = new ColorPanel<2>(this->nbColors, wxID_ANY, this, this->frame, &this->frame->GetConfiguration().boundary, 1, NULL, boundaryLabels);

	wxArrayString meshLabels;
	meshLabels.Add(_("Vertex"));
	meshLabels.Add(_("Front element"));
	meshLabels.Add(_("Element"));

	this->mesh = new ColorPanel<3>(this->nbColors, wxID_ANY, this, this->frame, &this->frame->GetConfiguration().mesh, 1, NULL, meshLabels);

	wxArrayString qualityLabels;
	qualityLabels.Add(_("[0.0 - 0.1)"));
	qualityLabels.Add(_("[0.1 - 0.2)"));
	qualityLabels.Add(_("[0.2 - 0.3)"));
	qualityLabels.Add(_("[0.3 - 0.4)"));
	qualityLabels.Add(_("[0.4 - 0.5)"));
	qualityLabels.Add(_("[0.5 - 0.6)"));
	qualityLabels.Add(_("[0.6 - 0.7)"));
	qualityLabels.Add(_("[0.7 - 0.8)"));
	qualityLabels.Add(_("[0.8 - 0.9)"));
	qualityLabels.Add(_("[0.9 - 1.0]"));

	this->quality = new ColorPanel<10>(this->nbColors, wxID_ANY, this, this->frame, &this->frame->GetConfiguration().quality, 1, NULL, qualityLabels);

	wxArrayString frontLabels;
	frontLabels.Add(_("Vertex"));
	frontLabels.Add(_("Boundary"));
	frontLabels.Add(_("Element"));
	frontLabels.Add(_("Rejected"));
	frontLabels.Add(_("Crossing"));

	this->front = new ColorPanel<5>(this->nbColors, wxID_ANY, this, this->frame, &this->frame->GetConfiguration().front, 1, NULL, frontLabels);

	wxArrayString cellLabels;
	cellLabels.Add(_("Default"));
	cellLabels.Add(_("In cells"));
	cellLabels.Add(_("On cells"));
	cellLabels.Add(_("Out cells"));
	cellLabels.Add(_("Unknown"));

	this->cell = new ColorPanel<5>(this->nbColors, wxID_ANY, this, this->frame, &this->frame->GetConfiguration().cell, 1, NULL, cellLabels);

	this->partition = new ColorPanel<1>(this->nbColors, wxID_ANY, this, this->frame, &this->frame->GetConfiguration().partition, 1);

	this->nbColors->AddPage(this->background, _("Background"));
	this->nbColors->AddPage(this->boundary, _("Boundary"));
	this->nbColors->AddPage(this->mesh, _("Mesh"));
	this->nbColors->AddPage(this->quality, _("Mesh quality"));
	this->nbColors->AddPage(this->front, _("Front"));
	this->nbColors->AddPage(this->cell, _("Tree cells"));
	this->nbColors->AddPage(this->partition, _("Partition"));

	this->nbColors->SetSelection(0);

	wxBoxSizer *bsTop = new wxBoxSizer(wxVERTICAL);
	bsTop->Add(this->nbColors, 1, wxEXPAND | wxALL, 0);
	bsTop->Add(this->CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 0);

	this->SetSizer(bsTop);

	bsTop->Layout();
    this->FitInside();
    this->DoLayoutAdaptation();

	/*this->Connect(this->bAdd->GetId(),      wxEVT_COMMAND_BUTTON_CLICKED,        wxCommandEventHandler(ColorDialog::OnAdd));
	this->Connect(this->bRemove->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED,        wxCommandEventHandler(ColorDialog::OnRemove));
	this->Connect(this->lbSchemes->GetId(), wxEVT_COMMAND_LISTBOX_SELECTED,      wxCommandEventHandler(ColorDialog::OnScheme));
	this->Connect(this->lbSchemes->GetId(), wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(ColorDialog::OnSchemeAndEnd));*/
	this->Connect(this->nbColors->GetId(), wxEVT_NOTEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler(ColorDialog::OnPageChanged));

	this->nbColors->ChangeSelection(0);
}

CRAbMesh::ColorDialog::~ColorDialog()
{

}

/*void CRAbMesh::ColorDialog::FillSchemes()
{
	this->lbSchemes->Clear();

	for (Frame::Configuration::Background::ColorList::iterator iter = this->frame->GetConfiguration().background.colors.begin();
		 iter != this->frame->GetConfiguration().background.colors.end(); iter++)
	{
		wxString str((*iter).name.c_str(), wxMBConvLibc());

		this->lbSchemes->Append(str);
	}
}

CRAbMesh::Frame::Configuration::Background::ColorList::iterator CRAbMesh::ColorDialog::SelectedScheme() const
{
	std::string name(this->lbSchemes->GetStringSelection().mb_str(wxMBConvLibc()));

	for (Frame::Configuration::Background::ColorList::iterator iter = this->frame->GetConfiguration().background.colors.begin();
		 iter != this->frame->GetConfiguration().background.colors.end(); iter++)
	{
		if ((*iter).name == name)
		{
			return iter;
		}
	}

	return this->frame->GetConfiguration().background.colors.end();
}*/

wxColor CRAbMesh::ColorDialog::Color(const CRAbMesh::Frame::Configuration::Color &color)
{
    unsigned char r = static_cast<unsigned char >(color.r*255.0);
    unsigned char g = static_cast<unsigned char >(color.g*255.0);
    unsigned char b = static_cast<unsigned char >(color.b*255.0);

    return wxColor(r, g, b);
}

CRAbMesh::Frame::Configuration::Color CRAbMesh::ColorDialog::Color(const wxColor &color)
{
	Frame::Configuration::Color c;

	c.r = static_cast<double>(color.Red())/255.0;
	c.g = static_cast<double>(color.Green())/255.0;
	c.b = static_cast<double>(color.Blue())/255.0;

	return c;
}

void CRAbMesh::ColorDialog::OnPageChanged(wxBookCtrlEvent& WXUNUSED(event))
{
    IColorPanel *cpanel = static_cast<IColorPanel *>(this->nbColors->GetCurrentPage());

    if (!cpanel)
    {
        return;
    }

    cpanel->ResetScheme();
}

/*void CRAbMesh::ColorDialog::SetScheme(Frame::Configuration::Background::Color &scheme)
{
	this->cpc1->SetColour(this->Color(scheme[2]));
	this->cpc2->SetColour(this->Color(scheme[1]));
	this->cpc3->SetColour(this->Color(scheme[3]));
	this->cpc4->SetColour(this->Color(scheme[0]));
}

CRAbMesh::Frame::Configuration::Background::Color CRAbMesh::ColorDialog::GetScheme() const
{
	Frame::Configuration::Background::Color sc;

	sc[2] = this->Color(this->cpc1->GetColour());
	sc[1] = this->Color(this->cpc2->GetColour());
	sc[3] = this->Color(this->cpc3->GetColour());
	sc[0] = this->Color(this->cpc4->GetColour());

	return sc;
}

void CRAbMesh::ColorDialog::OnAdd(wxCommandEvent &WXUNUSED(event))
{
	wxString s = wxGetTextFromUser(_("Name for color scheme (one word only)"), _("Adding scheme"), _("Name"), this);

	if (s.IsEmpty())
	{
		return;
	}

	std::string name(s.mb_str(wxMBConvLibc()));

	Frame::Configuration::Background::Color sc = this->GetScheme();

	sc.name = name;

	this->frame->GetConfiguration().background.colors.push_back(sc);

	this->frame->SaveConfiguration();

	this->FillSchemes();
}

void CRAbMesh::ColorDialog::OnRemove(wxCommandEvent &WXUNUSED(event))
{
	int ret = wxMessageBox(_("Are you sure you want to remove the scheme?"), _("Removing scheme"), wxYES | wxNO |  wxICON_QUESTION, this);

	if (ret == wxYES)
	{
		Frame::Configuration::Background::ColorList::iterator iter = this->SelectedScheme();

		if (iter != this->frame->GetConfiguration().background.colors.end())
		{
			this->frame->GetConfiguration().background.colors.erase(iter);

			this->frame->SaveConfiguration();

			this->FillSchemes();
		}
	}
}

void CRAbMesh::ColorDialog::OnScheme(wxCommandEvent &WXUNUSED(event))
{
	Frame::Configuration::Background::ColorList::iterator iter = this->SelectedScheme();

	if (iter != this->frame->GetConfiguration().background.colors.end())
	{
		this->SetScheme((*iter));
	}
}

void CRAbMesh::ColorDialog::OnSchemeAndEnd(wxCommandEvent &event)
{
	this->OnScheme(event);

	this->EndModal(wxID_OK);
}*/

#endif //#if USE_GUI
