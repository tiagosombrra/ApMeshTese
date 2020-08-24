#ifndef _MJMESH_BACK_TRACKER_H_
#define _MJMESH_BACK_TRACKER_H_

#include "MJMesh/Definitions.h"
#include "Performer/MeshBackTracker.h"

namespace MJMesh
{
	class MeshBackTracker : public Performer::MeshBackTracker
	{
	friend
		class MJMesh::BackTrackingFrontAdvancer;

	public:

		MeshBackTracker(UInt dimension, Data::Mesh *mesh,
			Data::Boundary *boundary, Data::BoxTree *tree, Data::Front *front,
			bool meshSorted, UInt maxBackTracks);
		virtual ~MeshBackTracker();

		virtual void setDimension(UInt dimension);

		virtual void setMaxBackTracks(UInt maxBackTracks);

		virtual void setTree(Data::BoxTree *tree);

		virtual void execute();

	protected:

		virtual MJMesh::BackTrackingFrontAdvancer *makeBackTrackingFrontAdvancer() const;
		virtual void clearBtfa();

		//given a front element that is adjacent to the given element, this
		// method inverts the front element if its normal is not pointing
		// towards the interior of the element
		static void invert(Data::FrontElement *fel, Data::Element *e);

		//these methods mark the given [vertex|front element|element] as
		// old, inserting it in the given set and removing it from the
		// given mesh
		static void markAsOld(Data::VertexSet &old, Data::Vertex *v, Data::Mesh *mesh);
		static void markAsOld(Data::FrontElementSet &old, Data::FrontElement *e, Data::Mesh *mesh);
		static void markAsOld(Data::ElementSet &old, Data::Element *e, Data::Mesh *mesh);

		//this method attempts to invert the element, if it does not belong to
		// the boundary, inserting its front elements in the given set. false
		// is returned if the given element belongs to the boundary, in which
		// case its front elements will not be inverted. otherwise, it returns
		// true
		// UPDATED 2011/02/04. if ignoreBoundary is true, the method will
		//  proceed as if the element does not belong to the boundary
/*#if USE_DELETE_FELS
		static bool process(Data::FrontElementList &created,
            Data::Element *e,
			const Data::Mesh *mesh, const Data::Boundary *boundary,
			Data::FrontElementSet &newFront, bool ignoreBoundary = false);
#else*/
		static bool process(Data::Element *e,
			const Data::Mesh *mesh, const Data::Boundary *boundary,
			Data::FrontElementSet &newFront, bool ignoreBoundary = false);
/*#endif //#if USE_DELETE_FELS*/

		//this method inverts the given front elements adjacent to the given
		// element. connectors are the front elements that connect the cavity
		// to the element. the front elements inverted will be inserted in the
		// given newFront set and the front elements that would be deleted will
		// be inserted in the given oldFrontElements set. OBS.: this method
		// tests if a front element is already in the newFront set, in which
		// case, it will be inserted in oldFrontElements
		static void process(Data::Element *e, Data::FrontElementList &fels,
			Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
			const Data::FrontElementList &connectors, Data::Mesh *mesh);

		//this method does the same as the above, except that it also marks the
		// given element as old, inserting it in the given oldElements
		// set
/*#if USE_DELETE_FELS
		static void process(Data::FrontElementList &created,
            Data::Element *e, Data::FrontElementList &fels,
			Data::ElementSet &oldElements,
			Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
			const Data::FrontElementList &connectors, Data::Mesh *mesh);
#else*/
		static void process(Data::Element *e, Data::FrontElementList &fels,
			Data::ElementSet &oldElements,
			Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
			const Data::FrontElementList &connectors, Data::Mesh *mesh);
/*#endif //#if USE_DELETE_FELS*/

		//this method finds the front elements adjacent to the given element and
		// tests if the element belongs to the boundary, in which case, it
		// returns false. otherwise, it inverts the front elements adjacent to
		// the given element and returns true. connectors are the front elements
		// that connect the cavity to the element.
		// UPDATED 2011/02/04. if ignoreBoundary is true, the method will
		//  proceed as if the element does not belong to the boundary
/*#if USE_DELETE_FELS
		static bool process(Data::FrontElementList &created,
            Data::Element *e,
			Data::Mesh *mesh, const Data::Boundary *boundary,
			Data::ElementSet &oldElements,
			Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
			Data::FrontElementList &connectors, bool ignoreBoundary = false);
#else*/
		static bool process(Data::Element *e,
			Data::Mesh *mesh, const Data::Boundary *boundary,
			Data::ElementSet &oldElements,
			Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
			Data::FrontElementList &connectors, bool ignoreBoundary = false);
/*#endif //#if USE_DELETE_FELS*/

		//same as above, but with only one connector.
		// UPDATED 2011/02/04. if ignoreBoundary is true, the method will
		//  proceed as if the element does not belong to the boundary
/*#if USE_DELETE_FELS
		static bool process(Data::FrontElementList &created,
            Data::Element *e,
			Data::Mesh *mesh, const Data::Boundary *boundary,
			Data::ElementSet &oldElements,
			Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
			Data::FrontElement *connector, bool ignoreBoundary = false);
#else*/
		static bool process(Data::Element *e,
			Data::Mesh *mesh, const Data::Boundary *boundary,
			Data::ElementSet &oldElements,
			Data::FrontElementSet &newFront, Data::FrontElementSet &oldFrontElements,
			Data::FrontElement *connector, bool ignoreBoundary = false);
/*#endif //#if USE_DELETE_FELS*/

		//this method creates a cavity and a new front. given an element, it
		// finds its adjacent elements and delete them from the mesh, along with
		// the front elements that belong to two of these elements and their
		// vertices that are not adjacent to any element. NULL will be returned
		// if the given element belongs to the boundary or all of its adjacent
		// elements belong to the boundary
		virtual Data::Front *makeFront(Data::Element *e, Data::ElementSet &oldElements,
			Data::FrontElementSet &oldFrontElements, Data::VertexSet &oldVertices) const;

		//this method inserts the given element in the given list, making sure
		// that the given list is ordered some way (according to the quality
		// of the element)
		virtual void insert(Data::ElementList &elements, Data::Element *e) const;

		virtual Data::ElementList sort() const;

		virtual bool worse(Real rank1, Real rank2) const;

		virtual bool betterMesh(const Data::ElementSet &newMesh, const Data::ElementSet &oldMesh) const;

	protected:

		UInt dimension;

		UInt maxBackTracks;

		Data::BoxTree *tree;

		Data::ElementList elements;

		MJMesh::BackTrackingFrontAdvancer *btfa;

#if USE_DELETE_FELS
        mutable Data::FrontElementList *created;
#endif //#if USE_DELETE_FELS

	};
}

#endif //#ifndef _MJMESH_BACK_TRACKER_H_
