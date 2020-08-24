#include "CRAbMesh/Frame.h"

#if USE_GUI

#include <wx/aboutdlg.h>
#include <wx/filename.h>
#include <wx/splash.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include "Data/State.h"
#include "CRAbMesh/App.h"
#include "CRAbMesh/Canvas.h"
#include "CRAbMesh/ViewModule.h"
#include "CRAbMesh/MeshesModule.h"
#include "CRAbMesh/SplinesModule.h"
#include "CRAbMesh/MJMeshModule.h"
#include "CRAbMesh/TemplateMJMeshModule.h"
#include "CRAbMesh/JmeshModule.h"
#if ((USE_NUMERICAL) && (USE_EXPRTK))
#include "CRAbMesh/NumericalModule.h"
#endif //#if ((USE_NUMERICAL) && (USE_EXPRTK))
#if USE_MTOOLS
#include "CRAbMesh/MToolsModule.h"
#endif //#if USE_MTOOLS
#if USE_TECGRAF_MSH_2D
#include "CRAbMesh/TecGrafMsh2DModule.h"
#endif //#if USE_TECGRAF_MSH_2D
#if USE_TECGRAF_MSH_3D
//#include "CRAbMesh/TecGrafMsh3DModule.h"
#endif //#if USE_TECGRAF_MSH_3D
#if USE_TECGRAF_MSH_SURF
#include "CRAbMesh/TecGrafMshSurfModule.h"
#endif //#if USE_TECGRAF_MSH_SURF
#include "CRAbMesh/PMJMeshModule.h"
#include "CRAbMesh/PJmeshModule.h"
#if USE_MTOOLS
#include "CRAbMesh/PMToolsModule.h"
#endif //#if USE_MTOOLS
#include "CRAbMesh/PMJMesh2Module.h"
#if USE_MTOOLS
#include "CRAbMesh/PMTools2Module.h"
#endif //#if USE_MTOOLS

#if USE_MPI
#include <mpi.h>
#endif //#if USE_MPI

#include "crab4_png.cpp"

using namespace CRAbMesh;

DECLARE_APP(CRAbMesh::App)

/*BEGIN_EVENT_TABLE(Frame, wxFrame)
    //EVT_AUI_PANE_CLOSE(Frame::OnAuiPaneClose)
END_EVENT_TABLE()*/

CRAbMesh::Frame::Configuration::Configuration()
{
	this->background.color.name = "";

	for (UInt i = 0; i < this->background.color.nColors(); i++)
	{
		this->background.color[i].r = this->background.color[i].g = this->background.color[i].b = 0.0;
	}

	this->boundary.color.name = "";

	for (UInt i = 0; i < this->boundary.color.nColors(); i++)
	{
		this->boundary.color[i].r = this->boundary.color[i].g = this->boundary.color[i].b = 0.0;
	}

	this->mesh.color.name = "";

	for (UInt i = 0; i < this->mesh.color.nColors(); i++)
	{
		this->mesh.color[i].r = this->mesh.color[i].g = this->mesh.color[i].b = 0.0;
	}

	this->quality.color.name = "";

	for (UInt i = 0; i < this->quality.color.nColors(); i++)
	{
		this->quality.color[i].r = this->quality.color[i].g = this->quality.color[i].b = 0.0;
	}

	this->front.color.name = "";

	for (UInt i = 0; i < this->front.color.nColors(); i++)
	{
		this->front.color[i].r = this->front.color[i].g = this->front.color[i].b = 0.0;
	}

	this->cell.color.name = "";

	for (UInt i = 0; i < this->cell.color.nColors(); i++)
	{
		this->cell.color[i].r = this->cell.color[i].g = this->cell.color[i].b = 0.0;
	}

	this->partition.color.name = "";

	for (UInt i = 0; i < this->partition.color.nColors(); i++)
	{
		this->partition.color[i].r = this->partition.color[i].g = this->partition.color[i].b = 0.0;
	}
}

CRAbMesh::Frame::Frame() :
    wxFrame(NULL, wxID_ANY, _("CRAb Mesh"), wxDefaultPosition, wxDefaultSize,
    wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL)
{
    static const bool useSplash = false;

    if (useSplash)
    {
        //wxImage splashImg(_("../../images/crab4.png"));
        wxImage splashImg = IMAGE_PNG_FROM_DATA(crab4);

        wxSplashScreen *splash = new wxSplashScreen(wxBitmap(splashImg),
            wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT, 3000, NULL, wxID_ANY,
            wxDefaultPosition, wxDefaultSize,
            wxBORDER_NONE | wxSTAY_ON_TOP | wxFRAME_NO_TASKBAR);

#if wxCHECK_VERSION(2, 9, 5)
        splash->SetClientSize(splashImg.GetSize().GetWidth(), splashImg.GetSize().GetHeight());
#elif wxCHECK_VERSION(2, 9, 0)
        splash->SetClientSize(splashImg.GetSize().GetWidth(), splashImg.GetSize().GetHeight() + 20);
#endif //#if wxCHECK_VERSION(2, 9, 5)
    }

    wxYield();

    //menu bar
    this->menuBar = new wxMenuBar(wxMB_DOCKABLE);

    wxMenu *menuFile = new wxMenu();
    wxMenu *menuAbout = new wxMenu();

    //this->miFileOpen = menuFile->Append(ID_MENU_ITEM_FILE_OPEN, _("&Open...\tCtrl+O"));
    this->miFileOpen = menuFile->Append(wxID_OPEN, _("&Open...\tCtrl+O"));
    //this->miFileSave = menuFile->Append(ID_MENU_ITEM_FILE_SAVE, _("&Save...\tCtrl+S"));
    this->miFileSave = menuFile->Append(wxID_SAVE, _("&Save...\tCtrl+S"));
    //this->miFileExport = menuFile->Append(ID_MENU_ITEM_FILE_EXPORT, _("E&xport...\tCtrl+S"));
    this->miFileExport = menuFile->Append(wxID_SAVEAS, _("&Export image...\tCtrl+E"));
#if USE_OPENCV
    this->miFileVideo = menuFile->AppendCheckItem(wxID_ANY, _("&Record video...\tCtrl+R"));
#endif //#if USE_OPENCV
    menuFile->AppendSeparator();
    this->miFileSaveCamera = menuFile->Append(wxID_ANY, _("Save camera...\tCtrl+Shift+S"));
    this->miFileOpenCamera = menuFile->Append(wxID_ANY, _("Open camera...\tCtrl+Shift+O"));
    menuFile->AppendSeparator();
    //this->miFileExit = menuFile->Append(ID_MENU_ITEM_FILE_EXIT, _("&Exit\tAlt+F4"));
    this->miFileExit = menuFile->Append(wxID_EXIT, _("E&xit\tAlt+F4"));

    //this->miAboutAbout = menuAbout->Append(ID_MENU_ITEM_ABOUT_ABOUT, _("&About...\tF1"));
    this->miAboutAbout = menuAbout->Append(wxID_ABOUT, _("&About...\tF1"));

    this->miFileOpen->SetHelp(_("Open an input file"));
    this->miFileSave->SetHelp(_("Save an input or an output file"));
    this->miFileExport->SetHelp(_("Export image file"));
#if USE_OPENCV
    this->miFileVideo->SetHelp(_("Save a video file"));
#endif //#if USE_OPENCV
    this->miFileSaveCamera->SetHelp(_("Save camera position"));
    this->miFileOpenCamera->SetHelp(_("Open camera position"));
    this->miFileExit->SetHelp(_("Exit program"));
    this->miAboutAbout->SetHelp(_("About this program"));

    this->menuBar->Append(menuFile, _("&File"));
    this->menuBar->Append(menuAbout, _("&About"));

    this->SetMenuBar(this->menuBar);

    //status bar
    this->statusBar = new wxStatusBar(this, wxID_ANY);
    //this->statusBar->SetFieldsCount(3);
    this->statusBar->SetFieldsCount(2);
    this->SetStatusBar(this->statusBar);

    //frame content
    this->aui.SetManagedWindow(this);

    this->pCanvas = new wxPanel(this);

    //Canvas
    this->canvas = new Canvas(this, this->pCanvas);

    wxBoxSizer *bsCanvas = new wxBoxSizer(wxHORIZONTAL);
    bsCanvas->Add(this->canvas, 1, wxEXPAND | wxALL, 2);
    this->pCanvas->SetSizer(bsCanvas);

    //Modules
    int mod = 0;
    this->modules[moduleNames[mod++]] = new ViewModule   (this, canvas);
#if (USE_ONLY_MTOOLS == 2)
    this->modules[moduleNames[mod++]] = new MToolsModule (this, canvas);
#elif (USE_ONLY_MTOOLS == 3) //#if (USE_ONLY_MTOOLS == 2)
    this->modules[moduleNames[mod++]] = new PMToolsModule(this, canvas);
#else
#if (!USE_ONLY_MTOOLS)
    this->modules[moduleNames[mod++]] = new MeshesModule (this, canvas);
#if USE_SPLINES
    this->modules[moduleNames[mod++]] = new SplinesModule(this, canvas);
#endif //#if USE_SPLINES
    this->modules[moduleNames[mod++]] = new MJMeshModule (this, canvas);
    this->modules[moduleNames[mod++]] = new TemplateMJMeshModule (this, canvas);
    this->modules[moduleNames[mod++]] = new JmeshModule  (this, canvas);
#endif //#if (!USE_ONLY_MTOOLS)
#if USE_MTOOLS
    this->modules[moduleNames[mod++]] = new MToolsModule (this, canvas);
#endif //#if USE_MTOOLS
//#if USE_MPI
#if (!USE_ONLY_MTOOLS)
#if USE_TECGRAF_MSH_2D
    this->modules[moduleNames[mod++]] = new TecGrafMsh2DModule(this, canvas);
#endif //#if USE_TECGRAF_MSH_2D
#if USE_TECGRAF_MSH_3D
    this->modules[moduleNames[mod++]] = NULL;//new TecGrafMsh3DModule(this, canvas);
#endif //#if USE_TECGRAF_MSH_3D
#if USE_TECGRAF_MSH_SURF
    this->modules[moduleNames[mod++]] = new TecGrafMshSurfModule(this, canvas);
#endif //#if USE_TECGRAF_MSH_SURF
    this->modules[moduleNames[mod++]] = new PMJMeshModule(this, canvas);
    this->modules[moduleNames[mod++]] = new PJmeshModule (this, canvas);
#endif //#if (!USE_ONLY_MTOOLS)
#if USE_MTOOLS
    this->modules[moduleNames[mod++]] = new PMToolsModule(this, canvas);
#endif //#if USE_MTOOLS
#if (!USE_ONLY_MTOOLS)
    this->modules[moduleNames[mod++]] = new PMJMesh2Module(this, canvas);
#endif //#if (!USE_ONLY_MTOOLS)
#if USE_MTOOLS
    this->modules[moduleNames[mod++]] = new PMTools2Module(this, canvas);
#endif //#if USE_MTOOLS
//#endif //#if USE_MPI
#endif //#if (USE_ONLY_MTOOLS == 3)

#if (!USE_ONLY_MTOOLS)
    this->modules[moduleNames[mod++]] = NULL;//new CMeshModule(this, canvas);
#if ((USE_NUMERICAL) && (USE_EXPRTK))
    this->modules[moduleNames[mod++]] = new NumericalModule(this, canvas);
#endif //#if ((USE_NUMERICAL) && (USE_EXPRTK))
#endif //#if (!USE_ONLY_MTOOLS)

    for (unsigned int j = 1; j < MODULES_COUNT; j++)
    {
        Module *module = this->modules[moduleNames[j]];

        if (!module)
        {
            continue;
        }

        for (unsigned int i = 0; i < module->GetPanelCount(); i++)
        {
            wxString title;
            wxPanel *p = module->GetPanel(i, title);

            this->aui.AddPane(p,
                wxAuiPaneInfo().
                Caption(title).
                Position(i/2).
                MinSize(p->GetSize()).
                CloseButton(false).
                Hide());

#if (!USE_ONLY_MTOOLS)
            if ((j == 1) && (i == 2))
            {
                this->aui.GetPane(p).Bottom().Position(0);
            }
            else
#endif //#if (!USE_ONLY_MTOOLS)
                if ((i%2) == 0)
            {
                this->aui.GetPane(p).Right();
            }
            else
            {
                this->aui.GetPane(p).Left();
            }
        }

        for (unsigned int i = 0; i < module->GetToolbarCount(); i++)
        {
            wxString title = _("");
            wxAuiToolBar *t = module->GetToolbar(i, title);

            this->aui.AddPane(t,
                wxAuiPaneInfo().
                ToolbarPane().
                Caption(title).
                CloseButton(false).
                Top().
                Hide().
                MinSize(t->GetSize()).
                MaxSize(t->GetSize()).
                BestSize(t->GetSize()).
                FloatingSize(t->GetSize()));
        }
    }

    this->currentModule = NULL;

    for (unsigned int i = 0; i < this->GetViewModule()->GetMenuCount(); i++)
    {
        wxString title;
        this->menuBar->Insert(i + 1, this->GetViewModule()->GetMenu(i, title), title);
    }

    wxString title;
    /*this->menuBar->Insert(1, this->GetViewModule()->GetModulesMenu(title), title);
    this->menuBar->Insert(2, this->GetViewModule()->GetMouseMenu(title), title);
    this->menuBar->Insert(3, this->GetViewModule()->GetViewMenu(title), title);*/

    wxAuiToolBar *tbMesh = this->GetViewModule()->GetMeshToolbar(title);

    this->aui.AddPane(tbMesh,
        wxAuiPaneInfo().
        ToolbarPane().
        Caption(title).
        CloseButton(false).
        Top().
        MinSize(tbMesh->GetSize()).
        MaxSize(tbMesh->GetSize()).
        BestSize(tbMesh->GetSize()).
        FloatingSize(tbMesh->GetSize()));
    //this->SetToolBar(this->GetViewModule()->GetMeshToolbar(title));

    this->PushEventHandler(this->GetViewModule());
    this->GetViewModule()->SetHandler(true);

    //Frame
    this->aui.AddPane(this->pCanvas,
        wxAuiPaneInfo().
        Caption(_("Drawing canvas")).
        CloseButton(false).
        Center());

    this->SetMinSize(wxSize(640, 480));

    //wxImage img(_("../../images/crab4.png"));
    wxImage img = IMAGE_PNG_FROM_DATA(crab4);
    img.Rescale(100, 76);

    wxBitmap bmp(img);

    wxIcon icon;
    icon.CopyFromBitmap(bmp);

    this->SetIcon(icon);

    //Outros
    this->canvas->Clear();

    this->Maximize(true);

    this->UpdateAui();

    this->configurationFile = _("CRAbMesh.conf");

    this->originalTitle = this->GetTitle();
    this->moduleTitle = _("");

    this->openPath = _("./../../../../samples/");
    this->openWildcard <<
        _("JMesh Files (*.2djmsh, *.3djmsh, *.dmsh, *.jmsh, *.wmsh, *gen)|*.2djmsh;*.3djmsh;*.dmsh;*.jmsh;*.wmsh;*.gen|") <<
        _("2D JMesh Files (*.2djmsh, *.gen)|*.2djmsh;*.gen|") <<
        _("3D JMesh Files (*.3djmsh, *.dmsh, *.jmsh, *.wmsh, *.gen)|*.3djmsh;*.dmsh;*.jmsh;*.wmsh;*.gen|") <<
        _("Generic JMesh Files (*.gen)|*.gen|") <<
        _("Grain Files (*.stl, *.vtk)|*.stl;*.vtk|") <<
        _("Analysis Files (*.cdb)|*.cdb|") <<
        _("Abaqus Files (*.inp)|*.inp|") <<
#if USE_TINYXML
        _("XML Mesh Files (*.xml)|*.xml|") <<
#endif //#if USE_TINYXML
        _("Gmsh Mesh Files (*.msh)|*.msh|") <<
        _("Other Mesh Files (*.off)|*.off|") <<
        _("Text Files (*.txt)|*.txt|") <<
        _("Neutral Files (*.dat, *.pos)|*.dat;*.pos|") <<
        _("All Files (*.*)|*.*");

    {
        wxStringTokenizer tokenizer(this->openWildcard, _("|"));

        if (tokenizer.CountTokens() > 2)
        {
            wxString s = _("All Readable Files (");

            wxArrayString allExtensions;

            while (tokenizer.HasMoreTokens())
            {
                /*wxString name = */tokenizer.GetNextToken();
                wxString extensions = tokenizer.GetNextToken();

                wxStringTokenizer extTokenizer(extensions, _(";"));

                while (extTokenizer.HasMoreTokens())
                {
                    wxString extension = extTokenizer.GetNextToken();

                    if (extension == _("*.*"))
                    {
                        continue;
                    }

                    if (allExtensions.Index(extension) == wxNOT_FOUND)
                    {
                        allExtensions.Add(extension);
                    }
                }
            }

            wxString exts;

            for (size_t i = 0; i < allExtensions.GetCount(); i++)
            {
                wxString ext = allExtensions.Item(i);

                if (i > 0)
                {
                    s << _(", ");
                    exts << _(";");
                }

                s << ext;
                exts << ext;
            }

            s << _(")|");
            s << exts;
            s << _("|");

            wxString oldWildcard = this->openWildcard;

            this->openWildcard.Clear();

            this->openWildcard << s;
            this->openWildcard << oldWildcard;
        }
    }

    this->savePath = _("../../../../samples");
    this->saveWildcard << _("2D JMesh Files (*.2djmsh)|*.2djmsh|") <<
        _("3D JMesh Files (*.3djmsh)|*.3djmsh|") <<
        _("Generic JMesh Files (*.gen)|*.gen|") <<
        _("Boundary Generic JMesh Files (*.gen)|*.gen|") <<
        _("Boundary as Mesh Generic JMesh Files (*.gen)|*.gen|") <<
        _("Boundary as Surface Generic JMesh Files (*.gen)|*.gen|") <<
        _("Mesh Generic JMesh Files (*.gen)|*.gen|") <<
        _("Mesh as Boundary Generic JMesh Files (*.gen)|*.gen|") <<
        _("Mesh as Surface Generic JMesh Files (*.gen)|*.gen|") <<
        _("Dmsh Files (*.dmsh)|*.dmsh|") <<
        _("Jmsh Files (*.jmsh)|*.jmsh|") <<
        _("Wmsh Files (*.wmsh)|*.wmsh|") <<
        _("Neutral Files (*.dat)|*.dat|") <<
        _("Abaqus Files (*.inp)|*.inp|") <<
        _("Text Files (*.txt)|*.txt|") <<
        _("All Files (*.*)|*.*");

    this->canvas->Refresh(false);
    this->canvas->Update();

    this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(Frame::OnClose));
    this->Connect(this->miFileOpen->GetId(),       wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnMenuOpen));
    this->Connect(this->miFileSave->GetId(),       wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnMenuSave));
    this->Connect(this->miFileExport->GetId(),     wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnMenuExport));
#if USE_OPENCV
    this->Connect(this->miFileVideo->GetId(),      wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnMenuVideo));
#endif //#if USE_OPENCV
    this->Connect(this->miFileSaveCamera->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnMenuSaveCamera));
    this->Connect(this->miFileOpenCamera->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnMenuOpenCamera));
    this->Connect(this->miFileExit->GetId(),       wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnMenuExit));
    this->Connect(this->miAboutAbout->GetId(),     wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnMenuAbout));

    /*if (splash)
    {
        delete splash;
    }*/
}

CRAbMesh::Frame::~Frame()
{
    this->Clear(true);

    this->currentModule = NULL;

    for (ModuleMap::iterator iter = this->modules.begin();
         iter != this->modules.end(); iter++)
    {
        Module *module = (*iter).second;

        if (!module)
        {
            continue;
        }

        module->Clear(true, true);

        for (unsigned int j = 0; j < module->GetMenuCount(); j++)
        {
            for (unsigned int i = 0; i < this->menuBar->GetMenuCount(); i++)
            {
                if (this->menuBar->GetMenu(i) == module->GetMenu(j))
                {
                    this->menuBar->Remove(i);

                    break;
                }
            }
        }

        for (unsigned int j = 0; j < module->GetToolbarCount(); j++)
        {
            this->aui.DetachPane(module->GetToolbar(j));
        }

        for (unsigned int j = 0; j < module->GetPanelCount(); j++)
        {
            this->aui.DetachPane(module->GetPanel(j));
        }

        if (module->IsHandler())
        {
            this->RemoveEventHandler(module);
            module->SetHandler(false);
        }

        delete module;
    }

    this->modules.clear();

    this->UpdateAui();

    this->aui.UnInit();

    CRAbMesh::Redirector::RemoveFiles();
}

const wxString &CRAbMesh::Frame::GetOpenPath() const
{
	return this->openPath;
}

const wxString &CRAbMesh::Frame::GetOpenWildcard() const
{
	return this->openWildcard;
}

std::string CRAbMesh::Frame::DetermineGenSaveExtension(int index) const
{
    wxArrayString tokens = wxStringTokenize(this->saveWildcard, _("|"));

    wxString type = tokens[2*index];

    int generic = type.Find(_("Generic"));

    if (generic == wxNOT_FOUND)
    {
        int found = type.Find(_("2D"));

        if (found != wxNOT_FOUND)
        {
            return ".2djmsh";
        }

        found = type.Find(_("3D"));

        if (found != wxNOT_FOUND)
        {
            return ".3djmsh";
        }

        found = type.Find(_("Dmsh"));

        if (found != wxNOT_FOUND)
        {
            return ".dmsh";
        }

        found = type.Find(_("Dmsh"));

        if (found != wxNOT_FOUND)
        {
            return ".dmsh";
        }

        found = type.Find(_("Jmsh"));

        if (found != wxNOT_FOUND)
        {
            return ".jmsh";
        }

        found = type.Find(_("Wmsh"));

        if (found != wxNOT_FOUND)
        {
            return ".wmsh";
        }

        found = type.Find(_("Neutral"));

        if (found != wxNOT_FOUND)
        {
            return ".dat";
        }

        found = type.Find(_("Abaqus"));

        if (found != wxNOT_FOUND)
        {
            return ".inp";
        }

        found = type.Find(_("Text"));

        if (found != wxNOT_FOUND)
        {
            return ".txt";
        }

        return "";
    }

    int boundary = type.Find(_("Boundary"));
    int mesh = type.Find(_("Mesh"));
    int surface = type.Find(_("Surface"));

    if (((mesh == wxNOT_FOUND) || (mesh > generic)) &&
        (boundary == wxNOT_FOUND) && (surface == wxNOT_FOUND))
    {
        return ".gen";
    }

    if (surface != wxNOT_FOUND)
    {
        if (boundary != wxNOT_FOUND)
        {
            return ".bsgen";
        }

        return ".msgen";
    }

    if ((mesh == wxNOT_FOUND) || (mesh > generic))
    {
        return ".bbgen";
    }

    if (boundary == wxNOT_FOUND)
    {
        return ".mvgen";
    }

    if (boundary < mesh)
    {
        return ".bvgen";
    }

    return ".mbgen";
}

std::string CRAbMesh::Frame::DetermineGenSaveExtension(const wxFileDialog *save) const
{
    return this->DetermineGenSaveExtension(save->GetFilterIndex());
}

void CRAbMesh::Frame::OnClose(wxCloseEvent &WXUNUSED(event))
{
    this->Close();
}

/*void CRAbMesh::Frame::OnAuiPaneClose(wxAuiManagerEvent &event)
{
    if (event.pane->window == this->pTree)
    {
        this->conf->setWindowTreeEnabled(false);
    }
    else if (event.pane->window == this->pConf)
    {
        this->conf->setWindowConfEnabled(false);
    }
}*/

void CRAbMesh::Frame::OnMenuOpen(wxCommandEvent &WXUNUSED(event))
{
    MeshesModule *meshes = dynamic_cast<MeshesModule *>(this->GetCurrentModule());

    if (meshes)
    {
        meshes->OpenFiles();

        return;
    }

    wxFileDialog *open = new wxFileDialog(this, _("Open file"), this->openPath, _(""),
        this->openWildcard,
#if wxCHECK_VERSION(2, 9, 0)
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
#else
        wxOPEN | wxFILE_MUST_EXIST);
#endif //#if wxCHECK_VERSION(2, 9, 0)

    if (open->ShowModal() == wxID_OK)
    {
        this->OpenFile(open->GetPath());

        this->openPath = open->GetPath().BeforeLast(wxFileName::GetPathSeparator());
    }

    delete open;
}

void CRAbMesh::Frame::OnMenuSave(wxCommandEvent &WXUNUSED(event))
{
    MeshesModule *meshes = dynamic_cast<MeshesModule *>(this->GetCurrentModule());

    if (meshes)
    {
        meshes->SaveFiles();

        return;
    }

    wxString name = _("");

    wxFileDialog *save = new wxFileDialog(this, _("Save file"), this->savePath, name,
        this->saveWildcard,
#if wxCHECK_VERSION(2, 9, 0)
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
#else
        wxSAVE | wxOVERWRITE_PROMPT);
#endif //#if wxCHECK_VERSION(2, 9, 0)

    if (save->ShowModal() == wxID_OK)
    {
        std::string ext = this->DetermineGenSaveExtension(save);

        std::string outputFile(save->GetPath().mb_str(wxMBConvLibc()));

        if (!ext.empty())
        {
            std::stringstream str;

            str << outputFile << ext;

            outputFile = str.str();
        }

        if (this->currentModule)
        {
            this->currentModule->SaveFile(outputFile);
        }

        this->savePath = save->GetPath().BeforeLast(wxFileName::GetPathSeparator());
    }

    delete save;
}

void CRAbMesh::Frame::OnMenuExport(wxCommandEvent &WXUNUSED(event))
{
    wxString wildcard = _("");
    wxString name = _("");
    static wxString path = _("../../../../samples");

    wildcard << _("PNG Files (*.png)|*.png");

    wxFileDialog *save = new wxFileDialog(this, _("Export image file"), path, name,
        wildcard,
#if wxCHECK_VERSION(2, 9, 0)
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
#else
        wxSAVE | wxOVERWRITE_PROMPT);
#endif //#if wxCHECK_VERSION(2, 9, 0)

    if (save->ShowModal() == wxID_OK)
    {
        wxString path = save->GetPath();

        if (!path.AfterLast('.').IsSameAs(_("png"), false))
        {
            if (static_cast<char>(path[path.Length() - 1]) != '.')
            {
                path << _(".");
            }

            path << _("png");
        }

        /*unsigned long int pointsPerPixel = 2;

        wxString val;

        val << pointsPerPixel;

        wxGetTextFromUser(_("How many points per pixel?"), _("Points per pixel"), val, this).ToULong(&pointsPerPixel);

        if (pointsPerPixel != 0)*/
        {
            std::string outputFile(path.mb_str(wxMBConvLibc()));

            this->canvas->SaveScreenshot(outputFile/*, static_cast<int>(pointsPerPixel)*/);
        }

        path = save->GetPath().BeforeLast(wxFileName::GetPathSeparator());
    }

    delete save;
}

#if USE_OPENCV
void CRAbMesh::Frame::OnMenuVideo(wxCommandEvent &event)
{
    if (event.IsChecked())
    {
        wxString wildcard = _("");
        wxString name = _("");
        static wxString path = _("/tmp");

        wildcard << _("AVI files (*.avi)|*.avi");

        wxFileDialog *save = new wxFileDialog(this, _("Record video file"), path, name,
            wildcard,
#if wxCHECK_VERSION(2, 9, 0)
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
#else
            wxSAVE | wxOVERWRITE_PROMPT);
#endif //#if wxCHECK_VERSION(2, 9, 0)

        if (save->ShowModal() == wxID_OK)
        {
            wxString path = save->GetPath();

            if (!path.AfterLast('.').IsSameAs(_("avi"), false))
            {
                if (static_cast<char>(path[path.Length() - 1]) != '.')
                {
                    path << _(".");
                }

                path << _("avi");
            }

            /*unsigned long int fps = 30;

            wxString val;

            val << fps;

            wxGetTextFromUser(_("How many frames per second?"), _("Frames per second"), val, this).ToULong(&fps);

            if (fps != 0)*/
            {
                std::string outputFile(path.mb_str(wxMBConvLibc()));

                this->canvas->SaveVideo(outputFile/*, static_cast<int>(fps)*/);
            }

            path = save->GetPath().BeforeLast(wxFileName::GetPathSeparator());
        }

        delete save;
    }
    else
    {
        this->canvas->StopVideo();
    }
}
#endif //#if USE_OPENCV

void CRAbMesh::Frame::OnMenuSaveCamera(wxCommandEvent &WXUNUSED(event))
{
    wxString name = _("");

    wxFileDialog *save = new wxFileDialog(this, _("Save camera"), this->savePath, name,
        _("Camera Files (*.camera)|*.camera"),
#if wxCHECK_VERSION(2, 9, 0)
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
#else
        wxSAVE | wxOVERWRITE_PROMPT);
#endif //#if wxCHECK_VERSION(2, 9, 0)

    if (save->ShowModal() == wxID_OK)
    {
        std::string outputFile(save->GetPath().mb_str(wxMBConvLibc()));

        if (this->currentModule)
        {
            this->currentModule->SaveCamera(outputFile);
        }
    }

    delete save;
}

void CRAbMesh::Frame::OnMenuOpenCamera(wxCommandEvent &WXUNUSED(event))
{
    wxFileDialog *open = new wxFileDialog(this, _("Open camera"), this->openPath, _(""),
        _("Camera Files (*.camera)|*.camera"),
#if wxCHECK_VERSION(2, 9, 0)
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
#else
        wxOPEN | wxFILE_MUST_EXIST);
#endif //#if wxCHECK_VERSION(2, 9, 0)

    if (open->ShowModal() == wxID_OK)
    {
        std::string inputFile(open->GetPath().mb_str(wxMBConvLibc()));

        bool opened = false;

        if (this->currentModule)
        {
            opened = this->currentModule->LoadCamera(inputFile);
        }

        if (opened)
        {
            this->canvas->Refresh(false);
        }
    }

    delete open;
}

void CRAbMesh::Frame::OnMenuExit(wxCommandEvent &WXUNUSED(event))
{
    this->Close();
}

void CRAbMesh::Frame::OnMenuAbout(wxCommandEvent &WXUNUSED(event))
{
    //wxImage img(_("../../images/crab4.png"));
    wxImage img = IMAGE_PNG_FROM_DATA(crab4);
    img.Rescale(100, 76);

    wxBitmap bmp(img);

    wxIcon icon;
    icon.CopyFromBitmap(bmp);

    wxAboutDialogInfo info;
    info.SetName(_("CRAb Mesh"));
    info.SetVersion(_("0.7"));
    info.SetDescription(_("CRAb Mesh - CRAb mesh generation tool"));
    info.SetCopyright(_("CRAb - Research group on Computer graphics, virtual Reality and Animation\nDepartment of Computing- Federal University of Ceará\nFortaleza - Ceará - Brazil"));
    info.AddDeveloper(_("Ph.D. Markos Oliveira Freitas - markos@lia.ufc.br"));
    info.AddDeveloper(_("M.Sc. Daniel Nascimento Teixeira - dant25@gmail.com"));
    info.AddDeveloper(_("Ph.D. Joaquim Bento Cavalcante-Neto - joaquimb@lia.ufc.br"));
    info.AddDeveloper(_("Ph.D. Creto Augusto Vidal - cvidal@lia.ufc.br"));
    info.AddDeveloper(_("Ph.D. Paul A. Wawrzynek"));
    info.AddDeveloper(_("Ph.D. Anthony R. Ingraffea"));
    info.SetIcon(icon);

    wxAboutBox(info);
}

void CRAbMesh::Frame::LoadConfiguration()
{
	wxTextFile conf(this->configurationFile);

	if (!conf.Exists())
	{
		this->CreateConfiguration();
	}
	else
	{
		conf.Open();

		for (wxString str = conf.GetFirstLine(); !conf.Eof(); str = conf.GetNextLine())
		{
			wxStringTokenizer tokzr(str);

			wxString first = tokzr.GetNextToken();

			if (first == _("background"))
			{
				this->configuration.background.color.name = "";

				for (UInt i = 0; i < this->configuration.background.color.nColors(); i++)
				{
					tokzr.GetNextToken().ToDouble(&this->configuration.background.color[i].r);
					tokzr.GetNextToken().ToDouble(&this->configuration.background.color[i].g);
					tokzr.GetNextToken().ToDouble(&this->configuration.background.color[i].b);
				}
			}
			else if (first == _("backgrounds"))
			{
				unsigned long int numColors = 0;

				tokzr.GetNextToken().ToULong(&numColors);

				for (unsigned long int i = 0; i < numColors; i++)
				{
					str = conf.GetNextLine();

					wxStringTokenizer tokzr(str);

					Configuration::Background::Color scheme;

					scheme.name = tokzr.GetNextToken().mb_str(wxMBConvLibc());

					for (UInt j = 0; j < this->configuration.background.color.nColors(); j++)
					{
						tokzr.GetNextToken().ToDouble(&scheme[j].r);
						tokzr.GetNextToken().ToDouble(&scheme[j].g);
						tokzr.GetNextToken().ToDouble(&scheme[j].b);
					}

					this->configuration.background.colors.push_back(scheme);
				}
			}
			else if (first == _("boundary"))
            {
                this->configuration.boundary.color.name = "";

				for (UInt i = 0; i < this->configuration.boundary.color.nColors(); i++)
				{
					tokzr.GetNextToken().ToDouble(&this->configuration.boundary.color[i].r);
					tokzr.GetNextToken().ToDouble(&this->configuration.boundary.color[i].g);
					tokzr.GetNextToken().ToDouble(&this->configuration.boundary.color[i].b);
				}
            }
			else if (first == _("boundarys"))
			{
				unsigned long int numColors = 0;

				tokzr.GetNextToken().ToULong(&numColors);

				for (unsigned long int i = 0; i < numColors; i++)
				{
					str = conf.GetNextLine();

					wxStringTokenizer tokzr(str);

					Configuration::Boundary::Color scheme;

					scheme.name = tokzr.GetNextToken().mb_str(wxMBConvLibc());

					for (UInt j = 0; j < this->configuration.boundary.color.nColors(); j++)
					{
						tokzr.GetNextToken().ToDouble(&scheme[j].r);
						tokzr.GetNextToken().ToDouble(&scheme[j].g);
						tokzr.GetNextToken().ToDouble(&scheme[j].b);
					}

					this->configuration.boundary.colors.push_back(scheme);
				}
			}
			else if (first == _("mesh"))
            {
                this->configuration.mesh.color.name = "";

				for (UInt i = 0; i < this->configuration.mesh.color.nColors(); i++)
				{
					tokzr.GetNextToken().ToDouble(&this->configuration.mesh.color[i].r);
					tokzr.GetNextToken().ToDouble(&this->configuration.mesh.color[i].g);
					tokzr.GetNextToken().ToDouble(&this->configuration.mesh.color[i].b);
				}
            }
			else if (first == _("meshs"))
			{
				unsigned long int numColors = 0;

				tokzr.GetNextToken().ToULong(&numColors);

				for (unsigned long int i = 0; i < numColors; i++)
				{
					str = conf.GetNextLine();

					wxStringTokenizer tokzr(str);

					Configuration::Mesh::Color scheme;

					scheme.name = tokzr.GetNextToken().mb_str(wxMBConvLibc());

					for (UInt j = 0; j < this->configuration.mesh.color.nColors(); j++)
					{
						tokzr.GetNextToken().ToDouble(&scheme[j].r);
						tokzr.GetNextToken().ToDouble(&scheme[j].g);
						tokzr.GetNextToken().ToDouble(&scheme[j].b);
					}

					this->configuration.mesh.colors.push_back(scheme);
				}
			}
			else if (first == _("quality"))
            {
                this->configuration.quality.color.name = "";

				for (UInt i = 0; i < this->configuration.quality.color.nColors(); i++)
				{
					tokzr.GetNextToken().ToDouble(&this->configuration.quality.color[i].r);
					tokzr.GetNextToken().ToDouble(&this->configuration.quality.color[i].g);
					tokzr.GetNextToken().ToDouble(&this->configuration.quality.color[i].b);
				}
            }
			else if (first == _("qualitys"))
			{
				unsigned long int numColors = 0;

				tokzr.GetNextToken().ToULong(&numColors);

				for (unsigned long int i = 0; i < numColors; i++)
				{
					str = conf.GetNextLine();

					wxStringTokenizer tokzr(str);

					Configuration::MeshQuality::Color scheme;

					scheme.name = tokzr.GetNextToken().mb_str(wxMBConvLibc());

					for (UInt j = 0; j < this->configuration.quality.color.nColors(); j++)
					{
						tokzr.GetNextToken().ToDouble(&scheme[j].r);
						tokzr.GetNextToken().ToDouble(&scheme[j].g);
						tokzr.GetNextToken().ToDouble(&scheme[j].b);
					}

					this->configuration.quality.colors.push_back(scheme);
				}
			}
			else if (first == _("front"))
            {
                this->configuration.front.color.name = "";

				for (UInt i = 0; i < this->configuration.front.color.nColors(); i++)
				{
					tokzr.GetNextToken().ToDouble(&this->configuration.front.color[i].r);
					tokzr.GetNextToken().ToDouble(&this->configuration.front.color[i].g);
					tokzr.GetNextToken().ToDouble(&this->configuration.front.color[i].b);
				}
            }
			else if (first == _("fronts"))
			{
				unsigned long int numColors = 0;

				tokzr.GetNextToken().ToULong(&numColors);

				for (unsigned long int i = 0; i < numColors; i++)
				{
					str = conf.GetNextLine();

					wxStringTokenizer tokzr(str);

					Configuration::Front::Color scheme;

					scheme.name = tokzr.GetNextToken().mb_str(wxMBConvLibc());

					for (UInt j = 0; j < this->configuration.front.color.nColors(); j++)
					{
						tokzr.GetNextToken().ToDouble(&scheme[j].r);
						tokzr.GetNextToken().ToDouble(&scheme[j].g);
						tokzr.GetNextToken().ToDouble(&scheme[j].b);
					}

					this->configuration.front.colors.push_back(scheme);
				}
			}
			else if (first == _("treecell"))
            {
                this->configuration.cell.color.name = "";

				for (UInt i = 0; i < this->configuration.cell.color.nColors(); i++)
				{
					tokzr.GetNextToken().ToDouble(&this->configuration.cell.color[i].r);
					tokzr.GetNextToken().ToDouble(&this->configuration.cell.color[i].g);
					tokzr.GetNextToken().ToDouble(&this->configuration.cell.color[i].b);
				}
            }
			else if (first == _("treecells"))
			{
				unsigned long int numColors = 0;

				tokzr.GetNextToken().ToULong(&numColors);

				for (unsigned long int i = 0; i < numColors; i++)
				{
					str = conf.GetNextLine();

					wxStringTokenizer tokzr(str);

					Configuration::TreeCell::Color scheme;

					scheme.name = tokzr.GetNextToken().mb_str(wxMBConvLibc());

					for (UInt j = 0; j < this->configuration.cell.color.nColors(); j++)
					{
						tokzr.GetNextToken().ToDouble(&scheme[j].r);
						tokzr.GetNextToken().ToDouble(&scheme[j].g);
						tokzr.GetNextToken().ToDouble(&scheme[j].b);
					}

					this->configuration.cell.colors.push_back(scheme);
				}
			}
			else if (first == _("partition"))
            {
                this->configuration.partition.color.name = "";

				for (UInt i = 0; i < this->configuration.partition.color.nColors(); i++)
				{
					tokzr.GetNextToken().ToDouble(&this->configuration.partition.color[i].r);
					tokzr.GetNextToken().ToDouble(&this->configuration.partition.color[i].g);
					tokzr.GetNextToken().ToDouble(&this->configuration.partition.color[i].b);
				}
            }
			else if (first == _("partitions"))
			{
				unsigned long int numColors = 0;

				tokzr.GetNextToken().ToULong(&numColors);

				for (unsigned long int i = 0; i < numColors; i++)
				{
					str = conf.GetNextLine();

					wxStringTokenizer tokzr(str);

					Configuration::Partition::Color scheme;

					scheme.name = tokzr.GetNextToken().mb_str(wxMBConvLibc());

					for (UInt j = 0; j < this->configuration.partition.color.nColors(); j++)
					{
						tokzr.GetNextToken().ToDouble(&scheme[j].r);
						tokzr.GetNextToken().ToDouble(&scheme[j].g);
						tokzr.GetNextToken().ToDouble(&scheme[j].b);
					}

					this->configuration.partition.colors.push_back(scheme);
				}
			}
		}
	}

	this->ApplyConfiguration(false);
}

void CRAbMesh::Frame::CreateConfiguration()
{
    //for the background color
    {
        Configuration::Background::Color s;

        s.name = "Black";
        s[0].r = 0.0; s[0].g = 0.0; s[0].b = 0.0;
        s[1].r = 0.0; s[1].g = 0.0; s[1].b = 0.0;
        s[2].r = 0.0; s[2].g = 0.0; s[2].b = 0.0;
        s[3].r = 0.0; s[3].g = 0.0; s[3].b = 0.0;
        this->configuration.background.colors.push_back(s);

        s.name = "White";
        s[0].r = 1.0; s[0].g = 1.0; s[0].b = 1.0;
        s[1].r = 1.0; s[1].g = 1.0; s[1].b = 1.0;
        s[2].r = 1.0; s[2].g = 1.0; s[2].b = 1.0;
        s[3].r = 1.0; s[3].g = 1.0; s[3].b = 1.0;
        this->configuration.background.colors.push_back(s);

        s.name = "CRAb";
        s[0].r = 0.0; s[0].g = 0.0; s[0].b = 1.0;
        s[1].r = 1.0; s[1].g = 1.0; s[1].b = 1.0;
        s[2].r = 0.0; s[2].g = 0.0; s[2].b = 1.0;
        s[3].r = 1.0; s[3].g = 1.0; s[3].b = 1.0;
        this->configuration.background.colors.push_back(s);

        s.name = "Brazil";
        s[0].r = 1.0 ; s[0].g = 0.8 ; s[0].b = 0.16;
        s[1].r = 0.24; s[1].g = 0.25; s[1].b = 0.58;
        s[2].r = 1.0 ; s[2].g = 0.8 ; s[2].b = 0.16;
        s[3].r = 0.0 ; s[3].g = 0.65; s[3].b = 0.35;
        this->configuration.background.colors.push_back(s);

        s.name = "Grey";
        s[0].r = 0.5; s[0].g = 0.5; s[0].b = 0.5;
        s[1].r = 0.5; s[1].g = 0.5; s[1].b = 0.5;
        s[2].r = 0.5; s[2].g = 0.5; s[2].b = 0.5;
        s[3].r = 0.5; s[3].g = 0.5; s[3].b = 0.5;
        this->configuration.background.colors.push_back(s);

        this->configuration.background.color = this->configuration.background.colors.front();
    }

	//for the boundary color
	{
        Configuration::Boundary::Color s;

        s.name = "Default";
        s[0].r = 1.0; s[0].g = 1.0; s[0].b = 0.0;
        s[1].r = 1.0; s[1].g = 1.0; s[1].b = 0.0;
        this->configuration.boundary.colors.push_back(s);

        s.name = "Greyscale";
        s[0].r = 0.0; s[0].g = 0.0; s[0].b = 0.0;
        s[1].r = 0.0; s[1].g = 0.0; s[1].b = 0.0;
        this->configuration.boundary.colors.push_back(s);

        this->configuration.boundary.color = this->configuration.boundary.colors.front();
	}

	//for the mesh color
	{
        Configuration::Mesh::Color s;

        s.name = "Default";
        s[0].r = 1.0; s[0].g = 1.0; s[0].b = 1.0;
        s[1].r = 1.0; s[1].g = 1.0; s[1].b = 1.0;
        s[2].r = 1.0; s[2].g = 1.0; s[2].b = 1.0;
        this->configuration.mesh.colors.push_back(s);

        s.name = "Greyscale";
        s[0].r = 0.0; s[0].g = 0.0; s[0].b = 0.0;
        s[1].r = 0.0; s[1].g = 0.0; s[1].b = 0.0;
        s[2].r = 0.0; s[2].g = 0.0; s[2].b = 0.0;
        this->configuration.mesh.colors.push_back(s);

        this->configuration.mesh.color = this->configuration.mesh.colors.front();
	}

	//for the mesh quality color
	{
        Configuration::MeshQuality::Color s;

        s.name = "Default";
        s[0].r = 1.0; s[0].g = 0.0; s[0].b = 0.0;
        s[1].r = 1.0; s[1].g = 0.5; s[1].b = 0.0;
        s[2].r = 1.0; s[2].g = 1.0; s[2].b = 0.0;
        s[3].r = 0.5; s[3].g = 1.0; s[3].b = 0.0;
        s[4].r = 0.25; s[4].g = 1.0; s[4].b = 0.0;
        s[5].r = 0.0; s[5].g = 1.0; s[5].b = 0.25;
        s[6].r = 0.0; s[6].g = 1.0; s[6].b = 0.5;
        s[7].r = 0.0; s[7].g = 1.0; s[7].b = 1.0;
        s[8].r = 0.0; s[8].g = 0.5; s[8].b = 1.0;
        s[9].r = 0.0; s[9].g = 0.0; s[9].b = 1.0;
        this->configuration.quality.colors.push_back(s);

        s.name = "Greyscale";
        s[0].r = 0.1; s[0].g = 0.1; s[0].b = 0.1;
        s[1].r = 0.2; s[1].g = 0.2; s[1].b = 0.2;
        s[2].r = 0.3; s[2].g = 0.3; s[2].b = 0.3;
        s[3].r = 0.4; s[3].g = 0.4; s[3].b = 0.4;
        s[4].r = 0.5; s[4].g = 0.5; s[4].b = 0.5;
        s[5].r = 0.6; s[5].g = 0.6; s[5].b = 0.6;
        s[6].r = 0.7; s[6].g = 0.7; s[6].b = 0.7;
        s[7].r = 0.8; s[7].g = 0.8; s[7].b = 0.8;
        s[8].r = 0.9; s[8].g = 0.9; s[8].b = 0.9;
        s[9].r = 1.0; s[9].g = 1.0; s[9].b = 1.0;
        this->configuration.quality.colors.push_back(s);

        this->configuration.quality.color = this->configuration.quality.colors.front();
	}

	//for the front color
	{
        Configuration::Front::Color s;

        s.name = "Default";
        s[0].r = 0.0; s[0].g = 0.0; s[0].b = 1.0;
        s[1].r = 0.5; s[1].g = 1.0; s[1].b = 0.5;
        s[2].r = 0.0; s[2].g = 0.0; s[2].b = 1.0;
        s[3].r = 1.0; s[3].g = 0.5; s[3].b = 0.5;
        s[4].r = 0.5; s[4].g = 0.5; s[4].b = 1.0;
        this->configuration.front.colors.push_back(s);

        s.name = "Greyscale";
        s[0].r = 0.0; s[0].g = 0.0; s[0].b = 0.0;
        s[1].r = 0.0; s[1].g = 0.0; s[1].b = 0.0;
        s[2].r = 0.0; s[2].g = 0.0; s[2].b = 0.0;
        s[3].r = 0.0; s[3].g = 0.0; s[3].b = 0.0;
        s[4].r = 0.0; s[4].g = 0.0; s[4].b = 0.0;
        this->configuration.front.colors.push_back(s);

        this->configuration.front.color = this->configuration.front.colors.front();
	}

	//for the tree cell color
	{
        Configuration::TreeCell::Color s;

        s.name = "Default";
        s[0].r = 1.0; s[0].g = 1.0; s[0].b = 1.0;
        s[1].r = 0.0; s[1].g = 1.0; s[1].b = 0.0;
        s[2].r = 1.0; s[2].g = 1.0; s[2].b = 0.0;
        s[3].r = 1.0; s[3].g = 0.0; s[3].b = 0.0;
        s[4].r = 0.0; s[4].g = 0.0; s[4].b = 1.0;
        this->configuration.cell.colors.push_back(s);

        s.name = "Greyscale";
        s[0].r = 0.0; s[0].g = 0.0; s[0].b = 0.0;
        s[1].r = 0.0; s[1].g = 0.0; s[1].b = 0.0;
        s[2].r = 0.5; s[2].g = 0.5; s[2].b = 0.5;
        s[3].r = 1.0; s[3].g = 1.0; s[3].b = 1.0;
        s[4].r = 0.25; s[4].g = 0.25; s[4].b = 0.25;
        this->configuration.cell.colors.push_back(s);

        this->configuration.cell.color = this->configuration.cell.colors.front();
	}

	//for the partition color
	{
        Configuration::Partition::Color s;

        s.name = "Default";
        s[0].r = 1.0; s[0].g = 1.0; s[0].b = 1.0;
        this->configuration.partition.colors.push_back(s);

        s.name = "Greyscale";
        s[0].r = 0.0; s[0].g = 0.0; s[0].b = 0.0;
        this->configuration.partition.colors.push_back(s);

        this->configuration.partition.color = this->configuration.partition.colors.front();
	}
}

void CRAbMesh::Frame::ApplyConfiguration(bool onlyBackground)
{
	this->SaveConfiguration();

	if (!onlyBackground)
    {
        for (unsigned int j = 0; j < MODULES_COUNT; j++)
        {
            Module *module = this->modules[moduleNames[j]];

            if (!module)
            {
                continue;
            }

            const Data::State *state = this->GetViewModule()->GetState(moduleNames[j]);

            if (!state)
            {
                continue;
            }

            state->setTypeColor(Data::State::BOUNDARY_VERTEX,        this->configuration.boundary.color[0].r, this->configuration.boundary.color[0].g, this->configuration.boundary.color[0].b);
            state->setTypeColor(Data::State::BOUNDARY_FRONT_ELEMENT, this->configuration.boundary.color[1].r, this->configuration.boundary.color[1].g, this->configuration.boundary.color[1].b);

            state->setTypeColor(Data::State::MESH_VERTEX,        this->configuration.mesh.color[0].r, this->configuration.mesh.color[0].g, this->configuration.mesh.color[0].b);
            state->setTypeColor(Data::State::MESH_FRONT_ELEMENT, this->configuration.mesh.color[1].r, this->configuration.mesh.color[1].g, this->configuration.mesh.color[1].b);
            state->setTypeColor(Data::State::MESH_ELEMENT,       this->configuration.mesh.color[2].r, this->configuration.mesh.color[2].g, this->configuration.mesh.color[2].b);

            state->setTypeColor(Data::State::MESH_ELEMENT_QUALITY_0, this->configuration.quality.color[0].r, this->configuration.quality.color[0].g, this->configuration.quality.color[0].b);
            state->setTypeColor(Data::State::MESH_ELEMENT_QUALITY_1, this->configuration.quality.color[1].r, this->configuration.quality.color[1].g, this->configuration.quality.color[1].b);
            state->setTypeColor(Data::State::MESH_ELEMENT_QUALITY_2, this->configuration.quality.color[2].r, this->configuration.quality.color[2].g, this->configuration.quality.color[2].b);
            state->setTypeColor(Data::State::MESH_ELEMENT_QUALITY_3, this->configuration.quality.color[3].r, this->configuration.quality.color[3].g, this->configuration.quality.color[3].b);
            state->setTypeColor(Data::State::MESH_ELEMENT_QUALITY_4, this->configuration.quality.color[4].r, this->configuration.quality.color[4].g, this->configuration.quality.color[4].b);
            state->setTypeColor(Data::State::MESH_ELEMENT_QUALITY_5, this->configuration.quality.color[5].r, this->configuration.quality.color[5].g, this->configuration.quality.color[5].b);
            state->setTypeColor(Data::State::MESH_ELEMENT_QUALITY_6, this->configuration.quality.color[6].r, this->configuration.quality.color[6].g, this->configuration.quality.color[6].b);
            state->setTypeColor(Data::State::MESH_ELEMENT_QUALITY_7, this->configuration.quality.color[7].r, this->configuration.quality.color[7].g, this->configuration.quality.color[7].b);
            state->setTypeColor(Data::State::MESH_ELEMENT_QUALITY_8, this->configuration.quality.color[8].r, this->configuration.quality.color[8].g, this->configuration.quality.color[8].b);
            state->setTypeColor(Data::State::MESH_ELEMENT_QUALITY_9, this->configuration.quality.color[9].r, this->configuration.quality.color[9].g, this->configuration.quality.color[9].b);

            state->setTypeColor(Data::State::FRONT_VERTEX,                 this->configuration.front.color[0].r, this->configuration.front.color[0].g, this->configuration.front.color[0].b);
            state->setTypeColor(Data::State::FRONT_BOUNDARY_FRONT_ELEMENT, this->configuration.front.color[1].r, this->configuration.front.color[1].g, this->configuration.front.color[1].b);
            state->setTypeColor(Data::State::FRONT_ELEMENT_FRONT_ELEMENT,  this->configuration.front.color[2].r, this->configuration.front.color[2].g, this->configuration.front.color[2].b);
            state->setTypeColor(Data::State::FRONT_REJECTED_FRONT_ELEMENT, this->configuration.front.color[3].r, this->configuration.front.color[3].g, this->configuration.front.color[3].b);
            state->setTypeColor(Data::State::FRONT_CROSSING_FRONT_ELEMENT, this->configuration.front.color[4].r, this->configuration.front.color[4].g, this->configuration.front.color[4].b);

            state->setTypeColor(Data::State::TREE_CELL_DEFAULT, this->configuration.cell.color[0].r, this->configuration.cell.color[0].g, this->configuration.cell.color[0].b);
            state->setTypeColor(Data::State::TREE_CELL_IN,      this->configuration.cell.color[1].r, this->configuration.cell.color[1].g, this->configuration.cell.color[1].b);
            state->setTypeColor(Data::State::TREE_CELL_ON,      this->configuration.cell.color[2].r, this->configuration.cell.color[2].g, this->configuration.cell.color[2].b);
            state->setTypeColor(Data::State::TREE_CELL_OUT,     this->configuration.cell.color[3].r, this->configuration.cell.color[3].g, this->configuration.cell.color[3].b);
            state->setTypeColor(Data::State::TREE_CELL_UNKNOWN, this->configuration.cell.color[4].r, this->configuration.cell.color[4].g, this->configuration.cell.color[4].b);

            state->setTypeColor(Data::State::PARTITION, this->configuration.partition.color[0].r, this->configuration.partition.color[0].g, this->configuration.partition.color[0].b);

            module->ClearDisplayLists(true);

            const_cast<Data::State *>(state)->clearDisplayLists();

            if (((!this->GetCurrentModule()) && (module == this->GetViewModule())) ||
                (module == this->GetCurrentModule()))
            {
                Data::setState(*state);
            }
        }
    }

	this->canvas->Refresh(false);
	this->canvas->Update();
}

void CRAbMesh::Frame::SaveConfiguration()
{
#if USE_MPI
    if (MPI::COMM_WORLD.Get_rank() != 0)
    {
        return;
    }
#endif //#if USE_MPI

	wxTextFile conf(this->configurationFile);

	if ((!conf.Create()) && (!conf.Open()))
	{
		return;
	}

	conf.Clear();

	wxString str;

	//for the background color
	{
	    str << _("background");

        for (UInt i = 0; i < this->configuration.background.color.nColors(); i++)
        {
            str << _(" ") << this->configuration.background.color[i].r;
            str << _(" ") << this->configuration.background.color[i].g;
            str << _(" ") << this->configuration.background.color[i].b;
        }

        conf.AddLine(str);

        str = _("");

        str << _("backgrounds ") << this->configuration.background.colors.size();

        conf.AddLine(str);

        str = _("");

        for (Configuration::Background::ColorList::iterator iter = this->configuration.background.colors.begin();
             iter != this->configuration.background.colors.end(); iter++)
        {
            str << wxString((*iter).name.c_str(), wxMBConvLibc());

            for (UInt i = 0; i < this->configuration.background.color.nColors(); i++)
            {
                str << _(" ") << (*iter)[i].r;
                str << _(" ") << (*iter)[i].g;
                str << _(" ") << (*iter)[i].b;
            }

            conf.AddLine(str);

            str = _("");
        }
	}

    //for the boundary color
	{
        str << _("boundary");

        for (UInt i = 0; i < this->configuration.boundary.color.nColors(); i++)
        {
            str << _(" ") << this->configuration.boundary.color[i].r;
            str << _(" ") << this->configuration.boundary.color[i].g;
            str << _(" ") << this->configuration.boundary.color[i].b;
        }

        conf.AddLine(str);

        str = _("");

        str << _("boundarys ") << this->configuration.boundary.colors.size();

        conf.AddLine(str);

        str = _("");

        for (Configuration::Boundary::ColorList::iterator iter = this->configuration.boundary.colors.begin();
             iter != this->configuration.boundary.colors.end(); iter++)
        {
            str << wxString((*iter).name.c_str(), wxMBConvLibc());

            for (UInt i = 0; i < this->configuration.boundary.color.nColors(); i++)
            {
                str << _(" ") << (*iter)[i].r;
                str << _(" ") << (*iter)[i].g;
                str << _(" ") << (*iter)[i].b;
            }

            conf.AddLine(str);

            str = _("");
        }
	}

	//for the mesh color
	{
        str << _("mesh");

        for (UInt i = 0; i < this->configuration.mesh.color.nColors(); i++)
        {
            str << _(" ") << this->configuration.mesh.color[i].r;
            str << _(" ") << this->configuration.mesh.color[i].g;
            str << _(" ") << this->configuration.mesh.color[i].b;
        }

        conf.AddLine(str);

        str = _("");

        str << _("meshs ") << this->configuration.mesh.colors.size();

        conf.AddLine(str);

        str = _("");

        for (Configuration::Mesh::ColorList::iterator iter = this->configuration.mesh.colors.begin();
             iter != this->configuration.mesh.colors.end(); iter++)
        {
            str << wxString((*iter).name.c_str(), wxMBConvLibc());

            for (UInt i = 0; i < this->configuration.mesh.color.nColors(); i++)
            {
                str << _(" ") << (*iter)[i].r;
                str << _(" ") << (*iter)[i].g;
                str << _(" ") << (*iter)[i].b;
            }

            conf.AddLine(str);

            str = _("");
        }
	}

	//for the mesh quality color
	{
        str << _("quality");

        for (UInt i = 0; i < this->configuration.quality.color.nColors(); i++)
        {
            str << _(" ") << this->configuration.quality.color[i].r;
            str << _(" ") << this->configuration.quality.color[i].g;
            str << _(" ") << this->configuration.quality.color[i].b;
        }

        conf.AddLine(str);

        str = _("");

        str << _("qualitys ") << this->configuration.quality.colors.size();

        conf.AddLine(str);

        str = _("");

        for (Configuration::MeshQuality::ColorList::iterator iter = this->configuration.quality.colors.begin();
             iter != this->configuration.quality.colors.end(); iter++)
        {
            str << wxString((*iter).name.c_str(), wxMBConvLibc());

            for (UInt i = 0; i < this->configuration.quality.color.nColors(); i++)
            {
                str << _(" ") << (*iter)[i].r;
                str << _(" ") << (*iter)[i].g;
                str << _(" ") << (*iter)[i].b;
            }

            conf.AddLine(str);

            str = _("");
        }
	}

	//for the front color
	{
        str << _("front");

        for (UInt i = 0; i < this->configuration.front.color.nColors(); i++)
        {
            str << _(" ") << this->configuration.front.color[i].r;
            str << _(" ") << this->configuration.front.color[i].g;
            str << _(" ") << this->configuration.front.color[i].b;
        }

        conf.AddLine(str);

        str = _("");

        str << _("fronts ") << this->configuration.front.colors.size();

        conf.AddLine(str);

        str = _("");

        for (Configuration::Front::ColorList::iterator iter = this->configuration.front.colors.begin();
             iter != this->configuration.front.colors.end(); iter++)
        {
            str << wxString((*iter).name.c_str(), wxMBConvLibc());

            for (UInt i = 0; i < this->configuration.front.color.nColors(); i++)
            {
                str << _(" ") << (*iter)[i].r;
                str << _(" ") << (*iter)[i].g;
                str << _(" ") << (*iter)[i].b;
            }

            conf.AddLine(str);

            str = _("");
        }
	}

	//for the tree cell color
	{
        str << _("treecell");

        for (UInt i = 0; i < this->configuration.cell.color.nColors(); i++)
        {
            str << _(" ") << this->configuration.cell.color[i].r;
            str << _(" ") << this->configuration.cell.color[i].g;
            str << _(" ") << this->configuration.cell.color[i].b;
        }

        conf.AddLine(str);

        str = _("");

        str << _("treecells ") << this->configuration.cell.colors.size();

        conf.AddLine(str);

        str = _("");

        for (Configuration::TreeCell::ColorList::iterator iter = this->configuration.cell.colors.begin();
             iter != this->configuration.cell.colors.end(); iter++)
        {
            str << wxString((*iter).name.c_str(), wxMBConvLibc());

            for (UInt i = 0; i < this->configuration.cell.color.nColors(); i++)
            {
                str << _(" ") << (*iter)[i].r;
                str << _(" ") << (*iter)[i].g;
                str << _(" ") << (*iter)[i].b;
            }

            conf.AddLine(str);

            str = _("");
        }
	}

	//for the partition color
	{
        str << _("partition");

        for (UInt i = 0; i < this->configuration.partition.color.nColors(); i++)
        {
            str << _(" ") << this->configuration.partition.color[i].r;
            str << _(" ") << this->configuration.partition.color[i].g;
            str << _(" ") << this->configuration.partition.color[i].b;
        }

        conf.AddLine(str);

        str = _("");

        str << _("partitions ") << this->configuration.partition.colors.size();

        conf.AddLine(str);

        str = _("");

        for (Configuration::Partition::ColorList::iterator iter = this->configuration.partition.colors.begin();
             iter != this->configuration.partition.colors.end(); iter++)
        {
            str << wxString((*iter).name.c_str(), wxMBConvLibc());

            for (UInt i = 0; i < this->configuration.partition.color.nColors(); i++)
            {
                str << _(" ") << (*iter)[i].r;
                str << _(" ") << (*iter)[i].g;
                str << _(" ") << (*iter)[i].b;
            }

            conf.AddLine(str);

            str = _("");
        }
	}

	conf.Write();

	conf.Close();
}

void CRAbMesh::Frame::SetConfiguration(const CRAbMesh::Frame::Configuration &configuration)
{
	this->configuration = configuration;
}

CRAbMesh::Frame::Configuration &CRAbMesh::Frame::GetConfiguration()
{
	return this->configuration;
}

ViewModule *CRAbMesh::Frame::GetViewModule() const
{
    return this->modules.empty() ? NULL : static_cast<ViewModule *>((*this->modules.find(moduleNames[0])).second);
}

void CRAbMesh::Frame::SetCurrentModule(Module *module)
{
    wxString loadedFilename = _("");

    if (this->currentModule)
    {
        loadedFilename = this->currentModule->GetLoadedFilename();

        this->RemoveEventHandler(this->currentModule);
        this->currentModule->SetHandler(false);

        //remove menus, toolbars and panels of currentModule from this->aui
        for (unsigned int j = 0; j < this->currentModule->GetMenuCount(); j++)
        {
            for (unsigned int i = 0; i < this->menuBar->GetMenuCount(); i++)
            {
                if (this->menuBar->GetMenu(i) == this->currentModule->GetMenu(j))
                {
                    this->menuBar->Remove(i);

                    break;
                }
            }
        }

        for (unsigned int j = 0; j < this->currentModule->GetToolbarCount(); j++)
        {
            this->aui.GetPane(this->currentModule->GetToolbar(j)).Hide();
        }

        for (unsigned int j = 0; j < this->currentModule->GetPanelCount(); j++)
        {
            this->aui.GetPane(this->currentModule->GetPanel(j)).Hide();
        }

        if (module)
        {
            this->currentModule->ChangeTo(module);
        }
    }

    Module *m = ((module) && (this->currentModule)) ? this->currentModule : NULL;

    this->currentModule = module;

    if (this->currentModule)
    {
        this->currentModule->ChangeFrom(m);

        if (this->currentModule->GetLoadedFilename().Length() == 0)
        {
            if (this->currentModule->LoadFile(std::string(loadedFilename.mb_str(wxMBConvLibc()))))
            {
                this->canvas->FitCamera();

                this->Retitle();
            }
        }

        for (unsigned int i = 0; i < this->currentModule->GetMenuCount(); i++)
        {
            wxString title;

            this->menuBar->Insert(i + 1, this->currentModule->GetMenu(i, title), title);
        }

        for (unsigned int i = 0; i < this->currentModule->GetToolbarCount(); i++)
        {
            this->aui.GetPane(this->currentModule->GetToolbar(i)).MinSize(wxDefaultSize).Show();
        }

        for (unsigned int i = 0; i < this->currentModule->GetPanelCount(); i++)
        {
            this->aui.GetPane(this->currentModule->GetPanel(i)).MinSize(wxDefaultSize).Show();
        }

        this->PushEventHandler(this->currentModule);
        this->currentModule->SetHandler(true);
    }

    this->UpdateAui();
}

void CRAbMesh::Frame::SetCurrentModule(const std::string &module)
{
    bool valid = false;

    for (unsigned int i = 1; i < MODULES_COUNT; i++)
    {
        if (module == moduleNames[i])
        {
            valid = true;

            break;
        }
    }

    if ((!valid) || (module == moduleNames[0]))
    {
        //this->moduleTitle = _("");

        this->SetCurrentModule(NULL);
    }
    else
    {
        //this->moduleTitle = wxString(module.c_str(), wxMBConvLibc());

        if (!this->modules[module])
        {
            //wxMessageBox(_("Sorry, module not implemented yet."), _("Sorry!"), wxICON_INFORMATION | wxOK, this);
            this->SetStatusText(_("Sorry, module not implemented yet."));
        }
        else
        {
            this->SetStatusText(_(""));
        }

        this->SetCurrentModule(this->modules[module]);
    }

    this->moduleTitle = (this->GetCurrentModule()) ? this->GetCurrentModule()->Name() : wxString();

    this->Retitle();

    this->UpdateAui();

    this->canvas->Refresh(false);
}

Module *CRAbMesh::Frame::GetCurrentModule() const
{
    return this->currentModule;
}

Module *CRAbMesh::Frame::GetCurrentOrViewModule() const
{
    return this->GetCurrentModule() ? this->GetCurrentModule() : this->GetViewModule();
}

ModuleMap CRAbMesh::Frame::GetModules() const
{
    return this->modules;
}

void CRAbMesh::Frame::SetCurrentOrViewCamera(Camera *camera)
{
    this->GetCurrentModule() ? this->GetCurrentModule()->SetCamera(camera) : this->GetViewModule()->SetCamera(camera);
}

Camera *CRAbMesh::Frame::GetCurrentOrViewCamera() const
{
    return this->GetCurrentModule() ? this->GetCurrentModule()->GetCamera() :
        (this->GetViewModule() ? this->GetViewModule()->GetCamera() : NULL);
}

bool CRAbMesh::Frame::OpenFile(const wxString &path)
{
    std::string inputFile(path.mb_str(wxMBConvLibc()));

    bool opened = false;

    if (this->currentModule)
    {
        this->currentModule->Clear(true);
        this->currentModule->Clear();
        this->Clear();

        opened = this->currentModule->LoadFile(inputFile);
    }

    for (unsigned int i = 0; ((!opened) && (i < MODULES_COUNT)); i++)
    {
#if (!USE_ONLY_MTOOLS)
        if (i == 1)
        {
            continue;
        }
#endif //#if (!USE_ONLY_MTOOLS)

        Module *module = this->modules[moduleNames[i]];

        if ((module) && (module != this->currentModule) && (module->LoadFile(inputFile)))
        {
            this->GetViewModule()->SetModuleOption(moduleNames[i]);

            opened = true;
        }
    }

#if (!USE_ONLY_MTOOLS)
    if (!opened)
	{
		MeshesModule *module = dynamic_cast<MeshesModule *>(this->modules[moduleNames[1]]);

        if ((module) && (module != this->currentModule) && (module->LoadFile(inputFile)))
        {
            this->GetViewModule()->SetModuleOption(moduleNames[1]);

            module->ForceShowMesh();

            opened = true;
        }
	}
#endif //#if (!USE_ONLY_MTOOLS)

    if (opened)
    {
        this->canvas->FitCamera();

        this->Retitle();

        this->canvas->Refresh(false);
    }

    return opened;
}

void CRAbMesh::Frame::Execute()
{
    this->miFileOpen->Enable(false);
    this->miFileSave->Enable(false);
#if (!USE_SCREENSHOT)
    //this->miFileExport->Enable(false);
#if USE_OPENCV
    this->miFileVideo->Enable(false);
#endif //#if USE_OPENCV
#endif //#if (!USE_SCREENSHOT)
    //this->miFileSaveCamera->Enable(false);
    //this->miFileOpenCamera->Enable(false);

    //wxBeginBusyCursor();

    this->SetStatusText(_("Executing. Please, wait..."));

    this->canvas->Execute();
}

void CRAbMesh::Frame::Ready()
{
    this->miFileOpen->Enable();
    this->miFileSave->Enable();
#if (!USE_SCREENSHOT)
    //this->miFileExport->Enable();
#if USE_OPENCV
    this->miFileVideo->Enable();
#endif //#if USE_OPENCV
#endif //#if (!USE_SCREENSHOT)
    //this->miFileSaveCamera->Enable();
    //this->miFileOpenCamera->Enable();

    //wxEndBusyCursor();
}

void CRAbMesh::Frame::UpdateAui()
{
    if (this->currentModule)
    {
        wxSize hor = wxDefaultSize;
        wxSize ver = wxDefaultSize;

        for (unsigned int i = 0; i < this->currentModule->GetPanelCount(); i++)
        {
            this->currentModule->GetPanel(i)->GetSizer()->Fit(this->currentModule->GetPanel(i));
            wxSize s = this->currentModule->GetPanel(i)->GetSizer()->CalcMin();

            wxAuiPaneInfo info = this->aui.GetPane(this->currentModule->GetPanel(i));

            if (((info.dock_direction == wxAUI_DOCK_LEFT) || (info.dock_direction == wxAUI_DOCK_RIGHT)) &&
                (s.GetWidth() > hor.GetWidth()))
            {
                hor.SetWidth(s.GetWidth());
            }

            if (((info.dock_direction == wxAUI_DOCK_TOP) || (info.dock_direction == wxAUI_DOCK_BOTTOM)) &&
                (s.GetHeight() > ver.GetHeight()))
            {
                ver.SetHeight(s.GetHeight());
            }
        }

        for (unsigned int i = 0; i < this->currentModule->GetPanelCount(); i++)
        {
            wxAuiPaneInfo info = this->aui.GetPane(this->currentModule->GetPanel(i));

            if ((info.dock_direction == wxAUI_DOCK_LEFT) || (info.dock_direction == wxAUI_DOCK_RIGHT))
            {
                this->aui.GetPane(this->currentModule->GetPanel(i)).MinSize(hor);
            }
            else if ((info.dock_direction == wxAUI_DOCK_TOP) || (info.dock_direction == wxAUI_DOCK_BOTTOM))
            {
                this->aui.GetPane(this->currentModule->GetPanel(i)).MinSize(ver);
            }
        }
    }

    this->aui.Update();
}

#if USE_OPENCV
void CRAbMesh::Frame::CheckVideoState(bool check)
{
    this->miFileVideo->Check(check);
}
#endif //#if USE_OPENCV

/*bool CRAbMesh::Frame::ShowFullScreen(bool show, long style)
{
    bool ok = wxFrame::ShowFullScreen(show, style);

    if (ok)
    {
        if (this->currentModule)
        {
            if (show)
            {
                for (unsigned int i = 0; i < )
            }
            else
            {
                this->aui.LoadPerspective(perspective);

                mod = NULL;
                perspective = _("");
            }
        }

        this->UpdateAui();
    }

    return ok;
}*/

void CRAbMesh::Frame::Retitle()
{
    this->SetTitle(this->Title());
}

void CRAbMesh::Frame::Clear(bool destroying)
{
    this->canvas->Clear(destroying);

    this->Retitle();

    this->SetStatusText(_(""));
}

void CRAbMesh::Frame::Close()
{
    this->Destroy();
}

wxString CRAbMesh::Frame::Title()
{
    wxString title = this->originalTitle;

    if (this->moduleTitle.Length() > 0)
    {
        title << _(" - ") + this->moduleTitle;
    }

    if ((this->GetCurrentModule()) && (this->GetCurrentModule()->GetLoadedFilename().Length() > 0))
    {
        title << _(" - ") << this->GetCurrentModule()->GetLoadedFilename();
    }

    return title;
}

#endif //#if USE_GUI
