#include "CRAbMesh/Canvas.h"

#if USE_GUI

#include <wx/dcbuffer.h>
#include <cfloat>

#include "CRAbMesh/Camera.h"
#include "CRAbMesh/Module.h"
#include "CRAbMesh/ViewModule.h"
#include "CRAbMesh/MeshesModule.h"
#include "CRAbMesh/Thread.h"
#include "CRAbMesh/App.h"
#include "CRAbMesh/Frame.h"
#include "Performer/MainDrive.h"
#include "Data/State.h"
#include "Data/Drawable.h"
#include "Data/Point3D.h"
#include "Data/GeometricShape.h"
#include "Data/Box.h"

//#include "../../images/crab3.xpm"
#include "crab5_png.cpp"

int CRAbMesh::Canvas::attribs[] = {
	WX_GL_RGBA,
	WX_GL_BUFFER_SIZE, 16,
	WX_GL_DOUBLEBUFFER,
	WX_GL_DEPTH_SIZE, 24,
	WX_GL_STENCIL_SIZE, 8,
	0};

//DECLARE_APP(CRAbMesh::App)

DECLARE_EVENT_TYPE(EVT_THREAD, -1)

CRAbMesh::Canvas::Canvas(Frame *frame, wxWindow *parent) :
    wxGLCanvas(parent, wxID_ANY, Canvas::attribs, wxDefaultPosition, wxDefaultSize,
	wxFULL_REPAINT_ON_RESIZE | wxSUNKEN_BORDER)
{
    this->frame = frame;

    this->timer.SetOwner(this, wxID_ANY);
    this->timerAnim.SetOwner(this, wxID_ANY);

    srand(static_cast<unsigned int>(time(NULL)));

    this->data = NULL;
	//this->iCrab = new wxImage(_("../../images/crab5.png"));
	this->iCrab = new wxImage(IMAGE_PNG_FROM_DATA(crab5));
	this->iCrab->Rescale(100, 76);

	this->thread = NULL;

    this->disp = 5;

	this->glContext = new wxGLContext(this);

	this->pending = false;
	this->clearing = false;

	this->SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	this->Connect(wxEVT_SIZE,             wxSizeEventHandler(Canvas::OnSize));
    this->Connect(wxEVT_PAINT,            wxPaintEventHandler(Canvas::OnPaint));
    this->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(Canvas::OnEraseBackground));

    this->Connect(wxEVT_LEFT_DOWN,   wxMouseEventHandler(Canvas::OnMouseLeftDown));
    this->Connect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(Canvas::OnMouseMiddleDown));
    //this->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(Canvas::OnMouseDoubleLeftDown));
    this->Connect(wxEVT_RIGHT_DOWN,  wxMouseEventHandler(Canvas::OnMouseRightDown));
    this->Connect(wxEVT_MOTION,      wxMouseEventHandler(Canvas::OnMouseMotion));
    this->Connect(wxEVT_MOUSEWHEEL,  wxMouseEventHandler(Canvas::OnMouseScroll));

	this->Connect(wxID_ANY, EVT_THREAD, wxCommandEventHandler(Canvas::OnThread));

	this->Connect(this->timer.GetId(),      wxEVT_TIMER, wxTimerEventHandler(Canvas::OnTimer));
	this->Connect(this->timerAnim.GetId(),  wxEVT_TIMER, wxTimerEventHandler(Canvas::OnTimer));
}

CRAbMesh::Canvas::~Canvas()
{
	if (this->iCrab)
	{
		delete this->iCrab;
	}

	if (this->thread)
	{
		this->thread->Delete();

		//this->thread = NULL;
	}

	if (this->glContext)
	{
		delete this->glContext;
	}

	if (this->data)
	{
		delete [] this->data;
	}
}

void CRAbMesh::Canvas::RenderTexture(int x, int y) const
{
	if (!this->data)
	{
		int width = this->iCrab->GetWidth();
		int height = this->iCrab->GetHeight();

		GLubyte	*bitmap = this->iCrab->GetData();
		GLubyte *alpha = this->iCrab->GetAlpha();

		int bytesPerPixel = this->iCrab->HasAlpha() ? 4 : 3;

		int size = width*height*bytesPerPixel;
		this->data = new GLubyte[size];

		int rev_val = height - 1;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				this->data[(x + y*width)*bytesPerPixel + 0] = bitmap[(x + (rev_val - y)*width)*3 + 0];
				this->data[(x + y*width)*bytesPerPixel + 1] = bitmap[(x + (rev_val - y)*width)*3 + 1];
				this->data[(x + y*width)*bytesPerPixel + 2] = bitmap[(x + (rev_val - y)*width)*3 + 2];

				if (bytesPerPixel == 4)
				{
					this->data[(x + y*width)*bytesPerPixel + 3] = alpha[x + (rev_val - y)*width];
				}
			}
		}
	}

	int w, h;
	this->GetClientSize(&w, &h);

	glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_STENCIL_TEST);

		glWindowPos2i(x, h - this->iCrab->GetHeight() - y);
		glDrawPixels(this->iCrab->GetWidth(), this->iCrab->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, this->data);
	glPopAttrib();
}

void CRAbMesh::Canvas::RenderAxes() const
{
	Camera *camera = this->frame->GetCurrentOrViewCamera();

	if (!camera)
	{
		return;
	}

	int w, h;
	this->GetClientSize(&w, &h);

	double pixel = 50.0;

	double px = -1.0 + 2.0*pixel/static_cast<double>(w);
	double py = -1.0 + 2.0*pixel/static_cast<double>(h);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		glLoadIdentity();
		bool perspective = camera->IsPerspective();
		double digitalZoom = camera->GetDigitalZoom();

		camera->SetPerspective(true);
		camera->SetDigitalZoom(1.0);
		camera->Projection();
		camera->SetPerspective(perspective);
		camera->SetDigitalZoom(digitalZoom);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		double vx = camera->GetPosition(0) - camera->GetFront(0);
		double vy = camera->GetPosition(1) - camera->GetFront(1);
		double vz = camera->GetPosition(2) - camera->GetFront(2);

		double size = (pixel*0.75)/std::sqrt(vx*vx + vy*vy + vz*vz);

		vx *= size;
		vy *= size;
		vz *= size;

		glLoadIdentity();
		gluLookAt(vx, vy, vz, 0.0, 0.0, 0.0,
				  camera->GetUp(0), camera->GetUp(1), camera->GetUp(2));

		double projection[16], modelview[16];

		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPushMatrix();
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_DEPTH_TEST);

			glDisable(GL_CLIP_PLANE0);

			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_CULL_FACE);

			glTranslatef(static_cast<GLfloat>(px), static_cast<GLfloat>(py), 0.0);
			glMultMatrixd(projection);
			glMultMatrixd(modelview);

			glLineWidth(2.0);

			glBegin(GL_LINES);
				//glColor3d(0.0, 0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
				glColor3d(1.0, 1.0, 1.0); glVertex3d(0.0, 0.0, 0.0);
				glColor3d(1.0, 0.0, 0.0); glVertex3d(4.0, 0.0, 0.0);

				//glColor3d(0.0, 0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
				glColor3d(1.0, 1.0, 1.0); glVertex3d(0.0, 0.0, 0.0);
				glColor3d(0.0, 1.0, 0.0); glVertex3d(0.0, 4.0, 0.0);

				//glColor3d(0.0, 0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
				glColor3d(1.0, 1.0, 1.0); glVertex3d(0.0, 0.0, 0.0);
				glColor3d(0.0, 0.0, 1.0); glVertex3d(0.0, 0.0, 4.0);
			glEnd();
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glPopAttrib();
}

void CRAbMesh::Canvas::RenderCrossbar() const
{
	Camera *camera = this->frame->GetCurrentOrViewCamera();

	if (!camera)
	{
		return;
	}

	double size = std::sqrt(
		std::pow(camera->GetFront(0) - camera->GetPosition(0), 2.0) +
		std::pow(camera->GetFront(1) - camera->GetPosition(1), 2.0) +
		std::pow(camera->GetFront(2) - camera->GetPosition(2), 2.0))*
		camera->GetDigitalZoom();

	size *= 0.05;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);

			glDisable(GL_CLIP_PLANE0);

			glTranslated(camera->GetFront(0), camera->GetFront(1), camera->GetFront(2));

			glLineWidth(1.0);

			glBegin(GL_LINES);
				glColor3d(1.0, 0.0, 0.0);
				glVertex3d(-size, 0.0, 0.0);
				glVertex3d( size, 0.0, 0.0);

				glColor3d(0.0, 1.0, 0.0);
				glVertex3d(0.0, -size, 0.0);
				glVertex3d(0.0,  size, 0.0);

				glColor3d(0.0, 0.0, 1.0);
				glVertex3d(0.0, 0.0, -size);
				glVertex3d(0.0, 0.0,  size);
			glEnd();
		glPopMatrix();
	glPopAttrib();
}

void CRAbMesh::Canvas::RenderBackground() const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_CULL_FACE);

			Frame::Configuration::Color c1, c2, c3, c4;
			c1 = this->frame->GetConfiguration().background.color[0];
			c2 = this->frame->GetConfiguration().background.color[1];
			c3 = this->frame->GetConfiguration().background.color[2];
			c4 = this->frame->GetConfiguration().background.color[3];

			glBegin(GL_QUADS);
				glColor3d(c1.r, c1.g, c1.b); glVertex2d( 1.0, -1.0);
				glColor3d(c2.r, c2.g, c2.b); glVertex2d( 1.0,  1.0);
				glColor3d(c3.r, c3.g, c3.b); glVertex2d(-1.0,  1.0);
				glColor3d(c4.r, c4.g, c4.b); glVertex2d(-1.0, -1.0);
			glEnd();

		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}

void CRAbMesh::Canvas::RenderAll(int w, int /*h*/) const
{
	this->RenderBackground();

	if (!this->frame->GetViewModule()->IsStereoscopyEnabled())
    {
		//std::cout << "onpaint rendering not stereo" << std::endl;

		this->Render();

		//std::cout << "onpaint rendering not stereo axes" << std::endl;

		if (this->frame->GetViewModule()->IsRenderAxesEnabled())
		{
			this->RenderAxes();
		}

		//std::cout << "onpaint rendering not stereo crossbar" << std::endl;

		if (this->frame->GetViewModule()->IsRenderCrossbarEnabled())
		{
			this->RenderCrossbar();
		}
    }
    else
    {
		Camera *camera = this->frame->GetCurrentOrViewCamera();

		camera->SetStereoPosition(Camera::LEFT);

		this->Render();

		if (this->frame->GetViewModule()->IsRenderAxesEnabled())
		{
			this->RenderAxes();
		}

		if (this->frame->GetViewModule()->IsRenderCrossbarEnabled())
		{
			this->RenderCrossbar();
		}

		camera->SetStereoPosition(Camera::RIGHT);

		this->Render();

		if (this->frame->GetViewModule()->IsRenderAxesEnabled())
		{
			this->RenderAxes();
		}

		if (this->frame->GetViewModule()->IsRenderCrossbarEnabled())
		{
			this->RenderCrossbar();
		}
    }

    if (this->frame->GetViewModule()->IsRenderIconEnabled())
    {
        this->RenderTexture(w - this->iCrab->GetWidth() - this->disp, this->disp);
    }
}

#if __WXMSW__
bool CRAbMesh::Canvas::RenderExclusively(bool force, bool enter, bool thread) const
#else
bool CRAbMesh::Canvas::RenderExclusively(bool force, bool enter, bool /*thread*/) const
#endif //#if __WXMSW__
{
	//std::cout << "render exclusively, enter = " << enter << ", thread = " << thread << std::endl;

	if (Data::isStepByStep())
	{
		//std::cout << "render exclusively, this->thread = " << this->thread << std::endl;

		if (this->thread)
		{
			if (enter)
			{
				//std::cout << "render exclusively, entering" << std::endl;
				//this->cs.Enter();
				if (((force) && (this->cs.Lock() == wxMUTEX_DEAD_LOCK)) ||
					((!force) && (this->cs.TryLock() == wxMUTEX_BUSY)))
				{
					return false;
				}
			}
			else
			{
				//std::cout << "render exclusively, leaving" << std::endl;
				//this->cs.Leave();
				this->cs.Unlock();
			}
		}
	}
#if __WXMSW__
	//this else probably only works under Windows
	else if ((!thread) && (this->thread))
	{
		if (enter)
		{
			this->thread->Pause();
		}
		else
		{
			this->thread->Resume();
		}
	}
#endif //#if __WXMSW__

	return true;
}

void CRAbMesh::Canvas::Render() const
{
	if (!this->frame->GetCurrentOrViewCamera())
	{
		return;
	}

	//std::cout << "onpaint render drawing camera" << std::endl;

	this->frame->GetCurrentOrViewCamera()->Draw();

	if (this->frame->GetCurrentModule())
	{
		//std::cout << "onpaint render entering critical section" << std::endl;
		//if (!this->RenderExclusively(false, true))
		if (!this->RenderExclusively(Data::isStepByStep(), true))
		{
			//std::cout << "could not enter critical section" << std::endl;

			return;
		}

		//std::cout << "onpaint render rendering" << std::endl;
		/*GLfloat pos[4];
		glGetLightfv(GL_LIGHT0, GL_POSITION, pos);
		std::cout << "getpos " << pos[0] << ", " << pos[1] << ", " << pos[2] << ", " << pos[3] << std::endl;*/
		this->frame->GetCurrentModule()->Render();

		//std::cout << "onpaint render leaving critical section" << std::endl;
		//this->RenderExclusively(false, false);
		this->RenderExclusively(Data::isStepByStep(), false);
	}

	if ((this->frame->GetViewModule()) &&
		(this->frame->GetViewModule() != this->frame->GetCurrentModule()))
	{
		//std::cout << "onpaint render rendering view module" << std::endl;
		this->frame->GetViewModule()->Render();
	}

	//this->RenderTexture();
}

void CRAbMesh::Canvas::Pick(int x, int y, bool holding)
{
	Module *module = this->frame->GetCurrentOrViewModule();

	if (!module)
	{
		return;
	}

	module->Pick(x, y, holding);
}

void CRAbMesh::Canvas::SetOpenGLStateBeforeRendering()
{
	this->SetCurrent(*this->glContext);

#if USE_SCREENSHOT
	glClearColor(1.0, 1.0, 1.0, 0.0);
#else //#if USE_SCREENSHOT
	glClearColor(0.0, 0.0, 0.0, 0.0);
#endif //#if USE_SCREENSHOT

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (this->frame->GetCurrentModule())
	{
		this->frame->GetCurrentModule()->SetOpenGLStateBeforeRendering();
	}

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);

	if (this->frame->GetViewModule()->IsDepthTestEnabled())
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	if (this->frame->GetViewModule()->IsCullFaceEnabled())
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	if (this->frame->GetViewModule()->IsWireframeEnabled())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void CRAbMesh::Canvas::OnSize(wxSizeEvent &event)
{
	// this is also necessary to update the context on some platforms
#if wxCHECK_VERSION(2, 9, 0)
	event.Skip();
#else
    wxGLCanvas::OnSize(event);
#endif //#if wxCHECK_VERSION(2, 9, 0)

    if (!this->frame->GetCurrentOrViewCamera())
	{
		return;
	}

	Camera *camera = this->frame->GetCurrentOrViewCamera();

    int w, h;
    this->GetClientSize(&w, &h);

#if (!wxCHECK_VERSION(2, 9, 0))
#ifndef __WXMOTIF__
    if (this->GetContext())
#endif //#ifndef __WXMOTIF__
#endif //#if (!wxCHECK_VERSION(2, 9, 0))
    {
        this->SetCurrent(*this->glContext);

        camera->SetDefaultCanvasSize(w, h);
    }

    camera->Draw();

	this->Refresh(false);
	this->Update();
}

void CRAbMesh::Canvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    if (!this->IsShown())
    {
    	return;
    }

    //std::cout << "onpaint creating buffer" << std::endl;

    wxAutoBufferedPaintDC dc(this);

	//std::cout << "onpaint setting opengl state" << std::endl;

	this->SetOpenGLStateBeforeRendering();

	int w, h;
	this->GetClientSize(&w, &h);

	//std::cout << "onpaint setting canvas size" << std::endl;

	if (!this->frame->GetCurrentOrViewCamera())
	{
		this->frame->SetCurrentOrViewCamera(new Camera(w, h));

		glewInit();
	}
	else
	{
		this->frame->GetCurrentOrViewCamera()->SetStereoPosition(this->frame->GetViewModule()->IsStereoscopyEnabled() ? Camera::STEREO : Camera::NEUTRAL);

		this->frame->GetCurrentOrViewCamera()->SetDefaultCanvasSize(w, h);
	}

	//std::cout << "onpaint rendering" << std::endl;

	/*MeshModule *mmodule = dynamic_cast<MeshModule *>(this->frame->GetCurrentModule());

    if (mmodule)
    {
        mmodule->StartGaugeTimer();
    }*/

	this->RenderAll(w, h);

    /*if (mmodule)
    {
        mmodule->EndGaugeTimer();
    }*/

    //std::cout << "onpaint flushing" << std::endl;

    glFlush();

    //std::cout << "onpaint rendering texture" << std::endl;

	//std::cout << "onpaint swapping buffers" << std::endl;

	this->SwapBuffers();

	//std::cout << "onpaint video operations" << std::endl;

#if USE_OPENCV
	if (this->video.isOpened())
	{
		wxImage *img = this->Screenshot(/*1*/);

		cv::Mat mat(img->GetHeight(), img->GetWidth(), CV_8UC3, img->GetData());
		IplImage iplimg = mat;
		cvConvertImage(&iplimg, &iplimg, CV_CVTIMG_SWAP_RB | CV_CVTIMG_FLIP);

		mat = cv::Mat(&iplimg);

		this->video << mat;

		delete img;
	}
#endif //#if USE_OPENCV

	//std::cout << "onpaint animation operations" << std::endl;

	if (this->frame->GetViewModule()->IsAnimationEnabled())
	{
		this->frame->GetViewModule()->AnimateCamera();

		if (!timerAnim.IsRunning())
		{
			//timerAnim.Start(1000/15);
			timerAnim.Start(1000/25);

			this->Refresh(false);
			this->Update();
		}
	}
	else if (timerAnim.IsRunning())
	{
		this->timerAnim.Stop();
	}

	//std::cout << "onpaint ending" << std::endl;
}

void CRAbMesh::Canvas::OnEraseBackground(wxEraseEvent &WXUNUSED(event))
{

}

void CRAbMesh::Canvas::OnMouseLeftDown(wxMouseEvent &event)
{
	if (!this->frame->GetCurrentOrViewCamera())
	{
		return;
	}

    this->frame->GetCurrentOrViewCamera()->SetLast(event.GetX(), event.GetY());

    if (this->frame->GetCurrentOrViewModule()->OnLeftDown(event.GetX(), event.GetY()))
	{
		this->Refresh(false);
	    this->Update();

		return;
	}

    if (this->frame->GetViewModule()->GetMouseOption() == ViewModule::MOUSE_PICK)
	{
	    this->Pick(event.GetX(), event.GetY(), false);
	    this->Refresh(false);
	    this->Update();
	}
}

void CRAbMesh::Canvas::OnMouseMiddleDown(wxMouseEvent &WXUNUSED(event))
{
	this->frame->GetViewModule()->ToggleAnimation();
}

void CRAbMesh::Canvas::OnMouseRightDown(wxMouseEvent &WXUNUSED(event))
{
	wxString title;

	MeshesModule *module = dynamic_cast<MeshesModule *>(this->frame->GetCurrentModule());

	if ((module) && (!module->GetPicked().empty()))
	{
		wxMenu *picked = module->GetPickedMenu(title);
		wxMenu *mouse = this->frame->GetViewModule()->GetMouseMenu(title);

		wxMenu *m = new wxMenu();

		wxMenuItemList pitems = picked->GetMenuItems();
		wxMenuItemList mitems = mouse->GetMenuItems();

		for (wxMenuItemList::iterator iter = pitems.begin();
			 iter != pitems.end(); iter++)
		{
			picked->Remove((*iter));
			m->Append((*iter));
		}

		m->AppendSeparator();

		for (wxMenuItemList::iterator iter = mitems.begin();
			 iter != mitems.end(); iter++)
		{
			mouse->Remove((*iter));
			m->Append((*iter));
		}

		this->PopupMenu(m);

		for (wxMenuItemList::iterator iter = pitems.begin();
			 iter != pitems.end(); iter++)
		{
			m->Remove((*iter));
			picked->Append((*iter));
		}

		for (wxMenuItemList::iterator iter = mitems.begin();
			 iter != mitems.end(); iter++)
		{
			m->Remove((*iter));
			mouse->Append((*iter));
		}

		delete m;
	}
	else
	{
		wxMenu *m = this->frame->GetViewModule()->GetMouseMenu(title);

		m->Detach();

		this->PopupMenu(m);

		m->Attach(this->frame->GetMenuBar());
	}
}

void CRAbMesh::Canvas::OnMouseMotion(wxMouseEvent &event)
{
 	if (!this->frame->GetCurrentOrViewCamera())
	{
		return;
	}

    int w, h;
 	this->GetClientSize(&w, &h);

 	this->SetCurrent(*this->glContext);

 	Camera *camera = this->frame->GetCurrentOrViewCamera();

    int x = event.GetX();
 	int y = event.GetY();

	if ((event.LeftIsDown()) && (this->frame->GetCurrentOrViewModule()->IsMouseMotionAllowed()))
    {
    	double mx, my, mz;

		camera->Unproject(x, y, mx, my, mz);

		if (this->frame->GetViewModule()->GetMouseOption() == ViewModule::MOUSE_PICK)
		{
			this->Pick(x, y, true);

			this->Refresh(false);
			this->Update();
		}
		else if (this->frame->GetViewModule()->GetMouseOption() == ViewModule::MOUSE_ROTATE)
		{
		 	camera->Rotate(x, y);

			this->Refresh(false);
			this->Update();
		}
		else if (this->frame->GetViewModule()->GetMouseOption() == ViewModule::MOUSE_TURN)
		{
		 	camera->Turn(x, y);

		 	this->Refresh(false);
		 	this->Update();
		}
		else if (this->frame->GetViewModule()->GetMouseOption() == ViewModule::MOUSE_LOOK)
		{
		 	camera->Look(x, y);

		 	this->Refresh(false);
		 	this->Update();
		}
		else if (this->frame->GetViewModule()->GetMouseOption() == ViewModule::MOUSE_TRANSLATE)
		{
		 	camera->Translate(x, y);

		 	this->Refresh(false);
		 	this->Update();
		}
		else if (this->frame->GetViewModule()->GetMouseOption() == ViewModule::MOUSE_MOVE)
		{
		 	camera->Move(x, y);

		 	this->Refresh(false);
		 	this->Update();
		}
		else if (this->frame->GetViewModule()->GetMouseOption() == ViewModule::MOUSE_OPTICAL_ZOOM)
		{
		 	camera->OpticalZoom(x, y);

		 	this->Refresh(false);
		 	this->Update();
		}
		else if (this->frame->GetViewModule()->GetMouseOption() == ViewModule::MOUSE_DIGITAL_ZOOM)
		{
		 	camera->DigitalZoom(x, y);

		 	this->Refresh(false);
		 	this->Update();
		}
		else if (this->frame->GetViewModule()->GetMouseOption() == ViewModule::MOUSE_MERGED_ZOOM)
		{
		 	camera->MergedZoom(x, y);

		 	this->Refresh(false);
		 	this->Update();
		}
	}

	camera->SetLast(x, y);
}

void CRAbMesh::Canvas::OnMouseScroll(wxMouseEvent &event)
{
	if (!this->frame->GetCurrentOrViewCamera())
	{
		return;
	}

	this->SetCurrent(*this->glContext);

	Camera *camera = this->frame->GetCurrentOrViewCamera();

	camera->OpticalZoom(event.GetLinesPerAction()*event.GetWheelRotation()/event.GetWheelDelta());

	int w, h;
 	this->GetClientSize(&w, &h);

	double mx, my, mz;

	camera->Unproject(event.GetX(), event.GetY(), mx, my, mz);

	this->Refresh(false);
}

void CRAbMesh::Canvas::OnThread(wxCommandEvent &event)
{
	this->thread = NULL;

	if (event.GetInt() != Thread::PENDING)
	{
		this->pending = false;

		//this->timer.Stop();
	}

	//std::cout << "stopping timer onthread" << std::endl;
	this->timer.Stop();

	//std::cout << "refreshing" << std::endl;
	this->Refresh(false);
	this->Update();
	//std::cout << "end refreshing" << std::endl;

    if (this->clearing)
    {
    	this->clearing = false;

    	this->pending = false;

    	//this->timer.Stop();
    }

    if (!this->pending)
    {
#if USE_OPENCV
    	//this->StopVideo();
#endif //#if USE_OPENCV
    }

    //std::cout << "clearing display lists" << std::endl;
    this->frame->GetViewModule()->ClearDisplayLists(true);
    //std::cout << "end clearing display lists" << std::endl;

    this->frame->GetCurrentModule()->Ready(clearing, pending);
}

void CRAbMesh::Canvas::OnTimer(wxTimerEvent &WXUNUSED(event))
{
	//std::cout << "timer" << std::endl;

	//std::cout << "clearing display lists" << std::endl;
	this->frame->GetViewModule()->ClearDisplayLists(false);
	//std::cout << "end clearing display lists" << std::endl;

	this->frame->GetCurrentOrViewModule()->OnTimer();

	//std::cout << "end timer" << std::endl;

	this->Refresh(false);
	this->Update();
}

void CRAbMesh::Canvas::ResetView()
{
	if (!this->frame->GetCurrentOrViewCamera())
	{
		return;
	}

	this->FitCamera();

	Camera *camera = this->frame->GetCurrentOrViewCamera();

    camera->Reset();

    switch (this->frame->GetViewModule()->GetViewOption())
    {
	case ViewModule::VIEW_FRONT:
		break;
	case ViewModule::VIEW_BACK:
		camera->Rotate(0.0, 1.0, 0.0, M_PI);
		break;
	case ViewModule::VIEW_RIGHT:
		camera->Rotate(0.0, 1.0, 0.0, M_PI*0.5);
		break;
	case ViewModule::VIEW_LEFT:
		camera->Rotate(0.0, 1.0, 0.0, M_PI*1.5);
		break;
	case ViewModule::VIEW_TOP:
		camera->Rotate(1.0, 0.0, 0.0, M_PI*1.5);
		break;
	case ViewModule::VIEW_BOTTOM:
		camera->Rotate(1.0, 0.0, 0.0, M_PI*0.5);
		break;
	case ViewModule::VIEW_DIAGONAL:
		camera->Rotate(0.0, 1.0, 0.0, M_PI*0.25);
		camera->Rotate(1.0, 0.0, -1.0, -M_PI*0.25);
		break;
    }

    this->Refresh(false);
    this->Update();
}

void CRAbMesh::Canvas::ResetProjection()
{
	ModuleMap modules = this->frame->GetModules();

	for (ModuleMap::iterator iter = modules.begin();
		 iter != modules.end(); iter++)
	{
		if (!(*iter).second)
		{
			continue;
		}

		if (!(*iter).second->GetCamera())
		{
			int w, h;

			this->GetClientSize(&w, &h);

			(*iter).second->SetCamera(new Camera(w, h));
		}

		bool perspective = (this->frame->GetViewModule()->GetProjectionOption() == ViewModule::PROJECTION_PERSPECTIVE);

		(*iter).second->GetCamera()->SetPerspective(perspective);
	}
}

void CRAbMesh::Canvas::FitCamera()
{
    ///TODO remove this method from here and put it in ViewModule

    int w, h;
	this->GetClientSize(&w, &h);

	Camera *camera = this->frame->GetCurrentOrViewCamera();

	if (!camera)
	{
		camera = new Camera(w, h);

		this->frame->SetCurrentOrViewCamera(camera);
	}

	if (camera->IsFitted())
	{
		return;
	}

    camera->SetDefaultCanvasSize(w, h);

    if (this->frame->GetCurrentModule())
    {
    	double minx, miny, minz, maxx, maxy, maxz;

		if (this->frame->GetCurrentModule()->BoundingBox(minx, miny, minz, maxx, maxy, maxz))
		{
			camera->Fit(minx, miny, maxx, maxy, minz, maxz);

			Data::getState().setBox(Data::Box(Data::Point3D(minx, miny, minz), Data::Point3D(maxx, maxy, maxz)));
			Data::getState().setMid(Data::Point3D((minx + maxx)*0.5, (miny + maxy)*0.5, (minz + maxz)*0.5));
		}
    }

    this->Refresh(false);
}

void CRAbMesh::Canvas::Center(const Data::Point *p)
{
	this->frame->GetCurrentOrViewCamera()->Center(
		p->getCoord(0) - Data::getTolerance(), p->getCoord(1) - Data::getTolerance(),
		p->getCoord(0) + Data::getTolerance(), p->getCoord(1) + Data::getTolerance(),
		p->getCoord(2) - Data::getTolerance(), p->getCoord(2) + Data::getTolerance());
}

void CRAbMesh::Canvas::Center(const Data::GeometricShape *s)
{
	Data::Box b = s->box();

	this->frame->GetCurrentOrViewCamera()->Center(
		b.getMin()->getCoord(0), b.getMin()->getCoord(1),
		b.getMax()->getCoord(0), b.getMax()->getCoord(1),
		b.getMin()->getCoord(2), b.getMax()->getCoord(2));
}

wxImage *CRAbMesh::Canvas::Screenshot(/*int pointsPerPixel*/)
{
	if (!this->frame->GetCurrentOrViewCamera())
	{
		return NULL;
	}

	int w, h;

	this->GetClientSize(&w, &h);

	this->SetOpenGLStateBeforeRendering();

	this->RenderAll(w, h);

	glFlush();

	glFinish();

	unsigned char *c = new unsigned char[w*h*4];
	unsigned char *data = new unsigned char[w*h*3];

	//for some unknown reason, this is not working
	//glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, c);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			data[(x + y*w)*3 + 0] = c[(x + y*w)*4 + 0];
			data[(x + y*w)*3 + 1] = c[(x + y*w)*4 + 1];
			data[(x + y*w)*3 + 2] = c[(x + y*w)*4 + 2];
		}
	}

	wxImage *img = new wxImage(w, h, data);

	delete [] c;

	return img;
}

void CRAbMesh::Canvas::Save(wxImage *img, const std::string &filename)
{
	wxString name(filename.c_str(), wxMBConvLibc());

	wxImage img2 = img->Mirror(false);

	img2.SaveFile(name, wxBITMAP_TYPE_PNG);
}

void CRAbMesh::Canvas::SaveScreenshot(const std::string &filename/*, int pointsPerPixel*/)
{
	wxImage *img = this->Screenshot(/*pointsPerPixel*/);

	this->Save(img, filename);

	delete img;
}

#if USE_OPENCV
void CRAbMesh::Canvas::SaveVideo(const std::string &filename/*, int fps*/)
{
	int w, h;
	this->GetClientSize(&w, &h);

	this->video.open(filename,
		//CV_FOURCC('P', 'I', 'M', '1'), //MPEG-1 codec (http://www.cs.iit.edu/~agam/cs512/lect-notes/opencv-intro/opencv-intro.html)
		CV_FOURCC('M', 'J', 'P', 'G'), //motion-jpeg codec (does not work well) //best so far
		//CV_FOURCC('M', 'P', '4', '2'), //MPEG-4.2 codec
		//CV_FOURCC('D', 'I', 'V', '3'), //MPEG-4.3 codec
		//CV_FOURCC('D', 'I', 'V', 'X'), //MPEG-4 codec
		//CV_FOURCC('U', '2', '6', '3'), //H263 codec
		//CV_FOURCC('I', '2', '6', '3'), //H263I codec
		//CV_FOURCC('F', 'L', 'V', '1'), //FLV1 codec
		//fps, cv::Size_<int>(w, h), true);
		30, cv::Size_<int>(w, h), true);
}

void CRAbMesh::Canvas::StopVideo()
{
	this->video = cv::VideoWriter();

	this->frame->CheckVideoState(false);
}
#endif //#if USE_OPENCV

void CRAbMesh::Canvas::Clear(bool destroying)
{
#if USE_OPENCV
	this->StopVideo();
#endif //#if USE_OPENCV

    if (this->frame->GetCurrentOrViewCamera())
    {
    	this->frame->GetCurrentOrViewCamera()->SetFitted(false);
	}

    //std::cout << "stopping timer clear" << std::endl;
    this->timer.Stop();

    if ((this->thread) &&
		(this->thread->IsRunning() || this->thread->IsPaused()))
	{
		this->clearing = true;

		this->thread->Delete();
	}
	else
	{
		this->pending = false;

		this->thread = NULL;

		if (this->frame->GetCurrentModule())
		{
			this->frame->GetCurrentModule()->Clear(true, destroying);
		}
	}

	this->Refresh(false);
}

void CRAbMesh::Canvas::Execute()
{
    //this->frame->GetCurrentModule()->SetParameters(!this->pending);
    this->frame->GetCurrentModule()->SetParameters(
		this->frame->GetCurrentModule()->GetDrive()->getStatus() == this->frame->GetCurrentModule()->GetDrive()->notInit);

    //when clock is paused, then the execution already started previously
    if (!this->pending)
    {
		this->pending = true;
    }

    if (!this->thread)
	{
		this->thread = new Thread(this, this->frame->GetCurrentModule());

		if (this->thread->Create() != wxTHREAD_NO_ERROR)
		{
			this->thread->Delete();

			wxMessageBox(_("Error in creating thread"), _("Error"));

			this->thread = NULL;

			return;
		}
	}

	if (this->thread)
	{
		//std::cout << "starting timer" << std::endl;
		this->timer.Start(1000);

		if ((this->pending) ||
			(!this->thread->IsPaused()))
		{
			this->thread->Run();
		}
		else
		{
			this->thread->Resume();
		}

		this->FitCamera();
	}
}

#endif //#if USE_GUI
