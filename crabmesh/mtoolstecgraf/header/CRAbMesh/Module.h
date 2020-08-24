#ifndef _CRAB_MESH_MODULE_H_
#define _CRAB_MESH_MODULE_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

namespace CRAbMesh
{
	class Module : public wxEvtHandler
	{
	public:

		Module(Frame *frame = NULL, Canvas *canvas = NULL);
		virtual ~Module();

		virtual void SetFrame(Frame *frame);
		virtual void SetCanvas(Canvas *canvas);
		virtual void SetCamera(Camera *camera);
		virtual Camera *GetCamera() const;

		virtual void SetHandler(bool handler);
		virtual bool IsHandler() const;

		virtual unsigned int GetDimension() const = 0;
		virtual Performer::MainDrive *GetDrive() const;

		virtual wxString Name() const = 0;

		virtual unsigned int GetMenuCount() const;
		virtual wxMenu *GetMenu(unsigned int i, wxString &title) const = 0;
		virtual wxMenu *GetMenu(unsigned int i);
		virtual unsigned int GetToolbarCount() const;
		virtual wxAuiToolBar *GetToolbar(unsigned int i, wxString &title) const = 0;
		virtual wxAuiToolBar *GetToolbar(unsigned int i);
		virtual unsigned int GetPanelCount() const;
		virtual wxPanel *GetPanel(unsigned int i, wxString &title) const = 0;
		virtual wxPanel *GetPanel(unsigned int i);

		///always call this method in derived classes
		virtual bool LoadFile(const std::string &filename);
		virtual bool SaveFile(const std::string &filename) = 0;

		virtual bool SaveCamera(const std::string &filename) const;
		virtual bool LoadCamera(const std::string &filename);

		virtual wxString GetLoadedFilename() const;
		virtual const Data::DrawableList &GetPicked() const;

		virtual void SetParameters(bool firstTime) = 0;

		virtual void Execute() = 0;
		///@param clearing indicates that the main drive should be cleared
		///@param pending indicates that the algorithm is still pending, i.e.,
		///       it is not finished yet
		virtual void Ready(bool clearing, bool pending) = 0;
		///always call this method in derived classes
		virtual void Clear(bool onlyDrive = false, bool destroying = false);
		virtual void ClearDisplayLists(bool force = true) const = 0;

		virtual void Render() const = 0;
		virtual void SetOpenGLStateBeforeRendering(/*const Camera *camera*/) const = 0;
		virtual bool BoundingBox(double &minx, double &miny, double &minz, double &maxx, double &maxy, double &maxz) const = 0;
		virtual void OnTimer();
		virtual bool OnLeftDown(int x, int y);
		virtual bool IsMouseMotionAllowed() const;
		virtual void ChangeFrom(const Module *module);
		virtual void ChangeTo(const Module *module);

		virtual Data::DrawableList Pick(int x, int y, bool holding = false);

		virtual void SetGUIState() = 0;

	/*protected:

		virtual void OnAuiPaneClose(wxAuiManagerEvent &event);*/

	protected:

		virtual void PickReal(int x, int y);
		virtual void PickIntersection(int x, int y);
		virtual void PickUnionFirst(int x, int y);

		virtual void MakeMainDrive() = 0;
		virtual void MakeMenus() = 0;
		/*///these methods must be called on their inherited version after
		///  the toolbars and panels are created*/
		virtual void MakeToolbars() = 0;
		virtual void MakePanels() = 0;

		virtual void RenderPick() const = 0;
		virtual unsigned int PickBufferSize() const = 0;
		virtual Data::DrawableList Picked(int hits, GLuint buffer[]) = 0;

	protected:

		Frame *frame;
		Canvas *canvas;
		Camera *camera;

		Performer::MainDrive *drive;

		MenuVector menus;
		ToolBarVector toolbars;
		//Data::BoolVector toolbarsEnabled;
		PanelVector panels;
		//Data::BoolVector panelsEnabled;

		Data::DrawableList picked;

	protected:
		///attributes for configuration

		std::string loadedFilename;

		bool handler;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_MODULE_H_
