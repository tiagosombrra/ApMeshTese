#ifndef _TECGRAF_MODEL_H_
#define _TECGRAF_MODEL_H_

#include "TecGraf/Definitions.h"

#if USE_TECGRAF

#include "Data/Box.h"
#include "Data/AbstractMesh.h"

namespace TecGraf
{
	class Model : public Data::AbstractMesh
	{
	public:

		Model(ULInt id = 0);
		virtual ~Model();

		virtual void add(Region *region);
		virtual void remove(Region *region);
		virtual bool have(Region *region) const;

		virtual UInt numRegions() const;

		virtual RegionList::iterator rBegin();
		virtual RegionList::const_iterator rBegin() const;
		virtual RegionList::iterator rEnd();
		virtual RegionList::const_iterator rEnd() const;

		virtual const RegionList &getRegions() const;

		using Data::AbstractMesh::have;
		virtual bool have(Surface *surface) const;

		virtual UInt numSurfaces() const;

#if USE_C__11
		virtual const SurfaceHash &getSurfaces() const;

		virtual SurfaceHash::iterator sBegin();
		virtual SurfaceHash::const_iterator sBegin() const;
		virtual SurfaceHash::iterator sEnd();
		virtual SurfaceHash::const_iterator sEnd() const;
#else
		virtual const SurfaceSet &getSurfaces() const;

		virtual SurfaceSet::iterator sBegin();
		virtual SurfaceSet::const_iterator sBegin() const;
		virtual SurfaceSet::iterator sEnd();
		virtual SurfaceSet::const_iterator sEnd() const;
#endif //#if USE_C__11

		virtual UInt dimension() const;

		virtual Data::Box box() const;

		virtual void to(Data::Boundary *boundary, Data::Mesh *mesh = NULL) const;
		virtual void to(Data::Mesh *mesh, Data::Boundary *boundary = NULL) const;

		virtual void clear();

#if USE_GUI
		virtual void setDrawingRegions(bool drawingRegions) const;
		virtual bool isDrawingRegions() const;

		virtual void clearLists() const;

		using Data::AbstractMesh::draw;
		virtual void draw() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		RegionList regions;

#if USE_C__11
		SurfaceHash surfaces;
#else
		SurfaceSet surfaces;
#endif //#if USE_C__11

#if USE_GUI
		mutable bool drawingRegions;
#endif //#if USE_GUI

	};
}

#endif //#if USE_TECGRAF

#endif //#ifndef _TECGRAF_MODEL_H_
