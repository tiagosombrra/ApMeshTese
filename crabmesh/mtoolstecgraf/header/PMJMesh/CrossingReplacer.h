#ifndef _PMJMESH_CROSSING_REPLACER_H_
#define _PMJMESH_CROSSING_REPLACER_H_

#include "PMJMesh/Definitions.h"

#include "Performer/Builder.h"

namespace PMJMesh
{
	class CrossingReplacer : public Performer::Builder
	{
	public:

		CrossingReplacer(UInt dimension, const Parallel::TaskVector *tasks = NULL);
		virtual ~CrossingReplacer();

		virtual void setDimension(UInt dimension);
		virtual void setTasks(const Parallel::TaskVector *tasks);

		virtual void execute();

		virtual void backReplace(MJMesh::FrontElement *fel, MJMesh::FrontElement *tmp, bool fronts, bool meshes, bool boundaries, bool check, const Data::Boundary *except = NULL) const;
		virtual void backReplace(MJMesh::Vertex *v, MJMesh::Vertex *tmp, bool fronts, bool meshes, bool boundaries, bool check, const Data::Boundary *except = NULL) const;

	protected:

		virtual void replace();
		virtual void backReplace();

		virtual void backReplace(MJMesh::FrontElement *fel, Data::FrontElementList *temporaries, const Data::Boundary *delExcept = NULL);
		virtual void backReplace(MJMesh::Vertex *v, Data::VertexList *temporaries, const Data::Boundary *delExcept = NULL);

	public:

		template <typename T>
		struct Replacement
		{
			T original;
			std::list<T> *temporaries;
		};

		typedef Replacement<Data::Vertex *> VertexReplacement;
		typedef Replacement<Data::FrontElement *> FrontElementReplacement;

#if USE_C__11
		typedef std::unordered_map<ULInt, VertexReplacement> VertexReplacementHashMap;
		typedef std::unordered_map<ULInt, FrontElementReplacement> FrontElementReplacementHashMap;
#endif //#if USE_C__11

		typedef std::map<ULInt, VertexReplacement> VertexReplacementMap;
		typedef std::map<ULInt, FrontElementReplacement> FrontElementReplacementMap;

	protected:

		UInt dimension;
		const Parallel::TaskVector *tasks;

		bool back;

#if USE_C__11
		VertexReplacementHashMap vertices;
		FrontElementReplacementHashMap frontElements;
#else
		VertexReplacementMap vertices;
		FrontElementReplacementMap frontElements;
#endif //#if USE_C__11
	};
}

#endif //#ifndef _PMJMESH_CROSSING_REPLACER_H_
