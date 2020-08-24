#include "TecGraf/Region.h"

#if USE_TECGRAF

#include "TecGraf/OrientedSurface.h"
#include "TecGraf/Surface.h"
#include "Data/Point3D.h"

using namespace TecGraf;

TecGraf::Region::Region(ULInt id) :
	Data::AbstractMesh::AbstractMesh(id)
{
	this->duration = 0.0;
}

TecGraf::Region::~Region()
{
	this->clear();
}

void TecGraf::Region::setDuration(Real duration)
{
	this->duration = duration;
}

Real TecGraf::Region::getDuration() const
{
	return this->duration;
}

void TecGraf::Region::add(Surface *surface, Int orientation)
{
	this->surfaces.push_back(this->makeOrientedSurface(surface, orientation));
}

void TecGraf::Region::remove(Surface *surface)
{
	for (OrientedSurfaceList::iterator iter = this->surfaces.begin();
		 iter != this->surfaces.end(); iter++)
	{
		if ((*iter)->getSurface() == surface)
		{
			this->surfaces.erase(iter);

			break;
		}
	}
}

bool TecGraf::Region::have(Surface *surface) const
{
	for (OrientedSurfaceList::const_iterator iter = this->surfaces.begin();
		 iter != this->surfaces.end(); iter++)
	{
		if ((*iter)->getSurface() == surface)
		{
			return true;
		}
	}

	return false;
}

const OrientedSurfaceList &TecGraf::Region::getSurfaces() const
{
	return this->surfaces;
}

OrientedSurfaceList::iterator TecGraf::Region::begin()
{
	return this->surfaces.begin();
}

OrientedSurfaceList::const_iterator TecGraf::Region::begin() const
{
	return this->surfaces.begin();
}

OrientedSurfaceList::iterator TecGraf::Region::end()
{
	return this->surfaces.end();
}

OrientedSurfaceList::const_iterator TecGraf::Region::end() const
{
	return this->surfaces.end();
}

void TecGraf::Region::clear()
{
	while (!this->surfaces.empty())
	{
		delete this->surfaces.front();

		this->surfaces.pop_front();
	}
}

Data::Box TecGraf::Region::box() const
{
	Data::Point3D min, max;

	for (UInt i = 0; i < 3; i++)
	{
		min.setCoord(i, REAL_MAX);
		max.setCoord(i, -REAL_MAX);
	}

	for (OrientedSurfaceList::const_iterator iter = this->surfaces.begin();
		 iter != this->surfaces.end(); iter++)
	{
		Data::Box *b = (*iter)->getSurface()->box();

		for (UInt i = 0; i < 3; i++)
		{
			if (b->getMin()->getCoord(i) < min.getCoord(i))
			{
				min.setCoord(i, b->getMin()->getCoord(i));
			}

			if (b->getMax()->getCoord(i) > max.getCoord(i))
			{
				max.setCoord(i, b->getMax()->getCoord(i));
			}
		}

		delete b;
	}

	return Data::Box(min, max);
}

OrientedSurface *TecGraf::Region::makeOrientedSurface(Surface *surface, Int orientation) const
{
	OrientedSurface *s = new OrientedSurface();

	s->setSurface(surface);
	s->setOrientation(orientation);

	return s;
}

#if USE_GUI
void TecGraf::Region::clearLists() const
{
	Data::AbstractMesh::clearLists();

	for (OrientedSurfaceList::const_iterator iter = this->surfaces.begin();
		 iter != this->surfaces.end(); iter++)
	{
		(*iter)->clearLists();
	}
}

void TecGraf::Region::draw(bool lighting) const
{
	/*if (!this->hasLists())
	{
		this->createList(1);
	}

	if (this->hasList())
	{
		this->drawList();

		return;
	}

	this->beginList();*/

	for (OrientedSurfaceList::const_iterator iter = this->surfaces.begin();
		 iter != this->surfaces.end(); iter++)
	{
		(*iter)->draw(lighting);
	}

	//this->endList();
}

void TecGraf::Region::draw() const
{
	GLboolean lighting = glIsEnabled(GL_LIGHTING);

	this->draw(lighting);
}

std::string TecGraf::Region::text() const
{
	std::stringstream str;

	str << this->getId();

	return str.str();
}
#endif //#if USE_GUI

#endif //#if USE_TECGRAF
