#ifndef _TECGRAF_REGION_H_
#define _TECGRAF_REGION_H_

#include "TecGraf/Definitions.h"

#if USE_TECGRAF

#include "Data/AbstractMesh.h"
#include "Data/Box.h"

namespace TecGraf
{
	class Region : public Data::AbstractMesh
	{
	public:

		Region(ULInt id = 0);
		virtual ~Region();

		virtual void setDuration(Real duration);
		virtual Real getDuration() const;

		using Data::AbstractMesh::have;

		virtual void add(Surface *surface, Int orientation);
		virtual void remove(Surface *surface);
		virtual bool have(Surface *surface) const;

		virtual const OrientedSurfaceList &getSurfaces() const;

		virtual OrientedSurfaceList::iterator begin();
		virtual OrientedSurfaceList::const_iterator begin() const;
		virtual OrientedSurfaceList::iterator end();
		virtual OrientedSurfaceList::const_iterator end() const;

		virtual void clear();

		virtual Data::Box box() const;

#if USE_GUI
		virtual void clearLists() const;

		using Data::AbstractMesh::draw;
		virtual void draw(bool lighting) const;
		virtual void draw() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual OrientedSurface *makeOrientedSurface(Surface *surface, Int orientation) const;

	protected:

		OrientedSurfaceList surfaces;

		Real duration;

	};
}

#endif //#ifndef _TECGRAF_REGION_H_

#endif //#if USE_TECGRAF
