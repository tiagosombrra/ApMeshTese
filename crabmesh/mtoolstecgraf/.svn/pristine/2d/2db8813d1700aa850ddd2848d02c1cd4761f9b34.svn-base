#ifndef _PERFORMER_BOUNDARY_CREATOR_H_
#define _PERFORMER_BOUNDARY_CREATOR_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class BoundaryCreator : public Performer::Builder
	{
	public:

		BoundaryCreator(Data::Mesh *mesh = NULL);
		virtual ~BoundaryCreator();

		virtual void setIdManager(IdManager *idManager);
		virtual IdManager *getIdManager() const;

		virtual void setMesh(Data::Mesh *mesh);

		virtual void setBoundary(Data::Boundary *boundary);
		virtual Data::Boundary *getBoundary() const;

		virtual void setOnlyVertices(bool onlyVertices);

		virtual Data::Boundary *makeBoundary() const = 0;

		virtual void execute();

	public:

#if USE_C__11
		typedef std::unordered_map<Data::Vertex *, std::unordered_set<Data::Vertex *> > DoneList2D;

		struct hash
		{
			std::size_t operator()(const Data::VertexVector &v) const
			{
				return std::hash<void *>()(v.front());
			}
		};

		typedef std::unordered_set<Data::VertexVector, hash> DoneList3D;
#else
		typedef std::map<Data::Vertex *, std::set<Data::Vertex *> > DoneList2D;
		std::set<Data::VertexVector> DoneList3D;
#endif //#if USE_C__11

	protected:

		virtual Data::FrontElement *makeOrFindFrontElement(const Data::VertexVector &vertices, Data::Element *e) const;
		virtual Data::FrontElement *makeFrontElement(const Data::VertexVector &vertices, Data::Element *e) const = 0;

		virtual UInt numFaces(UInt numVertices) const;
		virtual void face(UInt i, UInt numVertices, Data::UIntVector &vertices) const;

		virtual void execute2D(Data::Element *e, DoneList2D &done);
		virtual void execute2D();

		virtual void execute3D(Data::Element *e, DoneList3D &done);
		virtual void execute3D();

	protected:

		Data::Mesh *mesh;

		Data::Boundary *boundary;

		IdManager *idManager;

		bool onlyFind;
		bool onlyVertices;

	};
}

#endif //#ifndef _PERFORMER_BOUNDARY_CREATOR_H_
