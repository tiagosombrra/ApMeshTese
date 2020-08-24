#include "TecGraf/Model.h"

#if USE_TECGRAF

#include "TecGraf/Region.h"
#include "TecGraf/OrientedSurface.h"
#include "TecGraf/Surface.h"
#include "Data/Point3D.h"
#include "Data/Boundary.h"
#include "Data/Mesh.h"

using namespace TecGraf;

TecGraf::Model::Model(ULInt id) :
	Data::AbstractMesh::AbstractMesh(id)
{
#if USE_GUI
	this->drawingRegions = false;
#endif //#if USE_GUI
}

TecGraf::Model::~Model()
{
	this->clear();
}

void TecGraf::Model::add(Region *region)
{
	this->regions.push_back(region);

	for (OrientedSurfaceList::iterator iter = region->begin();
		 iter != region->end(); iter++)
	{
		this->surfaces.insert((*iter)->getSurface());
	}
}

void TecGraf::Model::remove(Region *region)
{
	RegionList::iterator it = std::find(this->regions.begin(), this->regions.end(), region);

	if (it == this->regions.end())
	{
		return;
	}

	this->regions.erase(it);

	for (OrientedSurfaceList::iterator iter = region->begin();
		 iter != region->end(); iter++)
	{
		Surface *s = (*iter)->getSurface();

		bool found = false;

		for (RegionList::iterator iter2 = this->regions.begin();
			 iter2 != this->regions.end(); iter2++)
		{
			if ((*iter2)->have(s))
			{
				found = true;

				break;
			}
		}

		if (!found)
		{
			this->surfaces.erase(s);
		}
	}
}

bool TecGraf::Model::have(Region *region) const
{
	return (std::find(this->regions.begin(), this->regions.end(), region) != this->regions.end());
}

UInt TecGraf::Model::numRegions() const
{
	return static_cast<UInt>(this->regions.size());
}

RegionList::iterator TecGraf::Model::rBegin()
{
	return this->regions.begin();
}

RegionList::const_iterator TecGraf::Model::rBegin() const
{
	return this->regions.begin();
}

RegionList::iterator TecGraf::Model::rEnd()
{
	return this->regions.end();
}

RegionList::const_iterator TecGraf::Model::rEnd() const
{
	return this->regions.end();
}

const RegionList &TecGraf::Model::getRegions() const
{
	return this->regions;
}

bool TecGraf::Model::have(Surface *surface) const
{
	return (this->surfaces.find(surface) != this->surfaces.end());
}

UInt TecGraf::Model::numSurfaces() const
{
	return static_cast<UInt>(this->surfaces.size());
}

#if USE_C__11
const SurfaceHash &TecGraf::Model::getSurfaces() const
#else
const SurfaceSet &TecGraf::Model::getSurfaces() const
#endif //#if USE_C__11
{
	return this->surfaces;
}

#if USE_C__11
SurfaceHash::iterator TecGraf::Model::sBegin()
#else
SurfaceSet::iterator TecGraf::Model::sBegin()
#endif //#if USE_C__11
{
	return this->surfaces.begin();
}

#if USE_C__11
SurfaceHash::const_iterator TecGraf::Model::sBegin() const
#else
SurfaceSet::const_iterator TecGraf::Model::sBegin() const
#endif //#if USE_C__11
{
	return this->surfaces.begin();
}

#if USE_C__11
SurfaceHash::iterator TecGraf::Model::sEnd()
#else
SurfaceSet::iterator TecGraf::Model::sEnd()
#endif //#if USE_C__11
{
	return this->surfaces.end();
}

#if USE_C__11
SurfaceHash::const_iterator TecGraf::Model::sEnd() const
#else
SurfaceSet::const_iterator TecGraf::Model::sEnd() const
#endif //#if USE_C__11
{
	return this->surfaces.end();
}

UInt TecGraf::Model::dimension() const
{
	return this->surfaces.empty() ? 0 : (*this->surfaces.begin())->dimension();
}

Data::Box TecGraf::Model::box() const
{
	Data::Point3D min, max;

	for (UInt i = 0; i < 3; i++)
	{
		min.setCoord(i, REAL_MAX);
		max.setCoord(i, -REAL_MAX);
	}

#if USE_C__11
	for (SurfaceHash::const_iterator iter = this->surfaces.begin();
#else
	for (SurfaceSet::const_iterator iter = this->surfaces.begin();
#endif //#if USE_C__11
		 iter != this->surfaces.end(); iter++)
	{
		Data::Box *b = (*iter)->box();

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

void TecGraf::Model::to(Data::Boundary *boundary, Data::Mesh *mesh) const
{
#if USE_C__11
	for (SurfaceHash::const_iterator iter = this->surfaces.begin();
#else
	for (SurfaceSet::const_iterator iter = this->surfaces.begin();
#endif //#if USE_C__11
		 iter != this->surfaces.end(); iter++)
	{
		Surface *s = (*iter);

		for (Data::Boundary::VertexIterator iter2 = s->vBegin();
			 iter2 != s->vEnd(); iter2++)
		{
			Data::Vertex *v = (*iter2);

			if ((mesh) && (!mesh->have(v)))
			{
				mesh->add(v, (boundary == NULL));
			}

			if ((boundary) && (!boundary->have(v)))
			{
				boundary->add(v);
			}
		}

		for (Data::Boundary::FrontElementIterator iter2 = s->eBegin();
			 iter2 != s->eEnd(); iter2++)
		{
			Data::FrontElement *fel = (*iter2);

			if (!mesh->have(fel))
			{
				mesh->add(fel);
			}

			if (!boundary->have(fel))
			{
				boundary->add(fel);
			}
		}
	}
}

void TecGraf::Model::to(Data::Mesh *mesh, Data::Boundary *boundary) const
{
	this->to(boundary, mesh);
}

void TecGraf::Model::clear()
{
	while (!this->regions.empty())
	{
		delete this->regions.front();

		this->regions.pop_front();
	}

#if USE_C__11
	for (SurfaceHash::iterator iter = this->surfaces.begin();
#else
	for (SurfaceSet::iterator iter = this->surfaces.begin();
#endif //#if USE_C__11
		 iter != this->surfaces.end(); iter++)
	{
		delete (*iter);
	}

	this->surfaces.clear();
}

#if USE_GUI
void TecGraf::Model::setDrawingRegions(bool drawingRegions) const
{
	this->drawingRegions = drawingRegions;
}

bool TecGraf::Model::isDrawingRegions() const
{
	return this->drawingRegions;
}

void TecGraf::Model::clearLists() const
{
	Data::AbstractMesh::clearLists();

	for (RegionList::const_iterator iter = this->regions.begin();
		 iter != this->regions.end(); iter++)
	{
		(*iter)->clearLists();
	}
}

void TecGraf::Model::draw() const
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

	GLboolean lighting = glIsEnabled(GL_LIGHTING);

	if (this->drawingRegions)
	{
		for (RegionList::const_iterator iter = this->regions.begin();
			 iter != this->regions.end(); iter++)
		{
			(*iter)->draw(lighting);
		}
	}
	else
	{
#if USE_C__11
		for (SurfaceHash::const_iterator iter = this->surfaces.begin();
#else
		for (SurfaceSet::const_iterator iter = this->surfaces.begin();
#endif //#if USE_C__11
			 iter != this->surfaces.end(); iter++)
		{
			(*iter)->draw(lighting);
		}
	}

	//this->endList();
}

std::string TecGraf::Model::text() const
{
	std::stringstream str;

	str << this->getId();

	return str.str();
}
#endif //#if USE_GUI

#endif //#if USE_TECGRAF
