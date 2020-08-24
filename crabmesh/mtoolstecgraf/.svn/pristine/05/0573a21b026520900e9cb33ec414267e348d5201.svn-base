#ifndef _MJMESH_BACK_TRACKING_FRONT_ADVANCER_H_
#define _MJMESH_BACK_TRACKING_FRONT_ADVANCER_H_

#include "MJMesh/Definitions.h"
#include "MJMesh/FrontAdvancer.h"

namespace MJMesh
{
	class BackTrackingFrontAdvancer : public MJMesh::FrontAdvancer
	{
    public:

        enum Mode
        {
            FRONT_ADVANCING,
            OPTIMIZATION
        };

	public:

		BackTrackingFrontAdvancer(UInt dimension,
			const MJMesh::Boundary *boundary, const Data::BoxTree *tree,
			MJMesh::Mesh *mesh, MJMesh::Front *front,
			//bool adjustFront = true, bool deleteElements = true,
			bool adjustFront, bool deleteElements,
			//UInt maxBackTracks = 0);
			UInt maxBackTracks, enum Mode mode);
		virtual ~BackTrackingFrontAdvancer();

		virtual void setAdjustFront(bool adjustFront);

		virtual void setDeleteElements(bool deleteElements);

		virtual void setMaxBackTracks(UInt maxBackTracks);

		virtual void setMode(enum Mode mode);

		virtual void setOldElements(const Data::ElementSet &oldElements);
		virtual const Data::ElementSet &getOldElements() const;

		virtual const Data::ElementSet &getNewElements() const;

		virtual void setOldFrontElements(const Data::FrontElementSet &oldFrontElements);
		virtual const Data::FrontElementSet &getOldFrontElements() const;

		virtual const Data::FrontElementSet &getNewFrontElements() const;

		virtual void setOldVertices(const Data::VertexSet &oldVertices);
		virtual const Data::VertexSet &getOldVertices() const;

		virtual const Data::VertexSet &getNewVertices() const;

#if USE_DELETE_FELS
        virtual void setCreated(Data::FrontElementList *created) const;
#endif //#if USE_DELETE_FELS

		virtual void deleteOldMesh();
		virtual void restoreOldMesh();

		//when the status, at the end, is FrontAdvancer::ended, then the back-
		// tracking was successful. if the status is FrontAdvancer::rejected,
		// then the back-tracking was not able to finish successful (either
		// because the boundary was reached or because the maximum number
		// of back tracks was reached), in which case the previous mesh should
		// be restored
		virtual void execute();

	public:

		enum
		{
			NOT_INIT_BACK_TRACKING = MJMesh::FrontAdvancer::REJECTED + 1,
			INIT_BACK_TRACKING,
			ADJ_ELEMENTS_FOUND,
			MID_POINT_FOUND,
			VISIBILITY_CHECKED,
			PROCESS_ELEMENT_FOUND
		};

		static const Performer::Status initBackTracking;
		static const Performer::Status notInitBackTracking;
		static const Performer::Status adjElementsFound;
		static const Performer::Status midPointFound;
		static const Performer::Status visibilityChecked;
		static const Performer::Status processElementFound;

	protected:

		virtual Data::FrontList adjustFront() const;

		//virtual Data::ElementList findAdjacentElements() const;
		virtual Data::Element *adjacency(Data::FrontElement *fel) const;

		//find the elements in the mesh adjacent to the front elements in the front
		// (possibly repeated)
		virtual void findFrontElements();

		//this method returns true if an element has all its adjacent front
		// elements in the current front. in this case, this element has to be
		// back-tracked. it returns false otherwise. adjacentElement is a set
		// with all the elements. max is the maximum number front elemens an
		// element can have in the front
		virtual bool hasInnerElement();

		virtual Data::Point *makeMid() const;
		virtual void makeMids();

		virtual void makeEdges();
		virtual void clearEdges();

		//this method returns the number of hits for the given element e with
		// the given edges
		virtual UInt checkVisibility(const Data::Element *e,
			const Data::EdgeList &edges) const;
		//this method returns the number of hits for the given front element e
		// with the given edges
		virtual UInt checkVisibility(const Data::FrontElement *fel,
			const Data::EdgeList &edges) const;

		/*//this method returns the maximum number of hits for the given elements
		// with the given edges. mapHitsElements maps each number of hits with
		// the list of elements that hits that number of times
		virtual UInt checkVisibility(const Data::ElementSet &elements,
			const Data::EdgeList &edges, IntElsMap &mapHitsElements) const;*/
		//this method returns the maximum number of hits for the given front
		// elements with the given edges. mapHitsFels maps each number of
		// hits with the list of front elements that hits that number of times
		virtual UInt checkVisibility(const Data::FrontElementList &fels,
			const Data::EdgeList &edges, IntFElsMap &mapHitsFels);
		/*//this method returns the maximum number of hits for the given front
		// elements with the given edges. mapHitsElelements maps each number of
		// hits with the list of elements that hits that number of times
		virtual UInt checkVisibility(const Data::FrontElementList &fels,
			const Data::EdgeList &edges, IntElsMap &mapHitsElements) const;*/

		virtual UInt updateHits(UInt add, Data::FrontElement *fel, IntFElsMap &mapHitsFels) const;

		//this method checks if any of the newly created triangles intercept
		// the current front. if an interception occurs, mapHitsFels and
		// maxHits are updated, so that the front element that most contributes
		// for the non-existence of a star-shaped polyhedron in the cavity will
		// be back-tracked. currently, this method is specific for 3 dimensions
		virtual UInt checkInterception();

		//this method creates the new mesh using the this->frontElements and
		// the given point
		virtual void makeNewMesh();

		/*//this method returns the element to be back tracked based on the number
		// of hits for each element. this method also returns the existing front
		// elements that are adacent to the element
		virtual Data::Element *findElementToProcess(IntElsMap &mapHitsElements,
			UInt maxHits, Data::FrontElementList &fels, bool ignoreBoundary) const;*/
		/*//this method returns the element to be back tracked based on the number
		// of hits for each front element. this method also returns the
		// existing front elements that are adjacent to the element to be
		// back tracked
		virtual Data::Element *findElementToProcess(IntFElsMap &mapHitsFels,
			UInt maxHits, Data::FrontElementList &fels, bool ignoreBoundary) const;*/

        //this method returns the element to be back tracked based on the number
		// of hits for each front element. this method also returns the
		// existing front elements that are adjacent to the element to be
		// back tracked
		virtual Data::Element *tryToProcess(Data::FrontElement *fel,
            Data::FrontElementList &fels, bool ignoreBoundary) const;

        //this method returns the element to be back tracked based on the number
		// of hits for each front element. this method also returns the
		// existing front elements that are adjacent to the element to be
		// back tracked
		virtual Data::Element *findElementToProcess(IntFElsMap &mapHitsFels,
			UInt maxHits, Data::FrontElementList &fels, bool &ignoreBoundary) const;

		virtual void process(Data::Element *e, Data::FrontElementList &fels,
			Data::FrontElementList &front, bool ignoreBoundary);

		//this method merges two fronts that are connected by the input front
		// element. one of the fronts is the this->front attribute, the other
		// one has to be found in the this->fronts list.
		virtual bool mergeFronts(Data::FrontElement *fel, Data::FrontElementSet &newFront);

		virtual bool backTrackOnce();
		virtual void backTrack();

		//virtual Data::PointList ideal(const Data::FrontElement *element, Real factor = 1.0) const;

		virtual Real radius(const Data::FrontElement *element) const;

		using MJMesh::FrontAdvancer::best;
		virtual Data::Vertex *best(const Data::FrontElement *element) const;

	protected:

		typedef std::map<Data::Point *, Data::EdgeList> PointEdgeListMap;

#if USE_DELETE_FELS
        typedef std::list<Data::FrontElementList *> FrontElementListList;
#endif //#if USE_DELETE_FELS

	protected:

		Data::ElementSet oldElements;
		Data::ElementSet newElements;

		Data::FrontElementSet oldFrontElements;
		Data::FrontElementSet newFrontElements;

		Data::VertexSet oldVertices;
		Data::VertexSet newVertices;

		Data::FrontElementList frontElements;

		Data::FrontList fronts;

		bool shouldAdjustFront;
		bool deleteElements;

		UInt maxBackTracks;
		enum Mode mode;

#if USE_DELETE_FELS
        mutable FrontElementListList created;
#endif //#if USE_DELETE_FELS

	private:

		//bool debug;

		UInt step;

		Data::ElementList elements;
		Data::FrontElementList fels;
		//Data::EdgeList edges;
		PointEdgeListMap edges;

		Data::ElementSet adjacentElements;
		Data::VertexList frontVertices;

		ElFElsMap mapElementFront;
		//IntElsMap mapHitsElements;
		IntFElsMap mapHitsFels;

		Data::PointList mids;
		Data::Point *mid;

		Data::Element *chosen;

		UInt maxHits;

		bool ignoreBoundary;

#if USE_GUI
		Real r, g, b;
#endif //#if USE_GUI
	};
}

#endif //#ifndef _MJMESH_BACK_TRACKING_FRONT_ADVANCER_H_
