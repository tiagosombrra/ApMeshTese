#include "CRAbMesh/Camera.h"

#if USE_GUI

#include <cmath>

CRAbMesh::Camera::Camera(int w, int h)
{
    this->w = this->h = 0;
    this->stencilSet = false;

    this->d = 0.01;
    this->digitalZoom = 1.0;
    this->mergedZoom = false;

    this->shift = 0.0;
    this->distNeutral = 0.5;
    this->focus = 16.5;

    this->SetStereoPosition(NEUTRAL);
    this->SetPerspective(true);

    this->SetDefaultUp(0.0, 1.0, 0.0);
    this->SetDefaultFront(0.0, 0.0, 0.0);
    this->SetDefaultPosition(0.0, 0.0, 20.0);

    this->SetDefaultCanvasSize(w, h);

    this->SetFitted(false);

    this->Reset();

    this->Draw();
}

CRAbMesh::Camera::~Camera()
{

}

void CRAbMesh::Camera::SetStereoPosition(enum CRAbMesh::Camera::StereoPosition stereoPosition)
{
	this->stereoPosition = stereoPosition;
}

enum CRAbMesh::Camera::StereoPosition CRAbMesh::Camera::GetStereoPosition() const
{
	return this->stereoPosition;
};

void CRAbMesh::Camera::SetPerspective(bool perspective)
{
	this->perspective = perspective;
}

bool CRAbMesh::Camera::IsPerspective() const
{
	return this->perspective;
}

void CRAbMesh::Camera::SetPosition(double x, double y, double z)
{
    this->position[X] = x;
    this->position[Y] = y;
    this->position[Z] = z;
}

double CRAbMesh::Camera::GetPosition(unsigned int index) const
{
    return (index < 3) ? this->position[index] : 0.0;
}

void CRAbMesh::Camera::SetFront(double x, double y, double z)
{
    this->front[X] = x;
    this->front[Y] = y;
    this->front[Z] = z;
}

double CRAbMesh::Camera::GetFront(unsigned int index) const
{
    return (index < 3) ? this->front[index] : 0.0;
}

void CRAbMesh::Camera::SetUp(double x, double y, double z)
{
    this->up[X] = x;
    this->up[Y] = y;
    this->up[Z] = z;
}

double CRAbMesh::Camera::GetUp(unsigned int index) const
{
    return (index < 3) ? this->up[index] : 0.0;
}

void CRAbMesh::Camera::SetDefaultPosition(double x, double y, double z)
{
    this->defaultPosition[X] = x;
    this->defaultPosition[Y] = y;
    this->defaultPosition[Z] = z;
}

double CRAbMesh::Camera::GetDefaultPosition(unsigned int index) const
{
	return (index < 3) ? this->defaultPosition[index] : 0.0;
}

void CRAbMesh::Camera::SetDefaultFront(double x, double y, double z)
{
    this->defaultFront[X] = x;
    this->defaultFront[Y] = y;
    this->defaultFront[Z] = z;
}

double CRAbMesh::Camera::GetDefaultFront(unsigned int index) const
{
	return (index < 3) ? this->defaultFront[index] : 0.0;
}

void CRAbMesh::Camera::SetDefaultUp(double x, double y, double z)
{
    this->defaultUp[X] = x;
    this->defaultUp[Y] = y;
    this->defaultUp[Z] = z;
}

double CRAbMesh::Camera::GetDefaultUp(unsigned int index) const
{
	return (index < 3) ? this->defaultUp[index] : 0.0;
}

void CRAbMesh::Camera::SetLast(int x, int y)
{
    this->lastX = x;
    this->lastY = y;
}

int CRAbMesh::Camera::GetLastX() const
{
	return this->lastX;
}

int CRAbMesh::Camera::GetLastY() const
{
	return this->lastY;
}

void CRAbMesh::Camera::SetCanvasSize(int w, int h)
{
    if (this->stereoPosition == NEUTRAL)
	{
		glDisable(GL_STENCIL_TEST);

		this->stencilSet = false;
	}
	else if ((w != this->w) || (h != this->h) || (!this->stencilSet))
	{
		int psize = w*h;

		unsigned char *pattern = new unsigned char[psize];

		for (int i = 0; i < psize; i++)
		{
			pattern[i] = static_cast<unsigned char>((i == 0) ? 0 : ((i < w) ? 1 - pattern[i - 1] : 1 - pattern[i - w]));
		}

		glEnable(GL_STENCIL_TEST);

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glWindowPos2i(0, 0);
		glDrawPixels(w, h, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pattern);

		delete [] pattern;

		this->stencilSet = true;
	}

	this->w = w;
    this->h = h;

    glViewport(0, 0, this->w, this->h);

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
	//this->Projection();
}

void CRAbMesh::Camera::SetDefaultCanvasSize(int w, int h)
{
	this->defaultW = w;
    this->defaultH = h;

    this->SetCanvasSize(this->defaultW, this->defaultH);
}

int CRAbMesh::Camera::GetCanvasWidth() const
{
	return this->w;
}

int CRAbMesh::Camera::GetCanvasHeight() const
{
	return this->h;
}

void CRAbMesh::Camera::SetDigitalZoom(double digitalZoom)
{
	this->digitalZoom = digitalZoom;
}

double CRAbMesh::Camera::GetDigitalZoom() const
{
	return this->digitalZoom;
}

/*void CRAbMesh::Camera::SetMergedZoom(bool mergedZoom)
{
	this->mergedZoom = mergedZoom;
}

bool CRAbMesh::Camera::IsMergedZoom() const
{
	return this->mergedZoom;
}*/

void CRAbMesh::Camera::StartPick(int x, int y)
{
    glRenderMode(GL_SELECT);

    glInitNames();
    //glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
		glLoadIdentity();
		gluPickMatrix(static_cast<double>(x), static_cast<double>(this->h - y), 10.0, 10.0, this->viewport);
		this->Projection();

	this->DrawModelview();
}

int CRAbMesh::Camera::EndPick()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glFlush();

	return static_cast<int>(glRenderMode(GL_RENDER));
}

void CRAbMesh::Camera::SetFitted(bool fitted)
{
	this->fitted = fitted;
}

bool CRAbMesh::Camera::IsFitted() const
{
	return this->fitted;
}

void CRAbMesh::Camera::Fit(double minx, double miny, double maxx, double maxy, double minz, double maxz)
{
	double x, y, z, spanHor, spanVer, spanWid, spanHei, zhor, zver;
	double xm, ym, xw, yh, factor = 0.95/this->d;

	this->SetDefaultPosition(0.0, 0.0, 1.0);
    this->SetDefaultFront(0.0, 0.0, 0.0);
    this->SetDefaultUp(0.0, 1.0, 0.0);
    this->Reset();
    this->Draw();

	//this->GetMatrices();

	this->Unproject(0.0, this->h, xm, ym, z);
	this->Unproject(this->w, 0.0, xw, yh, z);

	x = (maxx + minx)*0.5;
	y = (maxy + miny)*0.5;

	spanHor = maxx - minx;
	spanVer = maxy - miny;

	spanWid = (xw - xm)*factor*z;
	spanHei = (yh - ym)*factor*z;

	zhor = z*spanHor/spanWid;
	zver = z*spanVer/spanHei;

	z = std::max(zhor, zver) + maxz;

    this->SetDefaultFront(x, y, (minz + maxz)*0.5);
    this->SetDefaultPosition(x, y, z);
    this->Reset();

    //std::cout << "pos = (" << this->GetPosition(0) << ", " << this->GetPosition(1) << ", " << this->GetPosition(2) << ")\n";
    //std::cout << "frn = (" << this->GetFront(0) << ", " << this->GetFront(1) << ", " << this->GetFront(2) << ")" << std::endl;
    //std::cout << "up  = (" << this->GetUp(0) << ", " << this->GetUp(1) << ", " << this->GetUp(2) << ")" << std::endl;

    this->SetFitted(true);

    this->Draw();
}

void CRAbMesh::Camera::Center(double minx, double miny, double maxx, double maxy, double minz, double maxz)
{
	double c[3], v[3];

	c[X] = (minx + maxx)*0.5;
	c[Y] = (miny + maxy)*0.5;
	c[Z] = (minz + maxz)*0.5;

	v[X] = this->position[X] - this->front[X];
	v[Y] = this->position[Y] - this->front[Y];
	v[Z] = this->position[Z] - this->front[Z];

	double d = 4.0*this->Size(minx - c[X], miny - c[Y], minz - c[Z]);

	if (d < this->d)
	{
		d = this->d;
	}

	d /= this->Size(v[X], v[Y], v[Z]);

	//Translation
	this->front[X] = c[X];
	this->front[Y] = c[Y];
	this->front[Z] = c[Z];

	//Zoom
	this->position[X] = this->front[X] + v[X]*d;
	this->position[Y] = this->front[Y] + v[Y]*d;
	this->position[Z] = this->front[Z] + v[Z]*d;
}

void CRAbMesh::Camera::Rotate(int x, int y)
{
	double m[3], l[3], v[3], nz[3], nx[3];
	double size, sizeOverD, x1, y1, z1, a, b, c, delta, t;
	double x2lessx1, y2lessy1, z2lessz1, x1lessx0, y1lessy0, z1lessz0;

	//this->GetMatrices();

	this->Unproject(x, y, m[X], m[Y], m[Z]);
	this->Unproject(this->lastX, this->lastY, l[X], l[Y], l[Z]);

	nz[X] = this->front[X] - this->position[X];
	nz[Y] = this->front[Y] - this->position[Y];
	nz[Z] = this->front[Z] - this->position[Z];

	size = this->Size(nz[X], nz[Y], nz[Z]);
	sizeOverD = this->perspective ? size/(this->d*this->digitalZoom) : size*0.1;

	v[X] = (l[X] - m[X])*sizeOverD;
	v[Y] = (l[Y] - m[Y])*sizeOverD;
	v[Z] = (l[Z] - m[Z])*sizeOverD;

	nx[X] = this->up[Y]*nz[Z] - this->up[Z]*nz[Y];
    nx[Y] = this->up[Z]*nz[X] - this->up[X]*nz[Z];
    nx[Z] = this->up[X]*nz[Y] - this->up[Y]*nz[X];

	x1 = this->position[X] + v[X];
	y1 = this->position[Y] + v[Y];
	z1 = this->position[Z] + v[Z];

	x2lessx1 = this->front[X] - x1;
	y2lessy1 = this->front[Y] - y1;
	z2lessz1 = this->front[Z] - z1;

	x1lessx0 = -x2lessx1;
	y1lessy0 = -y2lessy1;
	z1lessz0 = -z2lessz1;

	a = (x2lessx1)*(x2lessx1) + (y2lessy1)*(y2lessy1) + (z2lessz1)*(z2lessz1);
	b = 2.0*(x2lessx1*x1lessx0 + y2lessy1*y1lessy0 + z2lessz1*z1lessz0);
	c = x1lessx0*x1lessx0 + y1lessy0*y1lessy0 + z1lessz0*z1lessz0 - size*size;

	delta = b*b - 4.0*a*c;
	t = (-b + std::sqrt(delta))/(2.0*a);

	if ((t < 0.0) || (t > 1.0))
	{
	   	t = (-b - std::sqrt(delta))/(2.0*a);
	}

	this->position[X] = x1 + (x2lessx1)*t;
	this->position[Y] = y1 + (y2lessy1)*t;
	this->position[Z] = z1 + (z2lessz1)*t;

	nz[X] = this->front[X] - this->position[X];
	nz[Y] = this->front[Y] - this->position[Y];
	nz[Z] = this->front[Z] - this->position[Z];

	this->up[X] = nz[Y]*nx[Z] - nz[Z]*nx[Y];
 	this->up[Y] = nz[Z]*nx[X] - nz[X]*nx[Z];
 	this->up[Z] = nz[X]*nx[Y] - nz[Y]*nx[X];

 	size = this->Size(this->up[X], this->up[Y], this->up[Z]);

 	this->up[X] /= size;
 	this->up[Y] /= size;
 	this->up[Z] /= size;

 	this->SetLast(x, y);
}

void CRAbMesh::Camera::Rotate(double x, double y, double z, double angle)
{
	double len, v[3], d, aux[3], sin, cos, up[3];

    len = 1.0/this->Size(x, y, z);

    x *= len;
    y *= len;
    z *= len;

    d = this->Size(0.0, y, z);

	//translation to origin
	v[X] = this->position[X] - this->front[X];
	v[Y] = this->position[Y] - this->front[Y];
	v[Z] = this->position[Z] - this->front[Z];

	up[X] = this->up[X];
	up[Y] = this->up[Y];
	up[Z] = this->up[Z];

	//rotation to X axis
	if (d > 0.0001)
	{
		aux[Y] = z/d*v[Y] - y/d*v[Z];
		aux[Z] = y/d*v[Y] + z/d*v[Z];

		v[Y] = aux[Y];
		v[Z] = aux[Z];

		aux[Y] = z/d*up[Y] - y/d*up[Z];
		aux[Z] = y/d*up[Y] + z/d*up[Z];

		up[Y] = aux[Y];
		up[Z] = aux[Z];
	}

	//rotation to Y axis
	aux[X] = d*v[X] - x*v[Z];
	aux[Z] = x*v[X] + d*v[Z];

	v[X] = aux[X];
	v[Z] = aux[Z];

	aux[X] = d*up[X] - x*up[Z];
	aux[Z] = x*up[X] + d*up[Z];

	up[X] = aux[X];
	up[Z] = aux[Z];

	//rotation to Z axis
	sin = std::sin(angle);
	cos = std::cos(angle);

	aux[X] = v[X]*cos - v[Y]*sin;
	aux[Y] = v[X]*sin + v[Y]*cos;

	v[X] = aux[X];
	v[Y] = aux[Y];

	aux[X] = up[X]*cos - up[Y]*sin;
	aux[Y] = up[X]*sin + up[Y]*cos;

	up[X] = aux[X];
	up[Y] = aux[Y];

	//rotation to Y axis - inverse
	aux[X] = d*v[X] + x*v[Z];
	aux[Z] = -x*v[X] + d*v[Z];

	v[X] = aux[X];
	v[Z] = aux[Z];

	aux[X] = d*up[X] + x*up[Z];
	aux[Z] = -x*up[X] + d*up[Z];

	up[X] = aux[X];
	up[Z] = aux[Z];

	//rotation to X axis - inverse
	if (d > 0.0001)
	{
		aux[Y] = z/d*v[Y] + y/d*v[Z];
		aux[Z] = -y/d*v[Y] + z/d*v[Z];

		v[Y] = aux[Y];
		v[Z] = aux[Z];

		aux[Y] = z/d*up[Y] + y/d*up[Z];
		aux[Z] = -y/d*up[Y] + z/d*up[Z];

		up[Y] = aux[Y];
		up[Z] = aux[Z];
	}

	//translation to origin - inverse
	v[X] += this->front[X];
	v[Y] += this->front[Y];
	v[Z] += this->front[Z];

	this->SetPosition(v[X], v[Y], v[Z]);
	this->SetUp(up[X], up[Y], up[Z]);
}

void CRAbMesh::Camera::Turn(int x, int y)
{
	if ((x == this->w/2) && (y == this->h/2))
	{
		return;
	}

	double m[2], l[2];

	//this->GetMatrices();

	m[X] = static_cast<double>(x) - static_cast<double>(this->w)*0.5;
	m[Y] = static_cast<double>(y) - static_cast<double>(this->h)*0.5;

	l[X] = static_cast<double>(this->lastX) - static_cast<double>(this->w)*0.5;
	l[Y] = static_cast<double>(this->lastY) - static_cast<double>(this->h)*0.5;

	double angle = std::acos((m[X]*l[X] + m[Y]*l[Y])/(this->Size(m[X], m[Y], 0.0)*this->Size(l[X], l[Y], 0.0)));

	angle = ((m[X]*l[Y] - m[Y]*l[X]) > 0.0) ? -angle : angle;

	this->Rotate(this->position[X] - this->front[X],
				 this->position[Y] - this->front[Y],
				 this->position[Z] - this->front[Z],
				 angle);
}

void CRAbMesh::Camera::Look(int x, int y)
{
	double m[3], l[3], v[3], nz[3], nx[3];
	double size, sizeOverD, x2, y2, z2, a, b, c, delta, t;
	double x2lessx1, y2lessy1, z2lessz1, x1lessx0, y1lessy0, z1lessz0;

	//this->GetMatrices();

	this->Unproject(x, y, m[X], m[Y], m[Z]);
	this->Unproject(this->lastX, this->lastY, l[X], l[Y], l[Z]);

	nz[X] = this->front[X] - this->position[X];
	nz[Y] = this->front[Y] - this->position[Y];
	nz[Z] = this->front[Z] - this->position[Z];

	size = this->Size(nz[X], nz[Y], nz[Z]);
	sizeOverD = this->perspective ? size/this->d : size*0.1;

	v[X] = (l[X] - m[X])*sizeOverD;
	v[Y] = (l[Y] - m[Y])*sizeOverD;
	v[Z] = (l[Z] - m[Z])*sizeOverD;

	nx[X] = this->up[Y]*nz[Z] - this->up[Z]*nz[Y];
    nx[Y] = this->up[Z]*nz[X] - this->up[X]*nz[Z];
    nx[Z] = this->up[X]*nz[Y] - this->up[Y]*nz[X];

	x2 = this->front[X] + v[X];
	y2 = this->front[Y] + v[Y];
	z2 = this->front[Z] + v[Z];

	x2lessx1 = x2 - this->position[X];
	y2lessy1 = y2 - this->position[Y];
	z2lessz1 = z2 - this->position[Z];

	x1lessx0 = 0.0;
	y1lessy0 = 0.0;
	z1lessz0 = 0.0;

	a = (x2lessx1)*(x2lessx1) + (y2lessy1)*(y2lessy1) + (z2lessz1)*(z2lessz1);
	b = 2.0*(x2lessx1*x1lessx0 + y2lessy1*y1lessy0 + z2lessz1*z1lessz0);
	c = x1lessx0*x1lessx0 + y1lessy0*y1lessy0 + z1lessz0*z1lessz0 - size*size;

	delta = b*b - 4.0*a*c;
	t = (-b + std::sqrt(delta))/(2.0*a);

	if ((t < 0.0) || (t > 1.0))
	{
	   	t = (-b - std::sqrt(delta))/(2.0*a);
	}

	this->front[X] = this->position[X] + (x2lessx1)*t;
	this->front[Y] = this->position[Y] + (y2lessy1)*t;
	this->front[Z] = this->position[Z] + (z2lessz1)*t;

	nz[X] = this->front[X] - this->position[X];
	nz[Y] = this->front[Y] - this->position[Y];
	nz[Z] = this->front[Z] - this->position[Z];

	this->up[X] = nz[Y]*nx[Z] - nz[Z]*nx[Y];
 	this->up[Y] = nz[Z]*nx[X] - nz[X]*nx[Z];
 	this->up[Z] = nz[X]*nx[Y] - nz[Y]*nx[X];

 	size = this->Size(this->up[X], this->up[Y], this->up[Z]);

 	this->up[X] /= size;
 	this->up[Y] /= size;
 	this->up[Z] /= size;

 	this->SetLast(x, y);
}

void CRAbMesh::Camera::Translate(int x, int y)
{
	double m[3], l[3], v[3], size;

	//this->GetMatrices();

	this->Unproject(x, y, m[X], m[Y], m[Z]);
	this->Unproject(this->lastX, this->lastY, l[X], l[Y], l[Z]);

	v[X] = l[X] - m[X];
	v[Y] = l[Y] - m[Y];
	v[Z] = l[Z] - m[Z];

	m[X] = this->front[X] - this->position[X];
	m[Y] = this->front[Y] - this->position[Y];
	m[Z] = this->front[Z] - this->position[Z];

	size = this->perspective ? this->Size(m[X], m[Y], m[Z])/this->d : 1.0;

	v[X] *= size;
	v[Y] *= size;
	v[Z] *= size;

	this->position[X] += v[X];
	this->position[Y] += v[Y];
	this->position[Z] += v[Z];

	this->front[X] += v[X];
	this->front[Y] += v[Y];
	this->front[Z] += v[Z];

	this->SetLast(x, y);
}

void CRAbMesh::Camera::Move(int x, int y)
{
	double m[3];
	static double factor = 40.0; //magic constant

	m[X] = (this->front[X] - this->position[X])/factor;
	m[Y] = (this->front[Y] - this->position[Y])/factor;
	m[Z] = (this->front[Z] - this->position[Z])/factor;

	if (y > this->lastY)
	{
		this->position[X] += m[X];
		this->position[Y] += m[Y];
	 	this->position[Z] += m[Z];

	 	this->front[X] += m[X];
		this->front[Y] += m[Y];
	 	this->front[Z] += m[Z];
	}
	else
	{
	 	this->position[X] -= m[X];
		this->position[Y] -= m[Y];
	 	this->position[Z] -= m[Z];

	 	this->front[X] -= m[X];
		this->front[Y] -= m[Y];
	 	this->front[Z] -= m[Z];
	}

	this->SetLast(x, y);
}

void CRAbMesh::Camera::OpticalZoom(int x, int y)
{
	double m[3], size;
	static double factor = 40.0; //magic constant

	m[X] = (this->front[X] - this->position[X])/factor;
	m[Y] = (this->front[Y] - this->position[Y])/factor;
	m[Z] = (this->front[Z] - this->position[Z])/factor;

	if (y > this->lastY)
	{
		m[X] += this->position[X];
		m[Y] += this->position[Y];
		m[Z] += this->position[Z];

		size = this->Size(this->front[X] - m[X], this->front[Y] - m[Y], this->front[Z] - m[Z]);

		if (size > this->d)
		{
			//Zoom In
		 	this->position[X] = m[X];
			this->position[Y] = m[Y];
		 	this->position[Z] = m[Z];
		}
		else if (this->mergedZoom)
		{
			//Zoom In
			this->DigitalZoom(x, y);
		}
	}
	else
	{
	 	if ((this->mergedZoom) && (this->digitalZoom < 1))
		{
			//Zoom Out
			this->DigitalZoom(x, y);

			if (this->digitalZoom > 1.0)
			{
				this->digitalZoom = 1.0;
			}
		}
		else
		{
			//Zoom Out
			this->position[X] -= m[X];
			this->position[Y] -= m[Y];
			this->position[Z] -= m[Z];
		}
	}

	this->SetLast(x, y);
}

void CRAbMesh::Camera::OpticalZoom(int disp)
{
	int x = this->lastX;
	int y = this->lastY;

	int tx = this->w/2;
	int ty = this->h/2;

	this->Translate(tx, ty);
	this->OpticalZoom(tx, ty + disp);
	this->lastY = ty;
	this->Translate(x, y);
}

void CRAbMesh::Camera::DigitalZoom(int x, int y)
{
	static const double zoomConstant = 1.1;

	int disp = y - this->lastY;

	disp = (disp > 1) ? static_cast<int>(std::ceil(std::log10(static_cast<double>(disp)))) :
		((disp < -1) ? -static_cast<int>(std::ceil(std::log10(static_cast<double>(-disp)))) : disp);

	if (y > this->lastY)
	{
		//Zoom In
		this->digitalZoom /= zoomConstant*disp;
	}
	else if (y < this->lastY)
	{
	 	//Zoom Out
		this->digitalZoom *= zoomConstant*(-disp);
	}

	this->SetLast(x, y);
}

void CRAbMesh::Camera::DigitalZoom(int disp)
{
	int x = this->lastX;
	int y = this->lastY;

	//int tx = this->w/2;
	//int ty = this->h/2;

	//this->Translate(tx, ty);
	//this->DigitalZoom(tx, ty + disp);
	this->DigitalZoom(x, y + disp);
	//this->lastY = ty;
	this->lastY = y;
	//this->Translate(x, y);
}

void CRAbMesh::Camera::MergedZoom(int x, int y)
{
	this->mergedZoom = true;

	this->OpticalZoom(x, y);

	this->mergedZoom = false;
}

void CRAbMesh::Camera::MergedZoom(int disp)
{
	this->mergedZoom = true;

	this->OpticalZoom(disp);

	this->mergedZoom = false;
}

void CRAbMesh::Camera::Base(double &ix, double &iy, double &iz,
	double &jx, double &jy, double &jz, double &kx, double &ky, double &kz)
{
	double size = 1.0;

	jx = this->GetUp(0);
	jy = this->GetUp(1);
	jz = this->GetUp(2);

	//size = 1.0/this->Size(jx. jy, jz);

	//jx *= size;
	//jy *= size;
	//jz *= size;

	kx = this->GetFront(0) - this->GetPosition(0);
	ky = this->GetFront(1) - this->GetPosition(1);
	kz = this->GetFront(2) - this->GetPosition(2);

	size = 1.0/this->Size(kx, ky, kz);

	kx *= size;
	ky *= size;
	kz *= size;

	ix = - jy*kz + jz*ky;
	iy = - jz*kx + jx*kz;
	iz = - jx*ky + jy*kx;
}

/*void CRAbMesh::Camera::Apply(double x, double y, double z, double w)
{
	double v[4], up[4], conj[4], aux[4];

	v[X] = this->front[X] - this->position[X];
	v[Y] = this->front[Y] - this->position[Y];
	v[Z] = this->front[Z] - this->position[Z];
	v[W] = 0.0;

	up[X] = this->up[X];
	up[Y] = this->up[Y];
	up[Z] = this->up[Z];
	up[W] = 0.0;

	conj[X] = -x;
	conj[Y] = -y;
	conj[Z] = -z;
	conj[W] = w;

	aux[X] = w*v[X] + x*v[W] + y*v[Z] - z*v[Y];
	aux[Y] = w*v[Y] - x*v[Z] + y*v[W] + z*v[X];
    aux[Z] = w*v[Z] + x*v[Y] - y*v[X] + z*v[W];
    aux[W] = w*v[W] - x*v[X] - y*v[Y] - z*v[Z];

	v[X] = aux[W]*conj[X] + aux[X]*conj[W] + aux[Y]*conj[Z] - aux[Z]*conj[Y];
	v[Y] = aux[W]*conj[Y] - aux[X]*conj[Z] + aux[Y]*conj[W] + aux[Z]*conj[X];
    v[Z] = aux[W]*conj[Z] + aux[X]*conj[Y] - aux[Y]*conj[X] + aux[Z]*conj[W];
    v[W] = aux[W]*conj[W] - aux[X]*conj[X] - aux[Y]*conj[Y] - aux[Z]*conj[Z];

    aux[X] = w*up[X] + x*up[W] + y*up[Z] - z*up[Y];
	aux[Y] = w*up[Y] - x*up[Z] + y*up[W] + z*up[X];
    aux[Z] = w*up[Z] + x*up[Y] - y*up[X] + z*up[W];
    aux[W] = w*up[W] - x*up[X] - y*up[Y] - z*up[Z];

	up[X] = aux[W]*conj[X] + aux[X]*conj[W] + aux[Y]*conj[Z] - aux[Z]*conj[Y];
	up[Y] = aux[W]*conj[Y] - aux[X]*conj[Z] + aux[Y]*conj[W] + aux[Z]*conj[X];
    up[Z] = aux[W]*conj[Z] + aux[X]*conj[Y] - aux[Y]*conj[X] + aux[Z]*conj[W];
    up[W] = aux[W]*conj[W] - aux[X]*conj[X] - aux[Y]*conj[Y] - aux[Z]*conj[Z];

    this->SetPosition(v[X] + this->front[X], v[Y] + this->front[Y], v[Z] + this->front[Z]);
    this->SetUp(up[X], v[Y] + up[Y], up[Z]);
}*/

void CRAbMesh::Camera::Reset()
{
    this->SetLast(0, 0);
    this->digitalZoom = 1.0;

    this->SetUp(this->defaultUp[X], this->defaultUp[Y], this->defaultUp[Z]);
    this->SetFront(this->defaultFront[X], this->defaultFront[Y], this->defaultFront[Z]);
    this->SetPosition(this->defaultPosition[X], this->defaultPosition[Y], this->defaultPosition[Z]);
    this->SetCanvasSize(this->defaultW, this->defaultH);
}

double CRAbMesh::Camera::Distance() const
{
    return this->Size(this->position[X] - this->front[X],
					  this->position[Y] - this->front[Y],
					  this->position[Z] - this->front[Z]);
}

void CRAbMesh::Camera::Unproject(double x, double y, double &mx, double &my, double &mz)
{
	this->Unproject(x, y, this->near, mx, my, mz);
}

void CRAbMesh::Camera::Unproject(double x, double y, double z, double &mx, double &my, double &mz)
{
	this->GetMatrices();

	/*if (std::fabs(z) > 0.0)
	{
		gluProject()
	}*/

	gluUnProject(x, this->h - y, z, this->modelview, this->projection, this->viewport, &mx, &my, &mz);
}

void CRAbMesh::Camera::Draw()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	this->Projection();

    this->DrawModelview();
}

void CRAbMesh::Camera::DrawModelview()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(this->position[X], this->position[Y], this->position[Z],
 			  this->front[X], this->front[Y], this->front[Z],
			  this->up[X], this->up[Y], this->up[Z]);
}

double CRAbMesh::Camera::Sign(double x)
{
	return (x < 0.0) ? -1.0 : 1.0;
}

double CRAbMesh::Camera::Size(double x, double y, double z)
{
	return std::sqrt(x*x + y*y + z*z);
}

void CRAbMesh::Camera::GetMatrices()
{
	glGetIntegerv(GL_VIEWPORT, this->viewport);
	glGetDoublev(GL_PROJECTION_MATRIX, this->projection);
	glGetDoublev(GL_MODELVIEW_MATRIX, this->modelview);
}

void CRAbMesh::Camera::Projection()
{
	//glFrustum(-(float)w/(float)h, (float)w/(float)h, -1.0, 1.0, 1.0, 1000.0);
    //glFrustum(-((float)w/(float)h)/10.0, ((float)w/(float)h)/10.0, -0.1, 0.1, 0.1, 1000.0);
    //glFrustum(-((float)this->w/(float)this->h)*this->d, ((float)this->w/(float)this->h)*this->d, -this->d, this->d, this->d, 1000.0);

    if (this->w >= this->h)
	{
		this->left = -(static_cast<float>(this->w)/static_cast<float>(this->h));
		this->right = (static_cast<float>(this->w)/static_cast<float>(this->h));

		this->bottom = -1.0;
		this->top = 1.0;

		this->near = this->d;
		this->far = 100000.0;
		//this->far = 10000.0*this->d;
		//this->far = 2.0*std::sqrt(3.0)*this->Size(this->GetPosition(0) - this->GetFront(0), this->GetPosition(1) - this->GetFront(1), this->GetPosition(2) - this->GetFront(2));
	}
	else
	{
		this->left = -1.0;
		this->right = 1.0;

		this->bottom = -(static_cast<float>(this->h)/static_cast<float>(this->w));
		this->top = (static_cast<float>(this->h)/static_cast<float>(this->w));

		this->near = this->d;
		this->far = 100000.0;
		//this->far = 10000.0*this->d;
		//this->far = 2.0*std::sqrt(3.0)*this->Size(this->GetPosition(0) - this->GetFront(0), this->GetPosition(1) - this->GetFront(1), this->GetPosition(2) - this->GetFront(2));
	}

	this->left *= this->digitalZoom;
	this->right *= this->digitalZoom;
	this->bottom *= this->digitalZoom;
	this->top *= this->digitalZoom;

	if (this->stereoPosition == NEUTRAL)
	{
		this->shift = 0.0;
	}
	else
	{
		this->shift = this->distNeutral*this->near/this->focus;

		if (this->stereoPosition == RIGHT)
		{
			this->shift *= -1.0;

			glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		}
		else if (this->stereoPosition == LEFT)
		{
			glStencilFunc(GL_EQUAL, 0x1, 0x1);
		}
		else
		{
			this->shift = 0.0;
		}
	}

    if (perspective)
    {
		glFrustum((this->left + this->shift)*this->d, (this->right + this->shift)*this->d,
				  this->bottom*this->d, this->top*this->d,
				  this->near, this->far);
    }
	else
	{
		double distance = this->Size(this->GetPosition(0) - this->GetFront(0),
			this->GetPosition(1) - this->GetFront(1), this->GetPosition(2) - this->GetFront(2));

		this->near = 0.0;

		glOrtho((this->left + this->shift)*distance, (this->right + this->shift)*distance,
				this->bottom*distance, this->top*distance,
				this->near, this->far);
	}

	if (this->stereoPosition != NEUTRAL)
	{
		if (this->stereoPosition == RIGHT)
		{
			glTranslated(-this->distNeutral, 0.0, 0.0);
		}
		else if (this->stereoPosition == LEFT)
		{
			glTranslated( this->distNeutral, 0.0, 0.0);
		}
	}
}

std::istream &CRAbMesh::operator>>(std::istream &stream, Camera &c)
{
	stream >> c.position[0];
	stream >> c.position[1];
	stream >> c.position[2];
	stream >> c.front[0];
	stream >> c.front[1];
	stream >> c.front[2];
	stream >> c.up[0];
	stream >> c.up[1];
	stream >> c.up[2];

//	stream >> c.defaultPosition[0];
//	stream >> c.defaultPosition[1];
//	stream >> c.defaultPosition[2];
//	stream >> c.defaultFront[0];
//	stream >> c.defaultFront[1];
//	stream >> c.defaultFront[2];
//	stream >> c.defaultUp[0];
//	stream >> c.defaultUp[1];
//	stream >> c.defaultUp[2];

//	stream >> c.left;
//	stream >> c.right;
//	stream >> c.bottom;
//	stream >> c.top;
//	stream >> c.near;
//	stream >> c.far;

	stream >> c.digitalZoom;

	stream >> c.distNeutral;
	stream >> c.focus;

	return stream;
}

std::ostream &CRAbMesh::operator<<(std::ostream &stream, const Camera &c)
{
	stream << c.position[0] << " " << c.position[1] << " " << c.position[2] << std::endl;
	stream << c.front[0] << " " << c.front[1] << " " << c.front[2] << std::endl;
	stream << c.up[0] << " " << c.up[1] << " " << c.up[2] << std::endl;

//	stream << c.defaultPosition[0] << " " << c.defaultPosition[1] << " " << c.defaultPosition[2] << std::endl;
//	stream << c.defaultFront[0] << " " << c.defaultFront[1] << " " << c.defaultFront[2] << std::endl;
//	stream << c.defaultUp[0] << " " << c.defaultUp[1] << " " << c.defaultUp[2] << std::endl;

//	stream << c.left << " " << c.right << " " << c.bottom << " " << c.top << " " << c.near << " " << c.far << std::endl;

	stream << c.digitalZoom << std::endl;

	stream << c.distNeutral << " " << c.focus << std::endl;

	return stream;
}

#endif //#if USE_GUI
