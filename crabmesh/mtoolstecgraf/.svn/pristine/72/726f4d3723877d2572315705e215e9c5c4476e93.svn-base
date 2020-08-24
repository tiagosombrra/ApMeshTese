#ifndef _PERFORMER_MESH_TOPOLOGY_CHECKER_H_
#define _PERFORMER_MESH_TOPOLOGY_CHECKER_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"
#include "Data/Mesh.h"

namespace Performer
{
	class MeshTopologyChecker : public Performer::Builder
	{
	public:

		MeshTopologyChecker();
		virtual ~MeshTopologyChecker();

		virtual void setMesh(const Data::Mesh *mesh);
		virtual void setBoundary(const Data::Boundary *boundary);
		virtual void setFront(const Data::Front *front);
		virtual void setStopOnFirst(bool stopOnFirst);

		virtual bool isConsistent() const;

		virtual void execute();

	protected:

	    virtual bool isTestingAngles() const;
	    virtual bool isTestingAngles(const Data::Vertex *test) const;

		//virtual bool check(const Data::Mesh::ConstVertexIterator &iter, const Data::Mesh::ConstVertexIterator &end) const;
		virtual bool check(const Data::Vertex *test) const;
		//virtual bool check(const Data::Mesh::ConstFrontElementIterator &iter, const Data::Mesh::ConstFrontElementIterator &end) const;
		virtual bool check(const Data::FrontElement *test) const;
		//virtual bool check(const Data::Mesh::ConstElementIterator &iter, const Data::Mesh::ConstElementIterator &end) const;
		virtual bool check(const Data::Element *test) const;

		virtual bool check(const Data::FrontElement *test, UInt i) const;
		virtual bool check(const Data::Element *test, UInt i) const;

    protected:

#if USE_C__11
        typedef std::unordered_multimap<ULInt, Data::Vertex *> IdVMap;
        typedef std::unordered_multimap<ULInt, Data::FrontElement *> IdFEMap;
        typedef std::unordered_multimap<ULInt, Data::Element *> IdEMap;

        typedef std::unordered_multimap<Real, Data::Vertex *> RVMap;
        typedef std::unordered_multimap<Real, Data::FrontElement *> RFEMap;
        typedef std::unordered_multimap<Real, Data::Element *> REMap;
#else
        typedef std::multimap<ULInt, Data::Vertex *> IdVMap;
        typedef std::multimap<ULInt, Data::FrontElement *> IdFEMap;
        typedef std::multimap<ULInt, Data::Element *> IdEMap;

        typedef std::multimap<Real, Data::Vertex *> RVMap;
        typedef std::multimap<Real, Data::FrontElement *> RFEMap;
        typedef std::multimap<Real, Data::Element *> REMap;
#endif //#if USE_C__11

	protected:

		const Data::Mesh *mesh;
		const Data::Boundary *boundary;
		const Data::Front *front;

		bool stopOnFirst;

		mutable bool consistent;

		UInt dimension;
		bool testElements;
		bool internalFrontElements;

		IdVMap idvMap;
		IdFEMap idfeMap;
		IdEMap ideMap;

		RVMap vMap;
		RFEMap feMap;
		REMap eMap;
	};
}

#endif //#ifndef _PERFORMER_MESH_TOPOLOGY_CHECKER_H_
