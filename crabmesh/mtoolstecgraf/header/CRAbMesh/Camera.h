#ifndef _CRAB_MESH_CAMERA_H_
#define _CRAB_MESH_CAMERA_H_

#include "CRAbMesh/Definitions.h"

#if USE_GUI

namespace CRAbMesh
{
	std::istream &operator>>(std::istream &stream, Camera &c);
	std::ostream &operator<<(std::ostream &stream, const Camera &c);

	class Camera
	{
	public:

		enum StereoPosition
		{
			NEUTRAL = 0,
			STEREO,
			LEFT,
			RIGHT
		};

	public:

		Camera(int w, int h);
		virtual ~Camera();

		virtual void SetPerspective(bool perspective);
		virtual bool IsPerspective() const;

		virtual void SetStereoPosition(enum CRAbMesh::Camera::StereoPosition stereoPosition);
		virtual enum CRAbMesh::Camera::StereoPosition GetStereoPosition() const;

		virtual void SetPosition(double x, double y, double z);
		virtual double GetPosition(unsigned int index) const;

		virtual void SetFront(double x, double y, double z);
		virtual double GetFront(unsigned int index) const;

		virtual void SetUp(double x, double y, double z);
		virtual double GetUp(unsigned int index) const;

		virtual void SetDefaultPosition(double x, double y, double z);
		virtual double GetDefaultPosition(unsigned int index) const;

		virtual void SetDefaultFront(double x, double y, double z);
		virtual double GetDefaultFront(unsigned int index) const;

		virtual void SetDefaultUp(double x, double y, double z);
		virtual double GetDefaultUp(unsigned int index) const;

		virtual void SetLast(int x, int y);
		virtual int GetLastX() const;
		virtual int GetLastY() const;

		virtual void SetCanvasSize(int w, int h);
		virtual void SetDefaultCanvasSize(int w, int h);
		virtual int GetCanvasWidth() const;
		virtual int GetCanvasHeight() const;

		virtual void SetDigitalZoom(double digitalZoom);
		virtual double GetDigitalZoom() const;

		//virtual void SetMergedZoom(bool mergedZoom);
		//virtual bool IsMergedZoom() const;

		virtual void StartPick(int x, int y);
		virtual int EndPick();

		virtual void SetFitted(bool fitted);
		virtual bool IsFitted() const;

		virtual void Fit(double minx, double miny, double maxx, double maxy, double minz, double maxz);
		virtual void Center(double minx, double miny, double maxx, double maxy, double minz, double maxz);

		//method that rotates the camera. x and y are the mouse position
		virtual void Rotate(int x, int y);
		virtual void Rotate(double x, double y, double z, double angle);
		//method that rotates the camera according to the (front-position) axis.
		//  x and y are the mouse position
		virtual void Turn(int x, int y);
		virtual void Look(int x, int y);
		//method that translates the camera. x and y are the mouse position
		virtual void Translate(int x, int y);
		//method that moves the camera. x and y are the mouse position
		virtual void Move(int x, int y);
		//method that zooms in or out the camera. x and y are the mouse position
		virtual void OpticalZoom(int x, int y);
		//method that zooms in or out the camera. disp is the vertical displacement
		virtual void OpticalZoom(int disp);
		//method that zooms in or out without moving the camera, just changing the window/viewport rate
		virtual void DigitalZoom(int x, int y);
		virtual void DigitalZoom(int disp);
		//method that zooms in or out merging OpticalZoom() and DigitalZoom()
		virtual void MergedZoom(int x, int y);
		virtual void MergedZoom(int disp);

		virtual void Base(double &ix, double &iy, double &iz,
						  double &jx, double &jy, double &jz,
						  double &kx, double &ky, double &kz);

		//(x, y, z, w) is a quaternion
		//virtual void Apply(double x, double y, double z, double w);

		virtual void Reset();

		virtual double Distance() const;

		virtual void Unproject(double x, double y, double &mx, double &my, double &mz);
		virtual void Unproject(double x, double y, double z, double &mx, double &my, double &mz);

		virtual void Projection();

		virtual void Draw();
		virtual void DrawModelview();

		friend std::istream &CRAbMesh::operator>>(std::istream &stream, Camera &c);
		friend std::ostream &CRAbMesh::operator<<(std::ostream &stream, const Camera &c);

	protected:

		static double Sign(double x);
		static double Size(double x, double y, double z);
		virtual void GetMatrices();

	public:

		enum
		{
			X = 0,
			Y,
			Z,
			W
		};

	protected:

		//vectors that deal with the camera: camera position, point where the
		// camera is looking at, and the up vector of the camera
		double position[3];
		double front[3];
		double up[3];

		//default values for the camera, initially defined and changed in
		// method fit()
		double defaultPosition[3];
		double defaultFront[3];
		double defaultUp[3];

		//last position of the camera
		int lastX;
		int lastY;

		//size of the canvas
		int w;
		int h;

		//default values for viewport size
		int defaultW;
		int defaultH;

		//magic constant
		double d;

		bool perspective;

		//frustum parameters
		double left;
		double right;
		double bottom;
		double top;
		double near;
		double far;

		double digitalZoom;
		bool mergedZoom;

		//tells if the camera has fitted or not
		bool fitted;

		//matrices used by opengl
		int viewport[4];
		double projection[16];
		double modelview[16];

		//stereoscopy
		StereoPosition stereoPosition;

		double distNeutral; //distance of displaced camera to neutral camera
		double focus; //plane of focus (in camera coordinates)
		double shift; //shift in frustum, when stereoscopy is enabled
		bool stencilSet;
	};
}

#endif //#if USE_GUI

#endif //#ifndef _CRAB_MESH_CAMERA_H_
