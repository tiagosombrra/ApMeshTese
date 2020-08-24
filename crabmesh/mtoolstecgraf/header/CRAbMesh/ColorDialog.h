#ifndef _CRAB_MESH_COLOR_DIALOG_H_
#define _CRAB_MESH_COLOR_DIALOG_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

#include "CRAbMesh/Frame.h"
#include <wx/clrpicker.h>
#include <wx/bmpcbox.h>

namespace CRAbMesh
{
	class ColorDialog : public wxDialog
	{
    friend
        class ViewModule;

    public:

        class IColorPanel : public wxPanel
        {
        public:

            IColorPanel(wxWindow *parent, wxWindowID id);
            virtual ~IColorPanel();

            virtual void ResetScheme() = 0;
        };

        template <unsigned int ncolors>
        class ColorPanel : public IColorPanel
        {
        public:

            typedef CRAbMesh::Frame::Configuration::ColorSchemeType<ncolors> ColorSchemeType;

        public:

            ColorPanel(wxWindow *parent, wxWindowID id, ColorDialog *dialog, Frame *frame, ColorSchemeType *type,
                unsigned int ncols = 1, unsigned int *change = NULL, const wxArrayString &titles = wxArrayString());
            virtual ~ColorPanel();

            virtual void SetScheme(typename ColorSchemeType::Color &scheme);
            virtual typename ColorSchemeType::Color GetScheme() const;

            virtual void ResetScheme();

            virtual void OnAdd(wxCommandEvent &event);
            virtual void OnRemove(wxCommandEvent &event);
            virtual void OnScheme(wxCommandEvent &event);
            virtual void OnSchemeAndEnd(wxCommandEvent &event);

        protected:

            virtual void FillSchemes();
            typename ColorSchemeType::ColorList::iterator SelectedScheme() const;

        protected:

            ColorDialog *dialog;
            Frame *frame;

            wxListBox *lbSchemes;

            wxButton *bAdd;
            wxButton *bRemove;

            wxColourPickerCtrl *cpcs[ncolors];

            ColorSchemeType *type;
            unsigned int *change;
        };

	public:

		ColorDialog(Frame *frame, wxWindow *parent);
		virtual ~ColorDialog();

		static wxColor Color(const Frame::Configuration::Color &color);
		static Frame::Configuration::Color Color(const wxColor &color);

    public:

        virtual void OnPageChanged(wxBookCtrlEvent& event);

	protected:

		Frame *frame;

		ColorPanel<4> *background;
		ColorPanel<2> *boundary;
		ColorPanel<3> *mesh;
		ColorPanel<10> *quality;
		ColorPanel<5> *front;
		ColorPanel<5> *cell;
		ColorPanel<1> *partition;

		wxNotebook *nbColors;
	};

template <unsigned int ncolors>
ColorDialog::ColorPanel<ncolors>::ColorPanel(wxWindow *parent, wxWindowID id, ColorDialog *dialog, Frame *frame, ColorSchemeType *type,
    unsigned int ncols, unsigned int *change, const wxArrayString &titles) :
        IColorPanel(parent, id)
{
    this->type = type;
    this->change = change;
    this->dialog = dialog;
    this->frame = frame;

    this->lbSchemes = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_NEEDED_SB | wxLB_SINGLE);

	this->FillSchemes();

	this->bAdd = new wxButton(this, wxID_ANY, _("+"));
	this->bRemove = new wxButton(this, wxID_ANY, _("-"));

	unsigned int count = 0;

	if (ncols <= 0)
    {
        ncols = 1;
    }

	unsigned int total = this->type->color.nColors();

	unsigned int nrows = total/ncols;

	if (nrows*ncols < total)
    {
        nrows += 1;
    }

	for (unsigned int i = 0; (i < nrows) && (count < total); i++)
    {
        for (unsigned int j = 0; (j < ncols) && (count < total); j++)
        {
            this->cpcs[i*ncols + j] = new wxColourPickerCtrl(this, wxID_ANY);

            count++;
        }
    }

	unsigned int ntitles = titles.GetCount() >= ncols ? ncols : static_cast<unsigned int>(titles.GetCount());

	this->SetScheme(this->type->color);

	wxBoxSizer *bsButtons = new wxBoxSizer(wxHORIZONTAL);
	bsButtons->Add(this->bAdd, 1, wxEXPAND | wxALL, 0);
	bsButtons->Add(this->bRemove, 1, wxEXPAND | wxALL, 0);

	wxBoxSizer *bsScheme = new wxBoxSizer(wxVERTICAL);
	bsScheme->Add(this->lbSchemes, 1, wxEXPAND | wxALL, 0);
	bsScheme->Add(bsButtons, 0, wxALIGN_CENTER_HORIZONTAL, 0);

	count = 0;

	wxFlexGridSizer *fgsColors = new wxFlexGridSizer(ncols + ntitles);

	for (unsigned int i = 0; (i < nrows) && (count < total); i++)
    {
        for (unsigned int j = 0; (j < ncols) && (count < total); j++)
        {
            if (titles.GetCount() > count)
            {
                wxString title = titles[count];

                wxStaticText *st = new wxStaticText(this, wxID_ANY, title);

                fgsColors->Add(st, 1, wxALIGN_CENTER_VERTICAL);
            }
            else if (!titles.IsEmpty())
            {
                fgsColors->AddStretchSpacer();
            }

            fgsColors->Add(this->cpcs[i*ncols + j]);

            count++;
        }
    }

    for (unsigned int i = 0; i < ntitles; i++)
    {
        fgsColors->AddGrowableCol(2*i);
    }

	wxBoxSizer *bsPanel = new wxBoxSizer(wxHORIZONTAL);
	bsPanel->Add(bsScheme, 1, wxEXPAND | wxALL, 0);
	bsPanel->Add(fgsColors, 0, wxALIGN_CENTER_VERTICAL, 0);

	this->SetSizer(bsPanel);

	this->Connect(this->bAdd->GetId(),      wxEVT_COMMAND_BUTTON_CLICKED,        wxCommandEventHandler(ColorDialog::ColorPanel<ncolors>::OnAdd));
	this->Connect(this->bRemove->GetId(),   wxEVT_COMMAND_BUTTON_CLICKED,        wxCommandEventHandler(ColorDialog::ColorPanel<ncolors>::OnRemove));
	this->Connect(this->lbSchemes->GetId(), wxEVT_COMMAND_LISTBOX_SELECTED,      wxCommandEventHandler(ColorDialog::ColorPanel<ncolors>::OnScheme));
	//this->Connect(this->lbSchemes->GetId(), wxEVT_LISTBOX,      wxCommandEventHandler(ColorDialog::ColorPanel<ncolors>::OnScheme));
	this->Connect(this->lbSchemes->GetId(), wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler(ColorDialog::ColorPanel<ncolors>::OnSchemeAndEnd));
	//this->Connect(this->lbSchemes->GetId(), wxEVT_LISTBOX_DCLICK, wxCommandEventHandler(ColorDialog::ColorPanel<ncolors>::OnSchemeAndEnd));
}

template <unsigned int ncolors>
ColorDialog::ColorPanel<ncolors>::~ColorPanel()
{
    if (this->change)
    {
        delete [] this->change;
    }
}

template <unsigned int ncolors>
void ColorDialog::ColorPanel<ncolors>::FillSchemes()
{
	this->lbSchemes->Clear();

	for (typename ColorSchemeType::ColorList::iterator iter = this->type->colors.begin();
		 iter != this->type->colors.end(); iter++)
	{
		wxString str((*iter).name.c_str(), wxMBConvLibc());

		this->lbSchemes->Append(str);
	}

	this->lbSchemes->SetSelection(wxNOT_FOUND);
}

template <unsigned int ncolors>
typename ColorDialog::ColorPanel<ncolors>::ColorSchemeType::ColorList::iterator ColorDialog::ColorPanel<ncolors>::SelectedScheme() const
{
	std::string name(this->lbSchemes->GetStringSelection().mb_str(wxMBConvLibc()));

	for (typename ColorSchemeType::ColorList::iterator iter = this->type->colors.begin();
		 iter != this->type->colors.end(); iter++)
	{
		if ((*iter).name == name)
		{
			return iter;
		}
	}

	return this->type->colors.end();
}

template <unsigned int ncolors>
void ColorDialog::ColorPanel<ncolors>::SetScheme(typename ColorSchemeType::Color &scheme)
{
    for (unsigned int i = 0; i < this->type->color.nColors(); i++)
    {
        this->cpcs[i]->SetColour(ColorDialog::Color(scheme[this->change ? this->change[i] : i]));
    }
}

template <unsigned int ncolors>
typename ColorDialog::ColorPanel<ncolors>::ColorSchemeType::Color ColorDialog::ColorPanel<ncolors>::GetScheme() const
{
	typename ColorSchemeType::Color sc;

	for (unsigned int i = 0; i < this->type->color.nColors(); i++)
    {
        sc[this->change ? this->change[i] : i] = ColorDialog::Color(this->cpcs[i]->GetColour());
    }

	return sc;
}

template <unsigned int ncolors>
void ColorDialog::ColorPanel<ncolors>::ResetScheme()
{
    this->lbSchemes->SetSelection(wxNOT_FOUND);
    this->SetScheme(this->type->color);
}

template <unsigned int ncolors>
void ColorDialog::ColorPanel<ncolors>::OnAdd(wxCommandEvent &WXUNUSED(event))
{
	wxString s = wxGetTextFromUser(_("Name for color scheme (one word only)"), _("Adding scheme"), _("Name"), this);

	if (s.IsEmpty())
	{
		return;
	}

	std::string name(s.mb_str(wxMBConvLibc()));

	typename ColorSchemeType::Color sc = this->GetScheme();

	sc.name = name;

	type->colors.push_back(sc);

	this->frame->SaveConfiguration();

	this->FillSchemes();
}

template <unsigned int ncolors>
void ColorDialog::ColorPanel<ncolors>::OnRemove(wxCommandEvent &WXUNUSED(event))
{
	int ret = wxMessageBox(_("Are you sure you want to remove the scheme?"), _("Removing scheme"), wxYES | wxNO |  wxICON_QUESTION, this);

	if (ret == wxYES)
	{
		typename ColorSchemeType::ColorList::iterator iter = this->SelectedScheme();

		if (iter != this->type->colors.end())
		{
			this->type->colors.erase(iter);

			this->frame->SaveConfiguration();

			this->FillSchemes();
		}
	}
}

template <unsigned int ncolors>
void ColorDialog::ColorPanel<ncolors>::OnScheme(wxCommandEvent &WXUNUSED(event))
{
	typename ColorSchemeType::ColorList::iterator iter = this->SelectedScheme();

	if (iter != this->type->colors.end())
	{
		this->SetScheme((*iter));
	}
}

template <unsigned int ncolors>
void ColorDialog::ColorPanel<ncolors>::OnSchemeAndEnd(wxCommandEvent &event)
{
	this->OnScheme(event);

	dialog->EndModal(wxID_OK);
}

}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_COLOR_DIALOG_H_
