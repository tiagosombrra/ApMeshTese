#ifndef _CRAB_MESH_CANVAS_H_
#define _CRAB_MESH_CANVAS_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

#include <wx/glcanvas.h>

#ifdef Status
	#undef Status
#endif

#if USE_OPENCV
//#include <opencv/cv.h>
#include <opencv/highgui.h>
#endif //#if USE_OPENCV

namespace CRAbMesh
{
	class Canvas : public wxGLCanvas
	{
	friend class Thread;

	public:

		Canvas(Frame *frame, wxWindow *parent);
		virtual ~Canvas();

		virtual void OnSize(wxSizeEvent &event);
		virtual void OnPaint(wxPaintEvent &event);
		virtual void OnEraseBackground(wxEraseEvent &event);

		virtual void OnMouseLeftDown(wxMouseEvent &event);
		virtual void OnMouseMiddleDown(wxMouseEvent &event);
		virtual void OnMouseRightDown(wxMouseEvent &event);
		virtual void OnMouseMotion(wxMouseEvent &event);
		virtual void OnMouseScroll(wxMouseEvent &event);

		virtual void OnThread(wxCommandEvent &event);

		virtual void OnTimer(wxTimerEvent &event);

		virtual void ResetView();
		virtual void ResetProjection();

		//method to leave the camera in the middle of the object
		virtual void FitCamera();
		virtual void Center(const Data::Point *p);
		virtual void Center(const Data::GeometricShape *s);

		virtual void SaveScreenshot(const std::string &filename/*, int pointsPerPixel*/);
#if USE_OPENCV
		virtual void SaveVideo(const std::string &filename/*, int fps*/);
		virtual void StopVideo();
#endif //#if USE_OPENCV

		//method to clear the canvas
		virtual void Clear(bool destroying = false);

		//executes the method
		virtual void Execute();

		virtual bool RenderExclusively(bool force, bool enter = true, bool thread = false) const;

	protected:

		virtual wxImage *Screenshot(/*int pointsPerPixel*/);
		virtual void Save(wxImage *img, const std::string &filename);

		virtual void RenderTexture(int x, int y) const;
		virtual void RenderAxes() const;
		virtual void RenderCrossbar() const;
		virtual void RenderBackground() const;
		virtual void RenderAll(int w, int h) const;
		virtual void Render() const;

		//method that deals the insertion of a new vertex by the mouse
		// x and y are the position of the mouse related to the canvas
		//virtual void Insert(int x, int y);
		virtual void Pick(int x, int y, bool holding);

		virtual void SetOpenGLStateBeforeRendering();

	protected:

		wxGLContext *glContext;
		static int attribs[];

		Frame *frame;

		Thread *thread;

		mutable GLubyte *data;
		wxImage *iCrab;

		wxTimer timer;
		wxTimer timerAnim;

		bool pending;
		bool clearing;
		int disp; //displacement for the CRAb logo

#if USE_OPENCV
		cv::VideoWriter video;
#endif //#if USE_OPENCV

		//mutable wxCriticalSection cs;
		mutable wxMutex cs;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_CANVAS_H_
