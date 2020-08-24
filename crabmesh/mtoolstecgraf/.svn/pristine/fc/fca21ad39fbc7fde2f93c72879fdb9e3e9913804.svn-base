#ifndef _PMJMESH2_CROSSING_REPLACER_H_
#define _PMJMESH2_CROSSING_REPLACER_H_

#include "PMJMesh2/Definitions.h"
#include "PMJMesh/CrossingReplacer.h"
#include "MJMesh/Boundary.h"

namespace PMJMesh2
{
    class CrossingReplacer : public PMJMesh::CrossingReplacer
    {
    public:

        CrossingReplacer(UInt dimension, bool byTask,
            const Data::BSPTree *bsp = NULL, const Data::BSPTreeCell *parent = NULL,
            MJMesh::Boundary *boundary = NULL,
            const Parallel::TaskVector *tasks = NULL);
		virtual ~CrossingReplacer();

		virtual void setByTask(bool byTask);
		virtual bool isByTask() const;

		virtual void setBSP(const Data::BSPTree *bsp);
		virtual void setParent(const Data::BSPTreeCell *parent);
		virtual void setBoundary(MJMesh::Boundary *boundary);

		virtual Data::Vertex *getTemporaryVertex(ULInt idTask, ULInt idVertex) const;
		virtual Data::FrontElement *getTemporaryFrontElement(ULInt idTask, ULInt idFrontElement) const;

        //virtual void execute();
		using PMJMesh::CrossingReplacer::backReplace;
        virtual void backReplace(ULInt id1, ULInt id2, const Data::Boundary *boundary);

        virtual void backReplaceToTemporary(ULInt id1, ULInt id2, ULInt idparent);
        virtual void backReplaceToOriginal(ULInt id, const Data::Boundary *boundary);
        virtual MJMesh::Vertex *backReplaceToOriginal(ULInt idTask, MJMesh::Vertex *temporary, const Data::Boundary *boundary);

    public:

#if USE_C__11
		typedef std::unordered_map<ULInt, VertexReplacementHashMap> TaskVertexReplacementHashMap;
		typedef std::unordered_map<ULInt, FrontElementReplacementHashMap> TaskFrontElementReplacementHashMap;

		//typedef std::unordered_map<ULInt, Data::ULIntList> TasksOfHashMap;
#endif //#if USE_C__11

		typedef std::map<ULInt, VertexReplacementMap> TaskVertexReplacementMap;
		typedef std::map<ULInt, FrontElementReplacementMap> TaskFrontElementReplacementMap;

		//typedef std::unordered_map<ULInt, Data::ULIntList> TasksOfMap;

	protected:

		virtual void replace();
		virtual void backReplace();

    protected:

        bool byTask;

        const Data::BSPTree *bsp;
        const Data::BSPTreeCell *parent;

        MJMesh::Boundary *boundary;
        MJMesh::Boundary created;

#if USE_C__11
        //TasksOfHashMap tasksOfVertex;
        //TasksOfHashMap tasksOfFrontElement;

        TaskVertexReplacementHashMap verticesOfTask;
        TaskFrontElementReplacementHashMap frontElementsOfTask;
#else
        //TasksOfMap tasksOfVertex;
        //TasksOfMap tasksOfFrontElement;

        TaskVertexReplacementMap verticesOfTask;
        TaskFrontElementReplacementMap frontElementsOfTask;
#endif //#if USE_C__11

    };
}

#endif //#ifndef _PMJMESH2_CROSSING_REPLACER_H_
