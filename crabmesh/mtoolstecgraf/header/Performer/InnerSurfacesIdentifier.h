#ifndef _PERFORMER_INNER_SURFACES_IDENTIFIER_H_
#define _PERFORMER_INNER_SURFACES_IDENTIFIER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class InnerSurfacesIdentifier : public Performer::Builder
	{
    public:

		struct Surface
		{
			~Surface();

			Data::Boundary *boundary;
			Data::Position position;
			bool manifold;
		};

		typedef std::list<Surface *> SurfaceList;

		struct FESurf
		{
			Data::FrontElement *fel;
			Data::Vertex *vertex;
			Surface *surface;
		};

		struct SurfaceTree;

		typedef std::list<SurfaceTree *> SurfaceTreeList;

		struct SurfaceTree
		{
			Surface *surface;

			SurfaceTreeList children;
		};

	public:

		InnerSurfacesIdentifier();
		virtual ~InnerSurfacesIdentifier();

		virtual void setBoundary(Data::Boundary *boundary);
		virtual void setMesh(Data::Mesh *mesh);
		virtual void setSurfaceDirection(UInt surfaceDirection);

		virtual void setNeedTree(bool needTree);
		virtual const SurfaceTree *getTree() const;
		virtual void unsetTree();

		virtual bool isManifold() const;

		virtual Data::FrontElementList getInnerSurfaces() const;

		virtual void execute();

	protected:

		struct Opening
		{
			Data::Vertex *v1, *v2;
		};

#if USE_C__11
		struct equal
		{
			bool operator()(const Opening &op1, const Opening &op2) const
			{
				return ((op1.v1 == op2.v1) && (op1.v2 == op2.v2));
			};
		};

		struct hash
		{
			size_t operator()(const Opening &op) const
			{
				return std::hash<void *>()(op.v1);
			};
		};
#else
		struct less
		{
			bool operator()(const Opening &op1, const Opening &op2) const
			{
				return ((op1.v1 < op2.v1) ||
						((op1.v1 == op2.v1) && (op1.v2 < op2.v2)));
			};
		};
#endif //#if USE_C__11

#if USE_C__11
		typedef std::unordered_set<Opening, hash, equal> OpeningSet;

		typedef std::unordered_map<Opening, Data::FrontElementList, hash, equal> OpeningMap;
		typedef std::unordered_map<Data::Boundary *, OpeningMap> BoundaryOpeningMap;

		typedef std::unordered_map<Surface *, FESurf> FESurfMap;
#else
		typedef std::set<Opening, less> OpeningSet;

		typedef std::map<Opening, Data::FrontElementList, less> OpeningMap;
		typedef std::map<Data::Boundary *, OpeningMap> BoundaryOpeningMap;

		typedef std::map<Surface *, FESurf> FESurfMap;
#endif //#if USE_C__11

	protected:

		virtual Data::Boundary *makeBoundary() const = 0;

		virtual Surface *surface(Data::FrontElement *fe) const;

		virtual void propagate();
		virtual void propagate(Data::FrontElement *fe);
		virtual void propagate(Data::FrontElement *fe, Surface *surface);

		static bool smallerCoords(const Data::Vertex *v1, const Data::Vertex *v2, UInt surfaceDirection);
		static bool smallerCoords0(const Data::Vertex *v1, const Data::Vertex *v2);
		static bool smallerCoords1(const Data::Vertex *v1, const Data::Vertex *v2);
		static bool smallerCoords2(const Data::Vertex *v1, const Data::Vertex *v2);
		static bool smallerCoordsAll(const Data::Vertex *v1, const Data::Vertex *v2);
		static void sort(Data::VertexVector &verts, UInt surfaceDirection);

		virtual Data::FrontElement *leftmost(const Data::Boundary *boundary, Data::Vertex *&leftmost) const;
		virtual FESurf leftmost() const;

		virtual Data::Position position(const Data::Point *point, const Data::Boundary *boundary) const = 0;
		virtual bool isInner(const SurfaceTree *node, const Data::Point *point, SurfaceTree *&innermost) const;

		virtual bool shouldInvert2D(const Data::FrontElement *adj,
			const Data::FrontElement *fel, const Data::Vertex *v) const;
		virtual bool shouldInvert3D(const Data::FrontElement *adj,
			const Data::FrontElement *fel,
			const Data::Vertex *v1, const Data::Vertex *v2) const;

		virtual void invert(Data::FrontElement *fel) const;
		virtual void invertIfNecessary(Data::FrontElement *fel, bool inner) const;

		virtual bool findCompleteBoundary2D(Data::FrontElement *fel, Surface *surface, Data::FrontElementList &toBeTested);
		virtual void findCompleteBoundary2D(Data::FrontElement *fel, Surface *surface);

		virtual bool findCompleteBoundary3D(Data::FrontElement *fel, Surface *surface, Data::FrontElementList &toBeTested);
		virtual void findCompleteBoundary3D(Data::FrontElement *fel, Surface *surface);

		virtual void findCompleteBoundary(Data::FrontElement *fel, Surface *surface);
		virtual void findCompleteBoundary(FESurf leftmost);

		virtual void findCompleteBoundaries();

		virtual void setInSurface(Opening op);

		virtual void clear(SurfaceTree *node);
		virtual void clearTree();

	protected:

		UInt dimension;

		Data::Boundary *boundary;
		Data::Mesh *mesh;
		UInt surfaceDirection;

		Data::FrontElementList innerSurfaces;

		SurfaceList surfaces;

		BoundaryOpeningMap openings;

		bool manifold;

		bool needTree;
		SurfaceTree *root;

		mutable FESurfMap leftmosts;

#if USE_C__11
		Data::FrontElementHash tested;
#else
		Data::FrontElementSet tested;
#endif //#if USE_C__11
	};
}

#endif //#ifndef _PERFORMER_INNER_SURFACES_IDENTIFIER_H_
