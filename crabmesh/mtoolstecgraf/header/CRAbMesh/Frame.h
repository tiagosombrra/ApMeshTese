#ifndef _CRAB_MESH_FRAME_H_
#define _CRAB_MESH_FRAME_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

namespace CRAbMesh
{
	class Frame : public wxFrame
	{
	friend class CRAbMesh::MeshesModule;

	public:

		Frame();
		virtual ~Frame();

		virtual const wxString &GetOpenPath() const;
		virtual const wxString &GetOpenWildcard() const;
		virtual std::string DetermineGenSaveExtension(int index) const;
		virtual std::string DetermineGenSaveExtension(const wxFileDialog *save) const;

		virtual void OnClose(wxCloseEvent &event);
		//virtual void OnAuiPaneClose(wxAuiManagerEvent &event);

		virtual void OnMenuOpen(wxCommandEvent &event);
		virtual void OnMenuSave(wxCommandEvent &event);
		virtual void OnMenuExport(wxCommandEvent &event);
#if USE_OPENCV
		virtual void OnMenuVideo(wxCommandEvent &event);
#endif //#if USE_OPENCV
		virtual void OnMenuSaveCamera(wxCommandEvent &event);
		virtual void OnMenuOpenCamera(wxCommandEvent &event);
		virtual void OnMenuExit(wxCommandEvent &event);

		virtual void OnMenuAbout(wxCommandEvent &event);

		virtual void LoadConfiguration();
		virtual void CreateConfiguration();
		virtual void ApplyConfiguration(bool onlyBackground);
		virtual void SaveConfiguration();

	public:

		struct Configuration
		{
			struct Color
			{
			    double r, g, b;

			    bool operator==(const Color &c) const
			    {
			        if (this == &c) return true;

			        if (this->r != c.r) return false;
			        if (this->g != c.g) return false;
			        if (this->b != c.b) return false;

			        return true;
			    };

			    bool operator!=(const Color &c) const
			    {
			        return !this->operator==(c);
			    };
            };

			template<unsigned int ncolors>
			struct ColorScheme
			{
			    Color colors[ncolors];
			    std::string name;

			    unsigned int nColors() {return ncolors;};
			    Color &operator[](int i) {return this->colors[i];};

			    bool operator==(const ColorScheme<ncolors> &c) const
			    {
			        if (this == &c) return true;

			        for (unsigned int i = 0; i < ncolors; i++)
                    {
                        if (this->colors[i] != c.colors[i])
                        {
                            return false;
                        }
                    }

                    return true;
			    };

			    bool operator!=(const ColorScheme<ncolors> &c) const
			    {
			        return !this->operator==(c);
			    };
            };

			template<unsigned int ncolors>
			struct ColorSchemeType
			{
			    typedef ColorScheme<ncolors> Color;
			    typedef std::list<ColorSchemeType::Color> ColorList;

			    ColorSchemeType::Color color;
			    ColorSchemeType::ColorList colors;
            };

			typedef ColorSchemeType<4> Background;
			typedef ColorSchemeType<2> Boundary;
			typedef ColorSchemeType<3> Mesh;
			typedef ColorSchemeType<10> MeshQuality;
			typedef ColorSchemeType<5> Front;
			typedef ColorSchemeType<5> TreeCell;
			typedef ColorSchemeType<1> Partition;

			Configuration();

			Background background;
			Boundary boundary;
			Mesh mesh;
			MeshQuality quality;
			Front front;
			TreeCell cell;
			Partition partition;
		};

	public:

		virtual void SetConfiguration(const Configuration &configuration);
		virtual Configuration &GetConfiguration();

	public:

		virtual ViewModule *GetViewModule() const;

		virtual void SetCurrentModule(Module *module);
		virtual void SetCurrentModule(const std::string &module);
		virtual Module *GetCurrentModule() const;
		virtual Module *GetCurrentOrViewModule() const;
		virtual ModuleMap GetModules() const;
		virtual void SetCurrentOrViewCamera(Camera *camera);
		virtual Camera *GetCurrentOrViewCamera() const;

		virtual bool OpenFile(const wxString &path);

		virtual void Execute();

		//clears the canvas
		virtual void Clear(bool destroying = false);

		virtual void Ready();

		//virtual void UpdateTime();

		virtual void UpdateAui();
#if USE_OPENCV
		virtual void CheckVideoState(bool check);
#endif //#if USE_OPENCV
		//virtual bool ShowFullScreen(bool show, long style = wxFULLSCREEN_ALL);

		virtual void Retitle();

	protected:

		//closes the program
		virtual void Close();

		virtual wxString Title();

	protected:

		Canvas *canvas;

		ModuleMap modules;
		Module *currentModule;

		wxMenuBar *menuBar;

		wxMenuItem *miFileOpen;
		wxMenuItem *miFileSave;
		wxMenuItem *miFileExport;
#if USE_OPENCV
		wxMenuItem *miFileVideo;
#endif //#if USE_OPENCV
		wxMenuItem *miFileSaveCamera;
		wxMenuItem *miFileOpenCamera;
		wxMenuItem *miFileExit;

		wxMenuItem *miAboutAbout;

		wxAuiManager aui;

		wxPanel *pCanvas;

		//wxTextCtrl *tcElapsedTime;
		//wxTextCtrl *tcTotalTime;

		wxStatusBar *statusBar;

		wxString originalTitle;
		wxString moduleTitle;

		wxString openPath;
		wxString openWildcard;
		wxString savePath;
		wxString saveWildcard;

		/*ConfigurationDialog *confDialog;*/

		wxString configurationFile;
		Configuration configuration;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_FRAME_H_
