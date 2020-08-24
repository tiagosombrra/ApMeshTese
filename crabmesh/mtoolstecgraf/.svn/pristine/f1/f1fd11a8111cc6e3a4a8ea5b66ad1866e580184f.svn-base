#ifndef _DATA_MESH_H_
#define _DATA_MESH_H_

#include "Data/Definitions.h"
#include "Data/FrontElement.h"
#include "Data/AbstractMesh.h"

namespace Data
{
	class Mesh :
		public Data::AbstractMesh
	{
	public:

		typedef VertexList::iterator VertexIterator;
		typedef VertexList::const_iterator ConstVertexIterator;

		typedef FrontElementList::iterator FrontElementIterator;
		typedef FrontElementList::const_iterator ConstFrontElementIterator;

		typedef ElementList::iterator ElementIterator;
		typedef ElementList::const_iterator ConstElementIterator;

#if USE_EXPERIMENTAL_MESH
		class InnerVertexIterator
		{
		friend
			class Data::Mesh;

		public:
			InnerVertexIterator();
			InnerVertexIterator(Mesh *mesh);
			InnerVertexIterator(Mesh *mesh, const VertexIterator &iter);

			InnerVertexIterator &operator++();
			InnerVertexIterator operator++(int);

			InnerVertexIterator &operator--();
			InnerVertexIterator operator--(int);

			InnerVertexIterator &operator=(const InnerVertexIterator &iter);

			bool operator==(const InnerVertexIterator &iter);
			bool operator!=(const InnerVertexIterator &iter);

			Vertex *operator*() const;

		protected:

			void ensureValidIncrement();
			void ensureValidDecrement();

		protected:

			Mesh *mesh;
			VertexIterator iter;
		};

		class ConstInnerVertexIterator
		{
		friend
			class Data::Mesh;

		public:
			ConstInnerVertexIterator();
			ConstInnerVertexIterator(const Mesh *mesh);
			ConstInnerVertexIterator(const Mesh *mesh, const ConstVertexIterator &iter);

			ConstInnerVertexIterator &operator++();
			ConstInnerVertexIterator operator++(int);

			ConstInnerVertexIterator &operator--();
			ConstInnerVertexIterator operator--(int);

			ConstInnerVertexIterator &operator=(const ConstInnerVertexIterator &iter);

			bool operator==(const ConstInnerVertexIterator &iter);
			bool operator!=(const ConstInnerVertexIterator &iter);

			Vertex *operator*() const;

		protected:

			void ensureValidIncrement();
			void ensureValidDecrement();

		protected:

			const Mesh *mesh;
			ConstVertexIterator iter;
		};
#endif //#if USE_EXPERIMENTAL_MESH

        class Quality
        {
        public:

            Quality();
            ~Quality();

            void setQuality(UInt numSlots, UInt *histogram, Real min, Real max);
            ///numSlots must be a power of 10
            ///colors must be an array of size[3][10]
#if USE_GUI
            void findQuality(Data::Mesh *mesh, UInt numSlots, Real *colors = NULL);
#else
            void findQuality(Data::Mesh *mesh, UInt numSlots);
#endif //#if USE_GUI
            std::string qualityString() const;

            UInt getNumSlots() const;
            const UInt *getHistogram() const;
            Real getMin() const;
            Real getMax() const;

            void clear();

        protected:

            void interval(UInt i, UInt total, std::stringstream &min, std::stringstream &max) const;

            Mesh *mesh;

            UInt numSlots;
            UInt *histogram;
            Real min;
            Real max;
        };

	public:

		Mesh(ULInt id = 0);
		virtual ~Mesh();

        virtual void sortMesh();

		virtual void setVertices(const VertexList &vertices);
		virtual const VertexList &getVertices() const;
		//virtual VertexList getVertices(bool sorted = false) const;
		//virtual VertexList getSortedVertices() const;//just for compilation errors

		virtual void setFrontElements(const FrontElementList &frontElements);
		virtual const FrontElementList &getFrontElements() const;
		//virtual FrontElementList getFrontElements(bool sorted = false) const;
		//virtual FrontElementList getSortedFrontElements() const;//just for compilation errors

		virtual void setElements(const ElementList &elements);
		virtual const ElementList &getElements() const;
		//virtual ElementList getElements(bool sorted = false) const;
		//virtual ElementList getSortedElements() const;//just for compilation errors

#if USE_EXPERIMENTAL_MESH
		//virtual void setInnerVertices(const VertexList &innerVertices) = 0;
		virtual VertexList getInnerVertices() const;
#else
		virtual void setInnerVertices(const VertexList &innerVertices);
		virtual const VertexList &getInnerVertices() const;
#endif //#if USE_EXPERIMENTAL_MESH
		//virtual VertexList getInnerVertices(bool sorted = false) const;
		//virtual VertexList getSortedInnerVertices() const;//just for compilation errors

		virtual UInt dimension() const;

		virtual bool isSurface() const;

		virtual void clear();

		virtual ULInt verticesSize() const;
		virtual ULInt innerVerticesSize() const;
		virtual ULInt frontElementsSize() const;
		virtual ULInt size() const;

		virtual void add(Vertex *v, bool inner = true);
#if USE_EXPERIMENTAL_MESH
		virtual void addInner(Vertex *v) = 0;
#else
		virtual void addInner(Vertex *v);
#endif //#if USE_EXPERIMENTAL_MESH
		virtual void add(FrontElement *e);
		virtual void add(Element *e);

		virtual bool remove(Vertex *v);
#if USE_EXPERIMENTAL_MESH
		virtual void removeInner(Vertex *v) = 0;
#else
		virtual void removeInner(Vertex *v);
#endif //#if USE_EXPERIMENTAL_MESH
		virtual bool remove(FrontElement *e);
		virtual bool remove(Element *e);

		virtual bool have(const Vertex *v) const;
#if USE_EXPERIMENTAL_MESH
		virtual bool haveInner(const Vertex *v) const = 0;
#else
		virtual bool haveInner(const Vertex *v) const;
#endif //#if USE_EXPERIMENTAL_MESH
		virtual bool have(const FrontElement *e) const;
		virtual bool have(const Element *e) const;

		virtual VertexIterator vBegin();
		virtual ConstVertexIterator vBegin() const;
		virtual VertexIterator vEnd();
		virtual ConstVertexIterator vEnd() const;

#if USE_EXPERIMENTAL_MESH
		virtual InnerVertexIterator ivBegin();
		virtual ConstInnerVertexIterator ivBegin() const;
		virtual InnerVertexIterator ivEnd();
		virtual ConstInnerVertexIterator ivEnd() const;
#else
		virtual VertexIterator ivBegin();
		virtual ConstVertexIterator ivBegin() const;
		virtual VertexIterator ivEnd();
		virtual ConstVertexIterator ivEnd() const;
#endif //#if USE_EXPERIMENTAL_MESH

		virtual FrontElementIterator feBegin();
		virtual ConstFrontElementIterator feBegin() const;
		virtual FrontElementIterator feEnd();
		virtual ConstFrontElementIterator feEnd() const;

		virtual ElementIterator eBegin();
		virtual ConstElementIterator eBegin() const;
		virtual ElementIterator eEnd();
		virtual ConstElementIterator eEnd() const;

		virtual VertexIterator erase(const VertexIterator &iter);
#if USE_EXPERIMENTAL_MESH
		virtual InnerVertexIterator eraseInner(const InnerVertexIterator &iter) = 0;
		virtual InnerVertexIterator eraseFromInner(const InnerVertexIterator &iter) = 0;
#else
		virtual VertexIterator eraseInner(const VertexIterator &iter);
		virtual VertexIterator eraseFromInner(const VertexIterator &iter);
#endif //#if USE_EXPERIMENTAL_MESH
		virtual FrontElementIterator erase(const FrontElementIterator &iter);
		virtual ElementIterator erase(const ElementIterator &iter);

		virtual bool in(const Point &p) const;
		virtual bool in(const Point *p) const;

		virtual bool on(const Point &p) const;
		virtual bool on(const Point *p) const;

		virtual bool out(const Point &p) const;
		virtual bool out(const Point *p) const;

		virtual ElementList adjacency(const Vertex *v) const;
		virtual ElementList adjacency(const FrontElement *e) const;

		virtual FrontElementList findAdjacentFrontElements(const Vertex *v) const;
		virtual FrontElementList findAdjacentFrontElements(const Element *e) const;
		virtual FrontElementList findSortedAdjacentFrontElements(const FrontElement *fe, UInt i) const;
		virtual FrontElementList findSortedAdjacentFrontElements(const FrontElement *fe, UInt i, const Boundary *boundary) const;
		virtual FrontElementList findSortedAdjacentFrontElements(const FrontElement *fe, UInt i, const Front *front) const;
		static FrontElementList findSortedAdjacentFrontElements(const FrontElement *fe, const Vertex *v, FrontElementList adjs);

		//all vertices in the given VertexList vertices should be contained in
		// this mesh
#if USE_C__11
		virtual VertexHash findLayer(const VertexList &vertices, UInt layer = 0) const;
		virtual void findLayer(const VertexList &vertices, UInt layer,
			VertexHash &layerVertices) const;
		virtual void findLayer(const VertexList &vertices, UInt layer,
			VertexHash &layerVertices, FrontElementHash &layerFrontElements) const;
		virtual void findLayer(const VertexList &vertices, UInt layer,
			VertexHash &layerVertices, ElementHash &layerElements) const;
		virtual void findLayer(const VertexList &vertices, UInt layer,
			VertexHash &layerVertices, FrontElementHash &layerFrontElements,
			ElementHash &layerElements) const;

		virtual VertexHash findAndRemoveLayer(const VertexList &vertices, UInt layer = 0);
		virtual void findAndRemoveLayer(const VertexList &vertices, UInt layer,
			VertexHash &layerVertices);
		virtual void findAndRemoveLayer(const VertexList &vertices, UInt layer,
			VertexHash &layerVertices, FrontElementHash &layerFrontElements);
		virtual void findAndRemoveLayer(const VertexList &vertices, UInt layer,
			VertexHash &layerVertices, ElementHash &layerElements);
		virtual void findAndRemoveLayer(const VertexList &vertices, UInt layer,
			VertexHash &layerVertices, FrontElementHash &layerFrontElements,
			ElementHash &layerElements);

		virtual VertexHash findLayers(const VertexList &vertices, UInt layers = 0) const;
		virtual void findLayers(const VertexList &vertices, UInt layers,
			VertexHash &layerVertices) const;
		virtual void findLayers(const VertexList &vertices, UInt layers,
			VertexHash &layerVertices, FrontElementHash &layerFrontElements) const;
		virtual void findLayers(const VertexList &vertices, UInt layers,
			VertexHash &layerVertices, ElementHash &layerElements) const;
		virtual void findLayers(const VertexList &vertices, UInt layers,
			VertexHash &layerVertices, FrontElementHash &layerFrontElements,
			ElementHash &layerElements) const;

		virtual VertexHash findAndRemoveLayers(const VertexList &vertices, UInt layers = 0);
		virtual void findAndRemoveLayers(const VertexList &vertices, UInt layers,
			VertexHash &layerVertices);
		virtual void findAndRemoveLayers(const VertexList &vertices, UInt layers,
			VertexHash &layerVertices, FrontElementHash &layerFrontElements);
		virtual void findAndRemoveLayers(const VertexList &vertices, UInt layers,
			VertexHash &layerVertices, ElementHash &layerElements);
		virtual void findAndRemoveLayers(const VertexList &vertices, UInt layers,
			VertexHash &layerVertices, FrontElementHash &layerFrontElements,
			ElementHash &layerElements);
#else
		virtual VertexSet findLayer(const VertexList &vertices, UInt layer = 0) const;
		virtual void findLayer(const VertexList &vertices, UInt layer,
			VertexSet &layerVertices) const;
		virtual void findLayer(const VertexList &vertices, UInt layer,
			VertexSet &layerVertices, FrontElementSet &layerFrontElements) const;
		virtual void findLayer(const VertexList &vertices, UInt layer,
			VertexSet &layerVertices, ElementSet &layerElements) const;
		virtual void findLayer(const VertexList &vertices, UInt layer,
			VertexSet &layerVertices, FrontElementSet &layerFrontElements,
			ElementSet &layerElements) const;

		virtual VertexSet findAndRemoveLayer(const VertexList &vertices, UInt layer = 0);
		virtual void findAndRemoveLayer(const VertexList &vertices, UInt layer,
			VertexSet &layerVertices);
		virtual void findAndRemoveLayer(const VertexList &vertices, UInt layer,
			VertexSet &layerVertices, FrontElementSet &layerFrontElements);
		virtual void findAndRemoveLayer(const VertexList &vertices, UInt layer,
			VertexSet &layerVertices, ElementSet &layerElements);
		virtual void findAndRemoveLayer(const VertexList &vertices, UInt layer,
			VertexSet &layerVertices, FrontElementSet &layerFrontElements,
			ElementSet &layerElements);

		virtual VertexSet findLayers(const VertexList &vertices, UInt layers = 0) const;
		virtual void findLayers(const VertexList &vertices, UInt layers,
			VertexSet &layerVertices) const;
		virtual void findLayers(const VertexList &vertices, UInt layers,
			VertexSet &layerVertices, FrontElementSet &layerFrontElements) const;
		virtual void findLayers(const VertexList &vertices, UInt layers,
			VertexSet &layerVertices, ElementSet &layerElements) const;
		virtual void findLayers(const VertexList &vertices, UInt layers,
			VertexSet &layerVertices, FrontElementSet &layerFrontElements,
			ElementSet &layerElements) const;

		virtual VertexSet findAndRemoveLayers(const VertexList &vertices, UInt layers = 0);
		virtual void findAndRemoveLayers(const VertexList &vertices, UInt layers,
			VertexSet &layerVertices);
		virtual void findAndRemoveLayers(const VertexList &vertices, UInt layers,
			VertexSet &layerVertices, FrontElementSet &layerFrontElements);
		virtual void findAndRemoveLayers(const VertexList &vertices, UInt layers,
			VertexSet &layerVertices, ElementSet &layerElements);
		virtual void findAndRemoveLayers(const VertexList &vertices, UInt layers,
			VertexSet &layerVertices, FrontElementSet &layerFrontElements,
			ElementSet &layerElements);
#endif //#if USE_C__11

#if USE_GUI
		using Colorable::draw;
		virtual void draw() const;
		virtual void drawPick() const;
		virtual void drawNormals(Real factor) const;
#endif //#if USE_GUI
		virtual std::string text() const;
//#endif //#if USE_GUI

	protected:

		virtual FrontElementList findSortedAdjacentFrontElements(const FrontElement *fe, UInt i, const Boundary *boundary, const Front *front) const;
		static FrontElementList findSortedAdjacentFrontElements(const FrontElement *fe, const Vertex *v, UInt i,
            const Boundary *boundary, const Front *front, FrontElementList adjs);

		//find the layer of vertices, front elements and elements adjacent
		//  to input list of vertices
#if USE_C__11
		virtual void findLayer(const VertexList &vertices, UInt layer,
			VertexHash &layerVertices,
			bool frontElements, FrontElementHash &layerFrontElements,
			bool elements, ElementHash &layerElements) const;

		virtual void findAndRemoveLayer(const VertexList &vertices, UInt layer,
			VertexHash &layerVertices,
			bool frontElements, FrontElementHash &layerFrontElements,
			bool elements, ElementHash &layerElements);
#else
		virtual void findLayer(const VertexList &vertices, UInt layer,
			VertexSet &layerVertices,
			bool frontElements, FrontElementSet &layerFrontElements,
			bool elements, ElementSet &layerElements) const;

		virtual void findAndRemoveLayer(const VertexList &vertices, UInt layer,
			VertexSet &layerVertices,
			bool frontElements, FrontElementSet &layerFrontElements,
			bool elements, ElementSet &layerElements);
#endif //#if USE_C__11

		//find all layers of vertices, front elements and elements adjacent
		//  to input list of vertices
#if USE_C__11
		virtual void findLayers(const VertexList &vertices, UInt layers,
			VertexHash &layerVertices,
			bool frontElements, FrontElementHash &layerFrontElements,
			bool elements, ElementHash &layerElements) const;

		virtual void findAndRemoveLayers(const VertexList &vertices, UInt layers,
			VertexHash &layerVertices,
			bool frontElements, FrontElementHash &layerFrontElements,
			bool elements, ElementHash &layerElements);
#else
		virtual void findLayers(const VertexList &vertices, UInt layers,
			VertexSet &layerVertices,
			bool frontElements, FrontElementSet &layerFrontElements,
			bool elements, ElementSet &layerElements) const;

		virtual void findAndRemoveLayers(const VertexList &vertices, UInt layers,
			VertexSet &layerVertices,
			bool frontElements, FrontElementSet &layerFrontElements,
			bool elements, ElementSet &layerElements);
#endif //#if USE_C__11

	protected:

		VertexList vertices;
		FrontElementList frontElements;
		ElementList elements;

#if USE_EXPERIMENTAL_MESH
        ULInt numInnerVertices;
#else
		VertexList innerVertices;
#endif //#if USE_EXPERIMENTAL_MESH
	};
}

#endif //#ifndef _DATA_MESH_H_
